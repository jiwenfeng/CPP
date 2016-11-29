--联盟遗迹
	
local REVIVE_NEED_INGOT = 10		--复活所需元宝
local ENTER_RELIC_NEED_LV =  3		--进入遗迹所需联盟等级

local RewardList = {}

function GetRewardList()
	return RewardList
end

function GetRewardListByQuality(quality)
	return RewardList[quality]
end

function QuitUniRelic(user)
--[[	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return false, "您没有加入联盟"
	end]]
	user:LeaveScene()
	return SCENE.GetReturnedTownId(user)
end

--进入秘境
local s_uniRelic_enter = function(connection, data)
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
--		return ERROR.TipMsg(connection, string.format("您加入联盟不足%s小时，不能进入联盟遗迹。", math.ceil(CLSUNION.GetMinWaitingTime() / 3600)))
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local sceneObj = uniRelic:GetRelicScene()
	if not sceneObj then
		return ERROR.TipMsg(connection, "联盟遗迹场景错误。")
	end
	connection:SendProtocol("c_uniRelic_enter", sceneObj:GetId())
--	PROTOCOL.s_scene_enter(connection, sceneObj:GetId())
end

--[[ 离开秘境
local s_uniRelic_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	local sceneObj = uniRelic:GetRelicScene()
	if user:GetScene() ~= sceneObj then
		return ERROR.TipMsg(connection, "您当前不在遗迹中。")
	end
	user:LeaveScene()
	local sceneId = SCENE.GetReturnedTownId(user)
	connection:SendProtocol("c_uniRelic_leave", sceneId)
--	return SCENE.ReturnToTown(user)
end]]
--获取秘境信息
local s_uniRelic_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local sData = uniRelic:GetUniRelicInfo(connection.roleId)
	connection:SendProtocol("c_uniRelic_info", sData)
end

local s_uniRelic_autoFight = function(connection, data)
	if data ~= 1 and data ~= 0 then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local info = uniRelic:GetFighterInfo(connection.roleId)
	info.autoFight = data
	connection:SendProtocol("c_uniRelic_autoFight", data)
end

local s_uniRelic_feed = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	if uniRelic:IsBossFeed(connection.roleId) then
		return ERROR.TipMsg(connection, "您今天已经喂养过饕餮了。")
	end
	if uniRelic:GetState() ~= CLSUNION.ACT_UNSTART then
		return ERROR.TipMsg(connection, "现在已经不能进行喂食了。")
	end
	uniRelic:SetBossFeed(connection.roleId, 1)
	connection:SendProtocol("c_uniRelic_feed", "true")
end

local s_uniRelic_release = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local state = uniRelic:GetState()
	if state ~= CLSUNION.ACT_UNSTART then
		return ERROR.TipMsg(connection, "饕餮已经被释放了。")
	end
	local roleInfo = union:GetMemberInfo(connection.roleId)
	if roleInfo.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主或副盟主才能释放饕餮。")
	end
	uniRelic:BossActReady(connection.roleId)
	connection:SendProtocol("c_uniRelic_releace", "true")
end

local s_uniRelic_bossInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	return uniRelic:SendBossInfoUpdate(connection.roleId)
end
--[[
local s_uniRelic_fight = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local state = uniRelic:GetState()
	if state == CLSUNION.ACT_OVER then
		return ERROR.TipMsg(connection, "战斗已经结束了。")
	end
	if state ~= CLSUNION.ACT_BEGIN then
		return ERROR.TipMsg(connection, "战斗还未开始。")
	end
	if uniRelic:IsRoleDead(connection.roleId) then
		return ERROR.TipMsg(connection, "您已处于灵魂状态，请稍后")
	end
	uniRelic:StartFight(user)
end]]

local s_uniRelic_revive = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	if not uniRelic:IsRoleDead(connection.roleId) then
		return ERROR.TipMsg(connection, "你没有阵亡，无需复活。")
	end
	if user:GetTotalIngot() < REVIVE_NEED_INGOT then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetTotalIngot(-REVIVE_NEED_INGOT)
	uniRelic:SetRoleRevive(connection.roleId)
	connection:SendProtocol("c_uniRelic_revive", "true")
end

local s_uniRelic_embrave = function(connection, data)
	local embraveInfo = CLSUNIRELIC.GetEmbraveInfoByType(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的鼓舞类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end
	local state = uniRelic:GetState()
	if state ~= CLSUNION.ACT_BEGIN and state ~= CLSUNION.ACT_READY then
		return ERROR.TipMsg(connection, "战斗尚未开始，不能进行战斗鼓舞。")
	end
	if data == "ingot" then
		if user:GetTotalIngot() < embraveInfo.cost then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
		end
		user:SetTotalIngot(-embraveInfo.cost)
	else
		if union:GetDevote(connection.roleId, "enable") < embraveInfo.cost then
			return ERROR.TipMsg(connection, "您的贡献不足，不能进行鼓舞。")
		end
	end
	local result = 0
	if math.random() >= embraveInfo.rate then
		uniRelic:SetRoleEmbrave(connection.roleId, data, embraveInfo.add)
		result = 1
	end
	connection:SendProtocol("c_uniRelic_embrave", {type = data, result = result})
end

local s_uniRelic_rank = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟等级不足，遗迹尚未开放。")
	end	
	local rank = uniRelic:GetRank()
	if not rank then
		return ERROR.TipMsg(connection, "活动尚未开始。")
	end
	local sData = uniRelic:GetTopRank(connection.roleId)
	connection:SendProtocol("c_uniRelic_rank", sData)
end

local s_uniRelic_begin = function (connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = UNION.GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local uniRelic = union:GetUnionAct("uniRelic")
	if not uniRelic then
		return ERROR.TipMsg(connection, "联盟遗迹功能尚未开放。")
	end
	local roleInfo = union:GetMemberInfo(connection.roleId)
	if roleInfo.identity >= CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主才能使用该功能。")
	end
	local state = uniRelic:GetState()
	if state == CLSUNION.ACT_BEGIN then
		return ERROR.TipMsg(connection, "活动已经开始。")
	elseif state == CLSUNION.ACT_OVER then
		return ERROR.TipMsg(connection, "活动已经结束。")
	end
	uniRelic:BossActBegin()
	connection:SendProtocol("c_uniRelic_begin", "true")
end

function __init__()

	local tb = Import("autocode/TotetsuReward.lua")
	RewardList = tb.GetTable()

	PROTOCOL.s_uniRelic_enter = s_uniRelic_enter
	PROTOCOL.s_uniRelic_leave = s_uniRelic_leave
	PROTOCOL.s_uniRelic_info = s_uniRelic_info
	PROTOCOL.s_uniRelic_bossInfo = s_uniRelic_bossInfo
	
	PROTOCOL.s_uniRelic_feed = s_uniRelic_feed
	PROTOCOL.s_uniRelic_release = s_uniRelic_release
	PROTOCOL.s_uniRelic_fight = s_uniRelic_fight
	PROTOCOL.s_uniRelic_revive = s_uniRelic_revive
	PROTOCOL.s_uniRelic_embrave = s_uniRelic_embrave
	PROTOCOL.s_uniRelic_autoFight = s_uniRelic_autoFight
	PROTOCOL.s_uniRelic_rank = s_uniRelic_rank
	PROTOCOL.s_uniRelic_begin = s_uniRelic_begin
end
