--市场

local FRESH_PRICE_TIMELONG = 3600	--刷新价格间隔
local MAX_TRADE_TIMES = 5		--最大贸易次数

local ResPriceList = {}

local resourceList = {
	["spar"] = {sell = {min = 500, max = 1000}, buy = {min = 300, max = 800}},
}

function FreshResourcePrice()
	CALLOUT.CallOut(FreshResourcePrice, FRESH_PRICE_TIMELONG)
	for k, v in pairs(resourceList) do
		ResPriceList[k] = {}
		for key, val in pairs(v) do
			ResPriceList[k][key] = math.random(val.min, val.max) / 1000
		end
		ResPriceList.endTime = os.time() + FRESH_PRICE_TIMELONG
	end
end

function GetSellResPrice(type)
	return ResPriceList[type].sell
end

function GetBuyResPrice(type)
	return ResPriceList[type].buy
end

--------------------------------------------------------------------
clsMarket = STRUCT.clsStruct:Inherit()

function clsMarket:__init__()
	Super(clsMarket).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsMarket:initsavedata()	
	self.__save.Lv = 0
	self.__save.resIncome = 0
	self.__save.tradeStat = {}
end

function clsMarket:inittempdata()
end

function clsMarket:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:SetTimeStamp(os.time())
	
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
--	self:SetResIncome(info.speed)
	self:SetResMax(info.upLimit)
end

function clsMarket:Upgrade()
	self:SetLv(1)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
--	self:SetResIncome(info.speed)
	self:SetResMax(info.upLimit)
end
--[[
function clsMarket:SetResIncome(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	tribe:SetResourceStat("gold", "speed", data)
end]]

function clsMarket:GetResIncome()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	return info.speed
end

function clsMarket:SetResMax(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	tribe:SetResourceStat("gold", "max", data)
end

function clsMarket:GetResMax()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	return tribe:GetResourceStat("gold", "max")
end

function clsMarket:GetTradeUpLimit()
	return self:GetLv() * 1000
end

--钱币产出实际比率
function clsMarket:GetTotalResIncomeRate()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local rate = 0
	local pet = user:GetPet()
	local base = self:GetResIncome()
	if pet then
		base = base * (1 + pet:GetTalentAdd(311021))
	end
	return math.floor(base) + ACADEMY.GetTechEffectByType(self:GetOwner(), "goldRate")
end

function clsMarket:ProStructFunc(time)
	local timelong = TRIBE.GetResourceFreshTimelong()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local timestamp = self:GetTimeStamp()
	local n = math.floor(((time or os.time()) - timestamp) / timelong)
	if n <= 0 then
		return
	end
	user:SetGold(math.floor(self:GetTotalResIncomeRate() * timelong * n / 3600))
	self:SetTimeStamp(timestamp + n * timelong)
end


function clsMarket:SetTradeNum(data)
	local info = self.__save.tradeStat
	if not info.time then
		self.__save.tradeStat.num = data
	else
		local day = CalDiffDays(info.time, os.time())
		if day > 0 then
			self.__save.tradeStat.num = data
		else
			self.__save.tradeStat.num = self.__save.tradeStat.num + data
		end
	end
	self.__save.tradeStat.time = os.time()
end

function clsMarket:GetTradeNum()
	local info = self.__save.tradeStat
	if not info or not info.time then
		return 0
	else
		local day = CalDiffDays(info.time, os.time())
		if day > 0 then
			return 0
		else
			return self.__save.tradeStat.num
		end
	end
end

function clsMarket:GetTradeMax()
	return MAX_TRADE_TIMES
end

function clsMarket:GetTradeIncomeRate()
	return ACADEMY.GetTechEffectByType(self:GetOwner(), "tradeRate")
end


local s_market_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local market = assert(tribe:GetStruct("market"))
	local sData = {tradeNum = market:GetTradeNum(), tradeMax = market:GetTradeMax()}
	connection:SendProtocol("c_market_list", sData)
end

local s_market_trade = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议格式。")
	end
	if not resourceList[data.type] then
		return ERROR.TipMsg(connection, "无效的资源类型。")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num < 0 then
		return ERROR.TipMsg(connection, "无效的资源数量。")
	end
	data.price = tonumber(data.price)
	local curPrice = GetSellResPrice(data.type)
	if not data.price or data.price ~= curPrice then
		return ERROR.TipMsg(connection, string.format("资源价格有误，当前的价格是：%g。", curPrice))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	if tribe:GetResourceStat(data.type, "num") < data.num then
		return ERROR.TipMsg(connection, "您当前的资源数量不足。")
	end
	local market = tribe:GetStruct("market")
	if not market then
		return ERROR.TipMsg(connection, "您还没有集市")
	end
	if market:GetTradeNum() >= market:GetTradeMax() then
		return ERROR.TipMsg(connection, "您今日的贸易次数已经用完。")
	end
	market:SetTradeNum(1)
	tribe:ModResourceStat(data.type, "num", -data.num)
	local total = math.floor((data.num * data.price) * (1 + market:GetTradeIncomeRate()))
	user:SetGold(total)
	connection:SendProtocol("c_market_trade", total)
end

local s_market_resPrice = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local sData = {}
	for k, v in pairs(ResPriceList) do
		if k == "endTime" then
			sData.timelong = v - os.time()
		else
			sData[k] = v
		end
	end
	connection:SendProtocol("c_market_resPrice", sData)
end

local s_market_buySpar = function(connection, data)
	if type(data) ~= "table" or not data.num or not data.price then
		return ERROR.TipMsg(connection, "无效的购买数量")
	end
	data.num = tonumber(data.num)
	data.price = tonumber(data.price)
	if data.num < 1000 then
		return ERROR.TipMsg(connection, "每次购买数量不能少于1000。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local market = assert(tribe:GetStruct("market"))
	if data.price ~= GetBuyResPrice("spar") then
		return ERROR.TipMsg(connection, "当前价格不正确")
	end
	local cost = data.num * data.price
	if user:GetGold() < cost then
		return ERROR.TipMsg(connection, string.format("购买%d晶石需要%d钱币,您钱币数量不足", data.num, cost))
	end
	user:SetGold(-cost)
	tribe:ModResourceStat("spar", "num", data.num)
	connection:SendProtocol("c_market_buySpar", data.num)
end

function __init__()
	FreshResourcePrice()
	PROTOCOL.s_market_list = s_market_list
	PROTOCOL.s_market_trade = s_market_trade
	PROTOCOL.s_market_resPrice = s_market_resPrice
	PROTOCOL.s_market_buySpar = s_market_buySpar
end
