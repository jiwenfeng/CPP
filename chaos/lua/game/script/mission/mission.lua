--mission.lua

local MaterMissChapter = {}			--主线任务章节


--任务模版列表
local MissionClassList = {
	["dialog"] = require "mission/clsDlgMis",		--对话类
	["fight"] = require "mission/clsFightMis",		--杀怪类
	["collect"] = require "mission/clsCollectMis",	--采集类
	["gotItem"] = require "mission/clsDevoteMis",	--捐献指定物品类
	["useItem"] = require "mission/clsUseMis",		--使用特定物品类
	["buyItem"] = require "mission/clsBuyMis",		--购买指定物品类
	["multi"] = require "mission/clsMultiMis",		--综合类
	
	["func"] = require "mission/clsMultiMis",		--特殊功能类
	["struct"] = require "mission/clsStructMis",	--特殊功能类
}

--任务状态排序优先级
local stateOrder = {
	[CLSMISS.FINISHED] = 1, [CLSMISS.UNSTARTED] = 2, [CLSMISS.DOING] = 3
}
--任务类型排序优先级
local missTypeOrder = {
	["master"] = 1, ["status"] = 2, ["hero"] = 3, ["branch"] = 4, ["daily"] = 5, ["missKind"] = 6,
}

function GetChapterInfoById(id)
	return MaterMissChapter[id]
end

function GetUserChapterInfo(user, chapterId)
	local sData = {}
	sData.chapterId = chapterId
	local info = GetChapterInfoById(chapterId)
	sData.content = info.describe
	sData.list = {}
	for k, v in ipairs(info.list) do
		local buff = {}
		local mis = CLSMISS.GetMissionInfoById(v)
		buff.name = mis.name
		local misObj = user:GetMissionById(v)
		if misObj then
			buff.state = misObj:GetState()
		elseif user:IsMissionOver(v) then
			buff.state = CLSMISS.ALLDONE
		else
			buff.state = CLSMISS.UNSTARTED
		end
		table.insert(sData.list, buff)
	end
	return sData
end

--通过任务ID获取任务的所属类
function GetMIssionClassById(id)
	local mis = CLSMISS.GetMissionInfoById(id)
	if not mis then
		return
	end
	return MissionClassList[mis.class]
end
--检查任务事件
function CheckMissionEvent(user, misClass, id, num, limit)
	if not MissionClassList[misClass] then
		return
	end
	id = tonumber(id)
	for _, misObj in pairs(user:GetMissionList()) do
		local class = misObj:GetClass()
		if misObj:GetState() ~= CLSMISS.FINISHED and (class == misClass or class == "multi") then
			misObj:ModMissionDetail(user, id, num, limit)
			misObj:CheckMissionOver(user)			
		end
	end
end

------------------------------------------------------------

function IsMissionValid(user, missionId)
	local missInfo = CLSMISS.GetMissionInfoById(missionId)
	if not missInfo then
		return false
	elseif user:GetLv() < missInfo.needLv then
		return false
	elseif missInfo.fore and not user:IsMissionOver(missInfo.fore) then
		return false
	elseif missInfo.needStatusLv and user:GetStatusLv() < missInfo.needStatusLv then
		return false
	end
	if user:IsMissionOver(missionId) then
		return false
	end
	return true
end

function GetAcceptableMissionInfo(user, missionId, showStartNpc)
	local misObj = user:GetMissionById(missionId)
	if not misObj then
		local mis = CLSMISS.GetMissionInfoById(missionId)
		if mis then
			local buff = {missionId = missionId, name = mis.name, type = mis.type, state = CLSMISS.UNSTARTED}
			if mis.needLv <= user:GetLv() then
				if showStartNpc then
					local info = NPC.GetNpcInfoById(mis.startNpcId)
					if info then
						buff.startNpc = info.name
					end
				end
			elseif mis.type == "master" then
				buff.state = CLSMISS.INVALID
				buff.needLv = mis.needLv
				local chapterInfo = GetChapterInfoById(mis.chapter)
				buff.chapterId = mis.chapter
				buff.chapterName = chapterInfo.name
			end
			return buff
		end
	end
