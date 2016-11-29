--名将
local MAX_HERO_SHOWN = 5			--最大展示武将数量
local MIN_APTLV_SHOWN = 4			--只展示紫色及以上的武将

local AllFamousCopyList = {}		--名将副本列表

function GetAllFamousCopyList()
	return AllFamousCopyList
end

function GetFamousCopyById(id)
	return AllFamousCopyList[id]
end
----------------------------------------------------------
local AllFamousHeroList = {}		--名将录静态信息
local FamousHeroTypeList = {}		--按类型分类


function GetAllFamousHeroInfoList()
	return AllFamousHeroList
end

function GetFamousHeroInfoById(id)
	return AllFamousHeroList[id]
end


function SaveFamousHeroList()
	os.execute("mkdir -p ../client/famousHero")
	for k in pairs(GetAllFamousHeroInfoList()) do
		local path = string.format("../client/famousHero/%s.txt", k)
		local fd = io.open(path, "w")
		assert(fd)
		local sData = GetFamousHeroDetailById(k)
		fd:write(JSON.encode(sData))
		fd:close()
	end
end


function GetHeroListByCopy(copyId)
	return FamousHeroTypeList[copyId]
end

------------------------------------
clsHero = CLSHERO.clsHero:Inherit()


function clsHero:InitHero(type, heroId, owner, heroData)	
	local template = heroData.template
	local tempInfo = GetFamousHeroInfoById(template)		--template : 名将id
	if not tempInfo then
		return
	end
	self:SetType(type)
	self:SetId(heroId)
	self:SetOwner(owner)
	
	self:SetName(tempInfo.name)
	self:SetModelId(tempInfo.modelId)
	self:SetHeadId(tempInfo.headId)
	self:SetSex(tempInfo.headId % 2)
	self:SetTemplate(template)
	self:SetJob(tempInfo.job)
	
	self:SetMaxAptLv(tempInfo.maxAptLv)
	local info = CLSHERO.GetHeroAptInfoByLv(tempInfo.maxAptLv)
	self:SetAptitude(math.min(tempInfo.aptitude, info.apt.min))
	self:SetGrowingRate(CLSHERO.GenGrowingRate(tempInfo.grownRate))
	
	self:InitPoint()
	self:InitAttrib(CLSHERO.GetAptLvByAptitude(tempInfo.aptitude))
	self:PresetAttr(tempInfo)
	self:InitSpecialAttrib()
	
	self:InitSkillList()
	if heroData.skillId then		--天赋技能
		self:SetSkill("1", heroData.skillId, 1)
	end
	if tempInfo.skillId then		--天赋技能
		self:SetSkill("4", tempInfo.skillId, 1)
	end
	
	self:AssignPoint()
	SetFamousHeroDataById(template, owner)
end

-----------------------------------
local FamousHeroDataList = {}		--名将录录用登记
local path_famousHero = "chaos.famousHero"

function GetAllFamousHeroData()
	return FamousHeroDataList
end

function GetFamousHeroDataById(id)
	return FamousHeroDataList[id]
end

--保存名将数据
function SetFamousHeroDataById(id, owner)
	FamousHeroDataList[id] = FamousHeroDataList[id] or {}
	local info = FamousHeroDataList[id]
	info.ownerList = info.ownerList or {}
	if owner then
		info.ownerList[owner] = 1
		info.state = 1
		info.owner = owner
	else
		info.state = 0
		info.owner = nil
	end
	DATA.SaveData(path_famousHero, FamousHeroDataList, id)
end

--副本名称列表
function GetFamousCopyNameList(user)
	local sData = {}
	for k, v in pairs(GetAllFamousCopyList()) do
		local buff = {copyId = k, name = v.name}
		local state = user:IsMissionOver(v.needMissionId) and 1 or 0
		buff.state = state
		if state == 0 then
			buff.needMission = v.needMissionName
		end
		table.insert(sData, buff)
	end
	table.sort(sData, function(a, b) return tonumber(a.copyId) < tonumber(b.copyId) end)
	return sData
end
--[[副本状态列表
function GetFamousCopyStateList(user)
	local sData = {}
	for k, v in pairs(GetAllFamousCopyList()) do
		local buff = {copyId = k}
		buff.name = v.name
		if curCopyId >= tonumber(v.needCopyId) then
			buff.state = 1
		else
			buff.state = 0
			buff.needCopy = v.needCopyName
		end
		table.insert(sData, buff)
	end
	table.sort(sData, function(a, b) return tonumber(a.copyId) < tonumber(b.copyId) end)
	return sData
end]]

