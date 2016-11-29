--日常任务


local DAILY_MISSION_PREFIX = 'D'			--日常任务id开头

local DailyMissLvList = {}			--日常任务等级信息表

function GetDailyMissListByLv(Lv)
	for _, v in ipairs(DailyMissLvList) do
		if Lv >= v.minLv and Lv <= v.maxLv then
			return v.list
		end
	end
end



function FreshDailyMission(user)
	local userLv = user:GetLv()
	local list = GetDailyMissListByLv(userLv)
	if not list then
		return
	end
	local id = list[math.random(1, #list)]
	local buff = {}
	buff.state = CLSMISS.UNSTARTED
	buff.missionId = DAILY_MISSION_PREFIX .. STATS.GenId("dailyMiss")
	buff.template = id
	buff.prize = {}
	local lvInfo = USERBASE.GetUserUpgradeInfo(userLv)
	buff.prize.exp = lvInfo.dailyMissExp
	buff.prize.nimbus = lvInfo.dailyMissNimbus
	user:SetCurDailyMission(buff)
	user:SetDailyMissFreshTime(os.time())
end

function GetDailyMissInfo(user)
	local mis = user:GetCurDailyMission()
	local sData = {}
	sData.dailyNum = user:GetDailyMissReceiveTimes()
	sData.dailyMax = user:GetDailyMissMax()
	if mis then
		sData.missionId = mis.missionId
		sData.state = mis.state
		sData.prize = mis.prize
		local misInfo = CLSMISS.GetMissionInfoById(mis.template)
		sData.name = misInfo.name
		sData.content = CLSMISS.GetFormatMissionDesc(mis.template)
	end
	return sData
end

function RenewAllUserDailyMission()
	for _, v in pairs(USER.GetAllOnlineUser()) do
		v:RenewDailyMission(true)
	end
end




local s_mission_dailyInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:IsFuncOpen(SETTING.FUNC_DAILYMISSION) then
		return ERROR.TipMsg(connection, "您尚未开启该功能。")
	end
	local sData = GetDailyMissInfo(user)
	connection:SendProtocol("c_mission_dailyInfo", sData)
end


function __init__()
	
	local DAILYMISS_LVINFO = Import("autocode/missDailyLvInfo.lua")
	DailyMissLvList = DAILYMISS_LVINFO.GetTable()
	
	CALLOUT.Daily(getfenv(2), 0, 0, RenewAllUserDailyMission)
	
	PROTOCOL.s_mission_dailyInfo = s_mission_dailyInfo
end


