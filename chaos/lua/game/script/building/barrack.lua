
GRID_LOCK = 1
GRID_UNLOCK = 2

local TRAIN_OVER_COST = 20
local ENLIST_TIME_LONG = 3600			--强制征兵间隔
local SOLDIER_CONVERSION_RATE = 0.1		--人口to兵力转换率
local EXP_DOUBLE_COST = 20
local MAX_ENLIST_TIMES = 5
local EXP_ADD_FREQUENCE = 30 * 60


local GridConfList = {
	[1] = {state = GRID_UNLOCK, cost = 0}, 
	[2] = {state = GRID_UNLOCK, cost = 0},
	[3] = {state = GRID_LOCK, cost = 100},
	[4] = {state = GRID_LOCK, cost = 200},
}

clsGrid = clsObject:Inherit()

function clsGrid:__init__()
	Super(clsGrid).__init__(self)
	self.__save = {}
	self.__temp = {}
	self.__temp.callout = {}
end

function clsGrid:SetId(id)
	self.__save.id = id
end

function clsGrid:GetId()	
	return self.__save.id
end

function clsGrid:SetState(state)
	self.__save.state = state
end

function clsGrid:GetState()
	return self.__save.state
end

function clsGrid:SetHero(heroId)
	self.__save.heroId = heroId
end

function clsGrid:GetHero()
	return self.__save.heroId
end

function clsGrid:SetTimestamp(time)
	self.__save.timestamp = time
end

function clsGrid:GetTimestamp()
	return self.__save.timestamp
end

function clsGrid:GetTrainTimelong()
	return 6 * 3600
end

function clsGrid:SetDoubleFlag(flag)
	self.__save.double = flag
end

function clsGrid:GetDoubleFlag()
	return self.__save.double
end

function clsGrid:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsGrid:InitGrid()
	self.__save = {}
end

function clsGrid:SetUnlockCost(cost)
	self.__temp.cost = cost
end

function clsGrid:GetUnlockCost()
	return self.__temp.cost
end

function clsGrid:StopTrain()
	local hero = user:GetHero(self:GetHero())
	local barrack = user:GetStruct("barrack")
	if hero then
		local exp = barrack:CalExp(data)
		hero:SetExp(exp)
		grid:SetHero()
		grid:SetDoubleFlag()
		grid:SetTimestamp()
		hero:SetWorkInfo("rest")
	end
end

function clsGrid:Restore(data)
	self.__save = data
end

function clsGrid:SetOwner(data)
	self.__temp.owner = data
end

function clsGrid:GetOwner()
	return self.__temp.owner
end

function NewGrid(id, state, owner, cost)
	local obj = clsGrid:New()
	obj:SetId(id)
	obj:SetState(state)
	obj:SetOwner(owner)
	obj:SetUnlockCost(cost)
	return obj
end
-----------------------------------------------------------
clsBarrack = STRUCT.clsStruct:Inherit()


local SoldierList = {}

function GetAllSoldierList()
	return SoldierList
end

function GetSoldierInfoByJob(job)
	return SoldierList[job]
end

function GetSoldierLv(job, lv)
	for k, v in ipairs(GetSoldierInfoByJob(job)) do
		if lv >= v.minLv and lv <= v.maxLv then
			return k
		end
	end
end

function GetUserSoldierInfo(user, heroId)
	local tribe = user:GetTribe()
	local obj = user
	if heroId then
		obj = user:GetHero(heroId)
	end
	if not obj then return end
	local soldierInfo = GetSoldierInfoByJob(obj:GetJob())
	if tribe then
		local barrack = tribe:GetStruct("barrack")
		if barrack then
			local lv = GetSoldierLv(obj:GetJob(), barrack:GetLv())
			return soldierInfo[lv]
		end
	end
	return soldierInfo[1]
end

--获取兵种信息
function GetHeroSoldierInfo(roleId, job)
	local tribeInfo = TRIBE.GetTribeDataById(roleId)
	local Lv = (tribeInfo.struct and tribeInfo.struct["barrack"]) or 1
	local soldierInfo = GetSoldierInfoByJob(job)
	local soldierLv = GetSoldierLv(job, Lv)
	return soldierInfo[soldierLv]
end

