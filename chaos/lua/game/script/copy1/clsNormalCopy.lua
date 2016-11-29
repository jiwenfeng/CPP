
--普通副本
clsNormalCopy = CLSCOPY.clsCopy:Inherit()
function clsNormalCopy:__init__()
	Super(clsNormalCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
end

function clsNormalCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.callout = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
	self.__temp.dialogOver = {}
	self.__temp.stats = {}
end

function clsNormalCopy:GetType()
	return "normal"
end

function clsNormalCopy:GetCallOut()
	return self.__temp.callout
end

--flag true 不生成副本场景
function clsNormalCopy:InitCopyInfo(id, owner, flag)
	Super(clsNormalCopy).InitCopyInfo(self, id, owner, flag)
	if not flag then
		self:InitGroundList()
	end
end

function clsNormalCopy:Enter()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if not user:IsCopyOpen(self:GetId()) then
		return false, "该副本尚未开启"
	end
	if user:GetVigour() < self:GetEnterVigour() then
		return false, "您体力不足"
	end
	user:SetVigour(-self:GetEnterVigour())
	user:SetCurCopy(self)
	return true
end

function clsNormalCopy:ProFightResult(npcObj, fightId, winner, prize, stats)
	print("ProFightResult:", self:GetOwner(), self.__temp.id, self.__temp.owner)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:SetWorkInfo("standby")
	npcObj:SetState()
	if winner == "attack" then
		local teamInfo = FIGHT.GetMonsterTeamInfoById(npcObj:GetId())
		for _, v in ipairs(teamInfo.list) do
			MISSION.CheckMissionEvent(user, "fight", v.id, 1)			--打怪任务
		end
		NPC.NpcLeave(npcObj)
		self:DelCopyMonster(npcObj:GetRoleId())
		if prize.exp then
			user:SetTeamExp(prize.exp)
		end
		if prize.nimbus then
			user:SetNimbus(prize.nimbus)
		end
		if prize.item then
			user:AddCopyItem(prize.item, self:GetName())
		end
		self:SetFoeHpSum(stats.defHpSum)
		self:SetDamageSum(stats.atkDamage)
		self:SetMyHpSum(stats.atkHpSum)
		self:SetMyHpLeft(stats.atkHp)
		if #self:GetCopyMonsterList() <= 0 then
			self:PassCopy()
		end
	else
		local location = user:GetLocateInfo()
		user:Goto(math.max(location.x - 250, 150), location.y)
	end
end

function clsNormalCopy:InitGroundList()
	self.__temp.ground = {}
	local num = math.random(1, 3)
	local locateList = {{x = 700, y = 500}, {x = 1500, y = 500}, {x = 2050, y = 500}}
	local lv = USER.GetUserNickInfo(self:GetOwner(), "Lv")
	for i = 1, num do
		local pos = math.random(1, #locateList)
		self.__temp.ground[i] = {type = math.random(0, 1) == 1 and "gold" or "nimbus", locate = locateList[i], num = (lv + 20) * 2 * math.random(6, 11)}
		table.remove(locateList, pos)
	end
end

function clsNormalCopy:GetEnterVigour()
	return COPY.NORMAL_COPY_COST_VIGOUR
end

function clsNormalCopy:GetFastDoneNeedLv()
	return COPY.NORMAL_FAST_MOPUP
end

function clsNormalCopy:GetMopupLv()
	return COPY.MOPUP_MIN_LV
end

function clsNormalCopy:GetMopupCost()
	return COPY.MOPUP_COST_GOLD
end

function clsNormalCopy:SetGroundItem(id, item)
	self.__temp.ground[id] = item
end

function clsNormalCopy:RemoveGroundItem(id)
	self.__temp.ground = self.__temp.ground or {}
	self.__temp.ground[id] = nil
end

function clsNormalCopy:GetGroundItemById(id)
	self.__temp.ground = self.__temp.ground or {}
	return self.__temp.ground[id]
end

function clsNormalCopy:GetGroundItemList()
	return self.__temp.ground
end

--是否剧情对话已经发生过
function clsNormalCopy:SetDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	self.__temp.dialogOver[data] = 1
end

function clsNormalCopy:IsDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	return self.__temp.dialogOver[data]
end

--怪物血量总和
function clsNormalCopy:SetFoeHpSum(hp)
	self.__temp.stats.foeHp = (self.__temp.stats.foeHp or 0) + hp
end

function clsNormalCopy:GetFoeHpSum()
	return self.__temp.stats.foeHp or 0
end
--伤害总和
function clsNormalCopy:SetDamageSum(damage)
	self.__temp.stats.damage = (self.__temp.stats.damage or 0) + damage
end

function clsNormalCopy:GetDamageSum()
	return self.__temp.stats.damage or 0
end

--我方血量总和
function clsNormalCopy:SetMyHpSum(hp)
	self.__temp.stats.myHpSum = (self.__temp.stats.myHpSum or 0) + hp
end

function clsNormalCopy:GetMyHpSum()
	return self.__temp.stats.myHpSum or 0
end

--我方剩余血量
function clsNormalCopy:SetMyHpLeft(hp)
	self.__temp.stats.myHpLeft = (self.__temp.stats.myHpLeft or 0) + hp
end

function clsNormalCopy:GetMyHpLeft()
	return self.__temp.stats.myHpLeft or 0
end

function clsNormalCopy:CalCopyScore()
	local ret = {}
	ret.atkScore = CLSCOPY.GetAttackScore(self:GetFoeHpSum(), self:GetDamageSum())
	ret.defScore = CLSCOPY.GetDefenceScore(self:GetMyHpLeft(), self:GetMyHpSum())
	ret.rate = CLSCOPY.GetAddRateByScore(ret.atkScore + ret.defScore)
	return ret
end

function clsNormalCopy:PassCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:SetCopyPassed()
	user:SetCopyPassed(self:GetId())
	self:SendCopyPassed()		--发送副本通关提示
	user:SetFuncStatById(SETTING.FUNC_NORMAL_COPY, 1)
end

function clsNormalCopy:GetPassCopyExp()
	local ret = self:CalCopyScore()
	local sData = {}
	sData.atkScore = ret.atkScore
	sData.defScore = ret.defScore
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	sData.baseExp = info.exp
	sData.addExp = math.ceil(info.exp * (ret.rate or 1))
	return sData
end

function clsNormalCopy:SendCopyStats()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = self:GetPassCopyExp()
	user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
	self:SetPassExpGot(true)
	user:SendProtocol("c_copy_stats", sData)
end

function clsNormalCopy:pickup(data)
	local info = GetGroundItemById(id)
	if not info then
		return false, "该位置没有可拾取的物品"
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local moneyInfo = MONEY.GetGoldInfoByType(info.type)
	USERBASE.clsUser[moneyInfo.set](self, info.num)
	self:RemoveGroundItem(id)
	return true
end

function clsNormalCopy:CheckMopupCondt(times)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if user:GetLv() < self:GetMopupLv() then
		return false, "等级不足"
	end
	if not user:IsCopyPassed(self:GetId()) then
		return false, "只能扫荡已通关的副本"
	end
	local mopup = user:GetCurMopup()
	if mopup and mopup.state == COPY.STATE_DOING then
		return false, "您正在进行副本扫荡"
	end
	local tribe = user:GetTribe()
	if tribe then
		local tower = tribe:GetStruct("retreat")
		if tower and tower:GetMopup() then
			return false, "您正在进行修仙塔扫荡"
		end
	end
	if user:GetVigour() < times * self:GetEnterVigour() then
		return false, "精力不足"
	end
	if user:GetGold() < times * self:GetMopupCost() then
		return false, "钱币不足"
	end
	if user:GetGridMax(USERBASE.PACKAGE) - user:GetItemGridNum(USERBASE.PACKAGE) < COPY.MOPUP_NEED_GRID then
		return false, "背包空间不足"
	end
	local scene = SCENE.GetEmptyScene("copyMopup")
	user:EnterScene(scene, 0, 0)
	user:SetCurMopupCopy(self)
	return true
end

function clsNormalCopy:StartMopupCallOut(timelong)
	timelong = timelong or COPY.MOPUP_TIMELONG
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local mopup = user:GetCurMopup()
	if mopup and mopup.state == COPY.STATE_DOING then
		self.__temp.mopupTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsNormalCopy.ProMopup, timelong, self)
	end
end

function clsNormalCopy:RemoveMopupCallOut()
	if self.__temp.mopupTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.mopupTimer)
	end
