
local VIGOUR_RESTORE_PER_TIME = 5	--每次恢复精力值
local RENAME_COST = 500				--修改名字所需元宝

local AllItemList = BASEITEM.GetAllItemList()
local AllOnlineUser = {}			--存储在线玩家数据
local FreezeList = {}

local FreezeIPList = {}

local path_freeze = "chaos_freeze"

local path_ipfreeze = "chaos_ipfreeze"

function GetAllOnlineUser()
	return AllOnlineUser
end

function SetOnlineUser(roleId, obj)
	AllOnlineUser[roleId] = obj
end

function GetOnlineUser(roleId)
	return AllOnlineUser[roleId]
end

function GetUserOnlineByPhyle(phyle)
	local list = {}
	for k, v in pairs(AllOnlineUser) do
		if v:GetPhyle() == phyle then
			table.insert(list, k)
		end
	end
	return list
end

local GenAttrTrainCost = {
	nimbus = 2000,
	ingot = 5,
}

------------------------------------------------------------------------
local AllUserNick = {}	--存储玩家昵称数据(roleId, username, nick, headId, job, vipLv, Lv, sex, unionId)

function GetAllUserNick(state)
	if not state then
		return AllUserNick
	else
		local buff = {}
		for k, v in pairs(AllUserNick) do
			if v.state == state then
				buff[k] = v
			end
		end
		return buff
	end
end

function SetAllUserNick(roleId, username, nick, headId, job, vipLv, state, sex)
	if not roleId then return end
	AllUserNick[roleId] = AllUserNick[roleId] or {}
	if username then AllUserNick[roleId].username = username end
	if nick then AllUserNick[roleId].nick = nick end
	if roleId then AllUserNick[roleId].roleId = roleId end
	if headId then AllUserNick[roleId].headId = headId end
	if job then AllUserNick[roleId].job = job end
	if vipLv then AllUserNick[roleId].vipLv = vipLv end
	if sex then AllUserNick[roleId].sex = sex end
	AllUserNick[roleId].state = state or 1
	SaveUserNick(roleId)
end

function GetRoleIdByNick(nick)
	for k, v in pairs(AllUserNick) do
		if v.nick == nick then
			return k
		end
	end
	return
end

function GetUserNickById(roleId)
	if roleId == SYSTEM_ID then
		return SYSTEM_NAME
	end
	if not AllUserNick[roleId] then
       return
    end
	return AllUserNick[roleId].nick
end

function GetUserHeadById(roleId)
	if roleId == SYSTEM_ID then
		return SYSTEM_HEADID
	end
	if not AllUserNick[roleId] then
		return
	end
	return AllUserNick[roleId].headId
end

function GetUserNameById(roleId)
	if not AllUserNick[roleId] then
		return
	end
	return AllUserNick[roleId].username
end

function GetUserUnionById(roleId)
	if not AllUserNick[roleId] then
		return
	end
	return AllUserNick[roleId].unionId
end

function GetRoleIdByUserName(username, serverId)
	for k, v in pairs(AllUserNick) do
		if v.username == username then
			if not serverId or GetServerIndexById(k) == serverId then
				return k
			end
		end
	end
	return
end

function GetJobByUserName(username)
	for k, v in pairs(AllUserNick) do
		if v.username == username then
			return v.job
		end
	end
	return
end

function GetUserNickInfo(roleId, type)
	if roleId == SYSTEM_ID then
		return
	end
	if not AllUserNick[roleId] then
		return
	end
	return AllUserNick[roleId][type]
end

function ModUserNickInfo(roleId, type, data)
	if not AllUserNick[roleId] then
		return
	end
	AllUserNick[roleId][type] = data
	SaveUserNick(roleId)
end

function SetUserFreeze(roleId, timelong, operator)
	if timelong then
		local user = GetOnlineUser(roleId)
		local Ip
		if user then
			Ip = user:GetLastLogin().ip
		end
		if timelong == 0 then
			FreezeList[roleId] = {operator = operator, ip = Ip}
		else
			FreezeList[roleId] = {endtime = os.time() + timelong, operator = operator, timglong = timglong, ip = Ip} 
		end
		DATA.SaveData(path_freeze, FreezeList, roleId)
	else
		FreezeList[roleId] = nil
		DATA.DBRemove(path_freeze, roleId)
	end
end

function IsAccountFreeze(roleId)
	local info = FreezeList[roleId]
	if not info then
		return false
	end
	if os.time() < (info.endtime() or math.huge) then
		return true
	end
	info = nil
	DATA.DBRemove(path_freeze, roleId)
	return true
end

function GetFreezeList()
	return FreezeList
end

function FreezeIP(ip, timelong, operator)
	if timelong then
		local user = GetOnlineUser(roleId)
		if timelong == 0 then
			FreezeIPList[ip] = {operator = operator}
		else
			FreezeIPList[ip] = {endtime = os.time() + timelong, operator = operator, timglong = timglong} 
		end
		DATA.SaveData(path_ipfreeze, FreezeList, roleId)
		LogoutSpecIp(ip)
	else
		FreezeIPList[roleId] = nil
		DATA.DBRemove(path_ipfreeze, roleId)
	end
end

function LogoutSpecIp(ip)
	for k, v in pairs(GetAllOnlineUser()) do
		local info = v:GetLastLogin()
		if v.ip == ip then
			EXIT.UserExit(v)
		end
	end
end

function GetFreezeInfoByIP(ip)
	return FreezeIPList[ip]
end

function GetIpFreezeList()
	return FreezeIPList
end

