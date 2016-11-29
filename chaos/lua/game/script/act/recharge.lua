--充值


local RechargePrizeList = {}
local FirstRechargePrize = {}

local UserRechargeList = {}		--玩家充值信息
local path_recharge = "chaos.recharge"

function GetRechargePrizeList()
	return RechargePrizeList
end

function GetRechargePrizeById(id)
	return RechargePrizeList[id]
end

function GetFirstRechargePrize()
	return FirstRechargePrize[1]
end

function GetUserRechargeInfo(roleId)
	return UserRechargeList[roleId]
end

function InitUserRechargeInfo(roleId)
	UserRechargeList[roleId] = {sum = 0, list = {}, prizeList = {}, firstRecharge = FUNCOPEN.STATE_DOING}
end

function SaveUserRechargeInfo(roleId)
	DATA.SaveData(path_recharge, UserRechargeList, roleId)
end


--统计玩家充值信息
function RecordUserRecharge(roleId, ingot, timestamp)
	if not UserRechargeList[roleId] then
		InitUserRechargeInfo(roleId)
	end
	local info = GetUserRechargeInfo(roleId)
	info.sum = info.sum + ingot
	table.insert(info.list, {num = ingot, timestamp = timestamp or os.time()})
	if info.firstRecharge == FUNCOPEN.STATE_DOING then		--首充奖励
		info.firstRecharge = FUNCOPEN.STATE_OK
	end
	for k, v in ipairs(GetRechargePrizeList()) do	--累计充值奖励
		if v.needIngot < info.sum then
			if not info.prizeList[k] then
				info.prizeList[k] = FUNCOPEN.STATE_OK
			end
		end
	end
	SaveUserRechargeInfo(roleId)
end

function ModRechargePrizeState(roleId, type, id, state)
	local info = GetUserRechargeInfo(roleId)
	if not info then return end
	if type == "first" then
		info.firstRecharge = state
	elseif type == "recharge" then
		info.prizeList[id] = state
	end
	SaveUserRechargeInfo(roleId)
end

function GetRechargePrizeState(roleId, type, id)
	local info = GetUserRechargeInfo(roleId)
	if not info then
		return FUNCOPEN.STATE_DOING
	end
	if type == "first" then
		return info.firstRecharge
	end
	return info.prizeList[id] or FUNCOPEN.STATE_DOING
end


function GetFormatedRechargeList(roleId)
	local sData = {}
	for k, v in ipairs(GetRechargePrizeList()) do
		local buff = {id = k}
		buff.name = v.name
		buff.picId = v.picId
		buff.state = GetRechargePrizeState(roleId, "recharge", k)
		buff.itemList = v.itemList
		table.insert(sData, buff)
	end
	return sData
end

function GetFormatRechargeFirstPrize(roleId)
	local info = GetFirstRechargePrize()
	local sData = {}
	sData.type = "first"
	sData.name = info.name
	sData.picId = info.picId
	sData.state = GetRechargePrizeState(roleId, "first")
	sData.itemList = v.itemList
	return sData
end

function ReceivePrize(user, type, id)
	local roleId = user:GetRoleId()
	local state = GetRechargePrizeState(roleId, type, id)
	if state == FUNCOPEN.STATE_DOING then
		return false, "条件不足，不能领取该奖励。"
	elseif state == FUNCOPEN.STATE_INVALID then
		return false, "您已经领取过该奖励了，不能重复领取。"
	end
	local prizeList
	if type == "first" then
		local info = GetFirstRechargePrize()
		prizeList = info.itemList
	elseif type == "recharge" then
		local info = GetRechargePrizeById(id)
		prizeList = info.itemList
	end
	if user:GetGridMax(USERBASE.PACKAGE) - user:GetItemGridNum(USERBASE.PACKAGE) <= #prizeList then
		return false, "您的背包空间不足。"
	end
	for k, v in pairs(prizeList) do
		local itemId = v.goodsId or v[user:GetJob()]
		local item = BASEITEM.NewItem(itemId, v.num, {lock = 1})
		if item then
			user:AddItem(USERBASE.PACKAGE, item)
		end
	end
	ModRechargePrizeState(roleId, type, id, FUNCOPEN.STATE_INVALID)
	if type == "first" then
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_FIRST_RECHARGE, FUNCOPEN.STATE_INVALID)
	else
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_RECHARGE_PRIZE, FUNCOPEN.STATE_DOING)
	end
	return true, FUNCOPEN.STATE_INVALID
end


local s_recharge_prizeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatedRechargeList(connection.roleId)
	connection:SendProtocol("c_recharge_prizeList", sData)
end

local s_recharge_firstPrize = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatRechargeFirstPrize(connection.roleId)
	connection:SendProtocol("c_recharge_firstPrize", sData)
end

local s_recharge_receivePrize = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.ErrorMsg(connection, "协议错误，无效的请求格式。")
	end
	if data.type ~= "first" or data.type ~= "recharge" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = ReceivePrize(user, data.type, data.id)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_recharge_receivePrize", {type = data.type, id = data.id, state = msg})
end



function __init__()
	local FIRST_RECHARGE = Import("autocode/rechargeFirst.lua")
	FirstRechargePrize = FIRST_RECHARGE.GetTable()
	
	local RECHARGE_PRIZE = Import("autocode/rechargePrizeList.lua")
	RechargePrizeList = RECHARGE_PRIZE.GetTable()
	
	DATA.Restore(path_recharge, UserRechargeList)

	PROTOCOL.s_recharge_prizeList = s_recharge_prizeList
	PROTOCOL.s_recharge_firstPrize = s_recharge_firstPrize
	PROTOCOL.s_recharge_receivePrize = s_recharge_receivePrize
	
end
