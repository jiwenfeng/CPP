--天工炉

clsFurnace = STRUCT.clsStruct:Inherit()

function clsFurnace:__init__()
	Super(clsFurnace).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsFurnace:initsavedata()
	self.__save.Lv = 0
end

function clsFurnace:inittempdata()
end

function clsFurnace:InitStrcut(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
end
--该建筑不能升级
function clsFurnace:Upgrade()

end

function clsFurnace:GetLv()

end

function clsFurnace:CheckUpgrade(lv)
	return false, "该建筑物不可升级"
end
