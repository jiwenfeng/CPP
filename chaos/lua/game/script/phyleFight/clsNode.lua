--据点

MAX_NODE_HP = 1500			--节点最大耐久
local NODE_REOPEN_HP = 1000		--节点重开后的耐久
local NODE_READY_TIMELONG = 5 * 60	--活动开启后节点的准备时间
local MATCH_NUM_PER_ROUND = 5	--每回合匹配数量
local MATCH_ROUND_TIME = 20		--匹配回合时间长度
local FIGHT_MATCH_TIMELONG = 2		--战斗匹配时间
local WIN_SCORE = 2				--获胜积分
local LOSE_SCORE = 1			--战败积分
local FIGHT_BYE_SCORE = 1		--轮空积分

local MIN_DURAT_WIN = 2			--最小连胜次数
ACT_READY_HOUR = 20		--活动结束时
ACT_READY_MIN = 0		--活动结束分
ACT_END_HOUR = 22		--活动结束时
ACT_END_MIN = 0		--活动结束分

READY = 0			--战斗未开启
OPEN = 1			--战斗开启
OVER = 2
local IDLE = 10
local IN_FIGHT = 11
local WAIT = 12

local AllNodeInfoList = {}			--节点列表

function GetAllNodeInfoList()
	return AllNodeInfoList
end

function GetNodeInfoById(id)
	return AllNodeInfoList[id]
end
--根据人数决定是否开放据点
function GetLevelByNum(num)
	if num <= 800 then
		return 1
	elseif num <= 1100 then
		return 2
	end
	return 3
end
----------------------------------------------------------

local AllFightRoleList = {}			--参战玩家列表
local PhyleSumScoreList = {}		--势力总积分表

--玩家本次势力战信息
function GetRoleInfoById(roleId)
	return AllFightRoleList[roleId]
end

function GetAllRoleInfoList()
	return AllFightRoleList
end

function SetRoleInfoById(roleId, data)		--data = {score = , topDurat = }
	AllFightRoleList[roleId] = data
end

function ModeRoleInfoById(roleId, type, data)
	local info = GetRoleInfoById(roleId)
	if info then
		info[type] = data
	end
end

function ModRoleInfoById(roleId, key, val)
	local info = GetRoleInfoById(roleId)
	if not info then return end	
	if key == "score" then
		info[key] = (info[key] or 0) + val
		local rank = NODE.GetScoreRank()
		rank:AddToRank({roleId = roleId, score = info.score})
	else
		info[key] = val
	end
end

function ClearAllRoleInfo()
	AllFightRoleList = {}
end

--势力总积分
function GetPhyleSumScore(phyle)
	return PhyleSumScoreList[phyle] or 0
end

function SetPhyleSumScore(phyle, score)
	PhyleSumScoreList[phyle] = (PhyleSumScoreList[phyle] or 0) + score
end

function ClearAllPhyleSumScore()
	PhyleSumScoreList = {}
end

function CountFighterNumByPhyle()
	local list = {}
	for k, v in pairs(GetAllRoleInfoList()) do
		local phyle = USER.GetUserNickInfo(k, "phyle")
		list[phyle] = (list[phyle] or 0) + 1
	end
	return list
end
--------------------------------------------------------
clsNode = clsObject:Inherit()

function clsNode:__init__()
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsNode:initsavedata()
end

function clsNode:inittempdata()
	self.__temp.memberList = {}
	self.__temp.phyleList = {}
	self.__temp.topwin = {}
	
	self.__temp.callout = {}
end


function clsNode:InitNode(id, phyle, timelong, flag)
	self:SetId(id)
	self:SetPhyle(phyle)
	if not flag then
		self:SetHpMax(MAX_NODE_HP)
		self:ResetHp(MAX_NODE_HP)
	else
		self:SetHpMax(NODE_REOPEN_HP)
		self:ResetHp(NODE_REOPEN_HP)
	end
	self:SetState(READY)
	timelong = timelong or NODE_READY_TIMELONG
	self:SetTimeStamp(os.time() + timelong)
	self:InitPhyleList()
	self:SendNodeUpdate("act")
