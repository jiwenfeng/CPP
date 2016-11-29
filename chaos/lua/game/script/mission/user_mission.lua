--玩家类：任务相关

local MAX_DAILY_MISSION_NUM = 10			--每日日常任务次数
local DAILY_MISSION_KINDNESS_TIMES = 1		--每日日行一善次数
local INIT_MASTER_MISSION_ID = "400001"		--初始主线任务id

local clsUser = USERBASE.clsUser

function clsUser:InitUserMission()
	self.__save.mission = {}
	self.__temp.mission.list = {}
	self.__save.mission.refresh = {}
	self.__save.mission.dailyCount = {}
	self.__save.mission.doneList = {}
	self:AddMission(INIT_MASTER_MISSION_ID)
--	self:RenewDailyMission()
end


function clsUser:GetMissionById(id)
	self.__temp.mission.list = self.__temp.mission.list or {}
	return self.__temp.mission.list[tostring(id)]
end

function clsUser:SetMissionById(id, obj)
	self.__temp.mission.list = self.__temp.mission.list or {}
	self.__temp.mission.list[tostring(id)] = obj
end

function clsUser:GetMissionList(state)
	self.__temp.mission.list = self.__temp.mission.list or {}
	local buff = {}
	for k, v in pairs(self.__temp.mission.list) do
		if not state or v:GetState() == state then
			buff[k] = v
		end
	end
	return buff
end

function clsUser:IsMissionAdd(missionId)
	self.__temp.mission = self.__temp.mission or {}
	for k, v in pairs(self.__temp.mission.list or {}) do
		if k == missionId and v:GetState() == CLSMISS.DOING then
			return true
		end
	end
	return false
end

--领取任务
function clsUser:AddMission(id)
	local template = id		--任务模版id
	local mis = CLSMISS.GetMissionInfoById(id)
	if not mis then
		local missInfo = self:GetCurDailyMission(id)
		if missInfo then
			template = missInfo.template
		else
			return false, "无效的任务id。"
		end
	end
	local class = MISSION.GetMIssionClassById(template)
	if not class then
		return false, "任务配置信息有误：class。"
	end
	local misObj = class:New()
	if template ~= id then
		local missInfo = self:GetCurDailyMission(id)
		misObj:InitSaveData(id, template, missInfo.quality, missInfo.prize)
	else
		misObj:InitSaveData(id)
	end
	self:SetMissionById(id, misObj)
	if misObj:GetType() == "daily" then
		self:ModDailyMissionState(CLSMISS.DOING)
	elseif misObj:GetType() == "missKind" then
		self:ModMissKindState(id, CLSMISS.DOING)
	end
	misObj:CheckMissionOver(self)
	return misObj
end

function clsUser:FinishAllAcpMission()
	for k, v in pairs(self.__temp.mission.list or {}) do
		self:FinishMission(k)
	end
end

--检查任务是否已经完成过
function clsUser:IsMissionOver(missionId)
	self.__save.mission.doneList = self.__save.mission.doneList or {}
	if CLSMISS.IsDailyMission(missionId) then
		return self.__save.mission.doneList["daily"] == missionId		--用于环任务
	end
	return self.__save.mission.doneList[missionId]
end

function clsUser:SetMissionOver(missionId)
	self.__save.mission.doneList = self.__save.mission.doneList or {}
	if CLSMISS.IsDailyMission(missionId) then
		self.__save.mission.doneList["daily"] = missionId
	else
		self.__save.mission.doneList[missionId] = os.time()
	end
end

function clsUser:ClearDailyMission()
	self.__save.mission.doneList = self.__save.mission.doneList or {}
	self.__save.mission.doneList["daily"] = nil
end


--完成任务
function clsUser:FinishMission(missionId)
	local misObj = self:GetMissionById(missionId)
	if not misObj then
		return false, "您没有该任务"
	end
	misObj:SendReward(self)
	if misObj:GetType() == "daily" then
		self:ModDailyMissionState(CLSMISS.ALLDONE)
		if self:GetDailyMissReceiveTimes() < self:GetDailyMissMax() then
			DAILYMISS.FreshDailyMission(self)
		end
	end
	self:SetMissionOver(missionId)
	if misObj:GetType() == "status" then
		local status = self:GetStatus()
		if status then
			CHAT.SendSysMsg("system", string.format("【%s】通关了副本【%s】，即将达成【%s】仙职。", self:GetName(), misObj:GetName(), misObj:GetName()))
			status:CheckUpgradeState()
		end
	end
	self:SetMissionById(missionId)
	misObj:Destroy()
	self:ClearMissionItem(missionId)
	local info = CLSMISS.GetMissionInfoById(misObj:GetTemplate() or missionId)
	if info.type == "daily" and not info.next then		--没有下一环则清除日常任务记录
		self:ClearDailyMission()
	elseif info.type == "master" then
		local nextInfo = CLSMISS.GetMissionInfoById(info.next)
		if nextInfo and nextInfo.autoRecv then
			local nextMisObj = self:AddMission(info.next)
			if info.next == info.chapter then
				nextMisObj:SetMissionFinish(self)
			end
		end
	elseif info.type == "missKind" then
		self:ModMissKindState(missionId, CLSMISS.ALLDONE)
	end
	return true
