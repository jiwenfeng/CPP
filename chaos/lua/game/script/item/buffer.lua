
local BufferList = BASEITEM.GetItemListByClass("buffer")

function GetBufferList()
	return BufferList
end

function GetBufferInfo(id)
	return BufferList[id]
end
-------------------------------------------------------

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
	num = num or 1
	if num > 1 then
		return false, "该类道具每次只能使用1个。"
	end
	local info = GetBufferInfo(self:GetId())
	if info.funcId ~= 1 then
		return false, "该物品不属于buffer类道具。"
	end
	BUFFER.AddOneBuffer(obj, info.buffId, info.num)
	return true
end
