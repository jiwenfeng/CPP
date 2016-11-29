local BUY_TIMES_BASE_COST = 2	--购买组队副本进入次数所需元宝基数

--组队副本
function GetFormatTeamCopyList(Lv)
	local sData = {}
	local list = CLSCOPY.GetCopyList()
	for k, v in pairs(list) do
		if v.type == "team" then
			local buff ={}
			buff.copyId = k
			buff.needLv = v.needLv
			buff.name = v.name
			buff.picId = v.picId
			if Lv >= v.needLv then
				buff.state = CLSCOPY.COPY_OPEN
			else
				buff.state = CLSCOPY.COPY_CLOSE
			end
			table.insert(sData, buff)
		end
	end
	return sData
end

function GetCopyDetail(copyId)
	local info = CLSCOPY.GetCopyInfoById(copyId)
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

local s_teamCopy_createTeam = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetTeam() then
		return ERROR.TipMsg(connection, "您已经创建队伍")
	end
	local team = CLSTEAMCOPY.NewTeam(connection.roleId)
	user:SetTeam(team)
	connection:SendProtocol("c_teamCopy_createTeam", "true")
end

local s_teamCopy_friendInvite = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有该用户")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local team = user:GetTeam()
	if not team then
		return ERROR.TipMsg(connection, "您还没有建立队伍")
	end
	local friend = user:GetFriendList()
	if not friend:IsInMemberList("friend", roleId) then
		return ERROR.TipMsg(connection, "该玩家不是您的好友")
	end
	if not USER.GetOnlineUser(roleId) then
		return ERROR.TipMsg(connection, "该好友不在线，不能邀请")
	end
	if team:IsUserInList(roleId) then
		return ERROR.TipMsg(connection, "该玩家已经在您的队伍中")
	end
	local ret, msg = team:AddMember(roleId)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_teamCopy_friendInvite", data)
end

local s_teamCopy_autoInvite = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local team = user:GetTeam()
	if not team then
		return ERROR.TipMsg(connection, "您还没有建立队伍")
	end
	local ret, msg = team:AutoAddMember()
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_teamCopy_autoInvite", "true")
end

local s_teamCopy_lineup = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local team = user:GetTeam()
	if not team then
		return ERROR.TipMsg(connection, "您当前没有队伍")
	end
	local sData = team:FormatLineup()
	connection:SendProtocol("c_teamCopy_lineup", sData)
end

local s_teamCopy_member = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建队伍。")
	end
	local function call_back(sData)
		connection:SendProtocol("c_teamCopy_member", sData)
	end
	teamObj:GetFormatMemberList(call_back)
end

local s_teamCopy_kick = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	local form = teamObj:GetForm()
	local grid = form:GetGrid(data)
	if not grid then
		return ERROR.TipMsg(connection, "无效的位置")
	end
	if grid:GetRoleId() == connection.roleId then
		return ERROR.TipMsg(connection, "您不能对自己执行该操作。")
	end
	grid:SetRoleId()
	connection:SendProtocol("c_teamCopy_kick", data)
end

local s_teamCopy_switch = function(connection, data)
	if type(data) ~= "table" or not data.src or not data.dest then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前没有创建组队副本。")
	end
	local ret, msg = teamObj:Switch(data.src, data.dest)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_teamCopy_switch", data)
end

local s_teamCopy_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local teamObj = user:GetTeam()
	if not teamObj then
		return ERROR.TipMsg(connection, "您当前不在组队副本中。")
	end
	if connection.roleId ~= teamObj:GetOwner() then
		return ERROR.TipMsg(connection, "您不是队长，无权进行该操作。")
	end
	teamObj:Destroy()
	user:SetTeam()
	connection:SendProtocol("c_teamCopy_leave", "true")
end

local s_teamCopy_buy = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local curTime = user:GetDailyTeamCopyBuyTimes()
	if curTime >= user:GetDailyTeamCopyMaxBuyTimes() then
		return ERROR.TipMsg(connection, "您今日的购买次数已经用完。")
	end
	local cost = (curTime + 1) * BUY_TIMES_BASE_COST
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "您元宝数量不足")
	end
	user:SetTotalIngot(-cost)
	user:SetDailyTeamCopyMax(1)
	user:SetDailyTeamCopyBuyTimes(1)
	connection:SendProtocol("c_teamCopy_buy", {buyTimes = user:GetDailyTeamCopyBuyTimes(), left = user:GetDailyTeamCopyMax() - user:GetDailyTeamCopyNum()})
end

local s_teamCopy_info = function(connection, data)
	data = tostring(data)
	if not CLSCOPY.GetCopyInfoById(data) then
		return ERROR.TipMsg(connection, "没有指定的副本信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetCopyDetail(data)
	connection:SendProtocol("c_teamCopy_info", sData)
end

local s_teamCopy_fight = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetWorkState() == "fighting" then
		return ERROR.TipMsg(connection, "您正在进行战斗")
	end
	local copy = user:GetCurCopy() 
	if not copy or copy:GetType() ~= "team" or not copy:GetScene() then
		return ERROR.TipMsg(connection, "当前不在组队副本中")
	end
	copy:ProFight()
end


function __init__()
	PROTOCOL.s_teamCopy_list = s_teamCopy_list
	PROTOCOL.s_teamCopy_info = s_teamCopy_info
	PROTOCOL.s_teamCopy_createTeam = s_teamCopy_createTeam
	PROTOCOL.s_teamCopy_lineup = s_teamCopy_lineup 
	PROTOCOL.s_teamCopy_member = s_teamCopy_member
	PROTOCOL.s_teamCopy_leave = s_teamCopy_leave
	PROTOCOL.s_teamCopy_kick = s_teamCopy_kick
	PROTOCOL.s_teamCopy_switch = s_teamCopy_switch
	PROTOCOL.s_teamCopy_friendInvite = s_teamCopy_friendInvite
	PROTOCOL.s_teamCopy_autoInvite = s_teamCopy_autoInvite
	PROTOCOL.s_teamCopy_buy = s_teamCopy_buy
	PROTOCOL.s_teamCopy_fight = s_teamCopy_fight
end
