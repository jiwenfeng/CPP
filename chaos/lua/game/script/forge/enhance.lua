--锻造系统 属性增强(enhance 强化	upgrade 进阶	inject 注灵		promote 提灵	recoin 重铸)

--锻造消耗
local AllForgeInfoList = {}

function GetInjectAttrList(quality)
	return AllForgeInfoList[quality].attr
end

--获取锻造消耗
function GetForgeCost(type, quality)
	if not AllForgeInfoList[quality] then return 0 end
	return AllForgeInfoList[quality][type]
end

--更新装备信息
function SendEquipUpdate(user, heroId, areaId)
	local equip, obj
	if heroId then
		if heroId == "user" or heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(areaId)
			obj = user
		else
			local hero = assert(user:GetHero(heroId))
			equip = hero:GetEquipByPos(areaId)
			obj = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, areaId)
	end
	local sData = equip:GetFullAttr()
	if heroId and sData.suitId then
		sData.suitNum = obj:GetSuitElementNum(sData.suitId)
	end
	sData.heroId = heroId
	sData.areaId = areaId
	user:SendProtocol("c_forge_update", sData)
end
------------------------------------------------------------------------
--强化

local PROTECT_ITEM_ID = 102011		--保护符id
local ENHANCE_STONE_ID = 102012		--强化石id

local EnhanceRate = {
	[1] = {rate = 1, loss = 0},
	[2] = {rate = 1, loss = 0},
	[3] = {rate = 1, loss = 0},
	[4] = {rate = 1, loss = 0},
	[5] = {rate = 0.95, loss = -1},
	[6] = {rate = 0.9, loss = -1},
	[7] = {rate = 0.85, loss = -1},
	[8] = {rate = 0.8, loss = -2},
	[9] = {rate = 0.7, loss = -2},
	[10] = {rate = 0.6, loss = -2},
	[11] = {rate = 0.4, loss = -3},
	[12] = {rate = 0.2, loss = -3},
}

function GetEnhanceRate(star, stoneId)
	if not stoneId or BASEITEM.GetItemById(stoneId).kind == 2 then
		return 1
	elseif not EnhanceRate[star+1] then
		return 0
	end
	return EnhanceRate[star+1].rate
end

function GetEnhanceFailLoss(star)
	return EnhanceRate[star +1].loss
end

function SendEnhanceMsg(user, equip)
	local star = equip:GetStar()
	if star >= 11 then
		local msg = string.format("恭喜【%s】强化【%d星 %s】成功", user:GetName(), star, equip:GetName())
		CHAT.SendSysMsg("world", msg)
	end
end

function GetEnhanceCost(enhanceLv, quality, stoneId)
	local stoneInfo = BASEITEM.GetItemById(stoneId)
	if not stoneInfo or stoneInfo.kind == 2 then
		return 0
	elseif not EnhanceRate[enhanceLv] then
		return 0
	end
	return 100 * (3 ^ quality) * enhanceLv
end

--更新强化后属性变化
function SendEnhanceAttrUpdate(user, equip, oldStar)
	local star = equip:GetStar()
	local sData = {}
	sData.star = star
	sData.attrAdd = equip:GetEnhencePreview(star, oldStar)
	user:SendProtocol("c_item_updateAttr", sData)
end


