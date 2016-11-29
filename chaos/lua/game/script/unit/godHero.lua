--神将系统

local REBIRTH_PILL_ID = 102020
local REBIRTH_PILL_NEED_INGOT = 100		--护元丹所需元宝
local REBIRTH_ATTR_KEPT = 0.2			--转世后属性保留
local MAX_REBIRTH_TIMES = 5		--最大转世次数
local INHERIT_NEED_INGOT = 10	--传承所需元宝
local INHERIT_EXP_RATE = 0.2	--传承经验继承比率

local HERO_FRESH_COST = 250000
local NPC_CITY_FRESH_RATE = 0.3
local NORMAL_CITY_FRESH_RATE = 0.05
local MAX_FRESH_HERO_NUM = 3

local HeroMaxLvList = {[1] = 60, [2] = 70, [3] = 80, [4] = 90, [5] = 100, [6] = 110}	--神将最大等级
local ExpStoneList = {[1] = 102019, [2] = 102022}		--三生石
local RebirthNeedPill = {[1] = 1, [2] = 5, [3] = 10, [4] = 20, [5] = 30, [6] = 0}

--------------------------------------------------------------------
clsGodHero = CLSHERO.clsHero:Inherit()

function clsGodHero:__init__()
	Super(clsGodHero).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsGodHero:InitHero(type, heroId, owner, heroData)
	self:SetType(type)
	self:SetId(heroId)
	self:SetOwner(owner)
	
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
	if heroData.maxAptLv then
		self:SetMaxAptLv(heroData.maxAptLv)
	end

	local aptitude = heroData.aptitude
	if heroData.aptLv then
		if heroData.aptLv >= self:GetMaxAptLv() then
			local info = CLSHERO.GetHeroAptInfoByLv(self:GetMaxAptLv())
			aptitude = info.apt.min
		else
			local info = CLSHERO.GetHeroAptInfoByLv(heroData.aptLv)
			aptitude = math.random(info.apt.min, info.apt.max)
		end
	end
	self:SetAptitude(aptitude)
	
	if heroData.growing then
		self:SetGrowingRate(heroData.growing)
	else
	--	local atkType = JOB.GetJobAttackType(heroData.job)
		self:SetGrowingRate(CLSHERO.GenHeroGrowingRate(heroData.job))
	end
	
	self:InitPoint()
	self:InitAttrib(CLSHERO.GetAptLvByAptitude(aptitude))
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



--神将转世
local AttrkeptList = {["strength"] = 1, ["intellect"] = 1, ["agility"] = 1, ["captain"] = 1}

function clsGodHero:ProRebirth()
	local attrList = {}
	self:SetRebirthTimes(1)
	local rebirthPoint = self:GetRebirthTimes() * 50
--[[	for k in pairs(AttrkeptList) do
		attrList[k] = math.floor(self:GetAttr(k) * REBIRTH_ATTR_KEPT)
	end]]
	local aptLv = self:GetAptLv() + 1
	local info = CLSHERO.GetHeroAptInfoByLv(aptLv)
	self.__save.Lv = 1
	self.__save.exp = 0
	self:SetAptitude(math.random(info.apt.min, info.apt.max))
	self:InitAttrib(aptLv)
	self:ResetPoint(rebirthPoint + info.point)
--[[	for k, v in pairs(attrList) do
		self:SetAttr(k, v)
	end]]
	self:AssignPoint()
end

--转世次数
function clsGodHero:GetRebirthTimes()
	return self.__save.rebirth or 0 
end

function clsGodHero:SetRebirthTimes(data)
	self.__save.rebirth = (self.__save.rebirth or 0) + data
	if self.__save.rebirth < 0 then
		self.__save.rebirth = 0
	elseif self.__save.rebirth > MAX_REBIRTH_TIMES then
		self.__save.rebirth = MAX_REBIRTH_TIMES
	end
end

function clsGodHero:GetMaxAptLv()
	return 1 + MAX_REBIRTH_TIMES
end

function clsGodHero:GetMaxLv()
	return math.min(HeroMaxLvList[self:GetAptLv()], SERVER_ROLE_MAX_LV)
