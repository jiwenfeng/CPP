--本命精灵患形卡

local PetCardList = BASEITEM.GetItemListByClass("petCard")

function GetPetCardList()
	return PetCardList
end

function GetPetCardById(id)
	return PetCardList[id]
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
	if num ~= 1 then
		return false, "元灵丹每次只能使用1个。"
	end
	local cardInfo = GetPetCardById(self:GetId())
	local user = USER.GetOnlineUser(obj:GetOwner())
	if user:GetLv() < cardInfo.needLv then
		return false, "等级未达到"
	end
	if obj:GetForm() == 0 then
		return false, "未孵化之前不能使用元灵丹。"
	end
	obj:Transform(cardInfo.num)
	user:SetPetTransformInfo(cardInfo.num)
	return true
end
