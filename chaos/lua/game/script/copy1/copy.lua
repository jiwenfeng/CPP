--副本
MOPUP_MIN_LV = 20			--扫荡开放等级
MOPUP_COST_GOLD = 1000	--扫荡所需钱币
MOPUP_TIMELONG = 10 
MOPUP_NEED_GRID = 2
local HERO_COPY_BUY_PRICE = 5		--英魂副本购买价格基数
STATE_DOING = 1
STATE_FINISH = 0
local FAST_MOPUP_NEED_INGOT = 2
HERO_FAST_MOPUP = 80
NORMAL_FAST_MOPUP = 60
NORMAL_COPY_COST_VIGOUR = 5	--进入副本所需精力
FAMOUS_COPY_COST_VIGOUR = 10	--英魂副本所需精力

--离线计算副本物品奖励时调用
function GetCopyPrize(copyId, num)
	if num <= 0 then return end
	local info = CLSCOPY.GetCopyInfoById(copyId)
	if not info then return end
	local prize = BASEITEM.GenRandomItemByNum(info.itemList, num)
	return prize
end

function GetHeroCopyTimesInfo(user)
	local buf = {}
	buf.leftTimes = user:GetDailyHeroCopyMaxTimes() - user:GetDailyHeroCopyUseTimes()
	buf.buyTimes = VIP.GetVipFuncValidTimes(user:GetVipLv(), VIP.VIP_HERO_COPY_ADD_TIMES) - user:GetHeroCopyBuyTimes()
	buf.buyPrice = (user:GetHeroCopyBuyTimes() + 1) * HERO_COPY_BUY_PRICE
	return buf
end

--格式化的副本列表
function GetFormatedCopyList(user, townId)
	local list = SCENE.GetSceneInfoById("town", townId)
	local sData = {}
	for i = list.minCopyId, list.maxCopyId do
		local id = tostring(i)
		local info = CLSCOPY.GetCopyInfoById(id)
		if info then
			local state
			if user:IsCopyPassed(id) then
				state = CLSCOPY.COPY_PASSED
			elseif user:IsCopyOpen(id) then
				state = CLSCOPY.COPY_OPEN
			else
				state = CLSCOPY.COPY_CLOSE
			end
			if state ~= CLSCOPY.COPY_CLOSE then
				local buf = {}
				buf.state = state
				buf.copyId = id
				buf.name = info.name
				buf.mapId = SCENE.GetSceneInfoById(info.type, id).mapId
				buf.show = info.show
				buf.needVigour = info.type == "normal" and NORMAL_COPY_COST_VIGOUR or FAMOUS_COPY_COST_VIGOUR
				for k, v in pairs(info.monsterList) do
					local teamInfo = FIGHT.GetMonsterTeamInfoById(v.teamId)
					if teamInfo.isBoss then
						buf.headId = teamInfo.headId
						break
					end
				end
				table.insert(sData, buf)
			end
		end
	end
	return sData
end

function GetCopyDetail(id)
	local info = CLSCOPY.GetCopyInfoById(id)
	if not info then return end
	local buff = {}
	buff.copyId = id
--	if info.type == "team" then
--		buff.needLv = info.needLv
--	else
		buff.mapId = info.mapId
--	end
	buff.describe = info.describe
	return buff
end

function NewCopy(id, owner)
	local info = CLSCOPY.GetCopyInfoById(id)
	local obj
	if info.type == "normal" then
		obj = CLSNORMALCOPY.clsNormalCopy:New()
	elseif info.type == "hero" then
		obj = CLSHEROCOPY.clsHeroCopy:New()
	elseif info.type == "team" then
		obj = CLSTEAMCOPY.clsTeamCopy:New()
	else
		return
	end
	print("NewCopy", obj:GetType(), owner, id)
	obj:InitCopyInfo(id, owner)
	return obj
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
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local townId = SCENE.GetTownIdByCopyListId(data)
	if not townId then
		return ERROR.TipMsg(connection, "无效的ID")
	end
	local info = SCENE.GetSceneInfoById("town", townId)
	local sData = {}
	sData.mopupLv = MOPUP_MIN_LV
	sData.mopupNeed = MOPUP_COST_GOLD
	sData.list = GetFormatedCopyList(user, townId)
	sData.type = info.copyType
	if info.copyType == "hero" then
		local timesInfo = GetHeroCopyTimesInfo(user)
		sData.leftTimes = timesInfo.leftTimes
		sData.buyTimes = timesInfo.buyTimes
		sData.buyPrice = timesInfo.buyPrice
	end
	connection:SendProtocol("c_copy_list", sData)
end

local s_copy_info = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local sData = CLSCOPY.GetCopyDetail(data)
	if not sData then
		return ERROR.TipMsg(connection, "无效的副本ID")
	end
	connection:SendProtocol("c_copy_info", sData)
end

