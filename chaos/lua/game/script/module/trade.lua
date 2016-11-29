--玩家交易系统

local TradeUserType = {source = 1, target = 1}
local tradeIndex = 0
local AllTradeObjList = {}

function GetTradeObjById(id)
	return AllTradeObjList[id]
end

function GetNextTradeId()
	tradeIndex = tradeIndex + 1
	return tradeIndex
end

function GetAnotherUserType(type)
	return type == "source" and "target" or "source"
end
--------------------------------------------
clsTrade = clsObject:Inherit()

function clsTrade:__init__()
	Super(clsTrade).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsTrade:initsavedata()
	self.__save.id = nil
	self.__save.source = {roleId = nil, list = {}}
	self.__save.target = {roleId = nil, list = {}}
end

function clsTrade:inittempdata()
	self.__temp.gold = {}
	self.__temp.ingot = {}
	self.__temp.itemList = {}
end

function clsTrade:InitTrade(id, source, target)
	self:SetId(id)
	self:SetTradeUser("source", source)
	self:SetTradeUser("target", target)
end

function clsTrade:SetId(data)
	self.__save.id = data
end

function clsTrade:GetId()
	return self.__save.id
end
--交易人
function clsTrade:SetTradeUser(type, userObj)
	self.__save[type].user = userObj
end

function clsTrade:GetTradeUser(type)
	return self.__save[type].user
end
--获取交易的对方
function clsTrade:GetTargetUser(meObj)
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		if user:GetRoleId() ~= meObj:GetRoleId() then
			return user
		end
	end
end
--交易清单
function clsTrade:GetTradeList(type)
	return self.__save[type].list
end
--钱币
function clsTrade:GetTradeGold(type)
	return self.__save[type].list.gold or 0
end

function clsTrade:SetTradeGold(type, gold)
	self.__save[type].list.gold = gold
end
--元宝
function clsTrade:GetTradeIngot(type)
	return self.__save[type].list.ingot or 0
end

function clsTrade:SetTradeIngot(type, ingot)
	self.__save[type].list.ingot = ingot
end
--增删物品
function clsTrade:AddOneItem(type, areaId, num)
	self.__save[type].list.item = self.__save[type].list.item or {}
	table.insert(self.__save[type].list.item, {areaId = areaId, num = num})
end

function clsTrade:DelOneItem(type, index)
	self.__save[type].list.item = self.__save[type].list.item or {}
	table.remove(self.__save[type].list.item, index)
end

function clsTrade:GetTradeItemList()
	self.__save[type].list.item = self.__save[type].list.item or {}
	return self.__save[type].list.item
end
--交易确认
function clsTrade:SetTradeConfirm(type, data)
	self.__save[type].confirm = data
end

function clsTrade:IsTradeConfirm(type)
	return self.__save[type].confirm
end
--交易锁定
function clsTrade:SetTradeLock(type, data)
	self.__save[type].lock = data
end

function clsTrade:IsTradeLocked(type)
	return self.__save[type].lock
end

function clsTrade:GetUserType(roleId)
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		if user:GetRoleId() == roleId then
			return k
		end
	end
end


--金币统计
function clsTrade:SetTempGold(type, data)
	self.__temp.gold[type] = (self.__temp.gold[type] or 0) + data
end

function clsTrade:GetTempGold(type)
	return self.__temp.gold[type]
end
--元宝统计
function clsTrade:SetTempIngot(type, data)
	self.__temp.ingot[type] = (self.__temp.ingot[type] or 0) + data
end

function clsTrade:GetTempIngot(type)
	return self.__temp.ingot[type]
end
--物品统计
function clsTrade:SetTempItemList(type, item)
	self.__temp.itemList[type] = self.__temp.itemList[type] or {}
	table.insert(self.__temp.itemList[type], item)
end

function clsTrade:GetTempItemList(type)
	return self.__temp.itemList[type]
end

