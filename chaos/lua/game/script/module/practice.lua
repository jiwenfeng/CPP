local REFRESH_FATE_FREQUENCE = 4 * 3600
local MIN_LV = 10
local REFRESH_COST = 10
local PRACTICE_TIMELONG = 6 * 3600

clsUser = USERBASE.clsUser

function clsUser:AddInvite(roleId)
	self.__temp.fate = self.__save.fate or {}
	self.__temp.fate.invite = self.__temp.fate.invite or {}
	table.insert(self.__temp.fate.invite, roleId)
end

function clsUser:GetInviteList()
	self.__temp.fate = self.__save.fate or {}
	return self.__temp.fate.invite
end

function clsUser:GetFateInfo()
	self.__save.fate = self.__save.fate or {}
	return self.__save.fate 
end

function clsUser:GetFateList()
	self.__save.fate = self.__save.fate or {}
	return self.__save.fate.list or {}
end

function clsUser:GetFateInfoByIndex(index)
	self.__save.fate = self.__save.fate or {}
	for k, v in pairs(self.__save.fate.list or {}) do
		if v.index == index then
			return v
		end
	end
end

function clsUser:FormatInviteList()
	local sData = {}
	self.__save.fate = self.__save.fate or {}
	self.__save.fate.invite = self.__save.fate.invite or {}
	for k, v in pairs(self:GetInviteList()) do
		local buf = {}
		buf.roleId = v
		buf.nick = USER.GetUserNickInfo(v, "nick")
		buf.status = USER.GetUserNickInfo(v, "status")
		buf.lv = USER.GetuserNickInfo(v, "Lv")
		table.insert(sData, buf)
	end
	return sData
end

function clsUser:ClearInviteList()
	self.__save.fate = self.__save.fate or {}
	self.__save.fate.invite = {}
end

function CreateRobot()
	local sData = {}
	local jobList = JOB.GetJobTypeList("basic")
	local job = jobList[math.random(1, #jobList)]
	local info = NEWUSER.GetModelInfoIdByJob(job, 0)
	sData.userId = STATS.GenId("practice")
	sData.nick= NAME.GenName(0, "user")
	sData.index = 4
	sData.headId = info.headId
	sData.sex = 0
	sData.job = job
	sData.modelId = info.modelId
	return sData
end

function clsUser:RefreshFateList()
	if self.__temp.fateCallId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.fateCallId)
	end
	self.__save.fate = self.__save.fate or {}
	self.__save.fate.list = {}
	local friend = self:GetFriendList()
	local first
	local t = {}
	for k, v in pairs(friend:GetMemberList("friend")) do
		local target = USER.GetOnlineUser(k)
		if target and target:GetLv() >= MIN_LV and target:GetWorkInfo() ~= "prac" then
			if first then
				t = {favorLv = v.favorLv, favor = v.favor, type = v.type}
				first = {roleId = v.roleId, index = 1, state = 1}
			elseif t.favorLv > v.favorLv or 
				(t.favorLv == v.favorLv and t.favor < v.favor) or 
				(t.favorLv == v.favorLv and t.favor == v.favor and t.type > v.type) then
				t = {favorLv = v.favor, favor = v.favor, type = v.type}
				first.roleId = k
			end
		end
	end
	if first then
		table.insert(self.__save.fate.list, first)
	end
	local sData = {}
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:GetLv() >= MIN_LV and k ~= t.roleId and v:GetWorkInfo() ~= "prac" and k ~= self:GetRoleId() then
			table.insert(sData, k)
		end
	end
	local index = 2
	local len = #sData
	while len >= 1 and index <= 3 do
		local index = math.random(1, len)
		local roleId = sData[index]
		if roleId then
			index = index + 1
			sData[index] = nil
			table.insert(self.__save.fate.list, {roleId = roleId, index = index, state = friend:IsInMemberList("friend", roleId) or 0})
			len = len - 1
		end
	end
	local tb = CreateRobot()
	table.insert(self.__save.fate.list, tb)
	self.__save.fate.timestamp = os.time()
	self:StartFateCallOut()
end

function clsUser:StartFateCallOut()
	if self:GetLv() < MIN_LV then
		return
	end
	self.__save.fate = self.__save.fate or {}
	self.__save.fate.timestamp = self.__save.fate.timestamp or os.time()
	local timelong = os.time() - self.__save.fate.timestamp
	if timelong >= REFRESH_FATE_FREQUENCE then
		self:RefreshFateList()
		self.__save.fate.timestmap = os.time() - (timelong % REFRESH_FATE_FREQUENCE)
	else
		self.__temp.fateCallId = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.RefreshFateList, timelong, self)
	end