end

function clsGodHero:GetRebirthNeedLv()
	return HeroMaxLvList[self:GetAptLv()]
end

function clsGodHero:SetExp(data, type, flag)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if not flag then
		if not user:IsGodHeroFuncOpen() then
			return
		end
		data = math.floor(data / 2)
	end
	self.__save.exp = self.__save.exp + data
	local expMax = tonumber(self:GetUpgradeExp())
	if self.__save.exp >= expMax then		
		local lv = self:GetLv()
		if lv >= self:GetMaxLv() then
			self.__save.exp = expMax
		else
			self.__save.exp = self.__save.exp - expMax
			self:Upgrade()
		end
	end
	DATA.log(string.format("%s获得经验 %s ,for %s", self:GetName(), data, type or ""), "godHero.log")
end

function clsGodHero:Upgrade()
	Super(clsGodHero).Upgrade(self)
	if self:GetLv() >= self:GetMaxLv() then
		if self:GetRebirthTimes() >= MAX_REBIRTH_TIMES then
			self:SendGodHeroMsg("top")
		else
			self:SendGodHeroMsg("maxLv")
		end
	end
end

function clsGodHero:SendGodHeroMsg(type)
	local roleId = self:GetOwner()
	local msg
	if type == "maxLv" then
		msg = string.format("您的神将【%s】等级达到%d级，可在【神魂祭坛】进行转世操作。", self:GetName(), self:GetLv())
	elseif type == "top" then
		local tempItem = BASEITEM.NewItem(BASEITEM.GetHeroCarIdByAptLv(self:GetAptLv()), 1, {save = self:GetSaveData()})
		local param = {goodsAttr = {[1] = tempItem:GetTipMsg()}}
		CHAT.SendSysMsg("system", string.format("玩家【%s】的神魂[%s]在尘世间的历练已满，终于修成大道。", CHAT.GetFormatedNick(USER.GetUserNickById(roleId)), self:GetName()), nil, param)
		CHAT.SendSysNotice(string.format("玩家【%s】的神魂[%s]在尘世间的历练已满，终于修成大道。", USER.GetUserNickById(roleId), self:GetName()), nil, true)
		msg = string.format("您的神将【%s】在尘世间的历练已满，终于修成大道。", self:GetName())
	end
	if msg then
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			ERROR.TipMsg(con, msg)
		end
	end
end


function clsGodHero:GetGodHeroDetail(flag)
	local sData = {}
	sData.heroId = self:GetId()
--	sData.headId = self:GetHeadId()
	sData.name = self:GetName()
	sData.job = self:GetJob()
	sData.modelId = self:GetModelId()
	sData.aptLv = self:GetAptLv()
	sData.Lv = self:GetLv()