end


function clsNode:SetId(data)
	self.__temp.id = data
end

function clsNode:GetId()
	return self.__temp.id
end

function clsNode:GetName()
	local info = GetNodeInfoById(self:GetId())
	return info.name
end

function clsNode:SetPhyle(data)
	self.__temp.phyle = data
end

function clsNode:GetPhyle()
	return self.__temp.phyle
end

function clsNode:SetState(data)
	self.__temp.state = data
end

function clsNode:GetState()
	return self.__temp.state
end

function clsNode:SetHpMax(data)
	self.__temp.hpMax = data
end

function clsNode:GetHpMax()
	return self.__temp.hpMax
end

function clsNode:ResetHp(data)
	self.__temp.hp = data
	if self.__temp.hp < 0 then
		self.__temp.hp = 0
	end
end

function clsNode:SetHp(data)
	self.__temp.hp = (self.__temp.hp or 0) + data
	if self.__temp.hp < 0 then
		self.__temp.hp = 0
	end
end

function clsNode:GetHp()
	return self.__temp.hp
end

--最高连胜
function clsNode:SetTopWin(roleId, times)
	self.__temp.topwin = {roleId = roleId, times = times}
end

function clsNode:GetTopWin()
	return self.__temp.topwin
end

function clsNode:GetPhyleList()
	return self.__temp.phyleList
end

function clsNode:InitPhyleList()
	self.__temp.phyleList = {}
	for k in pairs(PHYLE.GetAllPhyleList()) do
		self.__temp.phyleList[k] = {num = 0, score = 0}
	end
	self:InitMemberList()
end

--势力人数
function clsNode:SetPhyleMemNum(type, data)
	self.__temp.phyleList[type] = self.__temp.phyleList[type] or {}
	self.__temp.phyleList[type].num = (self.__temp.phyleList[type].num or 0) + data
end

function clsNode:GetPhyleMemNum(type)
	self.__temp.phyleList[type] = self.__temp.phyleList[type] or {}
	return self.__temp.phyleList[type].num or 0
end

function clsNode:SetPhyleScore(type, score)
	self.__temp.phyleList[type] = self.__temp.phyleList[type] or {}
	self.__temp.phyleList[type].score = (self.__temp.phyleList[type].score or 0) + score
	SetPhyleSumScore(type, score)
end

function clsNode:GetPhyleScore(type)
	self.__temp.phyleList[type] = self.__temp.phyleList[type] or {}
	return self.__temp.phyleList[type].score
end

--参战成员
function clsNode:GetMemberList(type)
	self.__temp.memberList[type] = self.__temp.memberList[type] or {}
	return self.__temp.memberList[type]
end

function clsNode:InitMemberList()
	self.__temp.memberList = {}
	self.__temp.memberList["attack"] = {}
	self.__temp.memberList["defend"] = {}
end

function clsNode:GetMemberNum(type)
	self.__temp.memberList[type] = self.__temp.memberList[type] or {}
	return table.size(self.__temp.memberList[type])
end

function clsNode:GetMemberType(roleId)
	local phyle = USER.GetUserNickInfo(roleId, "phyle")
	return phyle == self:GetPhyle() and "defend" or "attack"
end

function clsNode:GetMemberInfo(roleId)
	local type = self:GetMemberType(roleId)
	self.__temp.memberList[type] = self.__temp.memberList[type] or {}
	return self.__temp.memberList[type][roleId]
end

