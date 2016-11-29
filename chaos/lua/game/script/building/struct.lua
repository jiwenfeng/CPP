--建筑基类

local MAX_HERO_STAY_NUM = 2
local HERO_ATTR_ADD_RATE = 0.0001
local UpLvInfoList = {}		--设施升级信息列表

function GetUpLvInfo(type, lv)
	if not UpLvInfoList[type] then
		return
	end
	return UpLvInfoList[type][lv]
end

function GetUpLvInfoList(type)
	return UpLvInfoList[type]
end

--state 1 开启 0 未开启
local structList = {
	["mainHall"] = {state = 1, tribeLv = 1},	--议事厅
	["retreat"] = {state = 0, tribeLv = 20},		--修炼场
	["market"] = {state = 0, tribeLv = 3},		--市集
	["house"] = {state = 1, resource = 1, tribeLv = 1},			--民宅
	["spar"] = {state = 0, resource = 1, tribeLv = 10},		--晶石矿
	["barrack"] = {state = 0, resource = 1, tribeLv = 5},		--兵营
	["academy"] = {state = 0, tribeLv = 10},		--学馆
	["embassy"] = {state = 0, tribeLv = 30},		--使馆
	["godAltar"] = {state = 0, tribeLv = 1},		--使馆
	["furnace"] = {state = 0, tribeLv = 40},		--天工炉
}

function GetStructList()
	return structList
end

--------------------------------------------------------------------------------------------------
clsStruct = clsObject:Inherit()

function clsStruct:__init__()
	Super(clsStruct).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsStruct:initsavedata()
	self.__save.owner = nil
	self.__save.Lv = 0
end

function clsStruct:inittempdata()
	self.__temp.callout = {}
end

--初始化
function clsStruct:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)		--势力
	self:SetStructType(structType)	--建筑物类型
end

--部落类型
function clsStruct:SetType(data)
	self.__save.phyle = data
end

--建筑物类型
function clsStruct:GetType()
	return self.__save.phyle
end

--获取建筑物的最大等级
function clsStruct:GetMaxLv()
	local type = self:GetStructType()
	local info = GetUpLvInfoList(type)
	if not info then
		return 0
	end
	return #info
end

--属主
function clsStruct:GetOwner()
	return self.__save.owner
end

function clsStruct:SetOwner(data)
	self.__save.owner = data
end

--建筑类型
function clsStruct:GetStructType()
	return self.__save.structType
end

function clsStruct:SetStructType(data)
	self.__save.structType = data
end

--获取建筑所在部落对象
function clsStruct:SetAttachTribe(tribe)
	self.__temp.attached = tribe
end

function clsStruct:GetAttachTribe()
	return self.__temp.attached
end

--等级
function clsStruct:GetLv()
	return self.__save.Lv or 0
end

function clsStruct:SetLv(data)
	data = assert(tonumber(data))
	if data == 0 then
		return
	end
	self.__save.Lv = (self.__save.Lv or 0) + data
	if self.__save.Lv <= 0 then
		self.__save.Lv = 0
	elseif self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	end
end

function clsStruct:GetHeroAttrAddRate()
	local attr = structList[self:GetStructType()].attr
	if not attr then
		return 0
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local num = 0
	for k, v in pairs(self:GetHeroStayList()) do
		if v.heroId then
			local hero = user:GetHero(v.heroId)
			if hero then
				num = num + hero:GetTotalAttr(attr)
			end
		end
	end
	return num * HERO_ATTR_ADD_RATE
end

function clsStruct:CheckUpgrade(lv)
	if lv >= self:GetMaxLv() then
		return false, "该建筑已经达到最高等级。"
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	local mainHall = tribe:GetStruct("mainHall")
	local lvInfo = GetUpLvInfo(self:GetStructType(), lv)
	if lvInfo.condt.needUserLv and user:GetLv() < lvInfo.condt.needUserLv then
		return false, "您的主将等级不足。"
	end
	if lvInfo.condt.needMainLv and mainHall:GetLv() < lvInfo.condt.needMainLv then
		return false, "议事厅等级不足。"
	end
	if lvInfo.condt.needGold and user:GetGold() < lvInfo.condt.needGold then
		return false, "钱币不足。"
	end
	if lvInfo.condt.needSpar and tribe:GetResourceStat("spar", "num") < lvInfo.condt.needSpar then
		return false, "晶石数量不足"
	end
	return true
