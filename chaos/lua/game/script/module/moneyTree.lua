--摇钱树 moneyTree

local TREE_INCOME_RATE = 2.4		--摇钱树收益倍率
local FATAL_INCOME_RATE = 0.2		--暴击收益概率
local FATAL_INCOME_ADD_RATE = 1.2	--暴击收益倍率

local RockTreeCostList = {}

function GetRockTreeCost(times)
	return RockTreeCostList[times]
end

function CalRockTreeIncome(user)
	local tribe = user:GetTribe()
	local num = 0
	if tribe then
		local market = tribe:GetStruct("market")
		if market then
			num = market:GetTotalResIncomeRate()
		end
	end
	return math.ceil(num * TREE_INCOME_RATE + 4750 + 580 * math.max((user:GetLv() - 1), 0))
end


-----------------------------------------------

local clsUser = USERBASE.clsUser

function clsUser:GetRockTreeTimes()
	self.__save.rockTree = self.__save.rockTree or {}
	if not self.__save.rockTree.timestamp or CalDiffDays(self.__save.rockTree.timestamp, os.time())~=0 then
		self.__save.rockTree.times = 0
	end
	return self.__save.rockTree.times
end

function clsUser:SetRockTreeTimes(data)
	self.__save.rockTree = self.__save.rockTree or {}
	if not self.__save.rockTree.timestamp or CalDiffDays(self.__save.rockTree.timestamp, os.time())~=0 then
		self.__save.rockTree.times = 0
	end
	self.__save.rockTree.times = math.max((self.__save.rockTree.times or 0) + data, 0)
	self.__save.rockTree.timestamp = os.time()
end

function clsUser:GetRockTreeMaxTimes()
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_DAILY_ROCK_TREE)
end


--摇钱
function RockMoneyTree(user)
	local tribe = user:GetTribe()
	if not tribe then
		return false, "您还没有建立部落。"
	end
	local times = user:GetRockTreeTimes()
	if times >= user:GetRockTreeMaxTimes() then
		return false, "您今日的次数已经用完。"
	end
	local needIngot = GetRockTreeCost(times + 1)
	if user:GetTotalIngot() < needIngot then
		return false, "您的元宝不足，请充值。"
	end
	local isFatal = 0
	local income = CalRockTreeIncome(user)
	if math.random() <= FATAL_INCOME_RATE then
		isFatal = 1
		income = math.floor(income * FATAL_INCOME_ADD_RATE)
	end
	user:SetTotalIngot(-needIngot)
	user:SetGold(income)
	user:SetRockTreeTimes(1)
	return income, isFatal
end


local s_moneyTree_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:IsFuncOpen(SETTING.FUNC_TREE_ROCK) then
		return ERROR.TipMsg(connection, "您的等级不足，该功能尚未开启。")
	end
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local sData = {}
	local times = user:GetRockTreeTimes()
	sData.times = user:GetRockTreeMaxTimes() - times
	sData.cost = GetRockTreeCost(times + 1)
	sData.income = CalRockTreeIncome(user) 
	connection:SendProtocol("c_moneyTree_info", sData)
end

local s_moneyTree_rock = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:IsFuncOpen(SETTING.FUNC_TREE_ROCK) then
		return ERROR.TipMsg(connection, "等级未达到，该功能还未开启")
	end
	local ret, rst = RockMoneyTree(user)
	if not ret then
		return ERROR.TipMsg(connection, rst)
	end
	if user:GetRockTreeTimes() >= user:GetRockTreeMaxTimes() then
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.FUNC_TREE_ROCK, FUNCOPEN.STATE_INVALID)
	end
	local sData = {income = ret, isFatal = rst}
	connection:SendProtocol("c_moneyTree_rock", sData)
	PROTOCOL.s_moneyTree_info(connection, "get")
end



function __init__()
	local TREECOST = Import("autocode/moneyTreeCostList.lua")
	RockTreeCostList = TREECOST.GetTable()

	PROTOCOL.s_moneyTree_info = s_moneyTree_info
	PROTOCOL.s_moneyTree_rock = s_moneyTree_rock
end