--state IDLE 未战斗  IN_FIGHT 正在战斗 WAIT 已战斗
function clsNode:AddMember(roleId)
	local type = self:GetMemberType(roleId)
	self.__temp.memberList[type] = self.__temp.memberList[type] or {}
	local buff = {state = WAIT, win = 0, lose = 0, duratWin = 0, gold = 0}
	self.__temp.memberList[type][roleId] = buff
	local phyle = USER.GetUserNickInfo(roleId, "phyle")
	self:SetPhyleMemNum(phyle, 1)
	if not GetRoleInfoById(roleId) then
		SetRoleInfoById(roleId, {score = 0, topDurat = 0, nodeId = self:GetId()})
	else
		ModeRoleInfoById(roleId, "nodeId", self:GetId())
	end
	self:SendNodeUpdate("new", type, roleId)
	local user = assert(USER.GetOnlineUser(roleId))
	user:SetFuncStatById(SETTING.FUNC_PHYLE_FIGHT, 1)
end

function clsNode:RmMember(roleId)
	local type = self:GetMemberType(roleId)
	if not self.__temp.memberList[type][roleId] then
		return
	end
	self.__temp.memberList[type] = self.__temp.memberList[type] or {}
	self.__temp.memberList[type][roleId] = nil
	ModeRoleInfoById(roleId, "nodeId")
	local phyle = USER.GetUserNickInfo(roleId, "phyle")
	self:SetPhyleMemNum(phyle, -1)
	self:SendNodeUpdate("del", type, roleId)
end

function clsNode:RmAllMember(type)
	for k, v in pairs(self:GetMemberList(type)) do
		self.__temp.memberList[type][k] = nil
		ModeRoleInfoById(k, "nodeId")
		local phyle = USER.GetUserNickInfo(k, "phyle")
		self:SetPhyleMemNum(phyle, -1)
	end
end

function clsNode:SetMemberState(roleId, state)
	local info = self:GetMemberInfo(roleId)
	if not info then return end
	info.state = state
end

function clsNode:GetMemberState(roleId)
	local info = self:GetMemberInfo(roleId)
	if not info then return end
	return info.state
end

function clsNode:RecordMemberResult(roleId, result)
	local info = self:GetMemberInfo(roleId)
	local roleInfo = GetRoleInfoById(roleId)
	if not info then return end
	local score = 0
	if result == "win" then
		info.win = info.win + 1
		info.duratWin = info.duratWin + 1			--当前连胜
		score = WIN_SCORE * info.duratWin
		if info.duratWin > roleInfo.topDurat then	--最高连胜
			roleInfo.topDurat = info.duratWin
		end
		local topInfo = self:GetTopWin()
		if info.duratWin > (topInfo.times or MIN_DURAT_WIN) then
			self:SetTopWin(roleId, info.duratWin)
			self:SendNodeUpdate("topwin")
		end
	elseif result == "lose" then
		info.lose = info.lose + 1
		info.duratWin = 0
	elseif result == "bye" then
		score = WIN_SCORE
	end
	ModRoleInfoById(roleId, "score", score)
	local phyle = USER.GetUserNickInfo(roleId, "phyle")
	self:SetPhyleScore(phyle, score)
end

function clsNode:SetWinPhyle(phyle)
	self.__temp.winner = phyle
end

function clsNode:GetWinPhyle()
	return self.__temp.winner
end

--重新初始化
function clsNode:ReInitNode(phyle)
	local id = self:GetId()
	self:initsavedata()
	self:inittempdata()
	self:InitNode(id, phyle, nil, true)
	self:StartActOpenTimer()
	self:SetTimeStamp(os.time() + NODE_READY_TIMELONG)
	self:SendNodeUpdate("act")
	CHAT.SendSysNotice(string.format("城头铁鼓声犹震，匣里金刀血未干!【%s】势力的据点【%s】已重新卷入腥风血雨的战斗中。", PHYLE.GetPhyleName(self:GetPhyle()), self:GetName()))
	CHAT.SendSysMsg("phyle", string.format("【%s】势力占领的新据点【%s】已经重新燃起烽烟，请各路英雄速速赶往支援。", PHYLE.GetPhyleName(self:GetPhyle()), self:GetName()), nil, {id = self:GetId(), kind = "phyleFight"})
end

function clsNode:ProOccupy()
	local phyle = self:GetWinPhyle()
	local endTime = NODE.GetActTimeStamp()
	
	self:SetState(READY)
	if endTime - os.time() > NODE_READY_TIMELONG * 1.5 then
