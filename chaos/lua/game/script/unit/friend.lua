--好友系统
local FavorExpList = {}

local SINGLE = 0
local DOUBLE = 1
local MAX_LV = 10
local DAILY_STAY_ADD_FAVOR_TIMES = 1		--每日协防增加好感次数

function GetFavorLvInfo(lv)
	return FavorExpList[lv]
end

function GetFavorLvMax()
	return #FavorExpList
end

--修改双方好感度
function ModUserFavor(user, tRoleId, data)
	local friendList = user:GetFriendList()
	if friendList:GetFriendInfo(tRoleId, "type") == DOUBLE then
		friendList:ModFavor(tRoleId, data)
		local tuser = USER.GetOnlineUser(tRoleId)
		if tuser then
			local tFriendList = tuser:GetFriendList()
			tFriendList:ModFavor(user:GetRoleId(), data)
		else
			OFFLINE.SetOfflineData(tRoleId, "modfavor", user:GetRoleId(), data)
		end
	end
end

-------------------------------------------
clsFriend = clsObject:Inherit()

function clsFriend:__init__()
	self.__save = {}
	self:initsavedata()
end

function clsFriend:initsavedata()
	self.__save.friend = {}
	self.__save.black = {}
	self.__save.apply = {}
end

function clsFriend:InitFriend(owner)
	self:SetOwner(owner)
end

function clsFriend:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsFriend:RestoreData(data)
	self.__save = data
end

function clsFriend:SetOwner(data)
	self.__save.owner = data
end

function clsFriend:GetOwner()
	return self.__save.owner
end

function clsFriend:GetMemberList(type)
	return self.__save[type]
end

function clsFriend:ClearMemberList(type)
	self.__save[type] = {}
end

function clsFriend:IsInMemberList(type, roleId)
	return self.__save[type][roleId]
end

function clsFriend:AddMemberToList(type, roleId, data)
	self.__save[type][roleId] = data
end

function clsFriend:RemoveMemFromList(type, roleId)
	self.__save[type][roleId] = nil
end


function clsFriend:ClearFavor(roleId)
	local info = self:IsInMemberList("friend", roleId)
	if not info then
		return
	end
	info.favor = 0
	info.favorLv = 0
end

function clsFriend:SetStayFavor(roleId, data)
	local info = self:IsInMemberList("friend", roleId)
	if not info or info.type ~= DOUBLE then
		return
	end
	info.stayFavor = info.stayFavor or {}
	if not info.stayFavor.time or CalDiffDays(info.stayFavor.time, os.time()) ~= 0 then
		if info.stayFavor.num < DAILY_STAY_ADD_FAVOR_TIMES then
			self:ModFavor(roleId, data)
			local tuser = USER.GetOnlineUser(roleId)
			if tuser then
				local tFriendList = tuser:GetFriendList()
				tFriendList:ModFavor(self:GetOwner(), data)
			else
				OFFLINE.SetOfflineData(roleId, "modfavor", self:GetOwner(), data)
			end
			info.stayFavor.num = info.stayFavor.num + 1
			info.stayFavor.time = os.time()
		end
	end	
end

function clsFriend:ModFavor(roleId, data)
	local info = self:IsInMemberList("friend", roleId)
	if not info or info.type ~= DOUBLE then
		return
	end
	data = tonumber(data)
	info.favor = math.max(info.favor + data, 0)
	while true do
		local lvInfo = GetFavorLvInfo(info.favorLv)
		if not lvInfo then
			print("ModFavor Error!", info.favorLv)
		end
		if info.favor >= lvInfo.expNeed then
			if info.favorLv < GetFavorLvMax() then
				info.favorLv = info.favorLv + 1
				info.favor = math.max(info.favor - lvInfo.expNeed, 0)
			else
				info.favor = lvInfo.expNeed
				break
			end
		else
			break
		end
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:CheckAchvState("friend")
end

