--仙器系统

local ExchangeList = {}
local FairyDevClassifyList = {}
local MergeRuleList = {}

local SkillList = {}		--仙器生成技能表

local REMERGE_COST = 500		--融合所需元宝

function ClassifyFairyDevice()
	FairyDevClassifyList = {}
	for k, v in pairs(BASEITEM.GetAllItemList()) do 
		if v.class == "fairyDev" and v.isRefinable then
			FairyDevClassifyList[v.quality] = FairyDevClassifyList[v.quality] or {}
			table.insert(FairyDevClassifyList[v.quality], k)
		end
	end
end

function GetExchangeInfoById(id)
	return ExchangeList[id]
end

function GetExchangeList()
	return ExchangeList
end

--格式化兑换列表
function GetFormatedExchangeList()
	local sData = {}
	for k, v in pairs(GetExchangeList()) do
	local devObj = BASEITEM.NewItem(k, 1)
		local buff = devObj:GetFullAttr()
		buff.skillName = nil
		buff.skillLv = nil
		buff.num = nil
		buff.needDebris = v
		table.insert(sData, buff)
		devObj:Destroy()
	end
	table.sort(sData, function(a, b) return tonumber(a.goodsId) < tonumber(b.goodsId) end)
	return sData
end

function ClassifyAllSkillList()
	local list = CLSPET.GetAllPetInfoList()
	for k, v in pairs(list) do
		table.insert(SkillList, v.skillId)
	end
end

function GetAllSkillList()
	return SkillList
end

function GetDeviceList()
	return FairyDevClassifyList
end

function GetMergeRuleList(quality)
	return MergeRuleList[quality]
end

function FormatTripodList(user)
	local sData = {}
	for k, v in ipairs(TRIPOD.GetTripodList()) do 
		local buf = {}
		buf.trpdId = k
		buf.name = v:GetName()
		buf.needGold = v:GetGoldCost()
		buf.needIngot = v:GetIngotCost()
		buf.state = user:GetTripodState(k)
		buf.needVipLv = v:GetNeedVipLv()
		table.insert(sData, buf)
	end
	return sData
end

--一键炼器
function AutoRefine(user)
	local sData = {}
	while not user:IsFairyBagFull(USERBASE.FAIRY_BAG) do
		local trpId = user:GetCurTripod()
		local tripod = TRIPOD.GetTripodById(trpId)
		local ret, msg = tripod:Refine(user, nil, true)
		table.insert(sData, {ret = ret, data = msg})
		if not ret then
			break
		end
	end
	return sData
end

--拾取
function Pickup(user, pos, flag)
	local devObj = user:GetFairyDevByPos(USERBASE.FAIRY_PANEL, pos)
	if not devObj then
		return false, "无效的仙器位置。"
	end
	if devObj:GetQuality() <= 1 then
		return false, "该仙器不可拾取。"
	end
	if user:IsFairyBagFull(USERBASE.FAIRY_BAG) then
		return false, "您的仙器背包已满。"
	end
	user:AddFairyDev(USERBASE.FAIRY_BAG, devObj, true)
	user:DelFairyDevByPos(USERBASE.FAIRY_PANEL, pos, flag)
	return true
end

--一键拾取
function AutoPickup(user)
	local num = 0
	local buff = {[1] = {}, [2] = {}, [3] = {}, [4] = {}, [5] = {}}
	for k, v in pairs(user:GetFairyDevList(USERBASE.FAIRY_PANEL)) do
		local quality = v:GetQuality()
		if quality > 1 then
			table.insert(buff[quality], k)
			num = num + 1
		end
	end
	if num <= 0 then
		return false, "没有可拾取的仙器。"
	end
	for i = 5, 1, -1 do
		for _, v in ipairs(buff[i]) do
			local ret, msg = Pickup(user, v, true)
			if not ret then
				return ret, msg
			end
		end
	end
	return true
