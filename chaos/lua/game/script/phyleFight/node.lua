local MAX_NODE_MEMBER_NUM = 50			--节点的成员数
local ACT_READY_TIMELONG = 3600		--活动准备时间
local ACT_LAST_TIMELONG = 3600
local ACT_STAT_UPDATE_TIMELONG = 2
local TOP_RANK_NUM = 10
local MIN_JOIN_LV = 30

local WIN_PHYLE_BUFFER_ID = "20"	--最终胜利buffer
local NODE_NO_LOST_PRIZE = 105007
local NODE_OCCUPY_ONE_PHYLE = 105008
local NODE_OCCUPY_ALL_PHYLE = 105009
local RankReward = {[1] = 105010, [2] = 105011, [3] = 105012, [4] = 105013}

function GetRewardByRank(rank)
	if rank > TOP_RANK_NUM then
		return
	elseif rank >= #RankReward then
		return RankReward[#RankReward]
	else
		return RankReward[rank]
	end
end

local PhyleFightResult = {}
local path_phyleFight = "chaos.phyleFight"

function SetPhyleFightResult(phyle, rank)
	PhyleFightResult[phyle] = rank
--	DATA.SaveData(path_phyleFight, PhyleFightResult, phyle)
end

function GetPhyleFightResult(phyle)
	return PhyleFightResult[phyle]
end

function GetPhyleByRank(rank)
	for k, v in pairs(PhyleFightResult) do
		if v == rank then
			return k
		end
	end
end

--开启活动的节点列表
local AllNodeObjList = {__state = {state = "over", timestamp = nil}}

function GetNodeObjList()
	local buff = {}
	for k, v in pairs(AllNodeObjList) do
		if k ~= "__state" and k ~= "__rank" then
			buff[k] = v
		end
	end
	return buff
end

function GetNodeObjById(id)
	return AllNodeObjList[id]
end

function SetNodeObjById(id, obj)
	AllNodeObjList[id] = obj
end
--活动状态
function GetActState()
	local info = AllNodeObjList.__state
	return info.state
end

function GetActTimeStamp()
	local info = AllNodeObjList.__state
	return info.timestamp
end

function SetActState(state, timelong)
	local info = AllNodeObjList.__state
	info.state = state
	if timelong then
		info.timestamp = os.time() + timelong
	else
		info.timestamp = nil
	end
end
--玩家积分排名
function InitScoreRank()
	local rank = RANK.NewRank("real", "phyleFight")
	AllNodeObjList.__rank = rank
end

function GetScoreRank()
	return AllNodeObjList.__rank
end

function ClearScoreRank()
	local rank = AllNodeObjList.__rank
	RANK.DestroyRank(rank)
	AllNodeObjList.__rank = nil
end


--活动进入准备阶段
function ActInReady()
	if GetActState() ~= "over" then
		return
	end
	SetActState("ready", ACT_READY_TIMELONG)
	for k, v in pairs(CLSNODE.GetAllNodeInfoList()) do
		if v.level == 1 then
			local node = CLSNODE.clsNode:New()
			node:InitNode(k, v.phyle, ACT_READY_TIMELONG)
			SetNodeObjById(k, node)
		end
	end
	InitScoreRank()			--初始化玩家积分排名
	CHAT.SendSysNotice("势力战报名开启，保家卫国九死未悔，请各位英雄儿女踊跃参战！")
	StartActOpenCallOut()
	for k, v in pairs(USER.GetAllOnlineUser()) do
		FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_PHYLE_FIGHT, FUNCOPEN.STATE_OK)
	end
end

--活动开启
function ActOpen()
	if GetActState() ~= "ready" then
		return
	end
	SetActState("open", ACT_LAST_TIMELONG)
	for k, v in pairs(GetNodeObjList()) do
		v:FightStateOpen()
	end
	CheckNeedNodeOpen()
	StartActOverCallOut()
	StartActStatCallOut()
	CHAT.SendSysNotice("黄沙百战穿金甲，不破楼兰终不还！势力战已经开始，请各位壮士驰骋疆场奋勇杀敌！")
