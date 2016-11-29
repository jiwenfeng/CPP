
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
	self.__save.data = nil
end

function clsItem:inittempdata()

end


function clsItem:InitItem(id, num, _, _, _, _, data)
	Super(clsItem).InitItem(self, id, num)		--调用父类函数
	self:SetHeroData(data)
end

function clsItem:SetHeroData(data)
	self.__save.data = data
end

function clsItem:GetHeroData()
	return self.__save.data
end

function clsItem:GetName()
	local type = self.__save.data.type
	if type == "famousHero" then
		return string.format("英魂·%s", self.__save.data.name)
	elseif type == "godHero" then
		return string.format("神魂·%s", self.__save.data.name)
	end
	return string.format("武魂·%s", self.__save.data.name)
end

function clsItem:GetQuality()
	local aptitude = self.__save.data.aptitude or self.__save.data.aptLv
	return CLSHERO.GetAptLvByAptitude(aptitude)
end

function clsItem:GetNeedLv()
	return self.__save.data.Lv
end

function clsItem:GetAttrList()
	local sData = {}
	sData.hp = self.__save.data.attrib.hpMax
	sData.hpMax = self.__save.data.attrib.hpMax
	sData.physcAtkMin = self.__save.data.attrib.physcAtk.min + self.__save.data.attrib.physcAtk.add
	sData.physcAtkMax = self.__save.data.attrib.physcAtk.max + self.__save.data.attrib.physcAtk.add
	sData.physcDef = self.__save.data.attrib.physcDef.base + self.__save.data.attrib.physcDef.add
	sData.magicAtkMin = self.__save.data.attrib.magicAtk.min + self.__save.data.attrib.magicAtk.add
	sData.magicAtkMax = self.__save.data.attrib.magicAtk.max + self.__save.data.attrib.magicAtk.add
	sData.magicDef = self.__save.data.attrib.magicDef.base + self.__save.data.attrib.magicDef.add
	sData.speed = self.__save.data.attrib.speed
	return sData
end

function clsItem:GetFullAttr()
	local sData = {}
	sData.goodsId = self:GetId()
	sData.name = self:GetName()
	sData.usable = self:IsItemUsable() or 0
	local info = self.__save.data.info
	if info then
		sData.picId = info.headId
		sData.job = info.job
	else
		sData.job = self.__save.data.job
	end
	sData.quality = self:GetQuality()
	sData.needLv = self:GetNeedLv()
	sData.attr = self:GetAttrList()
	sData.lock = 0
	return sData
end

--使用物品
function clsItem:Use(obj)
	if obj:GetRoleType() ~= "user" then
		return false, "该物品只能由主角使用。"
	end
	if obj:GetLv() < self:GetNeedLv() then
		return false, "主角等级不足，不能使用该精魄。"
	end
	obj:AddHeroBySave(self:GetHeroData())
	return true
end
