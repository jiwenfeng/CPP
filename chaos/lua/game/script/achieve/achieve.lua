--成就系统

local AchvTypeList = {
	["grown"] = 1, ["status"] = 2, ["act"] = 3, 
	["copy"] = 4, ["undergo"] = 5, ["honor"] = 6
}
local AchieveList = {}		--成就表
local BadgeUpgradeList = {}	--升级表
local MAX_RECENT_RECORD = 3

local DOING = 0		--未完成
local FINISH = 1	--已经完成未领奖
local DONE = 2		--已经领取奖励

function GetAllAchvList()
	return AchieveList
end

function GetAchvInfoById(id)
	return AchieveList[id]
end

function GetBadgeInfoByLv(lv)
	return BadgeUpgradeList[lv]
end

function GetBadgeNameByLv(lv)
	local badgeInfo = GetBadgeInfoByLv(lv)
	return badgeInfo.name
end

function GetBadgeLvByPoint(point)
	local Lv = 0
	for k, v in pairs(BadgeUpgradeList) do
		if v.needPoint and point >= v.needPoint then
			Lv = math.max(k + 1, Lv)
		end
	end
	return Lv
end

local achvNeedShow = {
	["winPhyFight"] = function(user)
		local info = user:GetPhyleFightStats()
		return info.win or 0
	end,
	["sacrifice"] = function(user)
		return user:GetUnionSacrificeTimes()
	end,
	["favor"] = function(user, id)
		local info = GetAchvInfoById(id)
		local friend = user:GetFriendList()
		return friend:GetNumByFavor(info.num1)
	end,
	["friendNum"] = function(user)
		local friend = user:GetFriendList()
		return table.size(friend:GetMemberList("friend"))
	end,
	["sendFlower"] = function(user)
		return user:GetSendFlowerTimes()
	end,
	["gotFlower"] = function(user)
		return user:GetReceiveFlowerTimes()
	end,
	["cityAttack"] = function(user)
		return user:GetCityAttackTimes()
	end,
}

function GetUserAchvDetailNum(user, achvId)
	local info = GetAchvInfoById(achvId)
	if info then
		if achvNeedShow[info.label] then
			return achvNeedShow[info.label](user, achvId)
		else
			return 0
		end
	end
	
end

function GetAchvDataById(user, id, flag)
	local sData = {}
	local info = GetAchvInfoById(id)
	local sData = {}
	sData.achvId = id
	sData.name = info.name
	sData.prize = info.prize
	sData.point = info.point
	sData.describe = info.describe
	sData.state = user:GetAchvState(id) or DOING
	if not flag then
--		sData.type = v.type
		sData.kind = info.kind
		if info.showNum then
			sData.num = math.min(info.showNum, GetUserAchvDetailNum(user, id))
			sData.max = info.showNum
		end
	end
	return sData
end

function SendAchvUpdate(user, id)
	local roleId = user:GetRoleId()	
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_achv_update", GetAchvDataById(user, id, true))
	end
end

function SetUserAchvGot(user, id)
	local info = GetAchvInfoById(id)
	user:SetAchvById(id, FINISH, os.time())
	user:SetAchvPoint(info.point or 0)
	SendAchvUpdate(user, id)
end