end

--售出仙器
function SellFairyDev(user, pos, flag)
	local devObj = user:GetFairyDevByPos(USERBASE.FAIRY_PANEL, pos)
	if not devObj then
		return false, "无效的仙器位置。"
	end
	if devObj:GetQuality() >= 5 then
		return false, "橙色仙器不可卖出。"
	end
	local price = devObj:GetPrice()
	user:DelFairyDevByPos(USERBASE.FAIRY_PANEL, pos, flag)
	user:SetGold(price)
	return price
end

--一键售出
function AutoSell(user, quality)
	local buff = {}
	for k, v in pairs(user:GetFairyDevList(USERBASE.FAIRY_PANEL)) do
		if v:GetQuality() <= quality then
			table.insert(buff, k)
		end
	end
	if #buff <= 0 then
		return false, "没有对应品级的仙器。"
	end
	local price = 0
	for _, v in ipairs(buff) do
		local ret, msg = SellFairyDev(user, v, true)		
		if not ret then
			return ret, msg
		end
		price = price + ret
	end
	return price
end

--移动仙器（移动时发生位置变化或吞噬行为）
function MoveFairyDev(user, type_src, pos_src, type_dst, pos_dst)
	local devObj_src = user:GetFairyDevByPos(type_src, pos_src)
	if not devObj_src then
		return false, "无效的仙器位置"
	end
	local devObj_dst = user:GetFairyDevByPos(type_dst, pos_dst)
	if not devObj_dst then
		if type_src == USERBASE.FAIRY_EQUIP and type_dst == USERBASE.FAIRY_BAG then
			user:UnfixFairyDev(pos_src)
		elseif type_src == USERBASE.FAIRY_BAG and type_dst == USERBASE.FAIRY_EQUIP then
			user:WearFairyDev(pos_dst, pos_src)
		else
			user:AddFairyDevByPos(type_dst, pos_dst, devObj_src)
		end
		user:DelFairyDevByPos(type_src, pos_src)
	else
		Devour(user, type_dst, pos_dst, type_src, pos_src)
	end
	return true
end

--仙器吞噬（main表示吞噬主体，extra表示被吞噬物）
function Devour(user, type_main, pos_main, type_extra, pos_extra, flag)
	local devObj_main = user:GetFairyDevByPos(type_main, pos_main)
	local devObj_extra = user:GetFairyDevByPos(type_extra, pos_extra)
	if not devObj_main or not devObj_extra then
		return false, "没有仙器可以吞噬。"
	end
	if devObj_main:GetLv() >= devObj_main:GetMaxLv() then
		return false, "该仙器已经到达最高等级。"
	end
	if devObj_main:IsLocked() or devObj_extra:IsLocked() then
		return false, "锁定的仙器不能吞噬或被吞噬。"
	end
	local exp = devObj_extra:GetAddExp()
	local curLv = devObj_main:GetLv()
	devObj_main:SetExp(exp)
	local newLv = devObj_main:GetLv()
	user:DelFairyDevByPos(type_extra, pos_extra, flag)
	if not flag then
		user:SendFairyDevUpdate(type_main, pos_main)
	end
	return exp, curLv ~= newLv and newLv or nil
end

--获取吞噬主体
function GetDevourMainPos(user, type)
	local list = user:GetFairyDevList(type)
	local buff = {}
	local temp
	local pos
	for k, v in pairs(list) do
		if not v:IsLocked() and v:GetLv() < v:GetMaxLv() and v:GetQuality() > 1 then
			if pos then
				temp = user:GetFairyDevByPos(type, pos)
				if (v:GetQuality() > temp:GetQuality()) or (v:GetQuality() == temp:GetQuality() and v:GetLv() > temp:GetLv()) or (v:GetQuality() == temp:GetQuality() and v:GetLv() == temp:GetLv() and v:GetExp() > temp:GetExp()) or (v:GetQuality() == temp:GetQuality() and v:GetLv() == temp:GetLv() and v:GetExp() == temp:GetExp() and tonumber(k) < tonumber(pos)) then
					pos = k
				end
			end
			if not pos then
				pos = k
			end
		end
	end
	return pos
