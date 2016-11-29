local ArenaList = {
	__RestoreHook = true, 
	users = {},
	famous = {archer = {}, taoist = {}, warrior = {}, topwin = {}, winstreak ={}, toprank = {}},
	__state = {}
}
local RankList = {}		--排名榜
local path_arena = "chaos.arena"

local MIN_LV = 15
local COOL_DOWN_COST = 2
local MAX_TIMES_PERDAY = 30		--最多挑战的次数	
local FREE_TIMES_PERDAY = 15		--每天免费的次数
local MAX_RANK = 2025
local ARENA_REWARD_TIMELONG = 3 * 24 * 60 * 60
local COOL_DOWN_TIMELONG = 10 * 60
local CHALLENGE_COST_BASE = 5			
local CHALLENGE_SHOW_NUM = 5		--挑战区域展示数量
local DAILY_WINSTREAK_MIN = 5		--每日连胜最小上榜次数

local FamousRewardList = {
	taoist = 200000,
	warrior = 200000,
	archer = 200000,	
	topwin = 200000,	--最高连胜
	toprank = 300000,	--洪荒霸者
}

function GetMinLv()
	return MIN_LV
end

function GetMaxArenaTimes()
	return MAX_TIMES_PERDAY
end

function InitArenaData()
	ResetArenaUserData()
	local famous = ArenaList.famous
	famous.archer.rank = MAX_RANK
	famous.taoist.rank = MAX_RANK
	famous.warrior.rank = MAX_RANK
	famous.topwin.times = 0				--历史最高连胜
	famous.winstreak.times = 0			--今日最高连胜
	DATA.SaveData(path_arena, ArenaList, "famous")
end

function ResetArenaUserData()
	ArenaList = {
		__RestoreHook = true, 
		users = {},
		famous = {archer = {}, taoist = {}, warrior = {}, topwin = {}, winstreak = {}, toprank = {}},
		__state = {}
	}
	DATA.SaveData(path_arena, ArenaList, "users")
	DATA.SaveData(path_arena, ArenaList, "famous")
	DATA.SaveData(path_arena, ArenaList, "__state")
	print("ArenaData init done!")
end
--------------------------------------------------------------------------------------------------

local clsUser = USERBASE.clsUser

function clsUser:GetArenaTimes()
	self.__save.arena = self.__save.arena or {}
	if CalDiffDays(os.time(), self.__save.arena.timestamp or os.time()) ~= 0 then
		self.__save.arena.times = 0
	end
	return self.__save.arena.times or 0
end

function clsUser:SetArenaTimes(data)
	self.__save.arena = self.__save.arena or {}
	if CalDiffDays(os.time(), self.__save.arena.timestamp or os.time()) ~= 0 then
		self.__save.arena.times = 0
	end
	self.__save.arena.times = math.max((self.__save.arena.times or 0) + data, 0)
	self.__save.arena.timestamp = os.time()
end

--剩余挑战次数
function clsUser:GetArenaLeftTimes()
	return self:GetTotalTimes() - self:GetArenaTimes()
end

function clsUser:GetTotalTimes()
	return self:GetAddTimes() + FREE_TIMES_PERDAY
end

function clsUser:GetAddTimes()
	self.__save.arena = self.__save.arena or {}
	if not self.__save.arena.addtime or CalDiffDays(os.time(), self.__save.arena.addtime) ~= 0 then
		self.__save.arena.add = 0
	end
	return self.__save.arena.add
end

function clsUser:SetAddTimes(data)
	self.__save.arena = self.__save.arena or {}
	if not self.__save.arena.addtime or CalDiffDays(os.time(), self.__save.arena.addtime) ~= 0 then
		self.__save.arena.add = 0
	end
	self.__save.arena.add = math.max(self.__save.arena.add + data, 0)
	self.__save.arena.addtime = os.time()
end