end

--检测是否需要再开放据点
function CheckNeedNodeOpen()
	local num = table.size(USER.GetAllOnlineUser())
	local level = CLSNODE.GetLevelByNum(num)
	if level == 1 then
		return 
	end
	local msg = ""
	for k, v in pairs(CLSNODE.GetAllNodeInfoList()) do
		if v.level == level then
			local node = CLSNODE.clsNode:New()
			node:InitNode(k, v.phyle)
			SetNodeObjById(k, node)
			node:StartActOpenTimer()
			msg = msg .. string.format("%s【%s】", msg == "" and "" or "、", node:GetName())
		end
	end
	if msg ~= "" then
		CHAT.SendSysNotice(string.format("十里边烽狼烟动，泽国江山入战图！新据点%s已经进入战斗报名阶段。", msg))
	end
end

function CalActPhyleRank()
	local list = {}
	for k in pairs(PHYLE.GetAllPhyleList()) do
		table.insert(list, {phyle = k, score = CLSNODE.GetPhyleSumScore(k) or 0})
	end
	table.sort(list, function(a, b) return a.score > b.score end)
	for i = 1, #list do
		SetPhyleFightResult(list[i].phyle, i)
	end
end

--活动结束
function ActOver()
	if GetActState() ~= "open" then
		return
	end
	SetActState("over")
	for k, v in pairs(GetNodeObjList()) do
		v:FightStateClose()
	end
	
	CalActPhyleRank()
	local winPhyle = GetPhyleByRank(1)
	CHAT.SendSysNotice(string.format("醉卧沙场君莫笑，古来征战几人回！势力战已结束，恭喜【%s】势力赢得最终的胜利！", PHYLE.GetPhyleName(winPhyle)))
	SendActReward()
	for k in pairs(CLSNODE.GetAllRoleInfoList()) do
		local phyle = USER.GetUserNickInfo(k, "phyle")
		local isWin	= phyle == winPhyle
		local user = USER.GetOnlineUser(k)
		if user then
			user:SetPhyleFightStats(isWin)
		else
			OFFLINE.SetOfflineData(k, "phyleFight", isWin)
		end
	end
	local memList = CLSNODE.CountFighterNumByPhyle()
	STATS.SavePhyleFightHistory(string.format("%d:%d", CLSNODE.ACT_READY_HOUR, CLSNODE.ACT_READY_MIN), os.date("%H:%M", os.time()), memList["phoenix"], memList["dragon"], memList["tiger"], winPhyle)
	CLSNODE.ClearAllRoleInfo()		--清理玩家信息
	CLSNODE.ClearAllPhyleSumScore()	--清理势力总积分
	ClearScoreRank()				--清理玩家积分排名
	local memList = {}
	for k, v in pairs(GetNodeObjList()) do
		v:Destroy()
		SetNodeObjById(k)
	end
	for k, v in pairs(USER.GetAllOnlineUser()) do
		FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_PHYLE_FIGHT, FUNCOPEN.STATE_INVALID)
	end
end

function StartActOpenCallOut()
	if GetActState() ~= "ready" then
		return
	end
	local timelong = GetActTimeStamp() - os.time()
	CALLOUT.CallOut(ActOpen, timelong)
end

function StartActOverCallOut()
	if GetActState() ~= "open" then
		return
	end
	local timelong = GetActTimeStamp() - os.time()
	CALLOUT.CallOut(ActOver, timelong)
end

function StartActStatCallOut()
	if GetActState() ~= "open" then
		return
	end
	CALLOUT.CallOut(UpdateActStat, ACT_STAT_UPDATE_TIMELONG)
end

function UpdateActStat()
	for k in pairs(USER.GetUserFocusListByType("nodeList")) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			PROTOCOL.s_phyleFight_stat(con, "get")
			PROTOCOL.s_phyleFight_rank(con, "get")
		end
	end
	StartActStatCallOut()
