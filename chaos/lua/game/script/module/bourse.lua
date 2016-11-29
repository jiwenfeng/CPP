local TIMELONG = 12 * 60 * 60		--上架时长
local FACTORAGE = 0.1				--手续费
local DEPOSIT = 10000				--押金
local BID_HERO_APTLV = 4			--可拍卖的武将资质等级

local AllBourseList = {__RestoreHook = true}
local path_bourse = "chaos.bourse"
local itemList = BASEITEM.GetAllItemList()

--上架最低价格
local PRICE_LIMIT = {
	["gold"] = 10,
	["ingot"] = 2,
}


function GetAllBourseList()
	local buf = {}
	for k, v in pairs(AllBourseList) do
		if k ~= "__RestoreHook" then
			buf[k] = v
		end
	end
	return buf
end

function GetIndentById(id)
	return AllBourseList[tostring(id)]
end

function RemoveIndentByRoleId(roleId)
	for k, v in pairs(AllBourseList) do
		if v.owner == roleId then
			RemoveOneIndent(k)
		end
	end
end

function AddOneItem(user, item, num, type, price)
	local buf = {}
	local indentId = STATS.GenId("bourse")
	buf.goodsId = item:GetId()
	buf.price = price
	buf.goldType = type
	buf.num = num
	buf.owner = user:GetRoleId()
	buf.endTime = os.time() + TIMELONG
	if item:GetClass() == "equip" then
		buf.save = item:GetSaveData()
	elseif item:GetClass() == "heroCard" then
		buf.save = item:GetHeroData()
	end
	AllBourseList[indentId] = buf
	SaveIndent(indentId)
	RunIndentCallOut(indentId)
	SendNewIndent(indentId)
	return indentId
end
--更新新订单信息
function SendNewIndent(indentId)
	local sData = GetIndentData(indentId)
	for k in pairs(USER.GetUserFocusListByType("bourse")) do
		local user = USER.GetOnlineUser(k)
		if not user or user:GetFocus().type ~= "bourse" then
			USER.SetUserFocusOn("bourse", k)
		else
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_bourse_new", sData)
			else
				USER.SetUserFocusOn("bourse", k)
			end
		end
	end
end
--订单下架 / 消失
function SendIndentRemove(indentId)
	for k in pairs(USER.GetUserFocusListByType("bourse")) do
		local user = USER.GetOnlineUser(k)
		if not user or user:GetFocus().type ~= "bourse" then
			USER.SetUserFocusOn("bourse", k)
		else
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_bourse_del", indentId)
			else
				USER.SetUserFocusOn("bourse", k)
			end
		end
	end
end

function GetIndentData(indentId)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	local info = itemList[indent.goodsId]
	local buf = {}
	buf.goodsId = indent.goodsId
	buf.price = indent.price
	buf.goldType = indent.goldType
	buf.num = indent.num
	buf.endTime = indent.endTime - os.time()
	buf.indentId = indentId
--	buf.owner = USER.GetUserNickById(indent.owner)
	buf.roleId = indent.owner
	if indent.save then
		local item = BASEITEM.NewItem(indent.goodsId, indent.num, {save = indent.save})
		buf.attr = item:GetFullAttr()
		item:Destroy()
	end
	return buf
end

function GetIndentList(roleId)
	local sData = {}
	for k, v in pairs(GetAllBourseList()) do
		if not roleId or v.owner == roleId then
			local buf = GetIndentData(k)
			table.insert(sData, buf)
		end
	end
	return sData
end

function RunIndentCallOut(indentId)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	local timelong = indent.endTime - os.time()
	if timelong <= 0 then			-- 服务器启动期间延迟处理
		return RemoveOneIndent(indentId)
	end
	CALLOUT.CallOut(RemoveOneIndent, timelong, indentId)
end

