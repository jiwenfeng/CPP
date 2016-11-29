--英魂副本
clsHeroCopy = CLSCOPY.clsCopy:Inherit()
function clsHeroCopy:__init__()
	Super(clsHeroCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
end

function clsHeroCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.callout = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
	self.__temp.dialogOver = {}
	self.__temp.stats = {}
end

--flag true 不生成副本场景
function clsHeroCopy:InitCopyInfo(id, owner, flag)
	Super(clsHeroCopy).InitCopyInfo(self, id, owner, flag)
	if not flag then
		self:InitGroundList()
	end
end

function clsHeroCopy:GetType()
	return "hero"
end

function clsHeroCopy:GetCallOut()
	return self.__temp.callout
end

function clsHeroCopy:Enter()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if not user:IsCopyOpen(self:GetId()) then
		return false, "该副本未开启"
	end
	if user:GetDailyHeroCopyUseTimes() >= user:GetDailyHeroCopyMaxTimes() then
		return false, "已无剩余次数"
	elseif user:GetVigour() < self:GetEnterVigour() then
		return false, "体力不足"
	end
	user:SetDailyHeroCopyUseTimes(data)
	user:SetVigour(-self:GetEnterVigour())
	user:SetCurCopy(self)
	return true
end

function clsHeroCopy:GetEnterVigour()
	return COPY.FAMOUS_COPY_COST_VIGOUR
end

function clsHeroCopy:GetFastDoneNeedLv()
	return COPY.HERO_FAST_MOPUP 
end

function clsHeroCopy:GetMopupLv()
	return COPY.MOPUP_MIN_LV
end

function clsHeroCopy:GetMopupCost()
	return COPY.MOPUP_COST_GOLD
end

function clsHeroCopy:ProFightResult(npcObj, fightId, winner, prize, stats)
	local user = USER.GetOnlineUser(self:GetOwner())
	if not user then
		return self:DestroyCopy()
	end
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

--是否剧情对话已经发生过
function clsHeroCopy:SetDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	self.__temp.dialogOver[data] = 1
end

function clsHeroCopy:IsDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	return self.__temp.dialogOver[data]
end

--怪物血量总和
function clsHeroCopy:SetFoeHpSum(hp)
	self.__temp.stats.foeHp = (self.__temp.stats.foeHp or 0) + hp
end

function clsHeroCopy:GetFoeHpSum()
	return self.__temp.stats.foeHp or 0
end
--伤害总和
function clsHeroCopy:SetDamageSum(damage)
	self.__temp.stats.damage = (self.__temp.stats.damage or 0) + damage
end

function clsHeroCopy:GetDamageSum()
	return self.__temp.stats.damage or 0
end

--我方血量总和
function clsHeroCopy:SetMyHpSum(hp)
	self.__temp.stats.myHpSum = (self.__temp.stats.myHpSum or 0) + hp
end

function clsHeroCopy:GetMyHpSum()
	return self.__temp.stats.myHpSum or 0
end

--我方剩余血量
function clsHeroCopy:SetMyHpLeft(hp)
	self.__temp.stats.myHpLeft = (self.__temp.stats.myHpLeft or 0) + hp
end

function clsHeroCopy:GetMyHpLeft()
	return self.__temp.stats.myHpLeft or 0
end

function clsHeroCopy:CalCopyScore()
	local ret = {}
	ret.atkScore = CLSCOPY.GetAttackScore(self:GetFoeHpSum(), self:GetDamageSum())
	ret.defScore = CLSCOPY.GetDefenceScore(self:GetMyHpLeft(), self:GetMyHpSum())
	ret.rate = CLSCOPY.GetAddRateByScore(ret.atkScore + ret.defScore)
	return ret
end

function clsHeroCopy:PassCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:SetCopyPassed()
	user:SetCopyPassed(self:GetId())
	self:SendCopyPassed()		--发送副本通关提示
	user:SetFuncStatById(SETTING.FUNC_HERO_COPY, 1)
end

function clsHeroCopy:SendCopyStats()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = self:GetPassCopyExp()
	user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
	self:SetPassExpGot(true)
	user:SendProtocol("c_copy_stats", sData)
end

function clsHeroCopy:InitGroundList()
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

function clsHeroCopy:SetGroundItem(item)
	self.__temp.ground = item
end

function clsHeroCopy:RemoveGroundItem(id)
	self.__temp.ground = self.__temp.ground or {}
	self.__temp.ground[id] = nil
end

function clsHeroCopy:GetGroundItemById(id)
	self.__temp.ground = self.__temp.ground or {}
	return self.__temp.ground[id]
end

function clsHeroCopy:GetGoundItemList()
	return self.__temp.ground
end

function clsHeroCopy:pickup(data)
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

function clsHeroCopy:CheckMopupCondt(times)
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
	if times > user:GetDailyHeroCopyMaxTimes() - user:GetDailyHeroCopyUseTimes() then
		return false, "没有足够的剩余次数"
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

function clsHeroCopy:StartMopupCallOut(timelong)
	timelong = timelong or COPY.MOPUP_TIMELONG
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local mopup = user:GetCurMopup()
	if mopup and mopup.state == COPY.STATE_DOING then
		self.__temp.mopupTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsHeroCopy.ProMopup, timelong, self)
	end