--		CALLOUT.CallOut(clsNode.ReInitNode, 30, self, phyle)
		self:ReInitNode(phyle)
	end
end


function clsNode:ResetAllMemberSate()
	local function reset_state(type)
		for k, v in pairs(self:GetMemberList(type)) do
			v.state = IDLE
		end
	end
	reset_state("attack")
	reset_state("defend")
end

function clsNode:GetIdleMember(type)
	local list = {}
	for k, v in pairs(self:GetMemberList(type)) do
		if v.state == IDLE then
			table.insert(list, k)
		end
	end
	return list
end

function clsNode:SendFightNotice(type, winner, loser, score, duratWin, featGot, gold)
	local msg = {type = type, winner = USER.GetUserNickById(winner), loser = USER.GetUserNickById(loser), score = score, duratWin = duratWin, featGot = featGot, gold = gold}
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_phyleFight_notice", msg)
		end
	end
end

function clsNode:ProFightResult(winner, loser)
	local loseInfo = self:GetMemberInfo(loser)
	if loseInfo.duratWin >= MIN_DURAT_WIN then
		self:SendFightNotice("terminate", winner, loser, loseInfo.duratWin, loseInfo.duratWin)
	end
	self:RecordMemberResult(loser, "lose")
	self:RecordMemberResult(winner, "win")
	
	local winLv = USER.GetUserNickInfo(winner, "Lv")
	local loseLv = USER.GetUserNickInfo(loser, "Lv")
	
	local function send_prize(roleId, result)
		local gold = (winLv + loseLv) / 2 * (result == 1 and 200 or 100)
		local featGot = result == 1 and 2 or 1
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetGold(gold)
			user:SetPhyleFeatGot(featGot)
		else
			OFFLINE.SetOfflineData(roleId, "gold", gold)
			OFFLINE.SetOfflineData(roleId, "phyleFeat", featGot)
		end
		if result == 1 then			
			local winInfo = self:GetMemberInfo(winner)
			self:SendFightNotice("defeat", winner, loser, winInfo.duratWin * WIN_SCORE, winInfo.duratWin, featGot, gold)
		end
	end
	send_prize(winner, 1)
	send_prize(loser, 0)
end

function clsNode:ProMatchFight(atkId, defId)
	self:SetMemberState(atkId, IN_FIGHT)
	self:SetMemberState(defId, IN_FIGHT)
	local function callback(fightId, winner)
		self:SetMemberState(atkId, WAIT)
		self:SetMemberState(defId, WAIT)
		if winner == "attack" then
			self:ProFightResult(atkId, defId)
			self:SetHp(-1)
		else
			self:ProFightResult(defId, atkId)
		end
		self:SendMyInfoUpdate(atkId)
		self:SendMyInfoUpdate(defId)
	end
	local param = {fastFight = true}
	FIGHT.BeginFight("phyleFight", atkId, defId, {}, callback, param)
end

--处理轮空
function clsNode:ProMatchBye(roleId)
	self:SetMemberState(roleId, WAIT)
	self:RecordMemberResult(roleId, "bye")
	if self:GetMemberType(roleId) == "attack" then
		self:SetHp(-1)
	end
	self:SendFightNotice("bye", roleId, nil, FIGHT_BYE_SCORE, nil)
	self:SendMyInfoUpdate(roleId)
end

