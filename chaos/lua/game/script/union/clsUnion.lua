--联盟

local MAX_LV = 10							--联盟最大等级
local MAX_VICELEADER_NUM = 3				--副盟主数量
local MIN_WAIT_TIMELONG = 24 * 3600			--使用高级功能所需最少时间
local APPLY_LIST_UPLIMIT = 200				--申请列表大小
local FREE_INVITE_NOTICE_NUM = 1			--免费邀请公告次数
LEADER = 1		--盟主
VICELEADER = 2	--副盟主
ELITE = 3		--精英
COMMON = 4		--成员

ACT_INVALID = -1	--功能未开放
ACT_OVER = 0		--活动已结束
ACT_BEGIN = 1		--活动已开启
ACT_UNSTART = 2		--活动未开始
ACT_READY = 3		--活动准备阶段

--local RECONSIDERATION_TIMELONG = 24 * 3600	--复议时长
local RECONSIDERATION_TIMELONG = 60	--复议时长

local UnionExpList = {}

function GetUnionInfoByLv(lv)
	return UnionExpList[lv]
end

local UnionActInfoList = {}

function GetAllUnionActList()
	return UnionActInfoList
end

function GetUnionActInfoById(id)
	return UnionActInfoList[id]
end

function GetMinWaitingTime()
	return MIN_WAIT_TIMELONG
end

local UnionActClassList = {}		--联盟活动class列表

function GetActClassById(id)
	return UnionActClassList[id]
end

function SetActClassById(id, class)
	UnionActClassList[id] = class
end
-------------------------------------------------------------------------

clsUnion = clsObject:Inherit()

function clsUnion:__init__()
	Super(clsUnion).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsUnion:initsavedata()
	self.__save.member = {}
	self.__save.apply = {}
	self.__save.Lv = 0
	self.__save.name = nil
	self.__save.wealth = 0
	
	self.__save.joinable = true		-- 可加入
	self.__save.act = {}
end

function clsUnion:inittempdata()
	self.__temp.act = {}
	self.__temp.callout = {}
end

function clsUnion:initUnion(id, creator, phyle, info, announce)
	self:SetId(id)
	self:SetCreator(creator)
	self:SetLeader(creator)
	self:SetPhyle(phyle)
	self:SetName(info.name)
	self:SetPicId(info.picId)
	self:SetLv(1)
	self:SetWealth(0)
	self:SetCreateTime(os.time())
	self:SetAnnounce(announce)
end

function clsUnion:SetId(id)
	self.__save.id = id
end

function clsUnion:GetId()
	return self.__save.id
end

function clsUnion:SetCreator(data)
	self.__save.creator = data
end

function clsUnion:GetCreator()
	return self.__save.creator
end

function clsUnion:SetLeader(roleId)
	self.__save.leader = roleId
end

function clsUnion:GetLeader()
	return self.__save.leader
end

function clsUnion:SetPicId(id)
	self.__save.picId = id
end

function clsUnion:GetPicId()
	return self.__save.picId
end

function clsUnion:SetPhyle(phyle)
	self.__save.phyle = phyle
end

function clsUnion:GetPhyle(phyle)
	return self.__save.phyle
end

function clsUnion:SetName(name)
	self.__save.name = name
end

function clsUnion:GetName()
	return self.__save.name
end

function clsUnion:SetCreateTime(data)
	self.__save.createTime = data
end

function clsUnion:GetCreateTime()
	return self.__save.createTime or os.time()
end

--成员列表
function clsUnion:GetMemberNum()
	return table.size(self.__save.member)
end

function clsUnion:GetMaxMemberNum()
	return GetUnionInfoByLv(self:GetLv()).uplimit
end

function clsUnion:GetMemberInfo(roleId)
	return self.__save.member[roleId]
end
--成员平均等级
function clsUnion:GetMemberAvgLv()
	local lv, num = 0, 0
	local avgLv
	for k, v in pairs(self.__save.member) do
		lv = lv + v.Lv
		num = num + 1
	end
	if num < 10 then
		avgLv = math.ceil(lv / 10)
	else
		avgLv = math.ceil(lv / num)
	end
	return avgLv, num
end
--申请列表
function clsUnion:GetApplyNum()
	return table.size(self.__save.apply)
end

function clsUnion:GetApplyMax()
	return APPLY_LIST_UPLIMIT
end

function clsUnion:GetApplyInfo(roleId)
	return self.__save.apply[roleId]
