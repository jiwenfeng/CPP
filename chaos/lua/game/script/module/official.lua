--官职信息

local MIN_JOIN_LV = 20					--所需等级
local IMPEACH_SUCCESS_RATE = 0.8		--弹劾成功所需支持率
local BANNED_TIMELONG = 30 * 60			--禁言时长
local UNLOCK_BANNED_COST = 50
local IMPEACH_NEED_INGOT = 100			--弹劾所需元宝
local TIMELONG_BEFORE_OVER = 10 * 60	--结束前通知
local VOTE_ITEM_ID = 104009				--玉规id

local ELECT_TIME_LONG = 25 * 3600 - 5 * 60		--竞选时长
local IMPEACH_TIME_LONG = (6 * 24 - 1) * 3600 - 5 * 60	--弹劾时长
local MIN_OFFICIAL_LV = 3
local MAX_OFFICIAL_LV = 5

local posState = {["idle"] = 0, ["normal"] = 1, ["elect"] = 2, ["impeach"] = 3, ["join"] = 4}
local posList = {
	["11"] = "105", ["12"] = "103", ["13"] = "104", ["14"] = "101", 
	["15"] = "101", ["16"] = "102", ["17"] = "102"
}


local OfficialInfoList = {}			--官职信息

function GetOfficialInfoById(id)
	return OfficialInfoList[id]
end

function GetOfficialNameById(id)
	local info = GetOfficialInfoById(id)
	if not info then return end
	return info.name
end

--通过位置获取官职id
function GetOfficialIdByPos(pos)	
	return posList[pos]
end

function GetOfficialInfoByPos(pos)
	local id = GetOfficialIdByPos(pos)
	return GetOfficialInfoById(id)
end

function CalVoteRate(agree, against)
	local sum = agree + against
	if sum == 0 then
		return 0
	end
	local rate = agree / sum * 100
	return rate - rate % 0.01
end
-----------------------------------------------
local AllOfflicalDataList = {}		--当前官职数据表
local path_official = "chaos.official"

function GetOfficialDataByPos(phyle, pos)
	AllOfflicalDataList[phyle] = AllOfflicalDataList[phyle] or {}
	return AllOfflicalDataList[phyle][pos]
end

function GetOfficialListByPhyle(phyle)
	AllOfflicalDataList[phyle] = AllOfflicalDataList[phyle] or {}
	return AllOfflicalDataList[phyle]
end

function SetOfficialDataByPos(phyle, pos, data)
	AllOfflicalDataList[phyle] = AllOfflicalDataList[phyle] or {}
	AllOfflicalDataList[phyle][pos] = data
	DATA.SaveData(path_official, AllOfflicalDataList, phyle)
end

function RmOfficialDataByPos(phyle, pos)
	AllOfflicalDataList[phyle] = AllOfflicalDataList[phyle] or {}
	if pos then
		AllOfflicalDataList[phyle][pos] = nil
		DATA.SaveData(path_official, AllOfflicalDataList, phyle)
	else
		AllOfflicalDataList[phyle] = nil
		DATA.DBRemove(path_official, {_id = phyle})
	end
end

function SaveOfficialData(phyle, pos, unionId)
	local buff = {}
	buff.owner = unionId
	if unionId then
		buff.state = "normal"
	else
		buff.state = "idle"
	end
	buff.timestamp = os.time()
	SetOfficialDataByPos(phyle, pos, buff)
end

function GetOfficialPosByUnionId(phyle, unionId)
	AllOfflicalDataList[phyle] = AllOfflicalDataList[phyle] or {}
	for k, v in pairs(AllOfflicalDataList[phyle]) do
		if v.owner == unionId then
			return k
		end
	end
end

function ModOfficialData(phyle, pos, key, val)
	local info = GetOfficialDataByPos(phyle, pos)
	if not info then
		return
	end
	if key == "banned" then
		return SetDailyBanNum(phyle, pos, val - GetDailyBanNum(phyle, pos))
	end
	info[key] = val
	SetOfficialDataByPos(phyle, pos, info)
end

--检查职位是否空闲
function IsPosIdle(phyle, pos)
	local info = GetOfficialDataByPos(phyle, pos)
	if not info then return true end
	return not info.owner
end

