local BannedList = {}			--禁言列表
local path_banned = "chaos.banned"
local CHAT_DELAY = 3
local BUGLE_ID = 101001

function IsUserBanned(roleId)	
	local info = BannedList[roleId]
	if not info then
		return false
	end
	if info.state ~= 0 and info.time <= os.time() then		--禁言时间结束
		BannedList[roleId] = nil
		DATA.DBRemove(path_banned, {_id = roleId})
		return false
	end
	return info
end

function SetUserBanned(roleId, time, operator)
	time = tonumber(time)
	if not time then
		BannedList[roleId] = nil
		return DATA.DBRemove(path_banned, {_id = roleId})
	end
	if time == 0 then
		BannedList[roleId] = {state = 0, operator = operator}		--状态为0，永久禁言
	else
		BannedList[roleId] = {state = 1, time = os.time() + time, timelong = time, operator = operator}		--状态为1，禁言一段时间
	end
	DATA.SaveData(path_banned, BannedList, roleId)
end

function GetUserBanned(roleId)
	return BannedList[roleId]
end

function GetBannedList()
	return BannedList
end

--发送禁言提示信息
function SendBannedMsg(type, roleId)
	local info = IsUserBanned(roleId)
	if info then
		if info.state == 0 then
			local msg = "您已经被永久禁言，不能在该频道发言。"
			SendSysMsg(type, msg, {roleId})
		else
			local msg = string.format("您已经被%s禁言，状态剩余%s分钟。", USER.GetUserNickById(info.operator) or "GM", math.ceil((info.time - os.time()) / 60))
	--		SendSysMsg(type, msg, {roleId}, {kind = "banned", id = 50})
			SendSysMsg(type, msg, {roleId})
		end
	end
end

--发送消息函数
function SendChatMsg(connection, data)
	local function send_chat()
		connection:SendProtocol("c_chat", data)
	end
	local ret, msg = pcall(send_chat)
	if not ret then
		DATA.log(string.format("%s SendChat to %s Failed: %s", data.type, connection.roleId, msg), "chat.log")
	end
end

--私聊频道
function PrivateChat(connection, user, chatdata, tRoleId)
	if tRoleId == connection.roleId then
		return
	end
	chatdata.phyle = nil
	chatdata.sex = nil
	chatdata.title = nil
	local con = LOGIN.GetConnection(tRoleId)
	if not con or not con.roleId then
		return SendSysMsg("private", string.format("玩家%s已经下线.", USER.GetUserNickById(tRoleId) or ""), {connection.roleId})
	end
--	connection:SendProtocol("c_chat", chatdata)
	if con.user:IsInBlack(connection.roleId) then
		return
	end
	con:SendProtocol("c_chat", chatdata)
end


--世界频道
function WorldChat(connection, user, chatdata)
	if IsUserBanned(user:GetRoleId()) then
		return SendBannedMsg("world", user:GetRoleId())	
	end
	if chatdata.worldChat == 1 then			--标记当日不再提醒
		user:SetSetting(SETTING.WORLD_CHAT_NO_REMIND, 1)
	end
	chatdata.worldChat = nil
	for tRoleId, obj in pairs(USER.GetAllOnlineUser()) do
		local con = LOGIN.GetConnection(tRoleId)
		if con and con.roleId == tRoleId then
			SendChatMsg(con, chatdata)
		end
	end
	user:SetFuncStatById(SETTING.FUNC_WORLD_CHAT, 1)
end

--喇叭频道
function HearsayChat(connection, user, chatdata)
	if IsUserBanned(user:GetRoleId()) then
		return SendBannedMsg("hearsay", user:GetRoleId())	
	end
	--if not user:DelItem(BUGLE_ID, 1) then
	if not user:DelItem(USERBASE.PACKAGE, BUGLE_ID, 1) then
		return ERROR.TipMsg(connection, "操作失败", "您背包里没有【喇叭】，请到商城购买！")
	end
--	SendSysPrompt(connection.roleId, "您在喇叭频道发言，消耗【喇叭】1个")
	if chatdata.hearsayChat == 1 then
		user:SetSetting(SETTING.HEARSAY_CHAT_NO_REMIND, 1)
	end
	chatdata.hearsayChat = nil
	for tRoleId, obj in pairs(USER.GetAllOnlineUser()) do
		local con = LOGIN.GetConnection(tRoleId)
		if con and con.roleId == tRoleId then
			SendChatMsg(con, chatdata)
		end
	end
	DATA.log(string.format("%s发布喇叭消息:%s", connection.roleId, chatdata.msg), "chat.log")
