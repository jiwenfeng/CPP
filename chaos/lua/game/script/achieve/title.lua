--称号系统

local titleTypeList = {
	["status"] = 1, ["userLv"] = 2, ["userForce"] = 3, ["charm"] = 4, ["arena"] = 5, ["mission"] = 6, ["honor"] = 7
}

local STATE_VALID = 1
local STATE_INVALID = 0

local AllTitleInfoList = {}

function GetAllTitleInfoList()
	return AllTitleInfoList
end

function GetTitleInfoById(id)
	return AllTitleInfoList[id]
end

--通过名次获取称号
function GetTitleByRank(type, rank, sex)
	for k, v in pairs(GetAllTitleInfoList()) do
		if v.type == type and (not v.sex or v.sex == sex) then
			if not v.rank then
				if rank >= v.min and rank <= v.max then
					return k
				end
			elseif v.rank == rank then
				return k
			end
		end
	end
end

function IsValidTitleType(type)
	return titleTypeList[type]
end
--------------------------------------------------------
local clsUser = USERBASE.clsUser

function clsUser:GetTitleList()
	self.__save.title = self.__save.title or {}
	self.__save.title.list = self.__save.title.list or {}
	return self.__save.title.list
end

function clsUser:SetTitleById(titleId, state)
	local info = GetTitleInfoById(titleId)
	self.__save.title = self.__save.title or {}
	self.__save.title.list = self.__save.title.list or {}
	self.__save.title.list[titleId] = state
	if state == STATE_VALID then
		self:SetNewTitle(titleId, true)
	end
	self:CheckCurTitle(titleId, state)
end

function clsUser:GetTitleById(titleId)
	self.__save.title = self.__save.title or {}
	self.__save.title.list = self.__save.title.list or {}
	return self.__save.title.list[titleId]
end

function clsUser:CheckCurTitle(titleId, state)
	if state == STATE_VALID and not self:GetCurTitle() then
		self:SetCurTitle(titleId)
	elseif state ~= STATE_VALID and self:GetCurTitle() == titleId then
		self:SetCurTitle(nil)
	end
end


function clsUser:GetCurTitle()
	self.__save.title = self.__save.title or {}
	return self.__save.title.select
end

function clsUser:SetCurTitle(titleId)
	self.__save.title = self.__save.title or {}
	self.__save.title.select = titleId
	USER.ModUserNickInfo(self:GetRoleId(), "title", titleId)
	self:Broad2ExNeighbor("title", nil, nil, true)		--向场景更新称号变化
end

function clsUser:ClearCurTitle()
	self.__save.title = self.__save.title or {}
	self.__save.title.select = nil
end

--设置新称号标记
function clsUser:SetNewTitle(titleId, state)
	self.__save.title = self.__save.title or {}
	self.__save.title.newGot = self.__save.title.newGot or {}
	self.__save.title.newGot[titleId] = state
end

function clsUser:GetNewTitle()
	self.__save.title = self.__save.title or {}
	self.__save.title.newGot = self.__save.title.newGot or {}
	return self.__save.title.newGot
end
--------------------------------------------------------
--荣誉称号
local HonorTitleList = {
	["391101"] = "洪荒大事记：玩家%s夺下第一个城池，获得永久称号：传奇城主！",
	["391102"] = "洪荒大事记：玩家%s开启了【太虚仙境】，获得永久称号：仙境传说！",
	["391103"] = "洪荒大事记：玩家%s率先达到100级，获得永久称号：登峰造极！",
	["391104"] = "洪荒大事记：玩家%s率先晋级【大帝】，获得永久称号：上古大帝！",
	["391105"] = "洪荒大事记：玩家%s率先晋级【天帝】，获得永久称号：上古天帝！",
	["391106"] = "洪荒大事记：玩家%s率先晋级【天尊】，获得永久称号：上古天尊！",
	["391107"] = "洪荒大事记：玩家%s成为战力突破100000的第一人，获得永久称号：上古战神！",
}
local HonorTitleRecord = {}
local path_honor = "chaos.honor"

function GetHonorTitleRecordList()
	return HonorTitleRecord
end

function IsHonorTitleRecordExist(titleId)
	local record = GetHonortitleRecordById(titleId)
	return record.roleId
end

