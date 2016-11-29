--议事厅

local MAX_LEVY_TIMES = 5
local LEVY_TIME_LONG = 3600
-------------------------------------------------
clsMainHall = STRUCT.clsStruct:Inherit()

function clsMainHall:__init__()
	Super(clsMainHall).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsMainHall:initsavedata()	
	self.__save.Lv = 0
	self.__save.levy = {}		--征收
	self.__save.levyGot = 0
end

function clsMainHall:inittempdata()
	self.__temp.wild = {}
end

function clsMainHall:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:AddUpgradeList(owner)
end

function clsMainHall:AddUpgradeList(owner)
	local user = assert(USER.GetOnlineUser(owner))
	local obj = UPGRADELIST.NewList("levy", "1", UPGRADELIST.LIST_UNLOCK)
	user:AddUpgradeList("levy", "1", obj)
end

--征收
function clsMainHall:GetLevyMaxTimes()
	return MAX_LEVY_TIMES
end

function clsMainHall:GetLevyCount()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local obj = user:GetOneUpgradeList("levy", "1")
	if CalDiffDays(obj:GetEndTimestamp() - LEVY_TIME_LONG, os.time()) ~= 0 then
		self.__save.levy.num = 0
	end
	return self.__save.levy.num or 0
end

function clsMainHall:SetLevyCount(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local obj = user:GetOneUpgradeList("levy", "1")
	if CalDiffDays(obj:GetEndTimestamp() - LEVY_TIME_LONG, os.time()) ~= 0 then
		self.__save.levy.num = 0
	end	
	self.__save.levy.num = math.max((self.__save.levy.num or 0) + data, 0)
end

function clsMainHall:ResetLevyCount()
	self.__save.levy.num = 0
	self.__save.levy.timestamp = nil
end

function clsMainHall:GetWildById(id)
	return self.__temp.wild[id]
end

function clsMainHall:SetWildById(id, obj)
	self.__temp.wild[id] = obj
end

function clsMainHall:Havest(id)
	local user = USER.GetOnlineUser(self:GetOwner())
	if not user then return false end
	local tribe = user:GetTribe()
	if not tribe then return false end
	local wild = self:GetWildById(id)
	local ret = {num = wild:GetResNum(), goldType = wild:GetResType()}	
	if ret.goldType == "gold" then
		user:SetGold(ret.num)
	elseif ret.goldType == "spar" then
		tribe:ModResourceStat("spar", "num", ret.num)
	end
	wild:SetResNum(0)
	return ret
end

function clsMainHall:GetFormatWild()
	local sData = {}
	for k, v in pairs(self.__temp.wild) do
		table.insert(sData, v:GetMineInfo())
	end
	return sData
end

function clsMainHall:GetWildMineNum()
	return table.size(self.__temp.wild)
end

function clsMainHall:GetLevyGot()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	return info.revenue
end

function clsMainHall:ProLevy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:SetLevyCount(1)
	if self:GetLevyCount() < MAX_LEVY_TIMES then
		local obj = assert(user:GetOneUpgradeList("levy", "1"))
		obj:SetEndTimestamp(os.time() + LEVY_TIME_LONG)
	end
	local gold = self:GetLevyGot()
	user:SetGold(gold)
end

function clsMainHall:Upgrade()
	self:SetLv(1)
end

function clsMainHall:GetSaveData()
	self.__save.wild = {}
	for k, v in pairs(self.__temp.wild) do
		self.__save.wild[tostring(k)] = 1
	end
	return table.deepcopy(self.__save)
end

function clsMainHall:Restore(data)
	self.__save = data
	self.__temp.wild = {}
	for k, v in pairs(self.__save.wild) do
		local id = tostring(k)
		local wild = WILDMINE.GetWildMineObjById(id)
		self.__temp.wild[id] = wild
	end
end

local s_hall_levyInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local hall = assert(tribe:GetStruct("mainHall"))
	local sData = {}
	sData.levyCount = hall:GetLevyCount()
	sData.levyMax = hall:GetLevyMaxTimes()
	sData.levyGot = hall:GetLevyGot()
	local obj = user:GetOneUpgradeList("levy", "1")
	sData.timelong = math.max(obj:GetEndTimestamp() - os.time(), 0)
	connection:SendProtocol("c_hall_levyInfo", sData)
end

local s_hall_levy = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落，无法进行此操作")
	end
	local hall = assert(tribe:GetStruct("mainHall"))
	if hall:GetLevyCount() >= hall:GetLevyMaxTimes() then
		return ERROR.TipMsg(connection, "您今天的征收次数已经用完")
	end
	local obj = user:GetIdleUpgradeList("levy")
	if not obj then
		return ERROR.TipMsg(connection, "队列冷却时间未到，不能征收")
	end
	hall:ProLevy()
	user:SendUpgradeListUpdate(obj:GetType(), obj:GetId())
	connection:SendProtocol("c_hall_levy", "true")
end

local s_hall_harvest = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	data = tostring(data)	
	local wildMine = WILDMINE.GetWildMineObjById(data)
	if not wildMine then
		return ERROR.TipMsg(connection, "没有指定的野地矿藏信息。")
	end
	if wildMine:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您没有占领该野地")
	end
	local num, resType = wildMine:Havest()
	connection:SendProtocol("c_hall_harvest", {mineId = data, num = num, goldType = resType})
end

local s_hall_wildList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local hall = assert(tribe:GetStruct("mainHall"))
	local sData = hall:GetFormatWild()
	USER.SetUserFocusOn("mainHall", connection.roleId, 1)
	user:SetFocus("mainHall", 1)	
	connection:SendProtocol("c_hall_wildList", sData)
end

local s_hall_leave = function(connection, data)
	if data ~= "leave" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	USER.SetUserFocusOn("mainHall", connection.roleId)
	user:SetFocus()
	connection:SendProtocol("c_hall_leave", data)
end

local s_hall_pandect = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local sData = {}
	for k, v in pairs(STRUCT.GetStructList()) do
		local buf = {}
		buf.type = k
		buf.needLv = v.tribeLv
		table.insert(sData, buf)
	end
	connection:SendProtocol("c_hall_pandect", sData)
end

local s_hall_abandon = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local hall = tribe:GetStruct("mainHall")
	if not hall then
		return ERROR.TipMsg(connection, "您还没有建立议事厅")
	end
	local wild = hall:GetWildById(data)
	if not wild then
		return ERROR.TipMsg(connection, "无效的编号")
	end
	if wild:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "该野地不归你所有")
	end
	hall:SetWildById(id)
	hall:Abandon()
	connection:SendProtocol("c_hall_abandon", data)
end

function __init__()
	PROTOCOL.s_hall_levyInfo = s_hall_levyInfo
	PROTOCOL.s_hall_levy = s_hall_levy
	PROTOCOL.s_hall_harvest = s_hall_harvest
	PROTOCOL.s_hall_wildList = s_hall_wildList
	PROTOCOL.s_hall_leave = s_hall_leave
	PROTOCOL.s_hall_pandect = s_hall_pandect
	PROTOCOL.s_hall_abandon = s_hall_abandon
end
