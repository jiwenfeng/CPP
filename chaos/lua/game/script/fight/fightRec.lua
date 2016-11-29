--战斗录像
local path_fightRecord = "chaos.fightRec"
local FightRecordList = {}
local MAX_DELAY_TIME = 10

--保存战斗录像数据
function SaveFightRecord(fightId, data)
	local buff = {record = data}
	buff.time = os.time()
	buff._id = fightId
	if not DATA.UpdateData(path_fightRecord, {_id = fightId}, buff) then
		DATA.log(string.format("Save fightRecord %s failed: connect server Saved error!", fightId), "fightRec.log")
	end
end
--读取战斗录像数据
function ReadFightRecordData(fightId, callback)
	if not DATA.Read(path_fightRecord, {_id = fightId}, callback) then
		return false
	end
	return true
end

function RemoveFightRecord(fightId)
	DATA.DBRemove(path_fightRecord, {_id = fightId})
end

-----------------------------------------------------------------------------
--战斗录像回放类
clsRec = clsObject:Inherit()

function clsRec:__init__()
	Super(clsRec).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsRec:initsavedata()
	self.__save.round = 0
end

function clsRec:inittempdata()
	self.__temp.callout = {}
end


function clsRec:InitRec(id, record, roleId)
	self.__save.id = id
	self.__save.record = record
	self.__save.owner = roleId
	self:SetState(1)
end

function clsRec:GetId()
	return self.__save.id
end

function clsRec:GetRecord()
	return self.__save.record
end

function clsRec:GetOwner()
	return self.__save.owner
end

function clsRec:SetState(data)
	self.__save.state = data
end

function clsRec:GetState()
	return self.__save.state
end

function clsRec:GetRound()
	return self.__save.round
end

function clsRec:NewRound()
	self.__save.round = self.__save.round + 1
	self:CheckPlayNewRound()
end

function clsRec:SendFightProtocol(index)
	local record = self:GetRecord()
	local roleId = self:GetOwner()
	if index < 1 or index > #record then return end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		local buff = record[index]
		if index == 1 then
			local sData = buff[1].proData
			sData.kind = "record"
			con:SendProtocol(buff[1].proName, sData)
		elseif index == #record then
			con:SendProtocol(buff[1].proName, buff[1].proData)
		else	
			con:SendProtocol("c_fight_occur", buff)
		end
	end
end


function clsRec:StartPlayCallOut()
	self.__temp.timerId = CALLOUT.ObjCallOut(self.__temp.callout, clsRec.NewRound, MAX_DELAY_TIME, self)
end

function clsRec:RemovePlayCallOut()
	if self.__temp.timerId then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.timerId)
	end
end

function clsRec:CheckPlayNewRound()
	if self:GetState() ~= 1 then
		return
	end
	self:RemovePlayCallOut()
	local round = self:GetRound()
	local record = self:GetRecord()
	if round == 1 then
		self:SendFightProtocol(1)
		self:StartPlayCallOut()
	else
		for i = 2, #record do
			self:SendFightProtocol(i)
		end
		return ClearFightRecord(self:GetOwner())
	end
end

-------------------------------------------------------------------------------------
function PlayFightRecord(connection, fightId, roleId)
	local function play_record(data)
		data = UnSerialize(data)
		if not data or not data[1] or not data[1].record then
			return ERROR.TipMsg(connection, "没有指定的录像信息。")
		end
		local recObj = clsRec:New()
		recObj:InitRec(fightId, data[1].record, roleId)
		FightRecordList[roleId] = recObj
		recObj:NewRound()
	end
	if not ReadFightRecordData(fightId, play_record) then
		return ERROR.TipMsg(connection, "Saved connection Error!")
	end
end

function GetFightRecObj(roleId)
	return FightRecordList[roleId]
end

function ClearFightRecord(roleId)
	local recObj = FightRecordList[roleId]
	if not recObj then
		return
	end
	recObj:SetState()
	recObj:RemovePlayCallOut()
	recObj:Destroy()
	FightRecordList[roleId] = nil
end



--查看战斗回放
local s_fight_playRec = function(connection, data)
	if GetFightRecObj(connection.roleId) then
		ClearFightRecord(connection.roleId)
	end
	PlayFightRecord(connection, data, connection.roleId)
end
--停止播放
local s_fight_endRec = function(connection, data)
	if not GetFightRecObj(connection.roleId) then
		return
	end
	ClearFightRecord(connection.roleId)
end

local s_fight_result = function(connection, data)
	local recObj = GetFightRecObj(connection.roleId)
	if not recObj then
		return ERROR.TipMsg(connection, "没有该战斗回放信息。")
	end
	local record = recObj:GetRecord()
	local result = record[#record].list[1]
	return connection:SendProtocol(result.proName, result.data)
end

function __init__()
	
	PROTOCOL.s_fight_playRec = s_fight_playRec
	PROTOCOL.s_fight_endRec = s_fight_endRec
	PROTOCOL.s_fight_result = s_fight_result
	
end
