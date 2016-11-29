--势力

local DAILY_DEVOTE_TIMES = 10		--每日可捐献次数
local DEVOTE_NEED_GOLD = 50000		--每次捐献所需金币
local DEVOTE_ADD_FEAT = 5			--捐献获得军功	
local DEVOTE_ADD_DEGREE = 5			--捐献获得建设值
local PRAY_NEED_FEAT = 20			--祈祷所需军功
local NEED_NOTICE_GRADE = 3			--3级以上物品需要全服通告

local MAX_DISTANCE_NUM = 400		--势力人数差距
local VOUCHER_PRIZE = 100			--礼券奖励数量

local phyleList = {
	["phoenix"] = {name = "神农", buf = "晶石产量增加2%"}, 			--神农
	["dragon"] = {name = "轩辕", buf = "科研队列冷却时间减少2%"},	--轩辕
	["tiger"] = {name = "九黎", buf = "建筑队列冷却时间减少2%"},	--九黎
}

local phylePrizeList = {
	[1] = {goodsId = 105004, num = 5}, 
	[2] = {goodsId = 105004, num = 4}, 
	[3] = {goodsId = 105004, num = 3},
}

function GetAllPhyleList()
	return phyleList
end

function IsValidPhyle(type)
	return phyleList[type]
end

function GetPhyleName(phyle)
	return phyleList[phyle].name
end


--------------------------------------------------------------
local clsUser = USERBASE.clsUser

--部落势力
function clsUser:SetPhyle(data)
	self.__save.phyle = {}
	self.__save.phyle.type = data
	USER.ModUserNickInfo(self:GetRoleId(), "phyle", data)
	self:SyncData("phyle")
end

function clsUser:GetPhyle()
	self.__save.phyle = self.__save.phyle or {}
	return self.__save.phyle.type
end

function clsUser:SetPhyleFeatGot(data)
	self:SetPhyleFeat("enable", data)
	self:SetPhyleFeat("max", data)
end

--势力军功
function clsUser:SetPhyleFeat(type, data)
	assert(tonumber(data))
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.feat = self.__save.phyle.feat or {}
	self.__save.phyle.feat[type] = (self.__save.phyle.feat[type] or 0) + data
end

function clsUser:GetPhyleFeat(type)
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.feat = self.__save.phyle.feat or {}
	return self.__save.phyle.feat[type]
end
--势力捐献次数
function clsUser:SetPhyleDevoteTimes(data)
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.devote = self.__save.phyle.devote or {}
	local info = self.__save.phyle.devote
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		info.num = data
	else
		info.num = (info.num or 0) + data
	end
	info.time = os.time()
	self:SetFuncStatById(SETTING.FUNC_PHYDEVOTE, data)
end

function clsUser:GetPhyleDevoteTimes()
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.devote = self.__save.phyle.devote or {}
	local info = self.__save.phyle.devote
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:GetPhyleDevoteMaxTimes()
	return DAILY_DEVOTE_TIMES
end

--势力祈祷
function clsUser:SetPhylePrayTimes(data)
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.pray = self.__save.phyle.pray or {}
	local info = self.__save.phyle.pray
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		info.num = data
	else
		info.num = (info.num or 0) + data
	end
	info.time = os.time()
	self:SetFuncStatById(SETTING.FUNC_ALTAR, data)
end

function clsUser:GetPhylePrayTimes()
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.pray = self.__save.phyle.pray or {}
	local info = self.__save.phyle.pray
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:GetPhylePrayMaxTimes()
	local phyleObj = GetPhyleByType(self:GetPhyle())
	if not phyleObj then
		return 0
	end
	local altar = phyleObj:GetStruct("altar")
	return altar:GetMaxPrayTimes()
end

function clsUser:SetPhylePrayGot(prize, grade)
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.prayGot = self.__save.phyle.prayGot or {}
	self.__save.phyle.prayGot.prize = prize
	self.__save.phyle.prayGot.grade = grade
end

function clsUser:GetPhylePrayGot()
	self.__save.phyle = self.__save.phyle or {}
	self.__save.phyle.prayGot = self.__save.phyle.prayGot or {}
	return self.__save.phyle.prayGot
end

function clsUser:CheckNeedPhyle()
	if self:GetLv() >= 11 and not self:GetPhyle() then
		self:SendProtocol("c_phyle_chose", "true")
		--初始化队列
	end
end

----------------------------------------------------------
local AllPhyleDataList = {}
local path_phyle = "chaos.phyle"
local SAVE_PHYLE_TIME_LONG = 60 * 2

function SetPhyleByType(type, phyleObj)
	AllPhyleDataList[type] = phyleObj
end

function GetPhyleByType(type)
	return AllPhyleDataList[type]
end

function GetAllPhyleObjList()
	return AllPhyleDataList
