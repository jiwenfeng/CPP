--功能模块开启

STATE_OK = 1				--可以领取
STATE_DOING = 2			--正在进行
STATE_UNSTARTED = 3		--未开始
local STATE_OVER = 4			--已结束
STATE_APPEAR = 5			--出现
STATE_INVALID = 0		--消失

local FAMOUS_COPY_TOWM_ID = "697015"

local AllFuncOpenList = {}

function GetFuncOpenList()
	return AllFuncOpenList
end

function GetFuncOpenInfoById(id)
	return AllFuncOpenList[id]
end

function GetFuncOpenCondt(id)
	local info = GetFuncOpenInfoById(id)
	return info.condt
end

local FuncOpenList = {
	[SETTING.FUNC_ENHANCE] = function(user)			--锻造系统
		return true
	end,
	[SETTING.FUNC_FAIRYDEV] = function(user)
		user:InitFairyDev()			--初始化仙器系统
		return true
	end,
	[SETTING.FUNC_TAVERN] = function(user)			--武将招募
		return true
	end,
	[SETTING.FUNC_FORMATION] = function(user)
		user:InitFightForm()		--初始化战斗阵型
		return true
	end,
	[SETTING.FUNC_RANK] = function(user)			--排行榜
		return true
	end,
	[SETTING.FUNC_HEROINFO] = function(user)			--战魂
		return true
	end,
	[SETTING.FUNC_ACHIEVE] = function(user)			--成就
		user:InitAchvList()
		return true
	end,
	[SETTING.FUNC_DAILYMISSION] = function(user)			--日常任务	
		return true
	end,
	[SETTING.FUNC_ARENA] = function(user)			--竞技场
		ARENA.EnterArena(user)
		return true
	end,
	[SETTING.FUNC_UNION] = function(user)			--联盟
		return true	
	end,
	[SETTING.FUNC_BAG] = function(user)			--背包	
		return true	
	end,
	[SETTING.FUNC_MISSION_UI] = function(user)			--任务按钮
		user:InitUserMission()
		return true
	end,
	[SETTING.FUNC_HERO_COPY] = function(user)			--英魂副本
		user:SetTownOpen(FAMOUS_COPY_TOWM_ID)
		return true
	end,
	[SETTING.FUNC_PHYLE_FIGHT] = function(user)			--势力战
		return true
	end,
	[SETTING.FUNC_PET_UI] = function(user)			--本命精灵
		user:AddPet()
		return true
	end,
	[SETTING.FUNC_FRIEND] = function(user)			--好友系统
		user:InitFriendList()
		return true
	end,
	[SETTING.FUNC_PHYLE_CHOOSE] = function(user)			--势力选择
		user:CheckNeedPhyle()
		return true
	end,
	[SETTING.FUNC_TRIBE] = function(user)			--部落
		user:InitTribe()
		return true
	end,
	[SETTING.FUNC_WORLD_BOSS_1] = function(user)			--世界boss
		return true
	end,
	[SETTING.ACT_ONLINE_PRIZE] = function(user)			--在线奖励
		return true	
	end,
	[SETTING.ACT_RECHARGE_PRIZE] = function(user)			--累计充值礼包
		return true	
	end,
	[SETTING.ACT_FIRST_RECHARGE] = function(user)			--首充礼包
		return true	
	end,
	[SETTING.ACT_DAILY_ACT] = function(user)			--每日活动
		return true	
	end,
	[SETTING.ACT_ARENA_PRIZE] = function(user)			--竞技场奖励
		return true	
	end,
	[SETTING.ACT_PHYLE_PRIZE] = function(user)			--建设值奖励
		return true
	end,
	
--	[SETTING.FUNC_ST_SPAR] = function(user)			--晶石矿
--		local tribe = user:GetTribe()
--		if tribe then
---			tribe:AddStruct("spar")
--			return true
--		end
--	end,
--	[SETTING.FUNC_ST_ACADEMY] = function(user)		--学馆
--		local tribe = user:GetTribe()
--		if tribe then
--			tribe:AddStruct("academy")
--			return true
--		end
--	end,
---	[SETTING.FUNC_ST_EMBASSY] = function(user)		--使馆
--		local tribe = user:GetTribe()
--		if tribe then
--			tribe:AddStruct("embassy")
--			return true
--		end
--	end,
--	[SETTING.FUNC_ST_RETREAT] = function(user)		--修仙塔
--		local tribe = user:GetTribe()
--		if tribe then
--			tribe:AddStruct("retreat")
--			return true
--		end
--	end,
--	[SETTING.FUNC_ST_FURNACE] = function(user)		--天工炉
--		local tribe = user:GetTribe()
--		if tribe then
--			tribe:AddStruct("furnace")
--			return true
--		end
--	end,
	[SETTING.FUNC_TREE_ROCK] = function(user)		--摇钱树
		return true
	end,
	[SETTING.FUNC_TEAM_COPY] = function(user)			--组队副本
		return true
	end,
	[SETTING.FUNC_MEDITATE] = function(user)
		user:SetMeditate()
		return true
	end,
	[SETTING.FUNC_CITY_FIGHT] = function(user)		--城池争夺战
		return true
	end,
	[SETTING.FUNC_MON_ALBUM] = function(user)		--炼妖阵
		user:InitMonAlbumList()
		return true
	end,
	[SETTING.FUNC_MISS_KIND] = function(user)		--日行一善
		return true
	end,
	[SETTING.FUNC_TREASURE] = function(user)		--寻宝
		return true
	end,
}



