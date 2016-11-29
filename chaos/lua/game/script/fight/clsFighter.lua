--战斗成员

local ATTR_TRANSLATE_RATE = 5	--属性转化率
local PET_POS = "10"
local CITY_POS = "11"
local DEFALT_SKILL_ROUND = 0		--默认的技能回合为偶数回合

clsRole = clsObject:Inherit()

function clsRole:__init__()
	Super(clsRole).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsRole:initsavedata()
	self.__save.skillList = {}
	self.__save.state = {}
end

function clsRole:inittempdata()

end

function clsRole:InitFighter(label, roleId, type, fightData)
	self:SetLabel(label)
	self:SetRoleId(roleId)
	self:SetType(type)
	self:FillFigher(fightData)
end

function clsRole:SetLabel(label)
	self.__save.label = label
end

function clsRole:GetLabel()
	return self.__save.label
end

function clsRole:SetRoleId(id)
	self.__save.roleId = id
end

function clsRole:GetRoleId()
	return self.__save.roleId
end

function clsRole:SetType(type)
	self.__save.type = type
end

function clsRole:GetType()
	return self.__save.type
end

function clsRole:GetPos()
	if self:GetIdentity() == "pet" then
		return PET_POS
	elseif self:GetIdentity() == "city" then
		return CITY_POS
	end
	return self.__save.pos
end

function clsRole:FillFigher(fightData)
	self.__save.stuntId = fightData.stuntId
	self.__save.stuntLv = fightData.stuntLv
	self.__save.identity = fightData.identity
	self.__save.modelId = fightData.modelId
end

--行动结束
function clsRole:SetActDone(state)
	self.__temp.actDone = state
end

function clsRole:IsActDone()
	return self.__temp.actDone
end

function clsRole:GetIdentity()
	return self.__save.identity
end

function clsRole:SetIdentity(identt)
	self.__save.identity = identt
end

function clsRole:GetSkillByType(type)
	return self.__save.skillList[type] or {}
end

function clsRole:GetStuntId()
	return self.__save.stuntId
end

function clsRole:GetStuntLv()
	return self.__save.stuntLv
end

function clsRole:GetName()
	return self.__save.name
end

function clsRole:SetName(name)
	self.__save.name = name
end


function clsRole:SetModelId(modelId)
	self.__save.modelId = modelId
end

function clsRole:GetModelId()
	return self.__save.modelId
end

function clsRole:GetAptitude()
	return self.__save.aptitude
end

function clsRole:SetAtptitude(aptLv)
	self.__save.aptitude = aptLv
end

function clsRole:GetFighterInfo()
	local buff = {}
	buff.pos = self:GetPos()
	buff.roleId = self:GetRoleId()
	buff.identity = self:GetIdentity()
	buff.picId = self:GetModelId()
	return buff
end


function clsRole:IsStateExist(state)
	return self.__save.state[state]
end

function clsRole:IsDead()
	return self.__temp.dead
end
-----------------------------------------------------------------------------------

local clsFighter = clsRole:Inherit()

function clsFighter:InitFighter(label, roleId, type, fightData, flag)
	self:SetLabel(label)
	self:SetRoleId(roleId)
	self:SetType(type)
	self:FillFigher(fightData)
	if not flag or (self:GetIdentity() ~= "user" and self:GetIdentity() ~= "hero") then
		self:RestoreAllHp()
	end
end