end

function NewPhyle(type)
	local phyleObj = CLSPHYLE.clsPhyle:New()
	phyleObj:InitPhyle(type)
	SetPhyleByType(type, phyleObj)
	return phyleObj
end


function SaveAllPhyleData()
	for k, v in pairs(AllPhyleDataList) do
		local buff = v:SavePhyleData()
		buff._id = k
		DATA.UpdateData(path_phyle, {_id = k}, buff)
	end
end

function RestoreAllPhyleData()
	local function CallBack(Data, ErrMsg)
		Data = UnSerialize(Data)
		if not Data or #Data <= 0 then
			for k in pairs(phyleList) do
				NewPhyle(k)
			end
		else
			for _, v in pairs(Data) do
				local phyleObj = CLSPHYLE.clsPhyle:New()
				phyleObj:RestorePhyleData(v)
				SetPhyleByType(v._id, phyleObj)
			end
		end
	end
	DATA.Read(path_phyle, {}, CallBack)
end

function GetPhyleInfo(phyleObj, user)
	local sData = {}
	sData.devoteNum = user:GetPhyleDevoteTimes()
	sData.devoteMax = user:GetPhyleDevoteMaxTimes()
--	sData.degree = phyleObj:GetDegree()
	sData.devoteGold = DEVOTE_NEED_GOLD
	sData.list = {}
	for k, v in pairs(phyleObj:GetStructList()) do
		local buff = {}
		buff.type = k
		buff.Lv = v:GetLv()
		buff.maxLv = v:GetMaxLv()
		buff.exp = v:GetExp()
		buff.expMax = v:GetExpMax()
		table.insert(sData.list, buff)
	end
	return sData
end

function GetWeeklyPhyleDegreeList()
	local sData = {}
	for k, v in pairs(GetAllPhyleObjList()) do
		sData[k] = v:GetWeeklyDegree()
	end
	return sData
end

function RenewPhylePrize()
	if CalDiffDays(SERVER_START_TIME, os.time()) <= 3 then			--开服3天以上才发放该奖励
		return
	end
	local degreeList = {}
	for k, v in pairs(GetAllPhyleObjList()) do
		table.insert(degreeList, {phyle = k, degree = v:GetWeeklyDegree()})
	end
	table.sort(degreeList, function(a, b) return a.degree > b.degree end)
	for k, v in ipairs(degreeList) do
		local phyleObj = GetPhyleByType(v.phyle)
		phyleObj:SetPhylePrize(phylePrizeList[k])
	end
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:GetPhyle() then
			FUNCOPEN.SendFuncStateUpdate(v, SETTING.ACT_PHYLE_PRIZE, FUNCOPEN.STATE_OK)
		end
	end
end

function ReceivePhylePrize(user)
	local phyle = user:GetPhyle()
	if not phyle then
		return false, "您尚未选择势力，不能领取势力建设值奖励。"
	end
	local roleId = user:GetRoleId()
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj:GetPhylePrize() then
		return false, "您没有可以领取的建设值奖励。"
	end
	if phyleObj:IsPhylePrizeReceived(roleId) then
		return false, "您已经领取过本周的建设值奖励了。"
	end
	local prize = phyleObj:GetPhylePrize()
	local item = BASEITEM.NewItem(prize.goodsId, prize.num, {lock = 1})
	user:AddItem(USERBASE.PACKAGE, item)
	phyleObj:SetPhylePrizeReceived(roleId)
	FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_PHYLE_PRIZE, FUNCOPEN.STATE_INVALID)
	return true
end



--选择势力
local s_user_phyle = function(connection, data)
	if data ~= "random" and not IsValidPhyle(data) then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的势力类型：%s。", data or ""))
	end
	local minPhyle, minNum = data, nil
	for k, v in pairs(GetAllPhyleObjList()) do
		local num = v:GetMemberNum()
		if not minNum or num < minNum then
			minNum = num
			minPhyle = v:GetType()
		end
	end
	local selectType = data == "random" and minPhyle or data
	local obj = GetPhyleByType(selectType)
	if obj:GetMemberNum() - minNum >= MAX_DISTANCE_NUM then
		ERROR.TipMsg(connection, "您所选势力人数过多，请选择其他势力")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if phyle then
		return ERROR.TipMsg(connection, string.format("您已经加入了%s。", GetPhyleName(phyle)))
	end
	user:SetPhyle(selectType)
	obj:SetMemberNum(1)
	user:InitCityInfo()
	local voucher
	if data == "random" then
		voucher = VOUCHER_PRIZE
		user:SetVoucher(voucher)
	end
	user:SetFuncOpen(SETTING.FUNC_PHYLE_CHOOSE)
	user:Broad2ExNeighbor("phyle", nil, nil, true)
	connection:SendProtocol("c_user_phyle", {phyle = selectType, voucher = voucher})