end

function clsNormalCopy:CopyMopup(times)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local buf = {}
	buf.copyId = self:GetId()
	buf.max = times
	buf.num = 1
	buf.state = COPY.STATE_DOING
	buf.timestamp = os.time() + COPY.MOPUP_TIMELONG
	user:SetCurMopup(buf)
	user:SetVigour(-self:GetEnterVigour())
	user:SetGold(-self:GetMopupCost())
	user:SendProtocol("c_copy_mopup", {type = self:GetType(), copyId = self:GetId(), num = times, state = STATE_DOING, fastIngot = COPY.FAST_MOPUP_INGOT})
	if VIP.IsVipFuncValid(user:GetVipLv(), VIP.NORMAL_COPY_MOPUP_FINISH) or user:GetLv() >= self:GetFastDoneNeedLv() then
		return self:FastMopup()
	end
	self:StartMopupCallOut()
end

function clsNormalCopy:ProMopup(type)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	local mopup = user:GetCurMopup()
	self:RemoveMopupCallOut()
	local done = {times = mopup.num}
	user:SetFuncStatById(SETTING.FUNC_NORMAL_COPY, 1)
	for k, v in pairs(info.monsterList) do
		if v.exp then
			done.exp = (done.exp or 0) + v.exp
			user:SetTeamExp(v.exp, "copy")
		end
		if v.nimbus then
			done.nimbus = (done.nimbus or 0) + v.nimbus
			user:SetNimbus(v.nimbus, "copy")
		end
		done.itemList = done.itemList or {}
		for _, info in pairs(v.itemList or {}) do
			if info.needMissionId and user:IsMissionAdd(info.needMissionId) then
				BASEITEM.CombiItemToList(done.itemList, info.id, math.random(info.min, info.max), info.needMissionId)
			else
				BASEITEM.CombiItemToList(done.itemList, info.id, math.random(info.min, info.max))
			end
		end
		local monsterList = FIGHT.GetMonsterTeamInfoById(v.teamId)
		for k, v in pairs(monsterList.list) do
			MISSION.CheckMissionEvent(user, "fight", v.id, 1)
		end
	end
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	done.exp = (done.exp or 0) + info.exp
	user:SetTeamExp(info.exp, "copy")
	user:AddCopyItem(done.itemList, self:GetName())
	user:SetMopupGot(done.exp, done.nimbus, done.itemList)
	self:SendMopupUpdate(done)
	if mopup.num >= mopup.max then
		if type ~= "login" then
			self:SendMopupFinish()
			user:SetCurMopup()
			user:SetCurMopupCopy()
		end
	else
		user:SetVigour(-self:GetEnterVigour())
		user:SetGold(-self:GetMopupCost())
		mopup.timestamp = mopup.timestamp + COPY.MOPUP_TIMELONG
		if not type then
			self:StartMopupCallOut()
		end
	end
	mopup.num = mopup.num + 1
	STATS.SaveCopyHistory(self:GetId(), user:GetRoleId(), 1, 1)