end

--计算一键吞噬所得经验
function CalAutoDevourExp(user, type)
	local pos = GetDevourMainPos(user, type)
	local exp = 0
	for k, v in pairs(user:GetFairyDevList(type)) do
		if k ~= pos and not v:IsLocked() then
			exp = exp + v:GetAddExp()
		end
	end
	return exp
end

--一键吞噬
function AutoDevour(user, type)
	local pos = GetDevourMainPos(user, type)
	local buff = {}
	for k, v in pairs(user:GetFairyDevList(type)) do
		if k ~= pos and not v:IsLocked() then
			table.insert(buff, k)
		end
	end
	table.sort(buff, function(a, b)
		local obj1 = user:GetFairyDevByPos(type, a)
		local obj2 = user:GetFairyDevByPos(type, b)
		return 
		obj1:GetQuality() < obj2:GetQuality() or 
		(obj1:GetQuality() == obj2:GetQuality() and obj1:GetLv() < obj2:GetLv()) or
		(obj1:GetQuality() == obj2:GetQuality() and obj1:GetLv() == obj2:GetLv() and obj1:GetAddExp() < obj2:GetAddExp())
	end)
	local devObj_main = user:GetFairyDevByPos(type, pos)
	local curLv = devObj_main:GetLv()
	local sData = {exp = 0}
	for _, v in ipairs(buff) do
		local ret, msg = Devour(user, type, pos, type, v, true)
		if not ret then
			sData.msg = msg
			break
		end
		sData.exp = sData.exp + ret
	end
	local newLv = devObj_main:GetLv()
	user:SendFairyDevUpdate(type, pos)
	if newLv ~= curLv then
		sData.Lv = newLv
	end
	return sData
end

function ProDevourCoroutine(conn, user, data, auto)
	local ret = SendDevourConfirm(conn, user, data, auto)
	if ret then
		if auto then
			local sData = AutoDevour(user, data.type)
			conn:SendProtocol("c_fairyDev_autoDevour", sData)
			PROTOCOL.s_fairyDev_list(conn, data.type)
		else
			local ret, msg = Devour(user, data.type_dst, data.pos_dst, data.type_src, data.pos_src)
			if not ret then
				return conn:SendProtocol("c_fairyDev_devour", {result = 0, msg = msg})
			end
			conn:SendProtocol("c_fairyDev_devour", {result = 1, exp = ret, Lv = msg})
		end
	end
end

function SendDevourConfirm(conn, user, data, auto)
	local co = coroutine.running()
	user:SetDevourCort(co)
	local sData = {}
	if auto then
		sData.auto = 1
		local pos = GetDevourMainPos(user, data.type)
		sData.pos = pos
		sData.exp = CalAutoDevourExp(user, data.type)
		local devObj = user:GetFairyDevByPos(data.type, pos)
		sData.Lv = devObj:CanBeUpgrade(sData.exp)
	else
		sData.auto = 0
		sData.pos = data.pos_dst
		local devObj_src = user:GetFairyDevByPos(data.type_src, data.pos_src)
		sData.exp = devObj_src:GetAddExp()
		local devObj_dst = user:GetFairyDevByPos(data.type_dst, data.pos_dst)
		sData.Lv = devObj_dst:CanBeUpgrade(sData.exp)
	end
	conn:SendProtocol("c_fairyDev_confirm", sData)
	return coroutine.yield(co)
end

function ProDevourConfirm(user)
	local co = user:GetDevourCort()
	if co and coroutine.status(co) == "suspended" then
		coroutine.resume(co, 1)
		return
	end
end

