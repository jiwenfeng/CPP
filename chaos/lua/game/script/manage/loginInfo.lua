function VerifyUser(connection, data)
	conn.roleId = data.roleId
	return true
end

function BannedUser(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户信息:%s", data.nick)
	end
--[[	if not data.timelong then
		CHAT.SetUserBanned(roleId)
	else
		local timelong = tonumber(data.timelong)
		if not timelong then
			return false, string.format("无效的禁言时间：%s", data.time)
		end
		CHAT.SetUserBanned(roleId, timelong, conn.roleId)
	end]]
	CHAT.SetUserBanned(roleId, tonumber(data.timelong), conn.roleId)
	return true, data
end

function FreezeUser(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户信息:%s", data.nick)
	end
--[[	if not data.timelong then
		USER.SetUserFreeze(roleId)
	else
		local timelong = tonumber(data.timelong)
		if not timelong then
			return false, "无效的冻结时间"
		end
		USER.SetUserFreeze(roleId, timelong, conntion.roleId)
	end]]
	USER.SetUserFreeze(roleId, tonumber(data.timelong), connection.roleId)
	return true, data
end

function BannedList(connection, data)
	local list = CHAT.GetBannedList()
	local sData = {}
	for k, v in pairs(list) do
		local buf = {}
		buf.roleId = v.roleId
		buf.nick = USER.GetUserNickById(v.roleId)
		buf.timelong = v.timelong
		buf.endtime = v.time
		table.insert(sData, buf)
	end
	return true, sData
end

function FreezeList(connection, data)
	local list = USER.GetFreezeList()
	local sData = {}
	for k, v in pairs(list) do
		local buf = {}
		buf.roleId = v.roleId
		buf.nick = USER.GetUserNickById(v.roleId)
		buf.timelong = v.timelong
		buf.endtime = v.endtime
		buf.ip = v.ip
		buf.state = USER.GetOnlineUser(v.roleId) and 1 or 0
		buf.Lv = v.Lv
		buf.charge = v.charge
		table.insert(sData, buf)
	end
	return true, sData
end

function KickUser(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, "没有该用户"
	end
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return false, "用户不在线"
	end
	EXIT.UserExit(user)
	return true
end

--指定IP的登陆数量
function LoginNumByIp(connection, data)
	if not data.ip then
		return false, "无效的请求"
	end
	local num = 0
	for k, v in pairs(USER.GetAllOnlineUser()) do
		local info = v:GetLastLogin()
		if info.ip == data.ip then
			num = num + 1
		end
	end
	return true, {num = num}
end

function FreezeIP(connection, data)
	if not data.ip then
		return false, "无效的请求"
	end
	USER.FreezeIP(data.ip, tonumber(data.timelong), connection.roleId)
	return true
end

function FreezeIPList(connection, data)
	local sData = {}
	for k, v in pairs(USER.GetIpFreezeList()) do
		local buf = {}
		buf.ip = k
		buf.endtime = v.endtime
		buf.operator = v.operator
		table.insert(sData, buf)
	end
	return true, sData
end

function __init__()
	GAMEMANAGE.RegisterFunc(CMD.VERIFY_USER, VerifyUser)
	GAMEMANAGE.RegisterFunc(CMD.BANNED_USER, BannedUser)
	GAMEMANAGE.RegisterFunc(CMD.FREEZE_USER, FreezeUser)
	GAMEMANAGE.RegisterFunc(CMD.BANNED_LIST, BannedList)
	GAMEMANAGE.RegisterFunc(CMD.FREEZE_LIST, FreezeList)
	GAMEMANAGE.RegisterFunc(CMD.KICK_USER, KickUser)
	GAMEMANAGE.RegisterFunc(CMD.SPEC_IP_LOGIN_NUM, LoginNumByIp)
	GAMEMANAGE.RegisterFunc(CMD.FREEZE_IP, FreezeIP)
	GAMEMANAGE.RegisterFunc(CMD.FREEZE_IP_LIST, FreezeIPList)
end

