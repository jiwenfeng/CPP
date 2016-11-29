--武将类
local HeroAptInfoList = {}			--资质对应属性表
local INIT_HERO_POINT = 5
local NORMAL_HERO_MAX_APTLV = 5

function GetHeroExpList()
	local buff = {}
	for k, v in ipairs(USERBASE.GetUserExpList()) do
		buff[k] = v.uplimit
	end
	return buff
end

function GetUpgradeExp(Lv)   --获得升级经验
	return USERBASE.GetUpgradeExp(Lv)
end

-----------------------------------
function GetHeroAptInfoByLv(aptLv)
	return HeroAptInfoList[aptLv]
end
--通过资质查询资质等级
function GetAptLvByAptitude(aptitude)
	for k, v in ipairs(HeroAptInfoList) do
		if aptitude >= v.apt.min and aptitude <= v.apt.max then
			return k
		end
	end
end


local HeroAttrList = {
	strength = 0, intellect = 0, agility = 0, captain = 0,
	hpMax = 150, physcAtk = 50, magicAtk = 50, physcDef = 0, magicDef = 0, speed = 0,
}

function GetInitHeroAttr(aptLv)
	local initInfo = GetHeroAptInfoByLv(aptLv)
	local buff = {}
	for k in pairs(HeroAttrList) do
		if type(initInfo[k]) == "table" then
			buff[k] = math.random(initInfo[k].min, initInfo[k].max)
		else
			buff[k] = initInfo[k] or 0
		end
	end
	return buff
end

local growingRate = {
--	["physical"] = {strength = {min = 1.5, max = 2.5}, intellect = {min = 0, max = 0.5}, agility = {min = 0.5, max = 1.0}, captain = {min = 0.5, max = 1.0}},
--	["magic"] = {strength = {min = 0, max = 0.5}, intellect = {min = 1.5, max = 2.5}, agility = {min = 0.5, max = 1.0}, captain = {min = 0.5, max = 1.0}},
	["warrior"] = {strength = {min = 60, max = 100}, intellect = {min = 10, max = 20}, agility = {min = 20, max = 40}, captain = {min = 40, max = 60}},
	["archer"] = {strength = {min = 60, max = 100}, intellect = {min = 10, max = 20}, agility = {min = 40, max = 60}, captain = {min = 20, max = 40}},
	["taoist"] = {strength = {min = 10, max = 20}, intellect = {min = 80, max = 100}, agility = {min = 20, max = 40}, captain = {min = 20, max = 40}},
}

local grownList = {"strength", "intellect", "agility", "captain"}

--根据初始属性计算成长率
function GenGrowingRate(list, sum)		
	sum = sum or 0
	if sum <= 0 then
		for k, v in pairs(list) do
			sum = sum + v
		end
	end
	assert(sum > 0)
	local ret = {}
	local left = 1
	for i = 1, #grownList do
		local type = grownList[i]
		if i >= #grownList then
			table.insert(ret, {type = type, rate = left})
		else
			local rate = (list[type] or 0) / sum
			rate = rate - rate % 0.01
			left = left - rate
			table.insert(ret, {type = type, rate = rate})
		end
	end
	return ret
end

--生成成长值
function GenHeroGrowingRate(job)
	local basic = JOB.GetBasicJob(job)
	local info = growingRate[basic]
	local sum = 0
	local buff = {}	
	for i = 1, #grownList do
		local type = grownList[i]
	--	buff[type] = math.random(info[type].min * 10, info[type].max * 10) / 10
		buff[type] = math.random(info[type].min, info[type].max)
		sum = sum + buff[type]
	end
	return GenGrowingRate(buff, sum)
end

function CalPointAssignList(growing, point)
	local list = {}
	local function assign_point()
		local ran = math.random()
		local rate = 0
		for k, v in pairs(growing) do
			rate = rate + v.rate
			if rate >= ran then
				list[v.type] = (list[v.type] or 0) + 1
				break
			end
		end
	end
	point = point or INIT_HERO_POINT
	for i = 1, point do
		assign_point()
	end
	return list
end

