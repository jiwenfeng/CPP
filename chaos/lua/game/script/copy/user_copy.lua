--玩家副本扩展

local DAILY_TEAM_COPY_NUM = 3
DAILY_HERO_COPY_NUM = 5


local clsUser = USERBASE.clsUser

function clsUser:InitCopyInfo()
	self.__save.copy = {}
	for k in pairs(BASECOPY.GetAllCopyList()) do
		self.__save.copy[k] = {}
		self.__save.copy[k].passed = {}
		if k == "normal" then			
			self.__save.copy[k].town = {[USERBASE.INIT_SCENE] = 1}
		end
	end
end


function clsUser:SetCopyInfo(copyId)
	
end

function clsUser:GetCopyInfo(type)
	if not self.__save.copy then return end
	return self.__save.copy[type]
end
--已通关的副本
function clsUser:GetCopyPassed(type)
	local info = self:GetCopyInfo(type)
	if info then
		return self.__save.copy[type].passed
	end
end

function clsUser:SetCopyPassed(type, id)
	local info = self:GetCopyInfo(type)
	if info then
		info.passed[id] = 1
		local locate = self:GetLastLocateInfo()
		local townInfo = SCENE.GetSceneInfoById("town", locate.scene)
		if tostring(townInfo.maxCopyId) == id and townInfo.nextTown then
			self:SetTownOpen(townInfo.nextTown)
		end
	end
end

function clsUser:IsCopyPassed(type, id)
	local info = self:GetCopyInfo(type)
	if not info then return false end
	return info.passed[id]
end

--开放的城镇
function clsUser:GetOpenedTownList()
	local info = self:GetCopyInfo("normal")
	if not info then return end
	return info.town
end

function clsUser:SetTownOpen(townId)
	local info = self:GetCopyInfo("normal")
	if not info or not townId then return end
	info.town[tostring(townId)] = 1
end

function clsUser:IsTownOpened(townId)
	townId = tostring(townId)
	local info = self:GetCopyInfo("normal")
	if not info then return end
	return info.town[townId]
end

--当前进入的副本对象
function clsUser:SetCurCopy(copyObj)
	self.__temp.copy = copyObj
end

function clsUser:GetCurCopy()
	return self.__temp.copy
end


function clsUser:LeaveCopy(copyObj)
	local roleId = self:GetRoleId()
	local type = copyObj:GetType()
	local workInfo = self:GetWorkInfo()
	if workInfo.state == "fighting" then
		local fight = FIGHT.GetOneFight(workInfo.place)
		if fight then
			fight:SetFastFightState(true)
		else
			self:SetWorkInfo("standby")
		end
	end
	if copyObj:IsCopyPassed() then
--		copyObj:CopyPassed()
		if not copyObj:IsPassExpGot() then
			local info = BASECOPY.GetCopyInfoById(copyObj:GetId())
			self:SetTeamExp(info.exp)
			copyObj:SetPassExpGot(true)
		end
		local itemList = COPY.GetCopyPrize(copyObj:GetId(), copyObj:GetClickMaxTimes(roleId) - copyObj:GetClickTimes(roleId))
		for k, v in pairs(itemList or {}) do
			local item = BASEITEM.NewItem(v.id, v.num)
			self:AddItem(USERBASE.PACKAGE, item)
		end
		if type == "normal" or type == "hero" then
			self:SetCopyPassed(copyObj:GetType(), copyObj:GetId())
		end
	end
	STATS.SaveCopyHistory(copyObj:GetId(), roleId, 0, copyObj:IsCopyPassed() and 1 or 0)
	copyObj:DestroyCopy()
	self:SetCurCopy()
end


function clsUser:SetCurMopup(mopup)
	self.__save.copy = self.__save.copy or {}
	self.__save.copy.mopup = mopup
end

function clsUser:GetCurMopup()
	self.__save.copy = self.__save.copy or {}
	return self.__save.copy.mopup
end

function clsUser:GetCopyMopupTimestamp()
	local mopup = self:GetCurMopup()
	if not mopup then
		return
	end
	return mopup.timestamp
