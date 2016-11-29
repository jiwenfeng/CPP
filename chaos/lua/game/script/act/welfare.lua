
local QUICK_DONE_NEED_INGOT = 20
local FIND_LOST_NEED_INGOT = 20

local WelfareInfoList = {}

function GetWelfareInfoList()
	return WelfareInfoList
end

function GetWelfareInfoById(id)
	return WelfareInfoList[id]
end


local STATE_FINISH = 1		--已经完成
local STATE_DOING = 2		--正在进行
local STATE_CAN_FIND = 3		--可找回
local STATE_ALL_DONE = 4		--奖励已经领取

------------------------------------------------------------
local clsUser = USERBASE.clsUser


--记录领取福利奖励
function clsUser:SetWelfarePrizeGot(id)
	local welfareInfo = GetWelfareInfoById(id)
	if not welfareInfo then return false end
	local info = self:GetFuncStatById(id)
	if CalDiffDays(info.last or SERVER_START_TIME, os.time()) > 1 then
		info.canFind = true
	end
	info.last = os.time()
end

function clsUser:IsWelfarePrizeGot(id)
	local info = self:GetFuncStatById(id)
	if not info or not info.last then
		return false
	end
	return CalDiffDays(info.last, os.time()) == 0
end

--是否可以找回
function clsUser:IsWelfareCanFind(id)
	local welfareInfo = GetWelfareInfoById(id)
	if not welfareInfo then return false end
	local info = self:GetFuncStatById(id)
	if not info or not info.last or CalDiffDays(info.last, os.time()) ~= 0 then		--领取过今日的奖励才可以找回昨天的
		return false
	end
	return info.canFind
end

function clsUser:SetWelfareCanFind(id, state)
	local info = self:GetFuncStatById(id)
	if not info then
		return
	end
	info.canFind = state
end

function clsUser:CheckWelfareFinish(funcId)
	local welfareInfo = GetWelfareInfoById(funcId)
	if not welfareInfo then
		return
	end
	local state = GetWelfarePrizeState(self, funcId)
	if state == STATE_FINISH and not self:IsActStateNoticed(SETTING.ACT_DAILY_ACT) then
		FUNCOPEN.SendFuncStateUpdate(self, SETTING.ACT_DAILY_ACT, FUNCOPEN.STATE_OK)
		self:SetActStateNoticed(SETTING.ACT_DAILY_ACT, true)
	end
end

function clsUser:SetActStateNoticed(funcId, state)
	self.__temp.actNoticed = self.__temp.actNoticed or {}
	self.__temp.actNoticed[funcId] = state
end

function clsUser:IsActStateNoticed(funcId)
	self.__temp.actNoticed = self.__temp.actNoticed or {}
	return self.__temp.actNoticed[funcId]
end
------------------------------------------------------------------------------------

--立即领取奖励
function QuickReceiveWelfarePrize(user, id)
	local welfareInfo = GetWelfareInfoById(id)
	if not welfareInfo then return end
	ReceiveWelfarePrize(user, id)
end

function FinishWelfare(user, id)
	local myInfo = user:GetFuncStatById(id)
	local info = GetWelfareInfoById(id)
	if not info or not myInfo then
		return
	end
	myInfo.times = info.times
end

--领取福利奖励
function ReceiveWelfarePrize(user, id)
	local welfareInfo = GetWelfareInfoById(id)
	if not welfareInfo then return end
	user:SetWelfarePrizeGot(id)
	local nimbus = CalPrizeNimbus(id, user:GetLv())
	user:SetNimbus(nimbus)
	user:SendProtocol("c_show_tips", string.format("获得灵气%d", nimbus))
end

--计算奖励灵气
function CalPrizeNimbus(id, Lv)
	local welfareInfo = GetWelfareInfoById(id)
	if not welfareInfo then return 0 end
	return welfareInfo.times * (Lv + 20) * 400
end

function ReceiveGetBackPrize(user, id)
	if not user:IsWelfareCanFind(id) then
		return
	end
	user:SetWelfareCanFind(id)
	local nimbus = CalPrizeNimbus(id, user:GetLv())
	user:SetNimbus(nimbus)
end