function ProNormalEnhance(user, equip, stoneAreaId, proAreaId, autoBuy)
	local star = equip:GetStar()
	local num = user:GetItemNum(USERBASE.PACKAGE, ENHANCE_STONE_ID)
	local needIngot
	if num < star + 1 then
		if autoBuy then
			needIngot = BASEITEM.GetItemById(ENHANCE_STONE_ID).needIngot * (star + 1 - num)
			if user:GetTotalIngot() < needIngot then
				return false, "您的元宝不足，请充值。"
			end
		else
			return false, "强化石不足，不能完成强化。"
		end
	end	
	local needGold = GetEnhanceCost(equip:GetStar() + 1, equip:GetQuality(), ENHANCE_STONE_ID)
	if user:GetGold() < needGold then
		return false, "您的钱币不足，不能完成强化。"
	end
	local rate = GetEnhanceRate(star, ENHANCE_STONE_ID)
	local protectFlag = false
	if rate < 1 then
		if proAreaId then
			if not user:GetItemByAreaId(USERBASE.PACKAGE, proAreaId) then
				return false, "没有找到指定的保护符信息。"
			end
			protectFlag = true
		end
	end
	if num >= star + 1 then
		local stone = user:GetItemByAreaId(USERBASE.PACKAGE, stoneAreaId)
		local curNum = stone and stone:GetNum() or 0
		if curNum >= star + 1 then
			user:DelItemByAreaId(USERBASE.PACKAGE, stoneAreaId, star + 1)
		else
			user:DelItemByAreaId(USERBASE.PACKAGE, stoneAreaId, curNum)
			user:DelItem(USERBASE.PACKAGE, ENHANCE_STONE_ID, star + 1 - curNum)
		end
	else
		user:DelItem(USERBASE.PACKAGE, ENHANCE_STONE_ID, num)
		user:SetTotalIngot(-needIngot)
	end
	if protectFlag then
		user:DelItemByAreaId(USERBASE.PACKAGE, proAreaId, 1)
	end
	user:SetGold(-needGold)
	local result = 0
	if rate >= math.random() then
		equip:ModStar(1)
		result = 1
	else
		if not protectFlag then
			equip:ModStar(GetEnhanceFailLoss(star))
		end
	end
	return result, needIngot
end

--特殊强化
function ProSpecialEnhance(user, equip, stoneAreaId)
	local stone = user:GetItemByAreaId(USERBASE.PACKAGE, stoneAreaId)
	if not stone then
		return false, "强化石数量不足，不能完成强化。"
	end
	local info = BASEITEM.GetItemById(stone:GetId())
	local star = equip:GetStar()
	if star >= info.num then
		return false, "指定的强化石不能强化该装备。"
	end
	user:DelItemByAreaId(USERBASE.PACKAGE, stoneAreaId, 1)
	equip:SetStar(info.num)
	return 1
end

--强化
local s_forge_enhance = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.autoBuy = tonumber(data.autoBuy)
	if not data.autoBuy then
		return ERROR.TipMsg(connection, "请选择是否自动购买强化石。")
	end
	if not data.stoneAreaId and data.autoBuy ~= 1 then
		return ERROR.TipMsg(connection, "请放入强化石。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要强化的装备。")
	end
	if equip:IsMaxStar() then
		return ERROR.TipMsg(connection, "该装备已经达到最大强化等级。")
	end
	local stone = user:GetItemByAreaId(USERBASE.PACKAGE, data.stoneAreaId)
	if data.stoneAreaId then		
		if not stone or BASEITEM.GetItemById(stone:GetId()).type ~= "enhance" then
			return ERROR.TipMsg(connection, "您放入的强化石有误。")
		end
	end
	if data.proAreaId then
		local protect = user:GetItemByAreaId(USERBASE.PACKAGE, data.proAreaId)
		if not protect or protect:GetId() ~= PROTECT_ITEM_ID then
			return ERROR.TipMsg(connection, "您放入的保护符有误。")
		end
	end
	local result, msg
	local star = equip:GetStar()
	if not stone or BASEITEM.GetItemById(stone:GetId()).kind ~= 2 then
		result, msg = ProNormalEnhance(user, equip, data.stoneAreaId, data.proAreaId, data.autoBuy == 1)
	else
		result, msg = ProSpecialEnhance(user, equip, data.stoneAreaId)
	end
	if not result then
		return ERROR.TipMsg(connection, msg)
	end
	if star ~= equip:GetStar() then
		SendEquipUpdate(user, data.heroId, data.areaId)
--		SendEnhanceAttrUpdate(user, equip, star)
		SendEnhanceMsg(user, equip)
		if whoWear then
			whoWear:FreshTotalAttr()
		end
	end
	MISSION.CheckMissionEvent(user, "func", SETTING.FUNC_ENHANCE, 1)
	connection:SendProtocol("c_forge_enhance", {heroId = data.heroId, areaId = data.areaId, result = result, ingot = msg})
	PROTOCOL.s_enhance_info(connection, {heroId = data.heroId, areaId = data.areaId})
