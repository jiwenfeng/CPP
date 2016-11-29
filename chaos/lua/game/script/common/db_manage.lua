--数据库备份与恢复


local db_name = "chaos"
local BACKUP_KEEP_TIMELONG = 30 * 24 * 3600			--备份数据保留30天
local path_db_back = GAME_ROOT_PATH .. "/mdb/dbbak/"


--备份数据库
function BackupDB()
	local str = string.format("mongodump -h localhost:%d -d %s -o %s%s", PORT_BACKUP_SLAVE, db_name, path_db_back, os.date("%Y%m%d"))
	os.execute(str)
	Compress()
	DATA.log(string.format("备份数据库成功，当前版本备份日期为%s", os.date("%Y%m%d")), "dbmanage.log")
	DeleteOldDB(os.date("%Y%m%d"))

	local date = os.date("%Y%m%d", os.time() - BACKUP_KEEP_TIMELONG)
	if IsDataExist(date) then
		DeleteOldDB(date .. ".tar")
	end
	DATA.log("备份数据库全部操作完成。", "dbmanage.log")
end

function Compress()
	local str = string.format("tar -zcPf %s%s.tar %s%s", path_db_back, os.date("%Y%m%d"), path_db_back, os.date("%Y%m%d"))
	os.execute(str)
end

function DeCompress(s)
	local str = string.format("tar -xzvPf %s%s.tar", path_db_back, s)
	os.execute(str)
end

--恢复数据库
function RestoreDB(s)
	DeCompress(s)
	local str = string.format("mongorestore -h localhost:%s -d %s --directoryperdb %s%s/%s", PORT_BACKUP_MASTER, db_name, path_db_back, s, db_name)
	os.execute(str)
	DATA.log(string.format("恢复数据库成功，当前版本备份日期为%s", s), "dbmanage.log")
	DeleteOldDB(os.date("%Y%m%d"))
end

function IsDataExist(s)
	local file = string.format("%s%s.tar", path_db_back, s)
	local fd = io.open(file)
	if not fd then
		return false
	end
	fd:close()
	return true
end

--删除过期数据库
function DeleteOldDB(s)
	local str = string.format("rm -rf %s%s", path_db_back, s)
	os.execute(str)
	DATA.log(string.format("删除数据库成功，已删除版本为%s", s), "dbmanage.log")
end



function __init__()
	CALLOUT.Daily(getfenv(2), 3, 0, BackupDB)
end