function GenHeroAttrList(growing, aptLv)
	local initInfo = GetHeroAptInfoByLv(aptLv)
	local attrList = GetInitHeroAttr(aptLv)
	local list = CalPointAssignList(growing, initInfo.point)
	for k, v in pairs(list) do
		attrList[k] = attrList[k] + v
		if k == "strength" then
			attrList.physcAtk = attrList.physcAtk + v * 5
			attrList.physcDef = attrList.physcDef + v * 1
		elseif k == "intellect" then
			attrList.magicAtk = attrList.magicAtk + v * 5
			attrList.magicDef = attrList.magicDef + v * 1
		elseif k == "agility" then
			attrList.physcDef = attrList.physcDef + v * 1
			attrList.magicDef = attrList.magicDef + v * 1
			attrList.speed = attrList.speed + v * 0.5
		elseif k == "captain" then
			attrList.hpMax = attrList.hpMax + v * 10
			attrList.physcDef = attrList.physcDef + v * 1
			attrList.magicDef = attrList.magicDef + v * 1
		end
	end
	return attrList
end
------------------------------------------------
clsHero = CHAR.clsChar:Inherit()

function clsHero:__init__()
	Super(clsHero).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsHero:initsavedata()
	self.__save.Lv = 1              --等级
	self.__save.exp = 0
	self.__save.info = {}
	
	self.__save.point = {}			--升级属性点数
	self.__save.attrib = {}
	self.__save.special = {}
	
	self.__save.skill = {}
	self.__save.equip = {}
	self.__save.state = {}
end

function clsHero:inittempdata()
	self.__temp.equip = {}
end

function clsHero:GetRoleType()
	return "hero"
end

--成长值
function clsHero:SetGrowingRate(data)
	assert(type(data) == "table")
	self.__save.growing = data
end

function clsHero:GetGrowingRate()
	return self.__save.growing
end

function clsHero:SetOwner(data)
	self.__save.owner = data
end

function clsHero:GetOwner()
	return self.__save.owner
end

function clsHero:GetAptitude()
	return self.__save.aptitude
end

function clsHero:SetAptitude(data)
	data = assert(tonumber(data))
	self.__save.aptitude = data
end

function clsHero:ModAptitude(data)
	data = assert(tonumber(data))
	local aptInfo = GetHeroAptInfoByLv(self:GetAptLv())
	local maxInfo = GetHeroAptInfoByLv(self:GetMaxAptLv())
	self.__save.aptitude = self.__save.aptitude + data
	if self.__save.aptitude < aptInfo.apt.min then
		self.__save.aptitude = aptInfo.apt.min
	elseif self.__save.aptitude > maxInfo.apt.min then
		self.__save.aptitude = maxInfo.apt.min
	end
end

--最大品质等级
function clsHero:GetMaxAptLv()
	return self.__save.maxAptLv or NORMAL_HERO_MAX_APTLV
end

function clsHero:SetMaxAptLv(data)
	data = assert(tonumber(data))
	self.__save.maxAptLv = data
end

function clsHero:SetTemplate(data)
	self.__save.template = data
end

function clsHero:GetTemplate()
	return self.__save.template
end

function clsHero:SetType(type)
	self.__save.type = type
end

function clsHero:GetType()
	return self.__save.type
end

--调整点数分配 (用于资质提升后)
function clsHero:AdjustPoint()
	local aptLv = GetAptLvByAptitude(self:GetAptitude())
	local info = GetHeroAptInfoByLv(aptLv)
	local sum = 0
	for _, v in pairs(grownList) do
		sum = sum + self:GetAttr(v)
	end
	local left = self:GetLv() * info.point - sum
	if left > 0 then
		self:ModLeftPoint(left)
		self:AssignPoint()
	end
end