--每日禁言次数
function SetDailyBanNum(phyle, pos, num)
	local info = GetOfficialDataByPos(phyle, pos)
	if not info then
		return
	end
	info.banned = info.banned or {}
	local banInfo = info.banned
	if not banInfo.time or CalDiffDays(banInfo.time, os.time()) ~= 0 then
		banInfo.num = num
	else
		banInfo.num = banInfo.num + num
	end
end

function GetDailyBanNum(phyle, pos)
	local info = GetOfficialDataByPos(phyle, pos)
	if not info then
		return
	end
	info.banned = info.banned or {}
	local banInfo = info.banned
	if not banInfo.time or CalDiffDays(banInfo.time, os.time()) ~= 0 then
		return 0
	end
	return banInfo.num
end
------------------------------------------
local OfficialActList = {__RestoreHook = true, __state = {state = "close"}}	--官职活动信息表
local path_officialAct = "chaos.officialAct"

function GetOfficialActById(id)
	return OfficialActList[id]
end

function SetOfficialActById(id, data)
	OfficialActList[id] = data
	if data then
		DATA.SaveData(path_officialAct, OfficialActList, id)
	else
		DATA.DBRemove(path_officialAct, {_id = id})
	end
end

function GetOfficialActList()
	local buff = {}
	for k, v in pairs(OfficialActList) do
		if k ~= "__RestoreHook" and k ~= "__state" then
			buff[k] = v
		end
	end
	return buff
end

--活动状态
function GetOfficialActState()
	return OfficialActList.__state.state
end

function SetOfficialActState(state, timestamp)
	local info = OfficialActList.__state
	info.state = state
	info.endTime = timestamp
	SetOfficialActById("__state", info)
end
--活动结束时间点
function GetOfficialActEndTime()
	return OfficialActList.__state.endTime or os.time()
end

--初始化官职模块
function InitOfficialAct()
	SetOfficialActState("open")
	for phyle in pairs(PHYLE.GetAllPhyleList()) do
		for k in pairs(posList) do
			if IsPosIdle(phyle, k) then
				SaveOfficialData(phyle, k)
			end
		end
	end
	SendOfficialStateUpdate()
end


--竞选开启
function BeginElect()
	local state = GetOfficialActState()
	if state ~= "open" then
		return 
	end
	SetOfficialActState("elect", os.time() + ELECT_TIME_LONG)
	for phyle in pairs(PHYLE.GetAllPhyleList()) do
		for k in pairs(posList) do
			if IsPosIdle(phyle, k) then
				ModOfficialData(phyle, k, "state", "elect")
			end
		end
	end
	SendOfficialActNotice("elect", "begin")
	SendOfficialStateUpdate()
	StartOfficialActCallOut()
	StartOverNoticeCallOut()
end
--参加竞选
function JoinElect(union, pos)
	local buff = {}
	buff.phyle = union:GetPhyle()
	buff.officialId = GetOfficialIdByPos(pos)
	buff.pos = pos
	buff.vote = {agree = 0, against = 0}
	buff.timestamp = os.time()
	SetOfficialActById(union:GetId(), buff)
	SendofficialActUpdate(union:GetId(), "new")
end
--处理竞选数据
function ElectOver()
	local winnerList = {}
	for k, v in pairs(GetOfficialActList()) do
		local info = GetOfficialInfoById(v.officialId)
		if v.vote.agree >= info.support then
			winnerList[v.phyle] = winnerList[v.phyle] or {}
			winnerList[v.phyle][v.officialId] = winnerList[v.phyle][v.officialId] or {}
			table.insert(winnerList[v.phyle][v.officialId], {unionId = k, agree = v.vote.agree, time = v.timestamp})
		end
		SetOfficialActById(k)
	end
	for phyle, Data in pairs(winnerList) do		
		for k, v in pairs(Data) do
			table.sort(v, function(a, b) return a.agree > b.agree or (a.agree == b.agree and a.time < b.time) end)
			local index = 1
			for pos, id in pairs(posList) do
				if k == id and IsPosIdle(phyle, pos) then
					ElectSuccess(phyle, pos, v[index].unionId)
					index = index + 1
					if not v[index] then
						break
					end
				end
			end
		end
	end
	SetOfficialActState("open")
	for phyle in pairs(PHYLE.GetAllPhyleList()) do
		for k in pairs(posList) do
			if IsPosIdle(phyle, k) then
				ModOfficialData(phyle, k, "state", "idle")
			end
		end
	end
	SendOfficialStateUpdate()