function clsFighter:FillFigher(fightData)
	self.__save.name = fightData.name
	self.__save.aptitude = fightData.aptLv
	self.__save.headId = fightData.headId
	self.__save.modelId = fightData.modelId
	if fightData.heroId then
		self.__save.identity = "hero"
		self.__save.id = fightData.heroId
	elseif fightData.roleId then
		self.__save.identity = "user"
		self.__save.id = fightData.roleId
	else
		self.__save.identity = fightData.boss and "boss" or "npc"
		self.__save.id = fightData.npcId
	end
	self.__save.Lv = fightData.Lv
	self.__save.pos = fightData.pos
	self.__save.job = fightData.job		--职业
	self.__save.atkMode = JOB.GetJobAttackType(fightData.job)
	self.__save.atkRange = JOB.GetJobAttackRange(fightData.job)
	
	self.__save.hp = fightData.hp
	self.__save.hpMax = fightData.hpMax
	self.__save.physcAtkMax = fightData.physcAtkMax		--物理攻击
	self.__save.physcAtkMin = fightData.physcAtkMin		--物理攻击
	self.__save.magicAtkMin = fightData.magicAtkMin		--法术攻击
	self.__save.magicAtkMax = fightData.magicAtkMax		--法术攻击
	self.__save.physcDef = fightData.physcDef		--物理防御
	self.__save.magicDef = fightData.magicDef		--魔法防御
	self.__save.speed = fightData.speed
	
	self.__save.strength = fightData.strength or 0
	self.__save.intellect = fightData.intellect or 0
	self.__save.jink = fightData.jink or 0		--闪避
	self.__save.fatal = fightData.fatal or 0		--暴击
	self.__save.crush = fightData.crush or 0	--破甲
	self.__save.harmDeep = fightData.harmDeep or 0		--伤害加深
	self.__save.physcResist = fightData.physcResist or 0		--物理伤害抗性
	self.__save.magicResist = fightData.magicResist or 0	--法术伤害抗性
	self.__save.antiFatal = fightData.antiFatal or 0	--暴击抗性
	self.__save.hitRate = fightData.hitRate or 0		--命中
	
	self.__save.skillList = {active = {}, passive = {}}	--type:攻击 辅助
	for _, v in pairs(fightData.skillList or {}) do
		local skillInfo = BASESKILL.GetSkillInfo(v.id)
		if not skillInfo then
			DATA.log(string.format("invalid skillId : %s.", v.id), "clsFighter.log")
		else
			if skillInfo.type == 1 and skillInfo.list[v.Lv] then
				self.__save.skillList.active[v.id] = {Lv = v.Lv, cdTime = 0}
			elseif skillInfo.type == 2 and skillInfo.list[v.Lv] then
				self.__save.skillList.passive[v.id] = {Lv = v.Lv, cdTime = 0}
			end
		end
	end
	
	self.__save.counter = fightData.counter or 0
	self.__save.batter = fightData.batter or 0
	self.__save.ignoreMgcDef = fightData.ignoreMgcDef or 0
	self.__save.block = fightData.block or 0
	self.__save.hurtDown = fightData.hurtDown or 0
	self.__save.skillRound = fightData.skillRound
	
	self.__save.furySkill = fightData.furySkill
	self.__save.furyRounds = fightData.furyRounds
	self.__save.stuntId = fightData.stuntId
	self.__save.stuntLv = fightData.stuntLv
	if fightData.isMain or fightData.boss then
		self.__save.boss = 1
	end
end

function clsFighter:PrintFighterData()
	local str = self:GetName()
	str = str .. string.format("\tLv:%g", self:GetLv())
	str = str .. string.format("\tHp:%g", self:GetHp())
	str = str .. string.format("\tphyAtk:%g", self:GetPhyscAtk())
	str = str .. string.format("\tphyDef:%g", self:GetPhyscDef())
	str = str .. string.format("\tmgcAtk:%g", self:GetMagicAtk())
	str = str .. string.format("\tmgcDef:%g", self:GetMagicDef())
	str = str .. string.format("\tsp:%g", self:GetSpeed())
	str = str .. string.format("\tjink:%g", self:GetJinkRate())
	str = str .. string.format("\tfatal:%g", self:GetFatalRate())
	str = str .. string.format("\tcrush:%g", self:GetCrushRate())
	str = str .. string.format("\tharmDeep:%g", self:GetHarmDeep())
	str = str .. string.format("\tphyRes:%g", self:GetPhyscResist())
	str = str .. string.format("\tmgcRes:%g", self:GetMagicResist())
	str = str .. string.format("\tcouter:%g", self:GetCounterRate())
	str = str .. string.format("\tbatter:%g", self:GetBatterRate())
	str = str .. string.format("\tblock:%g", self:GetBlockRate())
--	print(str)
end

function clsFighter:GetFighterInfo()
	local buff = {}
	buff.pos = self:GetPos()
	buff.roleId = self:GetRoleId()
	local identity = self:GetIdentity()
	buff.identity = identity
	buff.picId = self:GetModelId()
	buff.nick = self:GetName()
	buff.Lv = self:GetLv()
	buff.aptLv = self:GetAptitude()
	buff.hp = self:GetHp()
	buff.hpMax = self:GetHpMax()
	if identity == "user" or identity == "hero" then
		buff.job = self:GetJob()
		local skillId = next(self:GetActiveSkill())
		if skillId then
			buff.skillList = buff.skillList or {}
			table.insert(buff.skillList, {type = 1, skillId = skillId})
		end
		if self:IsBoss() and self.__save.stuntId then
			buff.skillList = buff.skillList or {}
			table.insert(buff.skillList, {type = 1, skillId = self.__save.stuntId})
		end
	end
	return buff
end

function clsFighter:IsBoss()
	return self.__save.boss
end


function clsFighter:GetHeadId()
	return self.__save.headId
end

function clsFighter:SetHeadId(headId)
	self.__save.headId = headId
end

function clsFighter:GetId()
	return self.__save.id
end

function clsFighter:SetId(id)
	self.__save.id = id
end

function clsFighter:GetPos()
	return self.__save.pos
end

function clsFighter:SetPos(pos)
	self.__save.pos = pos