--发送交易开始
function clsTrade:SendTradeBegin()
	local sData = {tradeId = self:GetId(), list = {}}
	for k in pairs(TradeUserType) do			
		local buff = {}
		local user = self:GetTradeUser(k)
		buff.nick = user:GetName()
		buff.Lv = user:GetLv()
		table.insert(sData.list, buff)
	end
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local con = LOGIN.GetConnection(user:GetRoleId())
		if con and con.roleId == user:GetRoleId() then
			con:SendProtocol("c_trade_begin", sData)
		end
	end
end

function clsTrade:SendMoneyList(roleId)
	local sData = {tradeId = self:GetId(), list = {}}
	for k in pairs(TradeUserType) do			
		local buff = {}
		local user = self:GetTradeUser(k)
		buff.nick = user:GetName()
		buff.gold = self:GetTradeGold(k)
		buff.ingot = self:GetTradeIngot(k)
		table.insert(sData.list, buff)
	end
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		if not roleId or roleId == user:GetRoleId() then
			local con = LOGIN.GetConnection(user:GetRoleId())
			if con and con.roleId == user:GetRoleId() then
				con:SendProtocol("c_trade_moneyList", sData)
			end
		end
	end
end

function clsTrade:SendItemList(roleId)
	local sData = {tradeId = self:GetId(), list = {}}
	for k in pairs(TradeUserType) do
		local buff = {}
		local user = self:GetTradeUser(k)
		buff.nick = user:GetName()
		buff.itemList = {}
		for k, v in ipairs(self:GetTradeItemList()) do
			local item = user:GetItemByAreaId(USERBASE.PACKAGE, v.areaId)
			if item then
				local temp = user:GetItemAttr(USERBASE.PACKAGE, v.areaId, true)
				temp.num = v.num
				temp.index = k
				table.insert(buff.itemList, temp)
			end
		end
		table.insert(sData.list, buff)
	end
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		if not roleId or roleId == user:GetRoleId() then
			local con = LOGIN.GetConnection(user:GetRoleId())
			if con and con.roleId == user:GetRoleId() then
				con:SendProtocol("c_trade_itemList", sData)
			end
		end
	end
end

--发送交易结束
function clsTrade:SendTradeOver()
	local sData = {tradeId = self:GetId(), msg = "交易完成。"}
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local con = LOGIN.GetConnection(user:GetRoleId())
		if con and con.roleId == user:GetRoleId() then
			con:SendProtocol("c_trade_over", sData)
		end
	end
end

function clsTrade:SendTradeLockState()
	local sData = {tradeId = self:GetId(), list = {}}
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local buff = {}
		buff.nick = user:GetName()
		buff.state = self:IsTradeLocked(k) and 1 or 0
	end
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local con = LOGIN.GetConnection(user:GetRoleId())
		if con and con.roleId == user:GetRoleId() then
			con:SendProtocol("c_trade_state", sData)
		end
	end
end

--发送交易取消
function clsTrade:SendTradeCancel(operator)
	local sData = {tradeId = self:GetId()}
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local roleId = user:GetRoleId()
		if roleId == operator then
			sData.msg = "您取消了交易。"
		else
			sData.msg = "对方取消了交易。"
		end
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_trade_over", sData)
		end
	end
end


function clsTrade:ProTrade()
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local list = self:GetTradeList(k)
		local type = GetAnotherUserType(k)
		if list.gold then
			user:SetGold(-list.gold)
			self:SetTempGold(type, list.gold)
		end
		if list.ingot then
			user:SetIngot(-list.ingot)
			self:SetTempIngot(type, list.ingot)
		end
		for _, v in ipairs(list.item or {}) do
			local item = user:GetItemByAreaId(USERBASE.PACKAGE, v)
			if item then
				self:SetTempItemList(type, item)
				user:DelItemByAreaId(v)
			end
		end
	end
	for k in pairs(TradeUserType) do
		local user = self:GetTradeUser(k)
		local gold = self:GetTempGold(k)
		if gold then
			user:SetGold(gold)
		end
		local ingot = self:GetTempIngot(k)
		if ingot then
			user:SetIngot(ingot)
		end
		local itemList = self:GetTempItemList(k)
		if itemList then
			for _, v in ipairs(itemList) do
				user:AddItem(USERBASE.PACKAGE, v)
			end
		end
	end	
