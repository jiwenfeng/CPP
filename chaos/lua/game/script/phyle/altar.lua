--祭坛

local PrayTimesList = {
	[0] = 0, [1] = 3, [2] = 6, [3] = 9, [4] = 12, [5] = 15,
	[6] = 18, [7] = 21, [8] = 24, [9] = 27, [10] = 30,
}

local PrayGotList = {}

function GetAllPrayList()
	return PrayGotList
end
----------------------------------
clsAltar = PHYLESTRUCT.clsStruct:Inherit()

function clsAltar:__init__()
	Super(clsAltar).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsAltar:initsavedata()
	self.__save.Lv = 0
	self.__save.exp = 0
end

function clsAltar:inittempdata()
end

--初始化
function clsAltar:InitStruct(phyle, structType)
	self:SetPhyle(phyle)
	self:SetStructType(structType)
	self:SetLv(1)
end

function clsAltar:GetStructName()
	return "祭坛"
end

function clsAltar:GetMaxPrayTimes(lv)
	lv = lv or self:GetLv()
	return PrayTimesList[lv]
end

--计算祈祷收益
function clsAltar:GenPrayItem()
	local index = math.random(1, #PrayGotList)
	local ran = math.random()
	local rate = 0
	for k, v in ipairs(PrayGotList[index]) do
		rate = rate + v.rate
		if rate >= ran then
			local ret = {}
			if v.goodsId then
				ret.goodsId = v.goodsId
			else
				local buff = BASEITEM.GetItemListByKind(v.type, v.Lv)
				if #buff < 1 then
					DATA.log(string.format("type = %s, Lv = %s.", v.type, v.Lv), "altar.log")
				end
				local id = buff[math.random(1, #buff)]
				ret.goodsId = id
			end
			ret.num = v.num
			return index, ret, k
		end
	end
end





function __init__()

	local PRAYLIST = Import("autocode/phylePrayList.lua")
	PrayGotList = PRAYLIST.GetTable()
end
