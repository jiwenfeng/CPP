--副本


local NORMAL_COPY_COST_VIGOUR = 5	--进入副本所需精力
local FAMOUS_COPY_COST_VIGOUR = 10	--英魂副本所需精力
local MOPUP_MIN_LV = 20			--扫荡开放等级
local MOPUP_COST_GOLD = 1000	--扫荡所需钱币
local MOPUP_TIMELONG = 30
local MOPUP_NEED_GRID = 4
local FAST_MOPUP_NEED_INGOT = 1

local HERO_COPY_BUY_TIMES = 5		--英魂副本可购买次数
local HERO_COPY_BUY_PRICE = 5		--英魂副本购买价格基数
local STATE_DOING = 1
local STATE_FINISH = 0

--离线计算副本物品奖励时调用
function GetCopyPrize(copyId, num)
	if num <= 0 then return end
	local info = BASECOPY.GetCopyInfoById(copyId)
	if not info then return end
	local prize = BASEITEM.GenRandomItemByNum(info.itemList, num)
	return prize
end

function IsCopyOpen(user, id)
	local info = BASECOPY.GetCopyInfoById(id)
	if user:GetLv() < info.needLv then
		return false
	end
	if info.needMissionId and not user:GetMissionById(info.needMissionId) and not user:IsMissionOver(info.needMissionId) then
		return false
	end
	return true
end

--格式化的副本列表
function GetFormatedCopyList(user, townId, type)
	local list = SCENE.GetSceneInfoById("town", townId)
	local passedList = user:GetCopyPassed(type)
	local sData = {}
	for i = list.minCopyId, list.maxCopyId do
		local id = tostring(i)
		local info = BASECOPY.GetCopyInfoById(id)
		if info then
			local state = BASECOPY.COPY_PASSED
			if not passedList[id] then
				if IsCopyOpen(user, id) then
					state = BASECOPY.COPY_OPEN
				else
					state = BASECOPY.COPY_CLOSE
				end
			end
			if state ~= BASECOPY.COPY_CLOSE then
				local buff = {}
				buff.state = state
				buff.copyId = id
				buff.name = info.name
				table.insert(sData, buff)
			end
		end
	end
	return sData
end

function FormatCopyListById(id)
	local sData = {}
	for k, v in pairs() do
	end
end

function GetCopyDetail(id)
	local info = BASECOPY.GetCopyInfoById(id)
	if not info then return end
	local buff = {}
	buff.copyId = id
--	buff.name = info.name
	local type = BASECOPY.GetCopyTypeById(id)
	if type == "team" then
		buff.needLv = info.needLv
	else
		buff.mapId = info.mapId
	end
	buff.describe = info.describe
	return buff
end

function NewCopy(id, owner)
	local copy = CLSCOPY.clsCopy:New()
	copy:InitCopy(id, owner)
	return copy
end

function CopyMopUp(user, copyId, times)
	local buff = {}
	buff.copyId = copyId
	buff.max = times
	buff.num = 1
	buff.state = STATE_DOING
	buff.timestamp = os.time() + MOPUP_TIMELONG
	user:SetCurMopup(buff)
	user:StartCopyMopupCallOut()
	local info = BASECOPY.GetCopyInfoById(copyId)
	if info.type == "hero" then
		user:SetDailyHeroCopyTimes("num", 1)
		user:SendProtocol("c_heroCopy_buy", GetHeroCopyTimesInfo(user))
		user:SetVigour(-FAMOUS_COPY_COST_VIGOUR)
	else
		user:SetVigour(-NORMAL_COPY_COST_VIGOUR)
	end
	user:SetGold(-MOPUP_COST_GOLD)
	
	local scene = SCENE.GetEmptyScene("copyMopup")
	user:EnterScene(scene, 0, 0)
end