end


--强化信息
local s_enhance_info = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要强化的装备。")
	end
	local star = equip:GetStar()
	local sData = {}
	sData.heroId = data.heroId
	sData.areaId = data.areaId	
	sData.stoneAreaId = data.stoneAreaId
	sData.preview = equip:GetEnhencePreview(star + 1)
	local stone = user:GetItemByAreaId(USERBASE.PACKAGE, data.stoneAreaId)
	local stoneId = stone and stone:GetId() or ENHANCE_STONE_ID
	sData.rate = GetEnhanceRate(star, stoneId)
	sData.needGold = GetEnhanceCost(star + 1, equip:GetQuality(), stoneId)
	sData.needNum = math.min(star + 1, 12)
	if BASEITEM.GetItemById(stoneId).kind == 2 then
		sData.needNum = 1
	end
	connection:SendProtocol("c_enhance_info", sData)
end


-----------------------------------------------------------------------------------------
-- upgrade 进阶

local UPGRADE_REDUCE_LV = 3		--升级后降低强化等级
local UPGRADE_NEED_LV = 9		--可以进阶的最小强化等级

local UpgradeNeedMaterail = {
	["weapon"] = 102013,
	["armor"] = 102015,
	["jewelry"] = 102014,
}

function GetUpgradeNeedMaterialNum(quality)
	return 2 ^ quality
end

function GetEquipUpgradeCost(quality)
	return quality * quality * 20000
end

function EquipUpgrade(user, equip, autoBuy, ownerLv)
	if equip:GetStar() < UPGRADE_NEED_LV then
		return false, "该装备强化等级不足。"
	end
	local equipInfo = BASEITEM.GetItemById(equip:GetId())
	local complexInfo = BASEITEM.GetItemById(equipInfo.complex)
	if not complexInfo then
		return false, "该装备不能进行进阶。"
	elseif ownerLv and ownerLv < complexInfo.needLv then
		return false, "佩戴者的等级不能低于晋阶产物的需求等级。"
	end
	local materialId = UpgradeNeedMaterail[equip:GetType()]
	local num = user:GetItemNum(USERBASE.PACKAGE, materialId)
	local quality = equip:GetQuality()
	local need = GetUpgradeNeedMaterialNum(quality)
	local needIngot
	if num < need then
		if not autoBuy then
			return false, "进阶所需材料不足。"
		end
		local materialInfo = BASEITEM.GetItemById(materialId)
		needIngot = materialInfo.needIngot * (need - num)
		if user:GetTotalIngot() < needIngot then
			return false, "您的元宝不足，请充值。"
		end
	end
	local cost = GetEquipUpgradeCost(quality)
	if user:GetGold() < cost then
		return false, "您的钱币不足，不能完成进阶。"
	end
	if num >= need then
		user:DelItem(USERBASE.PACKAGE, materialId, need)
	else
		user:DelItem(USERBASE.PACKAGE, materialId, num)
		user:SetTotalIngot(-needIngot)
	end
	user:SetGold(-cost)
	equip:SetId(equipInfo.complex)
	equip:ModStar(-UPGRADE_REDUCE_LV)
	equip:CheckHoleList()
	return true, needIngot
end

local s_forge_upgrade = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要进阶的装备。")
	end
	data.autoBuy = tonumber(data.autoBuy)
	if not data.autoBuy then
		return ERROR.TipMsg(connection, "请选择是否自动购买材料。")
	end
	local result, msg = EquipUpgrade(user, equip, data.autoBuy == 1, whoWear and whoWear:GetLv())
	if not result then
		return ERROR.TipMsg(connection, msg)
	end
	if whoWear then
		whoWear:FreshTotalAttr()
	end
	SendEquipUpdate(user, data.heroId, data.areaId)
	connection:SendProtocol("c_forge_upgrade", {heroId = data.heroId, areaId = data.areaId, ingot = ingot})
end