function FormatSoliderList()
	local sData = {}
	for k, v in pairs(GetAllSoldierList()) do
		local list = {}
		for num, info in ipairs(v) do
			local buf = {}
			buf.name = info.name
			buf.maxLv = info.maxLv
			buf.minLv = info.minLv
			list[num] = buf
		end
		table.insert(sData, list)
	end
	return sData
end


function GetEnlistTimelong()
	return ENLIST_TIME_LONG
end

function clsBarrack:__init__()
	Super(clsBarrack).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
	self:initsavedata()
end

function clsBarrack:inittempdata()
	self.__temp.list = {}
end

function clsBarrack:initsavedata()
	self.__save.enlist = {}
	self.__save.list = {}
end

function clsBarrack:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:InitGridList()
	self:SetTimeStamp(os.time())
	self:AddUpgradeList(owner)
	local user = assert(USER.GetOnlineUser(owner))
	local tribe = user:GetTribe()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	tribe:SetResourceStat("soldier", "max", info.upLimit)
	tribe:SetResourceStat("soldier", "num", INIT_SOLDIER_NUM)
end

function clsBarrack:AddUpgradeList(owner)
	local user = assert(USER.GetOnlineUser(owner))
	local obj = UPGRADELIST.NewList("enlist", "1", UPGRADELIST.LIST_UNLOCK)
	user:AddUpgradeList("enlist", "1", obj)
end

function clsBarrack:Upgrade()
	self:SetLv(1)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	self:SetResMax(info.upLimit)
	self:FreshHeroAttr()
end

function clsBarrack:FreshHeroAttr()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:FreshAllHeroTotalAttr()
end

function clsBarrack:GetResIncome()
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	return info.speed
end

function clsBarrack:SetResMax(data)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	tribe:SetResourceStat("soldier", "max", data)
end

function clsBarrack:GetTotalResIncomeRate()
	local income = self:GetResIncome()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	return math.floor(income)
end

function clsBarrack:ProStructFunc(time)
	local timelong = TRIBE.GetResourceFreshTimelong()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	local timestamp = self:GetTimeStamp()
	local n = math.floor(((time or os.time()) - timestamp) / timelong)
	if n > 0 then
		for i = 1, n do
			local curP = tribe:GetResourceStat("people", "num")
			local maxP = tribe:GetResourceStat("people", "max")
			local soldier = tribe:GetResourceStat("soldier", "num")
			local total, cur = user:CalHpInfo()
			if curP > 0 and soldier + cur < maxP then
				tribe:ModResourceStat("soldier", "num", math.floor(self:GetTotalResIncomeRate() * timelong / 3600))
			else
				break
			end
		end
		self:SetTimeStamp(timestamp + n * timelong)
	end
end

function clsBarrack:Enlist()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local tribe = user:GetTribe()
	local maxP = tribe:GetResourceStat("people", "max")
	local num = math.floor(maxP * SOLDIER_CONVERSION_RATE)
	tribe:ModResourceStat("soldier", "num", num)
	tribe:ModResourceStat("people", "num", -num)
	self:SetEnlistCount(1)
end

function clsBarrack:GetEnlistCount()
	local info = self.__save.enlist
	local times = 0
	if CalDiffDays(os.time(), info.time) == 0 then
		times = info.num or 0
	end
	return times
end

function clsBarrack:SetEnlistCount(data)
	local info = self.__save.enlist
	if CalDiffDays(info.time or 0, os.time()) ~= 0 then
		self.__save.enlist.num = 0
	end
	self.__save.enlist.num = (self.__save.enlist.num or 0) + data
	self.__save.enlist.time = os.time()
	if self.__save.enlist.num < MAX_ENLIST_TIMES then
		local user = assert(USER.GetOnlineUser(self:GetOwner()))
		local obj = user:GetOneUpgradeList("enlist", "1")
		obj:SetEndTimestamp(os.time() + ENLIST_TIME_LONG)
	end
end

function clsBarrack:GetEnlistInfo()
	return self.__save.enlist
end

function clsBarrack:GetEnlistCDTimelong()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local obj = user:GetOneUpgradeList("enlist", "1")
	return math.max(obj:GetEndTimestamp() - os.time(), 0)
end

function clsBarrack:ResetEnlistInfo()
	self.__save.enlist = {}
end