end

--type 1主动结束修炼 0 被动结束
--系统生成的人物只能是被动结束
function clsUser:EndPractice(type)
	self:PracticeOver()
	if type == 0 then
		if self:GetId() ~= "robot" then
			self:StartMeditateCallOut()
		else
			self:LeaveScene()		
			self:Destroy()
		end
	elseif type == 1 then
		local target = self.__save.fate.prac.obj
		target:EndPractice(0)
	end
end

function clsUser:PracticeOver()
	if self.__temp.pracId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.pracId)
		self.__temp.pracId = nil
	end
	self:SetWorkInfo("rest")
	local locate = self:GetLocateInfo()
	self:Broad2ExNeighbor("stand", locate.x, locate.y)
end

function clsUser:BeginPractice(roleId, obj)
	local locate = self:GetLocateInfo()
	self:Broad2ExNeighbor("prac", locate.x, locate.y)
	local rate = 1.5
	if target:GetId() == "robot" then	
		rate = 1.1
	end
	if self:GetId() == "robot" then	--系统生成的人物不挂定时器
		return
	end
	self:SetWorkInfo("prac")
	self.__save.fate = self.__save.fate or {}
	self.__save.fate.prac = self.__save.fate.prac or {}
	self.__save.fate.prac.endtime = os.time() + PRACTICE_TIMELONG
	self.__save.fate.prac.target = obj
	self:StartPracCallOut(rate)
end

function clsUser:StartPracCallOut(rate)
	if os.time() >= self.__save.fate.prac.endtime then
		return self:EndPractice(1)
	end
	self.__temp.PracId = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.CalPracticeExp, 30, self, rate)
end

function clsUser:CalPracticeExp(rate)
	if self:GetId() ~= "robot" then
		local exp = math.floor(self:GetMeditateExp() / 2 * rate)
		self:SetExp(exp)
		self:SendMeditateUpdate("prac", exp)
	end
	self:StartPracCallOut(rate)
end

function clsUser:PracWithRobot()
	local data = self:GetFateInfoByIndex(4) 
	if data then
		local user = clsUser:New()
		user:InitUser(data.username, data.nick, data.userId, data.headId, data.job, data.sex, true)
		user:SetId("robot")
		user:AgreeInvite(self)
		return true
	end
	return false
end

function clsUser:AgreeInvite(target)
	local locate = target:GetLocateInfo()
	local newX = locate.x + 100
	local newY = locate.y
	if newX >= 2100 then
		newX = locate.x - 100
	end
	local scene = target:GetScene()
	self:EnterScene(scene, newX, newY)
	target:BeginPractice(self:GetRoleId(), self)
	self:BeginPractice(target:GetRoleId(), target)
end

function clsUser:FormatFateList()
	local sData = {}
	sData.list = {}
	local friend = self:GetFriendList()
	self.__save.fate = self.__save.fate or {}
	for k, v in pairs(self.__save.fate.list or {}) do
		local buf = {}
		buf.index = v.index
		buf.nick = v.nick or USER.GetUserNickInfo(v.roleId, "nick")
		buf.sex = v.sex or USER.GetUserNickInfo(v.roleId, "sex")
		buf.headId = v.headId
		buf.modelId = v.modelId or USER.GetUserNickInfo(v.roleId, "modelId") or "101"
		if v.index == 4 then
			buf.state = 1
		else
			buf.state = friend:IsInMemberList("friend", v.roleId) and 1 or 0	--0 添加好友
		end
		table.insert(sData.list, buf)
	end
	sData.timestamp = self.__save.fate.timestamp or os.time()
	sData.cost = REFRESH_COST
	return sData