local s_forge_upgradeInfo = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要进阶的装备。")
	end
	local star = equip:GetStar()
	local sData = {}
	sData.heroId = data.heroId
	sData.areaId = data.areaId
	sData.preview = equip:GetUpgradePreView(UPGRADE_REDUCE_LV)
	local equipInfo = BASEITEM.GetItemById(equip:GetId())
	if equipInfo.complex then
		local newItem = BASEITEM.NewItem(equipInfo.complex, 1, {star = star - UPGRADE_REDUCE_LV, rate = equip:GetGrade()})
		local complex = newItem:GetFullAttr()
		if complex.suitId and whoWear then
			complex.suitNum = whoWear:GetSuitElementNum(complex.suitId)
		end
		newItem:Destroy()
		sData.complex = complex
		sData.rate = 1
		sData.materialId = UpgradeNeedMaterail[equip:GetType()]
		sData.needNum = GetUpgradeNeedMaterialNum(equip:GetQuality())
		sData.needGold = GetEquipUpgradeCost(equip:GetQuality())
	end
	connection:SendProtocol("c_forge_upgradeInfo", sData)
end

----------------------------------------------------------------------------
local INJECT_STONE_ID = 102003		--注灵石id
local INJECT_ATTR_BASE = 0.1		--注灵增加属性比率


