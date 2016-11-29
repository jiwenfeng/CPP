--玩家副本扩展

local DAILY_TEAM_COPY_NUM = 3
DAILY_HERO_COPY_NUM = 5

local clsUser = USERBASE.clsUser

function clsUser:InitCopy()
	self.__save.town = {}
	self.__save.town[USERBASE.INIT_SCENE] = 1
end

function clsUser:IsCopyPassed(id)
	id = tostring(id)
	return self.__save.copy[id]
end

function clsUser:SetCopyPassed(id)
	self.__save.copy[tostring(id)] = 1
	local townInfo = SCENE.GetTownInfoByCopyId(id)
	if tostring(townInfo.maxCopyId) == id and townInfo.nextTown then
		self:SetTownOpen(townInfo.nextTown)
	end
end

function clsUser:IsCopyOpen(id)
	local info = CLSCOPY.GetCopyInfoById(id)
	if self:GetLv() < info.needLv then
		return false
	end
	if info.needMissionId and not self:GetMissionById(info.needMissionId) and not self:IsMissionOver(info.needMissionId) then
		return false
	end
	return true
end

function clsUser:SetTownOpen(townId)
	townId = tostring(townId)
	self.__save.town = self.__save.town or {}
	self.__save.town[townId] = 1
end

function clsUser:IsTownOpened(townId)
	townId = tostring(townId)
	self.__save.town = self.__save.town or {}
	return self.__save.town[townId]
end

function clsUser:GetOpenTownList()
	return self.__save.town
end

--当前进入的副本对象
function clsUser:SetCurCopy(copyObj)
	self.__temp.copy = copyObj
end

function clsUser:GetCurCopy()
	return self.__temp.copy
end

function clsUser:LeaveCopy(copyObj)
--	local roleId = self:GetRoleId()
--	local type = copyObj:GetType()
	local workInfo = self:GetWorkInfo()
	if workInfo.state == "fighting" then
		local fight = FIGHT.GetOneFight(workInfo.place)
		if fight then
			fight:SetFastFightState(true)
		else
			self:SetWorkInfo("standby")
		end
	end
	copyObj:DestroyCopy()
--[[	if copyObj:IsCopyPassed() then
		if not copyObj:IsPassExpGot() then
			local info = CLSCOPY.GetCopyInfoById(copyObj:GetId())
			self:SetTeamExp(info.exp)
			copyObj:SetPassExpGot(true)
		end
		local itemList = COPY.GetCopyPrize(copyObj:GetId(), copyObj:GetClickMaxTimes(roleId) - copyObj:GetClickTimes(roleId))
		for k, v in pairs(itemList or {}) do
			local item = BASEITEM.NewItem(v.id, v.num)
			self:AddItem(USERBASE.PACKAGE, item)
		end
		if type == "normal" or type == "hero" then
			self:SetCopyPassed(copyObj:GetId())
		end
	end]]
	STATS.SaveCopyHistory(copyObj:GetId(), self:GetRoleId(), 0, copyObj:IsCopyPassed() and 1 or 0)
	self:SetCurCopy()
--	copyObj:DestroyCopy()
--	self:SetCurCopy()
end

function clsUser:SetCurMopup(mopup)
	self.__save.copy = self.__save.copy or {}
	self.__save.copy.mopup = mopup
end

function clsUser:SetCurMopupCopy(obj)
	self.__temp.mopup = obj
end

function clsUser:GetCurMopupCopy()
	return self.__temp.mopup
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
	if not mopup or mopup.state == COPY.STATE_FINISH then
		return
	end
	local copy = COPY.NewCopy(mopup.copyId, self:GetRoleId(), true)
	self:SetCurMopupCopy(copy)
	while mopup and mopup.state == COPY.STATE_DOING do
		local timelong = mopup.timestamp - os.time()
		if timelong > 0 then
			local scene = SCENE.GetEmptyScene("copyMopup")
			self:EnterScene(scene, 0, 0)
			return copy:StartMopupCallOut(timelong)
		end
		copy:ProMopup("login")
	end
end