end
--免费邀请公告
function clsUnion:SetFreeInviteTimes(data)
	self.__save.invite = self.__save.invite or {}
	local info = self.__save.invite
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUnion:GetFreeInviteTimes()
	self.__save.invite = self.__save.invite or {}
	local info = self.__save.invite
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end

function clsUnion:GetMaxFreeInviteTimes()
	return FREE_INVITE_NOTICE_NUM
end

--type: member, apply
function clsUnion:IsInList(type, roleId)
	return self.__save[type][roleId]
end

function clsUnion:AddToList(type, roleId, data)
	self.__save[type][roleId] = data
end

function clsUnion:GetListByType(type)
	return self.__save[type] or {}
end

function clsUnion:RmFromList(type, roleId)
	if type == "member" then
		local delateInfo = self:GetDelateInfo()
		if delateInfo and delateInfo.list[roleId] then
			self:RemoveDelateRole(roleId)
		end
	end
	self.__save[type][roleId] = nil
end

--贡献度 type:enable, total
function clsUnion:SetDevote(roleId, type, data)
	local info = self.__save.member[roleId]
	if not info then
		return
	end
	info.devote = info.devote or {}
	info.devote[type] = math.max((info.devote[type] or 0) + data, 0)
end

function clsUnion:GetDevote(roleId, type)
	local info = self.__save.member[roleId]
	if not info or not info.devote then
		return 0
	end
	return info.devote[type] or 0
end

--副盟主数量
function clsUnion:GetViceLeaderMax()
	return MAX_VICELEADER_NUM
end

function clsUnion:GetViceLeaderNum()
	local num = 0
	for k, v in pairs(self:GetListByType("member")) do 
		if v.identity == VICELEADER then
			num = num + 1
		end
		if num >= MAX_VICELEADER_NUM then
			break
		end
	end
	return num
end