end


function clsTrade:TradeCancel(roleId)
	self:SendTradeCancel(roleId)
end

function clsTrade:TradeOver()
	self:ProTrade()
	self:SendTradeOver()
end

-------------------------------------
--发送交易请求
function SendTradeRequest(source, target)
	local con = LOGIN.GetConnection(target)
	if con and con.roleId == target then
		con:SendProtocol("c_trade_ask", USER.GetUserNickById(source))
	end
end
--交易建立
function BeginTrade(user, target)
	local id = GetNextTradeId()
	local trade = clsTrade:New()
	trade:InitTrade(id, user, target)
	trade:SendTradeBegin()
	AllTradeObjList[id] = trade	
	user:SetTrade(trade)
	target:SetTrade(trade)
end
--交易取消
function CancelTrade(user)
	local trade = user:GetTrade()
	if not trade then
		return
	end
	trade:TradeCancel(user:GetRoleId())
	user:SetTrade()
	local target = trade:GetTargetUser(user)
	target:SetTrade()
	trade:Destroy()
	AllTradeObjList[trade:GetId()] = nil
end

function FinishTrade(tradeId)
	local trade = GetTradeObjById(tradeId)
	if not trade then
		return
	end
	trade:TradeOver()
	for k in pairs(TradeUserType) do
		local user = trade:GetTradeUser(k)
		user:SetTrade()
	end
	trade:Destroy()
	AllTradeObjList[tradeId] = nil
end


--发起交易
local s_trade_request = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有该玩家信息。")
	end
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return ERROR.TipMsg(connection, "指定的玩家已经离线。")
	end
	if user:GetTrade() then
		return ERROR.TipMsg(connection, "该玩家正在进行交易。")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "该玩家正在战斗中，不能进行交易。")
	end
	SendTradeRequest(connection.roleId, roleId)
	connection:SendProtocol("c_trade_request", data)
end
--建立交易
local s_trade_create = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有该玩家信息。")
	end
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return ERROR.TipMsg(connection, "指定的玩家已经离线。")
	end
	if user:GetTrade() then
		return ERROR.TipMsg(connection, "该玩家正在进行交易。")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "该玩家正在战斗中，不能进行交易。")
	end
	local me = USER.GetOnlineUser(connection.roleId)
	if me:GetTrade() then
		return ERROR.TipMsg(connection, "您已经在进行交易了。")
	end
	if me:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "战斗中不能进行交易。")
	end
	return BeginTrade(user, me)
end
--查看交易资金
local s_trade_moneyList = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	return trade:SendMoneyList(connection.roleId)
end
--查看交易物品
local s_trade_itemList = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	return trade:SendItemList(connection.roleId)
end
--修改交易资金
local s_trade_setMoney = function(connection, data)
	if not type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	data.gold = tonumber(data.gold)
	if data.gold and data.gold <= 0 then
		return ERROR.TipMsg(connection, string.format("输入的%s数量有误。", MONEY.GetGoldTypeName("gold")))
	end
	data.ingot = tonumber(data.ingot)
	if data.ingot and data.ingot <= 0 then
		return ERROR.TipMsg(connection, string.format("输入的%s数量有误。", MONEY.GetGoldTypeName("ingot")))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	data.tradeId = tonumber(data.tradeId)
	if trade:GetId() ~= data.tradeId then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	if data.gold then
		trade:SetTradeGold(data.gold)
	end
	if data.ingot then
		trade:SetTradeIngot(data.ingot)
	end
	connection:SendProtocol("c_trade_setMoney", data)
	trade:SendMoneyList()
	local type = trade:GetUserType(connection.roleId)
	if trade:IsTradeLocked(GetAnotherUserType(type)) then
		trade:SetTradeLock(GetAnotherUserType(type))
		trade:SendTradeLockState()
	end