local FuncStateList = {
	[SETTING.ACT_ONLINE_PRIZE] = function(user)			--在线奖励
		local info = user:GetOnlinePrizeInfo()
		if info then
			if info.allOver then
				return
			elseif info.needTime < info.onlineSum then
				return STATE_OK
			end
		end
		return STATE_DOING, info.needTime - info.onlineSum - (os.time() - info.timestamp)
	end,
	[SETTING.ACT_RECHARGE_PRIZE] = function(user)			--累计充值礼包
		local state = STATE_DOING
		for k, v in pairs(RECHARGE.GetRechargePrizeList()) do
			local ret = RECHARGE.GetRechargePrizeState(user:GetRoleId(), "recharge", k)
			if ret == STATE_OK then
				return STATE_OK
			end
		end
		return state
	end,
	[SETTING.ACT_FIRST_RECHARGE] = function(user)			--首充礼包
		return RECHARGE.GetRechargePrizeState(user:GetRoleId(), "first")
	end,
	[SETTING.ACT_DAILY_ACT] = function(user)			--每日活动
		for k in pairs(WELFARE.GetWelfareInfoList()) do
			if WELFARE.GetWelfarePrizeState(user, k) == STATE_OK then
				return STATE_OK
			end
		end
		return STATE_DOING
	end,
	[SETTING.ACT_ARENA_PRIZE] = function(user)			--竞技场奖励
		local arenaInfo = ARENA.GetUserArenaInfo(user:GetRoleId())
		if not arenaInfo or not arenaInfo.prize then
			return
		end
		return STATE_OK
	end,
	[SETTING.ACT_PHYLE_PRIZE] = function(user)			--建设值奖励
		local phyle = user:GetPhyle()
		if not phyle then
			return
		end
		local roleId = user:GetRoleId()
		local phyleObj = PHYLE.GetPhyleByType(phyle)
		if not phyleObj:GetPhylePrize() then
			return
		end
		if phyleObj:IsPhylePrizeReceived(roleId) then
			return
		end
		return STATE_OK
	end,	
	[SETTING.FUNC_WORLD_BOSS_1] = function(user)			--世界boss
		for k, v in pairs(WORLDBOSS.GetWorldBossActList()) do
			if v:GetState() ~= "over" then
				return STATE_OK
			end
		end
	end,
	[SETTING.FUNC_PHYLE_FIGHT] = function(user)			--势力战
		local state = NODE.GetActState()
		if state == "over" then
			return
		end
		return STATE_DOING
	end,
	[SETTING.FUNC_TREE_ROCK] = function(user)			--摇钱树
		local maxTimes = user:GetRockTreeMaxTimes()
		local curTimes = user:GetRockTreeTimes()
		if curTimes >= maxTimes then
			return
		end
		return STATE_DOING
	end,
	[SETTING.FUNC_CITY_FIGHT] = function(user)			--城池争夺战
		local state = CITYFIGHT.GetCityAttackState()
		if state == "close" then
			return
		end
		return STATE_DOING
	end,
	[SETTING.FUNC_MISS_KIND] = function(user)			--日行一善
		local act = MISSKINDACT.GetMissKindAct()
		if not act then
			return
		end
		return STATE_DOING
	end,
}

