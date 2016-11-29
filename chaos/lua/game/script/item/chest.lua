
local chestList = {}

function GetChestList()
	return chestList
end

function GetChestInfoById(id)
	return chestList[id]
end

---------------------------------------------
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
	if obj:GetRoleType() ~= "user" then
		return false, "该道具只能由主角使用。"
	end
	local info = GetChestInfoById(self:GetId())
	if info.itemList then
		if obj:GetGridMax(USERBASE.PACKAGE) - obj:GetItemGridNum(USERBASE.PACKAGE) < #info.itemList then
			return false, "您的背包空间不足。"
		end
	end
	local msg_money = ""
	if info.gold then
		obj:SetGold(info.gold)
		msg_money = msg_money .. string.format("钱币%d", prizeList.gold)
	end
	if info.ingot then
		obj:SetVoucher(info.ingot)
		msg_money = msg_money .. string.format("元宝%d", info.ingot)
	end
	if info.nimbus then
		obj:SetNimbus(info.nimbus)
		msg_money = msg_money .. string.format("灵气%d", info.nimbus)
	end
	if info.voucher then
		obj:SetVoucher(info.voucher)
		msg_money = msg_money .. string.format("礼券%d", info.voucher)
	end
	if info.exp then
		obj:SetExp(info.exp)
		msg_money = msg_money .. string.format("经验%d", info.exp)
	end
	if #msg_money > 0 then
		obj:SendProtocol("c_show_tips", string.format("您获得了%s。", msg_money))
	end
	local msg_item = ""
	if info.itemList then
		local num = math.random(info.min, info.max)
		local list = BASEITEM.GenRandomItemByNum(info.itemList, num)
		for k, v in pairs(list) do
			local item = BASEITEM.NewItem(v.id, v.num, {lock = 1})
			obj:AddItem(USERBASE.PACKAGE, item)
			msg_item = msg_item .. string.format("%s×%d ", item:GetName(), item:GetNum())
		end
	end
	if #msg_item > 0 then
		obj:SendProtocol("c_show_tips", string.format("您获得物品%s。", msg_item))
	end
	return true
end

function __init__()
	local CHEST = Import("autocode/chestList.lua")
	chestList = CHEST.GetTable()
end
