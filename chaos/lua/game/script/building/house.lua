--民宅

local INIT_PEOPLE_NUM = 4000
-----------------------------------------------------------
clsHouse = STRUCT.clsStruct:Inherit()

function clsHouse:__init__()
	Super(clsHouse).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsHouse:initsavedata()	
	self.__save.Lv = 0
	self.__save.resIncome = 0
end

function clsHouse:inittempdata()
	self.__temp.callout = {}
end

function clsHouse:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:SetTimeStamp(os.time())
	
	local user = assert(USER.GetOnlineUser(owner))
	local tribe = user:GetTribe()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	tribe:SetResourceStat("people", "max", info.upLimit)
	tribe:SetResourceStat("people", "num", INIT_PEOPLE_NUM)
end

function clsHouse:Upgrade()
	self:SetLv(1)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	self:SetResMax(info.upLimit)
end

function clsHouse:GetResIncome()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	return info.speed
end

function clsHouse:SetResMax(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	tribe:SetResourceStat("people", "max", data)
end

function clsHouse:GetResMax()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	return tribe:GetResourceStat("people", "max")
end

function clsHouse:GetTotalResIncomeRate()
	local income = self:GetResIncome()
	return math.floor(income)
end


function clsHouse:ProStructFunc(time)
	local timelong = TRIBE.GetResourceFreshTimelong()
	local user = USER.GetOnlineUser(self:GetOwner())
	if not user then
		return
	end
	local tribe = user:GetTribe()
	local timestamp = self:GetTimeStamp()
	local n = math.floor(((time or os.time()) - timestamp) / timelong)
	if n <= 0 then
		return
	end
	tribe:ModResourceStat("people", "num", math.floor(self:GetTotalResIncomeRate() * timelong * n / 3600))
	self:SetTimeStamp(timestamp + n * timelong)
end