end

function clsFighter:GetLv()
	return self.__save.Lv
end

function clsFighter:SetLv(lv)
	self.__save.Lv = lv
end

function clsFighter:GetJob()
	return self.__save.job
end

function clsFighter:SetJob(job)
	self.__save.job = job
end

function clsFighter:GetAtkMode()
	return self.__save.atkMode
end

function clsFighter:SetAtkMode(mode)
	self.__save.atkMode = mode
end

function clsFighter:GetAtkRange()
	return self.__save.atkRange
end

function clsFighter:SetAtkRange(range)
	self.__save.atkRange = range
end

function clsFighter:GetHpMax()
	return self.__save.hpMax
end

function clsFighter:SetHpMax(num)
	self.__save.hpMax = (self.__save.hpMax or 0) + num
end

function clsFighter:GetHp()
	return self.__save.hp or 0
end

function clsFighter:SetHp(num)
	assert(tonumber(num))
	self.__save.hp = math.floor((self.__save.hp or 0) + num)
	if self.__save.hp > self.__save.hpMax then
		self.__save.hp = self.__save.hpMax
	elseif self.__save.hp < 0 then
		self.__save.hp = 0
	end
end

function clsFighter:RestoreAllHp()
	self:SetHp(self:GetHpMax() - self:GetHp())
end
--boss必杀技
function clsFighter:GetFurySkill()
	return self.__save.furySkill
end
--发出必杀技的回合
function clsFighter:GetFuryRounds()
	return self.__save.furyRounds
end

--行动次数
function clsFighter:SetActionRounds()
	self.__save.actionRounds = (self.__save.actionRounds or 0) + 1
end

function clsFighter:GetActionRounds()
	return self.__save.actionRounds or 0
end

--技能回合（1为奇数回合 0为偶数回合）
function clsFighter:GetSkillRound()
	return self.__save.skillRound or DEFALT_SKILL_ROUND
end
--是否为技能回合
function clsFighter:IsSkillRound()
	return self:GetActionRounds() % 2 == self:GetSkillRound()
end
----------------------------------------------------------------------------------
function clsFighter:SetFighterData(type, num)
	self.__save[type] = num
end

function clsFighter:GetFighterData(type)
	return self.__save[type]
end
----------------------------以下字段只提供 get 方法，不提供 set 方法---------------

function clsFighter:GetPhyscAtk(method, num, rate)
	local force = 0
	if method == 1 then
		force = num + self:GetStrength() * ATTR_TRANSLATE_RATE
	elseif method == 3 then
		force = self:GetPhyscAtk(1, num) + self:GetPhyscAtk(2, nil, rate)
	else
		force = math.random(self.__save.physcAtkMin, self.__save.physcAtkMax)
		if method == 2 then
			force = force * rate
		end
		local upInfo = self:IsStateExist("phy_atk_up")
		if upInfo then
			force = force + upInfo.num
		end
		local downInfo = self:IsStateExist("phy_atk_down")
		if downInfo then
			force = force + downInfo.num
		end
	end
	return force
end

function clsFighter:GetPhyscDef()
	local defence = self.__save.physcDef
	local upInfo = self:IsStateExist("phy_def_up")
	if upInfo then
		defence = defence + upInfo.num
	end
	local upRateInfo = self:IsStateExist("phy_def_per_up")
	if upRateInfo then
		defence = defence + upRateInfo.rate
	end
	local downInfo = self:IsStateExist("phy_def_down")
	if downInfo then
		defence = defence + downInfo.num
	end
	return defence
end

function clsFighter:GetMagicAtk(method, num, rate)
	local force = 0
	if method == 1 then
		force = num + self:GetIntellect() * ATTR_TRANSLATE_RATE
	elseif method == 3 then
		force = self:GetMagicAtk(1, num) + self:GetMagicAtk(2, nil, rate)
	else
		force = math.random(self.__save.magicAtkMin, self.__save.magicAtkMax)
		if method == 2 then
			force = force * rate
		end
		local upInfo = self:IsStateExist("mgc_atk_up")
		if upInfo then
			force = force + upInfo.num
		end
		local downInfo = self:IsStateExist("mgc_atk_down")
		if downInfo then
			force = force - downInfo.num
		end
	end
	return force
end

--魔法防御
function clsFighter:GetMagicDef()
	local defence = self.__save.magicDef
	local upInfo = self:IsStateExist("mgc_def_up")
	if upInfo then
		defence = defence + upInfo.num
	end
	local downInfo = self:IsStateExist("mgc_def_down")
	if downInfo then
		defence = defence - downInfo.num
	end
	return defence
end
--攻击力（由攻击方式决定）
function clsFighter:GetAttackForce()
	local mode = self:GetAtkMode()
	if mode == "magic" then
		return self:GetMagicAtk()
	elseif mode == "physc" then
		return self:GetPhyscAtk()
	end
