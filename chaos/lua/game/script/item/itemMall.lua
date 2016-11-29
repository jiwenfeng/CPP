--商城

local AllMallItemList = {}
local MallTypeList = {[1] = "home", [2] = "prop", [3] = "forge", [4] = "hero", [5] = "voucher", [6] = "hot"}

function GetAllMallItemList()
	return AllMallItemList
end

function GetMallItemByType(type)
	return AllMallItemList[type]
end

function GetMallItemInfoById(type, id)
	local list = GetMallItemByType(type)
	for k, v in ipairs(list) do
		if v.id == id then
			return v
		end
	end
end

----------------------------------------------------------
local HOT_ITEM_NUM = 4			--每次刷出的促销物品数量
local HOTITEM_FRESH_HOUR = 0	--促销物品刷新
local HOTITEM_FRESH_MIN = 0
local HOT_ITEM_TYPE = 6

local CurHotItemList = {}
local path_hotItem = "chaos.itemHot"

function GetCurHotItemList()
	return CurHotItemList
end


function RestoreItemHotList()
	local function call_back(Data)
		Data = UnSerialize(Data)
		for _, v in pairs(Data) do
			if v._id == "hotItem" then
				CurHotItemList = v
			end
		end
		RunFreshHotItemCallOut()
	end
	DATA.Read(path_hotItem, {}, call_back)	
end

function RefreshCurHotItem()
	local timelong = CalDiffSeconds(HOTITEM_FRESH_HOUR, HOTITEM_FRESH_MIN)
	local hotList = GetCurHotItemList()
	hotList.list = {}
	hotList.timestamp = os.time() + timelong	
	local buff = {}
	for index in ipairs(GetMallItemByType(HOT_ITEM_TYPE)) do
		table.insert(buff, index)		--这里是热卖品的顺序索引
	end
	if #buff > 0 then
		for i = 1, HOT_ITEM_NUM do
			local ran = math.random(1, #buff)
			table.insert(hotList.list, buff[ran])
			table.remove(buff, ran)
		end
	end
	SaveHotItemList()
--	RunFreshHotItemCallOut()
	return list
end

function SaveHotItemList()
	local hotList = GetCurHotItemList()
	local buff = {}
	buff.list = hotList.list
	buff.timestamp = hotList.timestamp
	buff._id = "hotItem"
	DATA.UpdateData(path_hotItem, {_id = "hotItem"}, buff)
end

function RunFreshHotItemCallOut()
	local hotList = GetCurHotItemList()
	if not hotList.timestamp or hotList.timestamp <= os.time() then
		return RefreshCurHotItem()
	end
--[[	local timelong = hotList.timestamp - os.time()
	CALLOUT.CallOut(RunFreshHotItemCallOut, timelong)]]
end


function GetFormatedHotItemList()
	local hotList = GetCurHotItemList()
	local itemList = GetMallItemByType(HOT_ITEM_TYPE)
	local sData = {list = {}}
	sData.timelong = hotList.timestamp - os.time()
	for _, v in ipairs(hotList.list or {}) do
		if itemList[v] then
			local buff = {}
			buff.type = HOT_ITEM_TYPE
			buff.goodsId = itemList[v].id
			buff.goldType = itemList[v].goldType
			buff.price = itemList[v].price
			buff.oldPrice = itemList[v].oldPrice
			table.insert(sData.list, buff)
		end
	end
	return sData
end
---------------------------------------------------------------


function GetMallItemList()
	local sData = {}
	for type, list in ipairs(GetAllMallItemList()) do
		if type ~= HOT_ITEM_TYPE then
			for _, v in ipairs(list) do
				local buff = {}
				buff.type = type
				buff.goodsId = v.id
				buff.goldType = v.goldType
				buff.price = v.price
				table.insert(sData, buff)
			end
		end
	end
	return sData
end

function BuyMallItem(user, type, goodsId, num)
	local itemInfo = BASEITEM.GetItemById(goodsId)
	if not itemInfo then
		return false, "没有指定的物品信息。"
	end
	local info = GetMallItemInfoById(type, goodsId)
	if not info then
		return false, "商城中没有出售该道具。"
	end
	local price = info.price
	local goldInfo = MONEY.GetGoldInfoByType(info.goldType)
	if USERBASE.clsUser[goldInfo.get](user) < price then
		return false, string.format("您的%s余额不足。", goldInfo.name)
	end
	local newItem = BASEITEM.NewItem(goodsId, num, {lock = itemInfo.lock})
	local ret, left = user:IsItemCanAdd(USERBASE.PACKAGE, newItem)
	if not ret or left < num then
		return false, "您的背包空间不足，请整理背包或者解锁更多的位置。"
	end
	user:AddItem(USERBASE.PACKAGE, newItem)
	
	--本名精灵意外之喜
	local pet = user:GetPet()
	local rate = pet:GetPetDiscount()
	if rate then
		price = math.ceil(price * rate)
		user:SendProtocol("c_show_tips", string.format("本命精灵通过与售货员交涉，您本次购物获得%g折优惠。", rate * 10))
	else
		local extraItem = pet:GetRandomItem()
		if extraItem then
			local item = BASEITEM.NewItem(extraItem.id, extraItem.num, {lock = 1})
			user:AddItem(USERBASE.PACKAGE, item, true)
			user:SendProtocol("c_show_tips", string.format("你的本命精灵受到了售货员的亲睐，额外获赠了%d个【%s】。",  extraItem.num, item:GetName()))
		end
	end
	USERBASE.clsUser[goldInfo.set](user, -price)
	MISSION.CheckMissionEvent(user, "func", SETTING.FUNC_MALL, 1)
	user:SetFuncStatById(SETTING.FUNC_MALL, 1)
	DATA.log(string.format("%s成功购得%d个%s，共花费%d个%s。", user:GetName(), num, newItem:GetName(), price, goldInfo.name), "itemMall.log")
	return true
end



--获取物品列表
local s_mall_itemList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local sData = GetMallItemList()
	return connection:SendProtocol("c_mall_itemList", sData)
end

--购买商城道具  data.goodsId, data.num, data.type
local s_mall_buy = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	data.type = tonumber(data.type)
	if not GetMallItemByType(data.type) then
		return ERROR.TipMsg(connection, "协议错误，请选择该道具所在的分类。")
	end
	data.num = tonumber(data.num)
	if not data.goodsId or not data.num then
		return ERROR.TipMsg(connection, "请选择要购买的道具及数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = BuyMallItem(user, data.type, data.goodsId, data.num)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_mall_buy", data)
end

local s_item_hotList = function(conn, data)
	if data ~= "get" then
		return ERROR.ErrorMsg(conn, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatedHotItemList()
	conn:SendProtocol("c_item_hotList", sData)
end

function __init__()
	local ITEMMALL = Import("autocode/itemMall.lua")
	AllMallItemList = ITEMMALL.GetTable()
	
	RestoreItemHotList()
	CALLOUT.Daily(getfenv(2), HOTITEM_FRESH_HOUR, HOTITEM_FRESH_MIN, RefreshCurHotItem)
	
	PROTOCOL.s_mall_itemList = s_mall_itemList
	PROTOCOL.s_mall_buy = s_mall_buy
	PROTOCOL.s_item_hotList = s_item_hotList
end

