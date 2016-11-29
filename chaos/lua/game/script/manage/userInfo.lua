
function ResetVigour(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, "没有该用户"
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:InitVigour()
	else
		OFFLINE.SetOfflineData(roleId, "initVigour")
	end
	return true
end

function RemoveIndent(connection, data) 
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, "没有该用户"
	end
	BROUSE.RemoveIndentByRoleId(roleId)
	return true
end

function FinishMission(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, "没有该用户"
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:FinishAllAcpMission()
	else
		OFFLINE.SetOfflineData(roleId, "finishMission")
	end
	return true
end


function SendBack(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户%s", data.nick)
	end
	local user = USER.GetOnlineUser(roleId)
	local sceneObj = SCENE.GetTownScene(USERBASE.INIT_SCENE)
	local sceneId = sceneObj:GetId()
	if not user then
		OFFLINE.SetOfflineData(roleId, "setLocate", USERBASE.INIT_SCENE,USERBASE.INIT_POS_X, USERBANSE.INIT_POS_Y)
	else
		user:Goto(USERBASE.INIT_POS_X, USERBASE.INIT_POS_Y, sceneObj)
	end
	return true
end

function AttachMainCity(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户%s", data.nick)
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		return user:AttackToMainCity()
	else
		OFFLINE.SetOfflineData(roleId, "attachToMaster")
		return true
	end
end

function LoadUserInfo(connection, data)
--[[	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户%s", data.nick)
	end
	local user = USER.GetOnlineUser(roleId)
	local sData = {}
	if user then
		sData.fight = user:GetWorkState() == "fight"
		sData.mopup = user:GetCurMopup() and 1 or 0
	else
		sData.vipLv = USER.GetUserNickInfo(roleId, "vipLv") or 0
		sData.fight = 0
		sData.mopup = 0
		sData.phyleFight = 0
	end
	return true, sData]]
end

function GetUserTitleList(connection, data)
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return false, string.format("没有该用户%s", data.nick)
	end
	local user = USER.GetOnlineUser(roleId)

end

function __init__()
	GAMEMANAGE.RegisterFunc(CMD.FINISH_MISSION, FinishMission)
	GAMEMANAGE.RegisterFunc(CMD.RESET_VIGOUR, ResetVigour)
	GAMEMANAGE.RegisterFunc(CMD.REMOVE_INDENT, RemoveIndent)
	GAMEMANAGE.RegisterFunc(CMD.SEND_TO_INIT_CITY, SendBack)
	GAMEMANAGE.RegisterFunc(CMD.ATTACH_TO_MAIN_CITY, AttachMainCity)
end