function ProMopup(user, type)
	local mopup = user:GetCurMopup()
	local info = BASECOPY.GetCopyInfoById(mopup.copyId)
	user:RemoveCopyMopupCallOut()
	local done = {times = mopup.num}
	user:SetFuncStatById(SETTING.FUNC_NORMAL_COPY, 1)
	for k, v in pairs(info.monsterList) do
		if v.exp then
			done.exp = (done.exp or 0) + v.exp
		end
		if v.nimbus then
			done.nimbus = (done.nimbus or 0) + v.nimbus
		end
		done.itemList = done.itemList or {}
		for k, v in pairs(v.itemList or {}) do
			if v.needMissionId then
				if user:IsMissionAdd(v.needMissionId) then
					BASEITEM.CombiItemToList(done.itemList, v.id, math.random(v.min, v.max), v.needMissionId)
				end
			else
				BASEITEM.CombiItemToList(done.itemList, v.id, math.random(v.min, v.max))
			end
		end
		local monsterList = FIGHT.GetMonsterTeamInfoById(v.teamId)
		for k, v in pairs(monsterList.list) do
			MISSION.CheckMissionEvent(user, "fight", v.id, 1)
		end
	end
	for i = 1, user:GetVipLv() > 0 and 2 or 1 do
		local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
		BASEITEM.CombiItemToList(done.itemList, prize[1].id, prize[1].num)
	end
	if done.exp then
		user:SetTeamExp(done.exp, "copy")
	end
	if done.nimbus then
		user:SetNimbus(done.nimbus)
	end
	BASECOPY.AddCopyItem(user, done.itemList, info.name)
	user:SetMopupGot(done.exp, done.nimbus, done.itemList)
	if mopup.num >= mopup.max then
		mopup.state = STATE_FINISH
		if type ~= "login" then
			SendMopupUpdate(user, done)
			SendMopupFinish(user)
		end
--		StopMopup(user)
	else
		if info.type == "hero" then
			user:SetDailyHeroCopyTimes("num", 1)
			user:SetVigour(-FAMOUS_COPY_COST_VIGOUR)
		else
			user:SetVigour(-NORMAL_COPY_COST_VIGOUR)
		end
		user:SetGold(-MOPUP_COST_GOLD)
		mopup.num = mopup.num + 1
		mopup.timestamp = mopup.timestamp + MOPUP_TIMELONG
		if not type then
			user:StartCopyMopupCallOut()
		end
		if type ~= "login" then
			SendMopupUpdate(user, done)
		end
	end
	STATS.SaveCopyHistory(mopup.copyId, user:GetRoleId(), 1, 1)
end

function SendMopupUpdate(user, done)
	local mopup = user:GetCurMopup()
	local sData = {}
	sData.state = mopup.state
	sData.done = done
	sData.curTimes = mopup.num
	sData.timelong = mopup.timestamp - os.time()
	local con = LOGIN.GetConnection(user:GetRoleId())
	if con and con.roleId == user:GetRoleId() then
		con:SendProtocol("c_copy_mopupUpdate", sData)
		local info = BASECOPY.GetCopyInfoById(mopup.copyId)
		if info.type == "hero" then
			con:SendProtocol("c_heroCopy_buy", GetHeroCopyTimesInfo(user))
		end
	end
end

function SendMopupFinish(user)
	local got = user:GetMopupGot()
	local con = LOGIN.GetConnection(user:GetRoleId())
	if con and con.roleId == user:GetRoleId() then
		local sData = {}
		sData.expGot = got.exp
		sData.nimbus = got.nimbus
		sData.itemGot = got.itemList
		local mopup = user:GetCurMopup()
		sData.state = mopup.state
		con:SendProtocol("c_copy_mopupFinish", sData)
		local info = BASECOPY.GetCopyInfoById(mopup.copyId)
		if info.type == "hero" then
			con:SendProtocol("c_heroCopy_buy", GetHeroCopyTimesInfo(user))
		end
	end
end

function FastMopup(user)
	user:RemoveCopyMopupCallOut()
	local mopup = user:GetCurMopup()
	if not mopup then return end
	for i = mopup.num, mopup.max do
		ProMopup(user, "fast")
	end
	mopup.timestamp = os.time()
end

function StopMopup(user)
	user:RemoveCopyMopupCallOut()
	user:SetCurMopup()
	
	user:LeaveScene()
	user:SetLocateInfo()
	
end

function GetHeroCopyTimesInfo(user)
	local buff = {}
	buff.leftTimes = user:GetDailyHeroCopyTimes("max") - user:GetDailyHeroCopyTimes("num")
	buff.buyTimes = HERO_COPY_BUY_TIMES + USERCOPY.DAILY_HERO_COPY_NUM - user:GetDailyHeroCopyTimes("max")
	buff.buyPrice = (user:GetDailyHeroCopyTimes("max") - USERCOPY.DAILY_HERO_COPY_NUM + 1) * HERO_COPY_BUY_PRICE
	return buff
end

function QuitCopy(user)
	local copy = user:GetCurCopy()
	if not copy then
		return false, "您当前不在副本中中。"
	end
	user:LeaveCopy(copy)
	return SCENE.GetReturnedTownId(user)
end