function clsUser:GetCDTimes()
	self.__save.arena = self.__save.arena or {}
	self.__save.arena.cdInfo = self.__save.arena.cdInfo or {}
	if not self.__save.arena.cdInfo.timestamp or CalDiffDays(os.time(), self.__save.arena.timestamp) ~= 0 then
		self.__save.arena.cdInfo.times = 0
	end
	return self.__save.arena.cdInfo.times
end

function clsUser:SetCDTimes(data)
	self.__save.arena = self.__save.arena or {}
	self.__save.arena.cdInfo = self.__save.arena.cdInfo or {}
	if not self.__save.arena.cdInfo.timestamp or CalDiffDays(os.time(), self.__save.arena.timestamp) ~= 0 then
		self.__save.arena.cdInfo.times = 0
	end
	self.__save.arena.cdInfo.times = math.max(self.__save.arena.cdInfo.times + data, 0)
	self.__save.arena.cdInfo.timestamp = os.time()
end


----------------------------------------------------------------------------------
--获取排行榜对应玩家
function GetRoleByRank(rank)
	return RankList[rank]
end
--添加到排行榜
function AddToRankList(roleId)
	local rank = #RankList + 1
	if rank > 2000 then
		rank = MAX_RANK
	end
	table.insert(RankList, rank, roleId)
	return rank
end
--交换玩家排名
function SwitchRoleRank(rank1, rank2)
	RankList[rank1], RankList[rank2] = RankList[rank2], RankList[rank1]
end

function GetSendPrizeTimestamp()
	return ArenaList.__state.endTime or os.time()
end


function CalChallengeOrder(index)
	local list = {}
	local function cal_order(index)
		if #list >= 5 then
			table.sort(list, function(a, b) return a > b end)
			return list
		end
		if index >= 500 then
			index = index - 25
		elseif index > 125 and index < 500 then
			index = index - 5
		else
			index = index - 1
			if index < 1 then
				index = 6 - index
			end
		end
		table.insert(list, index)
		return cal_order(index)
	end
	return cal_order(index)
end

function GetRewardByRank(rank)
	local gold = 0
	if rank == 1 then
		gold = 300000
	elseif rank > 1 and rank <= 100 then
		gold = (200 - rank) * 1000
	elseif rank > 100 and rank <= 200 then
		gold = 50000
	elseif rank > 200 and rank < MAX_RANK then
		gold = 30000
	end
	return gold
end

function GetFamousReward(type)
	return FamousRewardList[type]
end


--自己的排名奖励
function GetMyReward(roleId)
	local info = GetUserArenaInfo(roleId)
	local job = USER.GetUserNickInfo(roleId, "job")
	local basicJob = JOB.GetBasicJob(job)
	local famous = ArenaList.famous
	local gold = 0
	if famous[basicJob] and famous[basicJob].roleId == roleId then
		gold = gold + 200000
	end
	if famous["topwin"] and famous["topwin"].roleId == roleId then
		gold = gold + 200000
	end
	gold = gold + GetRewardByRank(info.rank)
	return gold
end

function InitUserArenaInfo(roleId, rank)
	local buff = {}
	buff.rank = rank
	buff.win = {today = 0, top = 0, now = 0, timestamp = os.time()}
	buff.log = {}
	ArenaList.users[roleId] = buff
end

function GetUserArenaInfo(roleId)
	return ArenaList.users[roleId]
end

function ModUserArenaRank(roleId, rank)
	local info = GetUserArenaInfo(roleId)
	info.rank = rank
end

--获取自己的连胜记录
function GetUserWinInfo(roleId)
	local info = GetUserArenaInfo(roleId)
	if not info then
		return
	end
	if CalDiffDays(os.time(), info.win.timestamp) ~= 0 then
		info.win.now = 0		--当前连胜次数
		info.win.today = 0		--今日最高连胜次数	top 历史最高连胜次数
	end
	return info.win
end

function ModUserWinInfo(roleId, type)
	local info = GetUserWinInfo(roleId)
	if not info then
		return
	end
	if not type then
		info.now = 0		--当前连胜
	else
		info.now = (info.now or 0) + 1
		if info.now > info.today then
			info.today = info.now
		end
		if info.now > info.top then
			info.top = info.now
		end
		AddToFamous(roleId)
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:CheckAchvState("arena")
	end
	info.timestamp = os.time()