end

--获取可以领取的任务列表(不包含日常任务) --shortGuide 右侧的任务导航栏
function GetAcceptableMissionList(user, shortGuide)
	local sData = {}
	for k, v in pairs(CLSMISS.GetAllMissionInfo()) do
		local id
--[[		if v.type == "master" and not v.fore then						-- fore 前置任务 next 下一个任务
			id = k
			while(user:IsMissionOver(id)) do
				local mis = CLSMISS.GetMissionInfoById(id)
				id = mis.next
			end			
		elseif (v.type == "branch" or v.type == "status" or v.type == "hero") and IsMissionValid(user, k) then]]
		if (v.type == "branch" or v.type == "status" or v.type == "hero") and IsMissionValid(user, k) then
			id = k
		end
		if id then
			local buff = GetAcceptableMissionInfo(user, id, shortGuide)
			if buff then
				table.insert(sData, buff)
			end
		end
	end
--[[	table.sort(sData, function (a, b)
			local mis_a = CLSMISS.GetMissionInfoById(a.missionId)
			local mis_b = CLSMISS.GetMissionInfoById(b.missionId)
			return mis_a.needLv < mis_b.needLv
		end)]]
	return sData
end


--查询npc相关任务列表
function GetMissionListByNpc(npcId, user, flag)
	local sData = {}
	local list = user:GetMissionList()
	for k, v in pairs(CLSMISS.GetAllMissionInfo()) do	
		if not flag and v.endNpcId == npcId then
			if list[k] and list[k]:GetState() == CLSMISS.FINISHED then
				table.insert(sData, {type = v.type, missionId = k, name = v.name, state = CLSMISS.FINISHED})
			end
		end
		if v.startNpcId == npcId then
			if (not flag and list[k] and list[k]:GetState() ~= CLSMISS.FINISHED) or (not list[k] and IsMissionValid(user, k)) then
				table.insert(sData, {type = v.type, missionId = k, name = v.name, state = list[k] and list[k]:GetState() or CLSMISS.UNSTARTED})
			end
		end
	end
	table.sort(sData, function(a, b) 
		return missTypeOrder[a.type] < missTypeOrder[b.type]
			or (missTypeOrder[a.type] == missTypeOrder[b.type] and stateOrder[a.state] < stateOrder[b.state])
		end)
	return sData
end


--获取正在进行的任务
function GetDoingMissionList(user, shortGuide)
	local sData = {}
	local hasMaster = false
	for k, v in pairs(user:GetMissionList()) do
		local misType = v:GetType()
		if misType == "master" then
			hasMaster = true
		end
		if misType ~= "daily" or shortGuide then
			table.insert(sData, v:GetMissionData(shortGuide))
		end
	end
	if not hasMaster then
		for k, v in pairs(CLSMISS.GetAllMissionInfo()) do
			if v.type == "master" and not v.fore then						-- fore 前置任务 next 下一个任务
				local id = k
				while(user:IsMissionOver(id)) do
					local mis = CLSMISS.GetMissionInfoById(id)
					id = mis.next
				end
				local buff = GetAcceptableMissionInfo(user, id, shortGuide)
				if buff then
					table.insert(sData, buff)					
				end
				break
			end
		end		
	end
	return sData
end




--登录时检查是否有任务完成
function LoginCheckMissionState(user)
	local list = user:GetMissionList()
	for k, v in pairs(list) do
		if v:GetState() == CLSMISS.FINISHED then
			local con = LOGIN.GetConnection(user:GetRoleId())
			if con and con.roleId == user:GetRoleId() then
				return con:SendProtocol("c_mission_finish", {})
			end
		end
	end
end

----------------------------------------------------------

