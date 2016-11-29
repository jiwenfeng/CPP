--人物基类

local INIT_POINT = 5		--初始属性点
local STR_TO_ATK = 5		--力量影响物理攻击
local STR_TO_DEF = 1		--力量影响物理防御
local INT_TO_ATK = 5		--智力影响法术攻击
local INT_TO_DEF = 1		--智力影响法术防御
local AGI_TO_DEF = 1		--敏捷影响双防
local AGI_TO_SPD = 0.5		--敏捷影响速度
local CAP_TO_DEF = 1		--统帅影响双防
local CAP_TO_HPM = 10		--统帅影响兵力上限

local MAX_SKILL_LV = 12
----------------------------------------------------------------------

clsChar = ROLE.clsRole:Inherit()

function clsChar:__init__()
	Super(clsChar).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsChar:initsavedata()
	self.__save.id = nil
	self.__save.name = nil
	self.__save.Lv = 1
	self.__save.exp = 0
	self.__save.info = {}
	self.__save.point = {}				--一级属性
	self.__save.attrib = {}				--二级属性
	self.__save.special = {}			--精英属性
	self.__save.skill = {}				--技能
	self.__save.equip = {}				--装备
end

function clsChar:inittempdata()

end

--Id
function clsChar:SetId(data)
	self.__save.id = data
end

function clsChar:GetId()
	return self.__save.id
end
--姓名
function clsChar:SetName(data)
	self.__save.name = data
end

function clsChar:GetName()
	return self.__save.name
end
--头像
function clsChar:SetHeadId(data)
	self.__save.info.headId = data
end

function clsChar:GetHeadId()
	return self.__save.info.headId
end

function clsChar:SetJob(data)
	self.__save.info.job = data
end

function clsChar:GetJob()
	return self.__save.info.job
end

--设置性别
function clsChar:SetSex(data)
	self.__save.sex = data
end

function clsChar:GetSex()
	return self.__save.sex
end

function clsChar:GetModelId()
	return self.__save.modelId
end

function clsChar:SetModelId(id)
	self.__save.modelId = id
	if self:GetRoleType() == "user" then
		USER.ModUserNickInfo(self:GetRoleId(), "modelId", id)
	end
end

--等级
function clsChar:SetLv(data)
	if tonumber(data) == 0 then
		return
	end
	self.__save.Lv = (self.__save.Lv or 0) + tonumber(data)
end

function clsChar:GetLv()
	return self.__save.Lv
end

function clsChar:SetExp(data, type)
	self.__save.exp = self.__save.exp + data
end

function clsChar:GetExp()
	self.__save.exp = self.__save.exp or 0
	return self.__save.exp
end

function clsChar:Upgrade()
	self:SetLv(1)
end

function clsChar:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsChar:Restore(data)
	assert(type(data) == "table")
	self.__save = data