end

function clsFighter:GetSpeed()
	local sp = self.__save.speed
	local upInfo = self:IsStateExist("sp_up")
	if upInfo then
		sp = sp + upInfo.num
	end
	local downInfo = self:IsStateExist("sp_down")
	if downInfo then
		sp = sp - downInfo.num
	end
	return sp
end

function clsFighter:GetStrength()
	return self.__save.strength
end

function clsFighter:GetIntellect()
	return self.__save.intellect
end



function clsFighter:GetJinkRate(hitRate)
	local rate = self.__save.jink or 0
	if hitRate then
		rate = rate / (hitRate + 10000) 
	end
	return rate
end

function clsFighter:GetFatalRate(antiFatal)
	local rate = self.__save.fatal or 0
	local stateInfo = self:IsStateExist("fatal_up")
	if stateInfo then
		rate = rate + stateInfo.num
	end
	if antiFatal then
		rate = rate / (antiFatal + 10000)
	end
	return rate
end

function clsFighter:GetCrushRate()
	local rate = self.__save.crush or 0
	return rate / 10000
end

function clsFighter:GetHarmDeep()
	local rate = self.__save.harmDeep or 0
	return rate / 10000
end

function clsFighter:GetPhyscResist()
	local rate = self.__save.physcResist or 0
	return rate / 10000
end

function clsFighter:GetMagicResist()
	local rate = self.__save.magicResist or 0
	return rate / 10000
end

function clsFighter:GetAntiFatalRate()
	return self.__save.antiFatal or 0
end

function clsFighter:GetHitRate()
	return self.__save.hitRate or 0
end

--反击概率
function clsFighter:GetCounterRate()
	local rate = self.__save.counter or 0
	local stateInfo = self:IsStateExist("counter")
	if stateInfo then
		rate = rate + stateInfo.rate
	end
	return rate
end
--攻击时连击概率
function clsFighter:GetBatterRate()
	return self.__save.batter or 0
end
--防御时降低一定伤害
function clsFighter:GetHarmReduce()
	return self.__save.hurtDown or 0
end
--攻击时无视法防
function clsFighter:GetMagicDefIgnore()
	return self.__save.ignoreMgcDef or 0
end
--格挡概率
function clsFighter:GetBlockRate()
	return self.__save.block or 0
end

function clsFighter:GetHurtDownRate()
	return self.__save.hurtDownRate or 0
end



--攻击技能
function clsFighter:GetActiveSkill()
	return self.__save.skillList.active
end
--辅助技能
function clsFighter:GetPassiveSkill()
	return self.__save.skillList.passive
end


function clsFighter:SetSkillCdTime(skillId, cdTime)
	local info = self.__save.skillList.active[skillId] or self.__save.skillList.passive[skillId]
	if not info then return end
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	info.cdTime = cdTime or skillInfo.list[info.Lv].cdTime
end

function clsFighter:IsSkillValid(skillId)
	local info = self.__save.skillList.active[skillId] or self.__save.skillList.passive[skillId]
	if not info then return false end
	if info.cdTime > 0 then
		return false
	end
	return info
end
--每回合减少技能cd时间
function clsFighter:ReduceSkillCdTime()
	for type, list in pairs(self.__save.skillList) do
		for k, v in pairs(list) do
			if v.cdTime > 0 then
				v.cdTime = v.cdTime - 1
			end
		end
	end
end

function clsFighter:GetValidSkillList()
	local buff = {}
	for type, list in pairs(self.__save.skillList) do
		for k, v in pairs(list) do
			if not v.cdTime or v.cdTime <= 0 then
				table.insert(buff, {skillId = k, skillLv = v.Lv})
			end
		end
	end
	return buff
end


function clsFighter:GetStateList()
	return self.__save.state
end

function clsFighter:GetFormatStateList()
	local stateList = {}
	for k, v in pairs(self.__save.state) do
		table.insert(stateList, k)
	end
	return stateList
end

function clsFighter:AddState(state, times, num, rate, isMyRound)
	if self:IsDead() then
		return
	end
	self.__save.state[state] = {times = times, num = num, rate = rate, isNew = isMyRound and 1 or 0}
end

function clsFighter:DelState(state)
	self.__save.state[state] = nil
end

--出战人员阵亡 rounds 回合
function clsFighter:SetDead(rounds)
	self.__save.state = {}
	self:AddState("dead", 1, 0)
	self.__temp.dead = rounds
end
------------------------------------------
function NewFigher(type, roleId, fighterType, data, flag)
	local class
	if type == "invisible" then
		class = clsRole
	else
		class = clsFighter
	end
	local fighter = class:New()
	fighter:InitFighter(type, roleId, fighterType, data, flag)
	return fighter
end