function GetFormatedFairyDevList(user, type)
	local sData = {type = type, list = {}}
	for i = 1, user:GetFairyGridMax(type) do
		local devObj = user:GetFairyDevByPos(type, tostring(i))
		if devObj then
			local buff = devObj:GetFullAttr()
			buff.pos = tostring(i)
			if USERBASE.FAIRY_PANEL ~= type then
				buff.price = nil
			end
			table.insert(sData.list, buff)
		end
	end
	if type == USERBASE.FAIRY_BAG then
		sData.priceAdd = user:GetFairyDevGridUnlockAdd()
		sData.unlockNum = user:GetFairyGridMax(type)
		sData.maxNum = user:GetFairyGridMax(type, true)
		sData.firstCost = user:CalFairyGridUnLockPrice(type, 1)
		sData.autoUse = user:GetSetting(SETTING.AUTO_USE_DEVOUR_PRO) or 0
	elseif type == USERBASE.FAIRY_EQUIP then
		sData.unlockNum = user:GetFairyGridMax(type)
		sData.maxNum = user:GetFairyGridMax(type, true)
	end
	return sData
end

--生成融合仙器
function GenMergeDevId(quality, id)
	local rule = GetMergeRuleList(quality)
	local list = {}
	local sum = 0
	for _, v in ipairs(rule) do
		if not id or (v.id ~= id) then
			table.insert(list, v)
			sum = sum + v.rate
		end
	end
	if rule then
		local ran = math.random(0, sum * 1000) / 1000
		local rate = 0
		for k, v in ipairs(list) do
			rate = rate + v.rate
			if rate >= ran then
				return v.id
			end
		end
	end
end

--仙器融合
function FairyDevMerge(user, list)
	if user:GetTotalIngot() < REMERGE_COST then
		return false, "您的元宝不足，不能完成融合。"
	end
	local sum = 0
	for k, v in ipairs(list) do
		local fairyDev = user:GetFairyDevByPos(USERBASE.FAIRY_BAG, tostring(v))
		if not fairyDev then
			return false, string.format("没有指定的仙器信息：%s.", tostring(v))
		end
		if not fairyDev:IsFusable() then
			return false, string.format("指定的位置%s上的仙器不能进行融合。", tostring(v))
		end
	end
	for k, v in ipairs(list) do
		local fairyDev = user:GetFairyDevByPos(USERBASE.FAIRY_BAG, v)
		sum = sum + fairyDev:GetQuality()
		user:DelFairyDevByPos(USERBASE.FAIRY_BAG, v)
	end
	local rule = GetMergeRuleList(sum)
	if not rule then
		return false, "指定的仙器不能融合。"
	end
	local devId = GenMergeDevId(sum)
	if not devId then
		return false, "仙器融合失败。"
	end
	user:SetTotalIngot(-REMERGE_COST)
	local devObj = BASEITEM.NewItem(devId, 1)
	DATA.log(string.format("id = %d,quality = %d, sum = %d, lv = %d", devObj:GetId(), devObj:GetQuality(), sum, devObj:GetLv()), "test")
	user:SetMergeFairyDev(devObj, sum)
	return true
end

function KeepMerged(user)
	local info = user:GetMergeFairyDev()
	if not info then
		return false, "没有未收取的融合仙器。"
	end
	user:AddFairyDev(USERBASE.FAIRY_BAG, info.fairyDev, true)
	user:SetMergeFairyDev()
	return true
end

--重炼
function ReMerge(user)
	local info = user:GetMergeFairyDev()
	if not info then
		return false, "没有可重炼的融合仙器。"
	end
	local devId = GenMergeDevId(info.quality, info.fairyDev:GetId())
	if not devId then
		return false, "仙器融合失败。"
	end
	local devObj = BASEITEM.NewItem(devId, 1)
	user:SetMergeFairyDev(devObj, info.quality)
	return true
end

