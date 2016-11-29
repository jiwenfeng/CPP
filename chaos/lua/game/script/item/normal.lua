local NormalList = BASEITEM.GetItemListByClass("normal")

function GetNormalList()
	return NormalList
end

function GetNormalListById(id)
	return NormalList[id]
end

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
	return false, "该物品不能直接使用。"
end