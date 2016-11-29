local MAX_PRIZE_NUM = 5
local scoreList = {
	[1] = {min = 1, max = 60, rate = 0},
	[2] = {min = 61, max = 80, rate = 0.05},
	[3] = {min = 81, max = 90, rate = 0.1},
	[4] = {min = 91, max = 100, rate = 0.15},
}

function GetPrizeNum()
	return MAX_PRIZE_NUM
end

function GetAddRateByScore(score)
	for k, v in ipairs(scoreList) do
		if score >= v.min and score <= v.max then
			return v.rate
		end
	end
end
--攻击得分
function GetAttackScore(hp, damage)
	local score = math.ceil((1 - hp / damage) * 60 + 20)
	return math.min(score, 50)
end
--防守得分
function GetDefenceScore(hp, hpMax)
	if hp > hpMax then
		hp = hpMax
	end
	return math.ceil((hp / hpMax) * 30 + 20)
end

function GetCopyFightPrize(copyId, teamId, roleId)
	local info = BASECOPY.GetCopyInfoById(copyId)
	local teamInfo
	for k, v in pairs(info.monsterList) do
		if v.teamId == teamId then
			teamInfo = v
			break
		end
	end
	if not teamInfo then return end
	local prize = {}
	prize.gold = teamInfo.gold
	prize.exp = teamInfo.exp
	prize.nimbus = teamInfo.nimbus
	prize.item = {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		for k, v in pairs(teamInfo.itemList) do
			local misObj = user:GetMissionById(v.needMissionId)
			if not v.needMissionId or (misObj and misObj:GetState() ~= CLSMISS.FINISHED) then
			--	local num = math.random(v.min, v.max)
				local num = 1
				if num > 0 then
					table.insert(prize.item, {goodsId = v.id, num = num, missionId = v.needMissionId})
				end
			end
		end
	end
	return prize
end

------------------------------
clsCopy = BASECOPY.clsCopy:Inherit()

function clsCopy:__init__()
	Super(clsCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
end

function clsCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.stats = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
	self.__temp.callout = {}
end

--怪物血量总和
function clsCopy:SetFoeHpSum(hp)
	self.__temp.stats.foeHp = (self.__temp.stats.foeHp or 0) + hp
end

function clsCopy:GetFoeHpSum()
	return self.__temp.stats.foeHp or 0
end
--伤害总和
function clsCopy:SetDamageSum(damage)
	self.__temp.stats.damage = (self.__temp.stats.damage or 0) + damage
end

function clsCopy:GetDamageSum()
	return self.__temp.stats.damage or 0
end

--我方血量总和
function clsCopy:SetMyHpSum(hp)
	self.__temp.stats.myHpSum = (self.__temp.stats.myHpSum or 0) + hp
end

function clsCopy:GetMyHpSum()
	return self.__temp.stats.myHpSum or 0
end

--我方剩余血量
function clsCopy:SetMyHpLeft(hp)
	self.__temp.stats.myHpLeft = (self.__temp.stats.myHpLeft or 0) + hp
end

function clsCopy:GetMyHpLeft()
	return self.__temp.stats.myHpLeft or 0
end

function clsCopy:DestroyCopy()
	local scene = self:GetScene()
	local npcList = scene:GetSceneRoleList()
	for k, v in pairs(npcList) do
		if v:GetRoleType() == "user" then
			v:LeaveScene()
		else
			NPC.NpcLeave(v)
		end
	end
	SCENE.DestroyScene(scene)
	self:Destroy()
end

function clsCopy:ProFight(id)
	local scene = self:GetScene()
	if not scene then
		return false, "无效的副本"
	end
	local npcObj = scene:GetSceneRole(id)
	if not npcObj then
		return false, "指定的敌人不存在。"
	end
	local roleId = self:GetOwner()
	local user = assert(USER.GetOnlineUser(roleId))
	local function callback(fightId, winner, prize, stats)
		local user = USER.GetOnlineUser(roleId)
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
				BASECOPY.AddCopyItem(user, prize.item, self:GetName())
			end
			self:SetFoeHpSum(stats.defHpSum)
			self:SetDamageSum(stats.atkDamage)
			self:SetMyHpSum(stats.atkHpSum)
			self:SetMyHpLeft(stats.atkHp)
			if #self:GetCopyMonsterList() <= 0 then
				self:PassCopy(user)
			end
		else
			local location = user:GetLocateInfo()
			user:Goto(math.max(location.x - 250, 150), location.y)
		end
	end
	local param = {saveHp = true, restoreHp = true, Lv = self:GetId(), teamId = npcObj:GetId()}
	local copyInfo = BASECOPY.GetCopyInfoById(self:GetId())
	if user:GetMissionById(copyInfo.needMissionId) then
		local misInfo = CLSMISS.GetMissionInfoById(copyInfo.needMissionId)
		if misInfo.remark then
			param.maxRage = true
		end
	end
	local locate = user:GetLocateInfo()
	local scene = self:GetScene()
	user:SetWorkInfo("fighting")
	npcObj:SetState("fighting")
	local fightId = FIGHT.BeginFight("copy", roleId, npcObj:GetId(), {roleId}, callback, param, {mapId = scene:GetMapId(), posX = locate.x, posY = locate.y})
	return true
end

function clsCopy:CalCopyScore()
	local ret = {}
	ret.atkScore = GetAttackScore(self:GetFoeHpSum(), self:GetDamageSum())
	ret.defScore = GetDefenceScore(self:GetMyHpLeft(), self:GetMyHpSum())
	ret.rate = GetAddRateByScore(ret.atkScore + ret.defScore)
	return ret
end

function clsCopy:PassCopy(user)
	self:SetCopyPassed()
	user:SetCopyPassed(self:GetType(), self:GetId())
	self:SendCopyPassed()		--发送副本通关提示
	user:SetFuncStatById(SETTING.FUNC_NORMAL_COPY, 1)
end

function clsCopy:SendCopyStats(user)
	local ret = self:CalCopyScore()
	local sData = {}
	sData.atkScore = ret.atkScore
	sData.defScore = ret.defScore
	local info = BASECOPY.GetCopyInfoById(self:GetId())
	sData.baseExp = info.exp
	sData.addExp = math.ceil(info.exp * (ret.rate or 1))
	user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
	self:SetPassExpGot(true)
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_copy_stats", sData)
	end
end
