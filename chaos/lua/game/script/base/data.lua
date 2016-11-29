--
--	用来与saved数据库服务器交互数据
--	数据的存储、读取等

--读写协议列表
local DB_READ = 1
local NORMAL_READ = 2
local DB_WRITE = 3
local NORMAL_WRITE = 4
local DB_REMOVE = 5
local APPEND = 6
local DB_COUNT = 7
local ENSURE_INDEX = 8
local DROP_INDEX = 9
local DB_BATCH_READ = 10

local DB_UPDATE = 11

local WRITE_OK = 12
local WRITE_FAILED = 13
local READ_OK = 14
local READ_FAILED = 15
local APPEND_FAILED = 16
local UPDATE_OK = 17
local DB_INSERT = 18
------------------------------------------
local dataobj = nil
local nConStatue = nil   --连接状态
--写日志文件
function log(str, filename)
	local date = os.date("%Y%m%d", os.time())
	if not filename then
		Append("data/log/" .. date .."/data.log", os.date("%F %T ") .. str .. "\n")
	else
		Append("data/log/" .. date .. "/" .. filename, os.date("%F %T ") .. str .. "\n")
	end
end

function proLog(username, proName, sData)
	local path = string.format("data/biglog/%s/%s.log", username or "nil", os.date("%Y%m%d%H", os.time()))
	Append(path, string.format("%s username = %s, proName = %s, sData = \n%s\n", os.date("%F %T"), username or "", proName or "", sData))
end

function Stats(str, filename)
	Append("data/stats/" .. filename, str .. "\n")
end


clsData = SOCKET.clsConnection:Inherit()
function clsData:__init__()
	Super(clsData).__init__(self)
	self:CreateSocket()
--	self.__buffer = ""
	return true
end


function clsData:ReadCB(Stream, zip, pro)
	self:Process(Stream, pro)
end

function clsData:OnError(flag)  --断开连接处理函数
	log(string.format("disconnect from server with %d\n", flag))
	nConStatue = nil
	lengine.shutdown(1, "saved is down")
--	reconnection()
end


function clsData:Process(Msg, ProID)
	ProID = ProID and tonumber(ProID)
	if not ProID then
		print("ERROR no cmd", Msg)
		return
	end	
	if not fun_tbl[ProID] then
		print("ERROR no PRONAME", Msg)
		return
	end
	return fun_tbl[ProID](self, Msg)
end



function clsData:SendProtocol(ProID, Arg, debug)
	return self:Write(Arg, 0, ProID)
end

--回调函数列表
local cblist = {}
local SN = 0
local function NewSN()
	SN = SN + 1
	return SN
end


--增强型读数据接口		match 查询条件  limit 返回结果数目  skip 跳过数量  fieldsToReturn 返回的字段
function EnhancedRead(filename, match, limit, skip, fieldsToReturn, callback)
	if (not dataobj) or (not nConStatue) then
		--error连接错误
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_READ, string.format("%d;%s;%s;%s;%s;%s", SN, filename, FormatSerialize(match or {}), limit or " ", skip or " ", FormatSerialize(fieldsToReturn) or " "))
	cblist[SN] = callback
	return true
end
--读数据接口
function Read(filename, match, callback)
	if (not dataobj) or (not nConStatue) then
		--error连接错误
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_READ, string.format("%d;%s;%s; ; ; ", SN, filename, FormatSerialize(match or {})))
	cblist[SN] = callback
	return true
end

--批量读取数据库
function BatchRead(filename, key, valueList, limit, callback)
	if (not dataobj) or (not nConStatue) then
		--error连接错误
		return false
	end
	assert(type(key) == "string" and type(valueList) == "table")
	local SN = NewSN()
	dataobj:SendProtocol(DB_BATCH_READ, string.format("%d;%s;%s;%s;%s; ", SN, filename, key, FormatSerialize(valueList or {}), limit or " "))
	cblist[SN] = callback
	return true
end

function NormalRead(filename, callback)
	if (not dataobj) or (not nConStatue) then
		--error连接错误
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(NORMAL_READ, string.format("%d;%s", SN, filename))
	cblist[SN] = callback
	return true
end


--写数据接口
function Write(filename, match, content, callback)
	if (not dataobj) or (not nConStatue) then
		--error
		return false
	end
	if type(match) ~= "table" then
		return false
	elseif table.size(match) <= 0 then
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_WRITE, string.format("%d;%s;%s;%s", SN, filename, FormatSerialize(match), FormatSerialize(content)))
	cblist[SN] = callback
	return true
end

function Insert(filename, content, callback)
	if (not dataobj) or (not nConStatue) then
		return false
	end
	if type(content) ~= "table" then
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_INSERT, string.format("%d;%s;%s", SN, filename, FormatSerialize(content)))
	cblist[SN] = callback
	return true
end


function Update(filename, match, content, callback)
	if (not dataobj) or (not nConStatue) then
		return false
	end
	if type(match) ~= "table" then
		return false
	elseif table.size(match) <= 0 then
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_UPDATE, string.format("%d;%s;%s;%s", SN, filename, FormatSerialize(match), FormatSerialize(content)))
	cblist[SN] = callback
	return true
end

function NormalWrite(filename, content, callback)
	if (not dataobj) or (not nConStatue) then
		--error
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(NORMAL_WRITE, string.format("%d;%s;%s", SN, filename, FormatSerialize(content)))
	cblist[SN] = callback
	return true
end