function GetFormatedHeroListById(copyId, user)
	local roleId = user:GetRoleId()
	local templateInList = {}
	for k, v in pairs(user:GetHeroList()) do
		local template = v:GetTemplate()
		if template then
			templateInList[template] = k
		end
	end
	local heroInfoList = GetHeroListByCopy(copyId)
	local sData = {}
	for aptLv, list in pairs(heroInfoList) do
		if aptLv >= MIN_APTLV_SHOWN then
			for _, v in pairs(list) do
				local info = GetFamousHeroInfoById(v)
				local buff = {}
				buff.name = info.name
				buff.headId = info.headId
				buff.job = info.job
				buff.aptLv = aptLv
				local heroId = templateInList[v]
				if heroId then
					local hero = user:GetHero(heroId)
			--		buff = hero:GetSimpleInfo(4)
					buff.heroId = v
					buff.Lv = hero:GetLv()
					buff.state = 2
				else
					buff.heroId = v
					buff.Lv = 1
					local heroData = GetFamousHeroDataById(v)
					if heroData and heroData.ownerList[roleId] then
						buff.state =  1
					else
						buff.state = 0
					end
				end
				table.insert(sData, buff)
			end
		end
	end
	table.sort(sData, function(a, b) return
			a.aptLv > b.aptLv 
			or (a.aptLv == b.aptLv and tonumber(a.heroId) < tonumber(b.heroId))
		end)
	return sData
end