end

--联盟
function UnionChat(connection, user, chatdata)
	if IsUserBanned(user:GetRoleId()) then
		return SendBannedMsg("union", user:GetRoleId())	
	end
	local unionId = user:GetUnionId()
	if not unionId then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local unionobj = UNION.GetUnionById(unionId)	--联盟系统出来后按需要修改
	local memberList = unionobj:GetListByType("member")
	for roleId in pairs(memberList) do
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			SendChatMsg(con, chatdata)
		end
	end
end

--附近
function NearbyChat(connection, user, chatdata)
	if IsUserBanned(user:GetRoleId()) then
		return SendBannedMsg("nearby", user:GetRoleId())	
	end
	local scene = user:GetScene()
	if not scene or scene:GetType() == "empty" then
		local con = LOGIN.GetConnection(user:GetRoleId())
		if con and con.roleId == user:GetRoleId() then
			SendChatMsg(con, chatdata)
		end
	else
		for k, v in pairs(scene:GetSceneRoleList()) do 
			if v:GetRoleType() == "user" then
				local roleId = v:GetRoleId()
				local con = LOGIN.GetConnection(roleId)
				if con and con.roleId == roleId then
					SendChatMsg(con, chatdata)
				end
			end
		end
	end
end

--势力
function PhyleChat(connection, user, chatdata)
	if IsUserBanned(user:GetRoleId()) then
		return SendBannedMsg("phyle", user:GetRoleId())	
	end
	local phyle = user:GetPhyle()
	for k, v in pairs(USER.GetAllOnlineUser()) do 
		if v:GetPhyle() == phyle then
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				SendChatMsg(con, chatdata)
			end
		end
	end
end

function TeamChat(connection, user, chatdata)
	return ERROR.TipMsg(connection, "您尚未加入队伍。")
	--[[local memList = user:GetTeamMemberList()
	for k, v in pairs(memList) do
		local con = LOGIN.GetConnection(v)
		if con and con.roleId == v then
			SendChatMsg(con, chatdata)
		end
	end]]    --队伍出来后根据需要更改
end

local chat_func = {
	world = WorldChat,		--世界
	phyle = PhyleChat,		--势力
	union = UnionChat,		--联盟
	hearsay = HearsayChat,	--喇叭
	nearby = NearbyChat,	--附近
	team = TeamChat,		--队伍
	private = PrivateChat,	--私聊
}

--聊天
function Chat(connection, user, chatdata, roleId)
	if chat_func[chatdata.type] then
		user:SetChatTimestamp(os.time())
		chat_func[chatdata.type](connection, user, chatdata, roleId)
	else
		DATA.log(string.format("%s试图在%s频道发言", connection.roleId, chatdata.type or "null"), "chat.log")
	end
end

function GetFormatedNick(nick)
	return string.format("<a href=\"event:person_%s\"><u>%s</u></a>", nick, nick)
end


--发送系统消息
function SendSysMsg(channel, msg, list, param)  --消息跟发送名单{roleId, roleId}  全部发送 list=nil
--	local sysmsg = {type = channel, msg = msg, from = "system"}
	local sysmsg = {type = channel, msg = msg}
	if param then
		sysmsg.operatorId = param.id
		sysmsg.kind = param.kind
		sysmsg.goodsAttr = param.goodsAttr
	end
	if not list then
		local conTbl = LOGIN.GetAllConnection()
		for roleId, con in pairs(conTbl) do
			if con and con.user then
				if not con.roleId or con.roleId ~= roleId then
					DATA.log(string.format("systemChat connection Error: roleId = %s, con.roleId = %s", roleId or "", con.roleId or ""), "chat.log")
				else
					SendChatMsg(con, sysmsg)
				end
			end
		end
	else
		for _, roleId in ipairs(list) do
			local con = LOGIN.GetConnection(roleId)
			if con and con.user then
				if not con.roleId or con.roleId ~= roleId then
					DATA.log(string.format("systemChat connection Error: roleId = %s, con.roleId = %s", roleId or "", con.roleId or ""), "chat.log")
				else
					SendChatMsg(con, sysmsg)
				end
			end
		end
	end
