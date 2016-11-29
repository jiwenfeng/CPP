--寻宝系统

local DEFALT_MAP_ID = 101041
local DEFALT_TREASURE_ID = "101041"


-----------------------------------------------------------------
local TreasurePlaceList = {}			--藏宝点信息
local path_treasure = "chaos.treasure"

--藏宝点信息
function GetTreasurePlaceList()
	local buff = {}
	for k, v in pairs(TreasurePlaceList) do
		buff[k] = v
	end
	return buff
end

function GetTreasurePlaceById(id)
	return TreasurePlaceList[id or DEFALT_TREASURE_ID]
end

function SetTreasurePlaceById(id, obj)
	TreasurePlaceList[id] = obj
end


function GetTreasureIdByMap(mapId)
	mapId = mapId or DEFALT_MAP_ID
	for k, v in pairs(GetTreasurePlaceList()) do
		if v:GetMapId() == mapId then
			return v
		end
	end
end

function RestoreTreasurePlace()
	for k in pairs(CLSTREASURE.GetAllTreasureList()) do
		local treasure = CLSTREASURE.clsTreasure:New()
		treasure:InitTreasure(k)
		SetTreasurePlaceById(k, treasure)
	end
	local function call_back(Data)
		Data = UnSerialize(Data)
		for k, v in pairs(Data) do
			local treasure = GetTreasurePlaceById(k)
			if treasure then
				treasure:Restore(v)
			end
		end
	end
	DATA.Read(path_treasure, {}, call_back)
end

function SaveTreasurePlace()
	for k, v in pairs(GetTreasurePlaceList()) do
		local saveData = v:GetSaveData()
		saveData._id = k
		DATA.UpdateData(path_treasure, {_id = k}, saveData)
	end
end

function GetTreasurePlace()
	local treasure = GetTreasurePlaceById()
	local list = {}
	table.insert(list, {id = treasure:GetId(), name = treasure:GetName(), state = 1})
	return list
end

----------------------------------------------------------------------------
local clsUser = USERBASE.clsUser

function clsUser:SetDigTreasureTimes(id, data)
	id = tostring(id)
	self.__save.treasure = self.__save.treasure or {}
	self.__save.treasure[id] = self.__save.treasure[id] or {}
	local info = self.__save.treasure[id]
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUser:GetDigTreasureTimes(id)
	id = tostring(id)
	self.__save.treasure = self.__save.treasure or {}
	self.__save.treasure[id] = self.__save.treasure[id] or {}
	local info = self.__save.treasure[id]
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end
----------------------------------------------------------------------



local s_treasure_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn(data, connection.roleId, 1)
	user:SetFocus("treasure", data)
	local treasure = GetTreasurePlaceById()
	local sData = treasure:GetTreasureInfo()
	connection:SendProtocol("c_treasure_info", sData)
end

local s_treasure_logList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local treasure = GetTreasurePlaceById()
	local list = treasure:GetLogList()
	connection:SendProtocol("c_treasure_logList", list)
end

local s_treasure_dig = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type ~= "treasure" then
		return ERROR.TipMsg(connection, "您当前没有进行寻宝。")
	end
	if user:IsBagFull(USERBASE.PACKAGE) then
		return ERROR.TipMsg(connection, "您的背包已满。")
	end
	data.times = tonumber(data.times)
	if not data.times or data.times <= 0 then
		return ERROR.TipMsg(connection, "请选择要挖掘的次数。")
	end
	local treasure = GetTreasurePlaceById()
	data.buyMap = tonumber(data.buyMap)
	data.buySpade = tonumber(data.buySpade)
	local ret, msg = treasure:BeginDig(user, data.times, data.spadeId, data.buyMap == 1, data.buySpade == 1)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_treasure_dig", ret)
end

local s_treasure_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type == "treasure" then
		USER.SetUserFocusOn(focus.id, connection.roleId)
		user:SetFocus()
	end
	connection:SendProtocol("c_treasure_leave", "true")
end





function __init__()
	
	RestoreTreasurePlace()
	CALLOUT.CallFre(SaveTreasurePlace, 5 * 60)
	
	PROTOCOL.s_treasure_info = s_treasure_info
	PROTOCOL.s_treasure_logList = s_treasure_logList
	PROTOCOL.s_treasure_dig = s_treasure_dig
	PROTOCOL.s_treasure_leave = s_treasure_leave
end


