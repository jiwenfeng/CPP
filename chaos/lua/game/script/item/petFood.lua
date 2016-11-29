--天元果/地元果

local PetFoodList = BASEITEM.GetItemListByClass("petFood")

function GetPetCardList()
	return PetFoodList
end

function GetPetCardById(id)
	return PetFoodList[id]
end

---------------------------------------------------
clsItem = BASEITEM.clsBaseItem:Inherit()

function clsItem:__init__()
	Super(clsItem).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsItem:initsavedata()
	self.__save.id = 0
	self.__save.num = 0
end

function clsItem:inittempdata()

end

--使用物品
function clsItem:Use(obj, num)
	return obj:Feed(self:GetId())
end