--随机生成一组属性值
function GenInjectAttr(quality)	
	local num = 3
	local ran = math.random()
	if ran <= 0.5 then
		num = 1
	elseif ran <= 0.35 then
		num = 2
	end
	local attrList = GetInjectAttrList(quality)
	local buff = {}
	for k in pairs(attrList) do
		table.insert(buff, k)
	end
	local list = {}
	while (num > 0 and #buff > 0) do
		local ran = math.random(1, #buff)
		table.insert(list, {type = buff[ran], num = attrList[buff[ran]] * INJECT_ATTR_BASE})
		table.remove(buff, ran)
		num = num - 1
	end
	return list
end

function ProInject(user, equip, autoBuy)
	local needNimbus = GetForgeCost("inject", equip:GetQuality())
	if user:GetNimbus() < needNimbus then
		return false, "灵气不足，无法完成注灵。"
	end
	local needIngot
	local num = user:GetItemNum(USERBASE.PACKAGE, INJECT_STONE_ID)
	if num < 1 then
		if not autoBuy then
			return false, "您的注灵石不足，无法完成注灵。"
		end
		needIngot = BASEITEM.GetItemById(INJECT_STONE_ID).needIngot
		if user:GetTotalIngot() < needIngot then
			return false, "您的元宝不足，请充值。"
		end
	end
	user:SetNimbus(-needNimbus)
	if num >= 1 then
		user:DelItem(USERBASE.PACKAGE, INJECT_STONE_ID, 1)
	else
		user:SetTotalIngot(-needIngot)
	end
	local attrList = GenInjectAttr(equip:GetQuality())
	equip:SetTempInject(attrList)
	return attrList, needIngot
end

--装备注灵
local s_forge_inject = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.autoBuy = tonumber(data.autoBuy)
	if not data.autoBuy then
		return ERROR.TipMsg(connection, "请选择是否自动购买注灵石。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要注灵的装备。")
	end
	local ret, msg = ProInject(user, equip, data.autoBuy == 1)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_forge_inject", {heroId = data.heroId, areaId = data.areaId, attrList = ret, ingot = msg})
	PROTOCOL.s_inject_info(connection, {heroId = data.heroId, areaId = data.areaId})
end

--获取注灵信息
local s_inject_info = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local equip
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要注灵的装备。")
	end
	local sData = {heroId = data.heroId, areaId = data.areaId}
	sData.needNimbus = GetForgeCost("inject", equip:GetQuality())
--	sData.attrList = equip:GetInjectInfo() or {}
	connection:SendProtocol("c_inject_info", sData)
end

local s_inject_save = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要注灵的装备。")
	end
	if not equip:GetTempInject() then
		return ERROR.TipMsg(connection, "没有可以替换的属性。")
	end
	equip:SaveInjectInfo()
	SendEquipUpdate(user, data.heroId, data.areaId)
	if whoWear then
		whoWear:FreshTotalAttr()
	end
	connection:SendProtocol("c_inject_save", {heroId = data.heroId, areaId = data.areaId})
end

----------------------------------------------------------------------------
local PROMOTE_STONE_ID = 102004		--提灵石id
local PROMOTE_ATTR_BASE = 0.1		--提灵增加属性比率
local PROMOTE_RATE_DOWN = 0.04		--每次提灵的成功率下降幅度


--计算提灵成功率
function CalPromoteRate(times)
	return 1 - times * PROMOTE_RATE_DOWN
end

function GetInjectAttrMax(quality, type)
	local list = GetInjectAttrList(quality)
	return list[type]
end

function IsEquipAttrMax(equip)
	local attrList = equip:GetInjectInfo()
	if not attrList then
		return false
	end
	for k, v in ipairs(attrList) do
		if v.num < GetInjectAttrMax(equip:GetQuality(), v.type) then
			return false
		end
	end
	return true
end

--提灵预览
function GetEquipPromoteView(equip)
	local list = {}
	for k, v in ipairs(equip:GetInjectInfo() or {}) do
		table.insert(list, {type = v.type, num = v.num, max = GetInjectAttrMax(equip:GetQuality(), v.type)})
	end
	return list
end

--更新提灵后的属性变化
function SendPromoteUpdate(user, heroId, areaId, attrList)
	local sData = {heroId = heroId, areaId = areaId}
	sData.attr = {}
	for k, v in pairs(attrList) do
		sData.attr[k] = v * PROMOTE_ATTR_BASE
	end
	local con = LOGIN.GetConnection(user:GetRoleId())
	if con and con.roleId == user:GetRoleId() then
		con:SendProtocol("c_item_updateAttr", sData)
	end
end

function EquipPromote(user, equip)
	local attrList = equip:GetInjectInfo()
	if not attrList then
		return false
	end
	local quality = equip:GetQuality()
	local buff = {}
	for k, v in ipairs(attrList) do
		if v.num < GetInjectAttrMax(quality, v.type) then
			table.insert(buff, k)
		end
	end
	if #buff < 1 then
		return false
	end
	local chosen = buff[math.random(1, #buff)]
	attrList[chosen].num = attrList[chosen].num + GetInjectAttrMax(quality, attrList[chosen].type) * PROMOTE_ATTR_BASE
	local result = {}
	result.type = attrList[chosen].type
	result.num = attrList[chosen].num
	return result
end


function ProPromote(user, equip, autoBuy)
	if not equip:GetInjectInfo() then
		return ERROR.TipMsg(connection, "请先对该装备进行注灵。")
	elseif IsEquipAttrMax(equip) then
		return false, "该装备的所有注灵属性已经提升到最大值。"
	end
	local needNimbus = GetForgeCost("promote", equip:GetQuality())
	if user:GetNimbus() < needNimbus then
		return false, "您的灵气不足，无法完成提灵。"
	end
	local needIngot
	local num = user:GetItemNum(USERBASE.PACKAGE, PROMOTE_STONE_ID)
	if num < 1 then
		if not autoBuy then
			return false, "提灵石数量不足，无法完成提灵操作。"
		end
		needIngot = BASEITEM.GetItemById(PROMOTE_STONE_ID).needIngot
		if user:GetTotalIngot() < needIngot then
			return false, "您的元宝不足，请充值。"
		end
	end
	user:SetNimbus(-needNimbus)
	if num >= 1 then
		user:DelItem(USERBASE.PACKAGE, PROMOTE_STONE_ID, 1)
	else
		user:SetTotalIngot(-needIngot)
	end
	local result = 0, attrList
	if math.random() < CalPromoteRate(equip:GetPromoteTimes()) then
		attrList = EquipPromote(user, equip)
		equip:ModPromoteTimes(1)
		result = 1
	end
	return result, needIngot, attrList
end

--装备提灵
local s_forge_promote = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.autoBuy = tonumber(data.autoBuy)
	if not data.autoBuy then
		return ERROR.TipMsg(connection, "请选择是否自动购买提灵石。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要提灵的装备。")
	end
	local ret, msg, attr = ProPromote(user, equip, data.autoBuy == 1)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	if attr then
	--	SendPromoteUpdate(user, data.heroId, data.areaId, attrList)
		SendEquipUpdate(user, data.heroId, data.areaId)
		if whoWear then
			whoWear:FreshTotalAttr()
		end
	end
	connection:SendProtocol("c_forge_promote", {heroId = data.heroId, areaId = data.areaId, result = ret, attrList = attr, ingot = msg})
end

local s_promote_info = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要提灵的装备。")
	end
	local sData = {heroId = data.heroId, areaId = data.areaId}
	sData.rate = CalPromoteRate(equip:GetPromoteTimes())
	sData.needNimbus = GetForgeCost("promote", equip:GetQuality())
	sData.attrList = GetEquipPromoteView(equip)
	connection:SendProtocol("c_promote_info", sData)
end

--------------------------------------------------------------------------------------------
local RECOIN_COST_NIMBUS = 5000	--重铸所需灵气
local RECOIN_ITEM_ID = 102001	--天工锤Id

function ProRecoin(user, equip, autoBuy)
	if user:GetNimbus() < RECOIN_COST_NIMBUS then
		return false, "灵气不足，无法完成重铸。"
	end
	local needIngot
	local num = user:GetItemNum(USERBASE.PACKAGE, RECOIN_ITEM_ID)
	if num < 1 then
		if not autoBuy then
			return false, "您的天工锤不足，无法完成重铸操作。。"
		end
		needIngot = BASEITEM.GetItemById(RECOIN_ITEM_ID).needIngot
		if user:GetTotalIngot() < needIngot then
			return false, "您的元宝不足，不能购买天工锤，请充值。"
		end
	end
	user:SetNimbus(-RECOIN_COST_NIMBUS)
	if needIngot then
		user:SetTotalIngot(-needIngot)
	else
		user:DelItem(USERBASE.PACKAGE, RECOIN_ITEM_ID, 1)
	end
	equip:ResetGrade()
	return true, needIngot
end

--装备重铸
local s_forge_recoin = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.autoBuy = tonumber(data.autoBuy)
	if not data.autoBuy then
		return ERROR.TipMsg(connection, "请放入天工锤。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip, whoWear
	if data.heroId then
		if data.heroId == "user" or data.heroId == user:GetRoleId() then
			equip = user:GetEquipByPos(data.areaId)
			whoWear = user
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, "无效的武将id。")
			end
			equip = hero:GetEquipByPos(data.areaId)
			whoWear = hero
		end
	else
		equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	end
	if not equip or equip:GetClass() ~= "equip" then
		return ERROR.TipMsg(connection, "请放入要重铸的装备。")
	end
	local ret, msg = ProRecoin(user, equip, data.autoBuy == 1)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	SendEquipUpdate(user, data.heroId, data.areaId)
	if whoWear then
		whoWear:FreshTotalAttr()
	end
	connection:SendProtocol("c_forge_recoin", {heroId = data.heroId, areaId = data.areaId, ingot = msg})
end

local s_recoin_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = {needNimbus = RECOIN_COST_NIMBUS}
	connection:SendProtocol("c_recoin_info", sData)
end



function __init__()
	
	local FORGECOST = Import("autocode/forgeCost.lua")
	AllForgeInfoList = FORGECOST.GetTable()
	
	PROTOCOL.s_forge_enhance = s_forge_enhance
	PROTOCOL.s_enhance_info = s_enhance_info
	
	PROTOCOL.s_forge_upgrade = s_forge_upgrade
	PROTOCOL.s_forge_upgradeInfo = s_forge_upgradeInfo
	
	PROTOCOL.s_forge_inject = s_forge_inject
	PROTOCOL.s_inject_info = s_inject_info
	PROTOCOL.s_inject_save = s_inject_save
	
	PROTOCOL.s_forge_promote = s_forge_promote
	PROTOCOL.s_promote_info = s_promote_info
	
	PROTOCOL.s_forge_recoin = s_forge_recoin
	PROTOCOL.s_recoin_info = s_recoin_info
	
end