--初始化武将
function clsHero:InitHero(type, heroId, owner, heroData)
	self:SetId(heroId)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetName(heroData.name)
	self:SetJob(heroData.job)
	
	self:SetModelId(heroData.modelId)
	if heroData.headId then
		self:SetHeadId(heroData.headId)
		self:SetSex(heroData.headId % 2)
	else
		local headId = JOB.GenHeroHeadIdByModel(heroData.modelId)
		self:SetHeadId(headId)
		self:SetSex(headId % 2)
	end
	
	self:SetMaxAptLv(heroData.maxAptLv or NORMAL_HERO_MAX_APTLV)
	
	local aptitude = heroData.aptitude
	if heroData.aptLv then
		if heroData.aptLv >= self:GetMaxAptLv() then
			local info = GetHeroAptInfoByLv(self:GetMaxAptLv())
			aptitude = info.apt.min
		else
			local info = GetHeroAptInfoByLv(heroData.aptLv)
			aptitude = math.random(info.apt.min, info.apt.max)
		end
	end
	self:SetAptitude(aptitude)
	if heroData.growing then
		self:SetGrowingRate(heroData.growing)
	else
	--	local atkType = JOB.GetJobAttackType(heroData.job)
		self:SetGrowingRate(GenHeroGrowingRate(heroData.job))
	end
	
	self:InitPoint()
	self:InitAttrib(GetAptLvByAptitude(aptitude))
	self:PresetAttr(heroData)
	self:InitSpecialAttrib()
	self:InitSkillList()
	
	if heroData.skillId then
		self:SetSkill("1", heroData.skillId, 1)
	end
	if heroData.featureSkill then	--天赋技能
		self:SetSkill("4", heroData.featureSkill, 1)
	end
	self:AssignPoint()
end

