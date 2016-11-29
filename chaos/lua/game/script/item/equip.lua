--装备类 (equip)
--特点：该类物品具备星级，可以镶嵌宝石，可以装备

local EquipList = BASEITEM.GetItemListByClass("equip")

function GetEquipList()
	return EquipList
end

function GetEquipInfo(id)
	return EquipList[id]
end

local MAX_STAR = 12						--最大星级
local MAX_HOLE = 3
local DEFAULT_EQUIP_RATE = 0.8			--装备默认品级

local EnhanceEffect = {
	["weapon"] = {
		[0] = 0, [1] = 0.02, [2] = 0.04, [3] = 0.08, [4] = 0.14, [5] = 0.22, [6] = 0.32,
		[7] = 0.44, [8] = 0.58,	[9] = 0.74, [10] = 0.92, [11] = 1.12, [12] = 1.34,
	},
	["armor"] = {
		[0] = 0, [1] = 0.02, [2] = 0.03, [3] = 0.06, [4] = 0.11, [5] = 0.17, [6] = 0.24,
		[7] = 0.33, [8] = 0.44, [9] = 0.56, [10] = 0.69, [11] = 0.84, [12] = 1.01,
	}
}

local ExtendAttrList = {
	["weapon"] = {{type = "fatal", num = 100}, {type = "crush", num = 100}, {type = "harmDeep", num = 200},},
	["armor"] = {{type = "jink", num = 20}, {type = "physcResist", num = 40}, {type = "magicResist", num = 40},},
}

local gemHoleNeedQuality = {[1] = 2, [2] = 4, [3] = 6}
--[[
local forceRateList = {			--属性对战斗力的加成影响
	["strength"] = 10, ["intellect"] = 10, ["agility"] = 15, ["captain"] = 15, ["hpMax"] = 1, 
	["physcAtk"] = 2, ["physcDef"] = 3, ["magicAtk"] = 2, ["magicDef"] = 3, ["speed"] = 4, 
	["jink"] = 5, ["fatal"] = 2, ["crush"] = 2, ["harmDeep"] = 2, 
	["physcResist"] = 3, ["magicResist"] = 3
}

function CalAttrFightForce(buff)
	local force = 0
	for k, v in pairs(forceRateList) do
		force = force + (buff[k] or 0) * v
	end
	return force
end]]

--装备品级列表
local EquipGradeList = {
	[1] = {min = 70, max = 80, type = "rejects"},
	[2] = {min = 80, max = 85, type = "normal"},
	[3] = {min = 85, max = 90, type = "good"},
	[4] = {min = 90, max = 100, type = "masterpiece"},
}

function GetEquipGradeInfoById(id)
	return EquipGradeList[id]
end
-------------------------------------------------------

clsItem = BASEITEM.clsBaseItem:Inherit()

function clsItem:__init__()
	Super(clsItem).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsItem:initsavedata()
	self.__save.id = 0
	self.__save.num = 0
	self.__save.star = 0		--星级
	self.__save.holes = {}		--装备打孔
end

function clsItem:inittempdata()

end

function clsItem:InitItem(id, num, missionId, lock, star, rate, save)
	Super(clsItem).InitItem(self, id, num, missionId, lock)	--调用父类函数
	if not save then
		self:SetStar(star or 0)
		if rate then
			self:SetGrade(rate)
		else
			self:ResetGrade()
		end
		local info = GetEquipInfo(id)
		self:CheckHoleList()
	else
		self:Restore(save)
		self:SetNum(num)
	end
end

function clsItem:IsSameItem(Obj)
	return false
end

--星级
function clsItem:GetStar()
	return self.__save.star or 0
end

function clsItem:SetStar(star)
	self.__save.star = star
	if self.__save.star < 0 then 
		self.__save.star = 0
	elseif self.__save.star >= MAX_STAR then
		self.__save.star = MAX_STAR
		self:GenExtendAttr()
	end
end

function clsItem:ModStar(data)
	self.__save.star = self.__save.star + data
	if self.__save.star < 0 then
		self.__save.star = 0
	elseif self.__save.star >= MAX_STAR then
		self.__save.star = MAX_STAR
		self:GenExtendAttr()
	end
end

function clsItem:IsMaxStar()
	return self.__save.star >= MAX_STAR
end


--提灵次数
function clsItem:SetPromoteTimes(data)
	self.__save.promoteTms = data
end

function clsItem:GetPromoteTimes()
	return self.__save.promoteTms or 0
end

function clsItem:ModPromoteTimes(data)
	self.__save.promoteTms = (self.__save.promoteTms or 0) + data
	if self.__save.promoteTms < 0 then
		self.__save.promoteTms = 0
	end
end

--注灵属性
function clsItem:SetInjectInfo(attr)
	self.__save.inject = attr
end

function clsItem:GetInjectInfo()
	return self.__save.inject
end

--临时注灵属性
function clsItem:SetTempInject(attr)
	self.__temp.inject = attr
end

function clsItem:GetTempInject()
	return self.__temp.inject
end

