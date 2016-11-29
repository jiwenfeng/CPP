--活动指引

local DailyActInfoList = {}

function GetActInfoById(id)
	return DailyActInfoList[id]
end

function GetAllActInfoList()
	return DailyActInfoList
end

--------------------------------------
--活动类型
local ActTypeFunc = {
	["worldBoss"] = function(id, user)
		local info = GetActInfoById(id)
		local times = user:GetWorldBossTimes(info.link)
		local state = WORLDBOSS.GetWorldBossActState(tostring(info.link))
		return times or 0, state
	end,
	["teamCopy"] = function(id, user)
		local info = GetActInfoById(id)
		local times = user:GetDailyTeamCopyNum()
		return times or 0
	end,
	["dailyMiss"] = function(id, user)
		local info = GetActInfoById(id)
		local times = user:GetDailyMissFinishTimes()
	--	local times = user:GetDailyMissNum()
		return times or 0
	end,
	["arena"] = function(id, user)
		local info = GetActInfoById(id)
		local times = user:GetArenaTimes()
		return times or 0
	end,
	["cityAttack"] = function(id, user)
		local info = GetActInfoById(id)
		local statInfo = user:GetFuncStatById(info.funcId)
		if statInfo then
			return statInfo.times or 0
		end
		return 0
	end,
}

function GetActJoinTimes(id, user)
	local info = GetActInfoById(id)
	return ActTypeFunc[info.type](id, user)
end

function GetActState(id, user)
	local info = GetActInfoById(id)
	local num, state = ActTypeFunc[info.type](id, user)
	if info.type == "worldBoss" then
		return state ~= "over"
	end
	if not info.times then
		return 1
	end
	return num < info.times
end

function FormatActivityList(user)
	local sData = {}
	for k, v in ipairs(GetAllActInfoList()) do
		local buf = {}
		buf.state  = GetActState(k, user)
		buf.picId = v.picId
		buf.name = v.name
		buf.maxNum = v.times
		buf.curNum = GetActJoinTimes(k, user)
		buf.describe = v.descrip
		buf.time = v.time
		buf.link = v.funcId
		buf.actId = tostring(v.link)
		buf.needLv = v.needLv
		table.insert(sData, buf)
	end
--	table.sort(sData, function(a, b) return a.state > b.state end)
	return sData
end

local s_activity_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = FormatActivityList(user)
	connection:SendProtocol("c_activity_list", sData)
end

function __init__()
	local tb = Import("autocode/dailyActList.lua")
	DailyActInfoList = tb.GetTable()
	PROTOCOL.s_activity_list = s_activity_list
end