local s_copy_list = function(connection, data)
--	if data ~= "normal" and data ~= "team" and data ~= "hero" then
	if data ~= "normal" and data ~= "hero" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {type = data}
	if data == "team" then
		sData.num = user:GetDailyTeamCopyNum()
		sData.max = user:GetDailyTeamCopyMax()
		sData.list = GetFormatTeamCopyList(user:GetLv())
	else
		local lastLocate = user:GetLastLocateInfo()
		local scene = SCENE.GetTownScene(lastLocate.scene)
		if not scene then
			return ERROR.TipMsg(connection, "无效的场景")
		end
		local townInfo = SCENE.GetSceneInfoById("town", lastLocate.scene)
		sData.selectMap = townInfo.selectMap
		sData.mopupLv = MOPUP_MIN_LV
		sData.mopupNeed = MOPUP_COST_GOLD
		sData.list = GetFormatedCopyList(user, scene:GetTemplate(), data)		
		if data == "hero" then
			local timesInfo = GetHeroCopyTimesInfo(user)
			sData.leftTimes = timesInfo.leftTimes
			sData.buyTimes = timesInfo.buyTimes
			sData.buyPrice = timesInfo.buyPrice
		end
	end
	connection:SendProtocol("c_copy_list", sData)
end


local s_copy_info = function(connection, data)
	data = tostring(data)
	if not BASECOPY.GetCopyInfoById(data) then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetCopyDetail(data)
	connection:SendProtocol("c_copy_info", sData)
end

local s_copy_enter = function(connection, data)
	local info = BASECOPY.GetCopyInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not IsCopyOpen(user, data) then
		return ERROR.TipMsg(connection, "该副本尚未开启。")
	end
	if user:GetCurCopy() then
		return ERROR.TipMsg(connection, "您已经位于一个副本中了。")
	end
	if info.type == "hero" then
		if user:GetDailyHeroCopyTimes("num") >= user:GetDailyHeroCopyTimes("max") then
			return ERROR.TipMsg(connection, "已无剩余次数。")
		elseif user:GetVigour() < FAMOUS_COPY_COST_VIGOUR then
			return ERROR.TipMsg(connection, "您的体力不足。")
		end
		user:SetDailyHeroCopyTimes("num", 1)
		user:SetVigour(-FAMOUS_COPY_COST_VIGOUR)
	else
		if user:GetVigour() < NORMAL_COPY_COST_VIGOUR then
			return ERROR.TipMsg(connection, "您的体力不足。")
		end
		user:SetVigour(-NORMAL_COPY_COST_VIGOUR)
	end
	local copy = NewCopy(data, connection.roleId)
	user:SetCurCopy(copy)
	local scene = copy:GetScene()
	connection:SendProtocol("c_copy_enter", scene:GetId())
--	connection:SendProtocol("c_copy_enter", copy:GetId())
--	PROTOCOL.s_scene_enter(connection, scene:GetId())
end


local s_copy_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	user:LeaveCopy(copy)
	local sceneId = SCENE.GetReturnedTownId(user)
	connection:SendProtocol("c_copy_leave", sceneId)
end
--[[
local s_copy_fight = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	if not copy:GetCopyMonster(data) then
		return ERROR.TipMsg(connection, "没有指定的敌人信息。")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "您正在进行战斗。")
	end
	local ret, msg = copy:ProFight(data)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
end
]]
local s_copy_stats = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	if copy:GetType() == "team" then
		return ERROR.TipMsg(connection, "该类副本没有统计界面。")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	return copy:SendCopyStats(user)
end

local s_copy_click = function(connection, data)
	data = tonumber(data)
	if not data or data < 1 or data > 5 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	if copy:IsPrizeTaken(connection.roleId) then
		return ERROR.TipMsg(connection, "您已经选择过奖励了。")
	end
	if copy:IsPosClicked(connection.roleId, data) then
		return ERROR.TipMsg(connection, "您已经选择过该位置了。")
	end
	local item = copy:SendClickReward(connection.roleId, data)
	connection:SendProtocol("c_copy_click", item)
	if copy:GetClickTimes(connection.roleId) >= copy:GetClickMaxTimes(connection.roleId) then
		copy:AutoChoose()
	end
end

local s_copy_prizeInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	copy:StartChooseCallOut()
	local timelong = copy:GetPrizeChooseTimestamp() - os.time()
	connection:SendProtocol("c_copy_prizeInfo", {timelong = timelong > 0 and timelong or 0, num = BASECOPY.GetPrizeNum()})
end

