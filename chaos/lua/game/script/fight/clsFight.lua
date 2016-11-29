--战斗类

local MAX_ROUND_TIME = 60
local MIN_ROUND_TIME = 0
local MAX_ROUND = 30			--最大回合数
local MAX_RAGE = 100			--最大怒气值
local INIT_MORALE_NUM = 100		--初始士气值
local MAX_ORDER = 8
local MIN_COMBO_NUM = 2
local RESTORE_MIN_LOST_RATE = 0.25	--触发补血技能的最小损血比率
local ACTIVE_SKILL_RATE = 0.4	--主动技能触发几率
local PASSIVE_SKILL_RATE = 0.2	--被动技能触发几率
local NONE_FORMATION = "321001"		--【无】阵法id
local BOSS_FIGHT_MAX_ROUND = 1		--boss战最大回合
local MAGIC_SKILL_TYPE = 2			--施法技能类型（辅助技能类型）
local STATE_ALL_OVER = 1		--战斗结束
local STATE_REIFORCE = 2		--战斗增援
local NEED_MOVE_SKILL_LINK_PIC = 350017		--近战联动特效id

local ACTION_DONE = 1	--已出手
local ACTION_WAIT = 0	--未出手

local PET_GUARD_ADD_DEFENCE_RATE = 0.2	--本命精灵舍命守护增加防御比率
local PET_GUARD_LOST_REDUCE_RATE = 0.5	--本命精灵舍命守护减少损失比率

local fightState = {
	["spun"] = "眩晕", ["bleed"] = "流血", ["powUp"] = "能力提升", ["invincible"] = "无敌", 
	["dying"] = "濒死", ["alive"] = "复活", ["dead"] = "死亡", ["poison"] = "中毒",
}

local actionList = {
	["wait"] = "站立",
	["normal"] = "普通攻击",			--普通攻击	
	["skill"] = "技能",			--技能攻击
	["stunt"] = "绝技",			--绝技攻击
	["counter"] = "反击",		--反击
	["miss"] = "闪避",		--闪避
	["block"] = "格挡",		--格挡
	["revival"] = "复活",		--复活
	["beat"] = "击中",			--被击中
	["fatal"] = "暴击",			--被暴击
	["crush"] = "破甲",
	["restore"] = "恢复",
}

local rageModeList = {["attack"] = 5, ["fatal"] = 10, ["crush"] = 10, ["dead"] = 20}		--增加怒气动作类型

--获取相对的类型
function GetOppositeType(type)
	return type == "attack" and "defend" or "attack"
end

---------------------------------------------------------------------

clsFight = clsObject:Inherit()

function clsFight:__init__()
	Super(clsFight).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsFight:initsavedata()
	self.__save.round = 0
	self.__save.fightId = 0
	
	self.__save.attack = {}
	self.__save.defend = {}
	self.__save.combo = 0
	self.__save.realActList = {}	--攻击顺序
	self.__save.against = {}		--玩家对抗表
	self.__save.minTm = 0
	self.__save.maxTm = 0			--每回合持续的时间
	self.__save.alive = {}
	self.__save.state = "process"
	self.__save.orderFlag = false
end

function clsFight:inittempdata()
	self.__temp.fighterRoleId = 0
	self.__temp.callout = {}
	self.__temp.userList = {}
	self.__temp.IsFastFight = false
	self.__temp.lastAttack = nil		--上一次的攻击方
	self.__temp.revival = {}			--复活列表
	self.__temp.actionList = {}			--攻击效果
	self.__temp.linkList = {}			--技能联动列表
	self.__temp.prizeList = {}
	self.__temp.reinforce = {}
	self.__temp.record = {}				--战斗录像
	self.__temp.sumDamage = {}
	self.__temp.initData = {}
	
	self.__temp.resource = {model = {}, skill = {}}			--战斗动画资源
end

--本次战斗中的玩家数据
function clsFight:SetUserDataList(list)
	self.__temp.userDataList = list
end

function clsFight:GetUserDataList()
	return self.__temp.userDataList
end

function clsFight:FormatFighterData(data)
	local dataType = type(data)
	local userDataList = self:GetUserDataList()
	if dataType == "string" then
		if FIGHT.GetMonsterTeamInfoById(data) then
			return FIGHT.FormatNpcTeamData(data)
		end
		assert(userDataList[data], string.format("%s fightData Error: fightType=%s.", data, self:GetFightType()))
		return FIGHT.FormatSingleUserData(userDataList[data])
	end
	return FIGHT.FormatTeamUserData(data, userDataList)
end

--param: saveHp 使用真实血量 restoreHp 战斗后恢复血量 stuntId 战斗开始时执行的技能id stuntLv 技能等级
--		maxRage 战斗开始时满怒气
--lv = lv 
--		(retreat目前使用：Lv 战斗发生层数 double 经验翻倍)
--reinforce怪物增援
function clsFight:InitFight(fightId, type, attacker, defender, userList, param, userDataList, locate)	
	self:SetFightId(fightId)
	self:SetFightType(type)
	self:SetUserDataList(userDataList)
	self:SetFightLocateInfo(locate)
	param = param or {}
	self:SetFightParam(param)				--战斗附加参数
	self:SetMaxTime(MAX_ROUND_TIME)
	local attackerData = self:FormatFighterData(attacker)
	self:InitFighterData("attack", attackerData)
	self:SetInitFighterData("attack", attacker)
	local defenderData = self:FormatFighterData(defender)
	self:InitFighterData("defend", defenderData)
	self:SetInitFighterData("defend", defender)
	self:InitBossHp()
	self:SetUserList(userList)
	self:InitReinForceInfo(param.reinforce)
	
	self:CalAttackOrder()		--计算攻击次序
	self:RecordFightStart()		--记录战斗开始
	self:SetState("wait")
	self:StartCallOut()
	self:CalInitSumHp()
--	self:RecordFightMinTime(self:GetCycle(), self:GetMinTime())
	
	self:SetCycle(1)
	self:BeginRound()
	self:SetFastFightState(param.fastFight)
end

function clsFight:Ready()
--	self:SendDrawBegin()
	self:BeginRound()
end

function clsFight:SetFightLocateInfo(locate)
	self.__temp.locate = locate
end

function clsFight:GetFightLocateInfo()
	return self.__temp.locate
end

function clsFight:GenFighterRoleId()
	self.__temp.fighterRoleId = self.__temp.fighterRoleId + 1
	return self.__temp.fighterRoleId
end

function clsFight:GetFighterList(type)
	return self.__save[type].list
end

function clsFight:GetInitFighterData(type)
	return self.__temp.initData[type]
end

function clsFight:SetInitFighterData(type, fightData)
	self.__temp.initData[type] = fightData
end

function clsFight:InitFighterData(type, fightData)
	self.__save[type] = {boss = nil, rage = 0, list = {}, morale = 100}
	self.__save.against[type] = {}
	self.__save.alive[type] = {}
	local param = self:GetFightParam()				--确定是否使用真实血量
	for k, v in pairs(fightData.list) do
		if v.boss or v.isMain then
			self.__save[type].boss = v.pos
			self.__save[type].rage = param.maxRage and MAX_RAGE or 0
			self.__save[type].rage = math.min(self.__save[type].rage + (v.rage or 0), 100)
			self:SetGroupIdentt(type, v.isMain and "user" or "npc")
			table.insert(self.__save.against[type], v.pos)
		end
		self.__save.alive[type][v.pos] = 1		--幸存人员列表
		table.insert(self.__save.realActList, {type = type, pos = v.pos, speed = v.speed, state = 0})		--state 0 未出手 1 已出手	实际出手顺序
		local roleId = v.roleId
		if not roleId then
			roleId = self:GenFighterRoleId()
		end
		local fighter = CLSFIGHTER.NewFigher("visible", roleId, type, v, param.saveHp)
		self.__save[type].list[v.pos] = fighter
		self:SetFightResource("model", fighter:GetModelId(), fighter:GetIdentity())
		--初始化本命精灵对象
		if v.isMain then
			if v.stuntId and v.stuntLv and v.stuntLv > 0 then
				self.__save[type].list["pet"] = CLSFIGHTER.NewFigher("invisible", self:GenFighterRoleId(), type, {stuntId = v.stuntId, stuntLv = v.stuntLv, identity = "pet", modelId = v.petModelId})
			end
			self:SetMorale(type, v.morale or 0)
		end
	end
	local formInfo = FORMATION.GetFormationInfoById(fightData.formation.formId or NONE_FORMATION)
	self:SetFormation(type, fightData.formation or NONE_FORMATION, formInfo.restraint or {})		--设置阵法
	if formInfo.linkSkill then
		self:InitLinkSkill(type, formInfo.linkSkill, (formInfo.list[fightData.formation.formLv or 1].linkRate or 0) + (fightData.linkRate or 0))
	end
	
	--初始化城池对象
	if self:GetFightType() == "cityAttack" or self:GetFightType() == "pillage" then
		if param[type].stuntId and param[type].stuntLv and param[type].stuntLv > 0 then
			self.__save[type].list["city"] = CLSFIGHTER.NewFigher("invisible", self:GenFighterRoleId(), type, {stuntId = param[type].stuntId, stuntLv = param[type].stuntLv, identity = "city"})
		end
	end
	if self:GetFightType() == "pillage" then
		self:SetPetGuardRate(fightData.petGuard or 0)
		if type == "defend" then
			if math.random() <= (fightData.petGuard or 0) then
				param[type] = param[type] or {}
				param[type].physcDefRate = (param[type].physcDefRate or 0) + PET_GUARD_ADD_DEFENCE_RATE
				param[type].magicDefRate = (param[type].magicDefRate or 0) + PET_GUARD_ADD_DEFENCE_RATE
				self:SetPetGuardState("defend_up")
			end
		end
	end
	self:ProFightBuffer(type)