--检查是否有相同类型的仙器存在
function IsSameFairyDevExist(user, type)
	for k, v in pairs(user:GetFairyDevList(USERBASE.FAIRY_EQUIP)) do
		if v:GetType() == type then
			return true
		end
	end
	return false
end

function Decomposition(user, pos)
	local obj = user:GetFairyDevByPos(USERBASE.FAIRY_BAG, pos)
	if not obj then
		return false, "该位置没有仙器"
	end
	if obj:GetQuality() < 5 then
		return false, "只有橙色的仙器才能分解"
	end
	if obj:IsLocked() then
		return false, "锁定状态的仙器不能分解"
	end
	local num = 1
	if not obj:IsRefinable() then
		num = 2
	end
	user:DelFairyDevByPos(USERBASE.FAIRY_BAG, pos)
	user:SetDebris(num)
	return true
end

--仙器列表
local s_fairyDev_list = function(conn, data)
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if not user:IsFuncOpen(SETTING.FUNC_FAIRYDEV) then
		return ERROR.TipMsg(conn, "该功能尚未开启。")
	end
	data = tonumber(data)
	if not user:GetFairyDevList(data) then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatedFairyDevList(user, data)
	conn:SendProtocol("c_fairyDev_list", sData)
end

--炼器鼎列表
local s_fairyDev_tripodList = function(conn, data)
	if data ~= "get" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if not user:IsFuncOpen(SETTING.FUNC_FAIRYDEV) then
		return ERROR.TipMsg(conn, "该功能尚未开启。")
	end
	local sData = {debris = user:GetDebris()}
	sData.minVipLv = 4
	sData.list = FormatTripodList(user)
	conn:SendProtocol("c_fairyDev_tripodList", sData)
end

--炼器
local s_fairyDev_refine = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	data.trpdId = tonumber(data.trpdId)
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local tripod = TRIPOD.GetTripodById(data.trpdId)
	if not tripod then
		return ERROR.ErrorMsg(conn, "无效的鼎编号")
	end
	if data.isIngot == 1 and data.noremind == 1 then
		user:SetSetting(SETTING.REFINE_USE_INGOT_NO_REMIND, 1)
	end
	local ret, msg = tripod:Refine(user, data.isIngot == 1)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	local sData = {trpdId = data.trpdId}
	sData.goodsId = msg.fairyDevId
	sData.active = msg.active
	conn:SendProtocol("c_fairyDev_refine", sData)
end

--一键炼器
local s_fairyDev_autoRefine = function(conn, data)
	if data ~= "set" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if user:IsFairyBagFull(USERBASE.FAIRY_BAG) then
		return ERROR.TipMsg(conn, "仙器空间已满。")
	end
	if not VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_AUTO_REFINE) then
		return ERROR.TipMsg(conn, "您VIP的等级不足，不能使用一键炼器功能")
	end
	local sData = AutoRefine(user)
	conn:SendProtocol("c_fairyDev_autoRefine", sData)
end

