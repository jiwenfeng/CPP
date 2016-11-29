--后台指令解析

local funcList = {}

function GetGmFuncById(funcId)
	return funcList[funcId]
end

function RegisterFunc(funcName, func)
	funcList[funcName] = func
end

function ProGmFuncById(funcId, connection, pro, data)
	local func = GetGmFuncById(funcId)
	if not func then
		return connection:SendProtocol(pro, {result = false, data = "无效的GM协议。"})
	end
	local ret, msg = func(connection, data)
	return connection:SendProtocol(pro, {result = ret, data = msg})
end
