--组队副本
local AllTeamObjList = {}		--组队副本对象列表
local teamIndex = 0				--副本id编号
local BUY_TIMES_BASE_COST = 2	--购买组队副本进入次数所需元宝基数
local TEAM_COPY_COST_VIGOUR = 10

function GetNewTeamIndex()
	teamIndex = teamIndex + 1
	return teamIndex
end

function GetAllTeamObjList()
	return AllTeamObjList
end

function GetTeamObjById(id)
	return AllTeamObjList[id]
end

function SetTeamObjById(id, obj)
	AllTeamObjList[id] = obj
end

function NewTeam(owner)
	local index = GetNewTeamIndex()
	local teamObj = CLSTEAM.clsTeam:New()
	teamObj:InitCopy(index, owner)
	SetTeamObjById(index, teamObj)
	return teamObj
end


function DestroyTeam(id)
	local teamObj = GetTeamObjById(id)
	for k, v in pairs(teamObj:GetMemberList()) do
		if v.roleId then
			teamObj:DelMember(v.roleId)
		end
	end
	teamObj:Destroy()
	SetTeamObjById(id)
end

function CheckEmptyTeam()
	for k, v in pairs(GetAllTeamObjList()) do
		if v:IsTeamEmpty() then
			DestroyTeam(k)
		end
	end
end


function NewCopy(copyId, owner)
	local copyObj = CLSTEAMCOPY.clsCopy:New()
	copyObj:InitCopy(copyId, owner)
	return copyObj
end

function DestroyCopy(id)
	local copyObj = GetTeamObjById(id)
	for k, v in pairs(copyObj:GetMemberList()) do
		if v.roleId then
			copyObj:DelMember(v.roleId)
		end
	end
	copyObj:Destroy()
	SetTeamObjById(id)
end

function GetFormatTeamCopyList(Lv)
	local sData = {}
	local list = BASECOPY.GetCopyInfoList("team")
	for k, v in pairs(list) do
		local buff ={}
		buff.copyId = k
		buff.needLv = v.needLv
		buff.name = v.name
		buff.picId = v.picId
		if Lv >= v.needLv then
			buff.state = BASECOPY.COPY_OPEN
		else
			buff.state = BASECOPY.COPY_CLOSE
		end
		table.insert(sData, buff)
	end
	return sData
end

function GetCopyDetail(copyId)
	local info = BASECOPY.GetCopyInfoById(copyId)
	local sData = {}
	sData.copyId = copyId
	sData.needLv = info.needLv
	sData.describe = info.describe
	sData.showList = {}
	for _, v in ipairs(info.showList) do
		table.insert(sData.showList, {goodsId = v, num = 1})
	end
	return sData
end




local s_teamCopy_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.left = user:GetDailyTeamCopyMax() - user:GetDailyTeamCopyNum()
	sData.buyTimes = user:GetDailyTeamCopyBuyTimes()
	sData.buyMax = user:GetDailyTeamCopyMaxBuyTimes()
	sData.baseCost = BUY_TIMES_BASE_COST
	sData.list = GetFormatTeamCopyList(user:GetLv())
	connection:SendProtocol("c_teamCopy_list", sData)
end

local s_teamCopy_info = function(connection, data)
	data = tostring(data)
	if not BASECOPY.GetCopyInfoById(data) then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetCopyDetail(data)
	connection:SendProtocol("c_teamCopy_info", sData)
end


local s_teamCopy_enter = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = NewTeam(connection.roleId)
	user:SetCurTeam(teamObj)
	connection:SendProtocol("c_teamCopy_enter", "true")
end

local s_teamCopy_fight = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local copy = user:GetCurCopy()
	if not copy then
		return ERROR.TipMsg(connection, "您当前没有在副本中")
	end
	if copy:GetType() ~= "team" then
		return ERROR.TipMsg(connection, "您当前不在组队副本中")
	end
	copy:ProFight()
	connection:SendProtocol("c_teamCopy_fight", data)
end

local s_teamCopy_member = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建组队副本。")
	end
	local function call_back(sData)
		connection:SendProtocol("c_teamCopy_member", sData)
	end
	teamObj:GetFormatMemberList(call_back)
end

local s_teamCopy_switch = function(connection, data)
	if type(data) ~= "table" or not data.src or not data.dest then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if not CLSTEAM.IsValidPos(data.src) or not CLSTEAM.IsValidPos(data.dest) then
		return ERROR.TipMsg(connection, "无效的位置数据。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建组队副本。")
	end
	teamObj:SwitchMember(data.src, data.dest)
	connection:SendProtocol("c_teamCopy_switch", data)
end

