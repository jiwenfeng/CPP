
--升级队列

clsList = clsObject:Inherit()

LIST_LOCK = 1
LIST_UNLOCK = 2

local UnlockCostList = {
	["building"] = {["1"] = 0, ["2"] = 200},
}


function clsList:__init__()
	Super(clsList).__init__(self)
	self.__save = {}
	self.__temp = {}
end

--队列ID
function clsList:SetId(id)
	self.__save.id = id
end

function clsList:GetId()
	return self.__save.id
end

--队列类型
function clsList:SetType(type)
	self.__save.type = type
end

function clsList:GetType()
	return self.__save.type
end

function clsList:SetRoleId(roleId)
	self.__temp.roleId = roleId
end

function clsList:GetRoleId(roleId)
	return self.__temp.roleId
end

--队列状态
function clsList:SetState(state)
	self.__save.state = state
end

function clsList:GetState(state)
	return self.__save.state
end

function clsList:GetUnlockCost()
	if not UnlockCostList[self.__save.type] or not UnlockCostList[self.__save.type][self.__save.id] then
		return 0
	end
	return UnlockCostList[self.__save.type][self.__save.id]
end

function clsList:SetEndTimestamp(data)
	self.__save.endtime = data
end

function clsList:GetEndTimestamp()
	return self.__save.endtime or os.time()
end

function clsList:SetTimeCapacity(data)
end

function clsList:GetTimeCapacity()
end

function clsList:ListCanUse()
	return self:GetEndTimestamp() <= os.time()
end

function clsList:ClearCDTime()
	self.__save.endtime = os.time()
end

function clsList:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsList:Restore(data)
	self.__save = data
end

function clsList:SetTimes(data)
end

function clsList:GetTimes()
end

--------------------------------time capacity---------------------------------
--建筑队列 科技队列
--
--
clsCapacityList = clsList:Inherit()

function clsCapacityList:__init__()
	Super(clsCapacityList).__init__(self)
	self.__save = {}
	self.__temp = {}
end

function clsCapacityList:GetTimeCapacity()
	local user = assert(USER.GetOnlineUser(self:GetRoleId()))
	local tribe = user:GetTribe()
	if self:GetType() == "building" then
		return math.ceil(tribe:GetTribeLv() / 3) * 1200 + 4800
	elseif self:GetType() == "academy" then
		return math.ceil(tribe:GetTribeLv() / 15) * 3600
	end
end

function clsCapacityList:ListCanUse()
	return self:GetEndTimestamp() - os.time() <= self:GetTimeCapacity()
end

function clsCapacityList:SetEndTimestamp(data)
	data = data or os.time()
	data = data - os.time()
	self.__save.endtime = (self.__save.endtime or os.time()) + data
end

--[[

--以下暂时未用到
------------------------------时间容量 加次数限制-------------------------
--征收 征兵队列
--
clsTimesList = clsCapacityList:Inherit()
function clsTimesList:__init__()
	Super(clsTimesList).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsTimesList:initsavedata()
end

function clsTimesList:inittempdata()
end

function clsTimesList:SetEndTimestamp(data)
end

function clsTimesList:GetEndTimestamp()
end

function clsTimesList:SetTimes(data)
end

function clsTimesList:GetTimes()
end

------------------------------次数限制------------------------------------
--
--
--
clsCountList = clsList:Inherit()
function clsCountList:__init__()
	Super(clsCountList).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsCountList:initsavedata()

end

function clsCountList:inittempdata()
end

function clsCountList:SetTimes(data)
end

function clsCountList:GetTimes()
end
--------------------------------------------------------------------------
]]


function NewList(type, id, state, roleId)
	local obj
	if type == "building" or type == "academy" then
		obj = clsCapacityList:New()
	elseif type == "levy" or type == "enlist" or type == "arena" then
		obj = clsList:New()
	else
		return
	end
	obj:SetType(type)
	obj:SetId(id)
	obj:SetState(state)
	obj:SetRoleId(roleId)
	return obj
end

local s_upgrade_unlock = function(connection, data)
	if type(data) ~= "table" or not data.type or not data.index then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user:GetOneUpgradeList(data.type, data.index)
	if not obj then
		return ERROR.TipMsg(connection, "您还没有该队列，不能解锁")
	end
	if obj:GetState() == LIST_UNLOCK then
		return ERROR.TipMsg(connection, "该列队已经解锁了。")
	end
	if not user:HasFreeUpgradeList() then
		local cost = obj:GetUnlockCost()
		if user:GetTotalIngot() < cost then
			return ERROR.TipMsg(connection, "没有足够的元宝")
		end
		user:SetTotalIngot(-cost)
	end
	user:SetUpgradeListState(data.type, data.index, LIST_UNLOCK)
	user:SendUpgradeListUpdate(data.type, data.index)
	connection:SendProtocol("c_upgrade_unlock", data)
end

function __init__()
	PROTOCOL.s_upgrade_unlock = s_upgrade_unlock
end