end
--竞选成功
function ElectSuccess(phyle, pos, unionId)
	local union = UNION.GetUnionById(unionId)
	if union then
		local oldPos = GetOfficialPosByUnionId(phyle, unionId)
		if oldPos then
			SaveOfficialData(phyle, oldPos)
		end
		SaveOfficialData(phyle, pos, unionId)
		local officialId = GetOfficialIdByPos(pos)
		local roleId = union:GetLeader()
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetOfficialInfo(officialId, pos)
		else
			OFFLINE.SetOfflineData(roleId, "official", officialId, pos)
		end
	end
	SendElectSuccessMsg(phyle, pos, unionId)
end

function SendElectSuccessMsg(phyle, pos, unionId)
	local union = UNION.GetUnionById(unionId)
	local officialInfo = GetOfficialInfoByPos(pos)
	local roleId = union:GetLeader()
	local myMsg = string.format("恭喜您成功晋级为【%s】。", officialInfo.name)
	local unMsg = ""
	local phyMsg
	local notice
	if officialInfo.level == MIN_OFFICIAL_LV then
		unMsg = string.format("联盟盟主【%s】成功竞选为【%s】，联盟名声大振！", USER.GetUserNickById(roleId), officialInfo.name)
	elseif officialInfo.level == MIN_OFFICIAL_LV + 1 then
		unMsg = string.format("联盟盟主【%s】成功晋级为【%s】，联盟名声远扬！", USER.GetUserNickById(roleId), officialInfo.name)
		phyMsg = string.format("顺天承命，玩家【%s】成功晋级为【%s】，本势力终于有所依托。", USER.GetUserNickById(roleId), officialInfo.name)
	elseif officialInfo.level == MAX_OFFICIAL_LV then
		unMsg = string.format("联盟盟主【%s】成功晋级为【%s】，联盟名扬天下！", USER.GetUserNickById(roleId), officialInfo.name)
		phyMsg = string.format("逆天改命，谁与争锋！联盟【%s】的盟主【%s】成功晋级为【%s】，成为本势力不可磨灭的精神领袖。", union:GetName(), USER.GetUserNickById(roleId), officialInfo.name)
		notice = string.format("犯我【%s】者，虽远必诛！恭喜玩家【%s】成功晋级为势力【%s】。", PHYLE.IsValidPhyle(phyle), USER.GetUserNickById(roleId), officialInfo.name)
	end
	CHAT.SendSysMsg("system", myMsg, {roleId})
	union:SendUnionMsg(unMsg)
	if phyMsg then
		CHAT.SendSysMsg("phyle", phyMsg, USER.GetUserOnlineByPhyle(phyle))
	end
	if notice then
		CHAT.SendSysNotice(notice)
	end
end

--处理投票
function ProVote(unionId, agree, against)
	local info = GetOfficialActById(unionId)
	if not info then return end
	if agree then
		info.vote.agree = info.vote.agree + agree
	end
	if against then
		info.vote.against = info.vote.against + against
	end
	SendofficialActUpdate(unionId, "vote")
end


--弹劾开启
function BeginImpeach()
	local state = GetOfficialActState()
	if state ~= "open" then
		return 
	end
	SetOfficialActState("impeach", os.time() + IMPEACH_TIME_LONG)
	SendOfficialActNotice("impeach", "begin")
	SendOfficialStateUpdate()
	StartOfficialActCallOut()
	StartOverNoticeCallOut()
end

--发起弹劾
function ImpeachOfficial(user, pos)
	local phyle = user:GetPhyle()
	local info = GetOfficialDataByPos(phyle, pos)
	if not info or not info.owner then
		return
	end
	local buff = {}
	buff.phyle = phyle
	buff.promoter = user:GetRoleId()
	buff.pos = pos
	buff.officialId = GetOfficialIdByPos(pos)
	buff.vote = {agree = 0, against = 0}
	buff.timestamp = os.time()
	SetOfficialActById(info.owner, buff)
	ModOfficialData(phyle, pos, "state", "impeach")
	SendofficialActUpdate(info.owner, "new")		--更新
end

function ImpeachOver()
	for k, v in pairs(GetOfficialActList()) do
		local info = GetOfficialInfoById(v.officialId)
		if v.vote.agree >= info.against then
			local rate = CalVoteRate(v.vote.agree, v.vote.against)
			if rate >= IMPEACH_SUCCESS_RATE then
				ImpeachSuccess(v.phyle, v.pos)
				SendImpeachOverMsg(v.phyle, v.pos, k, "win")
			end
		else
			ModOfficialData(v.phyle, v.pos, "state", "normal")
			SendImpeachOverMsg(v.phyle, v.pos, k, "lose")
		end
		SetOfficialActById(k)
	end
	SetOfficialActState("open")
	SendOfficialStateUpdate()