function IsIPFreezed(ip)
	local info = GetFreezeInfoByIP(ip)
	if not info then
		return false
	end
	if os.time() < (info.endtime() or math.huge) then
		return true
	end
	info = nil
	DATA.DBRemove(path_ipfreeze, ip)
	return true
end

---------------------------------------------------------------------
path_userData = "chaos.user"					--玩家数据保存路径
local path_userNick = "chaos.usernick"		--玩家昵称保存路径

local SaveUserList = {}
local SAVE_NUM_PER_SEC = 5			--每秒钟存储玩家数据个数
local SAVE_USER_DATA_MIN_TIME = 60 * 2		--最小刷新间隔

--错误回调记录
local function SaveErrorBack(data, errmsg)
	if errmsg then
		DATA.log("user.lua save error:" .. errmsg, "save.log")
	end
end

--保存昵称数据
function SaveUserNick(roleId, callback)
	return DATA.SaveData(path_userNick, AllUserNick, roleId, callback or SaveErrorBack)
end

function RemoveUserNickInfo(roleId)
	AllUserNick[roleId] = nil
	DATA.DBRemove(path_userNick, {_id = roleId})
end
--保存用户数据
function SaveUserData(userObj, callback)
	local roleId = userObj:GetRoleId()
	local buff = userObj:GetSaveData()
	buff._id = roleId
	if not DATA.UpdateData(path_userData, {_id = roleId}, buff, callback or SaveErrorBack) then

		DATA.log(string.format("Save %s user data failed: connect server Saved error!", roleId), "user.log")
	end
end
--读取玩家数据
function ReadUserData(roleId, callback)
	if not AllUserNick[roleId] or AllUserNick[roleId].state == 0 then
		return false
	end
	if not DATA.Read(path_userData, {_id = roleId}, callback) then
		return false
	end
	return true
end

--定时保存玩家
function GetSaveUserList()
	return SaveUserList
end

function SetRenewUserTimestamp(timestamp)
	SaveUserList.timestamp = timestamp or os.time()
end

function GetRenewUserTimestamp()
	SaveUserList.timestamp = SaveUserList.timestamp or os.time()
	return SaveUserList.timestamp
end

--刷新需要保存数据的玩家列表
function RenewSaveUserList()
--[[	if os.time() - GetRenewUserTimestamp() < SAVE_USER_DATA_MIN_TIME then		--设定最小时间，防止人少时过于频繁的刷新
		return
	end]]
	for roleId in pairs(GetAllOnlineUser()) do
		SetSaveUserState(roleId, true)
	end
	SetRenewUserTimestamp(os.time())
end

function SetSaveUserState(roleId, state)
	SaveUserList[roleId] = state
end

--存储所有在线玩家数据
function SaveAllOnlineUser()
	if table.size(GetSaveUserList()) <= 0 then
		RenewSaveUserList()
	end
	local count = 0
	for roleId in pairs(GetSaveUserList()) do
		local user = GetOnlineUser(roleId)
		if user then
			count = count + 1
			user:SetOfflineTime(os.time())
			user:SetTotalOnlineTime()
			user:SetTimeStamp()
			UpdateUserInfo(user)
		--	UpdateRankInfo(roleId)
		--	FIGHT.UpdateUserFightData(roleId)
		--	CITY.RefreshUserCityInfo(roleId)
			SaveUserData(user)
			if not EXIT.UserDataIsUsed(user) then
				EXIT.UserExit(user)
			end
		end
		SetSaveUserState(roleId)
		if count >= SAVE_NUM_PER_SEC then
			break
		end
	end
end

--关闭服务器时保存在线玩家数据，此时需要清理玩家数据中的计时器
function ShutDownSaveAll()
	for roleId, obj in pairs(GetAllOnlineUser()) do
		UpdateUserInfo(obj)
		UpdateRankInfo(roleId)
		FIGHT.UpdateUserFightData(roleId)
		obj:LeaveScene()
		obj:SetOfflineTime(os.time())
		obj:SetTotalOnlineTime()
		obj:ExitRemoveCallOut()
		SaveUserData(obj)		
		obj:Destroy()
		AllOnlineUser[roleId] = nil
	end
end
------------------------------------------------------------------------
--获取距离下一个半小时的秒数
local function GetNextHalfHour()
	local tm = os.date("*t", os.time())
	if tm.min >= 30 then
		tm.hour = tm.hour + 1
		tm.min = 0
	else
		tm.min = 30
	end
	tm.sec = 0
	return os.time(tm) - os.time()
end
--每半小时恢复精力值
function RestoreVigour(init)
	local timelong = GetNextHalfHour()
	CALLOUT.CallOut(RestoreVigour, timelong)
	if not init then
		for _, v in pairs(GetAllOnlineUser()) do
			local vigour_diff = v:GetVigourMax() - v:GetVigour()
			if vigour_diff > 0 then
				local vigour = math.min(vigour_diff, VIGOUR_RESTORE_PER_TIME)
				v:SetVigour(vigour)
				v:SetVigourRestoreTimestamp(os.time())
			end
		end
	end
end

function AddVigourBuffer()
	for _, v in pairs(GetAllOnlineUser()) do
		BUFFER.AddOneBuffer(v, "21")
		v:SetVigourBufferTimestamp(os.time())
	end
end

--[[每日重置精力值
function ResetVigour()
	for _, user in pairs(AllOnlineUser) do
		user:InitVigour()
	end
end]]

-------------------------------------------------------------------------
local path_userInfo = "chaos.userInfo"

function SaveUserInfo(roleId, data)
	local buff = data
	buff._id = roleId
	DATA.UpdateData(path_userInfo, {_id = roleId}, buff)
end

