--仙器类

local FairyDeviceExp = {}
local FairyDevList = BASEITEM.GetItemListByClass("fairyDev")

function GetUpgradeExp(quality, lv)
	return FairyDeviceExp[quality][lv].needExp
end

local FairyDevPriceList = {1000, 2000, 3000, 5000, 8000}
local DEVOUR_EXP_RATE = 1
----------------------------------------------------

clsItem = BASEITEM.clsBaseItem:Inherit()

function clsItem:InitItem(id, num, missionId, lock)
	self:initsavedata()
	self:SetId(id)
	self:SetLv(1)
	self:SetNum(num)
	self:SetMissionId(missionId)
	self:SetLocked(lock)
	self:GenSkill()
end

function clsItem:initsavedata()
	self.__save.Lv = 0
	self.__save.exp = 0
	self.__save.num = 0
--	self.__save.lock = false
end


function clsItem:SetExp(exp)
	self.__save.exp = (self.__save.exp or 0) + exp
	if self.__save.exp >= self:GetMaxExp() then
		self.__save.exp = self.__save.exp - self:GetMaxExp()
		self:Upgrade()
	end
end

function clsItem:GetExp()
	return self.__save.exp
end

function clsItem:GetMaxExp()
	return GetUpgradeExp(self:GetQuality(), self:GetLv())
end

function clsItem:Upgrade()
	if self:GetLv() >= self:GetMaxLv() then
		self.__save.exp = math.min(self:GetExp(), self:GetMaxExp())
	else
		self:SetLv(1)
		if self.__save.exp >= self:GetMaxExp() then
			self.__save.exp = self.__save.exp - self:GetMaxExp()
			return self:Upgrade()
		end
	end
end

function clsItem:SetLv(data)
	self.__save.Lv = self.__save.Lv + data
	if self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	elseif self.__save.Lv < 1 then
		self.__save.Lv = 1
	end
end

function clsItem:GetLv()
	return self.__save.Lv
end

function clsItem:ResetLv(Lv)
	self.__save.Lv = Lv or 1
end

function clsItem:GetMaxLv()
	local info = FairyDevList[self:GetId()]
	return #info.lvList
end
--升级所需经验
function clsItem:GetNeedExp()
	return GetUpgradeExp(self:GetQuality(), self:GetLv())
end
--吞噬附加经验
function clsItem:GetAddExp()
	local exp = self:GetExp() + FairyDeviceExp[self:GetQuality()][self:GetLv()].addExp
	return math.floor(exp * DEVOUR_EXP_RATE)
end

function clsItem:CanBeUpgrade(exp)
	local lv = self:GetLv()
	local totalExp = self:GetExp() + exp
	local quality = self:GetQuality()
	while true do 
		local needExp = GetUpgradeExp(quality, lv)
		if totalExp >= needExp and lv < self:GetMaxLv() then
			totalExp = totalExp - needExp
			lv = lv + 1
		else
			break
		end
	end
	if lv == self:GetLv() then
		return
	end
	return lv
end

function clsItem:GetPrice()
	return FairyDevPriceList[self:GetQuality()]
end

function clsItem:SetSkill(data)
	self.__save.skill = data
end

function clsItem:GetSkill()
	return self.__save.skill
end

function clsItem:GetNextDesc()
	local info = FairyDevList[self:GetId()]
	return info.lvList[math.min(self:GetLv() + 1, #info.lvList)].describe
end

function clsItem:GetDescrip(lv)
	local info = FairyDevList[self:GetId()]
	lv = math.min(lv or self:GetLv(), #info.lvList)
	return info.lvList[lv].describe
end

function clsItem:GetFullAttr()
	local sData = {}
	sData.goodsId = self:GetId()
	sData.quality = self:GetQuality()
	sData.num = self:GetNum()
	sData.lock = self:IsLocked() or 0
	sData.Lv = self:GetLv()
	if self:GetQuality() > 1 then
		sData.exp = self:GetExp()
		sData.expMax = self:GetNeedExp()
	--	sData.addExp = self:GetAddExp()
		local fairyDevInfo = FairyDevList[self:GetId()]
		sData.describe = fairyDevInfo.lvList[self:GetLv()].describe
		if fairyDevInfo.lvList[self:GetLv() + 1] then
			sData.nextDesc = fairyDevInfo.lvList[self:GetLv() + 1].describe
		end
	end
--	sData.price = self:GetPrice()
	local skillInfo = self:GetSkill()
	if skillInfo then
		sData.skillName = SKILL.GetSkillNameById(skillInfo.id)
		sData.skillLv = skillInfo.lv
	end
	sData.merge = self:IsFusable()
	if self:GetQuality() >= 5 then
		sData.detris = self:IsRefinable() and 1 or 2
	end
	return sData
end

function clsItem:GetAttrList()
	local info = FairyDevList[self:GetId()]
	return info.lvList[self:GetLv()].attr
end

function clsItem:IsRange()
	local info = FairyDevList[self:GetId()]
	return info.lvList[self:GetLv()].isRange
end

function clsItem:IsFusable()
	local info = FairyDevList[self:GetId()]
	return info.lvList[self:GetLv()].isFusable
end

function clsItem:IsRefinable()
	local info = FairyDevList[self:GetId()]
	return info.isRefinable
end

function clsItem:Use(obj, num)
	return false, "该物品不能直接使用。"
end

function clsItem:GenSkill()
	local info = BASEITEM.GetItemById(self:GetId())
	local num
	local rate = 0
	local ran = math.random()
	for k, v in pairs(info.rateList) do
		rate = rate + v.rate
		if ran <= rate then
			num = v.num
			break
		end
	end
	if num and num > 0 then
		local skillList = FAIRYDEV.GetAllSkillList()
		local buf = {}
		buf.lv = num
		buf.id = skillList[math.random(1, #skillList)]
		self:SetSkill(buf)
	end
end

function __init__()
	local FAIRYDEVEXP = Import("autocode/fairyDeviceExp.lua")
	FairyDeviceExp = FAIRYDEVEXP.GetTable()
end
