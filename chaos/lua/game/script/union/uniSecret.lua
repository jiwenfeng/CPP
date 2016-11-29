--联盟秘境

function GetUniSecretInfo(uniSecret, roleId)
	local sData = {}
	local state = uniSecret:GetState()
	sData.state = state
	if state == CLSUNION.ACT_BEGIN then
		sData.deityLv = uniSecret:GetDeityLv()
		sData.summonNum = uniSecret:GetSummonNum()
		sData.summonNeed = uniSecret:GetUpgradeSummNum()
		sData.baseRate = uniSecret:GetBaseExpRate()
		sData.sacrifNum = uniSecret:GetSacrificeTimes()
		sData.sacrifMax = CLSUNISECRET.GetMaxSacrificeTimes()
		sData.extRate = uniSecret:GetAdditionExpRete()
		sData.isSummon = 0
		sData.isSacrif = uniSecret:IsSacrificed(roleId) or 0
		sData.timelong = uniSecret:GetSummonTimestamp() + CLSUNISECRET.GetDeityLastTimelong() - os.time()
	else
		local union = UNION.GetUnionById(uniSecret:GetOwner())
		local info = union:GetMemberInfo(roleId)
		if not info or info.identity > CLSUNION.VICELEADER then
			sData.isSummon = 0
		else
			sData.isSummon = state == CLSUNION.ACT_UNSTART and 1 or 0
		end
		sData.isSacrif = 1
	end	
	return sData
end


--进入秘境
local s_uniSecret_enter = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if os.time() - info.joinTime < CLSUNION.GetMinWaitingTime() and union:GetCreator() ~= connection.roleId then
--		return ERROR.TipMsg(connection, string.format("您加入联盟不足%s小时，不能进入联盟秘境。", math.ceil(CLSUNION.GetMinWaitingTime() / 3600)))
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if not uniSecret then
		return ERROR.TipMsg(connection, "联盟秘境未开启")
	end
	local sceneObj = uniSecret:GetSecretScene()
	if not sceneObj then
		return ERROR.TipMsg(connection, "联盟秘境场景错误。")
	end
	uniSecret:InitRoleInfo(connection.roleId)
	connection:SendProtocol("c_uniSecret_enter", sceneObj:GetId())
--	PROTOCOL.s_scene_enter(connection, sceneObj:GetId())
end
--获取秘境信息
local s_uniSecret_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	local sData = GetUniSecretInfo(uniSecret, connection.roleId)
	connection:SendProtocol("c_uniSecret_info", sData)
end

--向秘境内的成员进行更新
function UpdateUniSecretInfo(uniSecret)
	local sceneObj = uniSecret:GetSecretScene()
	for k, v in pairs(sceneObj:GetSceneRoleList()) do
		if v:GetRoleType() == "user" then
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				PROTOCOL.s_uniSecret_info(con, "get")
			end
		end
	end
end
--召唤玄冥
function SummonDeity(union)
	local uniSecret = union:GetUnionAct("uniSecret")
	uniSecret:SummonDeity()
	UpdateUniSecretInfo(uniSecret)
	local msg = "秘境内玄冥已被召唤，进入可获得大量经验。"
	CHAT.SendSysMsg("union", msg, union:GetListByLevel(CLSUNION.COMMON), {kind = "uniSecret"})
end
--玄冥消失
function EndSummon(unionId)
	local union = UNION.GetUnionById(unionId)
	if union then
		local uniSecret = union:GetUnionAct("uniSecret")
		uniSecret:EndSummon()
		UpdateUniSecretInfo(uniSecret)
		local msg = "玄冥渐渐消失，秘境中的成员不再获得额外的经验收益。"
		CHAT.SendSysMsg("union", msg, union:GetListByLevel(CLSUNION.COMMON))
	end
end


