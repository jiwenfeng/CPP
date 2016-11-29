--本命精力系统


local QualityRate = {
	{quality = 1, rate = 0.1}, {quality = 2, rate = 0.5}, {quality = 3, rate = 0.35},
	{quality = 4, rate = 0.048}, {quality = 5, rate = 0.002},
}

local MAX_FRESH_ITEM_NUM = 6		--刷新物品数量
local REFRESH_FREQUENCY = 4 * 3600 	--刷新频率
local REFRESH_COST = 10				--元宝刷新价格

local ExchangeItemList = {}		--兑换物品列表
local ItemClassifiedList = {}	--物品分类列表

function GetExpByLv(lv)
	return PetExpList[lv] or 0
end

function GetAllPetInfoList()
	return AllPetInfoList
end



function GetAllExchangeList()
	return ExchangeItemList
end

function GetExchangeInfoById(id)
	return ExchangeItemList[id]
end


--分类所有可兑换的物品
function ClassifyItemList()
	for _, v in ipairs(QualityRate) do
		ItemClassifiedList[v.quality] = {}
	end
	for k, v in pairs(GetAllExchangeList()) do
		table.insert(ItemClassifiedList[v.quality], k)
	end
end

function CalRandomQuality()
	local ran = math.random()
	local rate = 0
	for k, v in ipairs(QualityRate) do
		rate = rate + v.rate
		if rate >= ran then
			return v.quality
		end
	end
end


--随机生成6种可兑换的物品
function GenExchangeItemList(pet, keepTime)
	local list = {}
	for i = 1, MAX_FRESH_ITEM_NUM do 		
		local quality = CalRandomQuality()
		local buff = ItemClassifiedList[quality]
		local index = math.random(1, #buff)
		local id = buff[index]
		local info = GetExchangeInfoById(id)
		local temp = {}
		temp.id = id
		temp.num = info.num
		temp.index = i
		table.insert(list, temp)
	end
	pet:SetExchangeList(list)
	if not keepTime then
		pet:SetAutoRefreshTimestamp(os.time() + REFRESH_FREQUENCY)
	end
end

--当前兑换列表
function GetFormatedExchangeList(pet)
	local sData = {}
	for k, v in ipairs(pet:GetExchangeList()) do 
		local info = GetExchangeInfoById(v.id)
		if info then
			local buf = {}
			buf.goodsId = v.id
			buf.needNimbus = info.needNimbus
			buf.num = v.num
			buf.index = v.index
			table.insert(sData, buf)
		end
	end
	return sData
end


--使用幻形卡效果
function PetTransform(user, id)
	local pet = user:GetPet()
	pet:Transform(id)
	user:SetPetTransformInfo(id)
end

function NewPet(owner, id)
	local pet = CLSPET.clsPet:New()
	local roleId = STATS.GenId("pet")
	pet:InitPet(roleId, id, owner)
	return pet
end

local s_pet_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", tostring(data)))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet then
		return ERROR.TipMsg(connection, "您没有本命精灵。")
	end
	if not pet:GetId() then
		pet:Transform(200001)
	end
	local sData = pet:GetPetDetailInfo()
--	sData.appList = GetAppellationBySex(user:GetSex())
	connection:SendProtocol("c_pet_info", sData)
end

local s_pet_talentList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", tostring(data)))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet then
		return ERROR.TipMsg(connection, "您没有本命精灵。")
	end
	local sData = {}
	local talentInfo = CLSPET.GetPetAptInfoByLv(pet:GetAptLv())
	sData.talentNum = talentInfo.talentNum
	sData.list = pet:GetAttrDescList()
	connection:SendProtocol("c_pet_talentList", sData)
end

local s_pet_attrDesc = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	local sData = {}
	sData.type = "pet"
	sData.name = pet:GetName()
	sData.list = pet:GetAttrDescList()
	connection:SendProtocol("c_pet_attrDesc", sData)
end

--变换形态
local s_pet_transform = function(connection, data)
	local info = CLSPET.GetPetInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "无效的本命精灵ID")
	end	
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	local ret, msg = pet:CheckPetTransForm()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	pet:InitTalent()
	pet:Transform(data)
	GenExchangeItemList(pet)
	connection:SendProtocol("c_pet_transform", data)
end