--	sData.exp = self:GetExp()
--	sData.expMax = self:GetUpgradeExp()
	sData.rebirth = self:GetRebirthTimes()
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(self:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(self:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(self:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(self:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(self:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(self:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(self:GetTotalAttr("speed"))
--[[	sData.strength = self:GetStrength()
	sData.intellect = self:GetIntellect()
	sData.agility = self:GetAgility()
	sData.captain = self:GetCaptain()]]
	local skillInfo = self:GetSkill("4")
	sData.skillList = {}
	if skillInfo and skillInfo.id then
		table.insert(sData.skillList, {type = 4, skillId = skillInfo.id, Lv = skillInfo.Lv})
	end
	sData.force = CHAR.CalFightForce(sData)
	
	if flag then
		sData.maxLv = self:GetRebirthNeedLv()
		sData.maxAptLv = self:GetMaxAptLv()
		sData.pillCost = REBIRTH_PILL_NEED_INGOT
		sData.pillNeed = self:GetRebirthNeedPill()
		sData.inheritCost = INHERIT_NEED_INGOT
	end
	return sData
end

--炼化三生石
function clsGodHero:Refining(user, index, num)
	local stoneId = ExpStoneList[index]
	if user:GetItemNum(USERBASE.PACKAGE, stoneId) < num then
		return false, "三生石数量不足。"
	end
	local curLv = self:GetLv()
	if curLv >= self:GetMaxLv() then
		return false, "该神魂已经满级，无法继续炼化三生石。"
	end
	local info = BASEITEM.GetItemById(stoneId)
	local exp = info.num * num
	self:SetExp(exp, "refine", true)
	user:DelItem(USERBASE.PACKAGE, stoneId, num)
	return exp, self:GetLv() ~= curLv and self:GetLv() or nil
end

--武将传承
function clsGodHero:HeroInherit(user, hero)
	local Lv = self:GetLv()
	if Lv >= self:GetMaxLv() then
		return false, string.format("神魂【%s】已经满级，无法继续接受传承。", self:GetName())
	end
	local cost = hero:GetLv() * INHERIT_NEED_INGOT
	if user:GetTotalIngot() < cost then
		return false, "您的元宝不足，请充值。"
	end
	user:SetTotalIngot(-cost)
	local exp = math.floor(hero:GetSumExp() * INHERIT_EXP_RATE)
	self:SetExp(exp, "inherit", true)
	hero:ReInitHero()
	return exp, self:GetLv() ~= Lv and self:GetLv() or nil
end

function clsGodHero:GetRebirthNeedPill()
	return RebirthNeedPill[self:GetAptLv()]
end

function clsGodHero:HeroRebirth(autoBuy)
	local roleId = self:GetOwner()
	local user = assert(USER.GetOnlineUser(roleId))
	local need = self:GetRebirthNeedPill()
	local num = user:GetItemNum(USERBASE.PACKAGE, REBIRTH_PILL_ID)
	if num < need then
		if not autoBuy then
			return false, "护元丹数量不足。"
		else
			local cost = (need - num) * REBIRTH_PILL_NEED_INGOT
			if user:GetTotalIngot() < cost then
				return false, "元宝数量不足。"
			end
			user:SetTotalIngot(-cost)
		end
	end
	user:DelItem(USERBASE.PACKAGE, REBIRTH_PILL_ID, math.min(need, num))
	self:ProRebirth()
	local tempItem = BASEITEM.NewItem(BASEITEM.GetHeroCarIdByAptLv(self:GetAptLv()), 1, {save = self:GetSaveData()})
	CHAT.SendSysMsg("system", string.format("%s 的神魂[%s]成功转世重生，前世血脉带来了更强的属性。", CHAT.GetFormatedNick(user:GetName()), self:GetName()), nil, {goodsAttr = {[1] = tempItem:GetTipMsg()}})
	return true
end

--刷新神将列表
function RefreshHeroList(rate)
	local sData = {}
	for i = 1, MAX_FRESH_HERO_NUM do
		local buf = {}
		buf.state = 0		--未录用
		local sex = math.random(0, 1)
		local jobList = {}
		for k, v in pairs(JOB.GetJobTypeList()) do
			table.insert(jobList, k)
		end
		buf.job = jobList[math.random(1, #jobList)]		
		local modelList = JOB.GetModelList(buf.job, sex)
		buf.modelId = modelList[math.random(1, #modelList)]
		buf.headId = JOB.GenHeroHeadIdByModel(buf.modelId)
		buf.name = NAME.GenName(sex)
		buf.aptLv = 1
		buf.skillId = SKILL.GenRandomSkill(buf.job, 1)			--攻击
		if math.random() <= rate then
			buf.featureSkill = SKILL.GetJobFeatureSkill(buf.job)		--特性
		end
	--	local atkType = JOB.GetJobAttackType(buf.job)
		buf.growing = CLSHERO.GenHeroGrowingRate(buf.job)
		for k, v in pairs(CLSHERO.GenHeroAttrList(buf.growing, 1)) do
			buf[k] = v
		end
		buf.type = "godHero"
		table.insert(sData, buf)
	end
	return sData
end



function GetFormatedEmplList(city)
	local sData = {}
	local list = city:GetGodHeroList()
	for k, v in ipairs(list) do
		local buff = {}
		buff.index = k
		buff.state = v.state
		buff.modelId = v.modelId
		buff.name = v.name
		buff.job = v.job
		buff.aptLv = v.aptLv
		buff.Lv = 1
		buff.hpMax = v.hpMax
		buff.hp = v.hpMax
		buff.physcAtkMin, buff.physcAtkMax = v.physcAtk, v.physcAtk
		buff.physcDef = v.physcDef
		buff.magicAtkMin, buff.magicAtkMax = v.magicAtk, v.magicAtk
		buff.magicDef = v.magicDef
		buff.speed = math.ceil(v.speed)
		buff.skillList = {}
--[[		if v.skillId then
			table.insert(buff.skillList, {type = 1, skillId = v.skillId, Lv = 1})
		end]]
		if v.featureSkill then
			table.insert(buff.skillList, {type = 4, skillId = v.featureSkill, Lv = 1})
		end
		buff.force = CHAR.CalFightForce(buff)
		table.insert(sData, buff)
	end
	return sData
end

function GetFormatedHeroList(user)
	local sData = {}
	for k, v in pairs(user:GetHeroList()) do
		if v:GetType() ~= "godHero" then
			local buff = {}
			buff.heroId = k
			buff.aptLv = v:GetAptLv()
			buff.name = v:GetName()
			buff.headId = v:GetHeadId()
			buff.Lv = v:GetLv()
			
			buff.job = v:GetJob()
			buff.modelId = v:GetModelId()
			buff.hp = v:GetHp()
			buff.hpMax = math.ceil(v:GetTotalAttr("hpMax"))
			buff.physcAtkMin = math.ceil(v:GetTotalAttr("physcAtkMin"))
			buff.physcAtkMax = math.ceil(v:GetTotalAttr("physcAtkMax"))
			buff.physcDef = math.ceil(v:GetTotalAttr("physcDef"))
			buff.magicAtkMin = math.ceil(v:GetTotalAttr("magicAtkMin"))
			buff.magicAtkMax = math.ceil(v:GetTotalAttr("magicAtkMax"))
			buff.magicDef = math.ceil(v:GetTotalAttr("magicDef"))
			buff.speed = math.ceil(v:GetTotalAttr("speed"))
			table.insert(sData, buff)
		end
	end
	table.sort(sData, function(a, b) return a.Lv > b.Lv or (a.Lv == b.Lv and a.aptLv > b.aptLv) end)
	return sData
end

local s_godHero_info = function(connection, data)
	if data ~= 0 and data ~= 1 then 
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用召唤神魂功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local hero = user:GetGodHero()
	local sData = {}
	if hero then
		sData = hero:GetGodHeroDetail(data == 1)
		sData.hasHero = 1
	else
		sData.hasHero = 0
	end
	connection:SendProtocol("c_godHero_info", sData)
end

local s_godHero_emplList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用召唤神魂功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local sData = {}
	sData.freshCost = HERO_FRESH_COST
	sData.list = GetFormatedEmplList(city)
	sData.needRemind = user:GetSetting(SETTING.GODHERO_FRESH_NO_REMIND) and 0 or 1
	connection:SendProtocol("c_godHero_emplList", sData)
end

--刷新神将列表
local s_godHero_refresh = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	if user:GetGold() < HERO_FRESH_COST then
		return ERROR.TipMsg(connection, "您的钱币不足。")
	end
	user:SetGold(-HERO_FRESH_COST)
	local rate = city:GetInitPhyle() == "npc" and NPC_CITY_FRESH_RATE or NORMAL_CITY_FRESH_RATE
	local sData = RefreshHeroList(rate)
	city:SetGodHeroList(sData)
	connection:SendProtocol("c_godHero_refresh", "true")
	PROTOCOL.s_godHero_emplList(connection, "get")
end
--招募
local s_godHero_hire = function(connection, data)
	data = tonumber(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local curHero = user:GetGodHero()
	if curHero then
		user:DelHero(curHero:GetId())
--		return ERROR.TipMsg(connection, "您已拥有了一名神魂。\n如需更换，请先解雇当前神魂。")
	end
	local info = city:IsGodHeroExist(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的神魂信息。")
	elseif info.state == 1 then
		return ERROR.TipMsg(connection, "指定的神魂已经被录用过了。")
	end
	city:SetHeroEmploy(data)
	user:AddHero("godHero", info)
	connection:SendProtocol("c_godHero_hire", data)
	PROTOCOL.s_godHero_info(connection, 1)
end
--炼化
local s_godHero_refining = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	data.index = tonumber(data.index)
	if not ExpStoneList[data.index] then
		return ERROR.TipMsg(connection, "协议错误，无效的三生石类型。")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的三生石数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local hero = user:GetGodHero()
	if not hero then
		return ERROR.TipMsg(connection, "您尚未招募神将。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = hero:Refining(user, data.index, data.num)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_godHero_refining", {index = data.index, num = data.num, exp = ret, Lv = msg})
end
--传承
local s_godHero_inherit = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local hero = user:GetGodHero()
	if not hero then
		return ERROR.TipMsg(connection, "您尚未招募神魂。")
	end
	local target = user:GetHero(data)
	if not target then
		return ERROR.TipMsg(connection, "指定的武将信息无效。")
	elseif target:GetType() == "godHero" then
		return ERROR.TipMsg(connection, "不能指定神将进行传承。")
	end
	local ret, msg = hero:HeroInherit(user, target)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_godHero_inherit", {heroId = data, exp = ret, Lv = msg})
end
--转世
local s_godHero_rebirth = function(connection, data)
	data = tonumber(data)
	if data ~= 0 and data ~= 1 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local hero = user:GetGodHero()
	if not hero then
		return ERROR.TipMsg(connection, "您尚未招募神魂。")
	end
	if hero:GetLv() < hero:GetRebirthNeedLv() then
		return ERROR.TipMsg(connection, "神魂等级不足，不能进行转世。")
	end
	if hero:GetAptLv() >= hero:GetMaxAptLv() then
		return ERROR.TipMsg(connection, "神魂已经达到最高品质。")
	end
	local ret, msg = hero:HeroRebirth(data == 1)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_godHero_rebirth", data)
end

local s_godHero_select = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local godHero = user:GetGodHero()
	if not godHero then
		return ERROR.TipMsg(connection, "您尚未招募神魂。")
	end
	local hero = user:GetHero(data)
	if not hero then
		return ERROR.TipMsg(connection, "指定的武将信息无效。")
	end
	local sData = {}
	sData.heroId = data
	sData.name = hero:GetName()
	sData.job = hero:GetJob()
	sData.modelId = hero:GetModelId()
	sData.aptLv = hero:GetAptLv()
	sData.Lv = hero:GetLv()
	sData.hp = hero:GetHp()
	sData.hpMax = math.ceil(hero:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(hero:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(hero:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(hero:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(hero:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(hero:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(hero:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(hero:GetTotalAttr("speed"))
	sData.inheritExp = math.floor(hero:GetSumExp() * INHERIT_EXP_RATE)
	sData.previewLv = godHero:GetPreviewLv(sData.inheritExp)
	connection:SendProtocol("c_godHero_select", sData)
end

local s_godHero_heroList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "您尚未进行城池依附。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能使用神魂召唤功能。")
	end
	if not city:HasGodHero() then
		return ERROR.TipMsg(connection, "您的城池不具有召唤神魂的功能。")
	end
	local sData = GetFormatedHeroList(user)
	connection:SendProtocol("c_godHero_heroList", sData)
end

function __init__()
	PROTOCOL.s_godHero_info = s_godHero_info
	
	PROTOCOL.s_godHero_emplList = s_godHero_emplList
	PROTOCOL.s_godHero_refresh = s_godHero_refresh
	PROTOCOL.s_godHero_hire = s_godHero_hire
	
	PROTOCOL.s_godHero_refining = s_godHero_refining
	PROTOCOL.s_godHero_inherit = s_godHero_inherit
	PROTOCOL.s_godHero_rebirth = s_godHero_rebirth
	
	PROTOCOL.s_godHero_heroList = s_godHero_heroList
	PROTOCOL.s_godHero_select = s_godHero_select
end
 
