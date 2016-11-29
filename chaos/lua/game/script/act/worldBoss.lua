--世界boss战

local WorldBossActList = {}

local FuncIdTb = {
	["501001"] = 41,
	["501002"] = 42,
	["501003"] = 43,
}

function GetFuncIdById(id)
	return FuncIdTb[id]
end

function GetWorldBossActList()
	return WorldBossActList
end

function GetWorldBossActById(id)
	return WorldBossActList[id]
end

function SetWorldBossActById(id, actObj)
	WorldBossActList[id] = actObj
end

function GetCurActiveAct()
	for k, v in pairs(GetWorldBossActList()) do
		return v
	end
end

function NewAct(id)
	local actObj = CLSWORLDBOSS.clsBossWar:New()
	actObj:InitBossWar(id)
	SetWorldBossActById(id, actObj)
	for k, v in pairs(USER.GetAllOnlineUser()) do
		FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_WORLD_BOSS_1, FUNCOPEN.STATE_OK)
	end
end

function ClearAct(id)
	local actObj = GetWorldBossActById(id)
	if actObj then
		actObj:ActOver()
		SetWorldBossActById(id)
	end
end

function GetWorldBossActState(id)
	local actObj = GetWorldBossActById(id)
	if actObj then
		return actObj:GetState()		-- "ready" / "begin" / "over"
	end
	local info = CLSWORLDBOSS.GetWorldBossInfoById(id)
	local hour, min = string.match(info.endTime, "(%d+):(%d+)")
	local tm = os.date("*t")
	tm.hour, tm.min = tonumber(hour), tonumber(min)
	return os.time() >= os.time(tm) and "over" or "close"
end

function FormatWorldBossList()
	local sData = {}
	for k, v in pairs(CLSWORLDBOSS.GetAllWorldBossList()) do
		local buf = {}
		buf.actId = k
		buf.picId = v.headId
		buf.name = v.name
		local info = FIGHT.GetMonsterInfoById(v.npcId)
		buf.bossName = info.name
		buf.Lv = info.Lv
		buf.describe = v.describe
		buf.needLv = v.needLv
		buf.startTime = v.startTime
		buf.endTime = v.endTime
		buf.state = GetWorldBossActState(k)
		table.insert(sData, buf)
	end
	return sData
end

--退出世界boss战场
function QuitWorldBoss(user)
	user:SetFocus()
	user:LeaveScene()
	return SCENE.GetReturnedTownId(user)
end


local s_bossWar_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = FormatWorldBossList()
	connection:SendProtocol("c_bossWar_list", sData)
end

local s_bossWar_enter = function(connection, data)
--[[	local actInfo = CLSWORLDBOSS.GetWorldBossInfoById(data)
	if not actInfo then
		return ERROR.TipMsg(connection, "没有指定的活动类型。")
	end
	local actObj = GetWorldBossActById(data)]]
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local actObj = GetCurActiveAct()
	if not actObj then
		return ERROR.TipMsg(connection, "该活动当前没有开启。")
	elseif actObj:GetState() == "over" then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local actId = actObj:GetId()
	local actInfo = CLSWORLDBOSS.GetWorldBossInfoById(actId)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < actInfo.needLv then
		return ERROR.TipMsg(connection, "您的等级不足，无法参加该活动。")
	end
	local scene = user:GetScene()
	if scene and (scene:GetType() ~= "town" and scene:GetType() ~= "uniRelic" and scene:GetType() ~= "uniSecret") then
		return ERROR.TipMsg(connection, "请先退出副本再参加该活动。")
	end
	local sceneObj = actObj:GetScene()
	actObj:AddRole(connection.roleId)
	user:SetFocus("worldBoss", actId)
	user:SetWorldBossTimes(actId)
	user:SetFuncStatById(GetFuncIdById(actId), 1)
	connection:SendProtocol("c_bossWar_enter", sceneObj:GetId())
end

local s_bossWar_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local sData = actObj:GetBossWarInfo(connection.roleId)
	connection:SendProtocol("c_bossWar_info", sData)
