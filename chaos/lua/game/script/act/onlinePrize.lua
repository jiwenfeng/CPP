--在线奖励模块


local ONLINE_PRIZE_INIT_ID = 1	--在线奖励初始id

local OnlinePrizeList = {}

function GetOnlinePrizeInfo(id)
	return OnlinePrizeList[id]
end

function GetOnlinePrizeList()
	return OnlinePrizeList
end

local clsUser = USERBASE.clsUser

function clsUser:InitOnlinePrizeInfo()
	self.__save.onlinePrize = self.__save.onlinePrize or {}
	local info = GetOnlinePrizeInfo(ONLINE_PRIZE_INIT_ID)
	if info then
		self:SetOnlinePrizeInfo(ONLINE_PRIZE_INIT_ID)
	end
end

function clsUser:SetOnlinePrizeInfo(id)
	self.__save.onlinePrize = self.__save.onlinePrize or {}
	local prizeInfo = GetOnlinePrizeInfo(id)
	if not prizeInfo then
		return
	end
	local info = self.__save.onlinePrize
	info.type = id					--奖励编号
	info.needTime = prizeInfo.timelong		--所需时长
	info.timestamp = os.time()		--时间戳
	info.onlineSum = 0				--累计时长
	self:StartOnlinePrizeCallOut()
end

function clsUser:GetOnlinePrizeInfo()
	self.__save.onlinePrize = self.__save.onlinePrize or {}
	return self.__save.onlinePrize
end

function clsUser:SetOnlinePrizeAllOver(state)
	local info = self:GetOnlinePrizeInfo()
	if info then
		info.allOver = state or true
	end
end

--上线时更新在线奖励信息
function clsUser:UpdateOnlinePrizeInfo()
	local info = self:GetOnlinePrizeInfo()
	if not info or not info.type then
		self:InitOnlinePrizeInfo()
	end
	info = self:GetOnlinePrizeInfo()
	local offlineTime = self:GetLastOfflineTime()
	if offlineTime and offlineTime > info.timestamp then
		info.onlineSum = info.onlineSum + (offlineTime - info.timestamp)
	end
	info.timestamp = os.time()
	self:StartOnlinePrizeCallOut()
end

--在线奖励定时器
function clsUser:StartOnlinePrizeCallOut()
	local info = self:GetOnlinePrizeInfo()
	if not info or not info.type then
		return
	end
	local timelong = info.needTime - info.onlineSum
	if timelong <= 0 then
		return SendOnlinePrizeNotice(self:GetRoleId(), FUNCOPEN.STATE_OK)
	end
	self.__temp.onlinePrizeTimer = CALLOUT.ObjCallOut(self:GetCallOut(), SendOnlinePrizeNotice, timelong, self:GetRoleId(), FUNCOPEN.STATE_OK)
end

function clsUser:RemoveOnlinePrizeCallOut()
	if self.__temp.onlinePrizeTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.onlinePrizeTimer)
	end
end


function SendOnlinePrizeNotice(roleId, state)
	local user = USER.GetOnlineUser(roleId)
	if user then
		local info = user:GetOnlinePrizeInfo()
		info.onlineSum = info.onlineSum + (os.time() - info.timestamp)
		info.timestamp = os.time()
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_ONLINE_PRIZE, FUNCOPEN.STATE_OK)
	end
end

--领取奖励
function ReceiveOnlinePrize(user)
	local info = user:GetOnlinePrizeInfo()
	if not info or not info.type then
		return false, "当前没有可以领取的奖励。"
	end
	if info.needTime > info.onlineSum + (os.time() - info.timestamp) then
		return false, "奖励即将来到，请耐心等候。"
	end
	if info.allOver then
		return false, "您已经领完所有的奖励了。"
	end
	local prizeInfo = GetOnlinePrizeInfo(info.type)
	local used = user:GetItemGridNum(USERBASE.PACKAGE)
	local total = user:GetGridMax(USERBASE.PACKAGE)
	if total - used < #prizeInfo.itemList then
		return false, "当前背包空余位置不足。"
	end
	local prizeInfo = GetOnlinePrizeInfo(info.type)
	local money_msg
	local item_msg
	if prizeInfo.gold then
		money_msg = money_msg .. string.format("%d金钱", prizeInfo.gold)
		user:SetGold(prizeInfo.gold)
	end
	if prizeInfo.nimbus then
		money_msg = money_msg .. string.format("，%d灵气", prizeInfo.nimbus)
		user:SetNimbus(prizeInfo.nimbus)
	end
	if prizeInfo.voucher then
		money_msg = money_msg .. string.format("，%d礼券", prizeInfo.voucher)
		user:SetVoucher(prizeInfo.voucher)
	end
	if prizeInfo.itemList then
		for k, v in pairs(prizeInfo.itemList) do
			local moneyType = MONEY.GetGoldTypeByPicId(v.id)
			if moneyType then
				local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
				USERBASE.clsUser[moneyInfo.set](user, v.num)
				if money_msg then
					money_msg = money_msg .. string.format(" %d%s ", v.num, moneyInfo.name)
				else
					money_msg = string.format("获得了%d%s", v.num, moneyInfo.name)
				end
			else
				local item = BASEITEM.NewItem(v.id, v.num, {lock = 1})
				if item then
					user:AddItem(USERBASE.PACKAGE, item)
					if not item_msg then
						item_msg = string.format("获得了【%s】x %d ", item:GetName(), v.num)
					else
						item_msg = item_msg .. string.format("【%s】x %d ", item:GetName(), v.num)
					end
				end
			end
		end
	end
	if prizeInfo.next then
		user:SetOnlinePrizeInfo(prizeInfo.next)
		local info = user:GetOnlinePrizeInfo()
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_ONLINE_PRIZE, FUNCOPEN.STATE_DOING, info.needTime)
	else
		user:SetOnlinePrizeAllOver()
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_ONLINE_PRIZE, FUNCOPEN.STATE_INVALID)
	end
	if money_msg then
		user:SendProtocol("c_show_tips", money_msg)
	end
	if item_msg then
		user:SendProtocol("c_show_tips", item_msg)
	end
	return true
end





	
local s_online_prize = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local info = user:GetOnlinePrizeInfo()
	if not info or not info.type then
		return ERROR.TipMsg(connection, "当前没有可以领取的奖励。")
	end
	if info.needTime < info.onlineSum then
		return ERROR.TipMsg(connection, "奖励即将来到，请耐心等候。")
	end
	if info.allOver then
		return ERROR.TipMsg(connection, "您已经领完所有的奖励了。")
	end
	local prizeInfo = GetOnlinePrizeInfo(info.type)
	local used = user:GetItemGridNum(USERBASE.PACKAGE)
	local total = user:GetGridMax(USERBASE.PACKAGE)
	if total - used < #prizeInfo.itemList then
		return ERROR.TipMsg(connection, "当前背包空余位置不足。")
	end
	ReceiveOnlinePrize(user)
	local sData = {funcId = SETTING.ACT_ONLINE_PRIZE}
	local nextInfo = GetOnlinePrizeInfo(prizeInfo.next)
	if nextInfo then
		sData.state = FUNCOPEN.STATE_DOING
		sData.timelong = nextInfo.timelong
	else
		sData.state = FUNCOPEN.STATE_DISAPPEAR
	end
	connection:SendProtocol("c_online_prize", sData)
end

function __init__()
	local ONLINEPRIZE = Import("autocode/onlinePrize.lua")
	OnlinePrizeList = ONLINEPRIZE.GetTable()
	
	PROTOCOL.s_online_prize = s_online_prize
	
end


	