end

function clsHeroCopy:RemoveMopupCallOut()
	if self.__temp.mopupTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.mopupTimer)
	end
end

function clsHeroCopy:CopyMopup(times)
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
	user:SendDailyHeroCopyUseTimes(1)
	user:SendProtocol("c_heroCopy_buy",	COPY.GetHeroCopyTimesInfo(user))
	user:SendProtocol("c_copy_mopup", {type = self:GetType(), copyId = self:GetId(), num = times, state = STATE_DOING, fastIngot = COPY.FAST_MOPUP_INGOT})
	if VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_HERO_COPY_MOPUP_FINISH) or user:GetLv() >= self:GetFastDoneNeedLv() then
		return self:FastMopup()
	end
	self:StartMopupCallOut()
end

function clsHeroCopy:ProMopup(type)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	local mopup = user:GetCurMopup()
	self:RemoveMopupCallOut()
	local done = {times = mopup.num}
	user:SetFuncStatById(SETTING.FUNC_HERO_COPY, 1)
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
		for k, v in pairs(v.itemList or {}) do
			if v.needMissionId and user:IsMissionAdd(v.needMissionId) then
				BASEITEM.CombiItemToList(done.itemList, v.id, math.random(v.min, v.max), v.needMissionId)
			else
				BASEITEM.CombiItemToList(done.itemList, v.id, math.random(v.min, v.max))
			end
		end
		local monsterList = FIGHT.GetMonsterTeamInfoById(v.teamId)
		for k, v in pairs(monsterList.list) do
			MISSION.CheckMissionEvent(user, "fight", v.id, 1)
		end
	end
	local times = VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_OPEN_EXTRA_CARD) and 2 or 1
	for i = 1, times do
		local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
		BASEITEM.CombiItemToList(done.itemList, prize[1].id, prize[1].num)
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
			user:SetCurMopupCopy(self)
		end
	else
		user:SetVigour(-self:GetEnterVigour())
		user:SetGold(-self:GetMopupCost())
		user:SetDailyHeroCopyUseTimes(1)
		mopup.timestamp = mopup.timestamp + COPY.MOPUP_TIMELONG
		if not type then
			self:StartMopupCallOut()
		end
	end
	mopup.num = mopup.num + 1
	STATS.SaveCopyHistory(self:GetId(), user:GetRoleId(), 1, 1)
end

function clsHeroCopy:SendMopupUpdate(done)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	local mopup = user:GetCurMopup()
	sData.state = mopup.state
	sData.done = done
	sData.curTimes = mopup.num
	sData.timelong = mopup.timestamp
	user:SendProtocol("c_copy_mopupUpdate", sData)
	user:SendProtocol("c_heroCopy_buy", COPY.GetHeroCopyTimesInfo(user))
end

function clsHeroCopy:SendMopupFinish()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local got = user:GetMopupGot()
	local sData = {}
	sData.expGot = got.exp
	sData.nimbus = got.nimbus
	sData.itemGot = got.itemList
	local mopup = user:GetCurMopup()
	sData.state = mopup.state
	user:SendProtocol("c_copy_mopupFinish", sData)
	user:SendProtocol("c_heroCopy_buy", COPY.GetHeroCopyTimesInfo(user))
end

function clsHeroCopy:FastMopup()
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

function clsHeroCopy:StopMopup()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:RemoveMopupCallOut()
	user:SetCurMopup()
	user:LeaveScene()
	user:SetLocateInfo()
	user:SetCurMopupCopy(self)
	return true
end

function clsHeroCopy:GetPassCopyExp()
	local ret = self:CalCopyScore()
	local sData = {}
	sData.atkScore = ret.atkScore
	sData.defScore = ret.defScore
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	sData.baseExp = info.exp
	sData.addExp = math.ceil(info.exp * (ret.rate or 1))
	return sData
end

function clsHeroCopy:DestroyCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if self:IsCopyPassed() then
		if not self:IsPassExpGot() then
			local sData = self:GetPassCopyExp()
			user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
			self:SetPassExpGot(true)
		end
		local itemList = COPY.GetCopyPrize(self:GetId(), self:GetClickMaxTimes(self:GetOwner()) - self:GetClickTimes(self:GetOwner()))
		for k, v in pairs(item or {}) do
			local item = BASEITEM.NewItem(v.id, v.num)
			user:AddItem(USERBASE.PACKAGE, item)
		end
		user:SetCopyPassed(self:GetId())
	end
	Super(clsHeroCopy).DestroyCopy(self)
end