end

--官员卸任
function ImpeachSuccess(phyle, pos)
	local officialData = GetOfficialDataByPos(phyle, pos)
	local union = UNION.GetUnionById(officialData.owner)
	if union then
		local roleId = union:GetLeader()
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetOfficialInfo()
		else
			OFFLINE.SetOfflineData(roleId, "official")
		end
	end
	SaveOfficialData(phyle, pos)
end

function SendImpeachOverMsg(phyle, pos, unionId, result)
	local union = UNION.GetUnionById(unionId)
	local officialInfo = GetOfficialInfoByPos(pos)
	local roleId = union:GetLeader()
	local actInfo = GetOfficialActById(unionId)
	local operator = USER.GetUserNickById(actInfo.promoter)
	local msg = ""
	local heMsg = ""
	if result == "win" then
		msg = string.format("玩家【%s】对您发起的弹劾获得大量民众的支持，因此您被解除了【%s】职务。", operator, officialInfo.name)
		heMsg = string.format("您成功地弹劾了玩家【%s】的【%s】职务。", USER.GetUserNickById(roleId), officialInfo.name)
	else
		msg = string.format("玩家【%s】对您发起的弹劾以失败告终。", operator)
		heMsg = string.format("您对【%s】发起的弹劾因支持率不足而未能如愿。", USER.GetUserNickById(roleId))
	end
	CHAT.SendSysMsg("system", heMsg, {actInfo.promoter})
	local user = USER.GetOnlineUser(roleId)
	if user then
		CHAT.SendSysMsg("system", msg, {roleId})
	else
		local mail = {title = "职位弹劾", content = string.format("%s %s", os.date("%Y-%m-%d %H:%M"), msg)}
		MAIL.SendSysMail(roleId, mail)
	end
end

--启动官职活动定时器
function StartOfficialActCallOut()
	local info = OfficialActList.__state
	if info.state == "close" or info.state == "open" then return end
	local timelong = info.endTime - os.time()
	if timelong <= 0 then
		if info.state == "elect" then
			ElectOver()
		elseif info.state == "impeach" then
			ImpeachOver()
		end
	else
		CALLOUT.CallOut(info.state == "elect" and ElectOver or ImpeachOver, timelong)
	end
end

--启动结束前提醒定时器
function StartOverNoticeCallOut()
	local info = OfficialActList.__state
	if info.state == "close" or info.state == "open" then return end
	local timelong = info.endTime - os.time() - TIMELONG_BEFORE_OVER
	if timelong <= 0 and info.endTime - os.time() > 0 then
		SendOverNotice()
	else
		CALLOUT.CallOut(SendOverNotice, timelong)
	end
end

function SendOverNotice()
	local info = OfficialActList.__state
	if info.state == "close" or info.state == "open" then return end
	SendOfficialActNotice(info.state, "over")
end

function SendOfficialActNotice(type, period)
	local msg = ""
	if type == "elect" then
		if period == "begin" then
			msg = "官职竞选活动已开启，请各位心存社稷的有志之士前往【邗昊】处报名。"
		elseif period == "over" then
			msg = "职位竞选将于十分钟后结束，请为自己支持的候选人投上关键的一票。"
		end
	elseif type == "impeach" then
		if period == "begin" then
			msg = "官职弹劾活动已开启，请各位心存社稷的有志之士前往【邗昊】处查看。"
		elseif period == "over" then
			msg = "职位弹劾将于十分钟后结束，请明断是非投出关键的一票。"
		end
	end
	CHAT.SendSysNotice(msg)
end

function SaveAllOfficialData()
	for k, v in pairs(OfficialActList) do
		if k ~= "__state" then
			SetOfficialActById(k, v)
		end
	end
end


function RestoreHook(ret)
	if not ret then
		return
	end
	StartOverNoticeCallOut()
	StartOfficialActCallOut()
