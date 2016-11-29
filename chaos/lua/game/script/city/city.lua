local DAILY_PILLAGE_TIMES = 5		--每日掠夺次数
local COERCE_PILLAGE_COST = 100		--掠夺时强制冷却花费
local FORCE_COLD_DOWN_COST = 500	--攻打不同势力的强制冷却花费
local DAILY_ATTACK_TIMES = 5		--城主每日进攻次数
local CITY_APPLY_COLD_TIME = 24 * 3600		--脱离城池后的冷却时间
local MAX_APPLY_WAIT_TIME = 24 * 3600 		--申请最大等待时间
local USER_ATTACH_APPLY_NUM = 3		--玩家发出的最大申请数
local CITY_APPLY_NUM = 100			--城池的申请列表上限
--local INVITE_NEED_INGOT = 50				--发布邀请所需元宝
local MAX_MAP_WIDTH = 5000			--slg大地图宽度
local MAX_MAP_HEIGHT = 2600			--slg大地图高度
local MAX_INVITE_TIMES = 5
local AllCityObjList = {}
local path_city = "chaos.city"

function GetAllCityList()
	return AllCityObjList
end

function GetCityObjById(id)
	return AllCityObjList[id]
end

function SetCityObjById(id, city)
	AllCityObjList[id] = city
end

function ClearAllCity()
	for k, v in pairs(AllCityObjList) do
		v:Destroy()
		v = nil
	end
end

function InitAllCityData()
	local list = CLSCITY.GetAllCityInfoList()
	for k, v in pairs(list) do
		local city = CLSCITY.clsCity:New()
		city:InitCity(k, nil, v.phyle, v.name, v.initLv, v.limitLv)
		SetCityObjById(k, city)
	end
end

function SaveAllCityData()
	for k, v in pairs(GetAllCityList()) do
		local buff = v:GetSaveData()
		buff._id = k
		DATA.UpdateData(path_city, {_id = k}, buff)
	end
end

function RestoreAllCity()
	local function callback(Data, ErrMsg)
		Data = UnSerialize(Data)
		if not Data or #Data <= 0 then
			InitAllCityData()
		else
			for _, v in pairs(Data) do
				local city = CLSCITY.clsCity:New()
				city:Restore(v)
				city:StartCallOut()			--保护定时器
				city:StartAllTacticTimer()	--计谋定时器
				SetCityObjById(v._id, city)
			end
		end
	end
	DATA.Read(path_city, {}, callback)
end

function AttachToMainCity(roleId, userInfo)
	local phyle = USER.GetUserNickInfo(roleId, "phyle")
	local cityId = CLSCITY.GetMainCityByPhyle(phyle)
	local mainCity = GetCityObjById(cityId)
	if not mainCity then
		return
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:SetCity(cityId, CLSCITY.ID_ATTACH)
	else
		OFFLINE.SetOfflineData(roleId, "setCity", cityId, CLSCITY.ID_ATTACH)
	end
	mainCity:AddToMemberList(roleId, CLSCITY.ID_ATTACH, userInfo)
end
------------------------------------------------------------
local AllUserInCityMap = {}

function GetAllUserInCityMap()
	return AllUserInCityMap
end

function SetUserInCityMap(roleId)
	AllUserInCityMap[roleId] = 1
end

function GetUserInCityMap(roleId)
	return AllUserInCityMap[roleId]
end

function ClearUserInCityMap(roleId)
	AllUserInCityMap[roleId] = nil
end

------------------------------------------------------------
local clsUser = USERBASE.clsUser

--初始化玩家依附信息
function clsUser:InitCityInfo()
	self.__save.city = self.__save.city or {}
	self.__save.city.pillage = self.__save.city.pillage or {}
	local phyle = self:GetPhyle()
	if phyle then
		self:QuitCity()
		AttachToMainCity(self:GetRoleId())
	end
end

function clsUser:GetCityName()
	if not self.__save.city then 
		return 
	end
	return self.__save.city.name
end

function clsUser:GetCityId()
	if not self.__save.city then return end
	return self.__save.city.id
end

function clsUser:GetCityIdentity()
	if not self.__save.city then return end
	return self.__save.city.identity
end

function clsUser:SetCityQuitTime(time)
	if not self.__save.city then return end
	self.__save.city.quitTime = time or os.time()
end