--匹配处理函数
function clsNode:BeginMatch(times)
	if self:GetState() ~= OPEN then
		return
	end
	times = times + 1
	self:StartMatchTimer(times)
	local atkList = self:GetIdleMember("attack")
	local defList = self:GetIdleMember("defend")
	for i = 1, MATCH_NUM_PER_ROUND do
		local atkId, defId
		if #atkList <= 0 then
			atkId = nil
		else
			local index = math.random(1, #atkList)
			atkId = atkList[index]
			table.remove(atkList, index)
		end
		if #defList <= 0 then
			defId = nil
		else
			local index = math.random(1, #defList)
			defId = defList[index]
			table.remove(defList, index)
		end
		if not atkId and not defId then
			break
		elseif not atkId or not defId then
			self:ProMatchBye(atkId or defId)
		else
			self:ProMatchFight(atkId, defId)
		end
		if self:GetHp() <= 0 then
			return self:FightStateClose(true)
		end
	end
	self:SendNodeUpdate("hp")
	self:SendNodeUpdate("score", "attack")
	self:SendNodeUpdate("score", "defend")
	
	if (times % (MATCH_ROUND_TIME / FIGHT_MATCH_TIMELONG)) == 0 then
		self:ResetAllMemberSate()
		
		--刷新成员列表状态
--[[		for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_phyleFight_member", self:GetFormatMemberList("attack"))
				con:SendProtocol("c_phyleFight_member", self:GetFormatMemberList("defend"))
			end
		end]]
	end
end


--据点战斗状态开启
function clsNode:FightStateOpen()
	local endTime = NODE.GetActTimeStamp()
	self:SetState(OPEN)
	self:RemoveActTimer()
	self:StartMatchTimer()
	self:SetTimeStamp(endTime)
	self:SendNodeUpdate("act")
end

--据点战斗状态关闭
function clsNode:FightStateClose(flag)
	self:SetState(OVER)
	self:RemoveMatchTimer()
	self:RemoveActTimer()
	self:CalWinPhyle()
	self:SetPhyle(self:GetWinPhyle())
--	local oldPhyle = self:GetOldPhyle()
	
	self:ClearAllMember()
	self:SetTimeStamp(os.time())
	self:SendNodeUpdate("act")
	
	if flag then
		self:ProOccupy()
	end	
end

function clsNode:CalWinPhyle()
	local winner = self:GetPhyle()
	if self:GetHp() <= 0 then
		local score = 0
		for k, v in pairs(self.__temp.phyleList) do
			if k ~= self:GetPhyle() and v.score > score then
				winner = k
				score = v.score
			end
		end
	end
	local phyle = self:GetPhyle()
	if winner ~= phyle then
	--	self:SetOldPhyle(phyle)
		self:SetPhyle(winner)
		CHAT.SendSysNotice(string.format("千军万马不知数，但见银山动地来！【%s】势力的据点【%s】竟被【%s】势力攻破占领！", PHYLE.GetPhyleName(phyle), self:GetName(), PHYLE.GetPhyleName(winner)))
	end
	self:SetWinPhyle(winner)
end

function clsNode:ClearAllMember()
	self:RmAllMember("attack")
	self:RmAllMember("defend")
	self:SendMemberListUpdate()
end

function clsNode:SendMemberListUpdate()
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_phyleFight_member", self:GetFormatMemberList("attack"))
			con:SendProtocol("c_phyleFight_member", self:GetFormatMemberList("defend"))
			PROTOCOL.s_phyleFight_myInfo(con, "get")
		end
	end
end

function clsNode:SendMyInfoUpdate(roleId)
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return
	end
	local focus = user:GetFocus()
	if focus.type == "phyleFight" and focus.id == self:GetId() then
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			PROTOCOL.s_phyleFight_myInfo(con, "get")
		end
	end
end


function clsNode:GetOldPhyle()
	return 
end

function clsNode:GetTimeStamp()
	return self.__temp.timestamp
end

function clsNode:SetTimeStamp(time)
	self.__temp.timestamp = time
end

function clsNode:StartActOpenTimer()
	local timelong = self:GetTimeStamp() - os.time()
	self.__temp.actTimer = CALLOUT.ObjCallOut(self.__temp.callout, clsNode.FightStateOpen, timelong, self)
end

function clsNode:RemoveActTimer()
	if self.__temp.actTimer then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.actTimer)
	end
end

function clsNode:StartMatchTimer(times)
	self.__temp.matchTimer = CALLOUT.ObjCallOut(self.__temp.callout, clsNode.BeginMatch, FIGHT_MATCH_TIMELONG, self, times or 1)
end

function clsNode:RemoveMatchTimer()
	if self.__temp.matchTimer then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.matchTimer)
	end
