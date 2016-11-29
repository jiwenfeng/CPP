--学馆

local AllTechList = {}


function GetTechInfoById(id)
	return AllTechList[id] or FORMATION.GetFormationInfoById(id)
end

function GetTechById(id)
	return AllTechList[id]
end

function GetAllTechInfoList()
	local list = {}
	for k, v in pairs(AllTechList) do
		list[k] = v
	end
	for k, v in pairs(FORMATION.GetAllFormationList()) do
		local buff = table.copy(v)
		buff.type = 3
		list[k] = buff
	end
	return list
end

function GetTechEffectByType(roleId, type)
	local effect = 0
	local user = USER.GetOnlineUser(roleId)
	if user then
		local tribe = user:GetTribe()
		if tribe then
			local academy = tribe:GetStruct("academy")
			if academy then
				for k, v in pairs(academy:GetTechList()) do
					effect = effect + (v:GetAttr()[type] or 0)
				end
			end
		end
	end
	return effect
end
--------------------clsTech------------------------------
clsTech = clsObject:Inherit()

function clsTech:__init__()
	Super(clsTech).__init__(self)
	self.__save = {}
	self.__temp = {}
end

function clsTech:GetOwner()
	return self.__temp.roleId
end

function clsTech:SetOwner(roleId)
	self.__temp.roleId = roleId
end

function clsTech:SetId(id)
	self.__save.id = id
end

function clsTech:GetId()
	return self.__save.id
end

function clsTech:SetLv(Lv)
	self.__save.Lv = math.max(Lv, 0)
end

function clsTech:UpdateLv()
	self.__save.Lv = (self.__save.Lv or 0) + 1
end

function clsTech:GetLv()
	return self.__save.Lv
end

function clsTech:GetName()
	local info = GetTechById(self:GetId())
	return info.name
end

function clsTech:Upgrade()
	self:UpdateLv()
	return true
end

function clsTech:GetDescribe(Lv)
	Lv = Lv or self:GetLv()
	if not Lv or Lv > self:GetMaxLv() then
		return
	end
	local info = GetTechById(self:GetId())
	return info.list[Lv].describe
end

function clsTech:GetUpgradeSpar(Lv)
	Lv = Lv or self:GetLv()
	if Lv > self:GetMaxLv() then
		return
	end
	local info = GetTechById(self:GetId())
	return info.list[Lv].needSpar
end

function clsTech:GetUpgradeTimelong(Lv)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_UPGRADELIST_NO_CD) then
		return 0
	end

	Lv = Lv or self:GetLv()
	if Lv > self:GetMaxLv() then
		return
	end
	local rate = 0
	local phyle = USER.GetUserNickInfo(self:getOwner(), "phyle")
	if phyle and phyle ~= "phoenix" then
		rate = rate + 0.02
	end
	local info = GetTechById(self:GetId())
	return math.ceil(info.list[Lv].timelong * (1 - rate))
end

function clsTech:GetAttr(Lv)
	Lv = Lv or self:GetLv()
	local info = GetTechById(self:GetId())
	return info.list[Lv].attr
end

function clsTech:GetMaxLv()
	local info = GetTechById(self:GetId())
	return #info.list
end

function clsTech:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsTech:RestoreData(data)
	self.__save = data
end

function clsTech:GetFormatInfo()
	local lv = self:GetLv()
	local sData = {}
	sData.techId = self:GetId()
	sData.name = self:GetName()
	sData.type = self:GetType()
	sData.Lv = lv
	sData.maxLv = self:GetMaxLv()
	if not lv then
		sData.state = "unlearn"
	else
		sData.state = "learned"
		sData.describe = self:GetDescribe()
		sData.sparNum = self:GetUpgradeSpar()
		sData.nextDesc = self:GetDescribe(lv + 1)
		if lv < self:GetMaxLv() then
			sData.needAcademyLv = lv + 1
		end
	end
	return sData
end
-----------------------------生产科技---------------------------
clsProductionTech = clsTech:Inherit()
function clsProductionTech:__init__()
	Super(self).__init__(self)
	self.__save = {}
	self.__temp = {}
