--物品基类

clsBaseItem = clsObject:Inherit()

local NORMAL = 1
local EQUIP = 2

function clsBaseItem:__init__()
	Super(clsBaseItem).__init__(self)
	self.__save = {}
	self:InitSaveData()
	return true
end

function clsBaseItem:InitSaveData()
	self.__save.id = 0
	self.__save.num = 0
	self.__save.type = nil				--类型
	self.__save.missionId = nil			--任务id
	self.__save.lock = nil				--绑定属性
end


--物品id
function clsBaseItem:SetId(data)
	self.__save.id = data
end

function clsBaseItem:GetId()
	return self.__save.id
end
--物品名称
function clsBaseItem:GetName()
	return GetItemById(self.__save.id).name
end
--物品数量
function clsBaseItem:GetNum()
	return self.__save.num
end

function clsBaseItem:SetNum(data)
	data = assert(tonumber(data))
	self.__save.num = tonumber(data)
end

function clsBaseItem:ModNum(data)
	assert(tonumber(data))
	self.__save.num = self.__save.num + data
end
--物品类型
function clsBaseItem:GetType()
	return GetItemTypeById(self.__save.id)
end

function clsBaseItem:GetAreaType()
	return GetItemAreaType(self.__save.id)
end



function clsBaseItem:GetClass()
	return GetItemById(self.__save.id).class
end

function clsBaseItem:GetNeedLv()
	return GetItemById(self:GetId()).needLv
end

--检查物品是否具有任务Id
function clsBaseItem:GetMissionId()
	return self.__save.missionId
end

function clsBaseItem:SetMissionId(data)
	self.__save.missionId = data
end
--检查物品是否锁定
function clsBaseItem:SetLocked(data)
	if data == 1 then
		self.__save.lock = 1
	else
		self.__save.lock = nil
	end
end

function clsBaseItem:IsLocked()
	return self.__save.lock
end

function clsBaseItem:SetAreaId(data)
	self.__save.areaId = data
end

function clsBaseItem:GetAreaId()
	return self.__save.areaId
end

--物品使用
function clsBaseItem:Use()

end

--保存物品数据
function clsBaseItem:GetSaveData()
	self.__save.areaId = nil
	return table.deepcopy(self.__save)
end

--恢复物品数据
function clsBaseItem:Restore(data)
	assert(type(data) == "table", "Restore item failed: Invalid data type!")
	self.__save = data
end

--期限
function clsBaseItem:GetTime()
	return self.__save.time
end

function clsBaseItem:SetTime(data)
	self.__save.time = data
end
--来源
function clsBaseItem:GetFrom()
	return self.__save.from
end

function clsBaseItem:SetFrom(data)
	self.__save.from = data
end

function clsBaseItem:GetQuality()
	return GetItemById(self.__save.id).quality
end

function clsBaseItem:GetLabel()
	return GetItemById(self.__save.id).label
end

function clsBaseItem:GetKind()
	return GetItemById(self.__save.id).kind
end

function clsBaseItem:GetAreaType()
	return GetItemById(self.__save.id).areaType
end

function clsBaseItem:IsItemUsable()
	return GetItemById(self.__save.id).clickUse
end

--检查是否为同一物品
function clsBaseItem:IsSameItem(Obj)
	if self.__save.id ~= Obj:GetId() then
		return false
	elseif self.__save.missionId ~= Obj:GetMissionId() then			--任务Id
		return false
	elseif self.__save.lock ~= Obj:IsLocked() then					--绑定
		return false
--[[	elseif self.__save.time ~= Obj:GetTime() then					--期限
		return false
	elseif self.__save.from ~= Obj:GetFrom() then					--来源
		return false]]
	end
	return true
end

function clsBaseItem:InitItem(goodsId, num, missionId, lock)
	self:SetId(goodsId)
	self:SetNum(num)
	self:SetMissionId(missionId)
--	if lock then
--		self:SetLocked(1)
--	end
	self:SetLocked(lock)
end

function clsBaseItem:GetFullAttr()
	local sData = {}
	sData.goodsId = self:GetId()
	sData.usable = self:IsItemUsable() or 0
	sData.lock = self:IsLocked() or 0
	local itemInfo = GetItemById(self:GetId())
	if itemInfo.recycleGold then
		sData.goldType = "gold"
		sData.price = itemInfo.recycleGold
	end
	return sData
