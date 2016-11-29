local illegalNameTbl = {}   --屏蔽词库
local MIN_NAME_LEN = 4
local MAX_NAME_LEN = 12

local UserModelList = {}
local HeroHeadIdList = {}

--转换为合法的字符串(转换屏蔽字符为*)
function TranslateLegalStr(str)
	local temp = str
	for _, v in pairs(illegalNameTbl) do
		temp = string.gsub(temp, v, "*")
	end
	return temp
end

--屏蔽非法名称
function NameIsIllegal(name)
	local len, cnChar = utfstrlen(name)
	if len + cnChar < MIN_NAME_LEN then
		return true, "名字的长度不能太短。"
	end
	if len + cnChar > MAX_NAME_LEN then
		return true, "名字的长度不能太长。"
	end
	if string.find(name, "[%c%s%z]+") then
		return true, "名字中存在非法字符。"
	end
	for _, rule in ipairs(illegalNameTbl) do
		if string.match(name, rule) then
			return true, "名字中不能含有敏感字符。"
		end
	end
	return false
end

function FormatUserModelList()
	local sData = {}
	for k, v in pairs(UserModelList) do
		for sex, info in pairs(v) do
			local buf = {}
			buf.job = k
			buf.sex = sex
			buf.headId = info.headId
			buf.picId = info.modelId
			table.insert(sData, buf)
		end
	end
	table.sort(sData, function(a, b) return a.headId < b.headId end)
	return sData
end

function GetHeroHeadIdList(modelId)
	return HeroHeadIdList[modelId]
end

function GetHeadIdByModelId(modelId)
	local list = GetHeroHeadIdList(modelId)
	return list[math.random(1, #list)]
end

function GetModelInfoIdByJob(job, sex)
	return UserModelList[job][sex]
end

--新建用户初始化新手引导数据
function InitUserGuideData(user)
--	user:SetUserGuideStep(0)
end

--新建用户*
function NewUser(connection, user)
	local function new_user_back(data, errmsg)
		if not data then
			connection:SendProtocol("c_new_user_error", "server_error")
			return DATA.log(string.format("NewUser error:%s", errmsg), "newuser.log")
		end
		user:SetLoginState(1)
		connection:SendProtocol("c_newuser_finish", user:GetName())

		connection.user = user
		connection.roleId = user:GetRoleId()
		DATA.log(string.format("%s新建了玩家数据", connection.username), "newuser.log")
		connection:SendProtocol("c_login_ready", connection.roleId)
	end
	USER.SaveUserData(user, new_user_back)
end


local s_new_modelList = function(connection, data)
	if data ~= "get" then
		return connection:SendProtocol("c_new_modelList", "error_request")
	end
	local sData = FormatUserModelList()
	connection:SendProtocol("c_new_modelList", sData)
end

local s_new_user = function(connection, data)
	if not data.nick or NameIsIllegal(data.nick) then
		return connection:SendProtocol("c_new_user_error", "nick_illegal")
	end
	if NAME.IsNameExist(data.nick) then
		return connection:SendProtocol("c_new_user_error", "nick_taken")
	end
	if not data.headId or not data.job or not data.sex or not data.picId then
		return connection:SendProtocol("c_new_user_error", "pro_error")
	end
	if not JOB.GetJobInfoById(data.job) then
		return connection:SendProtocol("c_new_user_error", "pro_error")
	end
	local user = USERBASE.clsUser:New()
	local userId = USER.GetRoleIdByUserName(connection.username, SERVER_ID)
	if not userId then
		userId = STATS.GenId("user")			--统一Id生成器
	end
	user:InitUser(connection.username, data.nick, userId, data.headId, data.job, data.sex, data.picId)
	user:SetLogin(connection.IP)
	InitUserGuideData(user)
	NAME.SetNameExist(data.nick)
	return NewUser(connection, user)
end

function __init__()
	local tb = Import("autocode/userModelList.lua")
	UserModelList = tb.GetTable()
	
	local ILLEGAL = Import("autocode/illegalName.lua")
	illegalNameTbl = ILLEGAL.GetTable()
	
	PROTOCOL.s_new_user = s_new_user
	PROTOCOL.s_new_modelList = s_new_modelList
end