--获取任务的详细信息
function GetMissionDetailData(id, user)
	local misObj = user:GetMissionById(id)
	if misObj then
		return true, misObj:GetDetailData(user)
	end
	local template = id
	local mission = CLSMISS.GetMissionInfoById(id)
	if not mission then
		local missInfo = user:GetCurDailyMission(id)
		if missInfo then
			template = missInfo.template
			mission = CLSMISS.GetMissionInfoById(template)
		else
			return false, "无效的任务id。"
		end
	end
	local sData = {}
	sData.missionId = id
	sData.aim = mission.aim
	if mission.needLv and user:GetLv() >= mission.needLv then
		sData.content = CLSMISS.GetFormatMissionDesc(id)
		sData.detail = {}
		for k, v in pairs(mission.target) do
			table.insert(sData.detail, {type = v.type, id = v.id, needNum = v.num, curNum = 0})
		end
	else
		sData.needLv = mission.needLv
	end
	if mission.type == "daily" then
		local missInfo = user:GetCurDailyMission(id)
		sData.prize = CLSMISS.GetFormatedPrize(user, missInfo.prize)
	else
		sData.prize = CLSMISS.GetFormatedPrize(user, mission.prize)
	end
	if mission.startNpcId then
		sData.startNpc = {}
		local info = NPC.GetNpcInfoById(mission.startNpcId)
		sData.startNpc.name = info.name
		sData.startNpc.npcId = mission.startNpcId
	end
	if mission.endNpcId then
		sData.endNpc = {}
		local info = NPC.GetNpcInfoById(mission.endNpcId)
		sData.endNpc.name = info.name
		sData.endNpc.npcId = mission.endNpcId
	end
	return true, sData
end



--任务面板列表
local s_mission_list = function(connection, data)
	if data ~= "acceptable" and data ~= "received" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	if data == "acceptable" then
		sData = GetAcceptableMissionList(user)
	elseif data == "received" then
		sData = GetDoingMissionList(user)
	end
	connection:SendProtocol("c_mission_list", sData)
