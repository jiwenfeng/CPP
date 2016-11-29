--建筑基类

local StructExpList = {
	[1] = 50000000, [2] = 100000000, [3] = 200000000, [4] = 350000000, [5] = 550000000,
	[6] = 800000000, [7] = 1100000000, [8] = 1450000000, [9] = 1850000000, [10] = 2300000000,
}

-----------------------------------------------------
clsStruct = clsObject:Inherit()

function clsStruct:__init__()
	Super(clsStruct).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsStruct:initsavedata()
	self.__save.Lv = 0
	self.__save.exp = 0
end

function clsStruct:inittempdata()
end

--初始化
function clsStruct:InitStruct(phyle, structType)
	self:SetPhyle(phyle)
	self:SetStructType(structType)
	self:SetLv(1)
end

--建筑势力
function clsStruct:SetPhyle(data)
	self.__save.phyle = data
end

function clsStruct:GetPhyle()
	return self.__save.phyle
end
--建筑类型
function clsStruct:SetStructType(data)
	self.__save.structType = data
end

function clsStruct:GetStructType()
	return self.__save.structType
end

--建筑等级
function clsStruct:GetMaxLv()
	return #StructExpList
end

function clsStruct:GetLv()
	return self.__save.Lv
end

function clsStruct:SetLv(data)
	self.__save.Lv = (self.__save.Lv or 0) + data
	if self.__save.Lv < 0 then
		self.__save.Lv = 0
	elseif self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	end
end

--建设值
function clsStruct:GetExpMax()
	return StructExpList[self:GetLv()] or StructExpList[#StructExpList]
end

function clsStruct:GetExp()
	return self.__save.exp
end

function clsStruct:SetExp(data)
	self.__save.exp = (self.__save.exp or 0) + data
	self:CheckUpgrade()
end

function clsStruct:CheckUpgrade()
	local expMax = self:GetExpMax()
	if self.__save.exp >= expMax then
		if self:GetLv() < self:GetMaxLv() then
			self.__save.exp = self.__save.exp - expMax
			return self:Upgrade()
		else
			self.__save.exp = expMax
		end
	end
end

--建筑升级
function clsStruct:Upgrade()
	self:SetLv(1)
	CHAT.SendSysMsg("phyle", string.format("经过众人的不懈努力，势力【%s】等级提升到%d级！", self:GetStructName(), self:GetLv()))
	self:CheckUpgrade()
end


function clsStruct:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsStruct:Restore(data)
	self.__save = data
end





