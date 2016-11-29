clsCon = SOCKET.clsConnection:Inherit()
function clsCon:__init__()
	Super(clsCon).__init__(self)
	self.__buffer = ""
	return true
end

CONNECT_NORMAL = 1
CONNECT_TESTER = 2

--在线玩家连接
local allconnection = {}
setmetatable(allconnection, {__mode="v"})
function GetConnection(roleId)
	return allconnection[roleId]
end

function SetConnection(roleId, con)
	allconnection[roleId] = con
end

function GetAllConnection()
	return allconnection
end

function GetOnlineUserNum()
	return table.size(allconnection)
end

function IsUserOnline(roleId)
	if not USER.GetOnlineUser(roleId) then
		return false
	end
	if not GetConnection(roleId) then
		return false
	end
	return true
end


--所有连接的心跳包计数
local allconPacketCount = {}
function GetPacketCountByUsername(roleId)		--获取
	return allconPacketCount[roleId] or 0
end

function SetPacketCountByUsername(roleId, data)		--修改
	allconPacketCount[roleId] = (allconPacketCount[roleId] or 0) + data
end

function ResetPacketCountList()		--置0
	for k in pairs(allconPacketCount) do
		allconPacketCount[k] = 0
	end
end
--检查心跳包数，为0则判断为掉线
function CheckPacketCountList()
	for k, v in pairs(allconPacketCount) do
		if v <= 0 then
			local con = GetConnection(k)
			if con then
				if con.user then
					EXIT.UserExit(con.user)
				end
				SetConnection(k)
			end
			allconPacketCount[k] = nil
		end
	end
	ResetPacketCountList()
end


function clsCon:ReadCB(Stream, zip, pro)
	if tonumber(zip) == 1 then
		Stream = lengine.uncompress(Stream)
	end
	DATA.proLog(self.username, PROTOCOL.GetProtocolNameById(pro), Stream)
	self:Process(Stream, pro)
end

function clsCon:OnError(flag)  --断开连接处理函数
	if not self.roleId then
		return
	end
	DATA.log(self.roleId .. " disconnect with : " .. flag)
	if self.type == CONNECT_NORMAL then
		allconPacketCount[self.roleId] = nil
		self.type = nil
		print("onerror testBug:", self.testBug)
		SetConnection(self.roleId)
		if self.user then
			EXIT.UserExit(self.user)
		end
	else
		SetConnection(self.roleId)
	end	
end


function clsCon:Process(Msg, ProID)
	ProID = ProID and tonumber(ProID)
	if not ProID then
		print("ERROR no cmd", Msg)
		return
	end
	local ProName = PROTOCOL.GetProtocolNameById(ProID)
	if not ProName then
		print("ERROR no PRONAME", Msg, self.username, ProID, os.time())
		return
	end
	if self.roleId and self.type ~= CONNECT_TESTER then
		SetPacketCountByUsername(self.roleId, 1)
	end
	local Data = lengine.jsondecode(Msg)
	assert(PROTOCOL[ProName], ProName .. Msg)
	return PROTOCOL[ProName](self, Data)
end


function clsCon:SendProtocol(Cmd, Arg, debug)
	if not self:GetFd() then
		return
	end
	if self.user and not self.user:GetLoginState() then			--在 LoginProcess 过程中屏蔽协议发送
		return
	end
	Arg = JSON.encode(Arg)
	DATA.proLog(self.username, Cmd, Arg)
	local ProID = PROTOCOL.GetProtocolIdByName(Cmd)
	CData = lengine.compress(Arg)
	local OutData = CData
	local IsZip = 1
	if string.len(Arg) < string.len(CData) then
		OutData = Arg
		IsZip = 0
	end
	return self:Write(OutData, IsZip, ProID)
end


clsSer = SOCKET.clsServer:Inherit()
function clsSer:OnAccept()
	local Connector, IP = self:Accept(clsCon)
	if USER.IsIPFreezed(IP) then
		return Connector:Close()
	end
	Connector.IP = IP
end

Server = clsSer:New("*", PORT_CONNECT_SERVER)



---权限策略服务器
clsPolicyCon = SOCKET.clsConnection:Inherit()
clsPolicyServer = SOCKET.clsServer:Inherit()
function clsPolicyServer:OnAccept()
	local Connector, Ip = self:Accept(clsPolicyCon)
	Connector:WritePolicy('<?xml version="1.0"?>\n<!DOCTYPE cross-domain-policy SYSTEM "http://www.macromedia.com/xml/dtds/cross-domain-policy.dtd">\n<cross-domain-policy>\n<site-control permitted-cross-domain-policies="all"/>\n<allow-access-from domain="*" to-ports="*"/>\n<allow-http-request-headers-from domain="*" headers="*"/>\n</cross-domain-policy>\0')
end

Policy = clsPolicyServer:New("*", PORT_POLICYSERVER)