--订单过期移除
function RemoveOneIndent(indentId, flag)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	if indent.num > 0 then
		SendMailIndentCancel(indentId, flag)
	end
	local user = USER.GetOnlineUser(indent.owner)
	if user then
		user:SetIndentCount(-1)
		local focus = user:GetFocus()
		if focus.type == "bourse" then
			local con = LOGIN.GetConnection(indent.owner)
			if con and con.roleId == indent.owner then
				PROTOCOL.s_bourse_info(con, "get")
			end
		end
	else
		OFFLINE.SetOfflineData(indent.owner, "indent", -1)
	end
	AllBourseList[indentId] = nil
	DATA.DBRemove(path_bourse, {_id = indentId})
	SendIndentRemove(indentId)
end

function CancelIndent(user, indentId)
	local indent = GetIndentById(indentId)
	if not indent then
		return false, "没有该订单信息。"
	end
	if indent.owner ~= user:GetRoleId() then
		return false, "只能下架自己的订单"
	end
	RemoveOneIndent(indentId, true)
end

function Buy(user, indentId, num)
	local indent = GetIndentById(indentId)
	if not indent then
		return false, "没有该订单信息。"
	elseif indent.num <= 0 then
		return false, "该物品没有足够的数量"
	end
	local roleId = user:GetRoleId()
	if indent.owner == roleId then
		return false, "不能购买自己上架的物品"
	end
	num = num or indent.num
	local moneyInfo = MONEY.GetGoldInfoByType(indent.goldType)
	local cost = indent.price
	if USERBASE.clsUser[moneyInfo.get](user) < cost then
		return false, string.format("您的%s余额不足。", moneyInfo.name)
	end
	SendMailItemBought(indentId, user:GetRoleId())
	SendMailItemSold(indentId)
	USERBASE.clsUser[moneyInfo.set](user, -cost)
	indent.num = indent.num - num
	if indent.num <= 0 then
		RemoveOneIndent(indentId)
		return true
	end
	SaveIndent(indentId)
	return true
end

--发送订单售出邮件
function SendMailItemSold(indentId)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	local item = BASEITEM.NewItem(indent.goodsId, indent.num, {save = indent.save})
	local fee = math.ceil(indent.price * FACTORAGE)
	local income = indent.price - fee
	income = income <= 0 and 1 or income
	local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
	mail.title = "交易行物品售出成功"
	mail.content = string.format("亲，您上架的物品<font color = \"#FFFF00\">%s</font>已经成功售出。\
\n\t   成交价：%8d 元宝\n\t   手续费：<font color = \"#FF0000\">%8d</font> 元宝\n\t   共获得：<font color = \
\"#FFFF00\">%8d</font> 元宝\n\t 返还押金：%8d 钱币\n\n感谢您使用交易行系统。", item:GetName(), indent.price, fee, income, DEPOSIT)
	mail.attach = {}
	mail.attach.gold = DEPOSIT
	mail.attach.ingot = income
	MAIL.SendSysMail(indent.owner, mail)
	item:Destroy()
end
--发送订单购买邮件
function SendMailItemBought(indentId, roleId)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	local item = BASEITEM.NewItem(indent.goodsId, indent.num, {save = indent.save})
	local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
	mail.title = "交易行物品购买成功"
	mail.content = string.format("亲，恭喜您成功购买了物品<font color = \"#FFFF00\">%s</font>，请及时领取。", item:GetName())
	mail.attach = {}
	mail.attach.item = {}
	table.insert(mail.attach.item, {id = indent.goodsId, num = indent.num, save = indent.save})
	MAIL.SendSysMail(roleId, mail)
	item:Destroy()
end
--发送订单取消邮件
function SendMailIndentCancel(indentId, flag)
	local indent = GetIndentById(indentId)
	if not indent then
		return
	end
	local item = BASEITEM.NewItem(indent.goodsId, indent.num, {save = indent.save})
	if not item then
		return
	end
	local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
	mail.title = "交易行物品下架通知"
	mail.content = string.format("亲，您出售的物品<font color = \"#FFFF00\">%s</font>已经下架，请及时领取。", item:GetName())
	mail.attach = {}
	if not flag then
		mail.attach.gold = DEPOSIT
	end
	mail.attach.item = {}
	table.insert(mail.attach.item, {id = indent.goodsId, num = indent.num, save = indent.save})
	MAIL.SendSysMail(indent.owner, mail)
	item:Destroy()
