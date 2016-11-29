
function SendMoney(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	local tb
	if roleId then
		tb[roleId] = 1
	else
		tb = USER.GetAllUserNick()
	end
	for k, v in pairs(tb) do
		local user = USER.GetOnlineUser(k)
		if user then
			if data.ingot then
				user:SetIngot(data.ingot)
			end
			if data.voucher then
				user:SetIngot(data.voucher)
			end
		else
			if data.voucher then
				OFFLINE.SetOfflineData(k, "voucher", data.voucher)
			end
			if data.ingot then
				OFFLINE.SetOfflineData(k, "ingot", data.ingot)
			end
		end
	end
	return true
end

function SendMail(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	local tb
	if roleId then
		tb[roleId] = 1
	else
		tb = USER.GetAllUserNick()
	end
	for k, v in pairs(tb) do
		MAIL.SendSysMail(k, data.mail) 
	end
	return true
end


function __init__()
	GAMEMANAGE.RegisterFunc(CMD.SEND_MONEY, SendMoney)
	GAMEMANAGE.RegisterFunc(CMD.SEND_MAIL, SendMail)
end