end

function clsNormalCopy:SendMopupUpdate(done)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	local mopup = user:GetCurMopup()
	sData.state = mopup.state
	sData.done = done
	sData.curTimes = mopup.num
	sData.timelong = mopup.timestamp
	user:SendProtocol("c_copy_mopupUpdate", sData)
end

function clsNormalCopy:SendMopupFinish()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local got = user:GetMopupGot()
	local sData = {}
	sData.expGot = got.exp
	sData.nimbus = got.nimbus
	sData.itemGot = got.itemList
	local mopup = user:GetCurMopup()
	sData.state = mopup.state
	user:SendProtocol("c_copy_mopupFinish", sData)
end

function clsNormalCopy:FastMopup()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:RemoveMopupCallOut()
	local mopup = user:GetCurMopup()
	if not mopup or mopup.state == COPY.STATE_FINISH then
		return
	end
	while(mopup.num <= mopup.max) do
		self:ProMopup("fast")
	end
	return true
end

function clsNormalCopy:StopMopup()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:RemoveMopupCallOut()
	user:SetCurMopup()
	user:LeaveScene()
	user:SetLocateInfo()
	user:SetCurMopupCopy()
	return true
end

function clsNormalCopy:DestroyCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if self:IsCopyPassed() then
		if not self:IsPassExpGot() then
			local sData = self:GetPassCopyExp()
			if user then
				user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
			else
				OFFLINE.SetOfflineData(self:GetOwner(), "setTeamExp", sData.baseExp + sData.addExp)
			end
			self:SetPassExpGot(true)
		end
		if user then
			user:SetCopyPassed(self:GetId())
--		else
--			OFFLINE.SetOfflineData(self:GetOwner(), "setCopyPassed", self:GetId())
		end
	end
	Super(clsNormalCopy).DestroyCopy(self)
end