end
--------------------------------------------
function GetOfficialPosList(user)
	local phyle = user:GetPhyle()
	local unionId = user:GetUnionId()
	local voteInfo
	if unionId then
		voteInfo = IsUnionInVote(unionId)
	end
	local sData = {}
	for k, v in pairs(GetOfficialListByPhyle(phyle)) do
		local info = GetOfficialInfoByPos(k)
		local buff = {pos = k}
		buff.state = v.state
		if v.state == "elect" and voteInfo and voteInfo.pos == k then
			buff.state = "join"
		end
		if v.owner then
			local union = UNION.GetUnionById(v.owner)
			if union then
				buff.unionName = union:GetName()
				local roleId = union:GetLeader()
				buff.nick = USER.GetUserNickById(roleId)
				buff.headId = USER.GetUserHeadById(roleId)
			end
		end
		local officialId = GetOfficialIdByPos(k)
		buff.officialId = officialId
		buff.name = info.name
		buff.level = info.level
		buff.needAgree = info.support
		buff.needPos = {}
		local list = info.electNeed
		if GetOfficialActState() == "impeach" then
			list = info.impeachNeed
		end
		for key in pairs(list) do
			local officialInfo = GetOfficialInfoById(key)
			table.insert(buff.needPos, {name = officialInfo.name, state = user:GetOfficialId() == key and 1 or 0})
		end
		if info.level == MIN_OFFICIAL_LV then
			local pos = GetOfficialPosByUnionId(phyle, unionId)
			table.insert(buff.needPos, {name = "联盟盟主", state = (not pos and user:GetUnionPos() == CLSUNION.LEADER) and 1 or 0})
		end
		buff.right = {}
		buff.right.expRate = info.attr.expRate
		buff.right.goldRate = info.attr.goldRate
		buff.right.ban = info.attr.ban and 1 or 0
		if info.attr.ban and user:GetOfficialId() == officialId then
			buff.right.banMax = info.attr.ban
			buff.right.banNum = GetDailyBanNum(phyle, k)
		end
		table.insert(sData, buff)
	end
	return sData
end


function GetFormatedActData(unionId, type)
	local actInfo = GetOfficialActById(unionId)
	local info = GetOfficialInfoById(actInfo.officialId)
	local union = UNION.GetUnionById(unionId)
	if union then
		local buff = {}
		buff.unionId = unionId
		buff.name = info.name
		buff.level = info.level
		buff.unionName = union:GetName()
		local roleId = union:GetLeader()
		buff.nick = USER.GetUserNickById(roleId)
		buff.headId = USER.GetUserHeadById(roleId)
		buff.agree = actInfo.vote.agree
		if type == "elect" then
			buff.needAgree = info.support
		elseif type == "impeach" then
			buff.against = actInfo.vote.against
			buff.rate = CalVoteRate(actInfo.vote.agree, actInfo.vote.against)
			buff.needAgree = info.against
		end
		return buff
	end
end

function GetFormatedActList(level, type)
	local sData = {}
	for k, v in pairs(GetOfficialActList()) do
		local info = GetOfficialInfoById(v.officialId)
		if info.level == level then
			local buff = GetFormatedActData(k, type)
			if buff then
				table.insert(sData, buff)
			end
		end
	end
	return sData
end

--更新官职活动的开启状态
function SendOfficialStateUpdate()
	for k in pairs(USER.GetUserFocusListByType("officialPos")) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			PROTOCOL.s_official_stat(con, "get")
			PROTOCOL.s_official_posList(con, "get")
		end
	end
	for k in pairs(USER.GetUserFocusListByType("elect")) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			PROTOCOL.s_official_stat(con, "get")
		end
	end
end

local actUpdateList = {
	new = function(unionId, type)
		local sData = GetFormatedActData(unionId, type)
		sData.action = "new"
		return sData
	end,
	del = function(unionId)
		local sData = {unionId = unionId}
		sData.action = "del"
		return sData
	end,
	vote = function(unionId, type)
		local sData = {unionId = unionId}
		local actInfo = GetOfficialActById(unionId)
		sData.agree = actInfo.vote.agree
		if type == "elect" then
			local info = GetOfficialInfoById(actInfo.officialId)
			sData.needAgree = info.support
		elseif type == "impeach" then
			sData.against = actInfo.vote.against
			sData.rate = CalVoteRate(actInfo.vote.agree, actInfo.vote.against)
			sData.action = "vote"
		end
		return sData
	end,
}
--更新官职活动的变化情况
function SendofficialActUpdate(unionId, kind)
	if not actUpdateList[kind] then return end
	local state = GetOfficialActState()
	if state ~= "elect" and state ~= "impeach" then return end
	local sData = actUpdateList[kind](unionId, state)
	for k in pairs(USER.GetUserFocusListByType("elect")) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_official_actUpdate", sData)
		end
	end
	if kind == "new" or kind == "del" then
		for k in pairs(USER.GetUserFocusListByType("officialPos")) do
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				PROTOCOL.s_official_posList(con, "get")
			end
		end
	end