function clsBarrack:GetEnlistMax()
	return MAX_ENLIST_TIMES
end

function clsBarrack:InitGridList()
	for k, v in ipairs(GridConfList) do
		local obj = NewGrid(k, v.state, self:GetOwner(), v.cost)
		self:AddGrid(k, obj)
	end
end

function clsBarrack:AddGrid(id, obj)
	self.__temp.list[id] = obj
end

function clsBarrack:GetGridById(id)
	return self.__temp.list[id]
end

function clsBarrack:GetGridList()
	return self.__temp.list
end

function clsBarrack:GetSaveData()
	self.__save.list = {}
	for k, v in ipairs(self:GetGridList()) do
		self.__save.list[k] = v:GetSaveData()
	end
	return table.deepcopy(self.__save)
end

function clsBarrack:Restore(data)
	self.__temp.list = {}
	self.__save = data
	self:InitGridList()
	for k, v in ipairs(self.__save.list or {}) do
		local obj = self:GetGridById(k)
		obj:Restore(v)
	end
end

function clsBarrack:CalExp(pos)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local grid = self:GetGridById(pos)
	local hero = user:GetHero(grid:GetHero())
	local n = math.floor((os.time() - grid:GetTimestamp()) / EXP_ADD_FREQUENCE)
	if n <= 0 then
		return 0
	end
	local exp = math.max(math.floor(hero:GetUpgradeExp() / 240), 500) * n
	if grid:GetDoubleFlag() then
		exp = exp * 2
	end
	return exp
end

function clsBarrack:FormatTrainingList()
	local sData = {}
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	for k, v in ipairs(self:GetGridList()) do
		local buf = {}
		buf.id = k
		buf.state = v:GetState()
		buf.needIngot = v:GetUnlockCost()
		local heroId = v:GetHero()
		buf.heroId = heroId
		if heroId then
			local hero = user:GetHero(heroId)
			buf.heroName = hero:GetName()
			buf.Lv = hero:GetLv()
			buf.timelong = os.time() - v:GetTimestamp()
			buf.exp = self:CalExp(k) 
			buf.double = v:GetDoubleFlag()
			if buf.double then
				buf.exp = buf.exp * 2
			end
			buf.job = hero:GetJob()
		end
		table.insert(sData, buf)
	end
	return sData
end

local s_barrack_unlock = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	if not barrack then
		return ERROR.TipMsg(connection, "您还没有建立兵营")
	end
	local grid = barrack:GetGridById(data)
	if not grid then
		return ERROR.TipMsg(connection, "无效的解锁编号")
	end
	if grid:GetState() == GRID_UNLOCK then
		return ERROR.TipMsg(connection, "该位置已经解锁")
	end
	local cost = grid:GetUnlockCost()
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "元宝不足，无法解锁")
	end
	user:SetTotalIngot(-cost)
	grid:SetState(GRID_UNLOCK)
	connection:SendProtocol("c_barrack_unlock", data)
end

local s_barrack_training = function(connection, data)
	if type(data) ~= "table" or not data.heroId or not data.pos then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	if not barrack then
		return ERROR.TipMsg(connection, "您还没有建立兵营")
	end
	local grid = barrack:GetGridById(data.pos)
	if not grid then
		return ERROR.TipMsg(connection, "无效的训练位置")
	end
	if grid:GetState() == GRID_LOCK then
		return ERROR.TipMsg(connection, "该位置未解锁")
	end
	if grid:GetHero() then
		return ERROR.TipMsg(connection, "该位置已经有武将训练")
	end
	local hero = user:GetHero()
	if not hero then
		return ERROR.TipMsg(connection, "武将信息不存在")
	end
	if hero:GetType() == "godHero" then
		return ERROR.TipMsg(connection, "神将不能训练")
	end
	local info = hero:GetWorkInfo()
	if info.state == "training" then
		return ERROR.TipMsg(connection, "该武将正在训练")
	end
	if info.state == "standby" then
		user:DelHeroFromAllLineup(heroId, flag)
	end
	if info.state == "stay" then
		EMBASSY.RecallStayHero(info.place, connection.roleId)
	end
	hero:SetWorkInfo("training")
	grid:SetHero(data.heroId)
	grid:SetTimestamp(os.time())
	connection:SendProtocol("c_barrack_training", data)
end