end


function clsFight:InitBossHp()
	local fightType = self:GetFightType()
	if fightType == "uniBoss" or fightType == "bossFight" then
		local param = self:GetFightParam()
		local pos = self:GetBossPos("defend")
		local boss = self:GetOneFighter("defend", pos)
		if boss then
			boss:SetFighterData("hp", param.bossCurHp or 0)
			boss:SetFighterData("hpMax", param.bossHpMax or 0)
		end
	end
end

function clsFight:ProFightBuffer(type)
	local param = self:GetFightParam()
	local info = param[type]
	if not info then
		return
	end
	for _, v in pairs(self.__save[type].list) do
		if v:GetLabel() == "visible" then
			if info.hpMax then
				v:SetFighterData("hpMax", v:GetFighterData("hpMax") * (1 + info.hpMax))
				v:SetFighterData("hp", v:GetFighterData("hp") * (1 + info.hpMax))
			end
			if info.speed then
				v:SetFighterData("speed", v:GetFighterData("speed") * (1 + info.speed))
			end
			if info.physcAtkRate then
				v:SetFighterData("physcAtkMin", v:GetFighterData("physcAtkMin") * (1 + info.physcAtkRate))
				v:SetFighterData("physcAtkMax", v:GetFighterData("physcAtkMax") * (1 + info.physcAtkRate))
			end
			if info.magicAtkRate then
				v:SetFighterData("magicAtkMin", v:GetFighterData("magicAtkMin") * (1 + info.magicAtkRate))
				v:SetFighterData("magicAtkMax", v:GetFighterData("magicAtkMax") * (1 + info.magicAtkRate))
			end
			if info.physcDefRate then
				v:SetFighterData("physcDef", v:GetFighterData("physcDef") * (1 + info.physcDefRate))
			end
			if info.magicDefRate then
				v:SetFighterData("magicDef", v:GetFighterData("magicDef") * (1 + info.magicDefRate))
			end
		end
	end
end

--参战方阵营(区分是否玩家)
function clsFight:SetGroupIdentt(type, identity)
	self.__save[type].identt = identity
end

function clsFight:GetGroupIdentt(type)
	return self.__save[type].identt
end

--设置阵型
function clsFight:SetFormation(type, data, restraint)
	self.__save[type].formation = {formId = data, restraint = restraint}
end

function clsFight:GetFormation(type)
	return self.__save[type].formation
end

--攻击者阵法是否克制被攻击者阵法
function clsFight:IsFormationRestraint(atkType, defType)
	local atkInfo = self:GetFormation(atkType)
	local defInfo = self:GetFormation(defType)
	return atkInfo.restraint[defInfo.formId]
end

--初始化阵型联动技能
function clsFight:InitLinkSkill(type, skillList, rate)
	local function init_link_skill(skillId)
	--	local buff = {rate = rate, list = {}}
		local buff = {rate = 1, list = {}}
		for pos, obj in pairs(self.__save[type].list) do
			for id in pairs(obj:GetSkillByType("active")) do
				if id == skillId then
					table.insert(buff.list, pos)
				end
			end
		end
		self.__save[type].linkList = self.__save[type].linkList or {}
		self.__save[type].linkList[skillId] = buff
	end
	for _, v in pairs(skillList) do
		init_link_skill(v)
	end
end

function clsFight:GetLinkSkillList(type)
	self.__save[type].linkList = self.__save[type].linkList or {}
	return self.__save[type].linkList
end

function clsFight:GetLinkSkillInfo(type, skillId)
	self.__save[type].linkList = self.__save[type].linkList or {}
	return self.__save[type].linkList[skillId]
end

