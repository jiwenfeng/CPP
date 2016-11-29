--藏宝点类

local MAX_LOG_NUM = 2
local LOG_NEED_QUALITY = 5
local NOTICE_NEED_QUALITY = 5

local TreasureList = {}

--藏宝点信息
function GetAllTreasureList()
	return TreasureList
end

function GetTreasureInfoById(id)
	return TreasureList[id]
end

-----------------------------------------------------------------
local TreasurePrize = {}		--寻宝奖励信息

--寻宝奖励信息
function GetAllTreasurePrize()
	return TreasurePrize
end

function GetTreasurePrizeById(id)
	return TreasurePrize[id]
end
--计算奖励范围
function CalPrizeInfoByTimes(id, spadeId, times)
	local info = GetTreasureInfoById(id)
	for _, v in ipairs(info.rateList[spadeId]) do
		if times >= v.min and times < (v.max or math.huge) then
			return v
		end
	end
end
--计算奖励
function CalTreasureGot(id, spadeId, times)
	local info = CalPrizeInfoByTimes(id, spadeId, times)
	local ran = math.random()
	local rate = 0
	for k, v in ipairs(info.prizeList) do
		rate = rate + v.rate
		if rate >= ran then
			local prizeInfo = GetTreasurePrizeById(v.type)
			local index = math.random(1, #prizeInfo)
			return prizeInfo[index]
		end
	end
end
-----------------------------------------------------------------

clsTreasure = clsObject:Inherit()

function clsTreasure:__init__()
	Super(clsTreasure).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsTreasure:initsavedata()
	self.__save.log = {}
end

function clsTreasure:inittempdata()
	
end

function clsTreasure:InitTreasure(id)
	self:SetId(id)
end

function clsTreasure:SetId(id)
	self.__save.id = id
end

function clsTreasure:GetId()
	return self.__save.id
end

function clsTreasure:GetName()
	local info = GetTreasureInfoById(self:GetId())
	return info.name
end

--藏宝图id
function clsTreasure:GetMapId()
	local info = GetTreasureInfoById(self:GetId())
	return info.mapId
end

function clsTreasure:GetMapNeedIngot()
	local mapId = self:GetMapId()
	local itemInfo = BASEITEM.GetItemById(mapId)
	if itemInfo then
		return itemInfo.needIngot or 20
	end
end

function clsTreasure:GetPicId()
	local info = GetTreasureInfoById(self:GetId())
	return info.picId
end

--铲子
function clsTreasure:GetSpadeList()
	local info = GetTreasureInfoById(self:GetId())
	local buff = {}
	for k in pairs(info.spadeList) do
		local itemInfo = BASEITEM.GetItemById(k)
		if itemInfo then
			table.insert(buff, {spadeId = k, needIngot = itemInfo.needIngot})
		end
	end
	table.sort(buff, function(a, b) return tonumber(a.spadeId) < tonumber(b.spadeId) end)
	return buff
end

function clsTreasure:IsValidSpade(spadeId)
	local info = GetTreasureInfoById(self:GetId())
	return info.spadeList[spadeId]
end

function clsTreasure:GetSpadeNeedIngot(spadeId)
	local itemInfo = BASEITEM.GetItemById(spadeId)
	if itemInfo then
		return itemInfo.needIngot or 10
	end
end

--藏宝点日志
function clsTreasure:GetLogList()
	return self.__save.log
end

function clsTreasure:AddLog(log)
	table.insert(self.__save.log, log)
	while #self.__save.log > MAX_LOG_NUM do
		table.remove(self.__save.log, 1)
	end
end

function clsTreasure:SendNewLog(log)
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_treasure_newLog", log)
		end
	end
end

function clsTreasure:BeginDig(user, times, spadeId, autoBuyMap, autoBuySpade)
	local mapNum = user:GetItemNum(USERBASE.PACKAGE, self:GetMapId())
	local needIngot = 0
	if mapNum < times then
		if not autoBuyMap then
			return false, "藏宝图数量不足。"
		end
		needIngot = needIngot + (times - mapNum) * self:GetMapNeedIngot()
	end
	if not self:IsValidSpade(spadeId) then
		return false, string.format("无效的铲子id。", spadeId)
	end
	local spadeNum = user:GetItemNum(USERBASE.PACKAGE, spadeId)
	if spadeNum < times then
		if not autoBuySpade then
			return false, string.format("%s数量不足。", BASEITEM.GetItemName(spadeId))
		end
		print(needIngot, times, spadeNum, self:GetSpadeNeedIngot(spadeId))
		needIngot = needIngot + (times - spadeNum) * self:GetSpadeNeedIngot(spadeId)
	end
	if user:GetTotalIngot() < needIngot then
		return false, "您的元宝不足，请充值。"
	end
	user:SetTotalIngot(-needIngot)
	user:DelItem(USERBASE.PACKAGE, self:GetMapId(), math.min(mapNum, times))
	user:DelItem(USERBASE.PACKAGE, spadeId, math.min(spadeNum, times))
	local prizeGot = {}
	for i = 1, times do
		self:ProDig(user, spadeId, prizeGot)
		if math.random() <= CLSPET.GetExtraItemRate() then
			self:ProDig(user, spadeId, prizeGot, true)
		end
	end
	AddPrizeToUser(user, prizeGot)
	return prizeGot
end

function clsTreasure:ProDig(user, spadeId, prizeGot, petExtra)
	local got = CalTreasureGot(self:GetId(), spadeId, user:GetDigTreasureTimes(spadeId))
	local itemInfo = BASEITEM.GetItemById(got.id)
	assert(itemInfo, string.format("invalid itemId:%s", got.id))
	BASEITEM.CombiItemToList(prizeGot, got.id, got.num)			
	if itemInfo.quality >= NOTICE_NEED_QUALITY then
		local msg = string.format("%s 人品爆发，在%s中获得了[%s]。", CHAT.GetFormatedNick(user:GetName()), self:GetName(), itemInfo.name)
		local tempItem = BASEITEM.NewItem(got.id, got.num)
		CHAT.SendSysMsg("system", msg, nil, {goodsAttr = {[1] = tempItem:GetTipMsg()}, kind = "worldMap", id = self:GetId()})
		self:AddLog(msg)
		self:SendNewLog(msg)
	end
	if petExtra then
		user:SendProtocol("c_show_tips", string.format("本命精灵神力相助，恭喜您获得了【%s】×%d。", itemInfo.name, got.num))
	else
		user:SendProtocol("c_show_tips", string.format("恭喜您获得了【%s】×%d。", itemInfo.name, got.num))
		user:SetDigTreasureTimes(spadeId, 1)
	end	
end

function AddPrizeToUser(user, prizeList)
	local attach = {}
	local count = 1
	for _, v in ipairs(prizeList) do
		local moneyType = MONEY.GetGoldTypeByPicId(v.goodsId)
		if moneyType then
			local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
			USERBASE.clsUser[moneyInfo.set](user, v.num)
		else
			local item = BASEITEM.NewItem(v.goodsId, v.num)
			if user:IsItemCanAdd(USERBASE.PACKAGE, item) then
				user:AddItem(USERBASE.PACKAGE, item)
			else
				attach[count] = attach[count] or {}
				table.insert(attach[count], {id = v.goodsId, num = v.num})
				if #attach[count] >= MAIL.MAIL_ATTACH_NUM then
					count = count + 1
				end
			end
		end
	end
	if #attach > 0 then
		for _, v in ipairs(attach) do
			local mail = {}
			mail.title = "寻宝奖励"
			mail.content = "由于背包已满，您的寻宝奖励已通过邮件发送给您，请及时领取。"
			mail.attach = {item = v}
			MAIL.SendSysMail(user:GetRoleId(), mail)
		end
	end
end

function clsTreasure:GetSaveData()
	return self.__save
end

function clsTreasure:Restore(data)
	self.__save = data
end

function clsTreasure:GetTreasureInfo()
	local sData = {}
	sData.id = self:GetId()
	sData.name = self:GetName()
	sData.picId = self:GetPicId()
	sData.needMap = self:GetMapId()
	sData.mapNeedIngot = self:GetMapNeedIngot()
	sData.spadeList = self:GetSpadeList()
	return sData
end


function __init__()

	local TREASURELIST = Import("autocode/treasureList.lua")
	TreasureList = TREASURELIST.GetTable()
	
	local PRIZELIST = Import("autocode/treasurePrize.lua")
	TreasurePrize = PRIZELIST.GetTable()
	
end