end

local s_phyle_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	local sData = GetPhyleInfo(phyleObj, user)
	connection:SendProtocol("c_phyle_info", sData)
end

local s_phyle_degree = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetWeeklyPhyleDegreeList()
	connection:SendProtocol("c_phyle_degree", sData)
end

local s_phyle_devote = function(connection, data)
	if not CLSPHYLE.GetStructList()[data] then
		return ERROR.TipMsg(connection, string.format("无效的建筑类型:%s。", data))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
--[[	local ret, lv = FUNCOPEN.IsFuncOpen(user:GetLv(), FUNC_PHYLEDEVOTE)
	if not ret then
		return ERROR.TipMsg(connection, string.format("等级不足，该功能%d级开放", lv))
	end]]
	if user:GetPhyleDevoteTimes() >= user:GetPhyleDevoteMaxTimes() then
		return ERROR.TipMsg(connection, "您今日的捐献次数已经用完。")
	end
	if user:GetGold() < DEVOTE_NEED_GOLD then
		return ERROR.TipMsg(connection, "您的钱币不足。")
	end
	local structObj = phyleObj:GetStruct(data)
	user:SetPhyleDevoteTimes(1)
	user:SetGold(-DEVOTE_NEED_GOLD)
	structObj:SetExp(DEVOTE_NEED_GOLD)
	user:SetPhyleFeatGot(DEVOTE_ADD_FEAT)
	phyleObj:SetDegree(DEVOTE_ADD_DEGREE)
	connection:SendProtocol("c_phyle_devote", DEVOTE_ADD_FEAT)
	PROTOCOL.s_phyle_info(connection, "get")
end

local s_phyle_exchgList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
--[[	local ret, lv = FUNCOPEN.IsFuncOpen(user:GetLv(), FUNC_PHYLEDEVOTE)
	if not ret then
		return ERROR.TipMsg(connection, string.format("等级不足，该功能%d级开放", lv))
	end]]
	local munition = phyleObj:GetStruct("munition")
	local sData = {feat = user:GetPhyleFeat("enable")}
	sData.Lv = munition:GetLv()
	sData.list = munition:GetExchangeList()
	connection:SendProtocol("c_phyle_exchgList", sData)
end

local s_phyle_exchange = function(connection, data)
	data = tonumber(data)
	if not BASEITEM.GetItemById(data) then
		return ERROR.TipMsg(connection, string.format("无效的物品id：%s。", data))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	local munition = phyleObj:GetStruct("munition")
	local exchgInfo = munition:GetItemExchangeInfo(data)
	if not exchgInfo then
		return ERROR.TipMsg(connection, string.format("没有指定物品的兑换信息:%s。", data or ""))
	end
	if munition:GetLv() < exchgInfo.needLv then
		return ERROR.TipMsg(connection, "军资库等级不足，无法完成兑换。")
	end
	if user:GetPhyleFeat("enable") < exchgInfo.needFeat then
		return ERROR.TipMsg(connection, "您的军功不足。")
	end
	user:SetPhyleFeat("enable", -exchgInfo.needFeat)
	local item = BASEITEM.NewItem(data, 1, {lock = 1})
	user:AddItem(USERBASE.PACKAGE, item)
	connection:SendProtocol("c_phyle_exchange", {goodsId = data, feat = user:GetPhyleFeat("enable")})
end


local s_phyle_prayInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	local sData = {}
	sData.prayNum = user:GetPhylePrayMaxTimes() - user:GetPhylePrayTimes()
	sData.prayFeat = PRAY_NEED_FEAT
	sData.feat = user:GetPhyleFeat("enable")
	local prayGot = user:GetPhylePrayGot()
	sData.prize = prayGot.prize
	connection:SendProtocol("c_phyle_prayInfo", sData)
end

local s_phyle_pray = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	if user:GetPhylePrayTimes() >= user:GetPhylePrayMaxTimes() then
		return ERROR.TipMsg(connection, "您今日的祈祷次数已经用完。")
	end
	if user:GetPhyleFeat("enable") < PRAY_NEED_FEAT then
		return ERROR.TipMsg(connection, "您的军功不足。")
	end
	user:SetPhyleFeat("enable", -PRAY_NEED_FEAT)
	local altar = phyleObj:GetStruct("altar")
	user:SetPhylePrayTimes(1)
	local index, prize, grade = altar:GenPrayItem()
	user:SetPhylePrayGot(prize, grade)
	connection:SendProtocol("c_phyle_pray", {index = index, prize = prize, feat = user:GetPhyleFeat("enable"), prayNum = user:GetPhylePrayMaxTimes() - user:GetPhylePrayTimes()})
end

