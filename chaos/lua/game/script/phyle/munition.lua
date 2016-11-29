--军资库

local AllExchangeList = {}

function GetAllExchangeList()
	return AllExchangeList
end

function GetExchangeInfoById(id)
	return AllExchangeList[id]
end
-----------------------------------
clsMunition = PHYLESTRUCT.clsStruct:Inherit()

function clsMunition:__init__()
	Super(clsMunition).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsMunition:initsavedata()
	self.__save.Lv = 0
	self.__save.exp = 0
end

function clsMunition:inittempdata()
end

--初始化
function clsMunition:InitStruct(phyle, structType)
	self:SetPhyle(phyle)
	self:SetStructType(structType)
	self:SetLv(1)
end

function clsMunition:GetExchangeList()
	local lv = self:GetLv()
	local sData = {}
	for k, v in pairs(GetAllExchangeList()) do
		local buff = {}
		buff.goodsId = k
		buff.needFeat = v.needFeat
		buff.needStrLv = v.needLv
		table.insert(sData, buff)
	end
	return sData
end

function clsMunition:GetExchangeNum(lv)
	lv = lv or self:GetLv()
	local num = 0
	for k, v in pairs(GetAllExchangeList()) do
		if v.needLv == lv then
			num = num + 1
		end
	end
	return num
end

function clsMunition:GetItemExchangeInfo(id)
	return GetExchangeInfoById(id)
end

function clsMunition:GetStructName()
	return "军资库"
end


function __init__()

	local EXCHG = Import("autocode/phyleExchange.lua")
	AllExchangeList = EXCHG.GetTable()
	
end