local s_copy_prizeShow = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	if not copy:IsPrizeTaken(connection.roleId) then
		return ERROR.TipMsg(connection, "请先选择奖励。")
	end
	local sData = copy:GetPrizeList(connection.roleId)
	table.sort(sData, function(a, b) return a.pos < b.pos end)
	connection:SendProtocol("c_copy_prizeShow", sData)
end


local s_copy_mopup = function(connection, data)
	if type(data) ~= "table" or not data.copyId or not data.num then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local info = BASECOPY.GetCopyInfoById(data.copyId)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MOPUP_MIN_LV then
		return ERROR.TipMsg(connection, "等级不足")
	end
	if not user:IsCopyPassed(info.type, data.copyId) then
		return ERROR.TipMsg(connection, "只能扫荡已通关的副本。")
	end
	if user:GetCurMopup() then
		return ERROR.TipMsg(connection, "您正在进行副本扫荡。")
	end
	local tribe = user:GetTribe()
	if tribe and user:IsFuncOpen(SETTING.FUNC_ST_RETREAT) then
		local retreat = tribe:GetStruct("retreat")
		if retreat and retreat:GetMopup() then
			return ERROR.TipMsg(connection, "您正在进行修仙塔扫荡。")
		end
	end
	local times = data.num 
	if info.type == "hero" then
		if user:GetDailyHeroCopyTimes("num") >= user:GetDailyHeroCopyTimes("max") then
			return ERROR.TipMsg(connection, "已无剩余次数。")
		end
		local vigour = user:GetVigour()
		if vigour < FAMOUS_COPY_COST_VIGOUR then
			return ERROR.TipMsg(connection, "")
		end
		if data.num == -1 then
			times = math.min(math.floor(vigour / FAMOUS_COPY_COST_VIGOUR), user:GetDailyHeroCopyTimes("max") - user:GetDailyHeroCopyTimes("num"))
		end
	else
		local vigour = user:GetVigour()
		if vigour < NORMAL_COPY_COST_VIGOUR or vigour < data.num * NORMAL_COPY_COST_VIGOUR then
			return ERROR.TipMsg(connection, "精力不足，无法扫荡")
		end
		if data.num == -1 then
			times = math.floor(vigour / NORMAL_COPY_COST_VIGOUR)
		end
	end
	if user:GetGold() < times * MOPUP_COST_GOLD then
		return ERROR.TipMsg(connection, "您的钱币不足。")
	end
	if user:GetGridMax(USERBASE.PACKAGE) - user:GetItemGridNum(USERBASE.PACKAGE) < MOPUP_NEED_GRID then
		return ERROR.TipMsg(connection, "您的背包空间不足。")
	end
	CopyMopUp(user, data.copyId, times)
	connection:SendProtocol("c_copy_mopup", {type = info.type, copyId = data.copyId, num = data.num, timelong = MOPUP_TIMELONG, state = STATE_DOING, fastIngot = FAST_MOPUP_NEED_INGOT})
end

local s_copy_mopupInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	local mopup = user:GetCurMopup()
	if mopup then
		local got = user:GetMopupGot()
		sData.state = mopup.state
		sData.copyId = mopup.copyId
		sData.itemGot = got.itemList
		if mopup.state == STATE_DOING then
			sData.curTimes = mopup.num
			sData.maxTimes = mopup.max
			sData.fastIngot = FAST_MOPUP_NEED_INGOT
			sData.timelong = mopup.timestamp - os.time()
		else
			sData.expGot = got.exp
			sData.nimbusGot = got.nimbus
		end
		sData.townSceneId = SCENE.GetReturnedTownId(user)
	else
		sData = 0
	end
	connection:SendProtocol("c_copy_mopupInfo", sData)
end

local s_copy_mopupStop = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local mopup = user:GetCurMopup()
	if not mopup then
		return ERROR.TipMsg(connection, "您当前没有进行副本扫荡。")
	end
	StopMopup(user)
	connection:SendProtocol("c_copy_mopupStop", "true")
end

local s_copy_mopupFinish = function(connection, data)
	if data ~= "ingot" and data ~= "voucher" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local mopup = user:GetCurMopup()
	if not mopup then
		return ERROR.TipMsg(connection, "您当前没有进行副本扫荡。")
	elseif mopup.state == STATE_FINISH then
		return ERROR.TipMsg(connection, "本次扫荡已经结束。")
	end
	local cost = (mopup.max - mopup.num + 1) * FAST_MOPUP_NEED_INGOT
	local moneyInfo = MONEY.GetGoldInfoByType(data)
	if USERBASE.clsUser[moneyInfo.get](user) < cost then
		return ERROR.TipMsg(connection, string.format("您的%s不足，请充值。", moneyInfo.name))
	end
	USERBASE.clsUser[moneyInfo.set](user, -cost)
	FastMopup(user)