end
--[[日常任务列表
local s_mission_dailyList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < DAILY_MISSION_OPEN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，尚未开放该功能。")
	end
	local sData = {type = "daily"}
	sData.timelong = user:GetDailyMissFreshTime() + DAILY_MISSION_FRESH_TIME - os.time()
	sData.freshCost = DAILY_MISSION_FRESH_COST
	sData.dailyNum = user:GetDailyMissNum()
	sData.dailyMax = user:GetDailyMissMax()
	sData.freeTimes = user:GetFreeRefreshTimes() -  user:GetRefreshTimes()
	sData.list = GetDailyMissionList(user)
	connection:SendProtocol("c_mission_dailyList", sData)
end]]
--查看任务详情
local s_mission_detail = function(connection, data)
	data = tostring(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetMissionById(data) then
		local mission = CLSMISS.GetMissionInfoById(data)
		if not mission then		
			local missInfo = user:GetCurDailyMission(data)
			if not missInfo then
				return ERROR.TipMsg(connection, "无效的任务id。")
			end
		end	
	end
	local rst, ret = GetMissionDetailData(data, user)
	if not rst then
		return ERROR.TipMsg(connection, ret)
	end
	connection:SendProtocol("c_mission_detail", ret)
end
--导航面板列表
local s_mission_guideList = function(connection, data)
	if data ~= "acceptable" and data ~= "received" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	if data == "acceptable" then
		sData = GetAcceptableMissionList(user, true)
	elseif data == "received" then
		sData = GetDoingMissionList(user, true)
	end
	table.sort(sData, function(a, b) 
		return missTypeOrder[a.type] < missTypeOrder[b.type]
			or (missTypeOrder[a.type] == missTypeOrder[b.type] and stateOrder[a.state] < stateOrder[b.state])
		end)
	connection:SendProtocol("c_mission_guideList", sData)
end
--领取任务
local s_mission_accept = function(connection, data)
	data = tostring(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local mission = CLSMISS.GetMissionInfoById(data)
	local missType
	if not mission then
		local missInfo = user:GetCurDailyMission(data)
		if not missInfo then
			return ERROR.TipMsg(connection, "无效的任务ID")
		end
		if user:GetDailyMissReceiveTimes() >= user:GetDailyMissMax()	then
			return ERROR.TipMsg(connection, "您今天的日常任务已经做完")
		end
		missType = "daily"
		mission = CLSMISS.GetMissionInfoById(missInfo.template)
	end
	if mission.needLv and user:GetLv() < mission.needLv then
		return ERROR.TipMsg(connection, "您的等级不足。")
	end
	if mission.fore and not user:IsMissionOver(mission.fore) then
		return ERROR.TipMsg(connection, "您尚未前置任务未完成，不能领取该任务。")
	end
	if missType == "daily" and user:GetCurDailyMission() then
		return ERROR.TipMsg(connection, "你已经领取了日常任务，快去完成吧！")
	end
	local ret, msg = user:AddMission(data)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_mission_accept", data)
	if missType == "daily" then
		local pet = user:GetPet()
		pet:SendDailyMissionAsk(data)
	end	
end
--领取奖励
local s_mission_prize = function(connection, data)
	data = tostring(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local misObj = user:GetMissionById(data)
	if not misObj then
		return ERROR.TipMsg(connection, "您尚未领取该任务。")
	end
	if misObj:GetState() ~= CLSMISS.FINISHED then
		return ERROR.TipMsg(connection, "该任务尚未完成，不能领取奖励。")
	end
	user:FinishMission(data)
	connection:SendProtocol("c_mission_prize", data)
end

local s_mission_cancel = function(connection, data)
	data = tostring(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local misObj = user:GetMissionById(data)
	if not misObj then
		return ERROR.TipMsg(connection, "您尚未领取该任务。")
	end
	if misObj:GetState() == CLSMISS.FINISHED then
		return ERROR.TipMsg(connection, "该任务已经达成，请领取奖励。")
	end
	user:CancelMission(data)
	connection:SendProtocol("c_mission_cancel", data)
end
--[[
local s_mission_quickDone = function(connection, data)
	data = tostring(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local misObj = user:GetMissionById(data)
	if not misObj then
		return ERROR.TipMsg(connection, "您尚未领取该任务。")
	end
	if misObj:GetType() ~= "daily" then
		return ERROR.TipMsg(connection, "日常任务才可以使用快速完成。")
	end
	user:FinishMission(data)
	connection:SendProtocol("c_mission_quickDone", data)
end

local s_mission_refresh = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	--没有免费次数
	if user:GetRefreshTimes() >= user:GetFreeRefreshTimes() then
		if user:GetTotalIngot() < DAILY_MISSION_FRESH_COST then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值")
		end
		user:SetTotalIngot(-DAILY_MISSION_FRESH_COST)
	else
		user:SetRefreshTimes(1)
	end
	local list = GenDailyMissionList(user)
	user:SetDailyMissionList(list)
	connection:SendProtocol("c_mission_refresh", "true")
	PROTOCOL.s_mission_dailyList(connection, "get")
end]]

local s_mission_chapterInfo = function(connection, data)
	if not GetChapterInfoById(data) then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetUserChapterInfo(user, data)
	connection:SendProtocol("c_mission_chapterInfo", sData)
end


function __init__()

	local MISS_CHAPTER = Import("autocode/missMasterChapter.lua")
	MaterMissChapter = MISS_CHAPTER.GetTable()
	
	
	PROTOCOL.s_mission_list = s_mission_list
	PROTOCOL.s_mission_dailyList = s_mission_dailyList
	PROTOCOL.s_mission_detail = s_mission_detail
	
	PROTOCOL.s_mission_guideList = s_mission_guideList
	PROTOCOL.s_mission_chapterInfo = s_mission_chapterInfo
	
	PROTOCOL.s_mission_accept = s_mission_accept
	PROTOCOL.s_mission_prize = s_mission_prize
	PROTOCOL.s_mission_cancel = s_mission_cancel
	PROTOCOL.s_mission_quickDone = s_mission_quickDone
	PROTOCOL.s_mission_refresh = s_mission_refresh
end