end
----------------------------------------------------------------------------------
-- 一级属性
function clsChar:ModStrength(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.strength = self.__save.point.strength + data
	self:SetPhyscAtk("min", data * STR_TO_ATK, flag)
	self:SetPhyscAtk("max", data * STR_TO_ATK, flag)
	self:SetPhyscDef("base", data * STR_TO_DEF, flag)
	if not flag then
		self:SyncData("strength")
	end
end

function clsChar:SetStrength(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.strength = data
	if not flag then
		self:SyncData("strength")
	end
end

function clsChar:GetStrength()
	return self.__save.point.strength
end

function clsChar:ModIntellect(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.intellect = self.__save.point.intellect + data
	self:SetMagicAtk("min", data * INT_TO_ATK, flag)
	self:SetMagicAtk("max", data * INT_TO_ATK, flag)
	self:SetMagicDef("base", data * INT_TO_DEF, flag)
	if not flag then
		self:SyncData("intellect")
	end
end

function clsChar:SetIntellect(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.intellect = data
	if not flag then
		self:SyncData("intellect")
	end
end

function clsChar:GetIntellect()
	return self.__save.point.intellect
end

function clsChar:ModAgility(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.agility = self.__save.point.agility + data
	self:SetPhyscDef("base", data * AGI_TO_DEF, flag)
	self:SetMagicDef("base", data * AGI_TO_DEF, flag)
	self:SetSpeed(data * AGI_TO_SPD, flag)
	if not flag then
		self:SyncData("agility")
	end
end

function clsChar:SetAgility(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.agility = data
	if not flag then
		self:SyncData("agility")
	end
end

function clsChar:GetAgility()
	return self.__save.point.agility
end


function clsChar:ModCaptain(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.captain = self.__save.point.captain + data
	self:SetHpMax(data * CAP_TO_HPM, flag)
	self:SetPhyscDef("base", data * CAP_TO_DEF, flag)
	self:SetMagicDef("base", data * CAP_TO_DEF, flag)
	if not flag then
		self:SyncData("captain")
	end
end

function clsChar:SetCaptain(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.point.captain = data
	if not flag then
		self:SyncData("captain")
	end
end

function clsChar:GetCaptain()
	return self.__save.point.captain
end

function clsChar:GetLeftPoint()
	return self.__save.point.left
end

function clsChar:SetLeftPoint(data)
	assert(tonumber(data))
	self.__save.point.left = data
	self:SyncData("point")
end

function clsChar:ModLeftPoint(data)
	assert(tonumber(data))
	self.__save.point.left = self.__save.point.left + data
	self:SyncData("point")
end

function clsChar:InitPoint()
	self.__save.point.strength = 0
	self.__save.point.intellect = 0
	self.__save.point.agility = 0
	self.__save.point.captain = 0
	self.__save.point.left = INIT_POINT
end

function clsChar:GetPoint()
	return self.__save.point
end

function clsChar:ResetPoint(data)
	self.__save.point.strength = 0
	self.__save.point.intellect = 0
	self.__save.point.agility = 0
	self.__save.point.captain = 0
	self.__save.point.left = data 
end

---------------------------------------------------------------------------
--二级属性

function clsChar:InitAttrib()
	self.__save.attrib = {}
	self.__save.attrib.hpMax = 0
	self.__save.attrib.hp = 0
	self.__save.attrib.physcAtk = {min = 0, max = 0, add = 0}
	self.__save.attrib.physcDef = {base = 0, add = 0}
	self.__save.attrib.magicAtk = {min = 0, max = 0, add = 0}
	self.__save.attrib.magicDef = {base = 0, add = 0}
	self.__save.attrib.speed = 0
end

function clsChar:SetHpMax(data, flag)
	data = assert(tonumber(data))
	self.__save.attrib.hpMax = self.__save.attrib.hpMax + data
	
	if not flag then
		self:SyncData("hpMax")
	end
end

function clsChar:GetHpMax()
	return self.__save.attrib.hpMax
end

function clsChar:GetCurHpMax()
	local hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	if not hpMax or hpMax <= 0 then
		return self:GetHpMax()
	end
	return hpMax
end

function clsChar:SetHp(data, flag)
	data = assert(tonumber(data))
	self.__save.attrib.hp = math.ceil((self.__save.attrib.hp or 0) + data)
	if self.__save.attrib.hp < 1 then
		self.__save.attrib.hp = 1
	elseif self.__save.attrib.hp > self:GetCurHpMax() then
		self.__save.attrib.hp = self:GetCurHpMax()
	end
	if not flag then
		self:SyncData("hp")
	end
end

function clsChar:GetHp()
	return self.__save.attrib.hp
end

function clsChar:SetPhyscAtk(type, data, flag)
	self.__save.attrib.physcAtk[type] = (self.__save.attrib.physcAtk[type] or 0) + data
	if not flag then
		self:SyncData("physcAtk")
	end
end

function clsChar:GetPhyscAtk(type)
	local info = self.__save.attrib.physcAtk
	if type then
		return info[type]
	end
	return info.min + info.add, info.max + info.add
end

function clsChar:SetPhyscDef(type, data, flag)
	self.__save.attrib.physcDef[type] = (self.__save.attrib.physcDef[type] or 0) + data
	if not flag then
		self:SyncData("physcDef")
	end
end

function clsChar:GetPhyscDef(type)
	local info = self.__save.attrib.physcDef
	if type then
		return info[type]
	else
		return info.base + info.add
	end
end

function clsChar:SetMagicAtk(type, data, flag)
	self.__save.attrib.magicAtk[type] = (self.__save.attrib.magicAtk[type] or 0) + data
	if not flag then
		self:SyncData("magicAtk")
	end
end

function clsChar:GetMagicAtk(type)
	local info = self.__save.attrib.magicAtk
	if type then
		return info[type]
	else
		return info.min + info.add, info.max + info.add
	end
end

function clsChar:SetMagicDef(type, data, flag)
	self.__save.attrib.magicDef[type] = (self.__save.attrib.magicDef[type] or 0) + data
	if not flag then
		self:SyncData("magicDef")
	end
end

function clsChar:GetMagicDef(type)
	local info = self.__save.attrib.magicDef
	if type then
		return info[type]
	else
		return info.base + info.add
	end
end

function clsChar:SetSpeed(data, flag)
	self.__save.attrib.speed = (self.__save.attrib.speed or 0) + data
	if not flag then
		self:SyncData("speed")
	end
end

function clsChar:GetSpeed()
	return self.__save.attrib.speed
end

---------------------------------------------------------------------------
--精英属性
function clsChar:InitSpecialAttrib()
	self.__save.special = {}
	self.__save.special.jink = 0		--闪避
	self.__save.special.hitRate = 0		--命中
	self.__save.special.fatal = 0		--暴击
	self.__save.special.antiFatal = 0	--抗暴
	self.__save.special.crush = 0		--破甲
	self.__save.special.harmDeep = 0	--伤害加深
	self.__save.special.physcResist = 0	--物抗
	self.__save.special.magicResist = 0	--法抗
end

function clsChar:SetJink(data)
	self.__save.special.jink = (self.__save.special.jink or 0) + data
end

function clsChar:GetJink()
	return self.__save.special.jink or 0
end

function clsChar:SetHitRate(data)
	self.__save.special.hitRate = (self.__save.special.hitRate or 0) + data
end

function clsChar:GetHitRate()
	return self.__save.special.hitRate or 0
end

function clsChar:SetFatal(data)
	self.__save.special.fatal = (self.__save.special.fatal or 0) + data
end

function clsChar:GetFatal()
	return self.__save.special.fatal or 0
end

function clsChar:SetAntiFatal(data)
	self.__save.special.antiFatal = (self.__save.special.antiFatal or 0) + data
end

function clsChar:GetAntiFatal()
	return self.__save.special.antiFatal or 0
end

function clsChar:SetCrush(data)
	self.__save.special.crush = (self.__save.special.crush or 0) + data
end

function clsChar:GetCrush()
	return self.__save.special.crush or 0
end

function clsChar:SetHarmDeep(data)
	self.__save.special.harmDeep = (self.__save.special.harmDeep or 0) + data
end

function clsChar:GetHarmDeep()
	return self.__save.special.harmDeep or 0
end

function clsChar:SetPhyscResist(data)
	self.__save.special.physcResist = (self.__save.special.physcResist or 0) + data
end

function clsChar:GetPhyscResist()
	return self.__save.special.physcResist or 0
end

function clsChar:SetMagicResist(data)
	self.__save.special.magicResist = (self.__save.special.magicResist or 0) + data
end

function clsChar:GetMagicResist()
	return self.__save.special.magicResist or 0
end


function clsChar:GetAptLv()
	return 5
end
----------------------------------------------------------------------------------------------------------------
--刷新数据    由子类重定义
function clsChar:SyncData(data, arg)
	
end

---------------------------------------------------------------------------------------------------------------
local EquipPosList = {
	[1] = "hand",		[2] = "upperbody",		[3] = "lowerbody",
	[4] = "foot",		[5] = "neck",			[6] = "finger",
}

function clsChar:GetEquipList()
	return self.__temp.equip
end

function clsChar:GetEquipByPos(pos)
	return self.__temp.equip[pos]
end

--格式化输出装备列表
function clsChar:GetFormatEquipList()
	local sData = {}
	for k, v in pairs(self:GetEquipList()) do
		local buff = v:GetFullAttr()
		buff.pos = k
		if buff.suitId then
		--	buff.suitList = self:GetValidSuitElementList(buff.suitId)
			buff.suitNum = self:GetSuitElementNum(buff.suitId)
		end
		table.insert(sData, buff)
	end
	return sData
end


--是否没有穿戴装备
function clsChar:IsNoEquipWorn()
	for k, v in pairs(self:GetEquipList()) do
		return false
	end
	return true
end

function clsChar:UnfixAllEquip()
	for k, v in pairs(self.__temp.equip) do
		self:UnfixEquip(k, true)
	end
	self.__save.equip = {}
	self.__temp.equip = {}
	self:FreshTotalAttr()
	return true
end

function clsChar:WearEquip(pos, equip)
	local item = self:GetEquipByPos(pos)
	if item then
		self:UnfixEquip(pos, true)
	end
	self.__temp.equip[pos] = equip
	equip:SetInUse(1)			--设置装备使用中
	self:FreshTotalAttr()
end

function clsChar:GetSuitElementNum(suitId)
	local num = 0
	for k, v in pairs(self:GetEquipList()) do
		if v:GetSuitId() == suitId then
			num = num + 1
		end
	end
	return num
end

--[[获取套装已激活的组建列表
function clsChar:GetValidSuitElementList(suitId)
	local basicJob = JOB.GetBasicJob(self:GetJob())
	local elementList = SUIT.GetSuitElementById(id, basicJob)
	local list = {}
	for k, v in pairs(elementList) do
		list[v] = k
	end
	local sData = {}
	for k, v in pairs(self.__save.equip) do
		local id = v:GetId()
		if list[id] then
			table.insert(sData, list[id])
		end
	end
	return sData
end]]

--保存装备数据
function clsChar:SaveEquipData()
	self.__save.equip = {}
	for k, v in pairs(self.__temp.equip) do
		self.__save.equip[k] = v:GetSaveData()
	end
end
--恢复装备数据
function clsChar:RestoreEquipData()
	for k, v in pairs(self.__save.equip or {}) do
		local equipObj = BASEITEM.NewItem(v.id, v.num, {missionId = v.missionId, lock = v.lock, time = v.time, from = v.from})
		if equipObj then
			equipObj:Restore(v)
			self.__temp.equip[k] = equipObj
		end
	end
end
-------------------------------------------------------------------
function clsChar:InitSkillList()
	self.__save.skill = {point = 0, list = {}}
end

function clsChar:GetSkillList()
	return self.__save.skill.list
end

function clsChar:GetMaxSkillLv()
	local lv = 0
	for k, v in pairs(self.__save.skill.list) do
		if v.Lv > lv then
			lv = v.Lv
		end
	end
	return lv
end

--格式化输出技能列表
function clsChar:GetFormatSkillList()
	local sData = {}
	for k, v in pairs(self.__save.skill.list) do
		if tonumber(k) <= 4 then
			local buff = SKILL.GetFormatedSkillInfo(v.id, v.Lv)
			table.insert(sData, buff)
		end
	end
	return sData
end

function clsChar:IsSkillExist(skillId)
	for k, v in pairs(self.__save.skill.list or {}) do
		if v.id == skillId then
			return k
		end
	end
end

function clsChar:GetSkillPoint()
	return self.__save.skill.point or 0
end

function clsChar:SetSkillPoint(data)
	self.__save.skill.point = self.__save.skill.point + data
end

--重置时临时保留的技能列表
function clsChar:SetTempSkillList(data)
	assert(type(data) == "table")
	self.__temp.skill = data
end

function clsChar:GetTempSkillList()
	return self.__temp.skill
end

function clsChar:SetSkill(pos, skillId, skillLv)
	self.__save.skill.list[tostring(pos)] = {id = skillId, Lv = skillLv}
end

function clsChar:GetSkill(pos)
	return self.__save.skill.list[tostring(pos)]
end

function clsChar:ResetSkillPoint()
	local num = 0
	for k, v in pairs(self.__save.skill.list) do
		num = num + v.Lv - (v.bookLv or 0) - 1
		v.Lv = 1
		v.bookLv = 0
	end
	self:SetSkillPoint(num)
	self:FreshTotalAttr()		--刷新全部属性
end

function clsChar:SkillUpgrade(pos, isBook)
	pos = tostring(pos)
	local info = self.__save.skill.list[pos]
	if info then
		info.Lv = info.Lv + 1
		if not isBook then
			self:SetSkillPoint(-1)
		else			--使用技能书升级
			self.__save.skill.list[pos].bookLv = (self.__save.skill.list[pos].bookLv or 0) + 1
		end
		self:FreshTotalAttr()		--刷新全部属性
	end
end

function clsChar:GetBookLv(pos)
	pos = tostring(pos)
	local info = self.__save.skill.list[pos]
	if info then
		return info.bookLv or 0
	end
end

function clsChar:IsAllSkillMaxLv()
	for k, v in pairs(self.__save.skill.list or {}) do
		if v.Lv < MAX_SKILL_LV then
			return false
		end
	end
	return true
end

function clsChar:IsSkillUpgradable(skillId)
	skillId = tostring(skillId)
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	local skillData = self:GetSkill(skillInfo.type)
	if skillData.id ~= skillId then
		return false
	end
	if skillData.Lv >= #skillInfo.list then
		return false
	end
	return true
end

-------------------------------------------------------------------

function clsChar:GetWorkState()
	self.__save.state = self.__save.state or {}
	return self.__save.state.state or "rest"
end

function clsChar:SetWorkInfo(state, place, startTime, endTime)
	self.__save.state = self.__save.state or {}
	self.__save.state.state = state
	self.__save.state.place = place
	self.__save.state.startTime = startTime
	self.__save.state.endTime = endTime
end

function clsChar:GetWorkInfo()
	self.__save.state = self.__save.state or {}
	return self.__save.state
end

--------------------------------------------------------------------
function clsChar:GetAttrTrainInfo()
	local sData = {}
	sData.physcAtkMin = self:GetPhyscAtk("min")
	sData.physcAtkMax = self:GetPhyscAtk("max")
	sData.physcAtkAdd = self:GetPhyscAtk("add")
	sData.physcDefBase = self:GetPhyscDef("base")
	sData.physcDefAdd = self:GetPhyscDef("add")
	sData.magicAtkMin = self:GetMagicAtk("min")
	sData.magicAtkMax = self:GetMagicAtk("max")
	sData.magicAtkAdd = self:GetMagicAtk("add")
	sData.magicDefBase = self:GetMagicDef("base")
	sData.magicDefAdd = self:GetMagicDef("add")
	return sData
end

function clsChar:GenAttrTrain()
end

function clsChar:GetAttrTrain()
	return self.__temp.attrTrain
end

function clsChar:SaveAttrTrain()
	local info = self.__temp.attrTrain
	self:SetPhyscAtk("add", info.physcAtkAdd - self:GetPhyscAtk("add"))
	self:SetPhyscDef("add", info.physcDefAdd - self:GetPhyscDef("add"))
	self:SetMagicAtk("add", info.magicAtkAdd - self:GetMagicAtk("add"))
	self:SetMagicDef("add", info.magicDefAdd - self:GetMagicDef("add"))
	self:FreshTotalAttr()
end
---------------------------------------------------------------------
--属性统计
local charAttrList = {
	["strength"] = {reset = "SetStrength", set = "ModStrength", get = "GetStrength", special = 1, grade = 1},
	["intellect"] = {reset = "SetIntellect", set = "ModIntellect", get = "GetIntellect", special = 1, grade = 1},
	["agility"] = {reset = "SetAgility", set = "ModAgility", get = "GetAgility", special = 1, grade = 1},
	["captain"] = {reset = "SetCaptain", set = "ModCaptain", get = "GetCaptain", special = 1, grade = 1},
	["speed"] = {set = "SetSpeed", get = "GetSpeed", grade = 2},
	["physcAtk"] = {set = "SetPhyscAtk", get = "GetPhyscAtk", special = 1, absent = 1},
	["physcAtkMin"] = {grade = 2},
	["physcAtkMax"] = {grade = 2},
	["physcDef"] = {set = "SetPhyscDef", get = "GetPhyscDef", grade = 2},
	["magicAtk"] = {set = "SetMagicAtk", get = "GetMagicAtk", special = 1, absent = 1},
	["magicAtkMin"] = {grade = 2},
	["magicAtkMax"] = {grade = 2}, 
	["magicDef"] = {set = "SetMagicDef", get = "GetMagicDef", grade = 2},
	["hpMax"] = {set = "SetHpMax", get = "GetHpMax", grade = 2},
	["jink"] = {set = "SetJink", get = "GetJink", grade = 3},
	["fatal"] = {set = "SetFatal", get = "GetFatal", grade = 3},
	["crush"] = {set = "SetCrush", get = "GetCrush", grade = 3},
	["harmDeep"] = {set = "SetHarmDeep", get = "GetHarmDeep", grade = 3},
	["physcResist"] = {set = "SetPhyscResist", get = "GetPhyscResist", grade = 3},
	["magicResist"] = {set = "SetMagicResist", get = "GetMagicResist", grade = 3},
	["counter"] = {}, --set = "SetCounterRate", get = "GetCounterRate"},
	["block"] = {}, --set = "SetBlockRate", get = "GetBlockRate"},
	["batter"] = {}, --set = "SetBatterRate", get = "GetBatterRate"},
	["ignoreMgcDef"] = {}, --set = "SetIgnoreMgcDef", get = "GetIgnoreMgcDef"},
	["hurtDown"] = {},--set = "SetHurtDown", get = "GetHurtDown"},
	["antiFatal"] = {set = "SetAntiFatal", get = "GetAntiFatal", grade = 3},
	["hitRate"] = {set = "SetHitRate", get = "GetHitRate", grade = 3},
}

--初始化属性统计
function clsChar:InitTotalAttrList()
	self.__save.total = {}
	for k, v in pairs(charAttrList) do
		if v.get then
			if k == "physcAtk" or k == "magicAtk" then
				local atkMin, atkMax = clsChar[v.get](self)
				self.__save.total[k .. "Min"] = atkMin
				self.__save.total[k .. "Max"] = atkMax
			else
				self.__save.total[k] = clsChar[v.get](self)
			end
		end
	end
end
--修改属性值
function clsChar:SetTotalAttr(type, num, initFlag)
	self.__save.total = self.__save.total or {}
	if not charAttrList[type] then
		return
	end
	if type == "physcAtk" or type == "magicAtk" then
		self.__save.total[type .. "Min"] = (self.__save.total[type .. "Min"] or 0) + num
		self.__save.total[type .. "Max"] = (self.__save.total[type .. "Max"] or 0) + num
	else
		self.__save.total[type] = (self.__save.total[type] or 0) + num
		if type == "strength" then
			self.__save.total["physcAtkMin"] = (self.__save.total["physcAtkMin"] or 0) + num * STR_TO_ATK
			self.__save.total["physcAtkMax"] = (self.__save.total["physcAtkMax"] or 0) + num * STR_TO_ATK
			self.__save.total["physcDef"] = (self.__save.total["physcDef"] or 0) + num * STR_TO_DEF
		elseif type == "intellect" then
			self.__save.total["magicAtkMin"] = (self.__save.total["magicAtkMin"] or 0) + num * INT_TO_ATK
			self.__save.total["magicAtkMax"] = (self.__save.total["magicAtkMax"] or 0) + num * INT_TO_ATK
			self.__save.total["magicDef"] = (self.__save.total["magicDef"] or 0) + num * INT_TO_DEF
		elseif type == "agility" then
			self.__save.total["physcDef"] = (self.__save.total["physcDef"] or 0) + num * AGI_TO_DEF
			self.__save.total["magicDef"] = (self.__save.total["magicDef"] or 0) + num * AGI_TO_DEF
			self.__save.total["speed"] = (self.__save.total["speed"] or 0) + num * AGI_TO_SPD
		elseif type == "captain" then
			self.__save.total["hpMax"] = (self.__save.total["hpMax"] or 0) + num * CAP_TO_HPM
			self.__save.total["physcDef"] = (self.__save.total["physcDef"] or 0) + num * CAP_TO_DEF
			self.__save.total["magicDef"] = (self.__save.total["magicDef"] or 0) + num * CAP_TO_DEF
		end
	end
end

function clsChar:GetTotalAttr(type)
	self.__save.total = self.__save.total or {}
--	return self.__save.total[type] or 0
	local roleId = self:GetRoleId() or self:GetOwner()
	local sum = self.__save.total[type] or 0
	local academy
	if type == "physcAtkMin" or type == "physcAtkMax" then
		academy = ACADEMY.GetTechEffectByType(roleId, "physcAtk")
		sum = sum + academy 
	elseif type == "magicAtkMin" or type == "magicAtkMax" then
		academy = ACADEMY.GetTechEffectByType(roleId, "magicAtk")
		sum = sum + academy
	else
		academy = ACADEMY.GetTechEffectByType(roleId, type)
		sum = sum + academy
	end
	return sum
end
--百分比加成属性
function clsChar:SetTotalAttrAddRate(grade, rate)
	local list = self.__save.total
	for k, v in pairs(charAttrList) do
		if v.grade == grade then
			list[k] = list[k] * (1 + rate)
		end
	end
end

--修改某一项的值
function clsChar:ModAttrByRate(type, rate)
	local list = self.__save.total
	if type == "physcAtk" then
		list["physcAtkMin"] = list["physcAtkMin"] * (1 + rate)
		list["physcAtkMax"] = list["physcAtkMax"] * (1 + rate)
	elseif type == "magicAtk" then
		list["magicAtkMin"] = list["magicAtkMin"] * (1 + rate)
		list["magicAtkMax"] = list["magicAtkMax"] * (1 + rate)
	else
		list[type] = list[type] * (1 + rate)
	end
end

--重设某项属性
function clsChar:ResetAttr(type, data, flag)
	if not charAttrList[type] or not charAttrList[type].reset then
		print("error!", charAttrList[type].reset)
	end
	clsChar[charAttrList[type].reset](self, data, flag)
end
--修改某项属性
function clsChar:SetAttr(type, data, flag)
	if not charAttrList[type] or not charAttrList[type].reset then
		print("error!", charAttrList[type].reset)
	end
	if charAttrList[type].multiple then
		if type == "physcAtk" or type == "magicAtk" then
			clsChar[charAttrList[type].set](self, "min", data, flag)
			clsChar[charAttrList[type].set](self, "max", data, flag)
		elseif type == "physcDef" or type == "magicDef" then
			clsChar[charAttrList[type].set](self, "base", data, flag)
		end
	else
		clsChar[charAttrList[type].set](self, data, flag)
	end
end

function clsChar:GetAttr(type)
	if not charAttrList[type] or not charAttrList[type].reset then
		print("error!", charAttrList[type])
	end
	return clsChar[charAttrList[type].get](self)
end

--回复血量到满血
function clsChar:RestoreAllHp(data)
	if not data then
		data = self:GetCurHpMax()
	end
--	local hpMax = self:GetCurHpMax()
	self.__save.attrib.hp = data
	self:SyncData("hp")
end
---------------------------------------------------------------------
local forceRateList = {			--属性对战斗力的加成影响
	["strength"] = 10, ["intellect"] = 10, ["agility"] = 15, ["captain"] = 15, ["hpMax"] = 1, 
	["physcAtkMax"] = 2, ["physcDef"] = 3, ["magicAtkMax"] = 2,	["magicDef"] = 3, ["speed"] = 4, 
	["jink"] = 5, ["fatal"] = 2, ["crush"] = 2, ["harmDeep"] = 2, 
	["physcResist"] = 3, ["magicResist"] = 3
}


function CalFightForce(buff)
	local force = 0
	for k, v in pairs(forceRateList) do
		force = force + (buff[k] or 0) * v
	end
	force = force + (buff.Lv or 1) * (buff.aptLv or 1) * 10
	local skillApt, skillLv = 0, 1
	for k, v in pairs(buff.skillList or {}) do
		local skillInfo = BASESKILL.GetSkillInfo(v.skillId)
		if skillInfo and skillInfo.quality > skillApt then
			skillLv = v.Lv
			skillApt = skillInfo.quality
		end
	end
	force = force +  skillApt * (skillLv or 1) * 10
	return force
end

function clsChar:CalFightForce()
	local force = 0
	for k, v in pairs(forceRateList) do
		force = force + self:GetTotalAttr(k) * v
	end
	local skillApt, skillLv = 0, 1
	for k, v in pairs(self:GetSkillList()) do
		local skillInfo = BASESKILL.GetSkillInfo(v.id)
		if skillInfo and skillInfo.quality > skillApt then
			skillLv = v.Lv
			skillApt = skillInfo.quality
		end
	end
	force = force +  skillApt * (skillLv or 1) * 10
	force = force + self:GetLv() * 10 * self:GetAptLv()
	self.__save.force = math.ceil(force)
	if self:GetRoleType() == "user" and force >= 100000 then
		TITLE.CheckHonorTitleRecord(self, "391107")
	end
end

function clsChar:GetFightForce()
	if not self.__save.force then
		self:CalFightForce()
	end
	return self.__save.force
end

function clsChar:ResetFightForce()
	self:CalFightForce()
end

function clsChar:GetFightAttrList()
	local sData = {}
	for k, v in pairs(charAttrList) do
		if not v.absent then
			sData[k] = self:GetTotalAttr(k)
		end
	end
	sData.hp = self:GetHp()
	sData.skillList = self:GetSkillList()
	return sData
end

function clsChar:GetBaseFightForce()
	local force = 0
	force = force +	self:GetStrength() * 10 + self:GetIntellect() *10 + self:GetAgility() * 15 + self:GetCaptain() * 15
	force = force + self:GetHpMax() * 1 + (self:GetPhyscAtk("max") + self:GetPhyscAtk("add")) * 2 + self:GetPhyscDef() * 3 + (self:GetMagicAtk("max") + self:GetMagicAtk("add")) * 2 + self:GetMagicDef() * 3 + self:GetSpeed() * 4
	force = force + self:GetJink() * 5 + self:GetFatal() * 2 + self:GetCrush() * 2 + self:GetHarmDeep() * 2 + self:GetPhyscResist() * 3 + self:GetMagicResist() * 3
	force = force + self:GetLv() * self:GetAptLv() * 10
	return force
end
---------------------------------------------------------------------

function __init__()

end
