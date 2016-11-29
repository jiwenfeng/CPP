
local INIT_SPAR_NUM = 1000


clsSpar = STRUCT.clsStruct:Inherit()

function clsSpar:__init__()
	Super(clsSpar).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedate()
	self:inittempdate()
end

function clsSpar:initsavedate()
	self.__save.Lv = 0
	self.__save.timestamp = 0
end

function clsSpar:inittempdate()
	self.__temp.callout = {}
end

function clsSpar:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetStructType(structType)
	self:SetType(type)
	self:SetTimeStamp(os.time())
	
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())

	tribe:SetResourceStat("spar", "max", info.upLimit)
	tribe:SetResourceStat("spar", "num", INIT_SPAR_NUM)
end

function clsSpar:Upgrade()
	self:SetLv(1)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	self:SetResMax(info.upLimit)
end

--资源基本增长速率
function clsSpar:GetResIncome()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	return info.speed
end

function clsSpar:SetResMax(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	tribe:SetResourceStat("spar", "max", data)
end

function clsSpar:GetResMax()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	return tribe:GetResourceStat("spar", "max")
end

--资源总增长速率
function clsSpar:GetTotalResIncomeRate()
	local income = self:GetResIncome()
	local phyle = USER.GetUserNickInfo(self:GetOwner(), "phyle")
	local user = USER.GetOnlineUser(self:GetOwner())
	local tribe = user:GetTribe()
	if phyle and phyle == "phoenix" then
		income = income * (1 + 0.02)
	end
	return math.floor(income) + ACADEMY.GetTechEffectByType(self:GetOwner(), "sparRate")
end


function clsSpar:ProStructFunc()
	local timelong = TRIBE.GetResourceFreshTimelong()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	local timestamp = self:GetTimeStamp()
	local n = math.floor(((time or os.time()) - timestamp) / timelong)
	if n <= 0 then
		return
	end
	tribe:ModResourceStat("spar", "num", math.floor(self:GetTotalResIncomeRate() * timelong * n / 3600))
	self:SetTimeStamp(timestamp + n * timelong)
end