end
--添加交易物品
local s_trade_addItem = function(connection, data)
	if not type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "请输入要交易的物品数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	data.tradeId = tonumber(data.tradeId)
	if trade:GetId() ~= data.tradeId then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	data.areaId = tostring(data.areaId)
	local item = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "没有指定的物品信息。")
	end
	local num = data.num
	for k, v in ipairs(trade:GetTradeItemList()) do
		if v.areaId == data.areaId then
			num = num + v.num
		end
	end
	if num > item:GetNum() then
		return ERROR.TipMsg(connection, string.format("物品【%s】数量不足。", item:GetName()))
	end
	local type = trade:GetUserType(connection.roleId)
	trade:AddOneItem(type, data.areaId, data.num)
	connection:SendProtocol("c_trade_addItem", data)
	trade:SendItemList()
	if trade:IsTradeLocked(GetAnotherUserType(type)) then
		trade:SetTradeLock(GetAnotherUserType(type))
		trade:SendTradeLockState()
	end
end
--删除交易物品
local s_trade_delItem = function(connection, data)
	if not type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	data.tradeId = tonumber(data.tradeId)
	if trade:GetId() ~= data.tradeId then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	data.index = tonumber(data.index)
	if not data.index then
		return ERROR.TipMsg(connection, "请选择要下架的物品。")
	end
	local type = trade:GetUserType(connection.roleId)
	trade:DelOneItem(type, data.index)
	connection:SendProtocol("c_trade_delItem", data)
	trade:SendItemList()
	if trade:IsTradeLocked(GetAnotherUserType(type)) then
		trade:SetTradeLock(GetAnotherUserType(type))
		trade:SendTradeLockState()
	end
end
--取消交易
local s_trade_cancel = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	return CancelTrade(user)
end
--锁定交易
local s_trade_lock = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	trade:SetTradeLock(trade:GetUserType(connection.roleId), true)
	trade:SendTradeLockState()
end
--取消交易锁定
local s_trade_unLock = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	trade:SetTradeLock(trade:GetUserType(connection.roleId))
	trade:SendTradeLockState()
end
--交易确认
local s_trade_confirm = function(connection, data)
	data = tonumber(data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local trade = user:GetTrade()
	if not trade or not GetTradeObjById(trade:GetId()) then
		return ERROR.TipMsg(connection, "该交易已经结束。")
	end
	if trade:GetId() ~= data then
		return ERROR.TipMsg(connection, "无效的交易id。")
	end
	local type = trade:GetUserType(connection.roleId)
	if not trade:IsTradeLocked(type) then
		return ERROR.TipMsg(connection, "您还没有锁定交易，不能进行交易确认。")
	end
	if not trade:IsTradeLocked(GetAnotherUserType(type)) then
		return ERROR.TipMsg(connection, "对方没有锁定交易，不能进行交易确认。。")
	end
	trade:SetTradeConfirm(trade:GetUserType(connection.roleId), true)
	connection:SendProtocol("c_trade_confirm", data)
	if trade:IsTradeConfirm("source") and trade:IsTradeConfirm("target") then
		return FinishTrade()
	end
end



function __init__()
	PROTOCOL.s_trade_request = s_trade_request
	PROTOCOL.s_trade_create = s_trade_create
	
	PROTOCOL.s_trade_moneyList = s_trade_moneyList
	PROTOCOL.s_trade_itemList = s_trade_itemList
	
	PROTOCOL.s_trade_setMoney = s_trade_modMoney
	PROTOCOL.s_trade_addItem = s_trade_addItem
	PROTOCOL.s_trade_delItem = s_trade_delItem
	
	PROTOCOL.s_trade_cancel = s_trade_cancel
	PROTOCOL.s_trade_lock = s_trade_lock
	PROTOCOL.s_trade_unLock = s_trade_unLock
	PROTOCOL.s_trade_confirm = s_trade_confirm
end