end

function IsUnionInVote(unionId)
	return GetOfficialActById(unionId)
end

--检查联盟盟主卸任
function CheckUnionRelieve(phyle, unionId)
	for k, v in pairs(GetOfficialListByPhyle(phyle)) do
		if v.owner == unionId then
			ImpeachSuccess(phyle, k)
		end
	end
end
--转让官职
function AssignOfficialPos(user, roleId)
	local officialInfo = user:GetOfficialInfo()
	if not officialInfo or not officialInfo.id then return end
	local tUser = USER.GetOnlineUser(roleId)
	if tUser then
		tUser:SetOfficialInfo(officialInfo.id, officialInfo.pos)
	else
		OFFLINE.SetOfflineData(roleId, "official", officialInfo.id, officialInfo.pos)
	end
	user:SetOfficialInfo()
end

function GetMyOfficialPos(user)
	local union = user:GetUnion()
	local unionId = union:GetId()
	if union:GetOwner() == user:GetRoleId() then
		for k, v in pairs(GetOfficialListByPhyle(user:GetPhyle())) do
			if v.owner == unionId then
				return k
			end
		end
	end
end


local s_official_stat = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	local state = GetOfficialActState()
	if state == "close" then
		return ERROR.TipMsg(connection, "该功能尚未开放。")
	end
	local sData = {}
	sData.state = state
	if state ~= "open" then
		local timestamp = GetOfficialActEndTime()
		if timestamp then
			sData.startTime = timestamp - (state == "elect" and ELECT_TIME_LONG or IMPEACH_TIME_LONG)
			sData.endTime = timestamp
		end
	end
	sData.needLv = MIN_JOIN_LV
	sData.impeachNeed = IMPEACH_NEED_INGOT
	connection:SendProtocol("c_official_stat", sData)
end

local s_official_posList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	local state = GetOfficialActState()
	if state == "close" then
		return ERROR.TipMsg(connection, "该功能尚未开放。")
	end
	USER.SetUserFocusOn("officialPos", connection.roleId, 1)
	user:SetFocus("official", "officialPos")
	local sData = GetOfficialPosList(user)
	connection:SendProtocol("c_official_posList", sData)
end

local s_official_electList = function(connection, data)
	data = tonumber(data)
	if not data or data < MIN_OFFICIAL_LV or data > MAX_OFFICIAL_LV then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	USER.SetUserFocusOn("elect", connection.roleId, 1)
	user:SetFocus("official", "elect")
	local sData = GetFormatedActList(data, "elect")
	connection:SendProtocol("c_official_electList", sData)
end

local s_official_impeachList = function(connection, data)
	data = tonumber(data)
	if not data or data < MIN_OFFICIAL_LV or data > MAX_OFFICIAL_LV then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	USER.SetUserFocusOn("elect", connection.roleId, 1)
	user:SetFocus("official", "elect")
	local sData = GetFormatedActList(data, "impeach")
	connection:SendProtocol("c_official_impeachList", sData)
end

local s_official_elect = function(connection, data)
	if not posList[data] then
		return ERROR.TipMsg(connection, "协议错误，无效的职位类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能进行该操作。")
	end
	if not IsPosIdle(phyle, data) then
		return ERROR.TipMsg(connection, "指定的职位已经有人任职了。")
	end
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if not union or union:GetLeader() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是联盟盟主，不能参加竞选。")
	end
	if GetOfficialActState() ~= "elect" then
		return ERROR.TipMsg(connection, "竞选尚未开始。")
	end
	local info = GetOfficialInfoByPos(data)
	local flag = false	
	if info.level <= MIN_OFFICIAL_LV then
		local pos = GetOfficialPosByUnionId(phyle, unionId)
		if not pos then
			flag = true
		end
	else
		for k, v in pairs(info.electNeed) do
			if user:GetOfficialId() == k then
				flag = true
				break
			end
		end
	end
	if not flag then
		return ERROR.TipMsg(connection, "您的职位不符合竞选要求。")
	end
	local voteInfo = IsUnionInVote(unionId)
	if voteInfo then
		if voteInfo.officialId == data then
			return ERROR.TipMsg(connection, "您已经参加该职位的竞选了。")
		end
		local former = GetOfficialInfoById(voteInfo.officialId)
		local msg = string.format("您已经参加了【%s】的竞选，是否放弃原职位竞选，参与【%s】竞选？", former.name, info.name)
		return connection:SendProtocol("c_official_confirm", {pos = data, msg = msg})
	end
	JoinElect(union, data)
	connection:SendProtocol("c_official_elect", data)