local s_copy_enter = function(connection, data)
	local info = CLSCOPY.GetCopyInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetCurCopy() then
		return ERROR.TipMsg(connection, "您已经位于一个副本中了。")
	end
	local mopup = user:GetCurMopup()
	if mopup and mopup.state == STATE_DOING then
		return ERROR.TipMsg(connection, "您正在进行副本扫荡，不能进入副本")
	end
	local copy = NewCopy(data, connection.roleId)
	if not copy then
		return ERROR.TipMsg(connection, "创建副本场景发生错误")
	end
	local ret, msg = copy:Enter()
	if not ret then
		copy:DestroyCopy()
		return ERROR.TipMsg(connection, msg)
	end
	local scene = copy:GetScene()
	local sData = {}
	sData.type = copy:GetType()
	sData.sceneId = scene:GetId()
	sData.groundList = copy:GetGroundItemList()
	connection:SendProtocol("c_copy_enter", sData)
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

local s_copy_stats = function(connection, data)
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
	return copy:SendCopyStats()
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
	if copy:GetType() == "normal" then
		return ERROR.TipMsg(connection, "普通副本不能翻牌")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	if copy:IsPrizeTaken() then
		return ERROR.TipMsg(connection, "您已经选择过奖励了。")
	end
	if copy:IsPosClicked(data) then
		return ERROR.TipMsg(connection, "您已经选择过该位置了。")
	end
	local item = copy:SendClickReward(data)
	connection:SendProtocol("c_copy_click", item)
	if copy:GetClickTimes() >= copy:GetClickMaxTimes() then
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
	if copy:GetType() == "normal" then
		return ERROR.TipMsg(connection, "普通副本没有翻牌界面")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	copy:StartChooseCallOut()
	local timelong = copy:GetPrizeChooseTimestamp() - os.time()
	connection:SendProtocol("c_copy_prizeInfo", {timelong = timelong > 0 and timelong or 0, num = CLSCOPY.GetPrizeNum()})
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
	if copy:GetType() == "normal" then
		return ERROR.TipMsg(connection, "普通副本没有翻牌功能")
	end
	if not copy:IsCopyPassed() then
		return ERROR.TipMsg(connection, "该副本尚未通关。")
	end
	if not copy:IsPrizeTaken() then
		return ERROR.TipMsg(connection, "请先选择奖励。")
	end
	local sData = copy:GetPrizeList()
	table.sort(sData, function(a, b) return a.pos < b.pos end)
	connection:SendProtocol("c_copy_prizeShow", sData)
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
	local info = CLSCOPY.GetCopyInfoById(copy:GetId())
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
	local info = CLSCOPY.GetCopyInfoById(copy:GetId())
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
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetDailyTeamCopyMaxBuyTimes() <= user:GetHeroCopyBuyTimes() then
		return ERROR.TipMsg(connection, "今日购买次数已经用完")
	end
	local cost = (user:GetHeroCopyBuyTimes() + 1) * HERO_COPY_BUY_PRICE
	if cost > user:GetTotalIngot() then
		return ERROR.TipMsg(connection, "元宝数量不足")
	end
	user:SetTotalIngot(-cost)
	user:SetHeroCopyBuyTimes(1)
	connection:SendProtocol("c_heroCopy_buy", GetHeroCopyTimesInfo(user))
end

local s_copy_pickup = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前不在副本中")
	end
	local ret, msg = copy:pickup(data)
	connection:SendProtocol("c_copy_pickup", data)
end

local s_copy_mopup = function(connection, data)
	if type(data) ~= "table" or not data.copyId or not data.num then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	if data.num <= 0 then
		return ERROR.TipMsg(connection, "无效的扫荡次数")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = NewCopy(data.copyId, connection.roleId, true)
	if not copy then
		return ERROR.TipMsg(connection, "无效的副本编号")
	end
	local ret, msg = copy:CheckMopupCondt(data.num)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	copy:CopyMopup(data.num)
end

local s_copy_mopupFinish = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurMopupCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前没有在扫荡")
	end
	local info = user:GetCurMopup()
	if not info or info.state == STATE_FINISH then
		return ERROR.TipMsg(connection, "您当前没有进行副本扫荡。")
	end
	local cost = (info.max - info.num + 1) * FAST_MOPUP_NEED_INGOT
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, string.format("您元宝数量不足"))
	end
	user:SetTotalIngot(-cost)
	local ret, msg = copy:FastMopup()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
end

local s_copy_mopupStop = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurMopupCopy()
	if not copy then 
		return ERROR.TipMsg(connection, "您当前没有进行扫荡")
	end
	local mopup = user:GetCurMopup()
	if not mopup or mopup.state == STATE_FINISH then
		return ERROR.TipMsg(connection, "当前扫荡已经结束")
	end
	local ret, msg = copy:StopMopup()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_copy_mopupStop", "true")
end
-------------------------------------------------

function __init__()
	PROTOCOL.s_copy_list = s_copy_list		--副本列表
	PROTOCOL.s_copy_info = s_copy_info		--副本信息
	PROTOCOL.s_copy_enter = s_copy_enter	--进入副本
	PROTOCOL.s_copy_leave = s_copy_leave	--退出副本
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
	PROTOCOL.s_heroCopy_buy = s_heroCopy_buy	--购买英魂副本进入次]数
	PROTOCOL.s_copy_pickup = s_copy_pickup
end