end
--系统个人信息提示
function SendSysPrompt(roleId, msg)
--	local prompt = {type = "prompt", msg = msg, from = "system"}
	local prompt = {type = "system", msg = msg}
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		SendChatMsg(con, prompt)
	end
end

--发送系统公告  漂浮提示
function SendSysNotice(msg, list, onlyNotice)
--	local notice = {type = "notice", msg = msg, from = "system"}
	local notice = {type = "notice", msg = msg}
	local sysMsg = {type = "system", msg = msg}
	if list then
		for _, roleId in ipairs(list) do
			local con = LOGIN.GetConnection(roleId)
			if con and con.roleId == roleId then
				SendChatMsg(con, notice)
				if not onlyNotice then
					SendChatMsg(con, sysMsg)
				end
			end
		end
	else
		for roleId, obj in pairs(USER.GetAllOnlineUser()) do
			local con = LOGIN.GetConnection(roleId)
			if con and con.roleId == roleId then
				SendChatMsg(con, notice)			--漂浮公告
				if not onlyNotice then
					SendChatMsg(con, sysMsg)			--系统频道
				end
			end
		end
	end
end

function __init__()
	DATA.Restore(path_banned, BannedList)
	
	PROTOCOL.s_chat = function(connection, tomsg)
		--聊天函数
		local command, data = string.match(tomsg.msg, "^[:$](%w+)[ ]?(.*)")
		if command and ADMIN.GetCommand(command) then
			return ADMIN.ProGmCommand(connection, command, data)
		end
		tomsg.msg = NEWUSER.TranslateLegalStr(tomsg.msg)
		if not chat_func[tomsg.type] then
			return ERROR.TipMsg(connection, string.format("无效的频道类型：%s。", tomsg.type or "null"))
		end
		local tRoleId = USER.GetRoleIdByNick(tomsg.to)
		if tomsg.to and (not tRoleId or tRoleId == SYSTEM_ID) then
			return ERROR.TipMsg(connection, string.format("没有指定的玩家信息：%s。", tomsg.to or ""))
		end
		local user = assert(USER.GetOnlineUser(connection.roleId))
		local timestamp = os.time() - (user:GetChatTimestamp() or 0)
--[[		if timestamp < CHAT_DELAY then
			return ERROR.TipMsg(connection, string.format("%d秒后才能再次发言", CHAT_DELAY - timestamp))
		end]]
		if tRoleId and user:IsInBlack(tRoleId) then
			return ERROR.TipMsg(connection, "对方在您的黑名单中。")
		end
		local chatdata = {}
		chatdata.type = tomsg.type
		chatdata.msg = tomsg.msg
		chatdata.from = user:GetName()
		chatdata.phyle = user:GetPhyle()
--		chatdata.title = user:GetCurTitle()
		chatdata.sex = user:GetSex()
		if tomsg.type == "world" then
			chatdata.worldChat = tomsg.worldChat
		end
		if tomsg.type == "hearsay" then
			chatdata.hearsayChat = tomsg.hearsayChat
		end
		local vipLv = user:GetVipLv()
		if vipLv > 0 then
			chatdata.isvip = 1
		end
		if tomsg.goodsAttr then
			chatdata.goodsAttr = {}	
			for _, v in ipairs(tomsg.goodsAttr) do
				local item
				if v.showType == USERBASE.PACKAGE or v.showType == USERBASE.DEPOT then
					item = user:GetItemByAreaId(v.showType, v.areaId)
				elseif v.showType == USERBASE.EQUIP then
					if v.target and v.target ~= "user" then
						local hero = user:GetHero(v.target)
						if not hero then
							return ERROR.TipMsg(connection, string.format("无效的武将id：%s。", v.target))
						end
						item = hero:GetEquipByPos(v.areaId)
					else
						item = user:GetEquipByPos(v.areaId)
					end
				else
					return ERROR.TipMsg(connection, string.format("无效的物品展示类型：%s。", v.showType))
				end
				if not item or item:GetName() ~= v.name then
					return ERROR.TipMsg(connection, string.format("指定的物品【%s】不存在或者已经被移走了。", v.name))
				end
				local buff = item:GetFullAttr()
				buff.missionId = item:GetMissionId()
				buff.goodsName = v.name
				table.insert(chatdata.goodsAttr, buff)
			end
		end
		return Chat(connection, user, chatdata, tRoleId)
	end
end