function UpdateUserInfo(user)
	local buff = user:GetUserInfo()
	buff.lineup = user:GetFormatedCurLineup()
	buff.attr = user:GetUserDetail()
	buff.detail = user:GetDetailAttr()
	buff.equip = user:GetFormatEquipList()
	SaveUserInfo(user:GetRoleId(), buff)
	UNION.RefreshUnionInfo(user)
end

function UpdateRankInfo(roleId)
	RANK.AddToRank("userLv", roleId)
	RANK.AddToRank("userForce", roleId)
	RANK.AddToRank("charm", roleId)
	RANK.AddToRank("hero", roleId)
end

function ReadUserInfo(roleId, callback)
	if not AllUserNick[roleId] then
		return false, "没有该用户信息。"
	end
	if not DATA.Read(path_userInfo, {_id = roleId}, callback) then
		return false, "连接错误，请重试。"
	end
	return true
end
------------------------------------------------------------------------------
local AllUserFocusList = {}			--玩家关注焦点

function GetAllUserFocusList()
	return AllUserFocusList
end

function GetUserFocusListByType(type)
	AllUserFocusList[type] = AllUserFocusList[type] or {}
	return AllUserFocusList[type]
end

function SetUserFocusOn(type, roleId, state)
	AllUserFocusList[type] = AllUserFocusList[type] or {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		local focus = user:GetFocus()
		if focus and focus.type then
			local focusOn = focus.id or focus.type
			if focusOn ~= type then
				if AllUserFocusList[focusOn] then
					AllUserFocusList[focusOn][roleId] = nil
				end
			end
		end
		user:SetFocus(state and type)
	end
	AllUserFocusList[type][roleId] = state
end

function IsUserFocusOn(type, roleId)
	AllUserFocusList[type] = AllUserFocusList[type] or {}
	return AllUserFocusList[type][roleId]
end

function ClearUserFocusOn(user)
	local focus = user:GetFocus()
	if focus and focus.type then
		if focus.type == "embassy" then
			local embassy = EMBASSY.GetEmbassyObjById(focus.id)
			if embassy then
				embassy:SetVisitor(user:GetRoleId())
			end
		else
			SetUserFocusOn(focus.id or focus.type, user:GetRoleId())
		end
	end
end

------------------------------------------------------------------------

--以Lv为中心，划分玩家等级区域列表
function ClassifyFightDataListByLv(Lv, num, exceptList)
	exceptList = exceptList or {}
	local LvList = {}
--	for k, v in pairs(GetAllOnlineUser()) do
--		local len = math.abs(v:GetLv() - Lv)
--		LvList[len] = LvList[len] or {}
--		table.insert(LvList[len], k)
--	end
	for k, v in pairs(GetAllUserNick()) do
		if v.Lv then
			local len = math.abs(v.Lv - Lv)
			LvList[len] = LvList[len] or {}
			table.insert(LvList[len], k)
		end
	end
	local function get_list() 
		local min = SERVER_ROLE_MAX_LV
		local list
		for k, v in pairs(LvList or {}) do
			if tonumber(k) <= min then
				list = v
				min = k
			end
		end
		return list, min
	end
	local except 
	local sData = {}
	while true do
		local buf, len = get_list(except or -1)
		if not len then
			break
		end
		while #buf > 0 do
			local index = math.random(1, #buf)
			if not exceptList[buf[index]] then
				table.insert(sData, buf[index])
				if #sData >= num then
					return sData
				end
			end
			table.remove(buf, index)
		end
		LvList[len] = nil
	end
	return sData
end

-----------------------------------------------------------------------------

------------------------------------------------------------------------------
local AttrTrainList = {}		--属性培养上限表

function GetAttrTrainUpLimit(posLv, vipLv)
	return AttrTrainList[posLv][vipLv]
end

--------------------------------------------------------------------------------
--给玩家奖励物品（背包满或不在线时发送邮件）
function AddItemToUser(roleId, id, num, param, title, content)
	local user = GetOnlineUser(roleId)
	local isFinish = false
	if user then
		local item = BASEITEM.NewItem(id, num, param)
		if user:IsItemCanAdd(USERBASE.PACKAGE, item) then
			user:AddItem(USERBASE.PACKAGE, item)
			isFinish = true
		end
	elseif not title or not content then
		OFFLINE.SetOfflineData(k, USERBASE.PACKAGE, "item", id, num, param)
		isFinish = true
	end
	if not isFinish then
		title = title or "背包已满"
		content = content or string.format("由于背包已满，您的部分物品已经通过附件的形式发送给您，请及时查收。")
		local mail = {title = title, content = content}
		mail.attach = {item = {}}
		table.insert(mail.attach.item, {id = id, num = num, lock = param.lock})
		MAIL.SendSysMail(roleId, mail)
	end
end


--------------------------------------------------------------------------------
--获取玩家基本信息
local s_user_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = user:GetUserGlory()
	sData.renameCost = RENAME_COST
	connection:SendProtocol("c_user_info", sData)
end

--查看自己/武将的战斗属性
local s_info_detail = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	if not data.roleId then
		return ERROR.TipMsg(connection, "请选择要查看的玩家。")
	end
	local user = GetOnlineUser(data.roleId)
	if user then
		local sData = {}
		if not data.heroId or data.heroId == "user" then
			sData = user:GetUserDetail()
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, string.format("没有找到该武将信息：%s。", data.heroId))
			end
			sData = hero:GetHeroDetail()
		end
		return connection:SendProtocol("c_info_detail", sData)
	else
		if not data.heroId or data.heroId == "user" then
			local function get_view(Data, msg)
				Data = UnSerialize(Data)
				if not Data or not Data[1] then
					return ERROR.TipMsg(connection, string.format("没有找到玩家%s的数据信息。", data.roleId))
				end
				local sData = Data[1].attr
				connection:SendProtocol("c_info_detail", sData)
			end
			local rst, ret = ReadUserInfo(data.roleId, get_view)
			if not rst then
				return ERROR.TipMsg(connection, ret)
			end
		else
			local hero = HERO.GetOnlineHero(data.heroId)
			if hero then
				hero:SetOnlineTimestamp(os.time())
				local sData = hero:GetHeroDetail()
				connection:SendProtocol("c_info_detail", sData)
			else
				local function call_back(ret, msg)
					if not ret then
						return ERROR.TipMsg(connection, msg)
					end
					local hero = HERO.GetOnlineHero(data.heroId)
					hero:SetOnlineTimestamp(os.time())
					local sData = hero:GetHeroDetail()
					connection:SendProtocol("c_info_detail", sData)
				end
				HERO.ReadOfflineHeroData(data.heroId, call_back)				
			end
		end
	end
end

--查看自己/武将的详细属性
local s_attr_detail = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	if not data.roleId then
		return ERROR.TipMsg(connection, "请选择要查看的玩家。")
	end
	local user = GetOnlineUser(data.roleId)
	if user then
		local sData = {}
		if not data.heroId or data.heroId == "user" then
			sData = user:GetDetailAttr()
		else
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, string.format("没有找到该武将信息：%s。", data.heroId))
			end
			sData = hero:GetDetailAttr()
		end
		return connection:SendProtocol("c_attr_detail", sData)
	else
		if not data.heroId or data.heroId == "user" then
			local function get_view(Data, msg)
				Data = UnSerialize(Data)
				if not Data or not Data[1] then
					return ERROR.TipMsg(connection, string.format("没有找到玩家%s的数据信息。", data.roleId))
				end
				local sData = Data[1].detail
				connection:SendProtocol("c_attr_detail", sData)
			end
			local rst, ret = ReadUserInfo(data.roleId, get_view)
			if not rst then
				return ERROR.TipMsg(connection, ret)
			end
		else
			local hero = HERO.GetOnlineHero(data.heroId)
			if hero then
				hero:SetOnlineTimestamp(os.time())
				local sData = hero:GetDetailAttr()
				connection:SendProtocol("c_attr_detail", sData)
			else
				local function call_back(ret, msg)
					if not ret then
						return ERROR.TipMsg(connection, msg)
					end
					local hero = HERO.GetOnlineHero(data.heroId)
					hero:SetOnlineTimestamp(os.time())
					local sData = hero:GetDetailAttr()
					connection:SendProtocol("c_attr_detail", sData)
				end
				HERO.ReadOfflineHeroData(data.heroId, call_back)
			end
		end
	end