function SetHonorTitleRecordById(titleId, roleId)
	HonorTitleRecord[titleId] = {roleId = roleId, timestamp = os.time()}
	DATA.SaveData(path_honor, HonorTitleRecord, titleId)
end

function GetHonortitleRecordById(titleId)
	HonorTitleRecord[titleId] = HonorTitleRecord[titleId] or {}
	return HonorTitleRecord[titleId]
end

function CheckHonorTitleRecord(user, titleId)
	if not IsHonorTitleRecordExist(titleId) then
		SetHonorTitleRecordById(titleId, user:GetRoleId())
		user:SetTitleById(titleId, STATE_VALID)
		CHAT.SendSysNotice(string.format(HonorTitleList[titleId], user:GetName()))
	end
end

function GetFormatedHonorTitleList()
	local sData = {}
	for k in pairs(HonorTitleList) do
		local buff = {titleId = k}
		local roleId = IsHonorTitleRecordExist(k)
		if roleId then
			buff.nick = USER.GetUserNickById(roleId)
		end
		table.insert(sData, buff)
	end
	table.sort(sData, function(a, b) return tonumber(a.titleId) < tonumber(b.titleId) end)
	return sData
end
--------------------------------------------------------
--清理称号信息
function ClearTitleInfo(roleId, type, rank)
	return SetTitleInfo(roleId, type, rank or 1, STATE_INVALID)
end

--设置称号信息
function SetTitleInfo(roleId, type, rank, state)
	local sex = USER.GetUserNickInfo(roleId, "sex")
	local titleId = GetTitleByRank(type, rank, sex)
	if titleId then
		local info = GetTitleInfoById(titleId)
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetTitleById(titleId, state or STATE_VALID)
		else
			OFFLINE.SetOfflineData(roleId, "title", titleId, STATE_VALID)
		end
	end
end


function GetFormatedTitleTypeList(user)
	local sData = {list = {}}
	sData.max = table.size(GetAllTitleInfoList())
	sData.now = table.size(user:GetTitleList())
	sData.curTitle = user:GetCurTitle()
	local newTitle = {}
	for k in pairs(user:GetNewTitle()) do
		local titleInfo = GetTitleInfoById(k)
		newTitle[titleInfo.kind] = true
	end
	for k, v in pairs(titleTypeList) do
		table.insert(sData.list, {type = k, isNew = newTitle[k] and 1 or 0})
	end
	table.sort(sData.list, function(a, b) return titleTypeList[a.type] < titleTypeList[b.type] end)
	return sData
end


function FormatTitleList(user, kind)
	local sData = {type = kind, list = {}}
	local titleList = user:GetTitleList()
	local newTitle = user:GetNewTitle()
	for k, v in pairs(GetAllTitleInfoList()) do
		if v.kind == kind then
			local buff = {titleId = k, name = v.name, quality = v.quality}
			buff.state = titleList[k] or -1
			buff.isNew = newTitle[k] and 1 or 0			
			table.insert(sData.list, buff)
			user:SetNewTitle(k)			--获取之后清除新称号标记
		end
	end
	table.sort(sData.list, function(a, b) return 
			a.quality > b.quality or (a.quality == b.quality and tonumber(a.titleId) > tonumber(b.titleId))
		end)
	return sData
end



local s_title_typeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatedTitleTypeList(user)
	connection:SendProtocol("c_title_typeList", sData)
end

local s_title_list = function(connection, data)
	if not titleTypeList[data] then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = FormatTitleList(user, data)
	connection:SendProtocol("c_title_list", sData)
end

local s_title_set = function(connection, data)
	local info = GetTitleInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local state = user:GetTitleById(data)
	if state ~= STATE_VALID then
		return ERROR.TipMsg(connection, "您尚未激活该称号。")
	end
	user:SetCurTitle(data)
	connection:SendProtocol("c_title_set", data)
end

function __init__()

	local TITLELIST = Import("autocode/titleList.lua")
	AllTitleInfoList = TITLELIST.GetTable()
	
	DATA.Restore(path_honor, HonorTitleRecord)
	
	PROTOCOL.s_title_typeList = s_title_typeList
	PROTOCOL.s_title_list = s_title_list
	PROTOCOL.s_title_set = s_title_set
	
end