end

function GetFormatNodeList(roleId)
	local sData = {list = {}}
	local info = CLSNODE.GetRoleInfoById(roleId)
	if info then
		sData.locate = info.nodeId
	end
	for k, v in pairs(CLSNODE.GetAllNodeInfoList()) do
		local buff = {nodeId = k, name = v.name, index = v.index}
		local node = GetNodeObjById(k)
		if node then
			buff.state = node:GetState()
			buff.phyle = node:GetPhyle()
			buff.hp = node:GetHp()
		else
			buff.state = CLSNODE.OVER
			buff.phyle = v.phyle
			buff.hp = CLSNODE.MAX_NODE_HP
		end
		table.insert(sData.list, buff)
	end
	return sData
end

--发送奖励
function SendActReward()
	SendPhyleTopScore()			--奖励积分第一的势力
	SendNodeOccupyPrize()			--据点占领奖励
	SendRankReward()			--积分排行奖励
end

function SendPhyleTopScore()
	local winPhyle = GetPhyleByRank(1)
	if winPhyle then
		for k in pairs(CLSNODE.GetAllRoleInfoList()) do
			if USER.GetUserNickInfo(k, "phyle") == winPhyle then
				local user = USER.GetOnlineUser(k)
				if user then
					BUFFER.AddOneBuffer(user, WIN_PHYLE_BUFFER_ID)
				else
					OFFLINE.SetOfflineData(k, "addBuffer", WIN_PHYLE_BUFFER_ID)
				end
			end
		end
	end
end

--发送据点争夺奖励
function SendNodeOccupyPrize()
	local all = {}
	local taken = {}
	local stay = {}
	for k, v in pairs(CLSNODE.GetAllNodeInfoList()) do
		taken[v.phyle] = taken[v.phyle] or {}		
		local node = GetNodeObjById(k)
		if node then			
			local phyle = node:GetPhyle()
			all[phyle] = true				--最后剩下的势力
			if v.phyle == phyle then
				stay[v.phyle] = true		--是否守住己方据点
			else
				taken[v.phyle][phyle] = true	--是否被敌方占据据点
			end
		end
	end
	if table.size(all) == 1 then			--所有据点被一方占领
		local leftPhyle = next(all)
		local title = "征服者礼包"
		local content = "恭喜您的势力在战斗中完全占领所有敌对势力据点，特赠予【征服者礼包】以表扬您在战斗中的英勇表现。"
		for k in pairs(CLSNODE.GetAllRoleInfoList()) do
			local phyle = USER.GetUserNickInfo(k, "phyle")
			if phyle == leftPhyle then
				USER.AddItemToUser(k, NODE_OCCUPY_ALL_PHYLE, 1, {lock = 1}, title, content)
				local user = USER.GetOnlineUser(k)
				if user then
					user:SendProtocol("c_show_tips", "获得【征服者礼包】× 1")
				end
			end
		end
	else
		for k, v in pairs(taken) do
			if table.size(v) == 1 and not stay[k] then		--失去所有据点并且仅被一方占领
				local takenPhyle = next(v)
				local title = "开拓者礼包"
				local content = string.format("恭喜您的势力在战斗中完全占领了%s势力的据点，特赠予【开拓者礼包】以表扬您在战斗中的英勇表现。", PHYLE.GetPhyleName(k))
				for roleId in pairs(CLSNODE.GetAllRoleInfoList()) do
					local phyle = USER.GetUserNickInfo(roleId, "phyle")
					if phyle == takenPhyle then
						USER.AddItemToUser(roleId, NODE_OCCUPY_ONE_PHYLE, 1, {lock = 1}, title, content)
						local user = USER.GetOnlineUser(roleId)
						if user then
							user:SendProtocol("c_show_tips", "获得【开拓者礼包】× 1")
						end
					end
				end
			end
		end
	end
	for k in pairs(CLSNODE.GetAllRoleInfoList()) do
		local phyle = USER.GetUserNickInfo(k, "phyle")
		if table.size(taken[phyle]) == 0 then		--己方据点没有被占领
			local title = "完璧礼包"
			local content = "恭喜您的势力在战斗中未丢失任何据点，特赠予【完璧礼包】以表扬您在战斗中的英勇表现。"
			USER.AddItemToUser(k, NODE_NO_LOST_PRIZE, 1, {lock = 1}, title, content)
			local user = USER.GetOnlineUser(k)
			if user then
				user:SendProtocol("c_show_tips", "获得【完璧礼包】× 1")
			end
		end
	end