end

--查看自己/武将的技能
local s_skill_list = function(connection, data)
	if not data.roleId then
		return ERROR.TipMsg(connection, "请选择要查看的玩家。")
	end
	local user = assert(GetOnlineUser(data.roleId))
	local obj = user
	if data.heroId and data.heroId ~= "user" then
		local hero = user:GetHero(data.heroId)
		if not hero then
			return ERROR.TipMsg(connection, string.format("没有找到该武将信息：%s。", data.heroId))
		end
		obj = hero
	end
	local sData = {roleId = data.roleId, heroId = heroId}
	sData.point = obj:GetSkillPoint()
	sData.list = obj:GetFormatSkillList()
	return connection:SendProtocol("c_skill_list", sData)
end
--查看自己/武将的装备
local s_equip_list = function(connection, data)
	if not data.roleId then
		return ERROR.TipMsg(connection, "请选择要查看的玩家。")
	end
	local sData = {roleId = data.roleId, heroId = data.heroId}
	local user = GetOnlineUser(data.roleId)
	if user then
		local obj = user
		if data.heroId and data.heroId ~= "user" then
			local hero = user:GetHero(data.heroId)
			if not hero then
				return ERROR.TipMsg(connection, string.format("没有找到该武将信息：%s。", data.heroId))
			end
			obj = hero
		end		
		sData.list = obj:GetFormatEquipList()
		return connection:SendProtocol("c_equip_list", sData)
	else
		if not data.heroId or data.heroId == "user" then
			local function get_view(Data, msg)
				Data = UnSerialize(Data)
				if not Data or not Data[1] then
					return ERROR.TipMsg(connection, string.format("没有找到玩家%s的数据信息。", data.roleId))
				end
				sData.list = Data[1].equip
				connection:SendProtocol("c_equip_list", sData)
			end
			local rst, ret = ReadUserInfo(data.roleId, get_view)
			if not rst then
				return ERROR.TipMsg(connection, ret)
			end
		else
			local hero = HERO.GetOnlineHero(data.heroId)
			if hero then
				hero:SetOnlineTimestamp(os.time())
				sData.list = hero:GetFormatEquipList()
				connection:SendProtocol("c_equip_list", sData)
			else
				local function call_back(ret, msg)
					if not ret then
						return ERROR.TipMsg(connection, msg)
					end
					local hero = HERO.GetOnlineHero(data.heroId)
					hero:SetOnlineTimestamp(os.time())
					sData.list = hero:GetFormatEquipList()
					connection:SendProtocol("c_equip_list", sData)
				end
				HERO.ReadOfflineHeroData(data.heroId, call_back)
			end
		end
	end