--喂养
local s_pet_feed = function(connection, data)
	local info = BASEITEM.GetItemById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的物品信息。")
	end
	if info.type ~= "petFood" then
		return ERROR.TipMsg(connection, "不能喂食该物品给本命精灵。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet or pet:GetForm() == 0 then
		return ERROR.TipMsg(connection, "本命精灵还未化形，不需要喂养。")
	end
	local num = user:GetItemNum(USERBASE.PACKAGE, data)
	if num <= 0 then
		return ERROR.TipMsg(connection, string.format("【%s】数量不足。", info.name))
	end
	local ret, msg = pet:Feed(data)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	user:DelItem(USERBASE.PACKAGE, data, 1)
	connection:SendProtocol("c_pet_feed", msg)
end

local s_pet_setHeading = function(connection, data)
	data = tonumber(data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local headingList = CLSPET.GetHeadingBySex(user:GetSex())
	if data < 1 or data > #headingList then
		return ERROR.TipMsg(connection, "无效的称谓索引。")
	end
	local pet = user:GetPet()
	pet:SetHeading(data)
	connection:SendProtocol("c_pet_setHeading", data)
end


--兑换列表
local s_pet_exchangeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet or pet:GetForm() == 0 then
		return ERROR.TipMsg(connection, "本命精灵还未孵化，不能进行兑换。")
	end
	local sData = {}
	sData.needIngot = REFRESH_COST
	sData.freshNum = pet:GetRefreshTimes()
	sData.freshMax = pet:GetRefreshMaxTimes()
--	sData.timelong = math.max(pet:GetAutoRefreshTimestamp() + REFRESH_FREQUENCY - os.time(), 0)
	sData.timelong = math.max(pet:GetAutoRefreshTimestamp() - os.time(), 0)
	sData.list = GetFormatedExchangeList(pet)
	connection:SendProtocol("c_pet_exchangeList", sData)
end

--兑换
local s_pet_exchange = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet or pet:GetForm() == 0 then
		return ERROR.TipMsg(connection, "本命精灵还未孵化，不能进行兑换。")
	end
	data = tonumber(data)
	local itemInfo = pet:GetExchgItemByIndex(data)
	if not itemInfo then
		return ERROR.TipMsg(connection, "没有该物品的兑换信息")
	elseif itemInfo.num <= 0 then
		return ERROR.TipMsg(connection, "该物品已经兑换完了。")
	end
	local exchgInfo = GetExchangeInfoById(itemInfo.id)
	if not exchgInfo then
		return ERROR.TipMsg(connection, "该物品不能进行兑换。")
	end
	if user:GetNimbus() < exchgInfo.needNimbus then
		return ERROR.TipMsg(connection, "您的灵气不足，无法完成兑换。")
	end
	local item = BASEITEM.NewItem(itemInfo.id, 1, {lock = 1})
	if not user:IsItemCanAdd(USERBASE.PACKAGE, item) then
		return ERROR.TipMsg(connection, "您的背包容量不足，无法完成兑换。")
	end
	pet:ModExchgItemByIndex(data, -1)
	user:AddItem(USERBASE.PACKAGE, item)
	user:SetNimbus(-exchgInfo.needNimbus)
	user:SendProtocol("c_show_tips", string.format("获得%s x %d", item:GetName(), item:GetNum()))
	connection:SendProtocol("c_pet_exchange", data)
end

--元宝刷新
local s_pet_refresh = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	if not pet or pet:GetForm() == 0 then
		return ERROR.TipMsg(connection, "本命精灵还未孵化，不能使用该功能。")
	end
--[[	if pet:GetAutoRefreshTimestamp() + REFRESH_FREQUENCY - os.time() <= 0 then
		GenExchangeItemList(pet)
	else]]
		if user:GetTotalIngot() < REFRESH_COST then
			return ERROR.TipMsg(connection, "您的元宝数量不足")
		end
		if pet:GetRefreshTimes() >= pet:GetRefreshMaxTimes() then
			return ERROR.TipMsg(connection, "您今天的刷新次数已经用完")
		end
		GenExchangeItemList(pet, true)
		pet:SetRefreshTimes(1)
		user:SetTotalIngot(-REFRESH_COST)
--	end
	connection:SendProtocol("c_pet_refresh", "true")
	PROTOCOL.s_pet_exchangeList(connection, "get")
end

local s_pet_dailyMissConfm = function(connection, data)
	data = tonumber(data)
	if data ~= 0 and data ~= 1 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end	
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	pet:ProDailyMissionAsk(data)
	connection:SendProtocol("c_pet_dailyMissConfm", data)
end


local s_pet_prompt = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	local prompt = pet:GetPetPrompt()
	if prompt then
		pet:SendPetPrompt(prompt.type, prompt.msg)
	else
		connection:SendProtocol("c_pet_prompt", 0)
	end
end

--获取本命精灵选择界面
local s_pet_select = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local pet = user:GetPet()
	pet:CheckPetForm()
end

local s_bible_honor = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local sData = TITLE.GetFormatedHonorTitleList()
	connection:SendProtocol("c_bible_honor", sData)
end

function __init__()
	
	
	local EXCHANGELIST = Import("autocode/petExchangeList.lua")
	ExchangeItemList = EXCHANGELIST.GetTable()

	local APTLVINFO = Import("autocode/petAptLv.lua")
	AptLvList = APTLVINFO.GetTable()
	
--	FormatTalentList()
	ClassifyItemList()
	
	PROTOCOL.s_pet_info = s_pet_info
	PROTOCOL.s_pet_talentList = s_pet_talentList
	PROTOCOL.s_pet_attrDesc = s_pet_attrDesc
	
	PROTOCOL.s_pet_transform = s_pet_transform
	PROTOCOL.s_pet_feed = s_pet_feed
	PROTOCOL.s_pet_setHeading = s_pet_setHeading
	PROTOCOL.s_pet_dailyMissConfm = s_pet_dailyMissConfm
	PROTOCOL.s_pet_prompt = s_pet_prompt
	PROTOCOL.s_pet_select = s_pet_select
	
	PROTOCOL.s_pet_exchangeList = s_pet_exchangeList
	PROTOCOL.s_pet_exchange = s_pet_exchange
	PROTOCOL.s_pet_refresh = s_pet_refresh
	
	PROTOCOL.s_bible_honor = s_bible_honor
end