end

function RestoreHook(Ret)
	if not Ret then
		print("restore bourse failed")
		return
	end
	for k in pairs(GetAllBourseList()) do
		RunIndentCallOut(k)
	end
end


function SaveIndent(indentId)
	local indent = GetIndentById(indentId)
	indent._id = indentId
	if not DATA.Write(path_bourse, {_id = indentId}, indent) then
		DATA.log(string.format("Save indent %s data failed: connect server Saved error!", indentId), "market.log")
	end
end

--加载交易行列表
local s_bourse_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn("bourse", connection.roleId, true)
	user:SetFocus("bourse")
	local sData = GetIndentList()
	connection:SendProtocol("c_bourse_list", sData)
end

local s_bourse_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.indentCount = user:GetIndentCount()
	sData.indentMax = user:GetMaxIndentNum()
	sData.ingotMin = PRICE_LIMIT["ingot"]
	connection:SendProtocol("c_bourse_info", sData)
end

local s_bourse_buy = function(connection, data)
	local indent = GetIndentById(data)
	if not indent then
		return ERROR.TipMsg(connection, "没有指定的订单信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = Buy(user, data)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_bourse_buy", data)
end

--上架
local s_bourse_sell = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.areaId = tostring(data.areaId)
	if not data.areaId then
		return ERROR.TipMsg(connection, "请选择上架的物品")
	end
	data.goldType = data.goldType or "ingot"
	if data.goldType ~= "ingot" then
		return ERROR.TipMsg(connection, "所有订单只能使用元宝交易。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local item = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	if not item then
		return ERROR.TipMsg(connection, "无效的物品信息。")
	end
	if item:IsLocked() == 1 then
		return ERROR.TipMsg(connection, "该物品被绑定，不能交易")
	end
	if item:GetMissionId() then
		return ERROR.TipMsg(connection, "该物品为任务物品，不能交易")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "请输入出售数量")
	elseif data.num > item:GetNum() then
		return ERROR.TipMsg(connection, "您背包中的物品数量不足。")
	end
	if user:GetGold() < DEPOSIT then
		return ERROR.TipMsg(connection, "您的钱币不足以支付押金，无法完成上架。")
	end
	if not user:IsIndentCanBeAdd() then
		return ERROR.TipMsg(connection, "您的订单数量已经达到上限了")
	end
	data.price = tonumber(data.price)
	if not data.price or data.price < PRICE_LIMIT[data.goldType] then
		return ERROR.TipMsg(connection, "出售价格不能低于限定的最低价格。")
	end
	local indentId = AddOneItem(user, item, data.num, data.goldType, data.price)
	user:SetGold(-DEPOSIT)
	user:DelItemByAreaId(USERBASE.PACKAGE, data.areaId, data.num)
	user:SetIndentCount(1)
	connection:SendProtocol("c_bourse_sell", "true")
--	PROTOCOL.s_bourse_info(connection, "get")
end

--下架
local s_bourse_cancel = function(connection, data)
	if not data then
		return TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local indent = GetIndentById(data)
	if not indent then
		return ERROR.TipMsg(connection, "没有该订单信息。")
	end
	if indent.owner ~= connection.roleId then
		return ERROR.TipMsg(connection, "只能下架自己的订单")
	end
	RemoveOneIndent(data, true)
	connection:SendProtocol("c_bourse_cancel", data)
end
---------------------------------------------------------