local s_teamCopy_friendInvite = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建组队副本。")
	end
	if not teamObj:GetNextFreePos() then
		return ERROR.TipMsg(connection, "队伍已经满员。")
	end
	local friend = user:GetFriendList()
	if not friend:IsInMemberList("friend", roleId) then
		return ERROR.TipMsg(connection, "该玩家不是您的好友，不能完成邀请。")
	end
	if teamObj:IsUserInTeam(roleId) then
		return ERROR.TipMsg(connection, "该玩家已经在队伍中")
	end
	teamObj:NewMember(roleId)
	connection:SendProtocol("c_teamCopy_friendInvite", data)
end

local s_teamCopy_autoInvite = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建组队副本。")
	end
	if not teamObj:GetNextFreePos() then
		return ERROR.TipMsg(connection, "队伍已经满员。")
	end
	teamObj:AutoInvite()
	connection:SendProtocol("c_teamCopy_autoInvite", "true")
end

local s_teamCopy_kick = function(connection, data)
	if not CLSTEAM.IsValidPos(data) then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	local info = teamObj:IsPosTaken(data)
	if not info then
		return ERROR.TipMsg(connection, "该位置是空的。")
	end
	if info.roleId == connection.roleId then
		return ERROR.TipMsg(connection, "您不能对自己执行该操作。")
	end
	teamObj:DelMember(info.roleId)
	connection:SendProtocol("c_teamCopy_kick", data)
end

local s_teamCopy_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	user:SetCurTeam()
	local teamId = teamObj:GetId()
	DestroyCopy(teamId)
	connection:SendProtocol("c_teamCopy_leave", "true")
end

local s_teamCopy_fight = function(connection, data)
	local info = BASECOPY.GetCopyInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	elseif info.type ~= "team" then
		return ERROR.TipMsg(connection, "该副本不是组队副本。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetLv() < info.needLv then
		return ERROR.TipMsg(connection, "您的等级不足，不能进入该副本。")
	end
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if teamObj:GetNextFreePos() then
		return ERROR.TipMsg(connection, "队伍人数不足，不能开始战斗。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	if user:GetDailyTeamCopyNum() >= user:GetDailyTeamCopyMax() then
		return ERROR.TipMsg(connection, "您当前的可用次数已经用完。")
	end
	if user:GetVigour() < TEAM_COPY_COST_VIGOUR then
		return ERROR.TipMsg(connection, "您的体力不足。")
	end
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "您正在进行战斗。")
	end
	user:SetVigour(-TEAM_COPY_COST_VIGOUR)
	local copyObj = NewCopy(data, connection.roleId)
	copyObj:SetTeam(teamObj)
	user:SetCurCopy(copyObj)
	copyObj:BeginFight(user)
end

local s_teamCopy_buy = function(connection, data)
	if data ~= "ingot" and data ~= "voucher" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local curTime = user:GetDailyTeamCopyBuyTimes()
	if curTime >= user:GetDailyTeamCopyMaxBuyTimes() then
		return ERROR.TipMsg(connection, "您今日的购买次数已经用完。")
	end
	local cost = (curTime + 1) * BUY_TIMES_BASE_COST
	local moneyInfo = MONEY.GetGoldInfoByType(data)
	if USERBASE.clsUser[moneyInfo.get](user) < cost then
		return ERROR.TipMsg(connection, string.format("您的%s不足，请充值。", moneyInfo.name))
	end
	USERBASE.clsUser[moneyInfo.set](user, -cost)
	user:SetDailyTeamCopyMax(1)
	user:SetDailyTeamCopyBuyTimes(1)
	connection:SendProtocol("c_teamCopy_buy", {buyTimes = user:GetDailyTeamCopyBuyTimes(), left = user:GetDailyTeamCopyMax() - user:GetDailyTeamCopyNum()})
end

local s_teamCopy_dismiss = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetCurTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	connection:SendProtocol("c_teamCopy_dismiss", data)
end

function __init__()

	PROTOCOL.s_teamCopy_list = s_teamCopy_list
	PROTOCOL.s_teamCopy_info = s_teamCopy_info
	PROTOCOL.s_teamCopy_enter = s_teamCopy_enter
	PROTOCOL.s_teamCopy_member = s_teamCopy_member
	PROTOCOL.s_teamCopy_leave = s_teamCopy_leave
	
--	PROTOCOL.s_teamCopy_choose = s_teamCopy_choose
	PROTOCOL.s_teamCopy_kick = s_teamCopy_kick
	PROTOCOL.s_teamCopy_switch = s_teamCopy_switch
	PROTOCOL.s_teamCopy_friendInvite = s_teamCopy_friendInvite
	PROTOCOL.s_teamCopy_autoInvite = s_teamCopy_autoInvite
	PROTOCOL.s_teamCopy_buy = s_teamCopy_buy
	
	PROTOCOL.s_teamCopy_fight = s_teamCopy_fight

--	CALLOUT.CallFre(CheckEmptyTeam, 30)
end