function AfterLogin(user)
	user:SetDurativeLogin(1)				--记录玩家连续登陆天数
	DelVerified(user:GetUserName())			--删除验证信息
end
--登录处理
function LoginProcess(connection, user)			
	
	local roleId = user:GetRoleId()	
	SetConnection(roleId, connection)
	connection.type = CONNECT_NORMAL
	connection.roleId = roleId
	
	user:SetLoginState()			--在登录处理过程中屏蔽所有发往客户端的协议
	user:SetLogin(connection.IP)
	user:SetOfflineProState(true)		--进入离线操作处理阶段
	USER.SetOnlineUser(roleId, user)				--设置玩家数据在线
	connection.testBug = 0

	local function AfterPro(ret)
		if not ret then
			return connection:SendProtocol("c_check_error", "data_error")
		end
		user:SetOfflineProState(false)			--离线操作处理阶段结束
		user:ProOfflineData()
		user:LoginProUserData()
		user:LoginStartCallOut()				--上线时启动定时器
		user:SetLoginState(1)					--解除发往客户端的协议屏蔽
		user:SetFuncStatById(SETTING.FUNC_DALIY_LOGIN, 1)			--每日登录
		user:SendOnlineMsg()
		connection:SendProtocol("c_login", user:GetUserData())
		AfterLogin(user)
		DATA.log(string.format("%s登录了游戏，IP：%s，当前在线玩家人数：%d", roleId, connection.IP, GetOnlineUserNum()), "login.log")
	end
	OFFLINE.LoginProOffline(user, AfterPro)			--处理离线操作
end



--用户登陆处理*
function CheckUserData(connection, roleId)
	local function User_Login_Back(data, errmsg)
		if not data then
			DATA.log(string.format("玩家%s登录游戏失败，没有用户数据。", roleId, errmsg or ""), "login.log")
			return connection:SendProtocol("c_check_error", "data_error")
		end
		data = UnSerialize(data)
		if not data or not data[1] then
			DATA.log(string.format("玩家%s登录游戏失败，%s", roleId, errmsg or ""), "login.log")
		 	return connection:SendProtocol("c_check_error", "data_error")
		end
		local user = USERBASE.clsUser:New()
		user:Restore(data[1])
		user:SetLoginState(1)
		connection.user = user
		connection.testBug = 1
		connection:SendProtocol("c_login_ready", roleId)
	end
	if connection.user then
		connection.user:SetLoginState(1)
		connection:SendProtocol("c_login_ready", roleId)
		return
	end
	if not USER.ReadUserData(roleId, User_Login_Back) then
		return connection:SendProtocol("c_check_error", "update_error")
	end
end


function DisconnectOnlineUser(connection, roleId)
	local con = GetConnection(roleId)
	if con and con:GetFd() and con ~= connection then
		if con.user and con.roleId == roleId then
			connection.user = con.user
			connection.testBug = 2
		end
		con:OnError(con:GetFd())
		con.username = nil
		con.roleId = nil
		con.user = nil
		con:Close()
	end
end



local illegalNameTbl = {"SYSTEM", "System", "system", ".*系.*统.*", ".*GM.*", "botao", ".*博淘.*"}   --待加
local exception = {["_"] = 1, ["~"] = 1, ["-"] = 1, ["@"] = 1, ["."] = 1}
function UserNameIsIllegal(username)
	if string.find(username, "[%c%s%z]+") then
		return true
	end
	for w in string.gmatch(username, "(%p+)") do
		if not exception[w] then
			return true
		end
	end
	if string.len(username) <= 0 then
		return true
	end
	for k, rule in ipairs(illegalNameTbl) do
		if string.match(username, rule) then
			return true
		end
	end
	return false
end

function IsAccountDown(username)
	local roleId = USER.GetRoleIdByUserName(username)
	if roleId then
		return USER.GetUserNickInfo(roleId, "state") == 0
	end
	return false
end

AllRegistedUser = {}
SAVE_FILE_NAME = "chaos.reginfo"

--保存玩家注册信息
function RecordRegInfo(name, pwd)
	AllRegistedUser[name] = {state = 1, passwd = lengine.md5(pwd)}
	DATA.SaveData(SAVE_FILE_NAME, AllRegistedUser, name)
end


local s_user_register = function(connection, data)
	if type(data) ~= "table" then
		return
	end
	if not data.username or not data.pwd then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "用户名或密码错误"})
		return
	end
	if UserNameIsIllegal(data.username) then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "用户名存在非法字符"})
		return
	end
	if tonumber(data.username) then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "用户名不能使用纯数字"})
		return
	end
	if string.len(data.username) < 4 then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "用户名不能少于4字符"})
		return
	end
	if string.len(data.username) > 16 then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "用户名不能超过16字符"})
		return
	end
	if AllRegistedUser[data.username] or data.username == "system" then
		connection:SendProtocol("c_user_register", {result = false, errorMsg = "该用户名已经被占用了"})
		return
	end
	RecordRegInfo(data.username, data.pwd)
	DATA.log(string.format("registe username=%s", data.username), "login.log")
	connection:SendProtocol("c_user_register", {result = true})
