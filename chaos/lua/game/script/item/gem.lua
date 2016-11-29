--宝石类 (gem)
--特点：该类物品具备物品的一般属性，不可直接使用，但同时具备一些属性值

local GemList = BASEITEM.GetItemListByClass("gem")

function GetGemList()
	return GemList
end

function GetGemInfo(id)
	return GemList[id]
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

function clsItem:InitItem(id, num, missionId, lock)
	Super(clsItem).InitItem(self, id, num, missionId, lock)		--调用父类函数
	local info = GetGemInfo(id)
	self:SetLv(info.Lv)
end

function clsItem:SetLv(data)
	data = assert(tonumber(data))
	self.__save.Lv = data
end

function clsItem:GetLv(data)
	return self.__save.Lv or 1
end

--宝石属性
function clsItem:GetAttr()
	return GetGemInfo(self:GetId()).attr
end

--获取宝石完整属性
function clsItem:GetFullAttr()
	local sData = {}
	sData.goodsId = self:GetId()
	sData.lock = self:IsLocked() or 0
	local gemInfo = GetGemInfo(self:GetId())
	if gemInfo.recycleGold then
		sData.goldType = "gold"
		sData.price = gemInfo.recycleGold
	end
	return sData
end

--宝石镶嵌概率
function clsItem:GetInlayRate()
	return GetGemInfo(self:GetId()).enchaseRate
end

--宝石摘除概率
function clsItem:GetRemoveRate()
	return GetGemInfo(self:GetId()).pickRate
end

--[[function clsItem:IsValidEquip(equipId)
	local equipInfo = BASEITEM.GetItemById(equipId)
	if not equipInfo then
		return false	
	end
	local wearpos = equipInfo.wearPos
	local gemInfo = GetGemInfo(self:GetId())
	for k, v in pairs(gemInfo.enchsList) do
		if v == wearpos then
			return true
		end
	end
	return false
end]]

--使用物品
function clsItem:Use(obj, num)
	return false, "该物品不能直接使用"
end

