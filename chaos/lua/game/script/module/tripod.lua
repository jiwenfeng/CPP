
local tripodList = {}

function GetTripodList()
	return tripodList
end

function GetTripodById(id)
	return tripodList[id]
end

function AddTripod(id, obj)
	tripodList[id] = obj
end

function InitAllTripod(list)
	for k, v in pairs(list) do
		local obj = clsTripod:New()
		obj:NewTripod(k, v)
		AddTripod(k, obj)
	end
end

clsTripod = clsObject:Inherit()

function clsTripod:__init__()
	Super(self).__init__(self)
	self.__temp = {}
	self:InitTempData()
end

function clsTripod:InitTempData()
end

function clsTripod:NewTripod(id, data)
	self.__temp = data
	self.__temp.id = id
end

function clsTripod:GetName()
	return self.__temp.name
end

function clsTripod:GetId()
	return self.__temp.id
end

function clsTripod:GetIngotCost()
	return self.__temp.needIngot
end

function clsTripod:GetGoldCost()
	return self.__temp.needGold
end

function clsTripod:GetRateList()
	return self.__temp.rateList
end

function clsTripod:GetData()
	return self.__temp
end

function clsTripod:GenQuality()
	local ran = math.random()
	local rate = 0
	for _, v in ipairs(self:GetRateList()) do
		rate = rate + v.rate
		if rate >= ran then
			return v.quality
		end
	end
end

function clsTripod:GetNeedVipLv()
	return self.__temp.needVipLv
end

function clsTripod:CanRefine(user, isIngot)
	if user:IsFairyBagFull(USERBASE.FAIRY_BAG) then
		return false, "您的仙器背包已满。"
	end
	if not isIngot and user:GetCurTripod() ~= self:GetId() then
		return false, "该鼎没有激活"
	elseif not isIngot and user:GetCurTripod() == self:GetId() and user:GetGold() < self:GetGoldCost() then
		return false, "钱币不足"
	end
	if isIngot then
		local cost = self:GetIngotCost()
		if not cost then
			return false, "该鼎不能使用元宝炼制"
		elseif cost > user:GetTotalIngot() then
			return false, string.format("使用元宝炼制该鼎需要花费%d元宝，您元宝数量不足", cost)
		end
	end
	return true
end

function clsTripod:ActivateNextTripod()
	local flag = true
	if self.__temp.forward < math.random() then
		flag = false
	end
	return flag
end

function clsTripod:GetNextTripod()
	return self.__temp.next
end

function clsTripod:Refine(user, isIngot, flag)
	local ret, msg = self:CanRefine(user, isIngot)
	if not ret then
		return ret, msg
	end
	local sData = {}
	local ingot
	local gold
	if isIngot then
		user:SetTotalIngot(-self:GetIngotCost())
		ingot = self:GetIngotCost()
	else
		user:SetGold(-self:GetGoldCost())
		gold = self:GetGoldCost()
	end

	local quality = self:GenQuality()
	local list = FAIRYDEV.GetDeviceList()[quality]
	local devId = list[math.random(1, #list)]
	local devObj = BASEITEM.NewItem(devId, 1)
	local pos = user:AddFairyDev(USERBASE.FAIRY_BAG, devObj, flag)	
	if flag then
		sData = devObj:GetFullAttr()
		sData.goodsId = nil
	end
	sData.fairyDevId = devId
	sData.ingot = ingot
	sData.gold = gold
	sData.pos = pos
	sData.trpdId = self:GetId()
	if self:ActivateNextTripod() and not isIngot then
		user:SetCurTripod(self:GetNextTripod())
		sData.active = self:GetNextTripod()
	else
		user:SetCurTripod(1) 
		sData.active = 1
	end
	if devObj:GetQuality() == 5 then
		CHAT.SendSysMsg("system", string.format("%s 鸿运当头，炼制出极品仙器[%s]。", CHAT.GetFormatedNick(user:GetName()) , devObj:GetName()), nil, {goodsAttr = {[1] = devObj:GetTipMsg()}})
	end
	user:SetFuncStatById(SETTING.FUNC_FAIRYDEV, 1)
	MISSION.CheckMissionEvent(user, "func", SETTING.FUNC_FAIRYDEV, 1)
	return true, sData
end

function __init__()
	local TRIPOD = Import("autocode/tripodList.lua")
	InitAllTripod(TRIPOD.GetTable())
end