--替换注灵属性
function clsItem:SaveInjectInfo()
	local attr = self:GetTempInject()
	if attr then
		self:SetInjectInfo(attr)
		self:SetPromoteTimes(0)
		self:SetTempInject()
	end
end


function clsItem:GenGradeById(id)
	local info = GetEquipGradeInfoById(id)
	self:SetGrade(math.random(info.min, info.max) / 100)
end

function clsItem:IsWeapon()
	local info = GetEquipInfo(self:GetId())
	if info.type == "weapon" then
		return true
	end
	return false
end


--品级
function clsItem:SetGrade(grade)
	self.__save.grade = grade
end

function clsItem:GetGrade()
	return self.__save.grade
end

function clsItem:ResetGrade()
	local grade = math.random(1, #EquipGradeList)
	self:SetGrade(math.random(EquipGradeList[grade].min, EquipGradeList[grade].max) / 100)
end

function clsItem:GetGradeLv()
	local grade = self:GetGrade() * 100
	for k, v in ipairs(EquipGradeList) do
		if grade >= v.min and grade <= v.max then
			return k
		end
	end
end

--获取装备基础属性
function clsItem:GetBaseAttr()
	local grade = self:GetGrade()
	local buff = {}
	for k, v in pairs(GetEquipInfo(self:GetId()).attr) do
		buff[k] = math.floor(v * grade)
	end
	return buff
end


function clsItem:IsInUse()
	return self.__save.inUse
end

function clsItem:SetInUse(data)
	self.__save.inUse = data
end


--额外属性
function clsItem:GenExtendAttr()
	local ExtendList = self:IsWeapon() and ExtendAttrList["weapon"] or ExtendAttrList["armor"]
	local quality = self:GetQuality()
	if not self.__save.extend then
		if self:GetStar() >= MAX_STAR then
			local chosen = ExtendList[math.random(1, #ExtendList)]
			local attr = {}
			attr[chosen.type] = chosen.num * quality
			self.__save.extend = attr
		end
	else
		for k in pairs(self.__save.extend) do
			self.__save.extend[k] = self.__save.extend[k] * quality
		end
	end
end

function clsItem:GetExtendAttr()
	return self.__save.extend
end


function clsItem:GetWearTarget()
	return GetEquipInfo(self:GetId()).target
end

--获取强化预览信息
function clsItem:GetEnhencePreview(newStar, oldStar)	
	local EnhanceInfo = self:IsWeapon() and EnhanceEffect.weapon or EnhanceEffect.armor
	newStar = newStar or self:GetStar()
	newStar = math.min(MAX_STAR, math.max(0, newStar))
	local sData = {}
	for k, v in pairs(self:GetBaseAttr()) do
		sData[k] = math.floor(v * (1 + EnhanceInfo[newStar]))
		if oldStar then
			oldStar = math.min(MAX_STAR, math.max(0, oldStar))
			sData[k] = math.floor(v * (EnhanceInfo[newStar] - EnhanceInfo[oldStar]))
		end
	end
	return sData
end

--获取进阶预览信息
function clsItem:GetUpgradePreView(lossLv)
	local EnhanceInfo = self:IsWeapon() and EnhanceEffect.weapon or EnhanceEffect.armor
	local star = self:GetStar()
	local itemInfo = BASEITEM.GetItemById(self:GetId())
	local sData = {}
	local nextInfo = BASEITEM.GetItemById(itemInfo.complex)
	if nextInfo then
		sData.star = star - lossLv
		for k, v in pairs(nextInfo.attr) do
			sData[k] = math.floor(v * (1 + EnhanceInfo[math.max(star - lossLv, 1)]))
		end
	end
	return sData
end

--装备打孔
function clsItem:CheckHoleList()
	local quality = self:GetQuality()
	for k, v in ipairs(gemHoleNeedQuality) do
		if quality >= v then
			self:AddOneHole(k)
		end
	end
end

function clsItem:AddOneHole(pos)
	self.__save.holes[tostring(pos)] = {}
end

function clsItem:GetOneHole(pos)
	return self.__save.holes[tostring(pos)]
end

function clsItem:GetHoleNum()
	return table.size(self.__save.holes)
end


--镶嵌宝石
function clsItem:AddOneGem(pos, gem)
	self.__save.holes[tostring(pos)] = gem
end

function clsItem:DelOneGem(pos)
	self.__save.holes[tostring(pos)] = {}
end

function clsItem:GetOneGem(pos)
	return self.__save.holes[tostring(pos)]
end

function clsItem:IsSameGemExist(gemId)
	local gemInfo = BASEITEM.GetItemById(gemId)
	for k, v in pairs(self.__save.holes) do
		if v.id then
			local itemInfo = BASEITEM.GetItemById(v.id)
			if itemInfo.kind == gemInfo.kind then
				return true
			end
		end
	end
	return false
end

--获取下一个可镶嵌的位置
function clsItem:GetNextGemPos()
	for i = 1, MAX_HOLE do
		local info = self:GetOneHole(i)
		if info and not info.id then
			return tostring(i)
		end
	end
end

--获取装备开孔状态
function clsItem:GetHoleList()
	local buff = {}
	for i = 1, MAX_HOLE do
		local pos = tostring(i)
		local info = self:GetOneGem(pos)
		if info then
			local temp = {index = pos}
			if info.id then
				local itemInfo = BASEITEM.GetItemById(info.id)
				temp.state = "enchased"
				temp.goodsId = info.id
		--		temp.goodsName = itemInfo.name
		--		temp.quality = itemInfo.quality
		--		temp.attr = itemInfo.attr
			else
				temp.state = "empty"
			end
			table.insert(buff, temp)
		end
	end
	return buff
end

--宝石列表
function clsItem:GetGemList()
	local buff = {}
	for k, v in pairs(self.__save.holes) do
		if v.id then
			table.insert(buff, {pos = k, goodsId = v.id})
		else
			table.insert(buff, {pos = k})
		end
	end
	return buff
end

function clsItem:GetNeedLv()
	return GetEquipInfo(self:GetId()).needLv
end

function clsItem:GetNeedStatus()
	return GetEquipInfo(self:GetId()).needStatus or 0
end
--获取物品的装备位置
function clsItem:GetEquipPos()
	return GetEquipInfo(self:GetId()).wearPos
end

--获取物品的装备对象
function clsItem:GetTarget()
	return GetEquipInfo(self:GetId()).target
end


local forceRateList = {			--属性对战斗力的加成影响
	["strength"] = 10, ["intellect"] = 10, ["agiltiy"] = 15, ["capttain"] = 15, ["hpMax"] = 1, 
	["physcAtk"] = 2, ["physcAtkMax"] = 2, ["physcDef"] = 3, ["magicAtk"] = 2, ["magicAtkMax"] = 2, ["magicDef"] = 3, 
	["speed"] = 4, ["jink"] = 5, ["fatal"] = 2, ["crush"] = 2, ["harmDeep"] = 2, 
	["physcResist"] = 3, ["magicResist"] = 3
}

function clsItem:CalFightForce()
	local force = 0
	local attrList = self:GetEquipAttrAdd()
	for k, v in pairs(attrList) do
		force = force + (forceRateList[k] or 0) * v
	end
	return math.ceil(force)
end

--获取装备全部属性
function clsItem:GetEquipAttrAdd()
	local sData = {}
	local info = self:IsWeapon() and EnhanceEffect.weapon or EnhanceEffect.armor
	local star = self:GetStar()
	for k, v in pairs(self:GetBaseAttr()) do
		sData[k] = math.floor(v * (1 + info[star]))
	end
	for k, v in pairs(self:GetExtendAttr() or {}) do
		sData[k] = (sData[k] or 0) + v
	end
	for k, v in pairs(self:GetInjectInfo() or {}) do
		sData[v.type] = (sData[v.type] or 0) + v.num
	end
	for k, v in pairs(self.__save.holes) do
		if v.id then
			local gemInfo = BASEITEM.GetItemById(v.id)
			for key, val in pairs(gemInfo.attr) do
				sData[key] = (sData[key] or 0) + val
			end
		end
	end
	return sData
end

--获取装备完整属性
function clsItem:GetFullAttr()
	local info = self:IsWeapon() and EnhanceEffect.weapon or EnhanceEffect.armor
	local buff = {goodsId = self:GetId()}
	buff.grade = self:GetGradeLv()
	local star = self:GetStar()
	buff.star = star
	buff.baseAttr = {}			--主属性
	for k, v in pairs(self:GetBaseAttr()) do
		buff.baseAttr[k] = math.floor(v * (1 + info[star]))
	end
	buff.extAttr = self:GetExtendAttr()		--扩展属性
	buff.injectAttr = {}					--注灵属性
	for k, v in ipairs(self:GetInjectInfo() or {}) do
		buff.injectAttr[v.type] = v.num
	end
	buff.holeList = self:GetHoleList()		--镶嵌属性
	buff.lock = self:IsLocked() or 0
--	buff.missionId = self:GetMissionId()
	local equipInfo = EquipList[self:GetId()]
	if equipInfo.recycleGold then
		buff.goldType = "gold"
		buff.price = equipInfo.recycleGold
	end
	local suitId = self:GetSuitId()
	if suitId then
		buff.suitId = suitId
		buff.suitNum = 0
		buff.suitMax = self:GetSuitElementMax()
	end
	buff.force = self:CalFightForce()
	return buff
end


function clsItem:GetNeedJob()
	return GetEquipInfo(self:GetId()).needJob
end


function clsItem:GetSuitId()
	return GetEquipInfo(self:GetId()).suitId
end

function clsItem:GetSuitElementMax()
	local suitInfo = SUIT.GetSuitInfoById(self:GetSuitId())
	if not suitInfo then
		return 0
	else
		return #suitInfo.list
	end
end


--将target的属性拷贝给item
function clsItem:CopyAttr(equip)
	local attrList = equip:GetSaveData()
	self.__save.holes = attrList.holes
	self.__save.inject = attrList.inject
	self.__save.promoteTms = attrList.promoteTms
end



--使用装备
function clsItem:Use(obj, num)
	return false, "装备不可以直接使用。"
end