--添加成员
function clsUnion:AddToMemberList(roleId, identity)
	local buff = {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		buff.Lv = user:GetLv()
		buff.force = user:GetFightForce()
	else
		local info = self:GetApplyInfo(roleId)
		if not info then
			return
		end
		buff.Lv = info.Lv
		buff.force = info.force
	end
	buff.identity = identity or ELITE
	buff.timestamp = os.time()
	buff.joinTime = os.time()
	buff.devote = {enable = 0, total = 0}
	self:AddToList("member", roleId, buff)
	self:RmFromList("apply", roleId)
end

--添加申请列表
function clsUnion:AddToApplyList(roleId)
	local user = assert(USER.GetOnlineUser(roleId))
	local sData = {}
	sData.Lv = user:GetLv()
	sData.force = user:GetFightForce()
	sData.timestamp = os.time()
	self:AddToList("apply", roleId, sData)
	
	self:SendUnionUpdate("apply", self:GetListByLevel(VICELEADER))
	CHAT.SendSysMsg("system", "有新的加入申请", self:GetListByLevel(VICELEADER))
end

function clsUnion:ModMemberIdentity(roleId, data)
	local info = self.__save.member[roleId]
	if not info then
		return
	end
	info.identity =  data
	if data == LEADER then
		self:SetLeader(roleId)
	end
end

function clsUnion:GetTotalForce()
	local num = 0
	local list = self:GetListByType("member")
	for k, v in pairs(list) do
		num = num + v.force
	end
	return num
end

function clsUnion:SetAnnounce(data)
	self.__save.announce = data
end

function clsUnion:GetAnnounce()
	return self.__save.announce or ""
end

function clsUnion:SetWealth(wealth)
	self.__save.wealth = math.max((self.__save.wealth or 0) + wealth, 0)
end

function clsUnion:GetWealth()
	return self.__save.wealth or 0
end

function clsUnion:GetMaxLv()
	return MAX_LV
end

function clsUnion:GetLv()
	return self.__save.Lv
end

function clsUnion:SetLv(data)
	self.__save.Lv = (self.__save.Lv or 0) + data
	if self.__save.Lv > MAX_LV then
		self.__save.Lv = MAX_LV 
	elseif self.__save.Lv < 1 then
		self.__save.Lv = 1
	end
	self:CheckUnionAct()
end

--是否可加入
function clsUnion:SetJoinable(flag)
	self.__save.joinable = flag
end

function clsUnion:IsJoinable()
	return self.__save.joinable
end

--是否有申请
function clsUnion:HasApply()
	return next(self.__save.apply or {})
end
--[[
function clsUnion:DelUser(type, roleId)
	self.__save[type][roleId] = nil
	if type == "member" then
		self:ModMemberNum(-1)
	end
end]]

function clsUnion:GetUnionInfo(roleId)
	local unionId = self:GetId()
	local sData = {}
	local rankObj = RANK.GetRankObjByType("real", "unionLv")
	sData.rank = rankObj:IsDataExist({unionId = unionId}) or -1
	sData.unionId = unionId
	sData.name = self:GetName()
	sData.leader = USER.GetUserNickById(self:GetLeader())
	sData.Lv = self:GetLv()
	sData.num = self:GetMemberNum()
	sData.max = self:GetMaxMemberNum()
	sData.phyle = self:GetPhyle()
	sData.picId = self:GetPicId()
	sData.state = self:GetDelateInfo() and 1 or 0
	sData.force = self:GetTotalForce()
	sData.inviteTimes = UNION.GetTotalInviteTimes() - self:GetInviteTimes()
	if roleId then
		local memberInfo = self:GetMemberInfo(roleId)
		sData.identity = memberInfo.identity
		sData.wealth = self:GetWealth()
		sData.announce = self:GetAnnounce()
		sData.devote = self:GetDevote(roleId, "enable")
		sData.needWealth = GetUnionInfoByLv(self:GetLv()).wealth
	end
	return sData
end

-- 捐赠
function clsUnion:Donate(roleId, devote)
	self:SetDevote(roleId, "enable", devote)
	self:SetDevote(roleId, "total", devote)
	self:SetWealth(devote)
	local user = assert(USER.GetOnlineUser(roleId))
	user:SetFuncStatById(SETTING.FUNC_UNION_DEVOTE, 1)
end

function clsUnion:AddMember(roleId, identity, flag)
	self:AddToMemberList(roleId, identity)
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:SetUnionInfo(self:GetId(), identity, os.time())
	else
		OFFLINE.SetOfflineData(roleId, "setUnionInfo", self:GetId(), identity, os.time())
	end
	self:SendNewMemberMsg(roleId, flag)
	UNION.RemoveUserApplyInfo(roleId)
end

--解散联盟
function clsUnion:Dismiss()
	for k, v in pairs(self:GetListByType("member")) do 
		local user = USER.GetOnlineUser(k)
		if user then
			user:QuitUnion()
		else
			self:RmFromList("member", k)
			OFFLINE.SetOfflineData(k, "quitUnion", os.time())
		end
	end
	--OFFICIAL.CheckUnionRelieve(self:GetPhyle(), self:GetId())
	for k, v in pairs(self:GetUniActList()) do
		v:Destroy()
	end
	self:Destroy()
	UNION.RemoveUnion(self:GetId())
end

function clsUnion:FormatMemberList(type)
	local sData = {}
	local curTime = os.time()
	for k, v in pairs(self:GetListByType(type)) do
		local buf = {}
		buf.nick = USER.GetUserNickById(k)
		buf.Lv = v.Lv
		buf.force = v.force
		local obj = USER.GetOnlineUser(k)
		buf.state = obj and 1 or 0
		if type == "member" then
			buf.identity = v.identity
			buf.devoteTtl = self:GetDevote(k, "total")
			buf.devote = self:GetDevote(k, "enable")
			if not obj then
				buf.timelong = os.time() - v.timestamp
			end
		elseif type == "apply" then
			buf.timestamp = v.timestamp
		end
		table.insert(sData, buf)
	end
	if type == "member" then
		table.sort(sData, function(a, b)
			return a.identity < b.identity 
				or (a.identity == b.identity and a.Lv > b.Lv)
		end)
	else
		table.sort(sData, function(a, b) 
			local time_a = self:IsInList("apply", USER.GetRoleIdByNick(a.nick))
			local time_b = self:IsInList("apply", USER.GetRoleIdByNick(b.nick))
			return time_a.timestamp > time_b.timestamp
		end)
	end
	return sData
end


--刷新成员信息
function clsUnion:RefreshInfo(type, user)
	local roleId = user:GetRoleId()
	local info = self:IsInList(type, roleId)
	if info then
		info.Lv = user:GetLv()
		info.force = user:GetFightForce()
--		info.name = user:GetName()
		if type == "member" then
			info.timestamp = os.time()
		end
	end
end

--发送联盟内部公告
--flag 只发给会长/副会长
function clsUnion:SendUnionMsg(msg, flag)
	local list = {}
	for k, v in pairs(self:GetListByType("member")) do
		if flag and v.identity <= 2 then
			table.insert(list, k)
		else
			table.insert(list, k)
		end
	end
	CHAT.SendSysMsg("union", msg, list)
end
--新成员加入通知
function clsUnion:SendNewMemberMsg(roleId, flag)
	if not flag then		--通知加入者
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			ERROR.TipMsg(con, string.format("联盟【%s】已经通过了您的入盟申请。", self:GetName()))
		else
			local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
			mail.title = "欢迎加入联盟"
			mail.content = string.format("联盟【%s】已经通过了您的入盟申请。", self:GetName())
			MAIL.SendSysMail(roleId, mail)
		end
	end
	--联盟内部公告
	if roleId == self:GetCreator() then
		self:SendUnionMsg(string.format("您已成功建立联盟【%s】", self:GetName()))
	else
		self:SendUnionMsg(string.format("欢迎新成员 %s 加入联盟。", USER.GetUserNickById(roleId)))
	end
end

--获取level及以上级别的成员列表
function clsUnion:GetListByLevel(level)
	local list = {}
	for k, v in pairs(self:GetListByType("member")) do
		if v.identity <= level then
			table.insert(list, k)
		end
	end
	return list
end

local updateFuncList = {
	Lv = function(union)
		local sData = {}
		sData.Lv = union:GetLv()
		sData.needWealth = GetUnionInfoByLv(sData.Lv).wealth
		sData.wealth = union:GetWealth()
		sData.max = union:GetMaxMemberNum()
		return sData
	end,
	leader = function(union)
		local sData = {}
		sData.leader = USER.GetUserNickById(union:GetLeader())
		return sData
	end,
	apply = function()
		return {newApply = 1}
	end,
}
--联盟更新信息
function clsUnion:SendUnionUpdate(type, list)
	local updateList = list
	if not updateList then
		updateList = {}
		for roleId in pairs(self:GetListByType("member")) do
			table.insert(updateList, roleId)
		end
	end
	local sData = updateFuncList[type](self)
	for _, v in pairs(updateList) do
		local con = LOGIN.GetConnection(v)
		if con and con.roleId == v then
			con:SendProtocol("c_union_update", sData)
		end
	end

end
--获取联盟活动列表
function clsUnion:GetUnionActList()
	local sData = {}
	for k, v in pairs(GetAllUnionActList()) do
		local buff = {}
		buff.actId = k
		buff.name = v.name
		buff.describe = v.describe
		buff.needUnionLv = v.needLv
		local act = self:GetUnionAct(v.type)
		buff.state = act and act:GetState() or ACT_INVALID
		table.insert(sData, buff)
	end
	return sData
end
--联盟活动
function clsUnion:SetUnionAct(type, act)
	self.__temp.act[type] = act
end

function clsUnion:GetUnionAct(type)
	return self.__temp.act[type]
end

function clsUnion:GetUniActList()
	return self.__temp.act
end

--自动开启联盟活动
function clsUnion:CheckUnionAct()
	for k, v in pairs(GetAllUnionActList()) do
		if v.needLv <= self:GetLv() then
			if not self:GetUnionAct(v.type) then
				self:SetUnionActOpen(v.type)
			end
		end
	end
end

--开启联盟活动
function clsUnion:SetUnionActOpen(type)
	local class = GetActClassById(type)
	if not class then
		return
	end
	local act = class:New()
	act:InitUniAct(self:GetId())
	self:SetUnionAct(type, act)
end

function clsUnion:SetInviteTimes(data)
	self.__save.info = self.__save.info or {}
	if not self.__save.info.timestamp or CalDiffDays(self.__save.info.timetamp, os.time()) ~= 0 then
		self.__save.info.times = 0
	end
	self.__save.info.times = math.max((self.__save.info.times or 0) + data, 0)
	self.__save.info.timestamp = os.time()
end

function clsUnion:GetInviteTimes()
	self.__save.info = self.__save.info or {}
	if not self.__save.info.timestamp or CalDiffDays(self.__save.info.timestamp, os.time()) ~= 0 then
		self.__save.info.times = 0
	end
	return self.__save.info.times
end

function clsUnion:GetSaveData()
	self.__save.act = {}
	for k, v in pairs(GetAllUnionActList()) do
		local act = self:GetUnionAct(v.type)
		if act then
			self.__save.act[v.type] = act:GetSaveData()
		end
	end
	return table.deepcopy(self.__save)
end


function clsUnion:Restore(data)
	self.__save = data
	for k, v in pairs(self.__save.act or {}) do
		local class = GetActClassById(k)
		local act = class:New()
		act:Restore(v)
		self.__temp.act[k] = act
	end
end

function clsUnion:GetCallOut()
	return self.__temp.callout
end

function clsUnion:GetDelateCallId()
	return self.__temp.callout.delateId
end

function clsUnion:BeginDelate(roleId)
	self.__save.delate = {}
	self.__save.delate.roleId = roleId
	self.__save.delate.endtime = os.time() + RECONSIDERATION_TIMELONG
	self.__temp.callout.delateId = CALLOUT.ObjCallOut(self:GetCallOut(), clsUnion.DelateLeader, RECONSIDERATION_TIMELONG, self)
	self:AddDelateMember(roleId)
	local nick = USER.GetUserNickInfo(roleId, "nick")
	self:SendUnionMsg(string.format("【%s】对盟主发起了弹劾", nick))
	MAIL.SendSysMail(self:GetLeader(), {title = "联盟弹劾", content = string.format("由于您长时间未登陆游戏，联盟成员【%s】对您发起了弹劾。", nick)})
end

--添加一个弹劾者
function clsUnion:AddDelateMember(roleId)
	self.__save.delate = self.__save.delate or {}
	self.__save.delate.list = self.__save.delate.list or {}
	self.__save.delate.list[roleId] = 1
end

function clsUnion:GetDelateInfo()
	return self.__save.delate
end

--是否已经参与弹劾
function clsUnion:IsRoleJoinDelate(roleId)
	self.__save.delate = self.__save.delate or {}
	self.__save.delate.list = self.__save.delate.list or {}
	return self.__save.delate.list[roleId]
end

function clsUnion:RemoveDelateRole(roleId)
	self.__save.delate = self.__save.delate or {}
	self.__save.delate.list = self.__save.delate.list or {}
	self.__save.delate.list[roleId] = nil
end

function clsUnion:DelateLeader()
	local info = self:GetDelateInfo()
	local msg = "弹劾人数不足，弹劾失败"
	if table.size(info.list) >= math.ceil(self:GetMemberNum() / 2) then
		self:ModMemberIdentity(self:GetLeader(), ELITE)
		self:ModMemberIdentity(info.roleId, LEADER)
		msg = string.format("弹劾结束，发起者【%s】成为新任盟主", USER.GetUserNickInfo(info.roleId, "nick"))
	end
	self.__save.delate = nil
	self.__temp.callout.delateId = nil
	self:SendUnionMsg(msg)
end

--撤销弹劾
function clsUnion:CancelDelate(flag)
	CALLOUT.RemoveCallOut(self:GetCallOut(), self:GetDelateCallId())
	local info = self:GetDelateInfo()
	local roleId = info.roleId
	local msg = string.format("盟主【%s】已撤销弹劾，您发起的弹劾失败了。", USER.GetUserNickInfo(self:GetLeader(), "nick"))
	self:SendUnionMsg("弹劾已撤销")
	if flag then
		msg = "您已被移出联盟，发起的弹劾已自动关闭"
	end
	MAIL.SendSysMail(info.roleId, {title = "弹劾撤销", content = msg})
	self.__save.delate = nil
	self.__temp.callout.delateId = nil
--	self:SaveUnionData()
end

function clsUnion:CheckDelateInfo()
	local info = self:GetDelateInfo()
	if not info then
		return
	end
	if info.endtime <= os.time() then
		self:DelateLeader()
	else
		self.__temp.callout.delateId = CALLOUT.ObjCallOut(self:GetCallOut(), clsUnion.DelateLeader, info.endtime - os.time(), self)
	end
end

function clsUnion:FormatDelateInfo()
	local info = self:GetDelateInfo()
	local sData = {}
	if info then
		sData.endtime = info.endtime - os.time()
		sData.leaderNick = USER.GetUserNickInfo(self:GetLeader(), "nick")
		sData.nick = USER.GetUserNickInfo(info.roleId, "nick")
		sData.totalNum = self:GetMemberNum()
		sData.supplyNum = table.size(info.list)
	end
	return sData
end

function clsUnion:SaveUnionData()
	local buf = self:GetSaveData()
	buf._id = self:GetId()
	DATA.UpdateData(UNION.GetDataBase(), {_id = self:GetId()}, buf)
end

function clsUnion:Destroy()
	CALLOUT.RemoveAll(self:GetCallOut())
	Super(clsUnion).Destroy(self)
end

function clsUnion:FormatList(type)
	local list = {}
	for k, v in pairs(self:GetListByType(type)) do
		local nick = USER.GetUserNickInfo(k)
		table.insert(list)
	end
	return list
end

function __init__()
	
	local EXPLIST = Import("autocode/unionExp.lua")
	UnionExpList = EXPLIST.GetTable()
	
	local ACTLIST = Import("autocode/unionActList.lua")
	UnionActInfoList = ACTLIST.GetTable()
end
