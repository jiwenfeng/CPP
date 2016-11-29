function CancelUnionDelate(connection, data)
	local union = UNION.GetUnionByName(data.unionName)
	if not union then
		return false, string.format("没有联盟:%s", data.unionName)
	end
	if not union:GetDelateInfo() then
		return false, "该联盟没有弹劾盟主"
	end
	union:CancelDelate()
	return true 
end

function ChangeUnionOwner(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户:%s", data.nick)
	end
	local union = UNION.GetUnionByName(data.unionName)
	if not union then
		return false, "指定的联盟不存在"
	end
	if not union:IsInList("member", roleId) then
		return false, string.format("%s不是%s里的成员", data.nick, data.unionName)
	end
	union:ModMemberIdentity(union:GetLeader(), CLSUNION.COMMON)
	union:ModMemberIdentity(roleId, CLSUNION.LEADER)
	if union:GetDelateInfo() then
		union:CancelDelate()
	end
	return true
end

function GetUnionInfo(connection, data)
	local union = UNION.GetUnionById(data.unionName)
	if not union then
		return false, "无效的联盟编号"
	end
	local sData = {}
	sData.unionId = data
	sData.unionName = union:GetName()
	sData.unionLv = union:GetLv()
	sData.phyle = union:GetPhyle()
	sData.creator = union:GetCreator()
	sData.timestamp = union:GetCreateTime()
	sData.number = string.format("%d/%d", union:GetMemberNum(), union:GetMaxMemberNum())
	sData.wealth = union:GetWealth()
	local lvInfo = CLSUNION.GetUnionInfoByLv(union:GetLv())
	sData.needWealth = lvInfo.wealth 
	sData.force = union:GetTotalForce()
	sData.announce = union:GetAnnounce()
	sData.memberList = union:FormatList("member")
	sData.applyList = union:FormatList("apply")
	sData.delateState = union:GetDelateInfo() and 1 or 0
	return true, sData
end

function __init__()
	GAMEMANAGE.RegisterFunc(CMD.CHANGE_UNION_LEADER, ChangeUnionOwner)
	GAMEMANAGE.RegisterFunc(CMD.CANCEL_UNION_DELATE, CancelUnionDelate)
	GAMEMANAGE.RegisterFunc(CMD.LOAD_UNION_INFO, GetUnionInfo)
end