end

local s_official_confirm = function(connection, data)
	if not posList[data] then
		return ERROR.TipMsg(connection, "协议错误，无效的职位类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能进行该操作。")
	end
	if not IsPosIdle(phyle, data) then
		return ERROR.TipMsg(connection, "指定的职位已经有人任职了。")
	end
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if not union or union:GetLeader() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是联盟盟主，不能参加竞选。")
	end
	if GetOfficialActState() ~= "elect" then
		return ERROR.TipMsg(connection, "竞选尚未开始。")
	end
	local info = GetOfficialInfoByPos(data)
	local flag = false
	if #info.electNeed <= 0 then
		flag = true
	else
		for k, v in pairs(info.electNeed) do
			if user:GetOfficialId() == k then
				flag = true
				break
			end
		end
	end
	if not flag then
		return ERROR.TipMsg(connection, "您的职位不符合竞选要求。")
	end
	local voteInfo = IsUnionInVote(unionId)
	local oldPos
	if voteInfo then
		oldPos = voteInfo.pos
		SendofficialActUpdate(unionId, "del")
		SetOfficialActById(unionId)
	end
	JoinElect(union, data)
	connection:SendProtocol("c_official_elect", {pos = data, oldPos = oldPos})
end

local s_official_impeach = function(connection, data)
	if not posList[data] then
		return ERROR.TipMsg(connection, "协议错误，无效的职位类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local phyle = user:GetPhyle()
	if not phyle then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能进行该操作。")
	end
	if IsPosIdle(phyle, data) then
		return ERROR.TipMsg(connection, "指定的职位是空闲的。")
	end
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if not union or union:GetLeader() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是联盟盟主，不能进行弹劾。")
	end
	if GetOfficialActState() ~= "impeach" then
		return ERROR.TipMsg(connection, "目前不是可弹劾时期。")
	end
	local info = GetOfficialInfoByPos(data)
	local flag = false
	if info.level == MIN_OFFICIAL_LV then
		flag = true
	else
		for k in pairs(info.impeachNeed) do
			if user:GetOfficialId() == k then
				flag = true
				break
			end
		end
	end
	if not flag then
		return ERROR.TipMsg(connection, "您的职位不符合弹劾要求。")
	end	
	local posInfo = GetOfficialDataByPos(phyle, data)
	if IsUnionInVote(posInfo.owner) then
		return ERROR.TipMsg(connection, "该官员已经处于弹劾中。")
	end
	if user:GetTotalIngot() < IMPEACH_NEED_INGOT then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetTotalIngot(-IMPEACH_NEED_INGOT)
	ImpeachOfficial(user, data)
	connection:SendProtocol("c_official_impeach", data)
end

local s_official_vote = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	data.num = tonumber(data.num)
	if not data.unionId or not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "请选择要投票的玩家及票数。")
	end
	data.action = tonumber(data.action)
	if data.action ~= 1 and data.action ~= 0 then
		return ERROR.TipMsg(connection, "请选择支持还是反对。")
	end
	local state = GetOfficialActState()
	if state ~= "elect" and state ~= "impeach" then
		return ERROR.TipMsg(connection, "现在不是竞选期或弹劾期，不能进行投票。")
	end
	local actInfo = GetOfficialActById(data.unionId)
	if not actInfo then
		return ERROR.TipMsg(connection, "没有指定的投票信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能进行该操作。")
	end
	if not user:DelItem(USERBASE.PACKAGE, VOTE_ITEM_ID, data.num) then
		return ERROR.TipMsg(connection, string.format("您的背包中的【%s】数量不足。", BASEITEM.GetItemById(VOTE_ITEM_ID).name))
	end
	if data.action == 1 then
		ProVote(data.unionId, data.num, 0)
	else
		ProVote(data.unionId, 0, data.num)
	end
	connection:SendProtocol("c_official_vote", data)