function GetWelfarePrizeState(user, id)
	local info = user:GetFuncStatById(id)
	local welfareInfo = GetWelfareInfoById(id)
	local state = STATE_DOING
	if info.times >= welfareInfo.times then
		state = STATE_FINISH
	end
	if user:IsWelfareCanFind(id) then		
		state = STATE_CAN_FIND
	end
	--已经完成  今日奖励已经领取且昨天的不可找回
	if CalDiffDays(info.last, os.time()) == 0 and not info.canFind then
		state = STATE_ALL_DONE
	end
	return state
end

function FormatWelfareList(user)
	local sData = {}
	sData.cost = QUICK_DONE_NEED_INGOT
	sData.list = {}
	local Lv = user:GetLv()
	for k, v in pairs(GetWelfareInfoList()) do
		if Lv >= v.needLv then
			local buf = {}
			buf.name = v.name
			buf.link = k
			buf.maxNum = v.times
			buf.nimbus = CalPrizeNimbus(k, Lv) 
			buf.picId = v.picId
			buf.state = GetWelfarePrizeState(user, k)
			local myInfo = user:GetFuncStatById(k)
			buf.curNum = math.min(myInfo.times, v.times)
			if buf.state ~= STATE_DOING then
				buf.curNum = v.times
			end
			table.insert(sData.list, buf)
		end
	end
	table.sort(sData.list, function(a, b) return a.state < b.state end)
	return sData
end





local s_welfare_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:SetActStateNoticed(SETTING.ACT_DAILY_ACT)
	local sData = FormatWelfareList(user)
	connection:SendProtocol("c_welfare_list", sData)
end

--立即完成
local s_welfare_finish = function(connection, data)
	local info = GetWelfareInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < info.needLv then
		return ERROR.TipMsg(connection, "您的等级不足，不能参与该活动。")
	end
	if user:IsWelfarePrizeGot(data) then
		return ERROR.TipMsg(connection, "您已经领取过该奖励了。")
	end
	if user:GetTotalIngot() < QUICK_DONE_NEED_INGOT then
		return ERROR.TipMsg(connection, "您的元宝不足。")
	end
--	QuickReceiveWelfarePrize(user, data)
	FinishWelfare(user, data)
	local myInfo = user:GetFuncStatById(data)
	user:SetTotalIngot(-QUICK_DONE_NEED_INGOT)
	local sData = {link = data, state = GetWelfarePrizeState(user, data), curNum = myInfo.times}
	connection:SendProtocol("c_welfare_finish", sData)
end

local s_welfare_getBack = function(connection, data)
	local info = GetWelfareInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < info.needLv then
		return ERROR.TipMsg(connection, "您的等级不足，不能参与该活动。")
	end
	if not user:IsWelfarePrizeGot(data) then
		return ERROR.TipMsg(connection, "您今天的活动没有完成，不能进行找回操作。")
	end
	if not user:IsWelfareCanFind(data) then
		return ERROR.TipMsg(connection, "您没有可找回的奖励。")
	end
	if user:GetTotalIngot() < FIND_LOST_NEED_INGOT then
		return ERROR.TipMsg(connection, "您的元宝数量不足")
	end
	user:SetTotalIngot(-FIND_LOST_NEED_INGOT)
	ReceiveGetBackPrize(user, data)
	local sData = {link = data, state = GetWelfarePrizeState(user, data)}
	connection:SendProtocol("c_welfare_getBack", sData)
end

local s_welfare_prize = function(connection, data)
	local info = GetWelfareInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local myInfo = user:GetFuncStatById(data)
	if myInfo.times < info.times then
		return ERROR.TipMsg(connection, "您尚未达成条件，不能领取该奖励。")
	end
	if user:IsWelfarePrizeGot() then
		return ERROR.TipMsg(connection, "您已经领取过该奖励了。")
	end
	ReceiveWelfarePrize(user, data)
	local state = FUNCOPEN.GetFuncState(user, SETTING.ACT_DAILY_ACT)
	if state ~= FUNCOPEN.STATE_OK then
		FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_DAILY_ACT, state)
	end
	local sData = {link = data, state = GetWelfarePrizeState(user, data)}
	connection:SendProtocol("c_welfare_prize", sData)
end


function __init__()
	local WELFARELIST = Import("autocode/welfareList.lua")
	WelfareInfoList = WELFARELIST.GetTable()

	PROTOCOL.s_welfare_list = s_welfare_list
	PROTOCOL.s_welfare_finish = s_welfare_finish
	PROTOCOL.s_welfare_getBack = s_welfare_getBack
	PROTOCOL.s_welfare_prize = s_welfare_prize
	
end