function clsFriend:GetNumByFavor(val)
	local num = 0
	for k, v in pairs(self:GetMemberList("friend")) do
		local sum = 0
		for i = 1, v.favorLv - 1, 1 do
			local info = GetFavorLvInfo(i)
			sum = sum + info.expNeed
		end
		sum = sum + v.favor
		if sum >= val then
			num = num + 1
		end
	end
	return num
end

function clsFriend:ModFriendInfo(roleId, key, val)
	local info = self:IsInMemberList("friend", roleId)
	if not info then
		return
	end
	if key == "favor" then
		return self:ModFavor(roleId, val)
	end
	info[key] = val
	if key == "type" then
		if val == SINGLE then
			self:ClearFavor(roleId)
		elseif val == DOUBLE then
			info.favorLv = math.max(info.favorLv, 1)
		end
	end
end

function clsFriend:GetFriendInfo(roleId, key)
	local info = self:IsInMemberList("friend", roleId)
	if not info then
		return
	end
	return info[key]
end

--添加好友
function clsFriend:MakeFriend(connection, roleId)
	if self:IsInMemberList("black", roleId) then
		return ERROR.TipMsg(connection, "该玩家已经在您的黑名单中了。")
	end
	local owner = self:GetOwner()
	local buff = {}
	buff.favor = 0
	buff.favorLv = 0
	buff.type = SINGLE
	buff.timestamp = os.time()
	local user = USER.GetOnlineUser(roleId)
	if user then
		buff.Lv = user:GetLv()
		buff.status = user:GetStatusLv()
		local friend = user:GetFriendList()
		local isInList = friend:IsInMemberList("friend", owner)
		if isInList then
			self:SendFriendMsg(roleId, 1)
			buff.type = DOUBLE
			buff.favorLv = 1
			friend:ModFriendInfo(owner, "type", DOUBLE)
		else
			friend:AddMemberToList("apply", owner, self:GetApplyInfo())
			self:SendFriendMsg(roleId, 0)
		end
		if self:IsInMemberList("apply", roleId) then
			self:RemoveMemFromList("apply", roleId)
		end
		self:AddMemberToList("friend", roleId, buff)
		connection:SendProtocol("c_friend_add", self:GetFormatedInfo("friend", roleId))
	else
		local function call_back(Data, msg)
			Data = UnSerialize(Data)
			if not Data or not Data[1] then
				return ERROR.TipMsg(connection, "该玩家已经离线。")
			end
			buff.Lv = Data[1].Lv
			buff.status = Data[1].status
			OFFLINE.SetOfflineData(roleId, "friendApply", owner, self:GetApplyInfo())
			self:AddMemberToList("friend", roleId, buff)
			if self:IsInMemberList("apply", roleId) then
				self:RemoveMemFromList("apply", roleId)
			end
			connection:SendProtocol("c_friend_add", self:GetFormatedInfo("friend", roleId))
		end
		if not USER.ReadUserInfo(roleId, call_back) then
			return ERROR.TipMsg(connection, "没有该玩家的数据。")
		end
	end
end

--删除好友
function clsFriend:RemoveFriend(roleId)
	local info = self:IsInMemberList("friend", roleId)
	if not info then
		return
	end
	local user = USER.GetOnlineUser(roleId)	--清除对方对自己的好感度
	if user then
		local friend = user:GetFriendList()
		friend:ModFriendInfo(self:GetOwner(), "type", SINGLE)
	else
		OFFLINE.SetOfflineData(roleId, "clearFavor", self:GetOwner())
	end
	self:RemoveMemFromList("friend", roleId)
end

