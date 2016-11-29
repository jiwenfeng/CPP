--属性镶嵌(enchase 镶嵌	combine 合成	pick 摘除)


function ProEnchase(user, equip, gemAreaId, pos)
	local gemObj = user:GetItemByAreaId(USERBASE.PACKAGE, gemAreaId)
	if not gemObj or gemObj:GetClass() ~= "gem" then
		return false, "指定的宝石信息有误。"
	end
	local gemId = gemObj:GetId()
	local gemInfo = BASEITEM.GetItemById(gemId)
	if equip:IsSameGemExist(gemId) then
		return false, "不能在装备上镶嵌同类型的宝石。"
	end
	if pos then
		local holeInfo = equip:GetOneHole(pos)
		if not holeInfo then
			return false, "无效的宝石孔编号。"
		elseif holeInfo.id then
			return false, "指定的宝石孔已经镶嵌过宝石了。"
		end
	else
		pos = equip:GetNextGemPos()
		if not pos then
			return false, "该装备已经没有多余的宝石孔了。"
		end
	end
	local needGold = ENHANCE.GetForgeCost("enchase", equip:GetQuality())
	if user:GetGold() < needGold then
		return false, "您的钱币不足，不能完成镶嵌操作。"
	end
	user:SetGold(-needGold)
	equip:AddOneGem(pos, {id = gemId})
	user:DelItemByAreaId(USERBASE.PACKAGE, gemAreaId, 1)
	return true
end


local s_forge_enchase = function(connection, data)
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
		return ERROR.TipMsg(connection, "请放入要镶嵌宝石的装备。")
	end
	local ret, msg = ProEnchase(user, equip, data.gemAreaId, data.pos)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	ENHANCE.SendEquipUpdate(user, data.heroId, data.areaId)
	connection:SendProtocol("c_forge_enchase", data)
end

local s_enchase_info = function(connection, data)
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
		return ERROR.TipMsg(connection, "请放入要镶嵌宝石的装备。")
	end
	local holeNum = equip:GetHoleNum()
	local sData = {heroId = data.heroId, areaId = data.areaId}
	sData.needGold = ENHANCE.GetForgeCost("enchase", equip:GetQuality())	
	connection:SendProtocol("c_enchase_info", sData)
end

---------------------------------------------------------------

function ProGemPick(user, equip, pos)
	local gemInfo = equip:GetOneGem(pos)
	if not gemInfo or not gemInfo.id then
		return false, "指定的位置没有镶嵌宝石。"
	end
	local needGold = ENHANCE.GetForgeCost("pick", equip:GetQuality())
	if user:GetGold() < needGold then
		return false, "您的钱币不足，不能完成镶嵌操作。"
	end
	user:SetGold(-needGold)
	equip:DelOneGem(pos)
	local gemAreaId = user:AddItemById(USERBASE.PACKAGE, gemInfo.id, 1, {lock = 1})
	return gemAreaId
end


local s_forge_pick = function(connection, data)
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
		return ERROR.TipMsg(connection, "请放入要镶嵌宝石的装备。")
	end
	if user:IsBagFull(USERBASE.PACKAGE) then
		return ERROR.TipMsg(connection, "您的背包空间不足。")
	end
	local ret, msg = ProGemPick(user, equip, data.pos)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	ENHANCE.SendEquipUpdate(user, data.heroId, data.areaId)
	connection:SendProtocol("c_forge_pick", {heroId = data.heroId, areaId = data.areaId, pos = data.pos, gemAreaId = msg})
end

----------------------------------------------------------------------------

local COMBINE_COST = 1000
local CombineNeed = {min = 3, max = 5, baseRate = 0.2}			--合成所需宝石

function GetCombineRate(num)
	return CombineNeed.baseRate * num
end

function GetCombineCost(lv, num)
	return COMBINE_COST * lv * num
end

function ProOneCombine(lv, num)
	if math.random() < GetCombineRate(num) then
		return true
	end
	return false
end