end
--查看自己的武将列表
local s_user_heroList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {}	
	table.insert(sData, {heroId = "user", type = "user", headId = user:GetHeadId(), name = user:GetName(), Lv = user:GetLv(), aptLv = user:GetAptLv(), state = user:GetWorkState(), job = user:GetJob()})
	for k, v in pairs(user:GetHeroList()) do
		table.insert(sData, {heroId = k, type = v:GetType(), template = v:GetTemplate(), headId = v:GetHeadId(), name = v:GetName(), Lv = v:GetLv(), aptLv = CLSHERO.GetAptLvByAptitude(v:GetAptitude()), state = v:GetWorkState(), job = v:GetJob()})
	end
	connection:SendProtocol("c_user_heroList", sData)
end

local s_heroSlot_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {}	
	sData.heroMax = user:GetHeroMax()
	sData.needLv = user:GetHeroUnLockNeed("auto")
--	sData.unlockNimbus = user:GetHeroUnLockNeed("nimbus")
	sData.unlockIngot = user:GetHeroUnLockNeed("ingot")
	sData.slotMax = user:GetHeroSlotMax()
	connection:SendProtocol("c_heroSlot_list", sData)
end

local s_heroSlot_unlock = function(connection, data)
--	if data ~= "nimbus" and data ~= "ingot" then
	if data ~= "set" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", tostring(data)))
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local cost = user:GetHeroUnLockNeed("ingot")
	if not cost then
		return ERROR.TipMsg(connection, "您已经将此类位置全部解锁。")
	end
	local goldInfo = MONEY.GetGoldInfoByType("ingot")
	if USERBASE.clsUser[goldInfo.get](user) < cost then
		return ERROR.TipMsg(connection, string.format("您的%s不足，无法完成解锁。", goldInfo.name))
	end
	USERBASE.clsUser[goldInfo.set](user, -cost)
	user:SetHeroUnLockNum("ingot", 1)
	user:SetHeroMax(1)
	local sData = {}
	sData.heroMax = user:GetHeroMax()
--[[	if data == "nimbus" then
		sData.unlockNimbus = user:GetHeroUnLockNeed("nimbus")
	else]]
		sData.unlockIngot = user:GetHeroUnLockNeed("ingot")
--	end
	connection:SendProtocol("c_heroSlot_unlock", sData)
end


--修改玩家名字
local s_user_rename = function(connection, data)
	local user = assert(GetOnlineUser(connection.roleId))
	if not data or data == "" then
		return connection:SendProtocol("c_user_rename", {result = 0, nick = user:GetName(), msg = "请输入新名字。"})
	end
	local ret, msg = NEWUSER.NameIsIllegal(data)
	if ret then
		return connection:SendProtocol("c_user_rename", {result = 0, nick = user:GetName(), msg = msg})
	end
	if NAME.IsNameExist(data) then
		return connection:SendProtocol("c_user_rename", {result = 0, nick = user:GetName(), msg = "该名字已经被占用了。"})
	end
	if user:GetTotalIngot() < RENAME_COST then
		return connection:SendProtocol("c_user_rename", {result = 0, nick = user:GetName(), msg = string.format("您的元宝余额不足%d，请充值。", RENAME_COST)})
	end
	user:SetTotalIngot(-RENAME_COST)
	NAME.DelNameExist(user:GetName())
	user:SetName(data)
	NAME.SetNameExist(data)
	user:SyncData("name")
	user:Broad2ExNeighbor("rename", nil, nil, true)
	connection:SendProtocol("c_user_rename", {result = 1, nick = user:GetName()})
end

local s_attrTrain_info = function(connection, data)
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {}
	if data == "user" then
		sData = user:GetAttrTrainInfo()
	else
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "请选择要进行属性培养的武将。")
		end
		sData = hero:GetAttrTrainInfo()
		sData.heroId = data
	end
	connection:SendProtocol("c_attrTrain_info", sData)
end

local s_attrTrain_gen = function(connection, data)
	if data.type ~= "nimbus" and data.type ~= "ingot" then
		return ERROR.TipMsg(connection, "请选择属性培养的类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	if USERBASE.clsUser[MONEY.GetGoldInfoByType(data.type).get](user) < GenAttrTrainCost[data.type] then
		return ERROR.TipMsg(connection, string.format("您%s的数量不足", MONEY.GetGoldTypeName(data.type)))
	end
	local sData = {}
	if not data.heroId or data.heroId == "user" then
		sData = user:GenAttrTrain(data.type)
		if not sData then
			return ERROR.TipMsg(connection, "属性已达到上限，不可培养。")
		end
	else
		local hero = user:GetHero(data.heroId)
		if not hero then
			return ERROR.TipMsg(connection, "请选择要进行属性培养的武将。")
		end
		sData = hero:GenAttrTrain(data.type)
		if not sData then
			return ERROR.TipMsg(connection, "属性已达到上限，不可培养。")
		end
		sData.heroId = data.heroId
	end
	USERBASE.clsUser[MONEY.GetGoldInfoByType(data.type).set](user, -GenAttrTrainCost[data.type])
	connection:SendProtocol("c_attrTrain_gen", sData)
end

local s_attrTrain_save = function(connection, data)
	local user = assert(GetOnlineUser(connection.roleId))
	if data == "user" then
		if not user:GetAttrTrain() then
			return ERROR.TipMsg(connection, "请先生成新的培养属性。")
		end
		user:SaveAttrTrain()
	else
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "请选择要进行属性培养的武将。")
		end
		if not hero:GetAttrTrain() then
			return ERROR.TipMsg(connection, "请先对该武将生成新的培养属性。")
		end
		hero:SaveAttrTrain()
	end
	connection:SendProtocol("c_attrTrain_save", data)
end