--可拍卖的武将列表
function GetBidableHeroList(user)
	local list = {}
	for k, v in pairs(user:GetHeroList()) do
		local aptLv = v:GetAptLv()
		if aptLv >= BID_HERO_APTLV and v:GetType() ~= "godHero" and v:GetWorkState() == "rest" then
			local buff = v:GetSimpleInfo()
			buff.skillList = nil
			buff.strength = v:GetStrength()
			buff.intellect = v:GetIntellect()
			buff.agility = v:GetAgility()
			buff.captain = v:GetCaptain()			
			buff.headId = v:GetHeadId()
			table.insert(list, buff)
		end
	end
	table.sort(list, function(a, b) return a.Lv > b.Lv end)
	return list
end

local s_bourse_heroList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetBidableHeroList(user)
	connection:SendProtocol("c_bourse_heroList", sData)
end

local s_bourse_heroReg = function(connection, data)	
	if not data.heroId then
		return ERROR.TipMsg(connection, "请选择要拍卖的战魂。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local hero = user:GetHero(data.heroId)
	if not hero then
		return ERROR.TipMsg(connection, "没有找到指定的战魂信息。")
	end
	local aptLv = hero:GetAptLv()
	if not BASEITEM.GetHeroCarIdByAptLv(aptLv) then
		return ERROR.TipMsg(connection, "卓越及以上品质的战魂才能参与拍卖。")
	end
	if hero:GetType() == "godHero" then
		return ERROR.TipMsg(connection, "神魂不能参与拍卖。")
	end
	if not hero:IsNoEquipWorn() then
		return ERROR.TipMsg(connection, "请先卸下战魂的装备。")
	end
	data.price = tonumber(data.price)
	if not data.price or data.price <= 0 then
		return ERROR.TipMsg(connection, "请输入战魂的元宝售价。")
	end
	if data.price < PRICE_LIMIT["ingot"] then
		return ERROR.TipMsg(connection, "出售价格不能低于限定的最低价格。")
	end
	if user:GetGold() < DEPOSIT then
		return ERROR.TipMsg(connection, "您的钱币不足以支付押金，无法完成上架。")
	end
	if not user:IsIndentCanBeAdd() then
		return ERROR.TipMsg(connection, "您的订单数量已经达到上限了")
	end
	if hero:GetWorkState() ~= "rest" then
		return ERROR.TipMsg(connection, "只有空闲状态的战魂才能上架")
	end
	local saveData = hero:GetSaveData()
	saveData.total = nil
	local item = BASEITEM.NewItem(BASEITEM.GetHeroCarIdByAptLv(aptLv), 1, {save = saveData})
	AddOneItem(user, item, 1, "ingot", data.price)
	user:SetGold(-DEPOSIT)
	user:DelHero(data.heroId)
	user:SetIndentCount(1)
	connection:SendProtocol("c_bourse_heroReg", data.heroId)
--	PROTOCOL.s_bourse_info(connection, "get")
end

local s_bourse_myList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {list = {}}
--	sData.indentCount = user:GetIndentCount()
	sData.indentMax = user:GetMaxIndentNum()
--	sData.goldMin = PRICE_LIMIT["gold"]
	sData.ingotMin = PRICE_LIMIT["ingot"]
	sData.list = GetIndentList(connection.roleId)
	connection:SendProtocol("c_bourse_myList", sData)
end

local s_bourse_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn("bourse", connection.roleId)
	user:SetFocus()
	connection:SendProtocol("c_bourse_leave", "true")
end


function __init__()
	DATA.Restore(path_bourse, AllBourseList)
	
	PROTOCOL.s_bourse_list = s_bourse_list
	PROTOCOL.s_bourse_leave = s_bourse_leave
	PROTOCOL.s_bourse_info = s_bourse_info
	
	PROTOCOL.s_bourse_buy = s_bourse_buy
	PROTOCOL.s_bourse_sell = s_bourse_sell
	PROTOCOL.s_bourse_cancel = s_bourse_cancel
	
--	PROTOCOL.s_bourse_myList = s_bourse_myList
	PROTOCOL.s_bourse_heroList = s_bourse_heroList
	PROTOCOL.s_bourse_heroReg = s_bourse_heroReg
end