function clsUser:GetCityQuitTime()
	if not self.__save.city then return end
	return self.__save.city.quitTime or (os.time() - 24 * 3600)
end

function clsUser:SetTempCity(cityId)
	self.__save.city.tempCity = cityId
end

function clsUser:GetTempCity()
	return self.__save.city.tempCity
end

function clsUser:SetCity(cityId, identity)
	self.__save.city = self.__save.city or {}
	self.__save.city.id = cityId
	self.__save.city.identity = identity
	USER.ModUserNickInfo(self:GetRoleId(), "cityId", cityId)
	SendMyStatUpdate(self)
	self.__save.city.quitTime = os.time() - 24 * 3600
end

function clsUser:QuitCity()	
	local cityId = self:GetCityId()
	if cityId then
		local city = GetCityObjById(cityId)
		city:RemoveMember(self:GetRoleId())
	end
end


--城池掠夺次数
function clsUser:SetPillageTimes(data)
	self.__save.city = self.__save.city or {}
	self.__save.city.pillage = self.__save.city.pillage or {}
	local info = self.__save.city.pillage
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
	MISSION.CheckMissionEvent(self, "func", SETTING.FUNC_PILLAGE, 1)
end

function clsUser:GetPillageTimes()
	self.__save.city = self.__save.city or {}
	self.__save.city.pillage = self.__save.city.pillage or {}
	local info = self.__save.city.pillage
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:GetMaxPillageTimes()
	return DAILY_PILLAGE_TIMES
end

--城池进攻次数
function clsUser:GetCityAttackTimes()
	self.__save.city = self.__save.city or {}
	self.__save.city.attack = self.__save.city.attack or {}
	local info = self.__save.city.attack
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:SetCityAttackTimes(data)
	self.__save.city = self.__save.city or {}
	self.__save.city.attack = self.__save.city.attack or {}
	local info = self.__save.city.attack
	if not info.time or CalDiffDays(info.time, os.time(), COUNT_RESET_HOUR) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUser:GetMaxCityAttackTimes()
	local identity = self:GetCityIdentity()
	if identity ~= CLSCITY.ID_MASTER then
		return 0
	end
	return DAILY_ATTACK_TIMES
end

--被掠夺保护时间
function clsUser:SetPillageProtectTime(time)
	self.__save.city = self.__save.city or {}
	self.__save.city.pillage = self.__save.city.pillage or {}
	self.__save.city.pillage.protectTime = time or os.time()
end

function clsUser:GetPillageProtectTime()
	self.__save.city = self.__save.city or {}
	self.__save.city.pillage = self.__save.city.pillage or {}
	return self.__save.city.pillage.protectTime or os.time()
end


--依附申请
function clsUser:GetAttachApplyNum()
	local num = 0
	local roleId = self:GetRoleId()
	for k, v in pairs(GetAllCityList()) do
		if v:IsInList("apply", roleId) then
			num = num + 1
		end
	end
	return num
end



--护城神魂
function clsUser:GetGodHero()
	for k, v in pairs(self:GetHeroList()) do
		if string.sub(k, -1, -1) == GODHERO_ID_ENDING then
			return v
		end
	end
end
--检查护城神魂是否状态ok (false 出战不能获得经验)
function clsUser:IsGodHeroFuncOpen()
	local cityId = self:GetCityId()
	local city = GetCityObjById(cityId)
	if not city or city:GetOwner() ~= self:GetRoleId() then
		return false
	end
	if not city:HasGodHero() then
		return false
	end
	return true
end

	
function clsUser:CheckCityForm(formId)
	local cityObj = GetCityObjById(self:GetCityId())
	if cityObj and cityObj:GetOwner() == self:GetRoleId() then
		if not cityObj:IsFightFormInited() then
			cityObj:InitFightForm()
		else
			cityObj:InitOneFormation(formId)
		end
	end
end

------------------------------------------------------------------------


function ClearAllAttachApply(roleId)
	for k, v in pairs(GetAllCityList()) do
		v:CancelApply(roleId)
	end
end


function RefreshUserCityInfo(roleId, flag)
	local cityId = USER.GetUserNickInfo(roleId, "cityId")
	local city = GetCityObjById(cityId)
	if city then
		city:RefreshInfo("member", roleId)
		if flag then
			city:SendMemberUpdate("union", roleId)
		end
		if city:IsMainCity() then
			for k, v in pairs(GetAllCityList()) do 
				if v:IsInList("apply", roleId) then
					v:RefreshInfo("apply", roleId)
				end
			end
		end
	end
