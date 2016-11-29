--排行榜

local DEFAULT_RANK_UPLIMIT = 500
local RankInfoList = {}

function GetRankInfoById(id)
	return RankInfoList[id]
end

function GetRankInfoList()
	return RankInfoList
end

local RankClassList = {
	["bossDmg"] = require "rank/clsBossDmg",
	["phyleFight"] = require "rank/clsPhyleFightRank",
	["userLv"] = require "rank/clsUserLvRank",
	["userForce"] = require "rank/clsUserForceRank",
	["unionLv"] = require "rank/clsUnionLvRank",
	["unionForce"] = require "rank/clsUnionForceRank",
	["charm"] = require "rank/clsCharmRank",
	["hero"] = require "rank/clsHeroRank",
	["equip"] = require "rank/clsEquipRank",
}

local RankObjList = {real = {}, show = {}}


--label: real/show
--type:榜单类型
function AddRankObj(label, type, obj)
	RankObjList[label][type] = obj
end

function GetRankObjByType(label, type)
	return RankObjList[label][type]
end


function GetRankNoByType(label, type, roleId)
	label = label or "real"
	local rank = RankObjList[label][type]
	if rank then
		return rank:GetRankNo(roleId)
	end
	return 0
end

function GetRankClassByType(type)
	return RankClassList[type]
end

function AddToRank(rankType, id)
	local obj = GetRankObjByType("real", rankType)
	if not obj then
		return 
	end
	obj:GetRankData(id)
end

function RmFromRank(rankType, key, id)
	local obj = GetRankObjByType("real", rankType)
	if not obj then
		return
	end
	obj:RmRankData(key, id)
end

--销毁排行榜
function DestroyRank(rankObj)
	local label = rankObj:GetLabel()
	local type = rankObj:GetType()
	rankObj:Destroy()
	AddRankObj(label, type)
	DATA.DBRemove(BASERANK.GetRankPath(label), {_id = type})
end

function NewRank(label, type, needSave, upLimit)
	local rankObj
	local class = GetRankClassByType(type)
	if class then
		rankObj = class:New()
		local info = GetRankInfoById(type)
		rankObj:InitRank(label, type, needSave, info and info.upLimit[label] or upLimit)
		AddRankObj(label, type, rankObj)
	end
	return rankObj
end

--恢复排行榜数据
function RestoreRankData(label)
	local function callback(data, msg)
		data = UnSerialize(data)
		for _, v in ipairs(data) do
			local obj = NewRank(label, v.type, true)
			if obj then
				obj:Restore(v)
			end
		end
	end
	DATA.Read(BASERANK.GetRankPath(label), {}, callback)
end
--保存排行榜数据
function SaveRankData(type)
	local function save_rank(type)
		for k, v in pairs(RankObjList[type]) do
			if v:IsNeedSave() then
				local buff = v:GetSaveData()
				buff._id = k
				DATA.UpdateData(BASERANK.GetRankPath(type), {_id = k}, buff)
			end
		end
	end
	if type then
		save_rank(type)
	else
		save_rank("real")
		save_rank("show")
	end
end

--刷新排行榜数据
function UpdateAllRank()
	for k, v in pairs(RankObjList["real"]) do
		if v:IsNeedSave() and k ~= "equip" then
			local obj = GetRankObjByType("show", k)
			if not obj then
				obj = NewRank("show", k, true)
			end
			obj:UpdateRank(v:GetRankList())
		end
	end
end

function UpdateEquipRank()
	local showObj = GetRankObjByType("show", "equip")
	local realObj = GetRankObjByType("real", "equip")
	if not realObj then
		return
	end
	if not showObj then
		showObj = NewRank("show", "equip", true)
	end
	showObj:UpdateRank(realObj:GetRankList())
end

--初始化所有的排行榜
function InitAllRank()
	local function del_rank(label, type)
		local rank = GetRankObjByType(label, type)
		if rank then
			rank:Destroy()
			AddRankObj(label, type)
		end
	end
	for k in pairs(GetRankInfoList()) do
		del_rank("real", k)
		del_rank("show", k)
		NewRank("real", k, true)
		NewRank("show", k, true)
	end
	SaveRankData()
	print("Init All Rank done!")
end

function GetRankTypeList()
	local sData = {}
	for k, v in pairs(GetRankInfoList()) do
		local buff = {type = k}
		buff.name = v.name
		table.insert(sData, {type = k, name = v.name})
	end
	table.sort(sData, function (a, b) 
		local info_a = GetRankInfoById(a.type)
		local info_b = GetRankInfoById(b.type)
		return info_a.index < info_b.index
	end)
	return sData
end

function SendOnlineMsg(roleId)
	local forceObj = GetRankObjByType("show", "userForce")
	local charmObj = GetRankObjByType("show", "charm")
	local forceInfo = forceObj:GetFormatRankData(1)
	local charmInfo = charmObj:GetFormatRankData(1)
	local nick = USER.GetUserNickInfo(roleId, "nick") 
	if forceInfo and forceInfo.roleId == roleId then
		CHAT.SendSysMsg("world", string.format(string.format("无敌战神【%s】君临于蛮荒大陆，王者的震撼在此登场", nick)))
	end
	if charmInfo and charmInfo.roleId == roleId then
		local sex = USER.GetUserNickInfo(roleId, "sex")
		local msg 
		if sex == 0 then
			msg = string.format("女神【%s】芳踪出现在蛮荒大陆，绝世的容颜让百花黯然凋零，日月为之失色", nick)
		else
			msg = string.format("花帝【%s】现于蛮荒大陆，俊秀的脸庞让无数的芳华女子都为之落泪", nick)
		end
		CHAT.SendSysMsg("world", msg)
	end
end

local s_rank_typeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetRankTypeList()
	connection:SendProtocol("c_rank_typeList", sData)
end

local s_rank_list = function(connection, data)
	local info = GetRankInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local rankObj = GetRankObjByType("show", data)
	if not rankObj then
		return ERROR.TipMsg(connection, "没有指定的排行榜。")
	end
	local sData = rankObj:GetTopRank(rankObj:GetUplimit(), connection.roleId)
	connection:SendProtocol("c_rank_list", sData)
end

local s_rank_equipList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:GenAllEquipShowList()
	local sData = user:GetAllEquipShowList()
	connection:SendProtocol("c_rank_equipList", sData)
end

local s_rank_addToRank = function(connection, data)
	data = tonumber(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equipInfo = user:AddEquipToRank(data) 
	if not equipInfo then
		return ERROR.TipMsg(connection, "指定的装备信息不存在。")
	end
	local rank = GetRankObjByType("real", "equip")
	if not rank:CanBeAddToRank(equipInfo) then
		return ERROR.TipMsg(connection, "装备战斗力低，无法提交")
	end
	rank:AddToRank(equipInfo)
	connection:SendProtocol("c_rank_addToRank", data) 
end


function __init__()
	
	local RANKLIST = Import("autocode/rankList.lua")
	RankInfoList = RANKLIST.GetTable()
	
	PROTOCOL.s_rank_typeList = s_rank_typeList
	PROTOCOL.s_rank_addToRank = s_rank_addToRank
	PROTOCOL.s_rank_equipList = s_rank_equipList
	PROTOCOL.s_rank_list = s_rank_list
	
	RestoreRankData("real")
	RestoreRankData("show")
	
	CALLOUT.Daily(getfenv(2), 0, 0, UpdateAllRank)
	
	CALLOUT.CallFre(UpdateEquipRank, 3600)

	CALLOUT.CallFre(SaveRankData, 5 * 60, "real")
end