end

--获取物品的浮窗提示信息
function clsBaseItem:GetTipMsg()
	local sData = {}
	sData = self:GetFullAttr()
	sData.areaType = self:GetAreaType()
	if self:GetClass() == "fairyDev" then
		sData.price = nil
	end
	if self:GetClass() == "heroCard" then
		sData.goodsName = self.__save.data.name
	else
		sData.goodsName = self:GetName()
	end
	return sData
end


local HeroCardIdList = {
	[4] = 104001,
	[5] = 104007,
	[6] = 104008,
}

function GetHeroCarIdByAptLv(aptLv)
	return HeroCardIdList[aptLv] or HeroCardIdList[4]
end

----------------------------------------------------------------------------------------------
local itemList = {}				--所有的物品固定属性表
local itemClass = {				--物品类别与文件路径对应表
	["normal"] = "item/normal.lua",			--普通道具类
	["buffer"] = "item/buffer.lua",			--buffer道具类
	["gem"] = "item/gem.lua",				--宝石类
	["equip"] = "item/equip.lua",			--装备
	["petCard"] = "item/petCard.lua",		--幻形丹
	["fairyDev"] = "item/clsFairyDev.lua",	--仙器类
	["heroCard"] = "item/heroCard.lua",		--武将精魄
	["heroApt"] = "item/heroApt.lua",		--飞仙丹
	["chest"] = "item/chest.lua",			--随机数量的道具宝箱
	["package"] = "item/package.lua",		--礼包
	["petFood"] = "item/petFood.lua",
	["restore"] = "item/restore.lua",		--恢复类道具
}
--商城道具

local AllNpcShopList = {}				--npc商店列表


function GetNpcShopInfoById(id)
	return AllNpcShopList[id]
end

function GetNpcShopItemListById(npcId)
	local sData = {}
	for _, v in pairs(GetNpcShopInfoById(npcId)) do
		local info = GetItemById(v)
		if info then
			local buff = {}
			buff.goodsId = v
			buff.goldType = info.needIngot and "ingot" or "gold"
			buff.price = info.needIngot or (info.needGold or 1000)
			table.insert(sData, buff)
		end
	end
	table.sort(sData, function(a, b) 
			local order_a = MONEY.GetGoldTypeOrder(a.goldType)
			local order_b = MONEY.GetGoldTypeOrder(b.goldType)
			return order_a > order_b or (order_a == order_b and a.price < b.price)
		end)
	return sData
end


local AreaTypeList = {equip = 1, gem = 1, prop = 1, mission = 1}
local HotItemList = {}

-- 获取物品区域类型
function GetItemAreaType(goodsId)
	return itemList[goodsId].areaType
end


function GetItemAreaTypeList()
	return AreaTypeList
end

function GetAllItemList()
	return itemList
end

function GetItemListByClass(class)
	local buff = {}
	for k, v in pairs(GetAllItemList()) do
		if v.class == class then
			buff[k] = v
		end
	end
	return buff
end

-- 获取物品信息
function GetItemById(goodsId)
	return itemList[goodsId]
end

function GetItemName(goodsId)
	local info = GetItemById(goodsId)
	return info.name
end

--适合只kind唯一的情况。主要在锻造系统中
function GetItemByKind(kind)
	for k, v in pairs(itemList) do 
		if v.kind == kind then
			return k
		end
	end
end

-- 获取物品类型
function GetItemTypeById(goodsId)
	return itemList[goodsId].type
end

-- 获取物品对应文件路径
function GetItemClassById(goodsId)
	if not itemList[goodsId] then
		print("baseitem.lua: Invalid item id :", goodsId)
		return
	end
	local class = itemList[goodsId].class
	return itemClass[class]
end

--根据type取物品列表
function GetItemListByType(type, Lv, quality)
	local buff = {}
	for k, v in pairs(GetAllItemList()) do
		if v.type == type and (not Lv or v.Lv == Lv) then
			if not quality or v.quality == quality then
				table.insert(buff, k)
			end
		end
	end
	return buff
end
--根据class取物品列表
function GetItemListByKind(type, Lv, quality)
	local list = {}
	for k, v in pairs(GetAllItemList()) do
		if v.class == type and (not Lv or (v.Lv or v.needLv) == Lv) then
			if not quality or v.quality == quality then
				table.insert(list, k)
			end
		end
	end
	return list