end

function clsUser:IsInMyList(roleId)
	for k, v in pairs(self:GetInviteList()) do
		if v == roleId then
			return true
		end
	end
	return false
end

local s_practice_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "您等级不够，不能寻找有缘人")
	end
	local sData = user:FormatFateList()
	connection:SendProtocol("c_practice_list", sData)
end

local s_practice_refresh = function(connection, data)
	if data ~= "refresh" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "您等级不足")
	end
	if user:GetTotalIngot() < REFRESH_COST then
		return ERROR.TipMsg(connection, "您元宝数量不足")
	end
	user:SetTotalIngot(-REFRESH_COST)
	user:RefreshFateList()
	connection:SendProtocol("c_practice_refresh", data)
	PROTOCOL.s_practice_list(connection, "get")
end

local s_practice_invite = function(connection, data)
	data = tonumber(data)
	if not data or data < 1 or data > 4 then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetWorkInfo() == "prac" then
		return ERROR.TipMsg(connection, "您已经在修炼状态")
	end
	if not user:IsInTown() then
		return ERROR.TipMsg(connection, "只有在城镇中才能使用此功能")
	end
	local info = user:GetFateInfoByIndex(data)
	if not info.roleId or not next(info) then
		return ERROR.TipMsg(connection, "该位置没有人")
	end
	if data == 4 then
		user:PracWithRobot()
	else
		local target = USER.GetOnlineUser(info.roleId)
		if not target then
			return ERROR.TipMsg(connection, "只能邀请在线好友修炼")
		end
		local friend = user:GetFriendList()
		if not friend:IsInMemberList("friend", info.roleId) then
			return ERROR.TipMsg(connection, "对方不是您的好友，请先添加好友")
		end
		target:AddInvite(connection.roleId)
		target:SendProtocol("c_practice_msg", "new")
	end
	connection:SendProtocol("c_practice_invite", data)
end

local s_practice_inviteList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "您等级不足")
	end
	local sData = user:FormatInviteList()
	connection:SendProtocol("c_practice_inviteList", sData)
end

local s_practice_leave = function(connection, data)
	if data ~= "leave" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:ClearInviteList()
	connection:SendProtocol("c_practice_leave", data)
end

local s_practice_agree = function(connection, data)
	if not date then
		return ERROR.TipMsg(connection, "无效的协议")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < MIN_LV then
		return ERROR.TipMsg(connection, "您等级不足")
	end
	local target = USER.GetOnlineUser(data)
	if not data then
		return ERROR.TipMsg(connection, "该人不在线")
	end
	if not user:IsInMyList(data) then
		return ERROR.TipMsg(connection, "该人没有邀请您双修")
	end
	if user:GetWorkInfo() == "prac" then
		return ERROR.TipMsg(connection, "您已经在双修中")
	end
	if not user:IsInTown() then
		return ERROR.TipMsg(connection, "您不在城镇中，不能使用此功能")
	end
	if target:GetWorkInfo() == "prac" then
		return ERROR.TipMsg(connection, "对方当前无法进行双修")
	end
	if not target:IsInTown() then
		return ERROR.TipMsg(connection, "对方当前无法进行双修")
	end
	user:AgreeInvite(target)
	connection:SendProtocol("c_practice_agree", data)
end

local s_practice_finish = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connetion, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetWorkInfo() ~= "prac" then
		return ERROR.TipMsg(connection, "您不在双修状态")
	end
	user:EndPractice(1)
	connection:SendProtocol("c_practice_finish", data)
end

function __init__()
	PROTOCOL.s_practice_finish = s_practice_finish
	PROTOCOL.s_practice_agree = s_practice_agree
	PROTOCOL.s_practice_leave = s_practice_leave
	PROTOCOL.s_practice_inviteList = s_practice_inviteList
	PROTOCOL.s_practice_invite = s_practice_invite
	PROTOCOL.s_practice_list = s_practice_list
	PROTOCOL.s_practice_refresh = s_practice_refresh
end