end

local s_official_close = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type == "official" then
		USER.SetUserFocusOn(focus.id, connection.roleId)
		user:SetFocus()
	end
	connection:SendProtocol("c_official_close", "true")
end

local s_official_banned = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local officialInfo = user:GetOfficialInfo()
	local info = GetOfficialInfoById(officialInfo.id)
	if not info or not info.attr.ban then 
		return ERROR.ERROR.TipMsg(connection, "您没有禁言权限。")
	end
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if connection.roleId == roleId then
		return ERROR.TipMsg(connection, "不能对自己禁言。")
	end
	local target = USER.GetOnlineUser(roleId)
	if not target then
		return ERROR.TipMsg(connection, "该玩家已经离线。")
	end
	local phyle = user:GetPhyle()
	if target:GetPhyle() ~= phyle then
		return ERROR.TipMsg(connection, "不能禁言其他势力的玩家。")
	end
	if CHAT.IsUserBanned(roleId) then
		return ERROR.TipMsg(connection, "此玩家已在默默地承受您的怒火。")
	end
	if GetDailyBanNum(phyle, officialInfo.pos) >= info.attr.ban then
		return ERROR.TipMsg(connection, "元神消耗过大，今日无法继续禁言。")
	end
	SetDailyBanNum(phyle, officialInfo.pos, 1)
	CHAT.SetUserBanned(roleId, BANNED_TIMELONG, connection.roleId)
	CHAT.SendBannedMsg("system", roleId)
	connection:SendProtocol("c_official_banned", data)
end

local s_official_unban = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local banInfo = CHAT.IsUserBanned(connection.roleId)
	if not banInfo then
		return ERROR.TipMsg(connection, "您没有被禁言。")
	elseif banInfo.state == 0 then
		return ERROR.TipMsg(connection, "不能解除永久禁言。")
	end
	if user:GetTotalIngot() < UNLOCK_BANNED_COST then
		return ERROR.TipMsg(connection, "您元宝数量不足。")
	end
	user:SetTotalIngot(-UNLOCK_BANNED_COST)
	CHAT.SetUserBanned(connection.roleId)
	connection:SendProtocol("c_official_unban", "true")
end

local s_official_cancel = function(connection, data)
	if not posList[data] then
		return ERROR.TipMsg(connection, "协议错误，无效的职位类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能进行该操作。")
	end
	if user:GetLv() < MIN_JOIN_LV then
		return ERROR.TipMsg(connection, "您的等级不足，不能进行该操作。")
	end
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if not union or union:GetLeader() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是联盟盟主，不能执行该操作。")
	end
	if GetOfficialActState() ~= "elect" then
		return ERROR.TipMsg(connection, "竞选尚未开始。")
	end
	local voteInfo = IsUnionInVote(unionId)
	if not voteInfo then
		return ERROR.TipMsg(connection, "您没有参加竞选。")
	end
	SetOfficialActById(unionId)
	SendofficialActUpdate(unionId, "del")
	connection:SendProtocol("c_official_cancel", data)
end


function __init__()
	local OFFICIAL = Import("autocode/officialList.lua")
	OfficialInfoList = OFFICIAL.GetTable()
	
	DATA.Restore(path_official, AllOfflicalDataList)	
	DATA.Restore(path_officialAct, OfficialActList)
	
	PROTOCOL.s_official_stat = s_official_stat
	PROTOCOL.s_official_posList = s_official_posList
	PROTOCOL.s_official_electList = s_official_electList
	PROTOCOL.s_official_impeachList = s_official_impeachList
	
	PROTOCOL.s_official_elect = s_official_elect
	PROTOCOL.s_official_confirm = s_official_confirm
	PROTOCOL.s_official_impeach = s_official_impeach
	PROTOCOL.s_official_vote = s_official_vote
	PROTOCOL.s_official_close = s_official_close
	PROTOCOL.s_official_cancel = s_official_cancel
	
	PROTOCOL.s_official_banned = s_official_banned
	PROTOCOL.s_official_unban = s_official_unban
	
	CALLOUT.CallFre(SaveAllOfficialData, 60 * 5)	--弹劾开始
	
	CALLOUT.Weekly(getfenv(2), 1, 21, 0, BeginImpeach)	--弹劾开始
	CALLOUT.Weekly(getfenv(2), 7, 20, 0, BeginElect)	--竞选开启
end