local AchvFuncList = {
	["userLv"] = function(user)
		local Lv = user:GetLv()
		for k, v in pairs(GetAllAchvList()) do
			if v.kind == "grade" and not user:IsAchvGot(k) then
				if v.label == "Lv" and Lv >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["attr"] = function(user)
		local attrList = {["strength"] = 1, ["intellect"] = 1, ["agility"] = 1, ["captain"] = 1}
		for k, v in pairs(GetAllAchvList()) do
			if v.kind == "attr" and not user:IsAchvGot(k) then
				if attrList[v.label] and user:GetTotalAttr(v.label) >= v.num1 then
					SetUserAchvGot(user, k)
				elseif v.label == "all" then
					local flag = true
					for attr in pairs(attrList) do
						if user:GetTotalAttr(attr) < v.num1 then
							flag = false
						end
					end
					if flag then
						SetUserAchvGot(user, k)
					end
				end
			end
		end
	end,
	["heroNum"] = function(user)
		local heroList = user:GetHeroList()
		local function get_num_by_quality(quality)
			local num = 0
			for k, v in pairs(heroList) do
				if v:GetAptLv() >= quality then
					num = num + 1
				end
			end
			return num
		end
		for k, v in pairs(GetAllAchvList()) do
			if v.kind == "hero" and not user:IsAchvGot(k) then
				if v.label == "heroNum" and get_num_by_quality(0) >= v.num1 then
					SetUserAchvGot(user, k)
				elseif v.label == "heroApt" and get_num_by_quality(v.num1) >= v.num2 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["status"] = function(user)
		local statusLv = user:GetStatusLv()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "statusLv" and not user:IsAchvGot(k) then
				if statusLv >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["arena"] = function(user)
		local info = ARENA.GetUserArenaInfo(user:GetRoleId())
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "arena" and not user:IsAchvGot(k) then
				if user:GetArenaTimes() >= v.num1 then
					SetUserAchvGot(user, k)
				end
			elseif v.label == "arenaWin" and not user:IsAchvGot(k) then
				if info and info.win.top >= v.num1 then
					SetUserAchvGot(user, k)
				end
			elseif v.label == "arenaRank" and not user:IsAchvGot(k) then
				if info and info.rank <= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["cityAttack"] = function(user)
		local times = user:GetCityAttackTimes()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "cityAttack" and not user:IsAchvGot(k) then
				if times >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["phyleFight"] = function(user)
		local info = user:GetPhyleFightStats()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "joinPhyFight" and not user:IsAchvGot(k) then
				if (info.sum or 0) >= v.num1 then
					SetUserAchvGot(user, k)
				end
			elseif v.label == "winPhyFight" and not user:IsAchvGot(k) then
				if (info.win or 0) >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
--	["bossActWin"] = function(user)  end,			--在 clsWorldBoss.lua 中实现
	["bossActKill"] = function(user, actObj)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "bossActKill" and not user:IsAchvGot(k) then
				if actObj:GetId() == tostring(v.num1) then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,	
	["sacrifice"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "sacrifice" and not user:IsAchvGot(k) then
				if user:GetUnionSacrificeTimes() >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
--	["unionBoss"] = function(user) end,			--在 clsUniRelic.lua 中实现
	["friend"] = function(user)
		local friend = user:GetFriendList()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "favor" and not user:IsAchvGot(k) then
				if friend:GetNumByFavor(v.num1) >= v.num2 then
					SetUserAchvGot(user, k)
				end
			elseif v.label == "friendNum" and not user:IsAchvGot(k) then
				if table.size(friend:GetMemberList("friend")) >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["doubleRetreat"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "doubleRetreat" and not user:IsAchvGot(k) then
				SetUserAchvGot(user, k)
			end
		end
	end,
	["sendFlower"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "sendFlower" and not user:IsAchvGot(k) then
				if user:GetSendFlowerTimes() >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["receiveFlower"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "gotFlower" and not user:IsAchvGot(k) then
				if user:GetReceiveFlowerTimes() >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["petForm"] = function(user)
		local info = user:GetPetTransformInfo()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "form" and not user:IsAchvGot(k) then
				if info and (info.times >= v.num1) then
					SetUserAchvGot(user, k)
				end
			elseif v.label == "formAll" and not user:IsAchvGot(k) then
				if info and table.size(info.list) >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["treasure"] = function(user, quality)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "gotGood" and not user:IsAchvGot(k) then
				if quality >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["formation"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "formation" and not user:IsAchvGot(k) then
				if table.size(user:GetFightFormList()) >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["vip"] = function(user)
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "vip" and not user:IsAchvGot(k) then
				if user:GetVipLv() >= v.num1 then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,
	["famousHero"] = function(user)
		local roleId = user:GetRoleId()
		for k, v in pairs(GetAllAchvList()) do
			if v.label == "famousHero" and not user:IsAchvGot(k) then
				local flag = true
				local i = 1
				while(v["num" .. i]) do
					local famousInfo = FAMOUSHERO.GetFamousHeroDataById(v["num" .. i])
					if not famousInfo or not famousInfo.ownerList[roleId] then
						flag = false
						break
					end
					i = i + 1
				end
				if flag then
					SetUserAchvGot(user, k)
				end
			end
		end
	end,	
}

function CheckAchvState(user, type, param)
	if not AchvFuncList[type] then
		return
	end
	return AchvFuncList[type](user, param)
end


--领取奖励
function GetAchievePrize(user, id)
	local info = GetAchvInfoById(id)
	local prize = info.prize
	local msg = "获得了"
	if prize.gold then
		msg = msg .. string.format("金钱%d", prize.gold)
		user:SetGold(prize.gold)
	end
	if prize.nimbus then
		msg = msg .. string.format("，灵气%d", prize.nimbus)
		user:SetNimbus(prize.nimbus)
	end
	if prize.item then
		local item = BASEITEM.NewItem(prize.item.goodsId, prize.item.num, {lock = 1})
		msg = msg .. string.format("，【%s】x %d", item:GetName(), item:GetNum())
		if user:IsItemCanAdd(USERBASE.PACKAGE, item) then
			user:AddItem(USERBASE.PACKAGE, item)
		else
			local mail = {}
			mail.title = "成就奖励"
			mail.content = string.format("由于背包已满，【%s】的成就奖励已发送到邮箱，请及时领取，避免损失。", info.name)
			mail.attach = {}
			mail.attach.item = {}
			table.insert(mail.attach.item, {id = prize.item.goodsId, num = prize.num, lock = 1})
			MAIL.SendSysMail(user:GetRoleId(), mail)
		end
	end
	user:SendProtocol("c_show_tips", msg)
end

function GetRecentAchvList(user)
	local list = {}
	for k, v in pairs(user:GetAchvList() or {}) do
		table.insert(list, {id = k, time = v.time})
	end
	table.sort(list, function(a, b) return a.time > b.time end)
	local sData = {type = "recent", list = {}}
	for i = 1, MAX_RECENT_RECORD do
		local buff = list[i]
		if buff then
			table.insert(sData.list, GetAchvDataById(user, buff.id))
		end
	end
	return sData
end

function GetFormatAchvList(user, type)
	if type == "recent" then
		return GetRecentAchvList(user)
	end
	local sData = {type = type, list = {}}
	for k, v in pairs(GetAllAchvList()) do
		if v.type == type then
			local buf = GetAchvDataById(user, k)
			table.insert(sData.list, buf)
		end
	end
--	前台排序
--	table.sort(sData.list, function(a, b) return tonumber(a.achvId) < tonumber(b.achvId) end)
	return sData
end

function GetAchvStatInfo(user)
	local sData = {}
	local badgeLv = user:GetBadgeLv()
	sData.badgeLv = badgeLv
	local badgeInfo = GetBadgeInfoByLv(badgeLv)
	sData.badgeName = badgeInfo.name
	sData.point = user:GetAchvPoint()
	sData.needPoint = badgeInfo.needPoint or -1
	sData.badgeAdd = user:GetBadgeAdd()
	sData.list = {}
	local list = {}
	for k, v in pairs(GetAllAchvList()) do
		list[v.type] = list[v.type] or {}
		list[v.type].max = (list[v.type].max or 0) + 1
		if user:IsAchvGot(k) then
			list[v.type].num = (list[v.type].num or 0) + 1
		end
	end
	for k, v in pairs(AchvTypeList) do
		local stat = list[k] or {}
		table.insert(sData.list, {type = k, max = stat.max or 0, num = stat.num or 0})
	end
	table.sort(sData.list, function(a, b) return AchvTypeList[a.type] < AchvTypeList[b.type] end)
	return sData
end

local s_achv_typeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = {}
	for k in pairs(AchvTypeList) do
		table.insert(sData, k)
	end
	table.sort(sData, function(a, b) return AchvTypeList[a] < AchvTypeList[b] end)
	table.insert(sData, 1, "recent")
	connection:SendProtocol("c_achv_typeList", sData)
end

local s_achv_list = function(connection, data)
	if data ~= "recent" and not AchvTypeList[data] then
		return ERROR.TipMsg(connection, "没有指定的成就分类。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatAchvList(user, data)
	connection:SendProtocol("c_achv_list", sData)
end

local s_achv_stat = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetAchvStatInfo(user)
	connection:SendProtocol("c_achv_stat", sData)
end

local s_achv_getPrize = function(connection, data)
	data = tostring(data)
	if not GetAchvInfoById(data) then
		return ERROR.TipMsg(connection, "没有指定的成就信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local info = user:GetAchvById(data)
	if not info then
		return ERROR.TipMsg(connection, "您尚未达成该成就。")
	end
	if info.state ~= FINISH then
		return ERROR.TipMsg(connection, "您已经领取该成就的奖励")
	end
	GetAchievePrize(user, data)
	user:SetAchvById(data, DONE)
	connection:SendProtocol("c_achv_getPrize", data)
end

function __init__()
	local ACHVLIST = Import("autocode/achvList.lua")
	AchieveList = ACHVLIST.GetTable()
	
	local BADGE = Import("autocode/badgeUpgradeList.lua")
	BadgeUpgradeList = BADGE.GetTable()
	
	PROTOCOL.s_achv_typeList = s_achv_typeList
	PROTOCOL.s_achv_list = s_achv_list
	PROTOCOL.s_achv_stat = s_achv_stat
	PROTOCOL.s_achv_getPrize = s_achv_getPrize
end