end

--计算升级cd时间
function clsStruct:GetUpgradeCdTime(lv)
	local user = USER.GetOnlineUser(self:GetOwner())
	if VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_UPGRADELIST_NO_CD) then
		return 0
	end
	lv = lv or self:GetLv()
	local info = GetUpLvInfo(self:GetStructType(), lv)
	local timecut = ACADEMY.GetTechEffectByType(self:GetOwner(), "timeCut")	
	local pet = user:GetPet()
	local rate = 0
	if pet then
		rate = pet:GetTalentAdd(311020)
	end
	local phyle = USER.GetUserNickInfo(self:GetOwner(), "phyle")
	if phyle and phyle ~= "phoenix" then
		rate = rate - 0.02
	end
	return math.ceil(info.timelong * (1 + timecut + rate))
end

function clsStruct:GetDescribe(lv)
	lv = lv or self:GetLv()
	local info = GetUpLvInfo(self:GetStructType(), lv)
	if structList[self:GetStructType()].attr then
		return string.format(info.describe, self:GetResIncome() * self:GetResIncomeRate())
	else
		return info.describe
	end
end

function clsStruct:GetUpgradeInfo()
	local lv = self:GetLv()
	local type = self:GetStructType()
	local sData = {type = type, Lv = lv, maxLv = self:GetMaxLv()}
	if lv and lv > 0 then
		local lvInfo = GetUpLvInfo(self:GetStructType(), lv)
		sData.describe = self:GetDescribe()
		local nextInfo = GetUpLvInfo(self:GetStructType(), lv + 1)
		if nextInfo then
			sData.nextDesc = self:GetDescribe(lv + 1)
			sData.needUserLv = nextInfo.needUserLv
			sData.mainLv = lvInfo.condt.needMainLv
			sData.condt = {}
			sData.condt.gold = lvInfo.condt.needGold
			sData.timelong = self:GetUpgradeCdTime()
		end
	end
	return sData
end

function clsStruct:GetNextLvInfo()
	local sData = {}
	local lv = self:GetLv()
	if lv >= MAX_LV then
		return sData	
	end
	local LvInfo = GetUpLvInfo(self:GetStructType(), lv + 1)	
	for k, v in ipairs(LvInfo.info) do
		table.insert(sData, v)
	end
	local owner = USER.GetOnlineUser(self:GetOwner())
	local identity = owner:GetIdentity()
	for k, v in pairs(LvInfo[identity] or {}) do
		table.insert(sData, v)
	end
	return sData
end

--设施升级
function clsStruct:Upgrade(offlineFlag)
	self:SetLv(1)	
end

function clsStruct:SetTimeStamp(data)
	self.__save.timestamp = data
end

function clsStruct:GetTimeStamp()
	return self.__save.timestamp
end

function clsStruct:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsStruct:Restore(data)
	self.__save = data
end

function clsStruct:ProStructFunc()
end

function clsStruct:LoginStartCallOut()
end

function clsStruct:OfflineRemoveCallOut()
end

function clsStruct:GetCallOut()
	return self.__temp.callout
end

function __init__()
	local TABLELIST = Import("autocode/uplv_mainHall.lua")
	UpLvInfoList["mainHall"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_retreat.lua")
	UpLvInfoList["retreat"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_house.lua")
	UpLvInfoList["house"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_market.lua")
	UpLvInfoList["market"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_barrack.lua")
	UpLvInfoList["barrack"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_academy.lua")
	UpLvInfoList["academy"] = TABLELIST.GetTable()
	
	TABLELIST = Import("autocode/uplv_embassy.lua")
	UpLvInfoList["embassy"] = TABLELIST.GetTable()

	TABLELIST = Import("autocode/uplv_sparMine.lua")
	UpLvInfoList["spar"] = TABLELIST.GetTable()
end