function clsHero:InitSkillList()
	self.__save.skill = {point = 0, list = {}}
	local list = SKILL.GetResetableSkillListByjob(self:GetJob())
	for i = 1, 3 do
		local id = list[i][math.random(1, #list[i])]
		self:SetSkill(tostring(i), id, 1)
	end
end

function clsHero:ReInitSkillList()
	local info = self.__save.skill
	info.point = 0
	for k, v in pairs(info.list) do
		v.Lv = v.bookLv or 1
	end
end

function clsHero:PresetAttr(list)
	for k in pairs(HeroAttrList) do
		if self.__save.point[k] then
			self.__save.point[k] = list[k] or 0
		elseif self.__save.attrib[k] then
			if k == "physcAtk" or k == "magicAtk" then
				self.__save.attrib[k].min = list[k] or 0
				self.__save.attrib[k].max = list[k] or 0
			elseif k == "physcDef" or k == "magicDef" then
				self.__save.attrib[k].base = list[k] or 0
			elseif k == "hpMax" then
				self.__save.attrib.hp = list[k] or 0
				self.__save.attrib.hpMax = list[k] or 0
			else
				self.__save.attrib[k] = list[k] or 0
			end
		end
	end
	self.__save.point.left = 0
end

--重置武将
function clsHero:ReInitHero()
	local attrib = self.__save.attrib
	self.__save.Lv = 1
	self.__save.exp = 0
	self:InitPoint()
	self:InitAttrib(self:GetAptLv())
	self:InitSpecialAttrib()
	self:ReInitSkillList()
	
	self.__save.attrib.physcAtk.add = attrib.physcAtk.add
	self.__save.attrib.physcDef.add = attrib.physcDef.add
	self.__save.attrib.magicAtk.add = attrib.magicAtk.add
	self.__save.attrib.magicDef.add = attrib.magicDef.add
	self:AssignPoint()
end

function clsHero:InitAttrib(aptLv)
	Super(clsHero).InitAttrib(self)
	
	local job = JOB.GetBasicJob(self:GetJob())
	local info = GetHeroAptInfoByLv(aptLv)
	self:SetHpMax(math.random(info.hpMax.min, info.hpMax.max))
	self:SetHp(self:GetHpMax())
	if job ~= "taoist" then
		local physcAtk = math.random(info.physcAtk.min, info.physcAtk.max)
		self:SetPhyscAtk("min", physcAtk)
		self:SetPhyscAtk("max", physcAtk)
		self:SetMagicAtk("min", 0)
		self:SetMagicAtk("max", 0)
	else
		self:SetPhyscAtk("min", 0)
		self:SetPhyscAtk("max", 0)
		local magicAtk = math.random(info.magicAtk.min, info.magicAtk.max)
		self:SetMagicAtk("min", magicAtk)
		self:SetMagicAtk("max", magicAtk)
	end
	self:SetPhyscDef("base", info.physcDef)	
	self:SetMagicDef("base", info.magicDef)
	self:SetSpeed(info.speed)
end
--生成属性培养数值
function clsHero:GenAttrTrain(type)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local vipLv = user:GetVipLv()
	local uplimit = USER.GetAttrTrainUpLimit(user:GetStatusLv(), vipLv + 1)
	local cur = {}
	cur.physcAtkAdd = self:GetPhyscAtk("add")
	cur.physcDefAdd = self:GetPhyscDef("add")
	cur.magicAtkAdd = self:GetMagicAtk("add")
	cur.magicDefAdd = self:GetMagicDef("add")
	local flag = true
	for k, v in pairs(cur) do
		if v < uplimit then
			flag = false
			break
		end
	end
	if flag then
		return
	end
	local function gen_num(cur)
		local num = 0
		if type == "nimbus" then
		local ran = math.random()
			if user:GetStatusLv() <= 0 or ran <= 0.6 then
				num = math.random(1, 5)
			else
				num = -math.random(1, 3)
			end
		elseif type == "ingot" then
			if math.random() <= 0.8 then
				num = math.random(4, 7)
			else
				num = math.random(1, 3)
			end
		end
		return math.min(math.max(0, cur + num), uplimit)
	end
	local info = {}
	for k, v in pairs(cur) do
		info[k] = gen_num(v)
	end
	self.__temp.attrTrain = info
	return self.__temp.attrTrain
end

--自动分配点数
function clsHero:AssignPoint(flag)
	local list = CalPointAssignList(self:GetGrowingRate(), self:GetLeftPoint())
	for k, v in pairs(list) do
		self:SetAttr(k, v)
	end
	self:SetLeftPoint(0)
	if not flag then
		self:FreshTotalAttr()
	end
end

function clsHero:GetMaxLv()
	return math.min(#USERBASE.GetUserExpList(), SERVER_ROLE_MAX_LV)
end

--设置等级
function clsHero:SetLv(data)
	if tonumber(data) == 0 then
		return
	end
	self.__save.Lv = self.__save.Lv + tonumber(data)
	if self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	elseif self.__save.Lv < 1 then
		self.__save.Lv = 1
	end
end

--设置经验
function clsHero:SetExp(data, type)	
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local pet = user:GetPet()
	local rate = pet:GetTalentAdd(311018)
	self.__save.exp = self.__save.exp + data * (1 + rate)
	local expMax = tonumber(self:GetUpgradeExp())
	if self.__save.exp >= expMax then		
		local lv = self:GetLv()
		if lv >= user:GetLv() or lv >= self:GetMaxLv() then
			self.__save.exp = expMax
		else
			self.__save.exp = self.__save.exp - expMax
			self:Upgrade()
		end
	end
	DATA.log(string.format("%s获得经验%s,for %s", self:GetName(), data, type or ""), "hero.log")
end

function clsHero:GetSumExp()
	local sum = self:GetExp()
	local curLv = self:GetLv()
	for i = 1, curLv - 1 do
		sum = sum + GetUpgradeExp(i)
	end
	return sum
end

function clsHero:GetUpgradeExp()
	return GetUpgradeExp(self:GetLv())
end


function clsHero:Upgrade()
	local aptLv = GetAptLvByAptitude(self:GetAptitude())
	local info = GetHeroAptInfoByLv(aptLv)
	self:SetLv(1)
	self:ModLeftPoint(info.point)
	if self:GetLv() % 2 == 0 then
		self:SetSkillPoint(1)
	end
	self:SetHpMax(10)	
	
	local expMax = self:GetUpgradeExp()
	if self.__save.exp >= expMax then
		if self:GetLv() < self:GetMaxLv() then
			self.__save.exp = self.__save.exp - expMax
			return self:Upgrade()
		else
			self.__save.exp = expMax
		end
	end
	self:AssignPoint()
end

function clsHero:GetPreviewLv(exp)
	local Lv = self:GetLv()
	local maxLv = self:GetMaxLv()
	local curExp = self:GetExp()
	for i = Lv, maxLv do
		local needExp = GetUpgradeExp(i)
		if curExp + exp < needExp then
			return i
		end
		curExp = 0
		exp = exp + curExp - needExp
	end
	return maxLv
end

function clsHero:GetAptLv()
	return GetAptLvByAptitude(self:GetAptitude())
end


function clsHero:FreshTotalAttr(flag)
	local Lv = self:GetLv()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:InitTotalAttrList()						--人物自身属性
	local suitList = {}
	for k, v in pairs(self:GetEquipList()) do		--装备附加属性
		if Lv >= v:GetNeedLv() then
			for k, v in pairs(v:GetEquipAttrAdd()) do
				self:SetTotalAttr(k, v)
			end
			local suitId = v:GetSuitId()
			if suitId then
				suitList[suitId] = (suitList[suitId] or 0) + 1
			end
		end
	end
	for k, v in pairs(suitList) do					--套装效果
		local list = SUIT.GetSuitAttrById(k, v)
		for _, table in ipairs(list or {}) do
			for name, num in pairs(table) do 
				self:SetTotalAttr(name, num)
			end
		end
	end
	for k, v in pairs(self:GetSkillList()) do			--技能附加效果
		for key, val in pairs(SKILL.GetPassiveSkillEffect(v.id, v.Lv) or {}) do
			self:SetTotalAttr(key, val)
		end
	end
--[[	for k, v in pairs() do		--科技系统
		
	end]]


--[[	local tribe = user:GetTribe()
	if tribe then
		local academy = tribe:GetStruct("academy")
		if academy then
			for k, v in pairs(academy:GetTechList()) do
				local info = ACADEMY.GetTechById(k)
				if info and info.type == 2 then
					for attrType, num in pairs(info.list[v.Lv].attr) do
						self:SetTotalAttr(attrType, num)
					end
				end
			end
		end
	end]]
	local status = user:GetStatus()		--仙印增加属性
	if status then
		for k, v in pairs(status:GetAttrAdd()) do
			self:SetTotalAttr(k, v)
		end
	end
	local pet = user:GetPet()
	local rate = pet:GetTalentAdd(311016)
	self:ModAttrByRate("physcAtk", rate)
	self:ModAttrByRate("magicAtk", rate)
	local rate = pet:GetTalentAdd(311017)
	self:ModAttrByRate("physcDef", rate)
	self:ModAttrByRate("magicDef", rate)
	if self:GetWorkState() == "standby" then
		local bufferList = user:GetBufferEffectList()
		for k, v in pairs(bufferList) do					--buffer附加属性
			local buffInfo = BUFFER.GetBufferInfoById(k)
			for key, val in pairs(buffInfo.attr or {}) do
				self:SetTotalAttr(key, val)
			end
		end
		local form = user:GetCurForm()		--阵法属性加成
		if form then
			for k, v in pairs(form:GetAttr()) do
				self:SetTotalAttr(k, v)
			end
		end
		if user:IsFuncOpen(SETTING.FUNC_FAIRYDEV) then				--仙器系统加成
			local list = user:GetFairyDevAttrAdd()
			for k, v in pairs(list.range) do
				self:SetTotalAttr(k, v)
			end
		end
	end
	local rate = 0
	local soldierInfo = BARRACK.GetUserSoldierInfo(user, self:GetId())
	rate = rate + (soldierInfo and soldierInfo.add or 0)
	self:SetTotalAttrAddRate(2, rate)
	self:CalFightForce()
	if flag then
		self:RestoreAllHp()
	else
		user:RestoreHp(self)
	end
end

--获取武将的战斗属性
function clsHero:GetFightAttr()
	local buff = table.copy(self:GetFightAttrList())
	buff.name = self:GetName()
	buff.heroId = self:GetId()
	buff.identity = "hero"
	buff.headId = self:GetHeadId()
	buff.modelId = self:GetModelId()
	buff.Lv = self:GetLv()
	buff.job = self:GetJob()
	buff.aptLv = self:GetAptLv()
	if self:GetTemplate() then
		buff.skillRound = 1
	end
	return buff
end

function clsHero:GetHeroDetail()
	local sData = {}
	sData.heroId = self:GetId()
	sData.Lv = self:GetLv()
	sData.job = self:GetJob()
	sData.force = self:GetFightForce()
	sData.state = self:GetWorkState()
	sData.exp = self:GetExp()
	sData.expMax = self:GetUpgradeExp()
	sData.modelId = self:GetModelId()
	sData.template = self:GetTemplate()
	
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(self:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(self:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(self:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(self:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(self:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(self:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(self:GetTotalAttr("speed"))
	local user = USER.GetOnlineUser(self:GetOwner())
	local soldierInfo
	if user then
		soldierInfo = BARRACK.GetUserSoldierInfo(user, self:GetId())
	else
		soldierInfo = BARRACK.GetHeroSoldierInfo(self:GetOwner(), self:GetJob())
	end
	sData.soldierType = soldierInfo.name
	return sData
end

function clsHero:GetDetailAttr()
	local sData = {}
	sData.heroId = self:GetId()
	sData.strength = math.ceil(self:GetTotalAttr("strength"))
	sData.intellect = math.ceil(self:GetTotalAttr("intellect"))
	sData.agility = math.ceil(self:GetTotalAttr("agility"))
	sData.captain = math.ceil(self:GetTotalAttr("captain"))
	sData.aptLv = self:GetAptLv()
	sData.jink = self:GetTotalAttr("jink")
	sData.crush = self:GetTotalAttr("crush")
	sData.fatal = self:GetTotalAttr("fatal")
	sData.harmDeep = self:GetTotalAttr("harmDeep")
	sData.physcResist = self:GetTotalAttr("physcResist")
	sData.magicResist = self:GetTotalAttr("magicResist")
	return sData
end

function clsHero:GetSimpleInfo(skillType, needApt)
	local sData = {}
	sData.heroId = self:GetId()
	sData.name = self:GetName()
	sData.headId = self:GetHeadId()
	sData.modelId = self:GetModelId()
	sData.Lv = self:GetLv()
	local aptLv = GetAptLvByAptitude(self:GetAptitude())
	sData.aptLv = aptLv
	sData.job = self:GetJob()
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(self:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(self:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(self:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(self:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(self:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(self:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(self:GetTotalAttr("speed"))
	sData.force = self:GetFightForce()
	sData.type = self:GetType()
	if needApt then
		sData.aptitude = self:GetAptitude()
		local aptInfo = GetHeroAptInfoByLv(aptLv)
		if aptLv >= self:GetMaxAptLv() then
			sData.aptMax = aptInfo.apt.min
		else
			sData.aptMax = aptInfo.apt.max
		end
	end
	if skillType then
		sData.skillList = {}
		for k, v in pairs(self:GetSkillList()) do
			if skillType == 0 or skillType == tonumber(k) then
				if v.id then
					table.insert(sData.skillList, {type = tonumber(k), skillId = v.id, Lv = v.Lv})
				end
			end
		end
	end
	return sData
end


function clsHero:GetBidAttrList()
	local sData = {}
	sData.force = self:GetBaseFightForce()
	sData.owner = USER.GetUserNickById(self:GetOwner())
	return sData
end

--[[
function clsHero:GetWorkState()
	local info = self:GetWorkInfo()
	return info.state or "rest"
end]]

--武将离职
function clsHero:Dimission()
	local owner = assert(USER.GetOnlineUser(self:GetOwner()))
	owner:DelHeroFromAllLineup(self:GetId())
	local info = self:GetWorkInfo()
	
	self:UnfixAllEquip()
end

function clsHero:UnfixEquip(pos, flag)
	local equip = self:GetEquipByPos(pos)
	if not equip then
		return
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	equip:SetInUse()
	self.__temp.equip[pos] = nil
	user:AddItem(USERBASE.PACKAGE, equip, true)
	if not flag then
		self:FreshTotalAttr()
	end
end


function clsHero:GetSaveData()
	self:SaveEquipData()
	return table.deepcopy(self.__save)
end

function clsHero:Restore(data)
	self.__save = data
	self:RestoreEquipData()
end

function clsHero:GetOnlineTimestamp()
	return self.__temp.onlinetime
end

function clsHero:SetOnlineTimestamp(data)
	self.__temp.onlinetime = data
end


function __init__()

	HEROAPT = Import("autocode/heroApt.lua")
	HeroAptInfoList = HEROAPT.GetTable()
	
	
end