--添加黑名单
function clsFriend:AddToBlack(connection, roleId)
	if self:IsInMemberList("friend", roleId) then
		self:RemoveFriend(roleId)
	end
	local buff = {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		buff.Lv = user:GetLv()
		buff.status = user:GetStatusLv()
		self:AddMemberToList("black", roleId, buff)
		
		if self:IsInMemberList("apply", roleId) then
			self:RemoveMemFromList("apply", roleId)
		end
		connection:SendProtocol("c_black_add", self:GetFormatedInfo("black", roleId))
	else
		local function call_back(Data, ErrMsg)
			Data = UnSerialize(Data)
			if not Data or not Data[1] then
				return ERROR.TipMsg(connection, "该玩家已经离线。")
			end
			buff.Lv = Data[1].Lv
			buff.status = Data[1].status
			self:AddMemberToList("black", roleId, buff)
			
			if self:IsInMemberList("apply", roleId) then
				self:RemoveMemFromList("apply", roleId)
			end
			connection:SendProtocol("c_black_add", self:GetFormatedInfo("black", roleId))
		end
		if not USER.ReadUserInfo(roleId, call_back) then
			return ERROR.TipMsg(connection, "没有该玩家的数据。")
		end
	end
end

function clsFriend:SendFriendMsg(roleId, inList)
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_friend_new", {inList = inList or 0, nick = USER.GetUserNickById(self:GetOwner())})
	end
end
--获取申请信息
function clsFriend:GetApplyInfo()
	local owner = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	sData.roleId = self:GetOwner()
--	sData.headId = self:GetHeadId()
	sData.Lv = owner:GetLv()
	sData.status = owner:GetStatusLv()
	return sData
end

--刷新列表
function clsFriend:RefreshMemberList(type)
	for k, v in pairs(self.__save[type]) do 
		local user = USER.GetOnlineUser(k)
		if user then
			v.Lv = user:GetLv()
			v.status = user:GetStatusLv()
			if type == "friend" then
				local friend = user:GetFriendList()
				if friend:IsInMemberList("friend", self:GetOwner()) then
					v.type = DOUBLE
					v.favorLv = 1
					friend:ModFriendInfo(self:GetOwner(), "type", DOUBLE)
				else
					v.type = SINGLE
					self:ClearFavor(k)
				end
			end
		end
	end
end

--------------------------------------
clsUser = USERBASE.clsUser

function clsUser:GetFriendList()
	if not self.__temp.friend then
		self:InitFriendList()
	end
	return self.__temp.friend
end

function clsUser:InitFriendList()
	local friendObj = FRIEND.clsFriend:New()
	friendObj:InitFriend(self:GetRoleId())
	self.__temp.friend = friendObj
end
--保存好友数据
function clsUser:SaveFriendData()
	local friendObj = self:GetFriendList()
	if friendObj then
		self.__save.friend = friendObj:GetSaveData()
	end
end
--恢复好友数据
function clsUser:RestoreFriendData()
	local friendData = self.__save.friend
	if friendData and friendData.owner then
		local friendObj = FRIEND.clsFriend:New()
		friendObj:RestoreData(friendData)
		self.__temp.friend = friendObj
	end
end

function clsUser:ClearFriendFavor(roleId)
	local friend = self:GetFriendList()
	if friend:IsInMemberList("friend", roleId) then
		friend:ModFriendInfo(roleId, "type", SINGLE)
		friend:ClearFavor(roleId)
	elseif friend:IsInMemberList("apply", roleId) then
		friend:RemoveMemFromList("apply", roleId)
	end
end

function clsUser:ModFriendFavor(roleId, data)
	local friend = self:GetFriendList()
	friend:ModFavor(roleId, data)
end

function clsUser:AddFriendApply(roleId, data)
	local friend = self:GetFriendList()
	if not friend:IsInMemberList("friend", roleId) then
		friend:AddMemberToList("apply", roleId, data)
	end
end

function clsUser:GetFavorLv(roleId)
	local friendList = self:GetFriendList()
	friendList:GetFavorLv(roleId)
end

function clsUser:IsFriend(roleId)
	local friendList = self:GetFriendList()
	return friendList:IsInMemberList("friend", roleId)
end
--双向好友
function clsUser:IsDoubleFriend(roleId)
	local friendList = self:GetFriendList()
	return friendList:GetFriendInfo(roleId, "type") == DOUBLE
end

function clsUser:IsInBlack(roleId)
	local friendList = self:GetFriendList()
	return friendList:IsInMemberList("black", roleId)
end

function clsUser:GetMaxFriendNum()
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_FRIEND_MEMBER_NUM)
end
--------------------------------------