end

function ModUserArenaLog(roleId, type, target, result, rank)
	local info = GetUserArenaInfo(roleId)
	local nick = USER.GetUserNickById(target)
	info.log = info.log or {}
	local buff = {type = type, nick = nick, result = result, rank = rank, time = os.time(), rank = rank}
	table.insert(info.log, buff)
	if #info.log > 5 then
		table.remove(info.log, 1)
	end
end

function ClearAllUserLog()
	for k, v in ipairs(RankList) do
		local info = GetUserArenaInfo(v)
		info.log = {}
	end
end

function UpdateFamous()
	if #RankList <= 0 then
		return
	end
	local toprank = ArenaList.famous["toprank"]
	local first = RankList[1]
	if toprank.roleId ~= first then
		if toprank.roleId then
			TITLE.ClearTitleInfo(toprank.roleId, "toprank")
		end
		toprank.roleId = first
		TITLE.SetTitleInfo(first, "toprank", 1)
	end
	local topwin = ArenaList.famous["topwin"]		--名人堂最高连胜
	local inList = {}
	local flag = true
	for rank, roleId in ipairs(RankList) do
		local job = USER.GetUserNickInfo(roleId, "job")
		local basicJob = JOB.GetBasicJob(job)
		if not inList[basicJob] then
			inList[basicJob] = true
			local info = ArenaList.famous[basicJob]
			if info.roleId ~= roleId then
				if info.roleId then
					TITLE.ClearTitleInfo(info.roleId, basicJob)
				end
				info.rank = rank
				info.roleId = roleId
				TITLE.SetTitleInfo(roleId, basicJob, 1)
			end
		end
		local info = GetUserArenaInfo(roleId)
		if info.win.top > (topwin.times or 0) then
			if flag then
				TITLE.ClearTitleInfo(topwin.roleId, "topwin")
			end
			topwin.times = info.win.top
			topwin.roleId = roleId 
			flag = false
		end
	end
	if not flag then
		TITLE.SetTitleInfo(topwin.roleId, "topwin", 1)
	end
	DATA.SaveData(path_arena, ArenaList, "famous")
end

function AddToFamous(roleId)
	local myInfo = GetUserArenaInfo(roleId)
	local famous = ArenaList["famous"]
	local streakInfo = famous.winstreak
	streakInfo.times = streakInfo.times or DAILY_WINSTREAK_MIN
	if streakInfo.times < myInfo.win.today or (myInfo.win.today >= DAILY_WINSTREAK_MIN and CalDiffDays(streakInfo.timestamp or os.time(), os.time()) ~= 0) then
		streakInfo.timestamp = os.time()
		streakInfo.roleId = roleId
		streakInfo.times = myInfo.win.today
		needSave = true
		SendArenaLogUpdate()
	end
	if needSave then
		DATA.SaveData(path_arena, ArenaList, "famous")
	end
end

function SendArenaLogUpdate()
	local sData = GetWinStreak()
	for k in pairs(USER.GetUserFocusListByType("arena")) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_arena_logUpdate", sData)
		end
	end
end

--今日的最高连胜信息
function GetWinStreak()
	local info = ArenaList.famous.winstreak
	local sData = {}
	if info.roleId then
		sData.nick = USER.GetUserNickById(info.roleId)
		sData.times = info.times
	end
	return sData
end

function GetFightPrize(lv, type)
	local itemList = {[102013] = 0.17, [102014] = 0.33, [102015] = 0.5}
	local sData = {}
	sData.gold = lv * 100
	sData.item = {}
	if type == "win" then
		sData.gold = lv * 200
	end
	if math.random() <= 0.2 then
		local rate = math.random()
		local ran = 0
		for k, v in pairs(itemList) do
			rate = ran + v
			if rate >= ran then
				table.insert(sData.item, {goodsId = k, num = 1})
				break
			end
		end
	end
	return sData
end