local s_barrack_double = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	local grid = barrack:GetGridById(data)
	if not grid then
		return ERROR.TipMsg(connection, "无效的Id")
	end
	if not grid:GetHero() then
		return ERROR.TipMsg(connection, "该位置没有武将修炼，不能加倍")
	end
	if grid:GetDoubleFlag() then
		return ERROR.TipMsg(connection, "该位置已经加倍")
	end
	if grid:GetTimestamp() + grid:GetTrainTimelong() <= os.time() then
		return ERROR.TipMsg(connection, "该位置修炼已经结束")
	end
	if user:GetTotalIngot() < EXP_DOUBLE_COST then
		return ERROR.TipMsg(connection, "元宝数量不足")
	end
	user:SetTotalIngot(-EXP_DOUBLE_COST)
	grid:SetDoubleFlag(true)
	grid:StopTrain()
	connection:SendProtocol("c_barrack_double", data)
end

local s_barrack_stop = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	local grid = barrack:GetGridById(data)
	if not grid then
		return ERROR.TipMsg(connection, "无效的ID")
	end
--	local heroId = grid:GetHero()
	if not grid:GetHero() then
		return ERROR.TipMsg(connection, "该位置没有武将训练")
	end
--[[	local hero = user:GetHero(heroId)
	if hero then
		local exp = barrack:CalExp(data)
		hero:SetExp(exp)
		grid:SetHero()
		grid:SetDoubleFlag()
		grid:SetTimestamp()
		hero:SetWorkInfo("rest")
	end]]
	grid:StopTrain()
	connection:SendProtocol("c_barrack_stop", data)
end

local s_barrack_trainingList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe() 
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	if not barrack then
		return ERROR.TipMsg(connection, "您还没有建立兵营")
	end
	local sData = barrack:FormatTrainingList()
	connection:SendProtocol("c_barrack_trainingList", sData)
end

local s_barrack_enlist = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local barrack = assert(tribe:GetStruct("barrack"))
	if barrack:GetEnlistCount() >= barrack:GetEnlistMax() then
		return ERROR.TipMsg(connection, "您今天的强制征兵次数已经用完。")
	end
	if tribe:GetResourceStat("people", "num") < 0 then
		return ERROR.TipMsg(connection, "部落人口数量为0，不能征兵。")
	end
	local obj = user:GetIdleUpgradeList("enlist")
	if not obj then
		return ERROR.TipMsg(connection, "征兵队列冷却时间未到")
	end
	barrack:Enlist()
	user:SendUpgradeListUpdate(obj:GetType(), obj:GetId())
	connection:SendProtocol("c_barrack_enlist", "true")
	PROTOCOL.s_tribe_info(connection, "get")
end

local s_barrack_enlistInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local barrack = tribe:GetStruct("barrack")
	if not barrack then
		return ERROR.TipMsg(connection, "您还没有建立兵营")
	end
	local sData = {}
	sData.count = barrack:GetEnlistCount()
	sData.max = barrack:GetEnlistMax()
	sData.soldier = math.floor(tribe:GetResourceStat("people", "max") * SOLDIER_CONVERSION_RATE)
	sData.timelong = barrack:GetEnlistCDTimelong()
	connection:SendProtocol("c_barrack_enlistInfo", sData)
end

local s_barrack_soldierList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local barrack = tribe:GetStruct("barrack")
	if not barrack then
		return ERROR.TipMsg(connection, "您还没有建立兵营")
	end
	local sData = FormatSoliderList()
	connection:SendProtocol("c_barrack_soldierList", sData)
end

function __init__()
	local SOLDIER = Import("autocode/soldierList.lua")
	SoldierList = SOLDIER.GetTable()

	PROTOCOL.s_barrack_soldierList = s_barrack_soldierList
	PROTOCOL.s_barrack_trainingList = s_barrack_trainingList
	PROTOCOL.s_barrack_stop = s_barrack_stop
	PROTOCOL.s_barrack_double = s_barrack_double
	PROTOCOL.s_barrack_training = s_barrack_training
	PROTOCOL.s_barrack_unlock = s_barrack_unlock
	PROTOCOL.s_barrack_enlist = s_barrack_enlist
	PROTOCOL.s_barrack_enlistInfo = s_barrack_enlistInfo
end

