--图腾

local ATTR_ADD_RATE_PER_LV = 0.01
local AttrAddList = {
	["physcAtkMin"] = 1, ["physcAtkMax"] = 1, ["physcDef"] = 1, ["hpMax"] = 1,
	["magicAtkMin"] = 1, ["magicAtkMax"] = 1, ["magicDef"] = 1, ["speed"] = 1,
}

-------------------------------------------
clsTotem = PHYLESTRUCT.clsStruct:Inherit()

function clsTotem:__init__()
	Super(clsTotem).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsTotem:GetAddRate()
	return ATTR_ADD_RATE_PER_LV 
end

function clsTotem:initsavedata()
	self.__save.Lv = 0
	self.__save.exp = 0
end

function clsTotem:inittempdata()
end

--初始化
function clsTotem:InitStruct(phyle, structType)
	self:SetPhyle(phyle)
	self:SetStructType(structType)
	self:SetLv(1)
end

function clsTotem:GetStructName()
	return "图腾"
end

--图腾属性加成
function clsTotem:GetAttrAddRate()
	local rate = ATTR_ADD_RATE_PER_LV * self:GetLv()
	local sData = {}
	for k, v in pairs(AttrAddList) do
		sData[k] = rate
	end
	return sData
end