local s_point_set = function(connection, data)
	if type(data) ~= "table" or table.size(data) <= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	data.strength = tonumber(data.strength) or 0
	data.intellect = tonumber(data.intellect) or 0
	data.agility = tonumber(data.agility) or 0
	data.captain = tonumber(data.captain) or 0
	local user = assert(GetOnlineUser(connection.roleId))
	if data.strength + data.intellect + data.agility + data.captain > user:GetLeftPoint() then
		return ERROR.TipMsg(connection, "您的剩余属性点不足。")
	end
	for k, v in pairs(data) do
		if v < 0 then
			return ERROR.TipMsg(connection, string.format("属性点%s为负数。", k))
		end
	end
	user:SetPoint(data.strength, data.intellect, data.agility, data.captain)
	local sData = {}
	sData.leftPoint = user:GetLeftPoint()
	sData.strength = user:GetStrength()
	sData.intellect = user:GetIntellect()
	sData.agility = user:GetAgility()
	sData.captain = user:GetCaptain()
	connection:SendProtocol("c_point_set", sData)
end

local s_point_recommend = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = user:RecommendPoint()
	connection:SendProtocol("c_point_recommend", sData)
end

--解锁格子
local s_grid_unlock = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if data.stoType ~= USERBASE.DEPOT and data.stoType ~= USERBASE.PACKAGE then
		return ERROR.TipMsg(connection, "您只能解锁背包或仓库格子。")
	end
	data.num = tonumber(data.num)
	if not data.num or data.num <= 0 then
		return ERROR.TipMsg(connection, "请选择要解锁的格子及数量。")
	end
--[[	if data.goldType ~= "ingot" and data.goldType ~= "voucher" then
		return ERROR.TipMsg(connection, string.format("只能使用%s或者%s来解锁格子。", MONEY.GetGoldTypeName("ingot"), MONEY.GetGoldTypeName("voucher")))
	end]]
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetGridMax(data.stoType) + data.num > USERBASE.GetPackageGridNum(data.stoType, "max") then
		return ERROR.TipMsg(connection, "可供解锁的格子数量不足。")
	end
	local price = user:CalUnlockPrice(data.stoType, data.num)
	data.price = tonumber(data.price)
	if data.price ~= price then
		return ERROR.TipMsg(connection, string.format("开%d个格子的费用计算错误", data.num))
	end
--[[	local goldInfo = MONEY.GetGoldInfoByType(data.goldType)
	if USERBASE.clsUser[goldInfo.get](user) < price then
		return ERROR.TipMsg(connection, string.format("您的%s不足，请充值。", goldInfo.name))
	end]]
	if user:GetTotalIngot() < price then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetGridMax(data.stoType, data.num)
	user:SetUnLockNum(data.stoType, "ingot", data.num)
--	USERBASE.clsUser[goldInfo.set](user, -price)
	user:SetTotalIngot(-price)
	local sData = {stoType = data.stoType, num = data.num}
	sData.firstCost = user:CalUnlockPrice(data.stoType, 1)
	connection:SendProtocol("c_grid_unlock", sData)
end

--装备
local s_equip_fix = function(connection, data)
	if type(data) ~= "table" or not data.areaId or not data.heroId or not data.pos then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local equip = user:GetItemByAreaId(USERBASE.PACKAGE, data.areaId)
	if not equip then
		return ERROR.TipMsg(connection, "没有指定的物品信息。")
	end
	local target = user
	if data.heroId and data.heroId ~= "user" then
		target = user:GetHero(data.heroId)
		if not target then
			return ERROR.TipMsg(connection, "没有指定的武将信息。")
		end
	end	
	local needJob = equip:GetNeedJob()
	if equip:GetEquipPos() ~= data.pos then
		return ERROR.TipMsg(connection, "该装备不能装备在此部位")
	end
--[[	if equip:GetWearTarget() ~= "all" and equip:GetWearTarget() ~= data.targetId then
		return ERROR.TipMsg(connection, "该目标无法装备此装备")
	end]]
	if needJob ~= "all" and needJob ~= JOB.GetBasicJob(target:GetJob()) then
		return ERROR.TipMsg(connection, "该职业无法装备此物品")
	end
	if equip:GetNeedLv() > target:GetLv() then
		return ERROR.TipMsg(connection, string.format("%s等级不足，无法使用该装备。", target:GetRoleType() == "user" and "您的" or "该武将"))
	end
	-- if equip:GetNeedStatus() > user:GetStatusLv() then
		-- return ERROR.TipMsg(connection, "您的仙职等级不足，无法使用该装备。")
	-- end
	target:WearEquip(data.pos, table.deepcopy(equip))
	user:DelItemByAreaId(USERBASE.PACKAGE, data.areaId, equip:GetNum())
	connection:SendProtocol("c_equip_fix", "true")
	PROTOCOL.s_equip_list(connection, {roleId = connection.roleId, heroId = data.heroId})
end

--卸下装备
local s_equip_unfix = function(connection, data)
	if type(data) ~= "table" or not data.heroId or not data.pos then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local target
	if data.heroId and data.heroId ~= "user" then
		target = user:GetHero(data.heroId)
		if not target then
			return ERROR.TipMsg(connection, "没有指定的武将信息。")
		end
	else
		target = user
	end	
	local equip = target:GetEquipByPos(data.pos)
	if not equip then
		return ERROR.TipMsg(connection, "该位置没有装备。")
	end
	if user:IsBagFull(USERBASE.PACKAGE) then
		return ERROR.TipMsg(connection, "您的背包已满。")
	end
	target:UnfixEquip(data.pos)
	connection:SendProtocol("c_equip_unfix", "true")
	PROTOCOL.s_equip_list(connection, {roleId = connection.roleId, heroId = data.heroId})