end

local updateList = {
	hp = function(node)
		local sData = {}
		sData.hp = node:GetHp()
		return sData
	end,
	score = function(node, type)
		local sData = {type = type}
		for k, v in pairs(node:GetPhyleList()) do
			if type == "defend" then
				if k == node:GetPhyle() then
					sData[k] = {score = v.score}
				end
			else
				if k ~= node:GetPhyle() then
					sData[k] = {score = v.score}
				end
			end
		end
		return sData
	end,
	new = function(node, type, roleId)
		local sData = {}
		sData.action = "new"
		sData.roleId = roleId
		sData.nick = USER.GetUserNickById(roleId)
		local info = node:GetMemberInfo(roleId)
		sData.state = info.state
		local phyle = USER.GetUserNickInfo(roleId, "phyle")
		sData[phyle] = {num = node:GetPhyleMemNum(phyle)}
		sData.type = type
		return sData
	end,
	del = function(node, type, roleId)
		local sData = {}
		sData.action = "del"
		sData.roleId = roleId
		local phyle = USER.GetUserNickInfo(roleId, "phyle")
		sData[phyle] = {num = node:GetPhyleMemNum(phyle)}
		sData.type = type
		return sData
	end,
	
	--刷新成员列表状态
--[[	state = function(node, nil, roleId)
		local sData = {}
		local info = node:GetMemberInfo(roleId)
		sData.state = info.state
		return sData
	end,]]
	act = function(node)
		local sData = {}
		sData.hp = node:GetHp()
		sData.state = node:GetState()
		sData.timelong = node:GetTimeStamp() - os.time()
		sData.result = node:GetWinPhyle()
		return sData
	end,
	topwin = function(node)
		local sData = {}
		local topwin = node:GetTopWin()
		sData.topwin = {nick = USER.GetUserNickById(topwin.roleId), times = topwin.times}
		return sData
	end,
}
--信息更新
function clsNode:SendNodeUpdate(kind, type, roleId)
	if not updateList[kind] then return end
	local sData = updateList[kind](self, type, roleId)
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_phyleFight_update", sData)
		end
	end
end

--参战成员列表
function clsNode:GetFormatMemberList(type)
	local sData = {nodeId = self:GetId(), type = type, list = {}}
	for k, v in pairs(self:GetPhyleList()) do
		if type == "defend" then
			if k == self:GetPhyle() then
				sData[k] = {num = v.num, score = v.score}
			end
		else
			if k ~= self:GetPhyle() then
				sData[k] = {num = v.num, score = v.score}
			end
		end
	end
	for k, v in pairs(self:GetMemberList(type)) do
		table.insert(sData.list, {roleId = k, nick = USER.GetUserNickById(k), state = v.state})
	end
	return sData
end

--节点战况信息
function clsNode:GetFormatInfo()
	local sData = {}
	sData.nodeId = self:GetId()
	sData.state = self:GetState()
	sData.name = self:GetName()
	sData.hp = self:GetHp()
	if sData.state ~= OVER then
		sData.timelong = self:GetTimeStamp() - os.time()
	end
	local topwin = self:GetTopWin()
	if topwin.roleId then
		sData.topwin = {nick = USER.GetUserNickById(topwin.roleId), times = topwin.times}
	end
	return sData
end

--个人信息
function clsNode:GetFormatRoleInfo(user)
	local roleId = user:GetRoleId()
	local info = self:GetMemberInfo(roleId) or {}
	local roleInfo = GetRoleInfoById(roleId) or {}
	local sData = {}
	sData.topDurat = roleInfo.topDurat or 0
	sData.duratWin = info.duratWin or 0
	sData.win = info.win or 0
	sData.lose = info.lose or 0
	sData.score = roleInfo.score or 0
	return sData
end

function __init__()
	local tb = Import("autocode/nodeList.lua")
	AllNodeInfoList = tb.GetTable()
end