function GetFuncState(user, funcId)
	if not user:IsFuncOpen(funcId) then
		return
	end
	local func = FuncStateList[funcId]
	if not func then
		return STATE_DOING
	end
	return func(user)
end



function SetFuncOpen(user, funcId)
	local func = FuncOpenList[funcId]
	if func then
		local ret = FuncOpenList[funcId](user)
		if ret then
			user:SetFuncOpen(funcId)
			if user:GetLv() > 1 then
				local state = GetFuncState(user, funcId)
				if state then
					SendFuncStateUpdate(user, funcId, state)
				end
				user:FreshSimpleStatList(funcId)
			end
		end
	end
end



function SendFuncStateUpdate(user, funcId, state, timelong)
	local info = GetFuncOpenInfoById(funcId)
	if info.type == "struct" then
		local focus = user:GetFocus()
		if focus and focus.type == "tribe" then
			user:SendProtocol("c_actPic_update", {funcId = funcId, type = info.type, state = state})
		else
			user:SetStructNeedNotice(funcId, info.type, state)
		end
	elseif info.type then
		user:SendProtocol("c_actPic_update", {funcId = funcId, type = info.type, state = state, timelong = timelong})
--	else
--		user:SendProtocol("c_actPic_update", {funcId = funcId, type = info.type, state = state})
	end
end

--检查游戏功能开启
function CheckFuncOpen(user)
	for k, v in pairs(GetFuncOpenList()) do
		if not user:IsFuncOpen(k) then
			local flag = true
			if user:GetLv() < v.condt.needLv then
				flag = false
			end
			if v.condt.needMissionId and not user:IsMissionOver(v.condt.needMissionId) then
				flag = false
			end
			if flag then
				SetFuncOpen(user, k)
			end
		end
	end
end

local funcPrizeList = {
	[SETTING.ACT_ONLINE_PRIZE] = function(user)			--在线奖励
		return ONLINEPRIZE.ReceiveOnlinePrize(user)
	end,
	[SETTING.ACT_RECHARGE_PRIZE] = function(user)			--累计充值礼包
		local id
		for k, v in pairs(RECHARGE.GetRechargePrizeList()) do
			local ret = RECHARGE.GetRechargePrizeState(user:GetRoleId(), "recharge", k)
			if ret == STATE_OK then
				id = k
				break
			end
		end
		if not id then
			return false, "没有可以领取的累计充值奖励。"
		end	
		return RECHARGE.ReceivePrize(user, "recharge", id)
	end,
	[SETTING.ACT_FIRST_RECHARGE] = function(user)			--首充礼包
		return RECHARGE.ReceivePrize(user, "first")
	end,
	[SETTING.ACT_ARENA_PRIZE] = function(user)			--竞技场奖励
		return ARENA.ReceiveReward(user)
	end,
	[SETTING.ACT_PHYLE_PRIZE] = function(user)			--建设值奖励
		return PHYLE.ReceivePhylePrize(user)
	end,
}

function ReceiveFuncPrize(user, funcId)
	if not funcPrizeList[funcId] then
		return false, "没有可领取的奖励。"
	end
	return funcPrizeList[funcId](user)
end




function GetFucListByType(user, type)
	local sData = {}
	for k, v in pairs(GetFuncOpenList()) do
		if v.type == type then
			if user:IsFuncOpen(k) then
				local state, timelong = GetFuncState(user, k)
				if state then
					local buff = {}
					buff.funcId = k
					buff.state, buff.timelong = state, timelong
					table.insert(sData, buff)
				end
			end
		end
	end
	table.sort(sData, function(a, b) 
			local info_a = GetFuncOpenInfoById(a.funcId)
			local info_b = GetFuncOpenInfoById(b.funcId)
			return info_a.order < info_b.order
		end)
	return sData
end


local s_guideLine_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFucListByType(user, "guideLine")
	connection:SendProtocol("c_guideLine_list", sData)
end

local s_actPic_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFucListByType(user, "act")
	connection:SendProtocol("c_actPic_list", sData)
end

local s_funcPrize_receive = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = ReceiveFuncPrize(user, data)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_funcPrize_receive", data)
end


function __init__()
	
	local FUNCLIST = Import("autocode/funcOpenList.lua")
	AllFuncOpenList = FUNCLIST.GetTable()
	
	PROTOCOL.s_guideLine_list = s_guideLine_list
	PROTOCOL.s_actPic_list = s_actPic_list
	
	PROTOCOL.s_funcPrize_receive = s_funcPrize_receive
end