end

function SendRankReward()
	local rank = GetScoreRank()
	if not rank then return end
	local list = rank:GetTopRank(TOP_RANK_NUM)
	for i = 1, #list do
		local reward = GetRewardByRank(i)
		local itemInfo = BASEITEM.GetItemById(reward)
		local title = string.format("%s", itemInfo.name)
		local content = string.format("恭喜您在势力战中积分排行第%d，为本势力做出巨大贡献，特赠予【%s】以表扬您在战斗中的英勇表现。", i, itemInfo.name)
		USER.AddItemToUser(list[i].roleId, reward, 1, {lock = 1}, title, content)
		local user = USER.GetOnlineUser(list[i].roleId)
		if user then
			user:SendProtocol("c_show_tips", string.format("获得了【%s】× 1", itemInfo.name))
		end
	end
end

local s_phyleFight_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn("nodeList", connection.roleId, 1)
	user:SetFocus("phyleFight", "nodeList")
	local sData = GetFormatNodeList(connection.roleId)
	connection:SendProtocol("c_phyleFight_list", sData)
end

local s_phyleFight_stat = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local state = GetActState()
	local sData = {state = state}
	if state ~= "over" then
		sData.timelong = GetActTimeStamp() - os.time()
	end
	for k in pairs(PHYLE.GetAllPhyleList()) do
		sData[k] = CLSNODE.GetPhyleSumScore(k) or 0
	end
	sData.needLv = MIN_JOIN_LV
	connection:SendProtocol("c_phyleFight_stat", sData)
end

local s_phyleFight_rank = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = {}
	local rank = GetScoreRank()
	if rank then
		sData = rank:GetTopRank(TOP_RANK_NUM)
	end
	connection:SendProtocol("c_phyleFight_rank", sData)
end

local s_phyleFight_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type == "phyleFight" then
		USER.SetUserFocusOn(focus.id, connection.roleId)
		user:SetFocus()
	end
	connection:SendProtocol("c_phyleFight_leave", "true")
end

local s_phyleFight_nodeInfo = function(connection, data)
	if not CLSNODE.GetNodeInfoById(data) then
		return ERROR.TipMsg(connection, "没有指定的据点信息。")
	end
	if GetActState() == "over" then
		return ERROR.TipMsg(connection, "活动尚未开启。")
	end
	local node = GetNodeObjById(data)
	if not node then
		return ERROR.TipMsg(connection, "该据点尚未开启。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能参加势力战。")
	end
	USER.SetUserFocusOn("nodeList", connection.roleId)
	USER.SetUserFocusOn(data, connection.roleId, 1)
	user:SetFocus("phyleFight", data)
	local sData = node:GetFormatInfo()
	connection:SendProtocol("c_phyleFight_nodeInfo", sData)
end