end

function clsUser:StartCopyMopupCallOut()
	local mopup = self:GetCurMopup()
	while mopup and mopup.state == 1 do
		local timelong = mopup.timestamp - os.time()
		if timelong > 0 then
			break
		end
		COPY.ProMopup(self, "login")
	end
	if mopup and mopup.state == 1 then
		self.__temp.mopupTimer = CALLOUT.ObjCallOut(self:GetCallOut(), COPY.ProMopup, mopup.timestamp - os.time(), self)
	end
end

function clsUser:RemoveCopyMopupCallOut()
	if self.__temp.mopupTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.mopupTimer)
	end
end

function clsUser:SetMopupGot(exp, nimbus, itemList)
	local mopup = self:GetCurMopup()
	if not mopup then return end
	mopup.stats = mopup.stats or {}
	mopup.stats.exp = (mopup.stats.exp or 0) + (exp or 0)
	mopup.stats.nimbus = (mopup.stats.nimbus or 0) + (nimbus or 0)
	mopup.stats.itemList = mopup.stats.itemList or {}
	if itemList then
		for k, v in pairs(itemList) do
			BASEITEM.CombiItemToList(mopup.stats.itemList, v.goodsId, v.num)
		end
	end
end

function clsUser:GetMopupGot()
	local mopup = self:GetCurMopup()
	if not mopup then return end
	mopup.stats = mopup.stats or {}
	return mopup.stats
end

--当前队伍
function clsUser:SetCurTeam(teamObj)
	self.__temp.curTeam = teamObj
end

function clsUser:GetCurTeam()
	return self.__temp.curTeam
end


--每日组队副本完成次数
function clsUser:SetDailyTeamCopyNum(data)
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
		info.max = DAILY_TEAM_COPY_NUM
		info.buy = 0
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUser:GetDailyTeamCopyNum()
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end
--每日组队副本最大可完成次数
function clsUser:SetDailyTeamCopyMax(data)
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = 0
		info.max = DAILY_TEAM_COPY_NUM + data
		info.buy = 0
	else
		info.max = (info.max or DAILY_TEAM_COPY_NUM) + data
	end
	info.time = os.time()
end

function clsUser:GetDailyTeamCopyMax()
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return DAILY_TEAM_COPY_NUM
	end
	return info.max or DAILY_TEAM_COPY_NUM
end
--每日组队副本购买次数
function clsUser:SetDailyTeamCopyBuyTimes(data)
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = 0
		info.max = DAILY_TEAM_COPY_NUM
		info.buy = data
	else
		info.buy = (info.buy or 0) + data
	end
	info.time = os.time()
end

function clsUser:GetDailyTeamCopyBuyTimes()
	self.__save.stats.teamCopy = self.__save.stats.teamCopy or {}
	local info = self.__save.stats.teamCopy
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.buy or 0
end
--每日组队副本可购买次数
function clsUser:GetDailyTeamCopyMaxBuyTimes()
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_TEAMCOPY_BUY_TIMES) or 0
end


--武将副本挑战次数	--type: num次数 max最大值
function clsUser:SetDailyHeroCopyTimes(type, data)
	self.__save.stats.heroCopy = self.__save.stats.heroCopy or {}
	self.__save.stats.heroCopy[type] = self.__save.stats.heroCopy[type] or {}
	local info = self.__save.stats.heroCopy[type]
	if type == "num" then
		self:SetFuncStatById(SETTING.FUNC_HERO_COPY, data)
	end
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = (type == "num" and 0 or DAILY_HERO_COPY_NUM) + data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUser:GetDailyHeroCopyTimes(type)
	self.__save.stats.heroCopy = self.__save.stats.heroCopy or {}
	self.__save.stats.heroCopy[type] = self.__save.stats.heroCopy[type] or {}
	local info = self.__save.stats.heroCopy[type]
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return type == "num" and 0 or DAILY_HERO_COPY_NUM
	end
	return info.num
end