function CanBeChallenged(roleId, rank)
	local info = GetUserArenaInfo(roleId)
	local list = CalChallengeOrder(info.rank)
	for k, v in ipairs(list) do
		if v == rank then
			return GetRoleByRank(rank)
		end
	end
end

function GetChallengeList(roleId)
	local info = GetUserArenaInfo(roleId)
	local list = CalChallengeOrder(info.rank)
	local sData = {}
	for _, v in ipairs(list) do
		local buf = {}
		buf.rank = v
		local targetId = GetRoleByRank(v)
		if targetId then
			buf.roleId = targetId
			buf.nick = USER.GetUserNickInfo(targetId, "nick")
			buf.lv = USER.GetUserNickInfo(targetId, "Lv")
			buf.headId = USER.GetUserNickInfo(targetId, "headId")
			buf.reward = GetRewardByRank(v)
			buf.job = USER.GetUserNickInfo(targetId, "job")
		end
		table.insert(sData, buf)
	end
	return sData
end

function GetFamousList()
	local sData = {}
	for k, v in pairs(ArenaList.famous) do
		if k ~= "winstreak" then
			local buf = {}
			buf.type = k
			buf.headId = USER.GetUserNickInfo(v.roleId, "headId")
			buf.reward = GetFamousReward(k)
			buf.nick = USER.GetUserNickInfo(v.roleId, "nick")
			buf.rank = v.rank
			buf.times = v.times
			buf.job = USER.GetUserNickInfo(v.roleId, "job")
			table.insert(sData, buf)
		end
	end
	return sData
end

function GetMyArenaInfo(user)
	local roleId = user:GetRoleId()
	local info = GetUserArenaInfo(roleId)
	local now = os.time()
	local sData = {}
	sData.rank = info.rank or 0
	local winInfo = GetUserWinInfo(roleId)
	sData.top = winInfo.top or 0
	sData.today = winInfo.today or 0
	sData.freeTimes = user:GetArenaLeftTimes()
	sData.reward = GetMyReward(roleId)
	local obj = user:GetOneUpgradeList("arena", "1")
	sData.cdTime = math.max(obj:GetEndTimestamp() - now, 0)
	sData.speedupCost = COOL_DOWN_COST
	sData.timestamp = math.max(GetSendPrizeTimestamp() - now, 0)
	sData.totalTimes = user:GetArenaTimes()
	sData.baseFree = FREE_TIMES_PERDAY
	sData.baseCost = CHALLENGE_COST_BASE
	return sData
end

function SendWinStreakMsg(roleId)
	local info = GetUserArenaInfo(roleId)
	local times = info.win.now or 0
	local msg
	if times == 15 then
		msg = string.format("%s势如破竹,", USER.GetUserNickById(roleId))
	elseif times == 30 then
		msg = string.format("%s绝世无双,", USER.GetUserNickById(roleId))
	end
	if msg then
		msg = msg .. string.format("在竞技场连胜%d场", times)
		CHAT.SendSysMsg("world", msg)
	end
end

function ChallengeOver(atkId, defId, type)
	ModUserWinInfo(atkId, type)
	ModUserWinInfo(defId, not type)
	local atkOldRank = GetUserArenaInfo(atkId).rank
	local defOldRank = GetUserArenaInfo(defId).rank
	if type then
		ChangeOrder(atkId, defId)
		local atkNewRank = GetUserArenaInfo(atkId).rank
		local defNewRank = GetUserArenaInfo(defId).rank
		ModUserArenaLog(atkId, "active", defId, "win", atkNewRank - atkOldRank)
		ModUserArenaLog(defId, "passive", atkId, "lose", defNewRank - defOldRank)
		SendWinStreakMsg(atkId)
	else
		ModUserArenaLog(atkId, "active", defId, "lose", 0)
		ModUserArenaLog(defId, "passive", atkId, "win", 0)
	end
end