end

function clsProductionTech:GetType()
	return 1
end
----------------------------战争科技----------------------------
clsFightTech = clsTech:Inherit()
function clsFightTech:__init__()
	Super(self).__init__(self)
	self.__save = {}
	self.__temp = {}
end

function clsFightTech:GetType()
	return 2
end

function clsFightTech:Upgrade()
	self:UpdateLv()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:FreshAllHeroTotalAttr()
	return true
end
---------------------------阵型科技------------------------------
clsFormTech = clsTech:Inherit()
function clsFormTech:__init__()
	Super(self).__init__(self)
	self.__temp = {}
end

function clsFormTech:SetId(id)
	self.__temp.id = id
end

function clsFormTech:GetId()
	return self.__temp.id
end

function clsFormTech:GetName()
	local info = GetTechById(self:GetId())
	return info.name
end

function clsFormTech:GetType()
	return 3
end

function clsFormTech:GetLv()
end

function clsFormTech:SetLv(data)
end

function clsFormTech:UpdateLv()
end

--阵型科技不可升级
function clsFormTech:Upgrade()
	return false, "科技阵型不能在此升级"
end

function clsFormTech:GetDescribe(Lv)
end

function clsFormTech:GetUpgradeSpar(Lv)
end

function clsFormTech:GetUpgradeTimelong(Lv)
end

function clsFormTech:GetAttr(Lv)
end

function clsFormTech:GetMaxLv()
end

function clsFormTech:GetFormatInfo()
	local sData = {}
	sData.techId = self:GetId()
	sData.name = self:GetName()
	sData.type = self:GetType()
	sData.describe = "占领非主城城池后可在阵型界面学习"
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	sData.state = user:GetCityIdentity() == CLSCITY.ID_MASTER and "learned" or "unlearn"
	return sData
end
----------------------------------------------------------

function NewTech(id, roleId)
	local info = GetTechById(id)
	if not info then
		return
	end
	local obj
	if info.type == 1 then
		obj = clsProductionTech:New()
	elseif info.type == 2 then
		obj = clsFightTech:New()
	elseif info.type == 3 then
		obj = clsFormTech:New()
	else
		return
	end
	obj:SetId(id)
	obj:SetOwner(roleId)
	return obj
end

--------------------------------------------------
clsAcademy = STRUCT.clsStruct:Inherit()

function clsAcademy:__init__()
	Super(clsAcademy).__init__(self)
	self.__save = {}
	self.__save.techList = {}
	self.__temp = {}
	self.__temp.techList = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsAcademy:initsavedata()	
	self.__save.Lv = 0
	self.__save.techList = {}
	self.__save.uplimit = 0
	self.__save.point = 0
end

function clsAcademy:inittempdata()
end

function clsAcademy:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:AddUpgradeList(owner)
	local info = STRUCT.GetUpLvInfo(structType, self:GetLv())
	self:SetTechUplimit(info.uplimit)
end

function clsAcademy:AddUpgradeList(owner)
	local user = assert(USER.GetOnlineUser(owner))
	local obj = UPGRADELIST.NewList("academy", "1", UPGRADELIST.LIST_UNLOCK)
	user:AddUpgradeList("academy", "1", obj)
end

function clsAcademy:Upgrade()
	self:SetLv(1)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	self:SetTechUplimit(info.uplimit)
	return true
end

--科技等级上限
function clsAcademy:SetTechUplimit(data)
	self.__save.uplimit = data
end

function clsAcademy:GetTechUplimit()
	return self.__save.uplimit
end

function clsAcademy:AddTech(id, obj)
	id = tostring(id)
	self.__temp.techList[id] = obj
end

function clsAcademy:GetTechLv(id)
	id = tostring(id)
	local tech = self.__temp.techList[id]
	if tech then
		return tech:GetLv()
	end
	return 0
end

function clsAcademy:GetTechById(techId)
	return self.__temp.techList[tostring(techId)]
end

function clsAcademy:GetTechList()
	return self.__temp.techList
end

function clsAcademy:GetSaveData()
	self:SaveTechData()
	return table.deepcopy(self.__save)
