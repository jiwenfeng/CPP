--炼狱神坛

clsGodAltar = STRUCT.clsStruct:Inherit()

function clsGodAltar:__init__()
	Super(clsGodAltar).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsGodAltar:initsavedata()
	self.__save.Lv = 0
end

function clsGodAltar:inittempdata()
end

function clsGodAltar:InitStrcut(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
end
--该建筑不能升级
function clsGodAltar:Upgrade()
end

function clsGodAltar:GetLv()
end

function clsGodAltar:CheckUpgrade(lv)
	return false, "该建筑物不可升级"
end
