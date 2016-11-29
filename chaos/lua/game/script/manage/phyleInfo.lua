function GetPhyleFightInfo(connection, data)
	local sData = {}
	sData = CLSNODE.CountFighterNumByPhyle()
	sData.start = string.format("%d:%d", CLSNODE.ACT_READY_HOUR, CLSNODE.ACT_READY_MIN)
	sData.over = string.format("%d:%d", CLSNODE.ACT_END_HOUR, CLSNODE.END_MIN)
	sData.state = NODE.GetActState() == "over" and 0 or 1
	return true, sData
end

function GetPhyleFighterList(connection, data)
	local sData = {}
	for k, v in pairs(CLSNODE.GetAllRoleInfoList()) do
		local buf = {}
		buf.roleId = k
		buf.nick = USER.GetUserNickInfo(k, "nick")
		buf.phyle = USER.GetUserNickInfo(k, "phyle")
		buf.nodeId = v.nodeId
		local node = NODE.GetNodeObjById(v.nodeId)
		buf.type = node:GetMemberType(k)
		table.insert(sData, buf)
	end
	return true, sData
end

function RmPhyleFighter(connection, data)
	local info = CLSNODE.GetRoleInfoById(data)
	if not info then
		return false, "参战人员中没有该玩家信息"
	end
	local node = NODE.GetNodeObjById(info.nodeId)
	if node then
		node:RmMember(data)
	end
	return true
end

function __init__()
	GAMEMANAGE.RegisterFunc(CMD.LOAD_PHYLE_INFO, GetPhyleFightInfo)
	GAMEMANAGE.RegisterFunc(CMD.REMOVE_PHYLE_FIGHTER, RmPhyleFighter)
	GAMEMANAGE.RegisterFunc(CMD.LOAD_PHYLE_FIGHTERLIST , GetPhyleFighterList)
end