end

function clsAcademy:Restore(data)
	self.__save = data
	self:RestoreTechData()
end

function clsAcademy:SaveTechData()
	self.__save.techList = {}
	for k, v in pairs(self.__temp.techList) do
		self.__save.techList[k] = v:GetSaveData()
	end
end

function clsAcademy:RestoreTechData()
	self.__temp.techList = {}
	for k, v in pairs(self.__save.techList) do
		local obj = NewTech(k, self:GetOwner())
		obj:SetLv(v.Lv)
		self:AddTech(k, obj)
	end
end

function clsAcademy:TechUpgrade(techId)
	local tech = self:GetTechById(techId) 
	local flag = false
	if not tech then
		tech = NewTech(techId, self:GetOwner())
		flag = true
	end
	local lv = tech:GetLv() or 0
	if lv >= tech:GetMaxLv() then
		return false, "该科技已经达到最大等级"
	end
	if lv >= self:GetTechUplimit() then
		return false, "学馆等级不足"
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	if not tribe then
		return false, "您还没有建立部落"
	end
	local spar = tech:GetUpgradeSpar(lv)
	if tribe:GetResourceStat("spar", "num") < spar then
		return false, "晶石数量不足"
	end
	local obj = user:GetIdleUpgradeList("academy")
	if not obj then
		return false, "科技队列冷却时间未到"
	end
	local ret, msg = tech:Upgrade() 
	if not ret then
		return ret, msg
	end
	if flag then
		self:AddTech(techId, tech)
	end
	tribe:ModResourceStat("spar", "num", -spar)
	obj:SetEndTimestamp(os.time() + tech:GetUpgradeTimelong())
	return true
end

function clsAcademy:GetFormatTechList()
	local sData = {}
	local roleId = self:GetOwner()
	for k in pairs(AllTechList) do
		local tech = self:GetTechById(k)
		if not tech then
			tech = NewTech(k, roleId)
		end
		table.insert(sData, tech:GetFormatInfo())
	end
	return sData
end

function clsAcademy:ClearTech()
	self.__temp.techList = {}
	self.__save.techList = nil
	self.__save.techList = {}
end

local s_tech_info = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local academy = assert(tribe:GetStruct("academy"))
	data = tostring(data)
	local tech = academy:GetTechById(data) or NewTech(data, connection.roleId)
	if not tech then
		return ERROR.TipMsg(connection, "无效的科技ID")
	end
	sData = tech:GetFormatInfo()
	connection:SendProtocol("c_tech_info", sData)
end

local s_tech_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local academy = assert(tribe:GetStruct("academy"))
	local sData = academy:GetFormatTechList()
	connection:SendProtocol("c_tech_list", sData)
end

local s_tech_upgrade = function(connection, data)
	data.techId = tostring(data.techId)
	if type(data) ~= "table" or not data.techId or not data.Lv then
		return ERROR.TipMsg(connection, "协议错误，请检查协议格式。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local academy = tribe:GetStruct("academy")
	if not academy then
		return ERROR.TipMsg(connection, "您还没有建立学馆，无法升级科技")
	end
	local ret, msg = academy:TechUpgrade(data.techId)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_tech_upgrade", {techId = data.techId, Lv = data.Lv + 1})
	user:SendUpgradeListUpdate("tech", 1)
end

local s_tech_upgradeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local obj = user:GetOneUpgradeList("academy", "1")
	if not obj then
		return ERROR.TipMsg(connection, "您还没有建立学馆")
	end
	local timelong = math.max(obj:GetEndTimestamp() - os.time(), 0)
	connection:SendProtocol("c_tech_upgradeList", timelong)
end

function __init__()
	TECHLIST = Import("autocode/techList.lua")
	AllTechList = TECHLIST.GetTable()
	PROTOCOL.s_tech_list = s_tech_list
	PROTOCOL.s_tech_info = s_tech_info
	PROTOCOL.s_tech_upgrade = s_tech_upgrade
	PROTOCOL.s_tech_upgradeList = s_tech_upgradeList
end