function clsFriend:GetFormatedInfo(type, roleId)
	local info = self:IsInMemberList(type, roleId)
	local buff = {}
	buff.nick = USER.GetUserNickById(roleId)
	buff.headId = USER.GetUserHeadById(roleId)
	buff.vipLv = USER.GetUserNickInfo(roleId, "vipLv")
	if type ~= "apply" then
		buff.favor = info.favor or 0
		buff.favorLv = info.favorLv or 0
		local lvInfo = GetFavorLvInfo(buff.favorLv)
		buff.favorMax = lvInfo and lvInfo.expNeed or 0
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		buff.state = 1
		buff.Lv = user:GetLv()
		buff.status = user:GetStatusLv()
	else
		buff.state = 0
		buff.Lv = info.Lv
		buff.status = info.status
	end
	return buff
end


function clsFriend:GetFormatedList(type)
	local sData = {}
	for k in pairs(self:GetMemberList(type)) do
		table.insert(sData, self:GetFormatedInfo(type, k))
	end
	return sData
end

local s_friend_getList = function(connection, data)
	if data ~= "friend" and data ~= "black" and data ~= "apply" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local friend = user:GetFriendList()
	local sData = {type = data}
	sData.list = friend:GetFormatedList(data)
	if data == "apply" then
		friend:ClearMemberList(data)
	end
	connection:SendProtocol("c_friend_getList", sData)
end

local s_friend_add = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "您不能添加自己为好友。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local friend = user:GetFriendList()
	if friend:IsInMemberList("friend", roleId) then
		return ERROR.TipMsg(connection, "该玩家已经在您的好友名单中了。")
	end
	if table.size(friend:GetMemberList("friend")) >= user:GetMaxFriendNum() then
		return ERROR.TipMsg(connection, "您的好友数量已经达到上限。")
	end
	friend:MakeFriend(connection, roleId)
	user:CheckAchvState("friend")
end

local s_friend_del = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local friend = user:GetFriendList()
	if not friend:IsInMemberList("friend", roleId) then
		return ERROR.TipMsg(connection, "该玩家不是您的好友。")
	end
	friend:RemoveFriend(roleId)
	connection:SendProtocol("c_friend_del", data)
end

local s_black_add = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "您不能添加自己到黑名单中。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local friend = user:GetFriendList()
	if friend:IsInMemberList("black", roleId) then
		return ERROR.TipMsg(connection, "该玩家已经在您的黑名单中了。")
	end
	if friend:IsInMemberList("friend", roleId) then
		PROTOCOL.s_friend_del(connection, data)
	end
	return friend:AddToBlack(connection, roleId)
end

local s_black_del = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local friend = user:GetFriendList()
	if not friend:IsInMemberList("black", roleId) then
		return ERROR.TipMsg(connection, "该玩家不在您的黑名单列表中。")
	end
	friend:RemoveMemFromList("black", roleId)
	connection:SendProtocol("c_black_del", data)
end


function __init__()
	
	local EXPLIST = Import("autocode/favorExp.lua")
	FavorExpList = EXPLIST.GetTable()
	
	PROTOCOL.s_friend_getList = s_friend_getList
	
	PROTOCOL.s_friend_add = s_friend_add
	PROTOCOL.s_friend_del = s_friend_del
	
	PROTOCOL.s_black_add = s_black_add
	PROTOCOL.s_black_del = s_black_del
end