local s_phyleFight_member = function(connection, data)
	if data ~= "attack" and data ~= "defend" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if GetActState() == "over" then
		return ERROR.TipMsg(connection, "活动尚未开启。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "phyleFight" then
		return ERROR.TipMsg(connection, "您没有参加势力战。")
	end
	if not CLSNODE.GetNodeInfoById(focus.id) then
		return ERROR.TipMsg(connection, "您尚未进入据点。")
	end
	local node = GetNodeObjById(focus.id)
	if not node then
		return ERROR.TipMsg(connection, "该据点尚未开启。")
	end
	local sData = node:GetFormatMemberList(data)
	connection:SendProtocol("c_phyleFight_member", sData)
end

local s_phyleFight_myInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
--[[	if GetActState() == "over" then
		return ERROR.TipMsg(connection, "活动尚未开启。")
	end]]
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "phyleFight" then
		return ERROR.TipMsg(connection, "您没有参加势力战。")
	end
	if not CLSNODE.GetNodeInfoById(focus.id) then
		return ERROR.TipMsg(connection, "您尚未进入据点。")
	end
	local node = GetNodeObjById(focus.id)
	if not node then
		return ERROR.TipMsg(connection, "该据点尚未开启。")
	end
	local sData = node:GetFormatRoleInfo(user)
	local info = CLSNODE.GetRoleInfoById(connection.roleId)
	if not info or not info.nodeId then
		sData.state = 0
	elseif info.nodeId ~= focus.id then
		sData.state = 0
		local nodeObj = GetNodeObjById(info.nodeId)
		sData.joined = nodeObj:GetName()
	else
		sData.state = 1
	end
	connection:SendProtocol("c_phyleFight_myInfo", sData)
end

local s_phyleFight_join = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if GetActState() == "over" then
		return ERROR.TipMsg(connection, "活动尚未开启。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "phyleFight" then
		return ERROR.TipMsg(connection, "您没有参加势力战。")
	end
	if not CLSNODE.GetNodeInfoById(focus.id) then
		return ERROR.TipMsg(connection, "您尚未进入据点。")
	end
	local node = GetNodeObjById(focus.id)
	if not node then
		return ERROR.TipMsg(connection, "该据点尚未开启。")
	end
	local type = node:GetMemberType(connection.roleId)
	if node:GetMemberNum(type) >= MAX_NODE_MEMBER_NUM then
		return ERROR.TipMsg(connection, "该据点参战人数已满。")
	end
	local info = CLSNODE.GetRoleInfoById(connection.roleId)
	if info and info.nodeId then
		local nodeObj = GetNodeObjById(info.nodeId)
		nodeObj:RmMember(connection.roleId)
	end
	node:AddMember(connection.roleId)
	connection:SendProtocol("c_phyleFight_join", "true")
end

local s_phyleFight_cancel = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "phyleFight" then
		return ERROR.TipMsg(connection, "您没有参加势力战。")
	end
	if not CLSNODE.GetNodeInfoById(focus.id) then
		return ERROR.TipMsg(connection, "您尚未进入据点。")
	end
	local node = GetNodeObjById(focus.id)
	if not node then
		return ERROR.TipMsg(connection, "该据点尚未开启。")
	end
	if not node:GetMemberInfo(connection.roleId) then
		return ERROR.TipMsg(connection, "您没有在此据点参战。")
	end
	node:RmMember(connection.roleId)
	connection:SendProtocol("c_phyleFight_cancel", "true")
end


function __init__()
	CALLOUT.Weekly(getfenv(2), 4, CLSNODE.ACT_READY_HOUR, CLSNODE.ACT_READY_MIN, ActInReady)
	CALLOUT.Weekly(getfenv(2), 7, CLSNODE.ACT_READY_HOUR, CLSNODE.ACT_READY_MIN, ActInReady)
	
--	DATA.Restore(path_phyleFight, PhyleFightResult)
	
	PROTOCOL.s_phyleFight_list = s_phyleFight_list
	PROTOCOL.s_phyleFight_stat = s_phyleFight_stat
	PROTOCOL.s_phyleFight_rank = s_phyleFight_rank
	PROTOCOL.s_phyleFight_leave = s_phyleFight_leave
	
	PROTOCOL.s_phyleFight_nodeInfo = s_phyleFight_nodeInfo
	PROTOCOL.s_phyleFight_member = s_phyleFight_member
	PROTOCOL.s_phyleFight_myInfo = s_phyleFight_myInfo
	
	PROTOCOL.s_phyleFight_join = s_phyleFight_join
	PROTOCOL.s_phyleFight_cancel = s_phyleFight_cancel
end