--获取技能联动响应者(返回pos)
function clsFight:GetSkillLinker(type, skillId, pos)
	local info = self:GetLinkSkillInfo(type, skillId)
	if not info then
		return
	end
	if #info.list <= 1 or math.random() > info.rate then
		return
	end
	local buff = {}
	for k, v in pairs(info.list) do
		if v ~= pos then
			table.insert(buff, v)
		end
	end
	return buff[math.random(1, #buff)]
end
--移除死亡的联动者
function clsFight:RmSkillLinker(type, pos)
	for skillId, info in pairs(self:GetLinkSkillList(type)) do
		for k, v in ipairs(info.list) do
			if v == pos then
				table.remove(info.list, k)
			end
		end
	end
end
--最大等待时间
function clsFight:SetMaxTime(data)
	self.__save.maxTm = data
end

function clsFight:GetMaxTime()
	return self.__save.maxTm
end
--最小等待时间
function clsFight:SetMinTime(data)
	self.__save.minTm = data
end

function clsFight:GetMinTime()
	return self.__save.minTm
end

function clsFight:GetRevival()
	return self.__temp.revival or {}
end

function clsFight:SetRevival(type, pos)
	table.insert(self.__temp.revival, {type = type, pos = pos})
end


--保存攻击动作
function clsFight:SaveAttackAction(fighter, action, skillId, damage, stateFlag, isLinked)
	self.__temp.actionList.attacker = self.__temp.actionList.attacker or {}
	local buff = {roleId = fighter:GetRoleId(), action = action, skillId = skillId}
	if not skillId then
		if fighter:GetAtkRange() == 0 and (action and action ~= "revival") then
			buff.needMove = 1
		end
	else
		local skillInfo = BASESKILL.GetSkillInfo(skillId)
		buff.skillName = skillInfo.name
		if isLinked and skillInfo.needMove then
			buff.skillPicId = NEED_MOVE_SKILL_LINK_PIC
		else			
			buff.needMove = skillInfo.needMove
			buff.skillPicId = skillInfo.atkAction
		end
	end
	if damage then
		buff.hp = fighter:GetHp()
		buff.damage = {[1] = damage}
	end
	if stateFlag then
		buff.state = fighter:GetFormatStateList()
	end
	buff.isLinked = isLinked	
	table.insert(self.__temp.actionList.attacker, buff)
end

function clsFight:SaveTargetPos(type, pos)
	self.__temp.actionList.targetType = type
	self.__temp.actionList.targetPos = pos
end

function clsFight:GetSavedTargetPos()
	return self.__temp.actionList.targetPos
end


--保存目标动作
function clsFight:SaveTargetAction(targetType, pos, action, damage, effect)
	local fighter = self:GetOneFighter(targetType, pos)
	local buff = {}
	buff.targetType = targetType
	buff.roleId = fighter:GetRoleId()
	buff.action = action
	buff.hp = fighter:GetHp()
	buff.state = fighter:GetFormatStateList()
	buff.damage = {damage}
	buff.effect = effect
	self.__temp.actionList.target = self.__temp.actionList.target or {}
	table.insert(self.__temp.actionList.target, buff)
end

--添加联动技能伤害
function clsFight:AddLinkerDamage(targetType, pos, damage)
	self.__temp.actionList.target = self.__temp.actionList.target or {}
	local fighter = self:GetOneFighter(targetType, pos)
	local roleId = fighter:GetRoleId()
	for k, v in ipairs(self.__temp.actionList.target) do
		if v.roleId == roleId then
			table.insert(v.damage, damage)
			v.hp = fighter:GetHp()
			v.state = fighter:GetFormatStateList()
		end
	end
end

function clsFight:GetActionList()
	return self.__temp.actionList
end

--计算怒气值
--mode 角色死亡/角色被攻击
function clsFight:CalRage(type, mode)
	local rage = 0
	if rageModeList[mode] then
		rage = rageModeList[mode]
	end
	return rage
end

function clsFight:SetRage(type, mode)
	if self:GetGroupIdentt(type) ~= "user" then		--只有玩家才有怒气值
		return
	end
	local rage = self:CalRage(type, mode)
	self.__save[type].rage = (self.__save[type].rage or 0) + rage
	if self.__save[type].rage > MAX_RAGE then
		self.__save[type].rage = MAX_RAGE
	elseif self.__save[type].rage < 0 then
		self.__save[type].rage = 0
	end
end

function clsFight:ResetRage(type)
	self.__save[type].rage = 0
end

--怒气值
function clsFight:GetRage(type)
	return self.__save[type].rage or 0
end

function clsFight:GetMaxRage()
	return MAX_RAGE
end

function clsFight:GetState()
	return self.__save.state
end

function clsFight:SetState(data)
	self.__save.state = data
end

function clsFight:SetFightId(data)
	self.__save.fightId = data
end

function clsFight:GetFightId()
	return self.__save.fightId
end
--攻击次数
function clsFight:SetRound(data)
	self.__save.round = data
end

function clsFight:GetRound()
	return self.__save.round
end
--回合数
function clsFight:GetCycle()
	return self.__save.cycle or 1
end

function clsFight:SetCycle(data)
	self.__save.cycle = (self.__save.cycle or 1) + data
end

function clsFight:SetName(data)
	self.__save.name = data
end

function clsFight:GetName()
	return self.__save.name
end

function clsFight:GetWinner()
	return self.__save.winner
end

function clsFight:SetWinner(data)
	self.__save.winner = data
end

function clsFight:SetFightType(data)
	self.__save.fightType = data
end

function clsFight:GetFightType()
	return self.__save.fightType
end

function clsFight:GetFastFightState()
	return self.__temp.IsFastFight
end

function clsFight:SetFastFightState(data)
	self.__temp.IsFastFight = data
	if data and self:GetState() == "wait" then
		self:BeginRound()
	end
end

function clsFight:GetBossName(type)
	local pos = self.__save[type].boss
	local fighter = self:GetOneFighter(type, pos)
	if not fighter then
		return "NPC"
	end
	return fighter:GetName()
end

function clsFight:GetBossId(type)
	local pos = self.__save[type].boss
	local fighter = self:GetOneFighter(type, pos)
	if not fighter then
		return "NPC"
	end
	return fighter:GetId()
end

function clsFight:GetBossPos(type)
	return self.__save[type].boss
end
--记录上次出手的战斗方，用以计算连击
function clsFight:SetLastAttack(type)
	if self.__temp.lastAttack ~= type then
		self:SetCombo(0)
	end
	self.__temp.lastAttack = type
end

function clsFight:GetLastAttack()
	return self.__temp.lastAttack
end

--计算攻击后双方士气值增减量
function clsFight:CalMorale(type, data)
	local sumHp = self:GetInitSumHp(type)
	return math.ceil(data * 50/ sumHp)
end

function clsFight:ModMorale(type, damage)
	local targetType = GetOppositeType(type)
	local morale = self:CalMorale(targetType, damage)
	self:SetMorale(targetType, -morale)			--被攻击方减少士气
	self:SetMorale(type, math.floor(morale / 2))	--攻击方增加一半的士气
end

function clsFight:SetMorale(type, data)
	self.__save[type].morale = (self.__save[type].morale or INIT_MORALE_NUM) + data
	if self.__save[type].morale < 0 then 
		self.__save[type].morale = 0
	elseif self.__save[type].morale > 200 then 
		self.__save[type].morale = 200
	end
end

function clsFight:GetMorale(type)
	return self.__save[type].morale
end

function clsFight:ResetMorale(type)
	self.__save[type].morale = INIT_MORALE_NUM
end


function clsFight:SetUserList(data)
	for _, roleId in pairs(data) do
		self.__temp.userList[roleId] = 0		-- 1 客户端动画播放完毕
	end
end

function clsFight:GetUserList()
	return self.__temp.userList or {}
end

function clsFight:UpdateUserState(roleId, state)
	if not self.__temp.userList[roleId] or self:GetMinTime() > 0 then 
		return false
	end
	self.__temp.userList[roleId] = state
	return true
end

function clsFight:IsAllUserReady()
	for k, v in pairs(self.__temp.userList) do
		if v == 0 then
			return false
		end
	end
	return true
end

function clsFight:IsNonUserExist()
	for k in pairs(self.__temp.userList) do
		if USER.GetOnlineUser(k) then
			return false
		end
	end
	return true
end

function clsFight:GetAliveList(type)
	return self.__save.alive[type]
end

function clsFight:DelOneAliveMember(type, pos)
	self.__save.alive[type] = self.__save.alive[type] or {}
	self.__save.alive[type][pos] = nil
end

function clsFight:SetCombo(data)
	self.__save.combo = data
end

function clsFight:ModCombo(type, flag)
	if self:GetLastAttack() == type then
		self.__save.combo = (self.__save.combo or 0) + 1
	else
		self.__save.combo = 1
	end
	self:SetComboChange(true)
end

function clsFight:GetCombo()
	return self.__save.combo or 0
end

function clsFight:GetOneFighter(type, pos)
	return self.__save[type].list[pos]
end


function clsFight:IsComboChage()
	return self.__temp.comboChange
end

function clsFight:SetComboChange(flag)
	self.__temp.comboChange = flag
end

function clsFight:DelOneFighter(type, pos)
	local fighter = self.__save[type].list[pos]
	fighter:Destroy()
	self.__save[type].list[pos] = nil
end

--修改出手状态
function clsFight:SetAttackState(type, pos, state)
	for k, v in pairs(self.__save.realActList) do 
		if v.pos == pos and v.type == type then 
			v.state = state
		end
	end
end


--从出手顺序表中移除已经死亡的玩家
function clsFight:RemoveActList(type, pos)
	for k, v in pairs(self.__save.realActList) do
		if v.type == type and v.pos == pos then
			table.remove(self.__save.realActList, k)
		end
	end
end

--下一次的出手顺序是否改变
function clsFight:IsOrderChange()
	return self.__save.orderFlag
end

function clsFight:SetOrderChange(data)
	self.__save.orderFlag = data
end


--回合后减少状态持续效果
function clsFight:ProStateAfterRound(fighter)
	if fighter:IsDead() then return end
	local updateFlag
	for k, v in pairs(fighter:GetStateList()) do
		if k ~= "shield" and v.isNew ~= 1 then
			v.times = v.times - 1
			if v.times <= 0 then
				fighter:DelState(k)
				updateFlag = true
				if k == "sp_up" or k == "sp_down" then
					self:SetOrderChange(true)
				end
			end
		end
		v.isNew = 0			--isNew = 1 代表给本回合行动者新增的状态，新状态在行动者本回合不会减少持续效果
	end
	return updateFlag
end

--处理回合开始时的状态效果
function clsFight:ProStateBeforeRound(fighter)
	local flag						--跳过行动标志
	local updateFlag				--更新标志
	local damage					--所受伤害
	local bleedInfo = fighter:IsStateExist("bleed") or fighter:IsStateExist("poison")
	if bleedInfo then
		fighter:SetHp(-bleedInfo.num)
		damage = -bleedInfo.num
		if fighter:GetHp() <= 0 then
			self:FighterKilled(fighter)
			flag = true
		end
		updateFlag = true
	end
	if not fighter:IsDead() and (fighter:IsStateExist("spun") or fighter:IsStateExist("frozen") or fighter:IsStateExist("scared")) then
		flag = true
	end
	if updateFlag then
		self:SaveAttackAction(fighter, nil, nil, damage, 1)
		self:SendFightUpdate()
	end
	return flag
end


function clsFight:SetCurFighter(fighter)
	self.__temp.curFighter = fighter
end

function clsFight:GetCurFighter()
	return self.__temp.curFighter
end

--处理一次战斗
function clsFight:ProOneAttack(type, pos)
	local fighter = self:GetOneFighter(type, pos)
	self:SetCurFighter(fighter)
	fighter:SetActionRounds()
	local flag = self:ProStateBeforeRound(fighter)		--处理回合开始时的状态效果
	if not flag then
		if fighter:GetFurySkill() and self:GetCycle() >= fighter:GetFuryRounds() then
			self:ProFurySkill(fighter)
		else
			self:CalAttackMode(fighter)
		end
		self:ProStateAfterRound(fighter)
		self:SendFightUpdate()
	elseif not fighter:IsDead() then
		if self:ProStateAfterRound(fighter) then
			self:SaveAttackAction(fighter, nil, nil, nil, 1)
			self:SendFightUpdate()
		end
	end
	self:SetAttackState(type, pos, ACTION_DONE)		--设置出手状态为已出手
	self:ProExtraAttackList()			--处理本次出手中引起的额外攻击
	fighter:ReduceSkillCdTime()
	if self:IsOrderChange() then
		self:CalAttackOrder()
		self:SetOrderChange()			--顺序重新计算之后重置状态
--		self:SendOrderList()
	end
end

local RestoreSkill = {["302001"] = true, ["302002"] = true}
--skillType 攻击技能 辅助技能
function clsFight:GetActiveSkill(fighter, skillType)
	local list = {}
	if skillType == "passive" then
		local flag = self:GetMinHpTarget(fighter:GetType(), RESTORE_MIN_LOST_RATE)		--有人缺血时优先使用补血技能
		if flag then
			for id in pairs(RestoreSkill) do
				local info = fighter:IsSkillValid(id)
				if info then
					table.insert(list, {skillId = id, Lv = info.Lv})
				end
			end
		end
	end
	if #list < 1 then
		for k, v in pairs(fighter:GetSkillByType(skillType)) do
			if v.cdTime <= 0 then
				table.insert(list, {skillId = k, Lv = v.Lv})
			end
		end
	end
	if #list < 1 then
		return
	end
	local index = math.random(1, #list)
	fighter:SetSkillCdTime(list[index].skillId)
--testcode
	local info = BASESKILL.GetSkillInfo(list[index].skillId)
	if info.atkAction >= 350023 then
		return
	end
--testcode	
	return list[index].skillId, list[index].Lv
end

--玩家死亡
function clsFight:FighterKilled(fighter)
	if fighter:IsDead() then
		return
	end
	local type = fighter:GetType()
	local pos = fighter:GetPos()
	fighter:SetDead(self:GetRound())
	
	self:RemoveActList(type, pos)
	self:DelOneAliveMember(type, pos)		--从幸存列表中删除死亡角色
	self:RmSkillLinker(type, pos)		--移除死亡的联动者
end


--处理复活
function clsFight:ProRevival()
	local list = self:GetRevival()
	for _, v in pairs(list) do
		local fighter = self:GetOneFighter(v.type, v.pos)
		self:SaveAttackAction(fighter, "revival", nil, fighter:GetHp(), 1)
		fighter:RestoreAllHp()		--临时写的，技能出来后需要改
		self:SaveTargetAction(v.type, v.pos, "revival")
		self:SendFightUpdate()
	end
	self.__temp.revival = {}
end

--额外攻击（counter 反击 batter 连续攻击）
function clsFight:SetExtraAttackList(mode, attacker, target)
	self.__temp.extraAtkList = self.__temp.extraAtkList or {}
	if not self:IsTargetInRange(attacker, target) then
		return
	end
	table.insert(self.__temp.extraAtkList, {mode = mode, atkType = attacker:GetType(), atkPos = attacker:GetPos(), targetType = target:GetType(), targetPos = target:GetPos()})
end

function clsFight:ProExtraAttackList()
	for k, v in ipairs(self.__temp.extraAtkList or {}) do
		local attacker = self:GetOneFighter(v.atkType, v.atkPos)
		local target = self:GetOneFighter(v.targetType, v.targetPos)
		if not attacker:IsDead() and not target:IsDead() then
			self:SaveAttackAction(attacker, v.mode)
			if v.mode == "counter" then
				self:NormalAttack(attacker, v.targetPos, true, true)
			else
				self:NormalAttack(attacker, v.targetPos, nil, true)
			end
			self:SendFightUpdate()
		end
	end
	self.__temp.extraAtkList = {}
end


function clsFight:ProBufferOnAttack(attacker, target, damage, isFurySkill)
	local damage, effect = damage, nil
	local shield = target:IsStateExist("shield")
	if shield then		--魔法盾
		local curNum = shield.num
		shield.num = shield.num - damage
		if shield.num <= 0 then
			target:DelState("shield")
		end
		damage = math.max(0, damage - curNum)
	end
	if not attacker:IsDead() and damage > 0 then
		local drainInfo = attacker:IsStateExist("drains")
		if drainInfo then
			local restored = math.ceil(damage * drainInfo.rate)
			attacker:SetHp(restored)
			self:SaveTargetAction(attacker:GetType(), attacker:GetPos(), nil, restored)		--吸血时攻击方无动作
		end
	end
	if not isFurySkill then
		local reboundInfo = target:IsStateExist("hurt_rebound")
		if not attacker:IsDead() and reboundInfo then		--反弹（完全避免伤害）
			local rebound = math.ceil(damage * reboundInfo.rate)
			attacker:SetHp(-rebound)
			self:ModMorale(target:GetType(), rebound)			
			if attacker:GetHp() <= 0 then
				self:SetRage(attacker:GetType(), "dead")
				self:FighterKilled(attacker)
			end
			self:SaveTargetAction(attacker:GetType(), attacker:GetPos(), "beat", -rebound)
			damage = 0
			effect = "rebound"
		end
		local feedbackInfo = target:IsStateExist("hurt_back")
		if not attacker:IsDead() and feedbackInfo then		--反震（不能避免伤害）
			local feedback = math.ceil(damage * feedbackInfo.rate)
			attacker:SetHp(-feedback)
			self:ModMorale(target:GetType(), feedback)			
			if attacker:GetHp() <= 0 then
				self:SetRage(attacker:GetType(), "dead")
				self:FighterKilled(attacker)
			end
			self:SaveTargetAction(attacker:GetType(), attacker:GetPos(), "beat", -feedback)
			effect = "feedback"
		end
	end
	return damage, effect
end

--普通攻击
function clsFight:NormalAttack(fighter, pos, counterFlag, batterFlag)
	local type = fighter:GetType()
	local targetType = GetOppositeType(type)
	local targetPos = pos or self:CalTargetPos("normal", fighter:GetJob(), targetType)
	self:SaveTargetPos(targetType, targetPos)
	local target = self:GetOneFighter(targetType, targetPos)	
	if target then
		local mode = self:CalAttackFlag(fighter, target)
		if mode == "miss" or mode == "block" then
			self:SaveTargetAction(targetType, targetPos, mode, nil)
			self:SetCombo(0)
			return
		end
		local damage = 0
		if mode ~= "block" then
			local atk = fighter:GetPhyscAtk()
			local def = target:GetPhyscDef()
			if fighter:GetAtkMode() == "magic" then
				atk = fighter:GetMagicAtk()
				def = target:GetMagicDef()
				if mode ~= "crush" and fighter:GetMagicDefIgnore() then		--忽视法术防御力
					def = def * (1 - fighter:GetMagicDefIgnore())
				end
			end
			local downInfo = target:IsStateExist("atk_down")
			if downInfo then
				atk = atk - downInfo.num
			end
			local correct = self:CalCorrectPoint(fighter:GetIdentity(), target:GetIdentity())		
			damage = self:CalBaseDamage("normal", atk, def, correct, fighter:GetLv(), target:GetLv(), mode)
			damage = self:CalFinalDamage(fighter, target, damage)
			local morale = self:GetMorale(type)
			local add = (morale - 100) / 100
			if add >= 0 then
				damage = damage * (1 + (add > 0.5 and 0.5 or add))
	--		else
	--			damage = damage * (1 + (add < -0.5 and -0.5 or add))
			end
			damage, effect = self:ProBufferOnAttack(fighter, target, math.ceil(damage))
		end
		target:SetHp(-damage)
		self:SetLastAttack(type)
		self:ModMorale(type, damage)			--计算士气值
		self:ModCombo(type, "normal")						--计算连击
		self:ModTotalDamage(type, damage)
		self:SetRage(targetType, rageModeList[mode] and mode or "attack")				--计算怒气值
		if target:GetHp() <= 0 then
			self:SetRage(targetType, "dead")
			self:FighterKilled(target)
		end
		self:SaveTargetAction(targetType, targetPos, mode, -damage, effect)
		if not fighter:IsDead() and not target:IsDead() then
			if not counterFlag and target:GetCounterRate() >= math.random() then
				self:SetExtraAttackList("counter", target, fighter)
			end
			if not batterFlag and fighter:GetBatterRate() >= math.random() then
				self:SendFightUpdate()
				self:SaveAttackAction(fighter, "batter")
				self:NormalAttack(fighter, targetPos, true, true)
			end
		end
	end
end

--计算攻击伤害
function clsFight:CalBaseDamage(type, attack, defence, correct, atkLv, defLv, mode, rate)
	local damage = 0
	if mode == "block" then
		return damage
	elseif mode == "crush" then
		defence = math.ceil(defence * 0.5)
	end
	if attack <= defence and atkLv > defLv then
		local extend
		if type == "skill" then 
			extend = math.random(10, 20)
		else
			extend = math.random(5, 10)
		end
		damage = (atkLv - defLv) + extend
	elseif attack <= defence and atkLv <= defLv then
		damage = math.random(5, 10)
	else
		damage = (attack - defence) * correct * math.max((1 + (atkLv - defLv) / SERVER_ROLE_MAX_LV), 0.8)
	end
	if mode == "fatal" then
		damage = damage * 1.5
	end
	return math.ceil(damage)
end


function clsFight:SkillAttack(fighter, skillId, skillLv, isFurySkill)
	local type = fighter:GetType()
	local targetType = SKILL.GetAttackTargetType(type, skillId)
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	local targetPos
	if skillInfo.range == 7 then
		targetPos = self:GetMinHpTarget(targetType)
	elseif skillInfo.range == 1 then
		targetPos = fighter:GetPos()
	elseif skillInfo.range == 2 or skillInfo.range == 3 then
		targetPos = "5"
	else
		targetPos = self:CalTargetPos("skill", fighter:GetJob(), targetType)
	end
	self:SaveTargetPos(targetType, targetPos)
	self:SetFightResource("skill", skillId)
	local effectList = SKILL.GetSkillEffect(self, fighter, skillId, skillLv, targetType, targetPos)
	for k, v in ipairs(effectList) do
		local target = self:GetOneFighter(targetType, v.pos)
		if target then
			if v.action == "miss" and #effectList <= 1 then
				self:SetCombo(0)
			end
			self:SetLastAttack(type)
			local damage = v.damage
			if damage then
				if v.action == "restore" then
					target:SetHp(-damage)
				else
					damage, effect = self:ProBufferOnAttack(fighter, target, math.ceil(damage), isFurySkill)
					self:ModMorale(type, damage)			--计算士气值
					self:ModCombo(type, "skill")						--计算连击
					self:SetRage(targetType, rageModeList[v.action] and v.action or "attack")		--计算怒气值
					self:ModTotalDamage(type, damage)
					target:SetHp(-damage)
					if target:GetHp() <= 0 then
						self:SetRage(targetType, "dead")
						self:FighterKilled(target)
					end
				end
			end
			if v.buffer and not target:IsDead() then
				for _, buff in pairs(v.buffer) do
					SKILL.ProSkillEffectBuffer(target, buff.id, buff.last, buff.num, buff.rate, damage, self:GetCurFighter() == target)
					if buff.id == "1" or buff.id == "2" then
						self:SetOrderChange(true)
					end
				end
			end
			self:SaveTargetAction(targetType, v.pos, v.action, v.damage and -damage or nil, effect)
			if skillInfo.type == 1 then
				if not fighter:IsDead() and not target:IsDead() and target:GetCounterRate() > math.random() then
					self:SetExtraAttackList("counter", target, fighter)
				end
			end
		end
	end
end

--联动技能攻击
function clsFight:LinkAttack(fighter, skillId, skillLv)
	local type = fighter:GetType()
	local targetType = SKILL.GetAttackTargetType(type, skillId)
	local targetPos = self:GetSavedTargetPos()
	local effectList = SKILL.GetSkillEffect(self, fighter, skillId, skillLv, targetType, targetPos, true)
	for k, v in ipairs(effectList) do
		local target = self:GetOneFighter(targetType, v.pos)		
		if target and v.damage then
			local damage = math.ceil(v.damage)
			if v.action == "restore" then
				target:SetHp(-damage)
			else
				local shield = target:IsStateExist("shield")
				if shield then		--魔法盾
					local curNum = shield.num
					shield.num = shield.num - damage
					if shield.num <= 0 then
						target:DelState("shield")
					end
					damage = math.max(0, damage - curNum)
				end
				self:ModMorale(type, damage)			--计算士气值
				self:ModCombo(type, "link")						--计算连击
				self:SetRage(targetType, rageModeList[v.action] and v.action or "attack")		--计算怒气值
				self:ModTotalDamage(type, damage)
				target:SetHp(-damage)
				if target:GetHp() <= 0 then
					self:SetRage(targetType, "dead")
					self:FighterKilled(target)
				end
			end
			self:AddLinkerDamage(targetType, v.pos, -damage)
		end
	end
end

--boss必杀技
function clsFight:ProFurySkill(fighter)
	local skillId = fighter:GetFurySkill()
	self:SaveAttackAction(fighter, "skill", skillId)
	self:SetFightResource("skill", skillId)
	return self:SkillAttack(fighter, skillId, 1, true)
end

--绝技攻击
function clsFight:StuntAttack(fighter, skillId, skillLv)
	local type = fighter:GetType()
	local atkAction
	local identity = fighter:GetIdentity()
	if identity == "pet" then
		self:ResetRage(type)
		atkAction = "stunt"
	end
	local targetType = SKILL.GetAttackTargetType(type, skillId)
	self:SaveAttackAction(fighter, atkAction, skillId)
	self:SetFightResource("skill", skillId)
	self:SaveTargetPos(targetType, "5")
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	local boss = self:GetOneFighter(type, self:GetBossPos(type))
	local attacker = identity == "pet" and boss or fighter
	local effectList = SKILL.GetSkillEffect(self, attacker, skillId, skillLv, targetType, "5")
	for k, v in ipairs(effectList) do
		local target = self:GetOneFighter(targetType, v.pos)
		if target then
			if v.action == "miss" and #effectList <= 1 then
				self:SetCombo(0)
			end
			self:SetLastAttack(type)
			local damage = v.damage
			if damage then
				if v.action == "restore" then
					target:SetHp(-damage)
				else
					damage, effect = self:ProBufferOnAttack(attacker, target, math.ceil(damage), true)
					self:ModMorale(type, damage)			--计算士气值
					self:ModCombo(type, "stunt")						--计算连击
					self:SetRage(targetType, rageModeList[v.action] and v.action or "attack")		--计算怒气值
					self:ModTotalDamage(type, damage)
					target:SetHp(-damage)
					if target:GetHp() <= 0 then
						self:SetRage(targetType, "dead")
						self:FighterKilled(target)
					end
				end
			end
			if v.buffer and not target:IsDead() then
				for _, buff in pairs(v.buffer) do
					SKILL.ProSkillEffectBuffer(target, buff.id, buff.last, buff.num, buff.rate, damage, self:GetCurFighter() == target)
					if buff.id == "1" or buff.id == "2" then
						self:SetOrderChange(true)
					end
				end
			end
			self:SaveTargetAction(targetType, v.pos, v.action, v.damage and -damage or nil, effect)
		end
	end
	self:SendFightUpdate("stunt")
end


function clsFight:CheckFightOver()
	local function check_over(type)
		local list = self:GetAliveList(type)
		if table.size(list) <= 0 then
			if self:IsReinForceExist(type) then
				return STATE_REIFORCE, type
			end
			self:SetWinner(type == "attack" and "defend" or "attack")
			return STATE_ALL_OVER
		end
	end
	local ret, rst = check_over("attack")
	if ret then
		return ret, rst
	end
	local ret, rst = check_over("defend")
	if ret then
		return ret, rst
	end
	if self:GetCycle() >= MAX_ROUND then
		self:MaxRound()
		return STATE_ALL_OVER
	end
	return false
end

--计算初始总血量
function clsFight:CalInitSumHp()
	self.__temp.sumHp = {}
	local function cal_init_sum(type)
		local sum = 0
		for k, v in pairs(self.__save[type].list) do
			if v:GetLabel() == "visible" then
				sum = sum + v:GetHp()
			end
		end
		self.__temp.sumHp[type] = sum
	end
	cal_init_sum("attack")
	cal_init_sum("defend")
end

function clsFight:GetInitSumHp(type)
	return self.__temp.sumHp[type]
end
--计算剩余总血量
function clsFight:CalSumHp(type)
	local sumHp = 0
	for k in pairs(self:GetAliveList(type)) do 
		local fighter = self:GetOneFighter(type, k)
		if not fighter:IsDead() then
			sumHp = sumHp + fighter:GetHp()
		end
	end
	return sumHp
end

function clsFight:MaxRound()
	if self:GetGroupIdentt("defend") ~= "user" then
		self:SetWinner("defend")
	else
		local atkHp = self:CalSumHp("attack")
		local defHp = self:CalSumHp("defend")
		if atkHp > defHp then
			self:SetWinner("attack")
		else
			self:SetWinner("defend")
		end
	end
end

--计算攻击方式
function clsFight:CalAttackMode(fighter)	
	local mode = "normal"
--	if fighter:IsSkillRound() then
	if true then
--	if true then
--		local skillList = fighter:GetValidSkillList()
		local skillList = {{skillId = "301005", skillLv = 1}, {skillId = "302011", skillLv = 1}}
		if #skillList > 0 then
			local chosen = skillList[math.random(1, #skillList)]
			local skillId, skillLv = chosen.skillId, chosen.skillLv
			local action = "skill"
			local skillInfo = BASESKILL.GetSkillInfo(skillId)
			if skillInfo.type == MAGIC_SKILL_TYPE then
				action = "magic"
			end
			self:SaveAttackAction(fighter, action, skillId)
			self:SkillAttack(fighter, skillId, skillLv)
		--	local pos = self:GetSkillLinker(fighter:GetType(), skillId, fighter:GetPos())
			local pos = self:GetLinkerPos(fighter:GetType(), fighter:GetPos())
			if pos then
				local linker = self:GetOneFighter(fighter:GetType(), pos)
				self:SaveAttackAction(linker, action, skillId, nil, nil, 1)
				local skillList = linker:GetActiveSkill()
			--	self:LinkAttack(linker, skillId, skillList[skillId].Lv)
				self:LinkAttack(linker, skillId, skillLv)
			end
			if action ~= "magic" then		--辅助技能之后可以再普通攻击1次
				return
			end
			self:SendFightUpdate()			
		end
	end
	self:SaveAttackAction(fighter, "normal")
	self:NormalAttack(fighter)
end

function clsFight:GetLinkerPos(type, except)
	for k, v in pairs(self:GetFighterList(type)) do
		if v:GetLabel() == "visible" and not v:IsDead() and k ~= except then
			return k
		end
	end
end

--获取战斗修正值
function clsFight:CalCorrectPoint(atkId, defId)
	local correct = math.ceil(math.random() * 10) / 100
	if atkId == "boss" or defId == "boss" then
		correct = correct + 0.6
	elseif atkId == "npc" or defId == "npc" then
		correct = correct + 0.7
	else
		correct = correct + 0.5
	end	
	return correct
end

function clsFight:CalAttackFlag(fighter, target, nonMiss)
	local flag = "beat"
	if not nonMiss and target:GetJinkRate(fighter:GetHitRate()) >= math.random() then
		flag = "miss"
	elseif fighter:GetAtkMode() ~= "magic" and target:GetBlockRate() >= math.random() then
		flag = "block"
	elseif fighter:GetFatalRate(fighter:GetAntiFatalRate()) >= math.random() then
		flag = "fatal"
	elseif fighter:GetCrushRate() >= math.random() then
		flag = "crush"
	end
	return flag
end

--获取Hp损失最多的目标
function clsFight:GetMinHpTarget(type, hpRate)
	local list = {}
	local all = {}
	for k, v in pairs(self:GetFighterList(type)) do
		if v:GetLabel() == "visible" and not v:IsDead() then
			local lost = v:GetHpMax() - v:GetHp()
			if lost > v:GetHpMax() * (hpRate or 0) then
				table.insert(list, {pos = k, lost = lost})
			end
			table.insert(all, k)
		end
	end
	if #list <= 0 then
		return all[math.random(1, #all)]
	end
	table.sort(list, function(a, b) return a.lost > b.lost end)
	return list[1].pos
end
--[[
function clsFight:IsTargetInRange(attacker, target)
	local jobInfo = JOB.GetJobInfoById(attacker:GetJob())
	if jobInfo.range == 1 then		--远程职业
		return true
	end
	local type = target:GetType()
	for i = 0, 2 do
		for j = 3, 1, -1 do
			local pos = tostring(i * 3 + j)
			local fighter = self:GetOneFighter(type, pos)
			if fighter and not fighter:IsDead() then
				if pos == target:GetPos() then
					return true
				end
				break
			end
		end
	end
	return false
end]]

function clsFight:IsTargetInRange(attacker, target)
	local type = target:GetType()
	local posList = {}
	for i = 3, 1, -1 do
		for j = 0, 2 do
			local pos = tostring(i + j * 3)
			local fighter = self:GetOneFighter(type, pos)
			if fighter and not fighter:IsDead() then
				posList[pos] = 1
			end
		end
		if table.size(posList) > 0 then
			break
		end
	end
	if posList[target:GetPos()] then
		return true
	end
	return false
end


--攻击目标位置
function clsFight:CalTargetPos(method, attackerJob, targetType)
	local jobInfo = JOB.GetJobInfoById(attackerJob)
	local attackList = {restraint = {}, nonrestraint = {}}	--可被攻击的所有位置
	local sneerList = {}		--拥有嘲讽状态的优先攻击
	for i = 3, 1, -1 do
		for j = 0, 2 do
			local pos = tostring(i + j * 3)
			local target = self:GetOneFighter(targetType, pos)
			if target and not target:IsDead() then
				if jobInfo.restraint[target:GetJob()] then
					table.insert(attackList.restraint, pos)
				else
					table.insert(attackList.nonrestraint, pos)
				end
				if target:IsStateExist("sneer") then
					table.insert(sneerList, pos)
				end
			end
		end
		if method ~= "skill" or jobInfo.range ~= 1 then		--普通攻击和战士的单体技能受站位限制
			if #attackList.restraint + #attackList.nonrestraint > 0 then
				break
			end
		end
	end
	if #sneerList > 0 then
		return sneerList[math.random(1, #sneerList)]
	end	
	if #attackList.restraint + #attackList.nonrestraint <= 0 then
		DATA.log(string.format("method = %s, attackerJob = %s, targetType = %s, attackList = %s, fightId = %s.", method, attackerJob, targetType, JSON.encode(attackList), self:GetFightId()), "clsFight.log")
	end
	local temp = attackList.nonrestraint
	if #attackList.nonrestraint < 1 or (#attackList.restraint > 0 and math.random() < 0.75) then	--75%概率攻击克制目标
		temp = attackList.restraint
	end
	local anemia = {}		--兵力少于25%
	local nonanemia = {}
	for _, v in pairs(temp) do 
		local target = self:GetOneFighter(targetType, v)
		if target:GetHp() <= target:GetHpMax() * 0.25 then
			table.insert(anemia, v)
		else
			table.insert(nonanemia, v)
		end
	end
	local targetList = nonanemia	--最终攻击目标
	if #nonanemia < 1 or (#anemia > 0 and math.random() < 0.75) then
		targetList = anemia
	end
	if #targetList <= 0 or not targetList then
		DATA.log(string.format("attackerJob = %s, jobInfo = %s,targetType = %s, attackList = %s, fightId = %s.", attackerJob, JSON.encode(jobInfo), targetType, JSON.encode(attackList), self:GetFightId()), "clsFight.log")
	end
	return targetList[math.random(1, #targetList)]
end

function clsFight:GetFightInfo(type)
	local sData = {}
	for k, v in pairs(self.__save[type].list) do
		table.insert(sData, v:GetFighterInfo())
	end
	return sData
end

function clsFight:StartCallOut()
	self.__save.callout = CALLOUT.ObjCallOut(self.__temp.callout, clsFight.IsRoundOver, 1, self)
end

function clsFight:IsRoundOver()
	local state = self:GetState()
	if state ~= "over" then 
		self:StartCallOut()
	end
	if state ~= "wait" then
		return
	end
	if self:IsNonUserExist() then
		self:SetFastFightState(true)
		return self:BeginRound()
	end
	self.__save.minTm = self.__save.minTm - 1
	self.__save.maxTm = self.__save.maxTm - 1
	local function is_round_over()
		if self.__save.minTm > 0 then
			return false
		end
		if self.__save.maxTm <= 0 then 
			return true
		end
		return self:IsAllUserReady()
	end
	if is_round_over() then
		self:BeginRound()
	end
end

function clsFight:InitRound()
	self.__save.round = self.__save.round + 1
	for k, v in pairs(self.__temp.userList) do
		self.__temp.userList[k] = 0
	end
	self.__save.minTm = 0
	self.__save.maxTm = 0
	for k, v in pairs(self.__save.realActList) do
		if v.state ~= ACTION_DONE then
			return
		end
	end
	self:SetCycle(1)
	self:InitRealActList()
end

--城池技能发动
function clsFight:ProCityStunt(type)
	local cityRole = self:GetOneFighter(type, "city")
	if cityRole and not cityRole:IsActDone() then
		cityRole:SetActDone(true)
		self:SetFightResource("model", cityRole:GetModelId(), cityRole:GetIdentity())
		self:StuntAttack(cityRole, cityRole:GetStuntId(), cityRole:GetStuntLv())
		return 1
	end
	return
end

function clsFight:ProPetStunt(type)
	local petRole = self:GetOneFighter(type, "pet")
	if petRole and self:GetRage(type) >= MAX_RAGE then
		self:SetFightResource("model", petRole:GetModelId(), petRole:GetIdentity())
		self:StuntAttack(petRole, petRole:GetStuntId(), petRole:GetStuntLv())
		return 1
	end
	return
end

--回合开始
function clsFight:BeginRound()
	local state, reinforceType = self:CheckFightOver()
	if state == STATE_ALL_OVER then
		self:SetState("over")
		return self:FightOverProc()
	end
	self:InitRound()
	self:SendRoundBegin()
	self:SetState("process")
	local flag = false
	if state == STATE_REIFORCE then
		self:ProReinForce(reinforceType)
		flag = 2
	end
	if self:GetFightType() == "cityAttack" or self:GetFightType() == "pillage" then
		if not flag then
			flag = self:ProCityStunt("attack")
		end
		if not flag then
			flag = self:ProCityStunt("defend")
		end
	end
	if not flag and self:GetRage("attack") >= MAX_RAGE then
		flag = self:ProPetStunt("attack")
	end
	if not flag and self:GetRage("defend") >= MAX_RAGE then
		flag = self:ProPetStunt("defend")
	end
	if not flag then
		for k, v in ipairs(self.__save.realActList) do
			if v.state == ACTION_WAIT then
				self:ProOneAttack(v.type, v.pos)	--处理一次战斗
				flag = 1
				break
			end
		end
	end
	if flag == 1 then
--		self:SendDrawBegin()
	end
	self:SetMinTime(MIN_ROUND_TIME)
	self:SetMaxTime(self.__save.minTm + MAX_ROUND_TIME)
--	self:SetState("wait")
--	self:RecordFightMinTime(self:GetCycle(), self:GetMinTime())
--	if self:GetFastFightState() then 
	return self:BeginRound()
--	end
end

--战斗结束处理
function clsFight:FightOverProc()
	self:GenFightPrize("win")
	self:GenFightPrize("lose")	
	self:RecordFightOver()
	
	self:SendFightDataStart()
	if self:GetFastFightState() then
		self:SendFightDataUpdate()
	else		
		self:SetState("send_wait")
		self:StartFightDataCallOut()
	end
end

function clsFight:RecordFightOver()
	local winner = self:GetWinner()
	local function record_fight_result(type)
		local result = type == winner and "win" or "lose"
		local sData	= self:GetFormatFightPrize(result)
		sData.fightId = self:GetFightId()
		sData.result = result
		sData.winner = self:GetBossName(winner)
	--	sData.loser = self:GetBossName(winner == "attack" and "defend" or "attack")
		self:RecordFightProcess(self:GetCycle() + 1, type, "c_fight_over", sData)
	end
	record_fight_result("attack")
	record_fight_result("defend")
end

function clsFight:RestoreUserTeamHp(type)
	local param = self:GetFightParam()
	if param.saveHp then
		local boss = self:GetOneFighter(type, self:GetBossPos(type))
		if not boss or boss:GetIdentity() ~= "user" then return end
		local roleId = boss:GetId()
		local user = USER.GetOnlineUser(roleId)
		local list = {}
		for k, v in pairs(self.__save[type].list) do
			if v:GetLabel() == "visible" then
				local buf = {}
				if v:GetId() ~= roleId then
					table.insert(list, v)
				end
			end
		end
		table.sort(list, function(a, b) return a:GetAptitude() > b:GetAptitude() or (a:GetAptitude() == b:GetAptitude()) and a:GetPos() < b:GetPos() end)
		table.insert(list, 1, boss)
		for k, v in ipairs(list) do
			if user then
				user:ProHpAfterFight(v:GetId(), v:GetHp(), param.restoreHp, param.restoreFlag)
			else
				OFFLINE.SetOfflineData(roleId, "fightHp", v:GetId(), v:GetHp(), param.restoreHp, restoreFlag)
			end
		end
	end
end


function clsFight:SendDrawBegin()
--	self:RecordFightProtocol("c_fight_draw", {fightId = self:GetFightId(), round = self:GetRound()})
end


function clsFight:SendFightUpdate(flag)
	local info = self:GetActionList()
	local sData = {fightId = self:GetFightId()}
	sData.attacker = info.attacker
	for k, v in pairs(sData.attacker) do
		v.targetType = info.targetType
		v.targetPos = info.targetPos
	end
	sData.target = info.target
	sData.rage = {attack = self:GetRage("attack"), defend = self:GetRage("defend")}
	sData.morale = {attack = self:GetMorale("attack"), defend = self:GetMorale("defend")}	
	if self:IsComboChage() then
		local combo = self:GetCombo()
		if combo >= MIN_COMBO_NUM then
			sData.combo = combo
		end
	end
	self:RecordFightProtocol("c_fight_update", sData)
	self.__temp.actionList = {}
	self:SetComboChange()			--每次update之后重置
end


function clsFight:InitRealActList()
	for k, v in pairs(self.__save.realActList) do
		v.state = ACTION_WAIT
	end
end
--计算出手顺序
function clsFight:CalAttackOrder()
	for k, v in pairs(self.__save.realActList) do
		local fighter = self:GetOneFighter(v.type, v.pos)
		v.speed = fighter:GetSpeed()
	end
	table.sort(self.__save.realActList, function (a, b)
		if a.speed == b.speed then
			a.correct = a.correct or math.random()
			b.correct = b.correct or math.random()
		end
		return a.speed > b.speed or (a.speed == b.speed and (a.correct  < b.correct))
	end)
end

function clsFight:SendOrderList()
	local list = {}
	local times = 0
	while #list < MAX_ORDER do
		for k, v in ipairs(self.__save.realActList) do
			if times > 0 or v.state == 0 then
				local fighter = self:GetOneFighter(v.type, v.pos)
				table.insert(list, {type = v.type, headId = fighter:GetHeadId(), name = fighter:GetName()})
			end
			if #list >= MAX_ORDER then
				break
			end
		end
		times = times + 1
	end
	self:RecordFightProtocol("c_fight_orderList", {fightId = self:GetFightId(), list = list})
end

function clsFight:SendRoundBegin()
	local sData = {round = self:GetRound(), fightId = self:GetFightId()}
	self:RecordFightProtocol("c_fight_round", sData)	
end

--计算最终伤害
function clsFight:CalFinalDamage(attacker, target, baseDamage)
	local damage = baseDamage
	local upInfo = attacker:IsStateExist("hurt_up")
	if upInfo then
		damage = damage + upInfo.num
	end
	damage = math.max(0, damage - target:GetHarmReduce())
	local rate = 1
	rate = rate + attacker:GetHarmDeep()
	local upRateInfo = attacker:IsStateExist("hurt_per_up")
	if upRateInfo then
		rate = rate + upRateInfo.rate
	end
	if attacker:GetAtkMode() == "physical" then
		rate = rate - target:GetPhyscResist()
	else
		rate = rate - target:GetMagicResist()
	end
	rate = rate - target:GetHurtDownRate()
	local downRateInfo = target:IsStateExist("hurt_per_down")
	if downRateInfo then
		rate = rate + downRateInfo.rate
	end
	if rate < 0 then
		print("clsFight.lua:CalFinalDamage, rate = ", rate, self:GetFightId(), attacker:GetName(), target:GetName())
		rate = 0
	end
	local formRate = self:IsFormationRestraint(attacker:GetType(), target:GetType())		--阵法克制
	rate = rate + (formRate or 0)
	return damage * rate
end

function clsFight:GetAllFighterSkillPicList()
	local list = {}
	local inList = {}
	local function add_to_list(type)
		for k, v in pairs(self.__save[type].list) do
			if v:GetLabel() == "visible" then
				for id in pairs(v:GetActiveSkill()) do
					local skillInfo = BASESKILL.GetSkillInfo(id)
					if skillInfo.atkAction and tonumber(skillInfo.atkAction) < 350023 then
						if not inList[skillInfo.atkAction] then
							table.insert(list, skillInfo.atkAction)
							inList[skillInfo.atkAction] = true
						end
					end
				end
				for id in pairs(v:GetPassiveSkill()) do
					local skillInfo = BASESKILL.GetSkillInfo(id)
					if skillInfo.atkAction and tonumber(skillInfo.atkAction) < 350023 then
						if not inList[skillInfo.atkAction] then
							table.insert(list, skillInfo.atkAction)
							inList[skillInfo.atkAction] = true
						end
					end
				end
			else
				local skillId = v:GetStuntId()
				local skillInfo = BASESKILL.GetSkillInfo(skillId)
				if not inList[skillInfo.atkAction] then
					table.insert(list, skillInfo.atkAction)
					inList[skillInfo.atkAction] = true
				end
			end
		end
	end
	add_to_list("attack")
	add_to_list("defend")
	return list
end

function clsFight:GetFightResourceList()
	local sData = {}
	sData.modelList = self.__temp.resource.model.list or {}
	sData.skillList = self.__temp.resource.skill.list or {}
	return sData
end

function clsFight:SetFightResource(type, id, identity)
	if not type or not id then
		return
	end
	local info = self.__temp.resource[type]
	info.inList = info.inList or {}
	info.list = info.list or {}
	if type == "skill" then
		local skillInfo = BASESKILL.GetSkillInfo(id)
		if skillInfo and skillInfo.atkAction then
			if not info.inList[skillInfo.atkAction] then
				info.inList[skillInfo.atkAction] = true
				table.insert(info.list, skillInfo.atkAction)
			end
		end
	elseif type == "model" then
		if not info.inList[id] then
			info.inList[id] = true
			table.insert(info.list, {modelId = id, identity = identity})
		end
	end
end


function clsFight:GetFormationInfo(type)
	local opposite = type == "attack" and "defend" or "attack"
	local buff = {}
	local formInfo_atk = self:GetFormation("attack")
	buff.attack = formInfo_atk.formId
	local formInfo_dfd = self:GetFormation("defend")
	buff.defend = formInfo_dfd.formId
	buff.restraint = 0
	if self:IsFormationRestraint(type, opposite) then
		buff.restraint = 1
	elseif self:IsFormationRestraint(opposite, type) then
		buff.restraint = -1
	end
	return buff
end

--战斗开始
function clsFight:RecordFightStart()
	local sData = {round = self:GetRound(), fightId = self:GetFightId(), locate = self:GetFightLocateInfo()}
	sData.attack = self:GetFightInfo("attack")
	sData.defend = self:GetFightInfo("defend")
	sData.reinforce = self:GetLeftReinForceNum("defend")
	sData.rage = {attack = self:GetRage("attack"), defend = self:GetRage("defend")}
	sData.morale = {attack = self:GetMorale("attack"), defend = self:GetMorale("defend")}
	sData.formation = {attack = self:GetFormationInfo("attack"), defend = self:GetFormationInfo("defend")}
	sData.against = {attack = self:GetBossName("attack"), defend = self:GetBossName("defend")}
	local user = USER.GetOnlineUser(self:GetBossId("attack"))
	if user then
		sData.fastFightTimes = user:GetDailyFastFightMaxNum() - user:GetDailyFastFightTimes()
	else
		sData.fastFightTimes = 0
	end
	sData.type = self:GetFightType()
	self:RecordFightProtocol("c_fight_start", sData)
end

--开始发送战斗数据
function clsFight:SendFightDataStart()
	local record = self:GetFightRecord()
	for roleId in pairs(self:GetUserList()) do
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			local type = roleId == self:GetBossId("defend") and "defend" or "attack"
			self:SendFightProtocol(roleId, type, 1)
		end
	end
end

--更新战斗数据
function clsFight:SendFightDataUpdate()
	self:SetState("send_over")
	self:RemoveFightDataCallOut()
	local record = self:GetFightRecord()
	for roleId in pairs(self:GetUserList()) do
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			local type = roleId == self:GetBossId("defend") and "defend" or "attack"
			for i = 2, #record do
				self:SendFightProtocol(roleId, type, i)
			end
		end
	end
	self:RestoreUserTeamHp("attack")		--恢复参战方兵力
	local winner = self:GetWinner()
	FIGHT.FightOverCallBack(self:GetFightId(), winner, self:GetFightPrize(winner == "attack" and "win" or "lose"), self:GetFightStats())
	self:SaveFightRecord()
end

--发送数据定时器
function clsFight:StartFightDataCallOut()
	self.__temp.fightDataTimer = CALLOUT.ObjCallOut(self.__temp.callout, clsFight.SendFightDataUpdate, MAX_ROUND_TIME, self)
end

function clsFight:RemoveFightDataCallOut()
	if self.__temp.fightDataTimer then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.fightDataTimer)
	end
end


function clsFight:RecordFightProtocol(proName, data)
	local cycle = self:GetCycle()
	local function record_process(type)
		local sData = table.copy(data)
		if data.rage then
			sData.rage = data.rage[type]
		end
		if data.formation then			
			sData.formation = data.formation[type]
		end
		self:RecordFightProcess(cycle, type, proName, sData)
	end
	record_process("attack")
	record_process("defend")
end

--记录每回合的技能释放总时间
function clsFight:RecordFightMinTime(round, timelong)
--[[	local index = round
	self.__temp.record[index] = self.__temp.record[index] or {}
	self.__temp.record[index].minTm = timelong]]
end

--记录每回合的战斗操作
function clsFight:RecordFightProcess(round, type, proName, data)
	local index = round
	self.__temp.record[index] = self.__temp.record[index] or {}
	self.__temp.record[index][type] = self.__temp.record[index][type] or {}
	table.insert(self.__temp.record[index][type], {proName = proName, proData = data})
end

function clsFight:GetFightRecord()
	return self.__temp.record
end

function clsFight:SendFightProtocol(roleId, type, index)
	local record = self:GetFightRecord()
	if index < 1 or index > #record then return end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		local buff = record[index][type]
		if index == 1 then
			local sData = buff[1].proData
			if not sData.resource then
				sData.resource = self:GetFightResourceList()
			end
			con:SendProtocol(buff[1].proName, sData)
		elseif index == #record then
			con:SendProtocol(buff[1].proName, buff[1].proData)
		else	
			con:SendProtocol("c_fight_occur", buff)
		end
	end
end

function clsFight:SaveFightRecord()
	local record = self:GetFightRecord()
	local saveData = {}
	for k, v in ipairs(record) do
		saveData[k] = v["attack"]
	end
	FIGHTREC.SaveFightRecord(self:GetFightId(), saveData)
end

--战斗附加参数
function clsFight:SetFightParam(param)
	self.__temp.param = param
end

function clsFight:GetFightStats()
	local buf = {}
	buf.defHpSum = self:GetInitSumHp("defend") 
	buf.atkHpSum = self:GetInitSumHp("attack")
	buf.atkHp = self:CalSumHp("attack")
	buf.atkDamage = self:GetTotalDamage("attack")
	return buf
end


function clsFight:ModTotalDamage(type, data)
	self.__temp.sumDamage[type] = math.max((self.__temp.sumDamage[type] or 0) + data, 0)
end

function clsFight:GetTotalDamage(type)
	return self.__temp.sumDamage[type] or 0
end

function clsFight:GetFightParam()
	return self.__temp.param
end

function clsFight:GetFightPrize(type)
	return self.__temp.prizeList[type] or {}
end

function clsFight:GetFormatFightPrize(type)
	local sData = {}
	local prize = self:GetFightPrize(type)
	sData.gold = prize.gold
	sData.nimbus = prize.nimbus
	sData.spar = prize.spar
	sData.feat = prize.feat
	sData.exp = prize.exp
	sData.item = prize.item
	return sData
end


function clsFight:SetFightPrize(type, prize)
	self.__temp.prizeList[type] = prize
end

function clsFight:GenFightPrize(type)
	local fightType = self:GetFightType()
	local prize = {}
	local param = self:GetFightParam()
	if fightType == "retreat" then
		if type == "win" then
			prize = RETREAT.GetRetreatPrize(param.Lv, param.double)
		end
	elseif fightType == "arena" then
		prize = ARENA.GetFightPrize(param.Lv, type)
	elseif fightType == "copy" then
		if type == "win" then
			prize = CLSCOPY.GetCopyFightPrize(param.Lv, param.teamId, self:GetBossId("attack"))
		end
	elseif fightType == "teamCopy" then
		if type == "win" then
			prize = CLSTEAMCOPY.GetTeamCopyPrize(param.Lv)
		end
	elseif fightType == "wildChest" then
		if type == "win" then
			prize = WILDCHEST.CalFightPrize(param.id)
		end
	elseif fightType == "pillage" then		
		if type == "win" then
			local rate = 1 + (param.attack.pillageIncrease or 0) - (param.defend.pillageReduce or 0)
			if not self:GetPetGuardState() then
				if math.random() <= self:GetPetGuardRate() then
					rate = rate * (1 - PET_GUARD_LOST_REDUCE_RATE)
					self:SetPetGuardState("lost_down")
				end
			end
			local buff = CLSCITY.CalPillageGot(param.Lv, type)
			prize.gold = math.floor(buff.gold * rate)
			prize.spar = math.floor(buff.spar * rate)
			prize.feat = buff.feat
		else
			if self:GetReinForceIndex("defend") > 0 then
				prize = CLSCITY.CalPillageGot(param.Lv, "lose2")
			else
				prize = CLSCITY.CalPillageGot(param.Lv, "lose")
			end
		end
		prize.petGuardState = self:GetPetGuardState()
	end
	self:SetFightPrize(type, prize)
end



-------------------------------------
--怪物增援
function clsFight:InitReinForceInfo(reinforceList)
	local function init_reinforce(type)
		self.__temp.reinforce[type] = {index = 0, list = {}}
		if reinforceList and reinforceList[type] then
			self.__temp.reinforce[type].list = reinforceList[type]
		end
	end
	init_reinforce("attack")
	init_reinforce("defend")
end

function clsFight:GetReinForceInfo(type)
	return self.__temp.reinforce[type]
end

--当前增援怪物索引（初始为0）
function clsFight:SetReinForceIndex(type, data)
	local info = self:GetReinForceInfo(type)
	info.index = info.index + data
end

function clsFight:GetReinForceIndex(type)
	local info = self:GetReinForceInfo(type)
	return info.index
end
--增援怪物列表
function clsFight:SetReinForceList(type, list)
	local info = self:GetReinForceInfo(type)
	info.list = list
end

function clsFight:GetReinForceList(type)
	local info = self:GetReinForceInfo(type)
	return info.list
end
--判断是否还有增援怪物
function clsFight:IsReinForceExist(type)
	local info = self:GetReinForceInfo(type)
	if not info.list then return false end
	local index = self:GetReinForceIndex(type)
	return info.list[index + 1]
end
--获取剩余增援怪物波数
function clsFight:GetLeftReinForceNum(type)
	return #self:GetReinForceList(type) - self:GetReinForceIndex(type)
end

function clsFight:ProReinForce(type)
	self:SetReinForceIndex(type, 1)
	local info = self:GetReinForceInfo(type)
	local reinforce = info.list[info.index]
	self:InitRealActList()
	local fightData = self:FormatFighterData(reinforce)
	self:InitFighterData(type, fightData)
	self:RecordReinforceUpdate(type)
	self:SetCycle(1)
end


--怪物增援
function clsFight:RecordReinforceUpdate(type)
	local sData = {round = self:GetRound(), fightId = self:GetFightId()}
	sData[type] = self:GetFightInfo(type)
	sData.reinforce = self:GetLeftReinForceNum(type)
	sData.rage = {attack = self:GetRage("attack"), defend = self:GetRage("defend")}
	self:RecordFightProtocol("c_fight_reinforce", sData)
end



--本命精灵舍命守护是否发生
function clsFight:SetPetGuardState(flag)
	self.__temp.petGuardState = flag
end

function clsFight:GetPetGuardState()
	return self.__temp.petGuardState
end

function clsFight:SetPetGuardRate(rate)
	self.__temp.petGuardRate = rate
end

function clsFight:GetPetGuardRate()
	return self.__temp.petGuardRate
end