end
--[[
local s_bossWar_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	user:SetFocus()
	connection:SendProtocol("c_bossWar_leave", "true")
--	SCENE.ReturnToTown(user)
end

local s_bossWar_fight = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local state = actObj:GetState()
	if state == "ready" then
		return ERROR.TipMsg(connection, "请先做好战斗准备。")
	elseif state == "over" then
		return ERROR.TipMsg(connection, "活动已经结束。")
	end
	if actObj:IsRoleDead(connection.roleId) then
		return ERROR.TipMsg(connection, "您已经阵亡。")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "您正在进行战斗。")
	end
	actObj:BeginFight(user)
end]]

local s_bossWar_embrave = function(connection, data)
	local buffInfo = CLSWORLDBOSS.GetBufferInfo(data)
	if not buffInfo then
		return ERROR.TipMsg(connection, "没有指定的buffer类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local roleInfo = actObj:GetRoleInfo(connection.roleId)
	if roleInfo.bufferList and (roleInfo.bufferList[data] or 0) >= buffInfo.max then
		return ERROR.TipMsg(connection, string.format("%s已经达到上限", buffInfo.name))
	end
	local moneyInfo = MONEY.GetGoldInfoByType(buffInfo.type)
	if not moneyInfo then
		return ERROR.TipMsg(connection, "无效的需求货币类型。")
	end
	if USERBASE.clsUser[moneyInfo.get](user) < buffInfo.num then
		return ERROR.TipMsg(connection, string.format("您的%s不足。", moneyInfo.name))
	end
	USERBASE.clsUser[moneyInfo.set](user, -buffInfo.num)
	local result = 0
	if math.random() <= buffInfo.rate then
		actObj:SetRoleBuffer(connection.roleId, data)
		result = 1
	end
	connection:SendProtocol("c_bossWar_embrave", {result = result, bufferId = data, bufferRate = roleInfo.bufferList[data] or 0})
end

local s_bossWar_revive = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end	
	if not actObj:IsRoleDead(connection.roleId) then
		return ERROR.TipMsg(connection, "您当前已经复活。")
	end	
	if not VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_WBOSS_REVIVAL_FREE) then
		local cost = CLSWORLDBOSS.GetRevivalCost()
		if user:GetTotalIngot() < cost then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
		end
		user:SetTotalIngot(-cost)
	end
	actObj:SetRoleRevive(connection.roleId)	
	connection:SendProtocol("c_bossWar_revive", "true")
end

local s_bossWar_bossInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local sData = actObj:GetBossInfo()
	connection:SendProtocol("c_bossWar_bossInfo", sData)
end

local s_bossWar_rank = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local sData = actObj:GetTopRank(connection.roleId)
	connection:SendProtocol("c_bossWar_rank", sData)
end

local s_bossWar_autoFight = function(connection, data)
	if data ~= 0 and data ~= 1 then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "worldBoss" then
		return ERROR.TipMsg(connection, "您当前不在世界boss副本中。")
	end
	local actObj = GetWorldBossActById(focus.id)
	if not actObj then
		return ERROR.TipMsg(connection, "该活动已经结束。")
	end
	local info = actObj:GetRoleInfo(connection.roleId)
	info.autoFight = data
	connection:SendProtocol("c_bossWar_autoFight", data)
end

function __init__()

	CALLOUT.Daily(getfenv(2), 10, 30, NewAct, "501001")
	CALLOUT.Daily(getfenv(2), 15, 00, NewAct, "501002")
	CALLOUT.Daily(getfenv(2), 20, 30, NewAct, "501003")

	PROTOCOL.s_bossWar_list = s_bossWar_list
	PROTOCOL.s_bossWar_info = s_bossWar_info
	PROTOCOL.s_bossWar_enter = s_bossWar_enter
	PROTOCOL.s_bossWar_leave = s_bossWar_leave
	
	PROTOCOL.s_bossWar_fight = s_bossWar_fight
	PROTOCOL.s_bossWar_embrave = s_bossWar_embrave
	PROTOCOL.s_bossWar_revive = s_bossWar_revive
	PROTOCOL.s_bossWar_autoFight = s_bossWar_autoFight
	PROTOCOL.s_bossWar_bossInfo = s_bossWar_bossInfo
	PROTOCOL.s_bossWar_rank = s_bossWar_rank
	
end