function ChangeOrder(atkId, defId)
	local atkInfo = GetUserArenaInfo(atkId)
	local defInfo = GetUserArenaInfo(defId)
	local atkRank = atkInfo.rank
	local defRank = defInfo.rank
	if atkRank > defRank then
		SwitchRoleRank(atkRank, defRank)		--交换玩家排名
		ModUserArenaRank(atkId, defRank)
		ModUserArenaRank(defId, atkRank)
	end
end

function EnterArena(user)
	local roleId = user:GetRoleId()
	if user:GetLv() >= MIN_LV and not GetUserArenaInfo(roleId) then
		local rank = AddToRankList(roleId)
		InitUserArenaInfo(roleId, rank)
		local obj = UPGRADELIST.NewList("arena", "1", UPGRADELIST.LIST_UNLOCK, user:GetRoleId())
		user:AddUpgradeList("arena", "1", obj)
	end	
end

function BeginChallenge(user, defendId)
	local roleId = user:GetRoleId()
	local Lv = USER.GetUserNickInfo(defendId, "Lv")
	user:SetFuncStatById(SETTING.FUNC_ARENA, 1)
	local function callback(fightId, winner, prize)
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetWorkInfo("standby")
			user:SendUpgradeListUpdate("arena", 1)
			user:SendProtocol("c_arena_challenge", {result = winner == "attack"})
		else
			OFFLINE.SetOfflineData(roleId, "workInfo", "rest")
		end
		ChallengeOver(roleId, defendId, winner == "attack")
		SendChallengePrize(roleId, prize)
	end
	local param = {Lv = Lv, restoreFlag = true}
	user:SetWorkInfo("fighting", nil)
	local fightId = FIGHT.BeginFight("arena", roleId, defendId, {roleId}, callback, param)
end

function SendChallengePrize(roleId, prize)
	local user = USER.GetOnlineUser(roleId)
	if prize.gold then
		user:SetGold(prize.gold)
	end
	if prize.item then
		for k, v in pairs(prize.item) do
			local item = BASEITEM.NewItem(v.goodsId, v.num, {lock = 1})
			user:AddItem(USERBASE.PACKAGE, item, true)
		end
	end
end

function RestoreHook(ret)
	local state = ArenaList.__state
	if not state.endTime then
		InitArenaData()				--初始化竞技场数据
		local timelong = CALLOUT.GetDiff(0, 0) + ARENA_REWARD_TIMELONG
		ArenaList.__state.endTime = os.time() + timelong
		CALLOUT.CallOut(SendReward, timelong)
		DATA.SaveData(path_arena, ArenaList, "__state")
	elseif state.endTime <= os.time() then
		SendReward()
	else
		CALLOUT.CallOut(SendReward,  state.endTime - os.time())
	end
	for k, v in pairs(ArenaList.users) do
		RankList[v.rank] = k
	end
end

function SaveArenaUserData()
	DATA.SaveData(path_arena, ArenaList, "users")
	DATA.SaveData(path_arena, ArenaList, "famous")
	DATA.SaveData(path_arena, ArenaList, "__state")
end

function SendReward()
	local state = ArenaList.__state
	state.endTime = CALLOUT.GetDiff(0, 0) + ARENA_REWARD_TIMELONG + os.time()
	state._id = "__state"
	for k, v in pairs(ArenaList.users) do
		local reward = GetMyReward(k)
		if reward > 0 then
			v.prize = {}
			v.prize.gold = reward
			local user = USER.GetOnlineUser(k)
			if user then
				FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_ARENA_PRIZE, FUNCOPEN.STATE_OK)
			end
		end
	end
	CALLOUT.CallOut(SendReward, state.endTime - os.time())
	DATA.UpdateData(path_arena, {_id = "__state"}, state)
end

function ReceiveReward(user)
	local info = GetUserArenaInfo(user:GetRoleId())
	if not info or not info.prize then
		return false, "没有可以领取的奖励信息。"
	end
--	user:SetGold(-info.prize.gold)
	user:SetGold(info.prize.gold)
	user:SendProtocol("c_show_tips", string.format("获得了金钱%d", info.prize.gold))
	info.prize = nil
	FUNCOPEN.SendFuncStateUpdate(user, SETTING.ACT_ARENA_PRIZE, FUNCOPEN.STATE_INVALID)
	return true