function QuitSecret(user)
--[[	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return false, "您没有加入联盟"
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if not uniSecret then
		return false, "联盟秘境未开启"
	end]]
	user:LeaveScene()
	return SCENE.GetReturnedTownId(user)
end

--召唤玄冥
local s_uniSecret_summon = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if os.time() - info.joinTime < CLSUNION.GetMinWaitingTime() and union:GetCreator() ~= connection.roleId then
		return ERROR.TipMsg(connection, string.format("您加入联盟不足%s小时，不能进入联盟秘境。", math.ceil(CLSUNION.GetMinWaitingTime() / 3600)))
	end
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作。")
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if not uniSecret then
		return EROR.TipMsg(connection, "联盟秘境未开启")
	end
	if uniSecret:GetState() == CLSUNION.ACT_OVER then
		return ERROR.TipMsg(conneciton, "活动已结束")
	end
	if uniSecret:GetState() ~= CLSUNION.ACT_UNSTART  then
		return ERROR.TipMsg(connection, "联盟秘境已开启")
	end
--	CLSUNISECRET.SetUniSecretOpen(union)
--	local uniSecret = union:GetUnionAct("uniSecret")
--	if uniSecret:GetState() ~= CLSUNION.ACT_UNSTART then
--		return ERROR.TipMsg(connection, "玄冥已经被召唤过了。")
--	end
	SummonDeity(union)
	connection:SendProtocol("c_uniSecret_summon", "true")
end

-- 献祭
local s_uniSecret_sacrifice = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您还没有加入联盟")
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if uniSecret and uniSecret:GetState() ~= CLSUNION.ACT_BEGIN then
		return ERROR.TipMsg(connection, "玄冥没有被召唤，不能进行献祭。")
	end
	if uniSecret:IsSacrificed(connection.roleId) then
		return ERROR.TipMsg(connection, "您今日已经献祭过了。")
	end
	uniSecret:ProSacrifice(connection.roleId)
	user:SetUnionSacrificeTimes(1)
	connection:SendProtocol("c_uniSecret_sacrifice", "true")
	uniSecret:SendUniSecretUpdate("sacrifice")
end

-- 离开秘境
local s_uniSecret_leave = function(connection, data)
--[[	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if not uniSecret then
		return ERROR.TipMsg(connection, "联盟秘境未开启")
	end
	local sceneObj = uniSecret:GetSecretScene()
	if user:GetScene() ~= sceneObj then
		return ERROR.TipMsg(connection, "您当前不在秘境中。")
	end
	user:LeaveScene()
	local sceneId = SCENE.GetReturnedTownId(user)
	connection:SendProtocol("c_uniSecret_leave", sceneId)]]
--	return SCENE.ReturnToTown(user)
end

local s_uniSecret_open = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作")
	end
	if os.time() - info.joinTime < CLSUNION.GetMinWaitingTime() and union:GetCreatot() ~= connection.roleId then
		return ERROR.TipMsg(connection, string.format("您加入联盟不足%s小时，不能进入联盟秘境。", math.ceil(CLSUNION.GetMinWaitingTime() / 3600)))
	end
	local uniSecret = union:GetUnionAct("uniSecret")
	if uniSecret then
		return ERROR.TipMsg(connection, "联盟秘境已开启")
	end
	CLSUNISECRET.SetUniSecretOpen(union)
	connection:SendProtocol("c_uniSecret_open", data)
--	PROTOCOL.s_uniSecret_enter(connection, "set")
end

function __init__()

	PROTOCOL.s_uniSecret_enter = s_uniSecret_enter	
	PROTOCOL.s_uniSecret_info = s_uniSecret_info	
	PROTOCOL.s_uniSecret_summon = s_uniSecret_summon	
	PROTOCOL.s_uniSecret_sacrifice = s_uniSecret_sacrifice	
	PROTOCOL.s_uniSecret_leave = s_uniSecret_leave	
--	PROTOCOL.s_uniSecret_open = s_uniSecret_open
end