local s_phyle_prayRecv = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	local prayGot = user:GetPhylePrayGot()
	if not prayGot.prize then
		return ERROR.TipMsg(connection, "没有可领取的物品。")
	end
	local msg
	local info = BASEITEM.GetItemById(prayGot.prize.goodsId)
	if not info then
		return ERROR.TipMsg(connection, string.format("无效的物品id：%s。", prayGot.prize.goodsId or "nil"))
	end
	if info.type == "pic" then
		local type = MONEY.GetGoldTypeByPicId(prayGot.prize.goodsId)
		local moneyInfo = MONEY.GetGoldInfoByType(type)
		USERBASE.clsUser[moneyInfo.set](user, prayGot.prize.num)
		if (prayGot.prize.goodsId == 104002 and prayGot.prize.num == 500000) or 
			(prayGot.prize.goodsId == 104005 and prayGot.prize.num == 100) then
			msg = string.format("%d%s", prayGot.prize.num, moneyInfo.name)
		end
	else
		local item = BASEITEM.NewItem(prayGot.prize.goodsId, prayGot.prize.num, {lock = 1})
		if not user:IsItemCanAdd(USERBASE.PACKAGE, item) then
			return ERROR.TipMsg(connection, "您的背包已满。")
		end
		user:AddItem(USERBASE.PACKAGE, item)
		msg = string.format("【%s】×%d", item:GetName(), prayGot.prize.num)
	end
	if msg then
		CHAT.SendSysMsg("system", string.format("%s在军功抽奖中时获得了%s。", user:GetName(), msg))
	end
	user:SetPhylePrayGot()
	connection:SendProtocol("c_phyle_prayRecv", "true")
end

local s_phyle_totemBuff = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有选择势力。")
	end
	local phyleObj = GetPhyleByType(phyle)
	if not phyleObj then
		return ERROR.TipMsg(connection, string.format("无效的势力类型：%s。", phyle))
	end
	local totem = phyleObj:GetStruct("totem")
	local sData = totem:GetAttrAddRate()
	connection:SendProtocol("c_phyle_totemBuff", sData)
end

local s_phyle_bufferInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	local info = IsValidPhyle(phyle)
	if not info then
		return
	end
	local sData = {}
	sData.phyle = info.phyle
	sData.bufDesc = info.buf
	connection:SendProtocol("c_phyle_bufferInfo", sData)
end

local s_phyle_structInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您还没有加入势力")
	end
	local phyleObj = GetPhyleByType(phyle)
	local sData = {}
	local totem = phyleObj:GetStruct("totem")
	local altar = phyleObj:GetStruct("altar")
	local munition = phyleObj:GetStruct("munition")
	local rate = totem:GetAddRate()
	sData.totem = {}
	sData.totem.cur = totem:GetLv() * rate
	sData.totem.next = math.min((totem:GetLv() + 1), totem:GetMaxLv()) * rate
	sData.altar = {}
	sData.altar.cur = altar:GetMaxPrayTimes()
	sData.altar.next = altar:GetMaxPrayTimes(math.min(altar:GetLv() + 1, altar:GetMaxLv()))
	sData.munition = {}
	sData.munition.cur = munition:GetExchangeNum()
	sData.munition.next = munition:GetExchangeNum(math.min(munition:GetLv() + 1), munition:GetMaxLv())
	connection:SendProtocol("c_phyle_structInfo", sData)
end

local s_phyle_chose = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = USER.GetOnlineUser(connection.roleId)
	user:CheckNeedPhyle()
end

function __init__()
	
	RestoreAllPhyleData()
	
	PROTOCOL.s_user_phyle = s_user_phyle
	PROTOCOL.s_phyle_info = s_phyle_info
	PROTOCOL.s_phyle_degree = s_phyle_degree
	PROTOCOL.s_phyle_devote = s_phyle_devote
	
	PROTOCOL.s_phyle_exchgList = s_phyle_exchgList
	PROTOCOL.s_phyle_exchange = s_phyle_exchange
	
	PROTOCOL.s_phyle_prayInfo = s_phyle_prayInfo
	PROTOCOL.s_phyle_pray = s_phyle_pray
	PROTOCOL.s_phyle_prayRecv = s_phyle_prayRecv
	
	PROTOCOL.s_phyle_totemBuff = s_phyle_totemBuff

	PROTOCOL.s_phyle_bufferInfo = s_phyle_bufferInfo
	PROTOCOL.s_phyle_structInfo = s_phyle_structInfo
--	PROTOCOL.s_phyle_chose = s_phyle_chose
	CALLOUT.CallFre(SaveAllPhyleData, SAVE_PHYLE_TIME_LONG)
	CALLOUT.Weekly(getfenv(2), 2, 0, 0, RenewPhylePrize)
end