end



--查看其他玩家信息
local s_user_view = function(connection, data)
	local roleId = GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("没有找到玩家【%s】的信息", data or "nil"))
	end
	local sData = {}
	sData.nick = data
	sData.roleId = roleId
	sData.headId = GetUserHeadById(roleId)
	sData.job = GetUserNickInfo(roleId, "job")
	sData.Lv = GetUserNickInfo(roleId, "Lv")
	sData.vipLv = GetUserNickInfo(roleId, "vipLv")
	sData.sex = GetUserNickInfo(roleId, "sex")
	sData.title = GetUserNickInfo(roleId, "title")
	sData.phyle = GetUserNickInfo(roleId, "phyle")
	local unionId = GetUserNickInfo(roleId, "unionId")
	local union = UNION.GetUnionById(unionId)
	sData.unionName = union and union:GetName() or ""
	local info = ARENA.GetUserArenaInfo(roleId)
	sData.rank = info and info.rank or -1
	local user = GetOnlineUser(roleId)
	if user then
	--	sData.position = user:GetOfficialInfo()
		sData.teamForce = user:GetlineUpForce()
		return connection:SendProtocol("c_user_view", sData)
	end
	local function get_view(Data, msg)
		Data = UnSerialize(Data)
		if not Data or not Data[1] then
			return ERROR.TipMsg(connection, string.format("没有找到玩家【%s】的信息", data))
		end
		sData.teamForce = Data[1].teamForce
	--	sData.position = Data[1].position
		return connection:SendProtocol("c_user_view", sData)
	end
	local rst, ret = ReadUserInfo(roleId, get_view)
	if not rst then
		return connection:SendProtocol("c_user_view_false", ret)
	end
end

--查看其他玩家队伍列表
local s_view_lineup = function(connection, data)
	local roleId = GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("没有玩家%s的信息。", data or ""))
	end
	local user = GetOnlineUser(roleId)
	local sData = {nick = data, roleId = roleId}
	if user then
		sData.list = user:GetFormatedCurLineup()
		return connection:SendProtocol("c_view_lineup", sData)
	else
		local function get_view(Data, msg)
			Data = UnSerialize(Data)
			if not Data or not Data[1] then
				return ERROR.TipMsg(connection, string.format("没有找到玩家%s的数据信息。", data))
			end
			sData.list = Data[1].lineup
			connection:SendProtocol("c_view_lineup", sData)
		end
		local rst, ret = ReadUserInfo(roleId, get_view)
		if not rst then
			return ERROR.TipMsg(connection, ret)
		end
	end
end

-------------------------------------------------------------------

local s_setting_set = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if #data <= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	for _, v in ipairs(data) do
		if not SETTING.IsValidSetting(v.type) then
			return ERROR.TipMsg(connection, string.format("无效的设置选项:%s。", v.type))
		end
	end
	local user = assert(GetOnlineUser(connection.roleId))
	for _, v in ipairs(data) do
		user:SetSetting(v.type, v.set)
	end
	connection:SendProtocol("c_setting_set", data)
end

local s_setting_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {}
	for k in pairs(SETTING.GetSettingList()) do
		table.insert(sData, {type = k, set = user:GetSetting(k) or 0})
	end
	connection:SendProtocol("c_setting_list", sData)
end

local s_meditate_start = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	if not user:IsInMeditate() then
		user:SetMeditate()
	end
	local meditate = user:IsInMeditate()
	local sData = {timelong = meditate.endTime - os.time(), expGot = meditate.expGot}
	connection:SendProtocol("c_meditate_start", sData)
end

local s_meditate_hide = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	if user:IsInMeditate() then
		user:RemoveMeditateCallOut()
	end
	connection:SendProtocol("c_meditate_hide", "true")
end

local s_meditate_receive = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local exp = user:RecieveMeditateExp()
	connection:SendProtocol("c_meditate_receive", exp)
end

local s_meditate_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	if not user:IsInMeditate() then
		user:SetMeditate()
	end
	local meditate = user:IsInMeditate()
	local sData = {}
	sData.timelong = math.min(meditate.timestamp, meditate.endTime) - meditate.startTime
	sData.expGot = meditate.expGot
	connection:SendProtocol("c_meditate_info", sData or 0)
end

local s_user_slgInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = user:GetSLGInfo()
	connection:SendProtocol("c_user_slgInfo", sData)
end

local s_user_addHp = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:RestoreHp()
	local formationList = user:GetCurFightLineup()
	for _, val in pairs(formationList) do
		if val.id ~= "user" then
			local hero = user:GetHero(val.id)
			if hero then
				user:RestoreHp(hero)
			end
		end
	end
	user:SyncData("teamHp")
	connection:SendProtocol("c_user_heroAdd", "true")
end

local BuyVigourType = {
	[1] = {vigour = 10, ingot = 20, name = "精神焕发"},
	[2] = {vigour = 15, ingot = 30, name = "身强体壮"},
	[3] = {vigour = 20, ingot = 40, name = "酒足饭饱"},
}
local s_vigour_buy = function(connection, data)
	local vigourInfo = BuyVigourType[data]
	if not vigourInfo then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	if user:GetDailyVigourBuyTimes() >= user:GetDailyVigourBuyMaxNum() then
		return ERROR.TipMsg(connection, "您今日可购买精力的次数已经用完。")
	end
	if user:GetTotalIngot() < vigourInfo.ingot then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