end


local s_arena_famousList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local sData = GetFamousList()
	connection:SendProtocol("c_arena_famousList", sData)
end

local s_arena_challgList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "等级不足，不能进入竞技场。")
	end
	local sData = GetChallengeList(connection.roleId)
	connection:SendProtocol("c_arena_challgList", sData)
end

local s_arena_myInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "等级不足，不能进入竞技场。")
	end
	USER.SetUserFocusOn("arena", connection.roleId, 1)
	user:SetFocus("arena")
	EnterArena(user)
	local sData = GetMyArenaInfo(user)
	connection:SendProtocol("c_arena_myInfo", sData)
end

local s_arena_challenge = function(connection, data)
	if not USER.GetUserNickById(data) then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "等级不足，不能进入竞技场。")
	end
	local cost = 0
	local obj = user:GetOneUpgradeList("arena", "1")
	if obj:GetEndTimestamp() > os.time() then
		cost = user:GetCDTimes() * COOL_DOWN_COST + COOL_DOWN_COST
	end
	if user:GetArenaLeftTimes() <= 0 then
		return ERROR.TipMsg(connection, "挑战次数已经用完，请购买")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "您正在进行战斗。")
	end
	user:SetTotalIngot(-cost)
	user:SetArenaTimes(1)
	obj:SetEndTimestamp(os.time() + COOL_DOWN_TIMELONG)
	BeginChallenge(user, data)
end

local s_arena_accelerate = function(connection, data)
	if data ~= 0 and data ~= 1 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user:GetOneUpgradeList("arena", "1")
	local cost = user:GetCDTimes() * COOL_DOWN_COST + COOL_DOWN_COST
	if obj:GetEndTimestamp() > os.time() then
		if user:GetTotalIngot() < cost then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
		end
		user:SetTotalIngot(-cost)
	else
		return ERROR.TipMsg(connection, "当前不需要加速")
	end
	if data == 1 then
		user:SetSetting(SETTING.ARENA_ACCELERATE_NO_REMIND, 1)
	end
	obj:ClearCDTime()
	connection:SendProtocol("c_arena_accelerate", "true")
end

local s_arena_log = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "等级不足，不能进入竞技场。")
	end
	local info = GetUserArenaInfo(connection.roleId)
	local sData = {}
	if info then
		sData.winLog = GetWinStreak()
		sData.logList = info.log
		sData.now = os.time()
	end
	connection:SendProtocol("c_arena_log", sData)
end

local s_arena_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn("arena", connection.roleId)
	user:SetFocus()
	connection:SendProtocol("c_arena_leave", "true")
end

local s_arena_addTimes = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connetion, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local times = user:GetAddTimes()
	if times >= 15 then
		return ERROR.TipMsg(connection, "今天新增挑战次数已经达到上限")
	end
	local cost = (15 + times + 1) * 2
	if cost > user:GetTotalIngot() then
		return ERROR.TipMsg(connection, "元宝数量不足")
	end
	user:SetTotalIngot(-cost)
	user:SetAddTimes(1)
	connection:SendProtocol("c_arena_addTimes", user:GetTotalTimes())
end

function __init__()
	CALLOUT.CallFre(SaveArenaUserData, 5 * 60)
	CALLOUT.Daily(getfenv(2), 0, 0, UpdateFamous)
	
	PROTOCOL.s_arena_myInfo = s_arena_myInfo
	PROTOCOL.s_arena_challgList = s_arena_challgList
	PROTOCOL.s_arena_famousList = s_arena_famousList
	PROTOCOL.s_arena_log = s_arena_log
	
	PROTOCOL.s_arena_challenge = s_arena_challenge
	PROTOCOL.s_arena_accelerate = s_arena_accelerate
	PROTOCOL.s_arena_leave = s_arena_leave
	PROTOCOL.s_arena_addTimes = s_arena_addTimes
	DATA.Restore(path_arena, ArenaList)
end

