DBConnection = mongo.Connection:New()
assert(DBConnection:connect("localhost:" .. PORT_CONNECT_DB), "cann't connect to dbserver")

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
--数据库读写接口(CName数据表名 DBName.tName)
--保存数据(Data是包含_id字段的数据表)
function DBSave(CName, match, Data)
--	if Data._id then
		return DBConnection:update(CName, match, Data, true, false)
--	end
--	return DBConnection:insert(CName, Data)
end
--读取、查询数据(Match是形如{key = val}形式的匹配表)
function DBFind(CName, Match, Limit, Skip, fieldsToReturn, Options, batchSize)
	return DBConnection:query(CName, Match, Limit, Skip, fieldsToReturn, Options, batchSize)
end
--删除整个表
function DBDrop(CName)
	return DBConnection:drop_collection(CName)
end
--删除表中记录(Match是形如{key = val}形式的匹配表)
function DBRemove(CName, Match)
	return DBConnection:remove(CName, Match)
end
--更新记录(Upsert	记录不存在时是否插入  Multi 存在多条记录时是否全部更新)
function DBUpdate(CName, Match, Data, Upsert, Multi)		
	return DBConnection:update(CName, Match, Data, Upsert, Multi)
end
--批量插入(Array是一个待插入数据的数组)
function DBBatchInsert(CName, Array)
	return DBConnection:insert_batch(CName, Array)
end

function DBInsert(CName, Data)
	return DBConnection:insert(CName, Data)
end

--查看表中索引
function DBGetIndexes(CName)
	return DBConnection:get_indexes(CName)
end
--查询存在记录
function DBIsExist(CName)
	return DBConnection:exists(CName)
end
--建立索引 (Keys 形如{key = 1} IsUnique 是否唯一值索引 Name所建索引的名称)
function DBEnsureIndex(CName, Keys, IsUnique, Name)
	return DBConnection:ensure_index(CName, Keys, IsUnique, Name)
end
--通过键值删除某个索引 (Keys 形如{key = 1})
function DBDropIndexByKey(CName, Keys)
	return DBConnection:drop_index_by_fields(CName, Keys)
end

--删除所有索引
function DBDropIndexes(CName)
	return DBConnection:drop_indexes(CName)
end
--获取表中记录数目
function DBCount(CName)
	return DBConnection:count(CName)
end
--数据库认证({dbname = DBName, username = user, password = pwd})
function DBAuth(Data)
	return DBConnection:auth(Data)
end

function DBIsFailed()
	return DBConnection:is_failed()
end
--获取数据库地址
function DBServerAddr()
	return DBConnection:get_server_address()
end


function log(str, filename)
	local filepath = filename
	if not filepath then
		filepath = "data/log/save.log"
	else
		filepath = "data/log/" .. filepath
	end
	Touch(filepath)
	local file = io.open(filepath, "a")
	assert(file)
	file:write(os.date("%F %T") .. str .. "\n")
	file:close()
end

function UpdateFile(data)
	if not string.match(data, "^[_%w]+[./_%w]*") then
		return log(string.format("%s更新失败", data), "savedUpdate.log")
	end
	if not (io.open(data, "r")) then
		return log(string.format("%s更新失败", data), "savedUpdate.log")
	end
	if UpdateImportFile(data) then
		log(string.format("更新%s成功", data), "savedUpdate.log")
	else
		log(string.format("%s更新失败", data), "savedUpdate.log")
	end
end

clssaveCon = SOCKET.clsConnection:Inherit()
function clssaveCon:__init__()
	Super(clssaveCon).__init__(self)
	self.__buffer = ""
	return true
end


function clssaveCon:ReadCB(Stream, zip, pro)
	self:Process(Stream, pro)
end

function clssaveCon:OnError(flag)  --断开连接处理函数
	log(self.ClientIp .. " disconnect with " .. flag)
end


function clssaveCon:Process(Msg, ProID)
	ProID = ProID and tonumber(ProID)
	if not ProID  then
		print("ERROR no cmd", Msg)
		return
	end
	if not funtbl[ProID] then
		print("ERROR no PRONAME", Msg)
		return
	end
	return funtbl[ProID](self, Msg)
end


function clssaveCon:SendProtocol(ProID, Arg, debug)
	return self:Write(Arg, 0, ProID)
end

clssaveSer = SOCKET.clsServer:Inherit()
function clssaveSer:OnAccept()
	local Connector, IP = self:Accept(clssaveCon)
	Connector.ClientIp = IP
	log(string.format("new client from:" .. IP))
	print("new client from: " .. IP)
end


Server = clssaveSer:New("127.0.0.1", PORT_CONNECT_SERVER)