end

--获取副本剧情对话id
local s_copy_dialog = function(connection, data)
	if data ~= "begin" and data ~= "end" then
		return ERROR.ErrorMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	local info = BASECOPY.GetCopyInfoById(copy:GetId())
	local dialog = -1
	local misObj = user:GetMissionById(info.needMissionId)
	if misObj then
		if (data == "begin" and misObj:GetState() == CLSMISS.DOING) or (data == "end" and misObj:GetState() == CLSMISS.FINISHED) then
			local missInfo = CLSMISS.GetMissionInfoById(info.needMissionId)
			if not copy:IsDialogOver(data) and missInfo then
				dialogInfo = missInfo[data .. "Plot"]
				if dialogInfo then
					dialog = dialogInfo.plotId
				end
			end
		end
	end
	connection:SendProtocol("c_copy_dialog", {type = data, dialog = dialog})
end
--剧情对话完成
local s_copy_dialogOver = function(connection, data)
	if data ~= "begin" and data ~= "end" and data ~= "boss" then
		return ERROR.ErrorMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中中")
	end
	copy:SetDialogOver(data)
	local info = BASECOPY.GetCopyInfoById(copy:GetId())
	local monsterId
	if user:GetMissionById(info.needMissionId) then
		local missInfo = CLSMISS.GetMissionInfoById(info.needMissionId)
		if missInfo then
			dialogInfo = missInfo[data .. "Plot"]
			if dialogInfo then
				monsterId = dialogInfo.monsterId
			end
		end
	end
	connection:SendProtocol("c_copy_dialogOver", data)
	if monsterId then
		local roleId = copy:GetCopyMonster(monsterId)
		if roleId then
			copy:ProFight(roleId)
		end
	end
end

local s_heroCopy_buy = function(connection, data)
	if data ~= "ingot" and data ~= "voucher" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetDailyHeroCopyTimes("max") >= HERO_COPY_BUY_TIMES + USERCOPY.DAILY_HERO_COPY_NUM then
		return ERROR.TipMsg(connection, "今日购买次数已经用完。")
	end
	local cost = (user:GetDailyHeroCopyTimes("max") - USERCOPY.DAILY_HERO_COPY_NUM + 1) * HERO_COPY_BUY_PRICE
	local moneyInfo = MONEY.GetGoldInfoByType(data)
	if USERBASE.clsUser[moneyInfo.get](user) < cost then
		return ERROR.TipMsg(connection, string.format("您的%s不足，请充值。", moneyInfo.name))
	end
	USERBASE.clsUser[moneyInfo.set](user, -cost)
	user:SetDailyHeroCopyTimes("max", 1)
	connection:SendProtocol("c_heroCopy_buy", GetHeroCopyTimesInfo(user))
end



function __init__()
	PROTOCOL.s_copy_list = s_copy_list		--副本列表
	PROTOCOL.s_copy_info = s_copy_info		--副本信息
	PROTOCOL.s_copy_enter = s_copy_enter	--进入副本
	PROTOCOL.s_copy_leave = s_copy_leave	--退出副本
	
--	PROTOCOL.s_copy_fight = s_copy_fight	--副本战斗
	PROTOCOL.s_copy_stats = s_copy_stats	--副本评分信息
	PROTOCOL.s_copy_click = s_copy_click	--选择奖励
	PROTOCOL.s_copy_prizeShow = s_copy_prizeShow	--副本奖励展示
	PROTOCOL.s_copy_prizeInfo = s_copy_prizeInfo	--副本奖励展示
	PROTOCOL.s_copy_dialog = s_copy_dialog			--副本剧情动画
	PROTOCOL.s_copy_dialogOver = s_copy_dialogOver	--副本剧情动画播放完毕
	
	PROTOCOL.s_copy_mopup = s_copy_mopup		--副本扫荡
	PROTOCOL.s_copy_mopupInfo = s_copy_mopupInfo		--副本扫荡
	PROTOCOL.s_copy_mopupStop = s_copy_mopupStop		--取消副本扫荡
	PROTOCOL.s_copy_mopupFinish = s_copy_mopupFinish	--快速完成副本扫荡
	
	PROTOCOL.s_heroCopy_buy = s_heroCopy_buy	--购买英魂副本进入次数
end