end

function IsNameExist(name)
	for k, v in pairs(GetAllCityList()) do
		if v:GetName() == name then
			return true
		end
	end
	return false
end
--------------------------------------------------------------------------------
function GetFormatCityList(roleId)
	local sData = {}
	for k, v in pairs(GetAllCityList()) do
		local buf = {}
		buf.cityId = k
		buf.picId = v:GetPicId()
		buf.Lv = v:GetLv()
		buf.limitLv = v:GetLimitLv()
		local locate = v:GetLocation()
		buf.posX = locate.x
		buf.posY = locate.y
		buf.phyle = v:GetPhyle()
		buf.name = v:GetName()
		if v:IsMainCity() then
			buf.isMain = 1
		end
		buf.isApply = v:IsInList("apply", roleId)
		local state = v:GetState()
		buf.state = state
		local owner = v:GetOwner()
		if owner then
			buf.owner = USER.GetUserNickById(owner)
			buf.vipLv = USER.GetUserNickInfo(owner, "vipLv")
		else
			buf.owner = ""
			if state == "wait" then
				buf.tempOwner = USER.GetUserNickById(v:GetTempOwner())
			end
		end
		local timelong = v:GetTimeStamp() - os.time()
		buf.timelong = timelong > 0 and timelong or 0
		table.insert(sData, buf)
	end
	return sData
end

function GetFormatedCityLink(city)
	local skillId = city:GetLinkSkill()
	local Lv = 0
	local linkList = city:GetAllLinkCity()
	local phyle = city:GetPhyle()
	local sData = {list = {}}
	for _, v in pairs(linkList) do
		local obj = GetCityObjById(v)
		local buff = {}
		buff.name = obj:GetName()
		buff.Lv = obj:GetLv()
		buff.cityId = v
		if obj:GetPhyle() == phyle then
			buff.state = 1
			Lv = Lv + 1
		else
			buff.state = 0
		end
		table.insert(sData.list, buff)
	end
	local skillInfo = SKILL.GetSkillInfoByLv(skillId, Lv)
	if skillInfo then
		sData.describe = skillInfo.explain
	end
	return sData
end

function SendMyStatUpdate(user)
	local roleId = user:GetRoleId()
	local sData = {}
	sData.cityId = user:GetCityId()
	sData.identity = user:GetCityIdentity()
	if not GetUserInCityMap(roleId) then
		return
	end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_city_statUpdate", sData)
	end
end



--打开大地图时获取
local s_city_myStat = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	SetUserInCityMap(connection.roleId)
	local sData = {}
	sData.cityId = user:GetCityId()
	sData.identity = user:GetCityIdentity()
	sData.pillageColdCost = COERCE_PILLAGE_COST
	sData.attackColdCost = FORCE_COLD_DOWN_COST
	sData.maxX = MAX_MAP_WIDTH
	sData.maxY = MAX_MAP_HEIGHT
	connection:SendProtocol("c_city_myStat", sData)
end
--关闭大地图
local s_city_close = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	ClearUserInCityMap(connection.roleId)
	connection:SendProtocol("c_city_close", "true")
end

