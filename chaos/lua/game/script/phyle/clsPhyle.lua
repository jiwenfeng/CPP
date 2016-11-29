--势力类

local WEEK_DAYS = 7

local phyleStruct = {
	["munition"] = MUNITION.clsMunition, 
	["altar"] = ALTAR.clsAltar,
	["totem"] = TOTEM.clsTotem,
}

function GetStructList()
	return phyleStruct
end

----------------------------------------
clsPhyle = clsObject:Inherit()

function clsPhyle:__init__()
	Super(clsPhyle).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsPhyle:initsavedata()
	self.__save.degree = 0
end

function clsPhyle:inittempdata()
	self.__temp.struct = {}
end

--初始化
function clsPhyle:InitPhyle(phyle)
	self:SetType(phyle)
	self:InitStructList()
end
--势力类型
function clsPhyle:SetType(data)
	self.__save.type = data
end

function clsPhyle:GetType()
	return self.__save.type
end

--建设值
function clsPhyle:GetDegree()
	return self.__save.degree
end

function clsPhyle:SetDegree(data)
	self.__save.degree = (self.__save.degree or 0) + data
	self:SetWeeklyDegree(data)
end
--每周建设值
function clsPhyle:SetWeeklyDegree(data)
	self.__save.weeklyDegree = self.__save.weeklyDegree or {}
	local info = self.__save.weeklyDegree
	if not info.time or CalDiffDays(info.time, os.time()) > WEEK_DAYS then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsPhyle:GetWeeklyDegree()
	self.__save.weeklyDegree = self.__save.weeklyDegree or {}
	local info = self.__save.weeklyDegree
	if not info.time or CalDiffDays(info.time, os.time()) > WEEK_DAYS then
		return 0
	end
	return info.num
end


function clsPhyle:GetStruct(structType)
	return self.__temp.struct[structType]
end

function clsPhyle:GetStructList()
	return self.__temp.struct
end


function clsPhyle:InitStructList()
	for k, v in pairs(phyleStruct) do
		local struct = v:New()
		struct:InitStruct(self:GetType(), k)
		self.__temp.struct[k] = struct
	end
end


function clsPhyle:SavePhyleData()
	self.__save.struct = {}
	for k, v in pairs(self.__temp.struct) do
		self.__save.struct[k] = v:GetSaveData()
	end
	return table.deepcopy(self.__save)
end


function clsPhyle:RestorePhyleData(data)
	self.__save = data
	for k, v in pairs(self.__save.struct) do
		local struct = phyleStruct[k]:New()
		struct:Restore(v)
		self.__temp.struct[k] = struct
	end
end

function clsPhyle:SetMemberNum(data)
	self.__save.num = math.max((self.__save.num or 0) + data, 0)
end

function clsPhyle:GetMemberNum()
	return self.__save.num or 0 
end

function clsPhyle:SetPhylePrize(prizeList)
	self.__save.weeklyPrize = self.__save.weeklyPrize or {}
	self.__save.weeklyPrize.prize = prizeList
	self.__save.weeklyPrize.userList = {}
end

function clsPhyle:GetPhylePrize()
	self.__save.weeklyPrize = self.__save.weeklyPrize or {}
	return self.__save.weeklyPrize.prize
end

function clsPhyle:ClearPhylePrizeInfo()
	self.__save.weeklyPrize = self.__save.weeklyPrize or {}
	self.__save.weeklyPrize.userList = {}
end

function clsPhyle:SetPhylePrizeReceived(roleId)
	if not self.__save.weeklyPrize or not self.__save.weeklyPrize.userList then
		return
	end
	self.__save.weeklyPrize.userList[roleId] = 1
end

function clsPhyle:IsPhylePrizeReceived(roleId)
	if not self.__save.weeklyPrize or not self.__save.weeklyPrize.userList then
		return
	end
	return self.__save.weeklyPrize.userList[roleId]
end