function clsUser:RemoveCopyMopupCallOut()
	local copy = self:GetCurMopupCopy()
	if copy then
		copy:RemoveMopupCallOut()
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
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_TEAMCOPY_BUY_TIMES) 
end

function clsUser:SetHeroCopyBuyTimes(data)
	self.__save.stats.heroCopy = self.__save.stats.heroCopy or {}
	self.__save.stats.heroCopy["buy"] = self.__save.stats.heroCopy["buy"] or {}
	if not self.__save.stats.heroCopy["buy"].timestamp or CalDiffDays(self.__save.stats.heroCopy["buy"].timestamp, os.time()) ~= 0 then
		self.__save.stats.heroCopy["buy"].times = 0
	end
	self.__save.stats.heroCopy["buy"].times = math.max(self.__save.stats.heroCopy["buy"].times + data, 0)
	self.__save.stats.heroCopy.timestamp = os.time()
end

function clsUser:GetHeroCopyBuyTimes()
	self.__save.stats.heroCopy = self.__save.stats.heroCopy or {}
	self.__save.stats.heroCopy["buy"] = self.__save.stats.heroCopy["buy"] or {}
	if not self.__save.stats.heroCopy["buy"].timestamp or CalDiffDays(self.__save.stats.heroCopy["buy"].timestamp, os.time()) ~= 0 then
		self.__save.stats.heroCopy["buy"].times = 0
	end
	return self.__save.stats.heroCopy["buy"].times
end

function clsUser:GetDailyHeroCopyMaxTimes()
	return user:GetLv() / 10 + self:GetHeroCopyBuyTimes() + DAILY_HERO_COPY_NUM
end

function clsUser:SetDailyHeroCopyUseTimes(data)
	self.__save.stats.heroCopy["use"] = self.__save.stats.heroCopy["use"] or {}
	if not self.__save.stats.heroCopy["use"].timestamp or CalDiffDays(self.__save.stats.heroCopy["use"].timestamp, os.time()) ~= 0 then
		self.__save.stats.heroCopy["use"].times = 0
	end
	self:SetFuncStatById(SETTING.FUNC_HERO_COPY, data)
	self.__save.stats.heroCopy["use"].times = math.max(self.__save.stats.heroCopy["use"].times + adta, 0)
	self.__save.stats.heroCopy["use"].timestamp = os.time()
end

function clsUser:GetDailyHeroCopyUseTimes()
	self.__save.stats.heroCopy["use"] = self.__save.stats.heroCopy["use"] or {}
	if not self.__save.stats.heroCopy["use"].timestamp or CalDiffDays(self.__save.stats.heroCopy["use"].timestamp, os.time()) ~= 0 then
		self.__save.stats.heroCopy["use"].times = 0
	end
	return self.__save.stats.heroCopy["use"].times
end

function clsUser:AddCopyItem(itemList, copyName)
	if type(itemList) ~= "table" then
		return
	end
	local list = {}
	for _, v in ipairs(itemList) do
		local item = BASEITEM.NewItem(v.goodsId, v.num, {missionId = v.missionId})
		if self:IsItemCanAdd(USERBASE.PACKAGE, item) then
			self:AddItem(USERBASE.PACKAGE, item)
		else
			table.insert(list, {id = v.goodsId, num = v.num, missionId = v.missionId})
		end
	end
	if #list > 0 then
		local mail = {}
		mail.title = "副本掉落物品"
		mail.content = string.format("亲，由于背包已满，您在副本【%s】中获得的奖励已发送到邮箱，请及时领取。", copyName)
		mail.attach = {}
		mail.attach.item = list
		MAIL.SendSysMail(self:GetRoleId(), mail)
		self:SendProtocol("c_show_tips", string.format("由于背包已满，您在副本【%s】中获得的奖励已发送到邮箱，请及时领取。", copyName))
	end
end

function clsUser:SetTeam(obj)
	self.__temp.team = {}
	self.__temp.team = obj
end

function clsUser:GetTeam()
	return self.__temp.team
end

function clsUser:DestroyTeam()
	local obj = self:GetTeam()
	if obj then
		obj:Destroy()
		self:SetTeam()
	end
end