--[[	if user:GetVigour() == user:GetVigourMax() then
		return ERROR.TipMsg(connection, "您精力已满，不需要再补充")
	end]]
	user:SetTotalIngot(-vigourInfo.ingot)
	user:SetDailyVigourBuyTimes(1)
	user:SetVigour(vigourInfo.vigour)
	connection:SendProtocol("c_vigour_buy", {type = data, times = user:GetDailyVigourBuyTimes(), max = user:GetDailyVigourBuyMaxNum()})
end

local s_vigour_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {times = user:GetDailyVigourBuyTimes(), max = user:GetDailyVigourBuyMaxNum(), list = {}}
	for k, v in ipairs(BuyVigourType) do
		table.insert(sData.list, {type = k, needIngot = v.ingot, vigour = v.vigour, name = v.name})
	end
	connection:SendProtocol("c_vigour_info", sData)
end

local s_loginInit_done = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or "null"))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:SetLoginInitDone(true)
	local sData
	local pet = user:GetPet()
	if pet and pet:IsWaitForChoice() then
		sData = sData or {}
		table.insert(sData, "pet")
	end
	if user:IsFuncOpen(SETTING.FUNC_PHYLE_CHOOSE) and not user:GetPhyle() then
		sData = sData or {}
		table.insert(sData, "phyle")
	end
	connection:SendProtocol("c_loginInit_done", sData or 0)
end


local myHeroTypeList = {
	aptPill = {user = false, godHero = false, noEquip = false, needRest = false, skill = 1},	--使用飞仙丹
	form = {user = true, godHero = true, noEquip = false, needRest = false, skill = 1},			--阵型列表
	bourse = {user = false, godHero = false, noEquip = true, needRest = true, skill = 1},		--交易行
	skillUp = {user = true, godHero = true, noEquip = false, needRest = false, skill = 0},		--使用技能书
}

local s_hero_myList = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的格式类型。")
	end
	local info = myHeroTypeList[data.type]
	if not info then
		return ERROR.TipMsg(connection, "没有指定的请求类型。")
	end
	local itemInfo
	if data.goodsId then
		itemInfo = BASEITEM.GetItemById(data.goodsId)
		if not itemInfo then
			return ERROR.TipMsg(connection, "没有指定的物品信息。")
		end
		if itemInfo.type ~= "skillLv" then
			return ERROR.TipMsg(connection, "该物品不是技能书。")
		end
	end
	local user = assert(GetOnlineUser(connection.roleId))
	local sData = {type = data.type, list = {}}
	if info.user then
		if not noEquip or user:IsNoEquipWorn() then
			if not needRest or user:GetWorkState() == "rest" then
				if not itemInfo or user:IsSkillUpgradable(itemInfo.num) then
					table.insert(sData.list, user:GetSimpleInfo(info.skill))
				end
			end
		end
	end
	for k, v in pairs(user:GetHeroList()) do
		if info.godHero or v:GetType() ~= "godHero" then
			if not noEquip or v:IsNoEquipWorn() then
				if not needRest or v:GetWorkState() == "rest" then
					if not itemInfo or v:IsSkillUpgradable(itemInfo.num) then
						table.insert(sData.list, v:GetSimpleInfo(info.skill, data.type == "aptPill"))
					end
				end
			end
		end
	end
	connection:SendProtocol("c_hero_myList", sData)
end


function __init__()
	DATA.Restore(path_userNick, AllUserNick)
--	DATA.Restore(path_fightData, AllFightDataList)
	
	ATTRTRAIN = Import("autocode/attrTrain.lua")
	AttrTrainList = ATTRTRAIN.GetTable()
	
	CALLOUT.CallFre(SaveAllOnlineUser, 60)
	RestoreVigour(true)
	CALLOUT.Daily(getfenv(2), 12, 0, AddVigourBuffer)
	CALLOUT.Daily(getfenv(2), 18, 0, AddVigourBuffer)
	
	PROTOCOL.s_user_slgInfo = s_user_slgInfo
	
	PROTOCOL.s_user_info = s_user_info
	PROTOCOL.s_user_rename = s_user_rename
	
	PROTOCOL.s_user_heroList = s_user_heroList
	PROTOCOL.s_info_detail = s_info_detail
	PROTOCOL.s_attr_detail = s_attr_detail
	PROTOCOL.s_skill_list = s_skill_list	
	
	PROTOCOL.s_attrTrain_info = s_attrTrain_info
	PROTOCOL.s_attrTrain_gen = s_attrTrain_gen
	PROTOCOL.s_attrTrain_save = s_attrTrain_save
	
	PROTOCOL.s_grid_unlock = s_grid_unlock
	PROTOCOL.s_equip_list = s_equip_list
	PROTOCOL.s_equip_unfix = s_equip_unfix
	PROTOCOL.s_equip_fix = s_equip_fix

	PROTOCOL.s_heroSlot_list = s_heroSlot_list
	PROTOCOL.s_heroSlot_unlock = s_heroSlot_unlock
	
	PROTOCOL.s_user_view = s_user_view
	PROTOCOL.s_view_lineup = s_view_lineup
	
	PROTOCOL.s_setting_set = s_setting_set
	PROTOCOL.s_setting_list = s_setting_list

	PROTOCOL.s_meditate_start = s_meditate_start
	PROTOCOL.s_meditate_hide = s_meditate_hide
	PROTOCOL.s_meditate_receive = s_meditate_receive
	PROTOCOL.s_meditate_info = s_meditate_info

	PROTOCOL.s_user_addHp = s_user_addHp
	PROTOCOL.s_vigour_info = s_vigour_info
	PROTOCOL.s_vigour_buy = s_vigour_buy
	
	PROTOCOL.s_loginInit_done = s_loginInit_done
	PROTOCOL.s_hero_myList = s_hero_myList
end