function DBRemove(filename, match, callback)
	if (not dataobj) or (not nConStatue) then
		--error
		return false
	end
	if type(match) ~= "table" then
		return false
	elseif table.size(match) <= 0 then
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_REMOVE, string.format("%d;%s;%s", SN, filename, FormatSerialize(match)))
	cblist[SN] = callback
	return true
end

function Append(filename, content)
	if not dataobj or not nConStatue then
		--error
		print("data.lua write log error:do not connect to saved server")
		return false
	end
	local len, str = msgpack.pack(content)
	local Arg = filename .. ";" .. str
--	dataobj:Write(string.format("%d;%s", string.len(Arg), Arg))
	dataobj:Write(Arg, 0, APPEND)
end

function Count(filename, callback)
	if not dataobj or not nConStatue then
		print("data.lua count num error:do not connect to saved server")
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DB_COUNT, string.format("%d;%s", SN, filename))
	cblist[SN] = callback
	return true
end

--建立索引   DATA.EnsureIndex(DBName, {key = 1})
function EnsureIndex(filename, key, callback)
	if not dataobj or not nConStatue then
		print("data.lua count num error:do not connect to saved server")
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(ENSURE_INDEX, string.format("%d;%s;%s", SN, filename, FormatSerialize(key)))
	cblist[SN] = callback
	return true
end

--删除索引	DATA.DropIndex(DBName, {key = 1})
function DropIndex(filename, key, callback)
	if not dataobj or not nConStatue then
		print("data.lua count num error:do not connect to saved server")
		return false
	end
	local SN = NewSN()
	dataobj:SendProtocol(DROP_INDEX, string.format("%d;%s;%s", SN, filename, FormatSerialize(key)))
	cblist[SN] = callback
	return true
end


--协议表
fun_tbl = {
--	write_ok = function(connection, data)
	[UPDATE_OK] = function(connection, data)
		local SN = data
		local func = cblist[tonumber(SN)]
		if func then
			func(true)
			cblist[SN] = nil
		end
	end,
	[WRITE_OK] = function(connection, data)
		local SN = data
		SN = tonumber(SN)
		local func = cblist[SN]
		if func then
			func(true)
			cblist[SN] = nil
		end
	end,
--	write_failed = function(connection, data)
	[WRITE_FAILED] = function(connection, data)
		local SN, errmsg = string.match(data, "^(%d+);(.*)")
		SN = tonumber(SN)
		local func = cblist[SN]
		if func then
			func(false, errmsg)
			cblist[SN] = nil
		end
	end,
--	read_ok = function(connection, data)
	[READ_OK] = function(connection, data)
		local SN, content = string.match(data, "^(%d+);(.*)")
		SN = tonumber(SN)
		local func = cblist[SN]
		if func then
			func(content)
			cblist[SN] = nil
		end
	end,
--	read_failed = function(connection, data)
	[READ_FAILED] = function(connection, data)
		local SN, errmsg = string.match(data, "^(%d+);(.*)")
		SN = tonumber(SN)
		local func = cblist[SN]
		if func then
			func(false, errmsg)
			cblist[SN] = nil
		end
	end,
--	append_failed = function(connection, data)
	[APPEND_FAILED] = function(connection, data)
		print(string.format("write log error:%d", m))
	end,
}

--从数据库恢复数据
-- fName = file path, tName = tale name
function Restore(fName, tName)
	local M = getfenv(2)
	local function CallBack(Data, ErrMsg)
		Data = UnSerialize(Data)
		for _, v in pairs(Data) do
			if v._data then
				tName[v._id] = v._data
			else
				local buff = table.deepcopy(v)
				buff._id = nil
				tName[v._id] = buff
			end
		end
		if tName.__RestoreHook then
			M.RestoreHook(fName)
		end
		print(string.format("%.6f\tLoad table  '%s'  done!", os.clock(), fName))
	end
	Read(fName, {}, CallBack)
end

-- 保存数据到数据库
-- tName = tale name, fName = file path
function SaveData(fName, tName, id, callback)
	local Data = tName[id]
	if not Data then
		return
	end
	local buff = {}
	if type(Data) == "table" then
		buff = table.deepcopy(Data)
	else
		buff._data = Data
	end
	buff._id = id
	if not DATA.Write(fName, {_id = id}, buff, callback) then
		DATA.log(string.format("save data failed: connect to Saved error! path:%s, id:%s", fName, id), "data.log")
	end
	return true
end

function UpdateData(fName, match, data, callback)
	assert(type(match) == "table")
	assert(type(data) == "table")
	assert(data._id)
	if not DATA.Write(fName, match, data, callback) then
		DATA.log(string.format("update data failed: connect to Saved error! path:%s, match:%s", fName, Serialize(match)), "data.log")
	end
	return true
end

function NormalUpdate(fName, match, data, callback)
	assert(type(match) == "table")
	assert(type(data) == "table")
	assert(data._id)
	if not Update(fName, match, data, callback) then
		DATA.log(string.format("update data failed: connect to Saved error! path:%s, match:%s", fName, Serialize(match)), "data.log")
	end
	return true
end

function reconnection()
	dataobj = clsData:New()
	nConStatue = dataobj:Connect("127.0.0.1", PORT_CONNECT_SAVED)
	if not nConStatue then
		CALLOUT.CallOut(reconnection, 5)
		return
	end
end

function __init__()
	--创建数据交互套接字，用于saved服务器的数据交互
	dataobj = clsData:New()
	nConStatue = dataobj:Connect("127.0.0.1", PORT_CONNECT_SAVED)
	if not nConStatue then
		lengine.shutdown(1, "connot connect to saved")
	end
end