end

--玩家登录验证列表
local UserVerified = {}

function IsVerified(username)
	return UserVerified[username]
end

function SetVerified(username)
	UserVerified[username] = os.time()
end

function DelVerified(username)
	UserVerified[username] = nil
end

--登录验证
local s_verify = function(connection, data)
	if not data.username or not data.pwd then
		return connection:SendProtocol("c_verify_error", "pro_error")
	end
	if not SYSTEM_LOGIN_STATE then
		return connection:SendProtocol("c_verify_error", "system_updating")
	end
	if connection.username then
		return connection:SendProtocol("c_verify_error", "repeat_verify")
	end
	if not AllRegistedUser[data.username] or lengine.md5(data.pwd) ~= AllRegistedUser[data.username].passwd then
		connection:SendProtocol("c_verify_error", "user_error")
		return
	end
	local roleId = USER.GetRoleIdByUserName(data.username)
	if roleId and USER.GetUserNickInfo(roleId, "state") == 0 then
		return connection:SendProtocol("c_verify_error", "account_down")
	end
	if UserNameIsIllegal(data.username) then
		DATA.log(string.format("普通接口登录失败，%s用户名非法", tostring(data.username) or "null"), "login.log")
		return connection:SendProtocol("c_verify_error", "name_illegal")
	end
	SetVerified(data.username)					--验证通过
	connection:SendProtocol("c_verify", data.username)
	
	connection.username = data.username
	if not roleId then
--		return connection:SendProtocol("c_login_play", data.username)
		return connection:SendProtocol("c_new_user", data.username)
	end
	DisconnectOnlineUser(connection, roleId)			--断开在线玩家
	CheckUserData(connection, roleId)				--检查用户数据（读取用户数据）
end

local s_login_play = function(connection, data)
	if not IsVerified(data) then
		return connection:SendProtocol("c_check_error", "not_verified")
	end
	if USER.GetNickByUsername(data) then
		return connection:SendProtocol("c_check_error", "user_exist")
	end
	connection:SendProtocol("c_new_user", data)
end

--[[
local s_login = function(connection, data)
	if not data.name or not data.pwd then
		return connection:SendProtocol("c_login_error", "user_error")
	end
	if USER.AllUserNick[data.name] and USER.AllUserNick[data.name].state == 0 then
		return connection:SendProtocol("c_login_error", "account_forbid")
	end
	if SELLER.AllSellerNick[data.name] then
		connection:SendProtocol("c_login_error", "account_taken")
		return
	end
	local function login_back(code, ret)
		if code ~= 200 then
			return connection:SendProtocol("c_login_error", "connect_error")
		else
			local match = string.match(ret, "[%c%p%s%z]*([%w_]*)$")
			if match ~= "true" then
				return connection:SendProtocol("c_login_error", match)
			end
		end
		NormalLogin(connection, HTTP.UrlDecode(data.name))
	end
	HTTP.Request(string.format("http://www.lovefish100.com/logingame.php?&name=%s&pwd=%s", data.name, data.pwd), login_back)
end
]]
--玩家验证后登录
local s_login = function(connection, data)
	if not data then
		return connection:SendProtocol("c_login_error", "pro_error")
	end
--[[	local username = USER.GetUserNameById(data)
	if username ~= connection.username then
		return connection:SendProtocol("c_login_error", "con_error")
	end
	if not IsVerified(username) then
		return connection:SendProtocol("c_login_error", "not_verified")
	end]]
	LoginProcess(connection, connection.user)
end

local s_user_modPwd = function(connection, data)
	local username = connection.username
	if not data.oldPwd or not data.newPwd then
		return connection:SendProtocol("c_user_modPwd", {result = false, msg = "请重新输入旧密码及新密码！"})
	end
	if not AllRegistedUser[username] or lengine.md5(data.oldPwd) ~= AllRegistedUser[username].passwd then
		return connection:SendProtocol("c_user_modPwd", {result = false, msg = "用户名或者密码错误，请重新输入"})
	end
	RecordRegInfo(username, data.newPwd)
	return connection:SendProtocol("c_user_modPwd", {result = true, msg = "修改成功！"})
end

local s_login_timer = function(connection, data)
	local user = USER.GetOnlineUser(connection.roleId)
	if not user then
		return
	end
	SetPacketCountByUsername(connection.roleId, 1)
end







function __init__()
	PROTOCOL.s_user_register = s_user_register
	PROTOCOL.s_verify = s_verify
	PROTOCOL.s_login = s_login
	PROTOCOL.s_login_play = s_login_play

	PROTOCOL.s_user_modPwd = s_user_modPwd
	PROTOCOL.s_login_timer = s_login_timer

	DATA.Restore(SAVE_FILE_NAME, AllRegistedUser)
	CALLOUT.CallFre(CheckPacketCountList, 5 * 60)
end