local s_city_list = function (connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatCityList(connection.roleId)
	connection:SendProtocol("c_city_list", sData)
end

local s_city_info = function (connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local city = GetCityObjById(data)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn(data, connection.roleId, 1)
	user:SetFocus("city", data)
	local sData = city:GetCityInfo(connection.roleId)
	connection:SendProtocol("c_city_info", sData)
end

local s_city_memberList = function(connection, data)
	local city = GetCityObjById(data)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	local sData = {cityId = data}
	sData.list = city:FormatMemberList()
	connection:SendProtocol("c_city_memberList", sData)
end

local s_city_nextInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if not city or city:IsMainCity() then
		return ERROR.TipMsg(connection, "您当前依附在主城，不能捐献。")
	end
	if city:GetLv() >= city:GetLimitLv() then
		return ERROR.TipMsg(connection, "城池等级已经达到瓶颈。")
	end
	local nextInfo = CLSCITY.GetCityUpgradeInfo(city:GetLv() + 1)
	local sData = {}
	if nextInfo then
		sData.sparNum = city:GetSparNum()
		sData.sparMax = city:GetSparMax()
		sData.sparNow = 0
		local tribe = user:GetTribe()
		if tribe then
			sData.sparNow = tribe:GetResourceStat("spar", "num")
		end
		sData.resRate = city:GetInitPhyle() == "npc" and nextInfo.neutralResRate or nextInfo.normalResRate
		sData.memberMax = nextInfo.num
	end
	connection:SendProtocol("c_city_nextInfo", sData)
end

local s_city_donate = function (connection, data)
	data = tonumber(data)
	if not data or data <= 0 then
		return ERROR.TipMsg(connection, "请输入要捐献的晶石数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if not city or city:IsMainCity() then
		return ERROR.TipMsg(connection, "您当前依附在主城，不能捐献。")
	end
	if city:GetLv() >= city:GetMaxLv() then
		return ERROR.TipMsg(connection, "城池已经达到最高等级。")
	elseif city:GetLv() >= city:GetLimitLv() then
		return ERROR.TipMsg(connection, "城池等级已经达到瓶颈。")
	end
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	if tribe:GetResourceStat("spar", "num") < data then
		return ERROR.TipMsg(connection, "晶石不足，不能捐献")
	end
	local Lv = city:GetLv()
	city:Donate(data)
	tribe:ModResourceStat("spar", "num", -data)
--[[	local sData = {num = data, expRate = math.floor(city:GetSparNum() / city:GetSparMax() * 100)}
	if city:GetLv() ~= Lv then
		sData.Lv = city:GetLv()
	end]]
	connection:SendProtocol("c_city_donate", "true")
	PROTOCOL.s_city_nextInfo(connection, "get")
end

local s_city_rename = function(connection, data)
	data = tostring(data)
	if not data or data == "" then
		return ERROR.TipMsg(connection, "请输入新的城池名。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能改名。")
	end
	if NEWUSER.NameIsIllegal(data) then
		return ERROR.TipMsg(connection, "该名字含有敏感字符。")
	end
	if IsNameExist(data) then
		return ERROR.TipMsg(connection, "该名字已经被占用。")
	end
	local cost = city:GetRenameCost()
	if user:GetGold() < cost then
		return ERROR.TipMsg(connection, "您的元宝数量不足。")
	end
	user:SetGold(-cost)
	city:SetName(data)
	city:SetRenameTimes(1)
	connection:SendProtocol("c_city_rename", {name = data, renameCost = city:GetRenameCost()})
end
--个人申请列表
local s_city_myApply = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	for k, v in pairs(GetAllCityList()) do
		if v:IsInList("apply", connection.roleId) then
			local buf = {}
			buf.cityId = k
			local locate = v:GetLocation()
			buf.posX, buf.posY = locate.x, locate.y
			buf.name = v:GetName()
			table.insert(sData, buf)
		end
	end
	connection:SendProtocol("c_city_myApply", sData)
end

local s_city_apply = function(connection, data)
	local city = GetCityObjById(data)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	elseif city:IsMainCity() then
		return ERROR.TipMsg(connection, "主城不能主动依附。")
	end
	if not city:GetOwner() then
		return ERROR.TipMsg(connection, "不能依附无主之城。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if city:GetPhyle() ~= user:GetPhyle() then
		return ERROR.TipMsg(connection, "不能依附其他势力的城池。")
	end
	local cityId = user:GetCityId()
	if user:GetCityIdentity() == CLSCITY.ID_MASTER then
		return ERROR.TipMsg(connection, "城主不能申请依附。")
	end
	if cityId == data then
		return ERROR.TipMsg(connection, "您已经依附在该城中了。")
	end
	if cityId ~= CLSCITY.GetMainCityByPhyle(user:GetPhyle()) then
		return ERROR.TipMsg(connection, "只有退出该城池之后，才能依附新的城池。")
	end
	if city:IsInList("apply", connection.roleId) then
		return ERROR.TipMsg(connection, "您已经向该城发出申请了。")
	end
	if user:GetAttachApplyNum() >= USER_ATTACH_APPLY_NUM then
		ERROR.TipMsg(connection, "您发送的依附申请已经达到上限。")
		return PROTOCOL.s_city_myApply(connection, "get")
	end
	if not city:IsJoinable() then
		return ERROR.TipMsg(connection, "该城没有开启依附申请。")
	end
	if city:GetMemberNum() >= city:GetMemberMax() then
		return ERROR.TipMsg(connection, "该城依附人数已达上限。")
	end
	if city:GetApplyNum() >= CITY_APPLY_NUM then
		return ERROR.TipMsg(connection, "申请人数已达上限，无法申请")
	end
	local quitTime = user:GetCityQuitTime()
	if quitTime and (os.time() - quitTime) < CITY_APPLY_COLD_TIME then
		return ERROR.TipMsg(connection, string.format("%d小时后方可重新依附。", math.ceil((CITY_APPLY_COLD_TIME - (os.time() - quitTime)) / 3600)))
	end
	city:AddToApplyList(user)
	connection:SendProtocol("c_city_apply", data)
end

local s_city_cancel = function(connection, data)
	local city = GetCityObjById(data)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	if not city:IsInList("apply", connection.roleId) then
		return ERROR.TipMsg(connection, "您没有申请依附该城")
	end
	city:CancelApply(connection.roleId)
	connection:SendProtocol("c_city_cancel", data)
end

local s_city_applyList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限查看")
	end
	local list = city:GetListByType("apply")
	local sData = {list = {}}
	sData.isjoinable = city:IsJoinable()
	for k, v in pairs(list) do
		if os.time() - v.timestamp > MAX_APPLY_WAIT_TIME then
			city:Refuse(k)
		else
			local buf = {}
			buf.roleId = k
			buf.nick = USER.GetUserNickById(k)
			buf.force = v.force
			buf.Lv = v.Lv
			table.insert(sData.list, buf)
		end
	end
	table.sort(sData.list, function(a, b) return list[a.roleId].timestamp > list[b.roleId].timestamp end)
	connection:SendProtocol("c_city_applyList", sData)
end

local s_city_agree = function (connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if not city:IsJoinable() then
		return ERROR.TipMsg(connection, "您已经关闭了城池的依附申请。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行该操作。")
	end
	local msg
	if data == "all" then
		for k in pairs(city:GetListByType("apply")) do
			if city:GetMemberNum() >= city:GetMemberMax() then
				msg =  "依附人数已达上限。"
				break
			end
			city:AttachToCity(k)
			city:SendCityMsg("apply", k)
			ClearAllAttachApply(k)
		end
	else
		local roleId = USER.GetRoleIdByNick(data)
		if not roleId then
			return ERROR.TipMsg(connection, "没有指定的玩家信息。")
		end
		if not city:IsInList("apply", roleId) then
			return ERROR.TipMsg(connection, "该玩家不在申请列表中。")
		end
		if city:GetMemberNum() >= city:GetMemberMax() then
			return ERROR.TipMsg(connection, "依附人数已达上限。")
		end
		city:AttachToCity(roleId)
		city:SendCityMsg("apply", roleId)
		ClearAllAttachApply(roleId)
	end
	connection:SendProtocol("c_city_agree", {type = data, msg = msg})
	if data == "all" then
		PROTOCOL.s_city_applyList(connection, "get")
	end
end

local s_city_refuse = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限")
	end
	if data == "all" then
		for k in pairs(city:GetListByType("apply")) do
			city:Refuse(k)
		end
	else
		local roleId = USER.GetRoleIdByNick(data)
		if not roleId then
			return ERROR.TipMsg(connection, "没有指定的玩家信息。")
		end
		city:Refuse(roleId)
	end
	connection:SendProtocol("c_city_refuse", data)
	if data == "all" then
		PROTOCOL.s_city_applyList(connection, "get")
	end
end

local s_city_inviteMsg = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限进行该操作。")
	end
--	if user:GetTotalIngot() < INVITE_NEED_INGOT then
--		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
--	end
	if city:GetInviteTimes() >= MAX_INVITE_TIMES then
		return ERROR.TipMsg(connection, "今天此城的邀请次数已经用完")
	end
	if city:GetApplyNum() >= CITY_APPLY_NUM then
		return ERROR.TipMsg(connection, "申请的人数已经达到到上限")
	end
--	if city:GetMemberNum() >= city:GetMemberMax() then
--		return ERROR.TipMsg(connection, "依附人数已达上限。")
--	end
--	user:SetTotalIngot(-INVITE_NEED_INGOT)
	city:SetJoinable(1)
	city:SetInviteTimes(1)
	local info = city:GetLocation()
	local nick = USER.GetUserNickById(connection.roleId)
	local msg = string.format("【%s(%d, %d)】城主【<a href=\"event:person_%s\"><u>%s</u></a>】求贤若渴，邀请各路豪杰共图霸业。", city:GetName(), info.x, info.y, nick, nick)
	CHAT.SendSysMsg("phyle", msg, USER.GetUserOnlineByPhyle(user:GetPhyle()), {kind = "cityInvite", id = city:GetId()})
	connection:SendProtocol("c_city_inviteMsg", MAX_INVITE_TIMES - city:GetInviteTimes())
end

local s_city_joinable = function(connection, data)
	if data ~= 1 and data ~= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限进行该操作。")
	end
	city:SetJoinable(data == 1 and 1)
	for k in pairs(city:GetListByType("apply")) do
		self:RmFromList("apply", k)
	end
	connection:SendProtocol("c_city_joinable", data)
	PROTOCOL.s_city_applyList(connection, "get")
end

--移除
local s_city_remove = function (connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "您不能移除自己。")
	end
	if not city:IsInList("member", roleId) then
		return ERROR.TipMsg(connection, "该玩家没有依附在您的城里")
	end
	city:LeaveCity(roleId)
	city:SendCityMsg("remove", roleId, connection.roleId)
	connection:SendProtocol("c_city_remove", data)
end

local s_city_demise = function(connection, data)
	if type(data) ~= "table" then
		return 	ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local myCity = GetCityObjById(user:GetCityId())
	if not myCity or myCity:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行转让。")
	end
	local city = GetCityObjById(data.cityId)
	if not city then
		return ERROR.TipMsg(connection, "无效的城池id。")
	end
	local owner = city:GetOwner()
	if (owner and owner ~= connection.roleId) or (not owner and city:GetTempOwner() ~= connection.roleId) then
		return ERROR.TipMsg(connection, "您没有该城的支配权，不能完成该操作。")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if not myCity:IsInList("member", roleId) then
		return ERROR.TipMsg(connection, "您只能把城池转让给自己的依附者。")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "您不能对自己执行该操作。")
	end
	if city:GetFightState() then
		return ERROR.TipMsg(connection, "城池处于战斗状态时不能进行转让。")
	end
--	city:Demise(user, roleId, myCity:GetId())
	if myCity:GetId() ~= data.cityId then
		myCity:DemiseNewCity(user, roleId, city)
	else
		myCity:DemiseCurCity(user, roleId)
	end
	connection:SendProtocol("c_city_demise", data)
end

local s_city_occupancy = function(connection, data)
	if type(data) ~= "table" then
		return 	ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local city = GetCityObjById(data.cityId)
	if not city then
		return ERROR.TipMsg(connection, "无效的城池id。")
	end
	local owner = city:GetOwner()
	if city:GetTempOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您没有该城的支配权，不能完成该操作。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local identity = user:GetCityIdentity()
	if identity == CLSCITY.ID_MASTER and not data.donee then
		return ERROR.TipMsg(connection, "您只能占领一座城池。")
	end
	if data.donee then
		if identity ~= CLSCITY.ID_MASTER then
			return ERROR.TipMsg(connection, "您不是城主，无法转让自己的城池给别人。")
		end
		local roleId = USER.GetRoleIdByNick(data.donee)
		if not roleId then
			return ERROR.TipMsg(connection, "没有指定的玩家信息。")
		end
		if roleId == connection.roleId then
			return ERROR.TipMsg(connection, "您不能把旧城池转让给自己。")
		end
		local myCity = GetCityObjById(user:GetCityId())
	--	myCity:Demise(user, roleId, myCity:GetId())
		myCity:DemiseCurCity(user, roleId, true)
	end
	city:Occupancy(user)
	connection:SendProtocol("c_city_occupancy", data)
end

--脱离城池
local s_city_quit = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if not city or city:IsMainCity() then
		return ERROR.TipMsg(connection, "您没有依附野地城池。")
	end
	city:Abandon(user)
	connection:SendProtocol("c_city_quit", "true")
end

--掠夺
local s_city_pillage = function(connection, data)
	if type(data) ~= "table" or not data.cityId or not data.nick then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "没有该用户")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local myCity = GetCityObjById(user:GetCityId())
	if not myCity then
		return ERROR.TipMsg(connection, "您还没有依附城池，不能掠夺")
	end
	if not myCity:IsMainCity() and myCity:GetId() == data.cityId then
		return ERROR.TipMsg(connection, "不能掠夺自己所在城池的玩家。")
	end
	local myUnionId = USER.GetUserNickInfo(connection.roleId, "unionId")
	if myUnionId and myUnionId == USER.GetUserNickInfo(roleId, "unionId") then
		return ERROR.TipMsg(connection, "不能掠夺自己所在联盟的玩家。")
	end
	if user:GetPillageTimes() >= DAILY_PILLAGE_TIMES then
		return ERROR.TipMsg(connection, "您今天的掠夺次数已经用完")
	end
	local city = GetCityObjById(data.cityId)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	local tuserInfo = city:IsInList("member", roleId)
	if not tuserInfo then
		return ERROR.TipMsg(connection, "指定的玩家并未依附在该城中。")
	end
	if tuserInfo.fightState then
		return ERROR.TipMsg(connection, "该玩家正在被掠夺。")
	end
	local cost = 0
	if tuserInfo.proTimestamp > os.time() then
--		if not data.coerce then
--			return ERROR.TipMsg(connection, "该玩家处于保护状态中，无法掠夺。")
--		end
		cost = COERCE_PILLAGE_COST
	end
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetTotalIngot(-cost)
	user:SetPillageTimes(1)
	if tuserInfo.proTimestamp >= os.time() then
		city:ModInfoInList("member", roleId, "proTimestamp", os.time())
		city:SendMemberUpdate("state", roleId)
	end
	city:ProPillage(user, roleId)
end

local s_city_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type == "city" then
		USER.SetUserFocusOn(focus.id, connection.roleId)
		user:SetFocus()
	end
	connection:SendProtocol("c_city_close", "true")
end

--突破瓶颈
local s_city_break = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = GetCityObjById(user:GetCityId())
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，没有权限进行该操作。")
	end
	local info = CLSCITY.GetCityUpLimitInfo(city:GetLv())
	if not info then
		return ERROR.TipMsg(connection, "您的城池尚未达到发展瓶颈，不需要进行突破。")
	end
	if city:GetLimitLv() > info.Lv then
		return ERROR.TipMsg(connection, "该城池已经突破了发展瓶颈。")
	end
	if user:GetIngot() < info.needIngot then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	city:BreakLimit()
	local sData = {}
	sData.limitLv = city:GetLimitLv()
	local limitInfo = CLSCITY.GetCityUpLimitInfo(city:GetLimitLv())
	if limitInfo then
		sData.breakNeed = limitInfo.needIngot
	end
	connection:SendProtocol("c_city_break", sData)
end

local s_city_getLink = function(connection, data)
	local city = GetCityObjById(data)
	if not city then
		return ERROR.TipMsg(connection, "没有指定的城池信息。")
	end
	local sData = GetFormatedCityLink(city)
	connection:SendProtocol("c_city_getLink", sData)
end


function __init__()
	RestoreAllCity()
	
	PROTOCOL.s_city_myStat = s_city_myStat
	PROTOCOL.s_city_list = s_city_list
	PROTOCOL.s_city_info = s_city_info
	PROTOCOL.s_city_memberList = s_city_memberList
	PROTOCOL.s_city_getLink = s_city_getLink
	PROTOCOL.s_city_close = s_city_close
	
	PROTOCOL.s_city_nextInfo = s_city_nextInfo
	PROTOCOL.s_city_donate = s_city_donate
	PROTOCOL.s_city_rename = s_city_rename
	PROTOCOL.s_city_inviteMsg = s_city_inviteMsg
	PROTOCOL.s_city_joinable = s_city_joinable
	PROTOCOL.s_city_break = s_city_break
	
	PROTOCOL.s_city_applyList = s_city_applyList
	PROTOCOL.s_city_agree = s_city_agree
	PROTOCOL.s_city_refuse = s_city_refuse
	PROTOCOL.s_city_remove = s_city_remove
	PROTOCOL.s_city_demise = s_city_demise
	PROTOCOL.s_city_occupancy = s_city_occupancy
	
	PROTOCOL.s_city_myApply = s_city_myApply
	PROTOCOL.s_city_apply = s_city_apply
	PROTOCOL.s_city_cancel = s_city_cancel
	PROTOCOL.s_city_quit = s_city_quit
	
	PROTOCOL.s_city_pillage = s_city_pillage
	PROTOCOL.s_city_leave = s_city_leave
	
	CALLOUT.CallFre(SaveAllCityData, 60 * 2)
end