end

--将新物品合并到list中
function CombiItemToList(list, id, num, missionId)
	if num <= 0 then return end
	for _, v in ipairs(list or {}) do
		if v.goodsId == id and v.missionId == missionId then
			v.num = v.num + num
			return
		end
	end
	table.insert(list, {goodsId = id, num = num, missionId = missionId})
end

--生成指定数量的随机物品 (全部物品共用概率)
--list = {{type, [num], Lv, rate} / {id, [num], rate}, {}, ...}
--times: generate times
--flag: Is unique
function GenRandomItemByNum(list, times, flag)
	local ret = {}
	local function add_to_list(id, num)
		for k, v in pairs(ret) do
			if v.id == id then
				v.num = v.num + num
				return
			end			
		end
		table.insert(ret, {id = id, num = num})
	end
	local temp = table.copy(list)
	for i = 1, times or 1 do
		local maxRate = 0
		for k, v in ipairs(temp) do
			maxRate = maxRate + v.rate
		end
		local ran = math.random(0, maxRate * 10000) / 10000
		local rate = 0
		for k, v in ipairs(temp) do
			rate = rate + v.rate
			if rate >= ran then
				if v.id then
					add_to_list(v.id, v.num or 1)
				elseif v.type == "piece" then
					local buff = GetItemListByType(v.type, v.Lv)
					if #buff <= 0 then
						DATA.log(string.format("GenRandomItemByNum Error on %s, %d.", v.type, v.Lv), "baseitem.log")
					end
					local id = buff[math.random(1, #buff)]
					add_to_list(id, v.num or 1)
				else
					local buff = GetItemListByKind(v.type, v.Lv, v.quality)
					if #buff <= 0 then
						DATA.log(string.format("GenRandomItemByNum Error on %s, %d, %d.", v.type, v.Lv, v.quality), "baseitem.log")
					end
					local id = buff[math.random(1, #buff)]
					add_to_list(id, v.num or 1)
				end
				if flag then
					table.remove(temp, k)
				end
				break
			end
		end
	end
	return ret
end

--生成指定概率的随机物品 (每个物品拥有自己的概率)
function GenRandomItemByRate(list)
	local ret = {}
	local function add_to_list(id, num)
		for k, v in pairs(ret) do
			if v.id == id then
				v.num = v.num + num
				return
			end			
		end
		table.insert(ret, {id = id, num = num})
	end
	for k, v in pairs(list) do
		if v.rate >= math.random() then
			if v.id then
				add_to_list(v.id, v.num or 1)
			else
				local buff = GetItemListByKind(v.type, v.Lv, v.quality)
				local id = buff[math.random(1, #buff)]
				add_to_list(id, v.num or 1)
			end
		end
	end
	return ret
end
--[[
function GetMallItemList()
	local sData = {}
	local function get_data(id, type)
		local info = GetItemById(id)
		local buff = {}
		buff.type = type
		buff.goodsId = id
		if type == 5 then
			buff.goldType = "voucher"
			buff.price = info.needVoucher
		else
			buff.goldType = info.needGold and "gold" or "ingot"
			buff.price = info.needGold or info.needIngot
		end
		return buff
	end
	for k, v in pairs(itemList) do
		if v.mall and MallTypeList[v.mall] then
			table.insert(sData, get_data(k, v.mall))
		end
		if v.needVoucher then
			table.insert(sData, get_data(k, 5))
		end
	end
	return sData
end]]


function SendRose(user, target, item, num)
	local goodsId = item:GetId()
	local info = GetItemById(goodsId)
	if not info then
		return
	end
	num = num or 1
	local tRoleId = target:GetRoleId()
	FRIEND.ModUserFavor(user, tRoleId, info.num * num)
	local sData = {}
	sData.goodsId = goodsId
	sData.num = num
--	sData.favor = info.num * num
	user:SetFuncStatById(SETTING.FUNC_SEND_ROSE, num)
	user:SetSendFlowerTimes(num)
	target:SetReceiveFlowerTimes(num)
	target:SetCharm(num)
	target:Broad2ExNeighbor("rose", nil, nil, true)
	local con = LOGIN.GetConnection(target:GetRoleId())
	if con and con.roleId == target:GetRoleId() then
		con:SendProtocol("c_rose_stunt", {type = "receive", from = user:GetName(), goodsId = goodsId})
	end
	
	local userList = {}
	if info.num == 999 then
		for k, v in pairs(USER.GetAllOnlineUser()) do
			table.insert(userList, k)
			v:SendProtocol("c_item_recvRose", sData)
		end
	elseif info.num == 99 then
		local sceneObj = target:GetScene()
		if sceneObj and sceneObj:GetType() ~= "empty" then
			for k, v in pairs(sceneObj:GetSceneRoleList()) do
				if v:GetRoleType() == "user" then
					table.insert(userList, k)
					v:SendProtocol("c_item_recvRose", sData)
				end
			end
		else
			table.insert(userList, tRoleId)
			target:SendProtocol("c_item_recvRose", sData)
		end
	else
		table.insert(userList, tRoleId)
		target:SendProtocol("c_item_recvRose", sData)
	end
	CHAT.SendSysMsg("system", string.format("%s 向 %s 赠送了%d朵[%s]。", CHAT.GetFormatedNick(user:GetName()), #userList == 1 and "您" or CHAT.GetFormatedNick(target:GetName()), num, info.name), userList, {goodsAttr = {[1] = item:GetTipMsg()}})
	user:SendProtocol("c_show_tips", string.format("您向%s赠送了%d朵【%s】。", target:GetName(), num, info.name))
end

--实例化一个物品
function NewItem(goodsId, num, param)			-- param = {missionId = , lock = , from = , time = ,}
	local classPath = GetItemClassById(goodsId)
	if not classPath then
		return
	end
	local class = Import(classPath)
	local item = class.clsItem:New()
	param = param or {}
	param.lock = GetItemById(goodsId).lock or param.lock
	item:InitItem(goodsId, num, param.missionId, param.lock, param.star, param.rate, param.save)
	item:SetTime(param.time)
	item:SetFrom(param.from)
	return item
end

function SendMailDonee(roleId, item, from)
	local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
	mail.title = "物品赠送通知"
	mail.content = string.format("亲，您的朋友%s给你赠送了物品<font color = \"#FFFF00\">%s</font> ×%d，快来领取吧！", from, item:GetName(), item:GetNum())
	mail.attach = {item = {}}
	table.insert(mail.attach.item, {id = item:GetId(), num = item:GetNum(), save = item:GetSaveData()})
	MAIL.SendSysMail(roleId, mail)
end

function BuyItemFromSys(user, goodsId, num, goldType)
	local info = GetItemById(goodsId)
	if not info then
		return false, "无效的物品信息"
	end
	local goldInfo = MONEY.GetGoldInfoByType(goldType)
	if not goldInfo then
		return false, string.format("无效的货币类型:%s。", goldType or "")
	end
	if not info[goldInfo.field] then
		return false, string.format("该物品不能使用%s购买。", goldInfo.name)
	end
	local price = info[goldInfo.field] * num
	if USERBASE.clsUser[goldInfo.get](user) < price then
		return false, string.format("您的%s余额不足。", goldInfo.name)
	end
	local item = NewItem(goodsId, num, {lock = info.lock})
	local ret, left = user:IsItemCanAdd(USERBASE.PACKAGE, item)
	if not ret or left < num then
		return false, "您的背包空间不足，请整理背包或者解锁更多的位置。"
	end
	user:AddItem(USERBASE.PACKAGE, item)
	USERBASE.clsUser[goldInfo.set](user, -price)
	DATA.log(string.format("%s成功购得%d个%s，共花费%d个%s。", user:GetName(), num, item:GetName(), price, goldInfo.name), "baseItem.log")
	return true, rate
end

--获取物品列表
local s_item_NPCItemList = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sceneObj = user:GetScene()
	if not sceneObj then
		return ERROR.TipMsg(connection, "您当前不在城镇中，不能购买物品。")
	end
	data = tostring(data)
	local npcObj = sceneObj:GetSceneRole(data)
	if not npcObj then
		return ERROR.TipMsg(connection, "没有指定的npc信息。")
	end
	if not GetNpcShopInfoById(npcObj:GetId()) then
		return ERROR.TipMsg(connection, "该npc不是商店npc。")
	end
	local sData = {roleId = data}
	sData.list = GetNpcShopItemListById(npcObj:GetId())
	return connection:SendProtocol("c_item_NPCItemList", sData)
end

--获取物品列表
local s_item_MallItemList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local sData = GetMallItemList()
	return connection:SendProtocol("c_item_MallItemList", sData)
end

--购买
local s_item_buy = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "无效的购买数量")
	end
	local ret, msg = BuyItemFromSys(user, data.goodsId, data.num, data.goldType)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	return connection:SendProtocol("c_item_buy", msg or 0)
end

local s_item_buyBack = function(connection, data)
	if type(data) ~= "table" or not data.areaId or not data.num then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "无效的购买数量")
	end
	local item = user:GetItemByAreaId(USERBASE.BUYBACK, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "指定的位置上没有可以回购的物品。")
	end
	if data.num > item:GetNum() then
		return ERROR.TipMsg(connection, "数量不足，无法购买")
	end
	local price = GetItemById(item:GetId()).recycleGold * data.num
	if user:GetGold() < price then
		return ERROR.TipMsg(connection, "您的钱币数量不足。")
	end
	local newItem = table.deepcopy(item)
	newItem:SetNum(data.num)
	user:AddItem(USERBASE.PACKAGE, newItem)
	user:DelItemByAreaId(USERBASE.BUYBACK, data.areaId, data.num)
	user:SetGold(-price)
	return connection:SendProtocol("c_item_buyBack", "true")
end


--赠送玫瑰
local s_item_sendRose = function(connection, data)
	if type(data) ~= "table" or not data.areaId then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local item = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "没有找到指定位置的物品。")
	end
	if item:GetType() ~= "rose" then
		return ERROR.TipMsg(connection, "该物品不是玫瑰花。")
	end
	local rRoleId = USER.GetRoleIdByNick(data.nick)
	if not rRoleId then
		return ERROR.TipMsg(connection, "该用户不存在")
	end
	local target = USER.GetOnlineUser(rRoleId)
	if not target then
		return ERROR.TipMsg(connection, "该用户不在线")
	end
	if rRoleId == connection.roleId then
		return ERROR.TipMsg(connection, "不能给自己赠送玫瑰花")
	end
	SendRose(user, target, item, data.num or 1)
	user:DelItemByAreaId(USERBASE.PACKAGE, data.areaId, data.num or 1)
	connection:SendProtocol("c_item_sendRose", "true")
end


local ItemTargetList = {[1] = "主将", [2] = "战魂", [3] = "本命精灵"}
--使用物品
local s_item_use = function(connection, data)
	if type(data) ~= "table" or (not data.areaId and not data.goodsId) then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.stoType = data.stoType or USERBASE.PACKAGE
	if data.stoType ~= USERBASE.PACKAGE then
		return ERROR.TipMsg(connection, "只有背包中的物品才能直接使用")
	end
	data.num = data.num or 1
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local item, itemInfo
	if data.areaId then
		item = user:GetItemByAreaId(data.stoType, data.areaId)
		if not item then
			return ERROR.TipMsg(connection, "没有找到指定位置的物品。")
		elseif item:GetNum() < data.num then
			return ERROR.TipMsg(connection, "物品数量不足")
		end
		data.goodsId = item:GetId()
		itemInfo = GetItemById(item:GetId())
	else
		itemInfo = GetItemById(data.goodsId)
		if not itemInfo then
			return ERROR.TipMsg(connection, string.format("无效的物品id：%s.", data.goodsId))
		end
		local num = user:GetItemNum(data.stoType, data.goodsId)
		if num < data.num then
			if not data.autoBuy then
				return ERROR.TipMsg(connection, "您背包中的【%s】数量不足。", itemInfo.name)
			elseif user:GetTotalIngot() < itemInfo.needIngot * (data.num - num) then
				return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
			end
		end
	end
	if itemInfo.clickUse ~= 1 then
		return ERROR.TipMsg(connection, "该物品不能直接使用")
	end
	if itemInfo.needLv > user:GetLv() then
		return ERROR.TipMsg(connection, "您的等级不足，不能使用该物品")
	end
	local obj
	if itemInfo.target == 4 then
		if data.target == 1 then
			obj = user
		else
			obj = user:GetHero(tostring(data.target))
			if not obj then
				return ERROR.TipMsg(connection, string.format("没有指定的战魂信息：%s。", data.target or ""))
			end
		end
	else
		if itemInfo.target ~= 2 and data.target ~= itemInfo.target then
			return ERROR.TipMsg(connection, string.format("只有%s才能使用【%s】。", ItemTargetList[itemInfo.target], itemInfo.name))
		elseif itemInfo.target == 2 and ItemTargetList[data.target] then
			return ERROR.TipMsg(connection, string.format("%s不能使用【%s】。", ItemTargetList[data.target], itemInfo.name))
		end
		if itemInfo.target == 1 then		
			obj = user
		elseif itemInfo.target == 3 then
			obj = user:GetPet()
			if not obj then
				return ERROR.TipMsg(connection, "您还没有本命精灵，不能使用该物品。")
			end
		else	
			obj = user:GetHero(tostring(data.target))
			if not obj then
				return ERROR.TipMsg(connection, string.format("没有指定的战魂信息：%s。", data.target or ""))
			end
		end
	end
	local newItem = item or NewItem(data.goodsId, data.num)
	local ret, msg = newItem:Use(obj, data.num)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	if data.areaId then
		user:DelItemByAreaId(data.stoType, data.areaId, data.num)
	else
		local num = user:GetItemNum(data.stoType, data.goodsId)
		user:DelItem(data.stoType, data.goodsId, math.min(data.num, num))
		if num < data.num then
			user:SetTotalIngot(-itemInfo.needIngot * (data.num - num))
		end
	end
	return connection:SendProtocol("c_item_use", {goodsId = newItem:GetId(), msg = msg})
end

local s_item_abandon = function(connection, data)
	if type(data) ~= "table" or not data.stoType or not data.areaId or not data.num then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	if data.stoType ~= USERBASE.PACKAGE then
		return ERROR.TipMsg(connection, "只有背包中的物品才能丢弃")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "无效的物品数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local item = user:GetItemByAreaId(data.stoType, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "无效的位置")
	end
	if item:GetNum() < data.num then
		return ERROR.TipMsg(connection, "物品数量不足。")
	end
	if item:GetClass() == "heroCard" then
		return ERROR.TipMsg(connection, "武将精魄不能丢弃。")
	end
	if item:GetMissionId() then
		return ERROR.TipMsg(connection, "任务物品不能丢弃")
	end
	user:DelItemByAreaId(data.stoType, data.areaId, data.num)
	return connection:SendProtocol("c_item_abandon", "true")
end

local s_item_userItemList =  function(connection, data)
	if data ~= USERBASE.DEPOT and data ~= USERBASE.PACKAGE and data ~= USERBASE.BUYBACK then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData
	if data ~= USERBASE.BUYBACK then
		sData = user:GetItemList(data)
	else
		sData = user:GetBuybackList()
	end
	table.sort(sData.list, function(a, b) return tonumber(a.areaId) < tonumber(b.areaId) end)
	connection:SendProtocol("c_item_userItemList", sData)
end

local s_item_sell = function(connection, data)
	if type(data) ~= "table" or not data.areaId or not data.num then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "无效的物品数量。")
	end	
	local ret, msg = user:SellItem(USERBASE.PACKAGE, data.areaId, data.num)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_item_sell", {goodsId = msg, price = ret, num = data.num})
end

local s_item_put = function(connection, data)
	if type(data) ~= "table" or not data.stoType or not data.areaId then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local item = user:GetItemByAreaId(data.stoType, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "没有指定的物品信息。")
	end
	local type = data.stoType == USERBASE.DEPOT and USERBASE.PACKAGE or USERBASE.DEPOT
	if not user:IsItemCanAdd(type, item) then
		return ERROR.TipMsg(connection, string.format("您的%s已满，请整理之后再进行操作。", type == USERBASE.DEPOT and "仓库" or "背包"))
	end
	user:AddItem(type, table.deepcopy(item), true)
	user:DelItemByAreaId(data.stoType, data.areaId, item:GetNum(), true)
	connection:SendProtocol("c_item_put", "true")
end

local s_item_sort = function(connection, data) 
	if data ~= USERBASE.DEPOT and data ~= USERBASE.PACKAGE then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:SortItem(data)
	local sData = user:GetItemList(data)
	connection:SendProtocol("c_item_sort", sData)
end

local s_item_switch = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if data.stoType ~= USERBASE.DEPOT and data.stoType ~= USERBASE.PACKAGE then
		return ERROR.TipMsg(connection, "请指定要交换位置的物品存放在背包还是仓库。")
	end
	if not data.from or not data.to then
		return ERROR.TipMsg(connection, "请选择要交换的位置id。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetItemByAreaId(data.stoType, data.from) then
		return ERROR.TipMsg(connection, "没有找到指定位置的物品。")
	end
	if tonumber(data.to) <= 0 or tonumber(data.to) > user:GetGridMax(data.stoType) then
		return connection:SendProtocol("c_item_switch", "false")
	end
	user:SwitchItem(data.stoType, data.from, data.to)
	connection:SendProtocol("c_item_switch", "true")
end


local s_rose_stunt = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_rose_stunt", {type = "feedback", from = USER.GetUserNickById(connection.roleId)})
	end
end
 
function __init__() 
	local function LoadTable(tbl)
		for k, v in pairs(tbl) do
			itemList[k] = v
		end
	end
	--加载普通类道具
	ITEMNORMAL = Import("autocode/itemList.lua")
	local datatbl = ITEMNORMAL.GetTable()
	LoadTable(datatbl)

	--加载宝石类道具
	GEM = Import("autocode/gemList.lua")
	datatbl = GEM.GetTable()
	LoadTable(datatbl)
	--装备
	local EQUIP = Import("autocode/itemEquip.lua")
	datatbl = EQUIP.GetTable()
	LoadTable(datatbl)
	--仙器
	local FAIRYDEV = Import("autocode/fairyDeviceList.lua")
	datatbl = FAIRYDEV.GetTable()
	LoadTable(datatbl)
	
	local NPCSHOP = Import("autocode/npcShopList.lua")
	AllNpcShopList = NPCSHOP.GetTable()
	

	PROTOCOL.s_item_NPCItemList = s_item_NPCItemList
--	PROTOCOL.s_item_MallItemList = s_item_MallItemList
	PROTOCOL.s_item_buy = s_item_buy
	
	PROTOCOL.s_item_use = s_item_use
	PROTOCOL.s_item_userItemList = s_item_userItemList
	PROTOCOL.s_item_sell = s_item_sell
	PROTOCOL.s_item_buyBack = s_item_buyBack
	
	PROTOCOL.s_item_sort = s_item_sort
	PROTOCOL.s_item_switch = s_item_switch
	PROTOCOL.s_item_put = s_item_put
	PROTOCOL.s_item_abandon = s_item_abandon
	PROTOCOL.s_item_sendRose = s_item_sendRose
	PROTOCOL.s_rose_stunt = s_rose_stunt
end

--数据文件，用于给客户端使用，数据有更新的时候运行一次，不需要每次运行
--存储unit/items
function SaveItemList()
	local sData = {}
	local f = io.open("item/act.txt", "w")
	for k, v in pairs(itemList) do
		local buff = {goodsName = v.name, goodsId = k, describe = v.describe or "", explain = v.explain or ""}
		buff.quality = v.quality or 1
		buff.areaType = v.areaType
		buff.type = v.type
		buff.kind = v.kind
		buff.needLv = v.needLv or 1
		if v.recycleGold then
			buff.goldType = "gold"
			buff.price = v.recycleGold
		end
		if v.class == "equip" then			
			buff.target = v.target
			buff.star = 0
			buff.baseAttr = v.attr
			buff.wearPos = v.wearPos
			buff.needJob = v.needJob
			buff.complex = v.complex
		elseif v.class == "gem" then
			buff.attr = v.attr
		end
		table.insert(sData, buff)
	end
	for k in pairs(SUIT.GetAllSuitList()) do		--套装
		table.insert(sData, SUIT.GetFormatedSuitDesc(k))
	end
	for k, v in pairs(TITLE.GetAllTitleInfoList()) do
		table.insert(sData, {goodsName = v.name, goodsId = k, quality = v.quality or 1, describe = v.describe})
	end
	for k, v in pairs(BASESKILL.GetAllSkillList()) do
		table.insert(sData, {goodsName = v.name, goodsId = k, quality = v.quality or 1, type = v.type})
	end
	for k, v in pairs(FORMATION.GetAllFormationList()) do
		table.insert(sData, {name = v.name, goodsId = k, needLv = v.needLv})
	end
	f:write(JSON.encode(sData))
	f:close()
end
