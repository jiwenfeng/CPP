
local ADD_SIGN_COST = 10		--补签所需元宝

local vipPrizeList = {}
local signPrizeList = {}
------------------------------------------------
local clsUser = USERBASE.clsUser

function clsUser:GetSignInfo()
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	return self.__save.guard.sign
end

function clsUser:ClearSignInfo()
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	local tm = os.date("*t")
	for k, v in pairs(self.__save.guard.sign) do
		if v ~= tm.month then
			v = nil
		end
	end
end

function clsUser:IsTodaySigned(day)
	local tm = os.date("*t")
	day = day or tm.day
	return self.__save.guard.sign[tostring(day)] == tm.month
end

function clsUser:Sign(day)
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	local tm = os.date("*t")
	day = tonumber(day) or tm.day
	if day == 1 then
		self:ClearSignInfo()
	end
	if self:IsTodaySigned(day) then
		return false, "今日已经签到过了。"
	end
	self.__save.guard.sign[tostring(day)] = tm.month
	SendSignPrize(self)
	return true
end

--获取签到次数
function clsUser:GetSignTimes()
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	local tm = os.date("*t")
	local num = 0
	for k, v in pairs(self.__save.guard.sign) do
		if v == tm.month then
			num = num + 1
		end
	end
	return num
end

function clsUser:FormatSignInfo()
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	local tm = os.date("*t")
	local sData = {}
	for k, v in pairs(self.__save.guard.sign) do
		if v == tm.month then
			table.insert(sData, k)
		end
	end
	return sData
end

--vip每日奖励
function clsUser:SetVipDailyPrizeGot(timestamp)
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.vipDaily = timestamp or os.time()
end

function clsUser:IsVipDailyPrizeGot()
	self.__save.guard = self.__save.guard or {}
	return CalDiffDays(os.time(), self.__save.guard.vipDaily or SERVER_START_TIME) == 0
end

--补签
function clsUser:AddSign()
	self.__save.guard = self.__save.guard or {}
	self.__save.guard.sign = self.__save.guard.sign or {}
	local tm = os.date("*t")
	local day
	for i = 1, tm.day - 1 do
		if not self:IsTodaySigned(i) then
			day = i
			break
		end
	end
	if not day then
		return false, "当月没有可补签的日子"
	end
	if self:GetTotalIngot() < ADD_SIGN_COST then
		return false, "您元宝数量不足"
	end
	self:SetTotalIngot(-ADD_SIGN_COST)
	self:Sign(day)
	return true, day
end
-------------------------------------------------------------
function GetSignPrizeByTimes(times)
	return signPrizeList[times]
end
--发送奖励
function SendSignPrize(user)
	local times = user:GetSignTimes()
	local info = GetSignPrizeByTimes(times)
	if not info then
		return
	end
	local extraItem = {}
	for k, v in ipairs(info) do
		local moneyType = MONEY.GetGoldTypeByPicId(v.id)
		if moneyType then
			local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
			clsUser[moneyInfo.set](user, v.num)
		else
			local item = BASEITEM.NewItem(v.id, v.num, {lock = 1})
			if user:IsItemCanAdd(USERBASE.PACKAGE, item) then
				user:AddItem(USERBASE.PACKAGE, item)
			else
				table.insert(extraItem, {id = v.id, num = v.num, lock = 1})
			end
		end
	end
	if #extraItem > 0 then
		local mail = {}
		local tm = os.date("*t")
		mail.title = string.format("%s月%s日签到奖励", tm.month, tm.day)
		mail.content = "背包已满，签到奖励通过邮件发送给您，请注意查收。"
		mail.attach = {item = extraItem}
		MAIL.SendSysMail(user:GetRoleId(), mail)
	end
end


function GetVipDailyPrizeByLv(lv)
	return vipPrizeList[lv]
end

function GetFormatVipDaliyPrize(lv)
	local prize = GetVipDailyPrizeByLv(lv)
	if not prize then
		return
	end
	local buff = {goodsId = prize.id, content = {}}
	local tempItem = BASEITEM.NewItem(prize.id, 1)
	local info = tempItem:GetPackageInfo()
	buff.content.gold = info.gold
	buff.content.exp = info.exp
	buff.content.nimbus = info.nimbus
	buff.content.voucher = info.voucher
	buff.content.spar = info.spar
	if info.itemList then
		buff.content.itemList = {}
		for k, v in pairs(info.itemList) do
			if v.id then
				table.insert(buff.content.itemList, {goodsId = v.id, num = v.num})
			end
		end
	end
	return buff
end

function SendVipDailyPrize(user)
	local info = GetVipDailyPrizeByLv(user:GetVipLv())
	if not info then
		return false, "您没有可以领取的奖励。"
	end
	if user:IsVipDailyPrizeGot() then
		return false, "您今天的奖励已经领取"
	end
	local item = BASEITEM.NewItem(info.id, info.num, {lock = 1})
	if not user:IsItemCanAdd(USERBASE.PACKAGE, item) then
		return false, "背包已满"
	end
	user:AddItem(USERBASE.PACKAGE, item)
	user:SetVipDailyPrizeGot(os.time())
	return true
end

function GetSignPrizeList(user)
	local times = user:GetSignTimes()
	local sData = {}
	for k, v in pairs(signPrizeList) do
		local buf = {}
		buf.times = k
		buf.list = v
--		buf.state = (times >= k)	--未领取
		table.insert(sData, buf)
	end
	return sData
end

local s_sign_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.signList = user:FormatSignInfo()
	sData.times = user:GetSignTimes()
	sData.today = os.time()
	connection:SendProtocol("c_sign_info", sData)
end

local s_sign_prizeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetSignPrizeList(user)
	connection:SendProtocol("c_sign_prizeList", sData)
end

local s_sign_vipPrize = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local Lv = user:GetVipLv()
	local sData = {}
	sData.now = GetFormatVipDaliyPrize(Lv)
	sData.next = GetFormatVipDaliyPrize(Lv + 1)
	sData.state = user:IsVipDailyPrizeGot() and 1 or 0
	connection:SendProtocol("c_sign_vipPrize", sData)
end

local s_sign_sign = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = user:Sign()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_sign_sign", "true")
	PROTOCOL.s_sign_info(connection, "get")
end

local s_sign_getPrize = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = SendVipDailyPrize(user)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_sign_getPrize", user:IsVipDailyPrizeGot() and 1 or 0)
end

local s_sign_addSign = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = user:AddSign()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_sign_addSign", "true")
	PROTOCOL.s_sign_info(connection, "get")
end

function __init__()
	local tb = Import("autocode/vipDailyPrize.lua")
	vipPrizeList = tb.GetTable()
	
	tb = Import("autocode/signPrizeList.lua")
	signPrizeList = tb.GetTable()

	PROTOCOL.s_sign_info = s_sign_info
	PROTOCOL.s_sign_prizeList = s_sign_prizeList
	PROTOCOL.s_sign_vipPrize = s_sign_vipPrize
	PROTOCOL.s_sign_sign = s_sign_sign
	PROTOCOL.s_sign_getPrize = s_sign_getPrize
	PROTOCOL.s_sign_addSign = s_sign_addSign
end