end

--修改任务实现详情
function clsUser:ModMissionDetail(missionId, id, num)
	local misObj = self:GetMissionById(missionId)
	if misObj then
		if num then
			misObj:ModMissionDetail(self, id, num)
		end
		misObj:CheckMissionOver(self)
	end
end



function clsUser:SetCurDailyMission(data)
	assert(type(data) == "table")
	self.__save.mission.dailyMiss = data
end

function clsUser:GetCurDailyMission()
	return self.__save.mission.dailyMiss
end

function clsUser:ModDailyMissionState(state)
	local mis = self:GetCurDailyMission()
	if not mis then return end
	mis.state = state
end

--放弃任务
function clsUser:CancelMission(id)
	local misObj = self:GetMissionById(id)
	if not misObj then
		return
	end
	if misObj:GetType() == "daily" then
		DAILYMISS.FreshDailyMission(self)
	elseif misObj:GetType() == "missKind" then
		self:ModMissKindState(id, CLSMISS.UNSTARTED)
	end
	self:SetMissionById(id)
	misObj:Destroy()
end

--清理所有的任务数据
function clsUser:ClearAllMission()
	for k, v in pairs(self.__temp.mission.list) do
		self:CancelMission(k)
	end
	self.__temp.mission = {}
	self.__save.mission = {}
	self:AddMission(INIT_MASTER_MISSION_ID)
	self:RenewDailyMission()
end

--保存任务数据
function clsUser:SaveMissionData()
	self.__save.mission.list = {}
	for k, v in pairs(self:GetMissionList()) do
		self.__save.mission.list[k] = v:GetSave()
	end
end

--恢复任务
function clsUser:RestoreMissionData()
	self.__temp.mission.list = {}
	for k, v in pairs(self.__save.mission.list or {}) do
		local class = MISSION.GetMIssionClassById(v.template or k)
		if class then
			local misObj = class:New()
			misObj:Restore(v)
			self.__temp.mission.list[k] = misObj
		end		
	end
end


--日常任务刷新时间
function clsUser:SetDailyMissFreshTime(data)
	self.__save.mission.dailyFreshTime = data or os.time()
end

function clsUser:GetDailyMissFreshTime()
	return self.__save.mission.dailyFreshTime or os.time()
end

--每日凌晨刷新日常任务 (flag 强制刷新)
function clsUser:RenewDailyMission(flag)
	if not self:IsFuncOpen(SETTING.FUNC_DAILYMISSION) then
		return
	end
	if not flag then
		local timestamp = self:GetDailyMissFreshTime()
		if timestamp and CalDiffDays(timestamp, os.time()) == 0 then
			return
		end
	end
	local mis = self:GetCurDailyMission()
	if mis then
		if self:GetMissionById(mis.missionId) then
			self:CancelMission(mis.missionId)
		end
	end
	DAILYMISS.FreshDailyMission(self)
end

-----------------------------------------------------------------
--每日任务完成次数
function clsUser:GetDailyMissMax()
	local times = VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_DAILY_MISS_NUM_ADD)
	return MAX_DAILY_MISSION_NUM + (times or 0)
end

--每日领取日常任务次数
function clsUser:GetDailyMissReceiveTimes()
	self.__save.mission.dailyReceive = self.__save.mission.dailyReceive or {}
	local info = self.__save.mission.dailyReceive
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:SetDailyMissReceiveTimes(data)
	self.__save.mission.dailyReceive = self.__save.mission.dailyReceive or {}
	local info = self.__save.mission.dailyReceive
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = (info.num or 0) + data
	end
	info.time = os.time()
end

--每日完成日常任务次数
function clsUser:GetDailyMissFinishTimes()
	self.__save.mission.dailyFinish = self.__save.mission.dailyFinish or {}
	local info = self.__save.mission.dailyFinish
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end


function clsUser:SetDailyMissFinishTimes(data)
	self.__save.mission.dailyFinish = self.__save.mission.dailyFinish or {}
	local info = self.__save.mission.dailyFinish
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = (info.num or 0) + data
	end
	info.time = os.time()
end

-------------------------------------------------------
--日行一善

--刷新时间
function clsUser:SetMissKindFreshTime(data)
	self.__save.mission.missKindFreshTime = data or os.time()
end

function clsUser:GetMissKindFreshTime()
	return self.__save.mission.missKindFreshTime
end

function clsUser:SetMissKindList(data)
	assert(type(data) == "table")
	self.__save.mission.missKindList = data
end

function clsUser:GetMissKindList()
	self.__save.mission.missKindList = self.__save.mission.missKindList or {}
	return self.__save.mission.missKindList
end

function clsUser:ModMissKindState(index, state)
	for k, v in pairs(self:GetMissKindList()) do
		if v.index == index then
			v.state = state
		end
	end
end