funtbl = {
	[DB_WRITE] = function(Connection, data)
		local SN, filename, match, content = string.match(data, "^(%d+);([^;]+);([^;]+);(.*)")		-- filename : "DBname.TBLname"
--		log(filename .. "\n" .. match .. "\n" .. content, "dbwrite.log")
		match = UnSerialize(match)
		content = UnSerialize(content)
		local rst, rtn = DBSave(filename, match, content)
		if not rst then
--			log(string.format("ERROR:%s", rtn))
			Connection:SendProtocol(WRITE_FAILED, SN .. ";" .. rtn)
			return
		end
		Connection:SendProtocol(WRITE_OK, SN)
	end,
--	dbread = function(Connection, data)
	[DB_READ] = function(Connection, data)
		local SN, filename, match, limit, skip, fieldsToReturn = string.match(data, "^(%d+);([^;]+);([^;]+);([^;]+);([^;]+);(.+)")
		match = UnSerialize(match)
		limit = tonumber(limit)
		skip = tonumber(skip)
		fieldsToReturn = UnSerialize(fieldsToReturn)
		local cursor = DBFind(filename, match, limit, skip, fieldsToReturn)			-- 数据库返回的是一个游标
		if not cursor then
--			log("ERROR: DataBase connection error.")
			Connection:SendProtocol(READ_FAILED, SN .. ";" .. "Error read")
			return
		end
		local sData = {}
		for v in cursor:results() do
			table.insert(sData, v)
		end
		sData = FormatSerialize(sData)
--		log(name .. "n" .. sData, "dbread.log")
		Connection:SendProtocol(READ_OK, SN .. ";" .. sData)
	end,
	[DB_BATCH_READ] = function(Connection, data)
		local SN, filename, key, list, limit = string.match(data, "^(%d+);([^;]+);([^;]+);([^;]+);([^;]+);")
		list = UnSerialize(list)
		limit = tonumber(limit)
		local sData = {}
		for _, v in ipairs(list) do
			local match = {}
			match[key] = v
			local cursor = DBFind(filename, match, limit)
			if cursor then
				for v in cursor:results() do
					table.insert(sData, v)
					break
				end
			end
		end
		sData = FormatSerialize(sData)
		Connection:SendProtocol(READ_OK, SN .. ";" .. sData)
	end,
--	write = function(Connection, data)
	[NORMAL_WRITE] = function(Connection, data)
			local SN, filename, content = string.match(data, "^(%d+);([^;]+);(.*)")
			Touch(filename)
			local file, m = io.open(filename, "w")
			if not file then
--				log(string.format("ERROR:%s",m))
				Connection:SendProtocol(WRITE_FAILED, SN .. ";" .. m)
				return
			end
			file:write(content)
			file:close()
			Connection:SendProtocol(WRITE_OK, SN)
		end,
--	append = function(Connection, data)
	[APPEND] = function(Connection, data)
			local filename, content = string.match(data, "^([^;]+);(.*)")
			Touch(filename)
			local file, m = io.open(filename, "a")
			if not file then
--				log(string.format("ERROR:%s",m))
				Connection:SendProtocol("append_failed", m)
				return
			end
			file:write(msgpack.unpack(content))
			file:close()
		end,
--	read = function(Connection, data)
	[NORMAL_READ] = function(Connection, data)
			local SN, filename = string.match(data, "^(%d+);(.+)")
		--	Touch(filename)
			local file, m = io.open(filename, "r")
			if not file then
--				log(string.format("ERROR:%s",m))
				Connection:SendProtocol(READ_FAILED, SN .. ";" .. m)
				return
			end
			local content = file:read("*a")
			file:close();
			Connection:SendProtocol(READ_OK, SN .. ";" .. content)
		end,
--	dbcount = function(Connection, data)
	[DB_COUNT] = function(Connection, data)
			local SN, filename = string.match(data, "^(%d+);(.+)")
			local ret, msg = DBCount(filename)
			if not ret then
	--			log(string.format("ERROR:%s",msg))
				Connection:SendProtocol(READ_FAILED, SN .. ";" .. msg)
				return
			end
			local sData = {name = string.match(filename, "^[%w_]+.([%w_]+)"), num = ret}
			sData = Serialize(sData)
			Connection:SendProtocol(READ_OK, SN .. ";" .. sData)
		end,
--	dbremove = function(Connection, data)
	[DB_REMOVE] = function(Connection, data)
			local SN, filename, match = string.match(data, "^(%d+);([^;]+);(.*)")
			match = UnSerialize(match)
			local ret, msg = DBRemove(filename, match)
			if not ret then
	--			log(string.format("ERROR:%s", msg))
				Connection:SendProtocol(WRITE_FAILED, SN .. ";" .. msg)
				return
			end
			Connection:SendProtocol(WRITE_OK, SN)
		end,
--	ensureIndex = function(Connection, data)
	[ENSURE_INDEX] = function(Connection, data)
			local SN, filename, match = string.match(data, "^(%d+);([^;]+);(.*)")
			local key = UnSerialize(match)
			local ret, msg = DBEnsureIndex(filename, key)
			if not ret then
--				log(string.format("ERROR:%s", msg))
				Connection:SendProtocol(WRITE_FAILED, SN .. ";" .. msg)
				return
			end
			Connection:SendProtocol(WRITE_OK, SN)
		end,
--	dropIndex = function(Connection, data)
	[DROP_INDEX] = function(Connection, data)
			local SN, filename, match = string.match(data, "^(%d+);([^;]+);(.*)")
			local key = UnSerialize(match)
			local ret, msg = DBDropIndexByKey(filename, key)
			if not ret then
--				log(string.format("ERROR:%s", msg))
				Connection:SendProtocol(WRITE_FAILED, SN .. ";" .. msg)
				return
			end
			Connection:SendProtocol(WRITE_OK, SN)
		end,
	[DB_UPDATE] = function(connection, data)
			local SN, filename, match, content = string.match(data, "^(%d+);([^;]+);([^;]+);(.*)")		-- filename : "DBname.TBLname"
			match = UnSerialize(match)
			content = UnSerialize(content)
			local rst, rtn = DBUpdate(filename, match, content, true)
			if not rst then
				connection:SendProtocol(WRITE_FAILED, SN .. ";" .. rtn)
				return
			end
			connection:SendProtocol(UPDATE_OK, SN)
		end,
	[DB_INSERT] = function(connection, data)
		local sn, filename, content = string.match(data, "^(%d+);([^;]+);(.*)")
		content = UnSerialize(content)
		local rst, rtn = DBInsert(filename, content)
		if not rst then
			return connection:SendProtocol(WRITE_FAILED, sn .. ";" .. rtn)
		end
		connection:SendProtocol(WRITE_OK, sn)
	end
}