function ProCombine(user, gemId, num, perNum)
	local info = BASEITEM.GetItemById(gemId)
	if not info then
		return false, "无效的物品id"
	elseif info.type ~= "piece" and info.type ~= "gem" then
		return ERROR.TipMsg(connection, "该物品不能进行合成。")
	end
	local curNum = user:GetItemNum(USERBASE.PACKAGE, gemId)
	if curNum < num then
		return false, string.format("您背包中只有%d个【%s】。", curNum, info.name)
	end
	local count = math.floor(num / perNum)
	local complexInfo = BASEITEM.GetItemById(info.complex)
	local cost = GetCombineCost(complexInfo.Lv, num)
	if user:GetGold() < cost then
		return false, "您的钱币不足，无法完成合成。"
	end
	user:SetGold(-cost)
	local lockGemTimes = math.ceil(user:GetItemNum(USERBASE.PACKAGE, gemId, {lock = 1}) / perNum)
	local success, lockNum = 0, 0		--合成宝石总数及绑定宝石数量
	for i = 1, count do
		if ProOneCombine(info.Lv, perNum) then
			success = success + 1
			if i <= lockGemTimes then		--先消耗绑定宝石，因此先生成绑定宝石
				lockNum = lockNum + 1
			end
		end
	end
	user:DelItem(USERBASE.PACKAGE, gemId, count * perNum)
	if lockNum > 0 then
		user:AddItemById(USERBASE.PACKAGE, info.complex, lockNum, {lock = 1})
	end
	if success > lockNum then
		user:AddItemById(USERBASE.PACKAGE, info.complex, success - lockNum)
	end
	if info.type == "piece" and success > 0 then
		local msg = string.format("成功合成宝图，可前往藏宝峡谷挖掘宝藏。")
		CHAT.SendSysMsg("system", msg, {user:GetRoleId()}, {kind = "worldMap", id = TREASURE.GetTreasureIdByMap(info.complex)})
	end
	return info.complex, success
end

local s_forge_combine = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	data.num = tonumber(data.num)
	if not data.num then
		return ERROR.TipMsg(connection, "请输入要合成的宝石数量。")
	end
	data.gemId = tonumber(data.gemId)
	local info = BASEITEM.GetItemById(data.gemId)
	if not info then
		return ERROR.TipMsg(connection, "无效的物品id。")
	end
	data.perNum = tonumber(data.perNum)
	if not data.perNum or data.perNum < CombineNeed.min or data.perNum > CombineNeed.max then
		return ERROR.TipMsg(connection, "请选择每次合成需要使用的物品数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local num = user:GetItemNum(USERBASE.PACKAGE, data.gemId)
	if num < data.num then
		return ERROR.TipMsg(connection, string.format("您背包中只有%d个【%s】。", num, info.name))
	end
	if user:IsBagFull(USERBASE.PACKAGE) then
		return ERROR.TipMsg(connection, "您的背包空间不足。")
	end
	local ret, msg = ProCombine(user, data.gemId, data.num, data.perNum)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_forge_combine", {gemId = data.gemId, complex = ret, success = msg})
end

local s_combine_info = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	data.gemId = tonumber(data.gemId)
	local info = BASEITEM.GetItemById(data.gemId)
	if not info then
		return ERROR.TipMsg(connection, "无效的物品id。")
	end
	if info.type ~= "piece" and info.type ~= "gem" then
		return ERROR.TipMsg(connection, "该物品不能进行合成。")
	end
	local complexInfo = BASEITEM.GetItemById(info.complex)
	local sData = {gemId = data.gemId}
	sData.complex = info.complex
	sData.baseGold = COMBINE_COST * complexInfo.Lv
	sData.baseRate = CombineNeed.baseRate
	connection:SendProtocol("c_combine_info", sData)
end



function __init__()
	
	PROTOCOL.s_forge_enchase = s_forge_enchase
	PROTOCOL.s_enchase_info = s_enchase_info
	PROTOCOL.s_forge_pick = s_forge_pick
	
	PROTOCOL.s_forge_combine = s_forge_combine
	PROTOCOL.s_combine_info = s_combine_info
	
end