--获取名将展示列表
function GetFamousHeroShowList()
	local copyList = {}
	for k in pairs(GetAllFamousCopyList()) do
		table.insert(copyList, k)
	end
	local list = {}
	local inList = {}	
	while #list < MAX_HERO_SHOWN do
		local copyId = copyList[math.random(1, #copyList)]
		local heroList = GetHeroListByCopy(copyId)
		local aptLv = MIN_APTLV_SHOWN
		if math.random() == 1 then
			aptLv = MIN_APTLV_SHOWN + 1
		end
		local id = heroList[aptLv][math.random(1, #heroList[aptLv])]
		if not inList[id] then
			table.insert(list, {index = #list + 1, heroId = id})
			inList[id] = true
		end
	end
	return list
end

--根据资质返回一个英魂id	(exception 排除列表)
function GenFamousHeroByAptLv(user, aptLv, copyId, exception)
	local copyList = {}
	if copyId then
		table.insert(copyList, copyId)
	else
		for k, v in pairs(GetAllFamousCopyList()) do
			if user:IsMissionOver(v.needMissionId) then
				table.insert(copyList, k)
			end
		end
	end
	if #copyList < 1 then
		return
	end
	local function get_id(copyId, aptLv)		
		local heroList = GetHeroListByCopy(copyId)
		local list = {}
		for _, v in pairs(heroList[aptLv] or {}) do
			local famousData = GetFamousHeroDataById(v)
			if (not famousData or famousData.state == 0) and not exception[v] then
				table.insert(list, v)
			end
		end
		if #list >= 1 then
			return list[math.random(1, #list)]
		end
	end
	
	local copyId = copyList[math.random(1, #copyList)]
	local heroId = get_id(copyId, aptLv)		--先随机一个副本抽取
	if heroId then
		return heroId
	end
	for _, v in pairs(copyList) do			--按顺序抽取其他副本
		if v ~= copyId then
			local heroId = get_id(v, aptLv)
			if heroId then
				return heroId
			end
		end
	end
	if aptLv == 5 then			--橙色英魂比较少，都被招募完毕的话，则降级刷出紫色英魂
		return GenFamousHeroByAptLv(user, aptLv - 1, copyId, exception)
	end
end

--名将展示数据
function GetFamousHeroShownData(id)
	local info = GetFamousHeroInfoById(id)
	local buff = {}
	buff.heroId = id
	buff.modelId = info.modelId
	buff.name = info.name
	buff.aptLv = CLSHERO.GetAptLvByAptitude(info.aptitude)
	buff.job = info.job
	buff.template = id
	
	buff.Lv = 1
	buff.hpMax = info.hpMax
	buff.hp = info.hpMax
	buff.physcAtkMin, buff.physcAtkMax = info.physcAtk, info.physcAtk
	buff.physcDef = info.physcDef
	buff.magicAtkMin, buff.magicAtkMax = info.magicAtk, info.magicAtk
	buff.magicDef = info.magicDef
	buff.speed = math.ceil(info.speed)
	local skillInfo = BASESKILL.GetSkillInfo(info.skillId)
	if skillInfo then
		sData.skillList = {{type = 4, skillName = skillInfo.name, Lv = 1}}
	end
	return buff
end

--格式化输出名将数据（给前台保存）
function GetFamousHeroDetailById(id)
	local info = GetFamousHeroInfoById(id)
	local sData = {}
	sData.heroId = id
	sData.name = info.name
	sData.headId = info.headId
	sData.modelId = info.modelId
	sData.job = info.job
	sData.Lv = info.Lv or 1
	sData.aptLv = CLSHERO.GetAptLvByAptitude(info.aptitude)
	sData.strength = info.strength
	sData.intellect = info.intellect
	sData.agility = info.agility
	sData.captain = info.captain
	sData.hpMax = info.hpMax
	sData.physcAtkMin, sData.physcAtkMax = info.physcAtk, info.physcAtk
	sData.physcDef = info.physcDef
	sData.magicAtkMin, sData.magicAtkMax = info.magicAtk, info.magicAtk
	sData.magicDef = info.magicDef
	sData.speed = info.speed
	sData.force = info.force
	sData.combin = info.combin
	sData.describe = info.describe
	local skillInfo = BASESKILL.GetSkillInfo(info.skillId)
	if skillInfo then
		sData.skillList = {{type = 4, skillName = skillInfo.name, Lv = 1}}
	end
	return sData
end





local s_hero_showList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetFamousHeroShowList()
	connection:SendProtocol("c_hero_showList", sData)
end

local s_famous_nameList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFamousCopyNameList(user)
	connection:SendProtocol("c_famous_nameList", sData)
end

local s_famous_stateList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFamousCopyNameList(user)
	connection:SendProtocol("c_famous_stateList", sData)
end

local s_famous_heroList = function(connection, data)
	if not GetFamousCopyById(data) then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {copyId = data}
	sData.list = GetFormatedHeroListById(data, user)
	connection:SendProtocol("c_famous_heroList", sData)
end

local s_famous_heroDetail = function(connection, data)
	if not GetFamousHeroInfoById(data) then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	local inList = false
	for k, v in pairs(user:GetHeroList()) do
		if v:GetTemplate() == data then
			sData = v:GetSimpleInfo()
			sData.heroId = data
			inList = true
		end
	end
	if not inList then
		sData = GetFamousHeroShownData(data)
		ERROR.TipMsg(connection, "该状态的武将信息应该读表。看到这个消息说明前台数据读取有误。")
	end
	connection:SendProtocol("c_famous_heroDetail", sData)
end

local s_famous_getOwner = function(connection, data)
	local famousInfo = GetFamousHeroInfoById(data)
	if not famousInfo then
		return ERROR.TipMsg(connection, "没有指定的英魂信息。")
	end
	local famousData = GetFamousHeroDataById(data)
	local sData = {heroId = data}
	if famousData then
		sData.nick = USER.GetUserNickById(famousData.owner)
	end
	connection:SendProtocol("c_famous_getOwner", sData)
end

function __init__()

	local function LoadTable(tbl)
		for k, v in pairs(tbl) do
			AllFamousHeroList[k] = v
			FamousHeroTypeList[v.copyId] = FamousHeroTypeList[v.copyId] or {}			--按副本分类
			local aptLv = CLSHERO.GetAptLvByAptitude(v.aptitude)	
			FamousHeroTypeList[v.copyId][aptLv] = FamousHeroTypeList[v.copyId][aptLv] or {}		--按资质分类
			table.insert(FamousHeroTypeList[v.copyId][aptLv], k)
			NAME.SetNameExist(v.name)
		end
	end
	--三国名将
	local SANGO = Import("autocode/famousHeroSango.lua")
	LoadTable(SANGO.GetTable())
	--金庸群侠
	local JINYONG = Import("autocode/famousHeroJinYong.lua")
	LoadTable(JINYONG.GetTable())


	local FAMOUS_COPY = Import("autocode/famousCopyList.lua")
	AllFamousCopyList = FAMOUS_COPY.GetTable()
	
	DATA.Restore(path_famousHero, FamousHeroDataList)
	
	PROTOCOL.s_hero_showList = s_hero_showList
	PROTOCOL.s_famous_stateList = s_famous_stateList
	
	PROTOCOL.s_famous_nameList = s_famous_nameList
	PROTOCOL.s_famous_heroList = s_famous_heroList
	PROTOCOL.s_famous_heroDetail = s_famous_heroDetail
	PROTOCOL.s_famous_getOwner = s_famous_getOwner
end