--拾取
local s_fairyDev_pick = function(conn, data)
--[[	local user = assert(USER.GetOnlineUser(conn.roleId))
	local ret, msg = Pickup(user, data)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_pick", data)]]
end

--一键拾取	
local s_fairyDev_autoPick = function(conn, data)
--[[	if data ~= "set" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local ret, msg = AutoPickup(user)
	conn:SendProtocol("c_fairyDev_autoPick", {result = ret, msg = msg})]]
end

--售出
local s_fairyDev_sell = function(conn, data)
--[[	local user = assert(USER.GetOnlineUser(conn.roleId))
	data = tostring(data)
	local ret, msg = SellFairyDev(user, data)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_sell", data)]]
end

--一键售出
local s_fairyDev_autoSell = function(conn, data)
--[[	data = tonumber(data)
	if not FairyDevClassifyList[data] then
		return ERROR.TipMsg(conn, "无效的仙器品质。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local ret, msg = AutoSell(user, data)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_autoSell", {quality = data, price = ret})
	PROTOCOL.s_fairyDev_list(conn, USERBASE.FAIRY_PANEL)]]
end

--吞噬
local s_fairyDev_devour = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if not data.type_src or not data.pos_src or not data.type_dst or not data.pos_dst then
		return ERROR.TipMsg(conn, "请选择要吞噬的仙器。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local dev = user:GetFairyDevByPos(data.type_dst, data.pos_dst)
	if not dev then
		return ERROR.TipMsg(conn, "请选择吞噬的仙器")
	end
	if dev:GetQuality() == 1 then
		return ERROR.TipMsg(conn, "白色仙器只能被吞噬")
	end
	local function func()
		return ProDevourCoroutine(conn, user, data)
	end
	local co = coroutine.create(func)
	return coroutine.resume(co)
end

--一键吞噬
local s_fairyDev_autoDevour = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	data.type = data.type or USERBASE.FAIRY_BAG
	if data.type ~= USERBASE.FAIRY_BAG then
		return ERROR.TipMsg(conn, "只能在仙器背包界面使用一键吞噬。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if not GetDevourMainPos(user, data.type) then
		return ERROR.TipMsg(conn, "至少要有1个未满级的非锁定仙器才能使用一键吞噬。")
	end
	if CalAutoDevourExp(user, data.type) <= 0 then
		return ERROR.TipMsg(conn, "没有可吞噬的仙器。")
	end
	local function func()
		return ProDevourCoroutine(conn, user, data, true)
	end
	local co = coroutine.create(func)
	return coroutine.resume(co)
end

--确认吞噬
local s_fairyDev_confirm = function(conn, data)
	if data ~= "set" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	return ProDevourConfirm(user)
end

--移动仙器
local s_fairyDev_move = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if not data.type_src or not data.pos_src or not data.type_dst or not data.pos_dst then
		return ERROR.TipMsg(conn, "请选择要移动的仙器。")
	end
	if data.type_src == USERBASE.FAIRY_PANEL or data.type_dst == USERBASE.FAIRY_PANEL then
		return ERROR.TipMsg(conn, "不能在炼器界面移动仙器。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local fairyDev_src = user:GetFairyDevByPos(data.type_src, data.pos_src)
	if not fairyDev_src then
		return ERROR.TipMsg(conn, "没有指定位置的仙器信息。")
	end
	local fairyDev_dst = user:GetFairyDevByPos(data.type_dst, data.pos_dst)
	if not fairyDev_dst then
		if data.type_src == USERBASE.FAIRY_EQUIP and data.type_dst == USERBASE.FAIRY_BAG then
			return PROTOCOL.s_fairyDev_unfix(conn, data.pos_src)
		elseif data.type_src == USERBASE.FAIRY_BAG and data.type_dst == USERBASE.FAIRY_EQUIP then
			local buff = {targetPos = data.pos_dst, type = data.type_src, pos = data.pos_src}
			return PROTOCOL.s_fairyDev_wear(conn, buff)
		end
	else
		return PROTOCOL.s_fairyDev_devour(conn, data)
	end
	local ret, msg = MoveFairyDev(user, data.type_src, data.pos_src, data.type_dst, data.pos_dst)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_move", data)
end

--装备仙器
local s_fairyDev_wear = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if not data.pos then
		return ERROR.TipMsg(conn, "请选择要装备的仙器。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if not data.targetPos then
		if table.size(user:GetFairyDevList(USERBASE.FAIRY_EQUIP)) >= user:GetFairyGridMax(USERBASE.FAIRY_EQUIP) then
			return ERROR.TipMsg(conn, "您已经没有位置可以装备新的仙器了。")
		end
	elseif tonumber(data.targetPos) > user:GetFairyGridMax(USERBASE.FAIRY_EQUIP) then
		return ERROR.TipMsg(conn, "指定的位置尚未开启。")
	end
	local fairyDev = user:GetFairyDevByPos(USERBASE.FAIRY_BAG, data.pos)
	if not fairyDev then
		return ERROR.TipMsg(conn, "没有指定的仙器信息。")
	end
--	if fairyDev:IsLocked() then
--		return ERROR.TipMsg(conn, "不能装备锁定的仙器。")
--	end
	if fairyDev:GetQuality() == 1 then
		return ERROR.TipMsg(conn, "不能装备白色品质的仙器")
	end
	if IsSameFairyDevExist(user, fairyDev:GetType()) then
		return ERROR.TipMsg(conn, "不能装备多个相同类型的仙器。")
	end
	local ret, msg = user:WearFairyDev(data.targetPos, data.pos)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_wear", data)
end

--卸下仙器
local s_fairyDev_unfix = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if not data.targetPos then
		return ERROR.TipMsg(conn, "请选择要卸下的仙器。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if user:IsFairyBagFull(USERBASE.FAIRY_BAG) then
		return ERROR.TipMsg(conn, "您的仙器背包已满。")
	end
	local ret, msg = user:UnfixFairyDev(data.targetPos, data.pos)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_unfix", data)
end

--锁定/解锁仙器
local s_fairyDev_lock = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if not data.type or not data.pos then
		return ERROR.TipMsg(conn, "请选择要操作的仙器。")
	end
	if data.lock ~= 0 and data.lock ~= 1 then
		return ERROR.TipMsg(conn, "请输入仙器的锁定状态。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local fairyDev = user:GetFairyDevByPos(data.type, data.pos)
	if not fairyDev then
		return ERROR.TipMsg(conn, "没有指定的仙器信息。")
	end
	local state = fairyDev:IsLocked()
	if not state and data.lock == 0 then
		return ERROR.TipMsg(conn, "该仙器未被锁定。")
	elseif state and data.lock == 1 then
		return ERROR.TipMsg(conn, "该仙器已经锁定。")
	end
	fairyDev:SetLocked(data.lock == 1)
	conn:SendProtocol("c_fairyDev_lock", {type = data.type, pos = data.pos, lock = fairyDev:IsLocked() and 1 or 0})
end

local s_fairyDev_exchgList = function(conn, data)
	if data ~= "get" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local sData = {}
	sData.list = GetFormatedExchangeList()
	sData.debris = user:GetDebris()
	conn:SendProtocol("c_fairyDev_exchgList", sData)
end

local s_fairyDev_exchange = function(conn, data)
	data = tonumber(data)
	local num = GetExchangeInfoById(data)
	if not num then
		return ERROR.TipMsg(conn, "没有指定的兑换信息。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if user:GetDebris() < num then
		return ERROR.TipMsg(conn, string.format("您还缺少%d个碎片，无法完成兑换。", num - user:GetDebris()))
	end
	if user:IsFairyBagFull(USERBASE.FAIRY_BAG) then
		return ERROR.TipMsg(conn, "您的仙器背包已满。")
	end
	local fairyDev = BASEITEM.NewItem(data, 1)
	user:AddFairyDev(USERBASE.FAIRY_BAG, fairyDev)
	user:SetDebris(-num)
	conn:SendProtocol("c_fairyDev_exchange", {goodsId = data, debris = user:GetDebris()})
end

local s_fairyBag_unLock = function(conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(conn, "请选择要解锁的格子")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local price = user:CalFairyGridUnLockPrice(USERBASE.FAIRY_BAG, data.num)
	data.price = tonumber(data.price)
	if data.price ~= price then
		return ERROR.TipMsg(conn, string.format("开%d个格子的费用计算错误", data.num))
	end
	local goldInfo = MONEY.GetGoldInfoByType("ingot")
	if USERBASE.clsUser[goldInfo.get](user) < price then
		return ERROR.TipMsg(conn, string.format("您的%s不足，请充值。", goldInfo.name))
	end
	user:SetFairyGridMax(USERBASE.FAIRY_BAG, data.num)
	USERBASE.clsUser[goldInfo.set](user, -price)
	local sData = {num = data.num}
	sData.firstCost = user:CalFairyGridUnLockPrice(USERBASE.FAIRY_BAG, 1)
	conn:SendProtocol("c_fairyBag_unLock", sData)
end

local s_fairyDev_merge = function (conn, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	if #data ~= 2 then
		return ERROR.TipMsg(conn, "请放入要进行融合的2件仙器。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local ret, msg = FairyDevMerge(user, data)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	local mergeInfo = user:GetMergeFairyDev()
	conn:SendProtocol("c_fairyDev_merge", mergeInfo.fairyDev:GetFullAttr())
end

local s_fairyDev_keep = function(conn, data)
	if data ~= "set" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local ret, msg = KeepMerged(user)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_fairyDev_keep", "true")
end

local s_fairyDev_reMerge = function(conn, data)
	if data ~= 1 and data ~= 0 then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	if data == 1 then
		user:SetSetting(SETTING.REMERGE_USE_INGOT_NO_REMIND, 1)
	end
	local ret, msg = ReMerge(user)
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	local mergeInfo = user:GetMergeFairyDev()
	conn:SendProtocol("c_fairyDev_reMerge", mergeInfo.fairyDev:GetFullAttr())
end

local s_fairyDev_decomposition = function(connection, data)
	if not data then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = Decomposition(user, data)
	if not ret then
		return ERROR.ErrorMsg(connection, msg)
	end
	connection:SendProtocol("c_fairyDev_decomposition", user:GetDebris())
end

function __init__()

	local EXCHANGE = Import("autocode/fairyDeviceExchange.lua")
	ExchangeList = EXCHANGE.GetTable()

	local MERGERULE = Import("autocode/mergeRuleList.lua")
	MergeRuleList = MERGERULE.GetTable()
	
	ClassifyFairyDevice()
	ClassifyAllSkillList()

	PROTOCOL.s_fairyDev_list = s_fairyDev_list
	PROTOCOL.s_fairyDev_tripodList = s_fairyDev_tripodList
	
	PROTOCOL.s_fairyDev_refine = s_fairyDev_refine
	PROTOCOL.s_fairyDev_autoRefine = s_fairyDev_autoRefine
	PROTOCOL.s_fairyDev_pick = s_fairyDev_pick
	PROTOCOL.s_fairyDev_autoPick = s_fairyDev_autoPick
	PROTOCOL.s_fairyDev_sell = s_fairyDev_sell
	PROTOCOL.s_fairyDev_autoSell = s_fairyDev_autoSell
	
	PROTOCOL.s_fairyDev_devour = s_fairyDev_devour
	PROTOCOL.s_fairyDev_autoDevour = s_fairyDev_autoDevour
	PROTOCOL.s_fairyDev_confirm = s_fairyDev_confirm
	PROTOCOL.s_fairyDev_move = s_fairyDev_move
	
	PROTOCOL.s_fairyDev_wear = s_fairyDev_wear
	PROTOCOL.s_fairyDev_unfix = s_fairyDev_unfix
	PROTOCOL.s_fairyDev_lock = s_fairyDev_lock
	PROTOCOL.s_fairyBag_unLock = s_fairyBag_unLock
	
	PROTOCOL.s_fairyDev_exchgList = s_fairyDev_exchgList
	PROTOCOL.s_fairyDev_exchange = s_fairyDev_exchange	
	PROTOCOL.s_fairyDev_merge = s_fairyDev_merge
	PROTOCOL.s_fairyDev_keep = s_fairyDev_keep
	PROTOCOL.s_fairyDev_reMerge = s_fairyDev_reMerge
	PROTOCOL.s_fairyDev_decomposition = s_fairyDev_decomposition
end
