--使馆

local OFFLINE_TIME_LONG = 2 * 60	--离线使馆存在时间
local STAY_ADD_FAVOR_NUM = 2		--协防增加好感数量
local FORM_ID = "321002"
local FORM_LV = 10
local FRESH_EXP_TIMELONG = 30 * 60

local AllEmbassyList = {}		--在线使馆列表
local path_embassy = "chaos.embassy"
local EmbassyInLoading = {}		--恢复中的使馆列表

function GetAllEmbassyList()
	return AllEmbassyList
end

function GetEmbassyObjById(roleId)
	return AllEmbassyList[roleId]
end

function SetEmbassyObjById(roleId, obj)
	AllEmbassyList[roleId] = obj
end

--正在加载中的使馆
function SetEmbassyInLoading(roleId, call_back)
	EmbassyInLoading[roleId] = EmbassyInLoading[roleId] or {}
	if call_back then
		table.insert(EmbassyInLoading[roleId], call_back)
	end
end

function GetEmbassyInLoading(roleId)
	return EmbassyInLoading[roleId]
end

function RemoveEmbassyInLoading(roleId)
	EmbassyInLoading[roleId] = nil
end

-----------------------------------------------------
clsEmbassy = STRUCT.clsStruct:Inherit()

function clsEmbassy:__init__()
	Super(clsEmbassy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsEmbassy:initsavedata()	
	self.__save.Lv = 0
	self.__save.enlist = {}
	self.__save.heroList = {}
end

function clsEmbassy:inittempdata()
	self.__temp.callout = {}
	self.__temp.visit = {}
end

function clsEmbassy:GetCallOut()
	return self.__temp.callout
end

function clsEmbassy:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
	self:SetTimeStamp(os.time())
	self:InitPosList()
end

function clsEmbassy:InitPosList()
	for i = 1, self:GetMaxStayNum() do
		self:SetPosOpen(tostring(i))
	end
end

function clsEmbassy:Upgrade()
	self:SetLv(1)
end

function clsEmbassy:AddHero(pos, data)
	pos = tostring(pos)
	self.__save.heroList[pos] = data
end

function clsEmbassy:DelHero(pos)
	pos = tostring(pos)
	self.__save.heroList[pos] = {}
end

function clsEmbassy:GetHeroList()
	return self.__save.heroList
end

function clsEmbassy:SetPosOpen(pos)
	self.__save.heroList[pos] = {}
end

function clsEmbassy:GetStayHeroNum()
	local num = 0
	for k, v in pairs(self:GetHeroList()) do
		if v.heroId then
			num = num + 1
		end
	end
	return num
end

--最大可驻扎武将数量
function clsEmbassy:GetMaxStayNum()
	return 5
end

function clsEmbassy:GetHeroStay(pos)
	pos = tostring(pos)
	return self.__save.heroList[pos]
end

--添加驻扎武将
function clsEmbassy:SetHeroStay(pos, owner, heroId)
	pos = tostring(pos)
	local user = assert(USER.GetOnlineUser(owner))
	local hero = user:GetHero(heroId)
	if hero then
		hero:SetWorkInfo("stay", self:GetOwner(), os.time())
		local buff = {}
		buff.heroId = heroId
		buff.owner = owner
		buff.Lv = hero:GetLv()
		buff.defend = hero:GetAttr("physcDef") + hero:GetAttr("magicDef")
		self:AddHero(pos, buff)
		local friendList = user:GetFriendList()
		local info = friendList:IsInMemberList("friend", self:GetOwner())
		if info then
			friendList:SetStayFavor(self:GetOwner(), STAY_ADD_FAVOR_NUM)
		end
		self:StartCallOut(pos)
		SaveEmbassyData(self:GetOwner())
	end
end

function clsEmbassy:StartCallOut(pos, time)
	local info = self:GetHeroStay(pos)
	local n = math.floor(((time or os.time()) - (info.timestamp or os.time())) / FRESH_EXP_TIMELONG)
	local timelong
	if n <= 0 then
		timelong = FRESH_EXP_TIMELONG
	else
		timelong = os.time() - info.timestamp + n * FRESH_EXP_TIMELONG
		timelong = timelong == 0 and FRESH_EXP_TIMELONG or timelong
		for i = 1, n do
			self:AddHeroStayExp(pos, n)
		end
	end
	info.timestamp = (info.timetamp or os.time()) + n * FRESH_EXP_TIMELONG
	self.__temp.callout.pos = CALLOUT.ObjCallOut(self:GetCallOut(), clsEmbassy.AddHeroStayExp, timelong, self, pos)
end

function clsEmbassy:HasSendHero(roleId)
	for k, v in pairs(self:GetHeroList()) do
		if v.owner == roleId then
			return k
		end
	end
	return false
end

--更新驻扎武将的等级信息
function clsEmbassy:RenewHeroStay(user)
	local roleId = user:GetRoleId()
	for k, v in pairs(self:GetHeroList()) do
		if v.owner == roleId then
			local hero = user:GetHero(v.heroId)
			if hero then
				local buff = {}
				buff.heroId = v.heroId
				buff.owner = roleId
				buff.Lv = hero:GetLv()
				buff.defend = hero:GetAttr("physcDef") + hero:GetAttr("magicDef")
				self:AddHero(k, buff)
				SaveEmbassyData(self:GetOwner())
			end
		end
	end
end


function clsEmbassy:DelHeroStay(pos)
	local info = self:GetHeroStay(pos)
	if info and info.heroId then
		local user = USER.GetOnlineUser(info.owner)
		if user then
			local hero = user:GetHero(info.heroId)
			if hero then
				hero:SetWorkInfo("rest")
			end
		else
			OFFLINE.SetOfflineData(info.owner, "heroState", info.heroId, "rest")	--修改武将状态
		end
	end
	self:DelHero(pos)
	SaveEmbassyData(self:GetOwner())
end

function clsEmbassy:ProStructFunc(time)
	for k, v in pairs(self.__save.heroList) do
		if next(v) then
			self:StartCallOut(k)
		end
	end
	SaveEmbassyData(self:GetOwner())
end

--获取驻守武将的战斗数据
function clsEmbassy:GetStayHeroFightData()
	local num = self:GetStayHeroNum()
	if num <= 0 then
		return
	end
	local formInfo = FORMATION.GetFormationInfoById(FORM_ID)
	local sData = {formation = {formId = FORM_ID, formLv = FORM_LV}, lineup = {}}
	local max = 0
	local pos = 0
	for k, v in pairs(self:GetHeroList()) do
		if v.defend > max then
			max = v.defend
			pos = k
		end
	end
	local index = 1
	local info = self:GetHeroStay(pos)
	table.insert(sData.lineup, {id = info.heroId, owner = info.owner, pos = "6"})
	for k, v in pairs(self:getHeroList()) do
		local formPos = formInfo.list[FORM_LV].posList[index]
		if formPos ~= "6" and k ~= pos then
			local buf = {}
			buf.id = v.heroId
			buf.roleId = v.owner
			buf.pos = formPos
			table.insert(sData.lineup, buf)
		end
		index = index + 1
	end
	return sData
end

--增加驻守武将经验
function clsEmbassy:AddHeroStayExp(pos)
	local info = STRUCT.GetUpLvInfo(self:GetStructType(), self:GetLv())
	local heroInfo = self:GetStayHero(pos)
	if heroInfo.heroId then
		local exp = math.floor(USERBASE.GetUpgradeExp(heroInfo.Lv) / 192 * (1 + info.expAdd))
		local user = USER.GetOnlineUser(v.owner)
		if user then
			user:SetHeroExp(heroInfo.heroId, exp, "embassy")
		else
			OFFLINE.SetOfflineData(heroInfo.owner, "heroExp", exp, "embassy")
		end
	end
end

function clsEmbassy:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsEmbassy:Restore(data)
	self.__save = data
end
-------------------------------------------
--[[被离线加载起来的时候记录离线加载的时间
function clsEmbassy:GetOfflineTimestamp()
	self.__temp.offlineTime = self.__temp.offlineTime or os.time()
	return self.__temp.offlineTime
end

function clsEmbassy:SetOfflineTimestamp(time)
	self.__temp.offlineTime = os.time()
end]]
--设置访问者
function clsEmbassy:SetVisitor(roleId, state)
	self.__temp.visit = self.__temp.visit or {}
	self.__temp.visit[roleId] = state
end

function clsEmbassy:GetVisitNum()
	return table.size(self.__temp.visit)
end

function clsEmbassy:IsEmbassyNotUsed()
	if USER.GetOnlineUser(self:GetOwner()) then
		return false
	end
	if self:GetVisitNum() > 0 then
		return false
	end
	return true
end
--保存使馆数据
function SaveEmbassyData(roleId)
	local embassy = GetEmbassyObjById(roleId)
	if embassy then
		local buff = embassy:GetSaveData()
		buff._id = roleId
		DATA.UpdateData(path_embassy, {_id = roleId}, buff)
	end
end
--恢复使馆数据
function RestoreEmbassyData(roleId, call_back)
	if type(call_back) ~= "function" and type(call_back) ~= "nil" then
		return
	end
	if GetEmbassyObjById(roleId) then
		return call_back(true)
	end
	local function read_back(Data)
		Data = UnSerialize(Data)
		if not Data or not Data[1] then
			DATA.log(string.format("没有找到该玩家的使馆信息：%s。", roleId), "embassy.log")
			for _, func in ipairs(GetEmbassyInLoading(roleId)) do
				func(false, string.format("玩家%s尚未建立使馆。", USER.GetUserNickById(roleId)))
			end
			return
		end
		local embassy = clsEmbassy:New()
		embassy:Restore(Data[1])
		embassy:ProStructFunc()
		SetEmbassyObjById(roleId, embassy)
		for _, func in ipairs(GetEmbassyInLoading(roleId)) do
			func(true)
		end
		RemoveEmbassyInLoading(roleId)		--使馆恢复完毕
	end
	if GetEmbassyInLoading(roleId) then		--判断使馆是否已在恢复当中
		return SetEmbassyInLoading(roleId, call_back)
	end
	SetEmbassyInLoading(roleId, call_back)	--设置使馆处于恢复当中
	local ret, msg = DATA.Read(path_embassy, {_id = roleId}, read_back)
	if not ret then
		DATA.log(string.format("read data failed on %s: connection error!", roleId), "embassy.log")
	end
end

--定期检查没有被使用的使馆对象
function CheckOnlineEmbassy()
	for k, v in pairs(GetAllEmbassyList()) do
		v:ProStructFunc()
		SaveEmbassyData(k)
		if v:IsEmbassyNotUsed() then
			SetEmbassyObjById(k)
		end
	end
end

function RenewStayHeroInfo(user)
	for k, v in pairs(user:GetHeroList()) do
		local info = v:GetWorkInfo()
		if info.state == "stay" then
			local embassy = GetEmbassyObjById(info.place)
			if embassy then
				embassy:RenewHeroStay(user)
			end
		end
	end
end

--召回驻扎的武将
function RecallStayHero(roleId, owner)
	local embassy = GetEmbassyObjById(roleId)
	if embassy then
		local pos = embassy:HasSendHero(owner)
		if pos then
			embassy:DelHeroStay(pos)
		end
	else
		local function call_back(ret, msg)
			if ret then
				local embassy = GetEmbassyObjById(roleId)
				local pos = embassy:HasSendHero(owner)
				if pos then
					embassy:DelHeroStay(pos)
				end
			end
		end
		RestoreEmbassyData(roleId, call_back)
	end
end


--查看使馆武将列表   roleId表示查看人(不是使馆主人)
function clsEmbassy:GetViewInfo(roleId, call_back)
	local list = self:GetHeroList()
	local max = self:GetMaxStayNum()
	local userList = {}
	for i = 1, max do
		local pos = tostring(i)
		if list[pos] and list[pos].heroId then
			table.insert(userList, list[pos].owner)
		end
	end
	local function read_back(dataList)
		local sData = {}
		for i = 1, max do
			local pos = tostring(i)
			local buff = {pos = pos, state = "open"}
			if list[pos].heroId and dataList[list[pos].owner] then
				buff.owner = USER.GetUserNickById(list[pos].owner)
				if roleId and list[pos].owner == roleId then
					buff.state = "valid"
				end
				buff.Lv = list[pos].Lv
				local heroData = dataList[list[pos].owner].list[list[pos].heroId]
				if heroData then
					buff.headId = heroData.headId
					buff.name = heroData.name
					buff.aptLv = heroData.aptLv
					buff.job = heroData.job
					buff.hp = heroData.hpMax
					buff.hpMax = heroData.hpMax
					buff.physcAtkMax = heroData.physcAtkMax
					buff.physcAtkMin = heroData.physcAtkMin
					buff.magicAtkMax = heroData.magicAtkMax
					buff.magicAtkMin = heroData.magicAtkMin
					buff.magicDef = heroData.magicDef
					buff.physcDef = heroData.physcDef
					buff.speed = heroData.speed
				end
			end
			table.insert(sData, buff)
		end
		call_back(sData)
	end
	FIGHT.LoadFightDataList(userList, read_back)
end
-------------------------------------------
--查看使馆武将列表
local s_embassy_stayList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落，不能使用该操作。")
	end
	local embassy = tribe:GetStruct("embassy")
	if not embassy then
		return ERROR.TipMsg(connection, "您尚未建立使馆。")
	end
	local function call_back(sData)
		connection:SendProtocol("c_embassy_stayList", sData)
	end
	embassy:GetViewInfo(nil, call_back)	
end

--查看别人使馆武将列表
local s_embassy_viewList = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟，不能查看协防情况。")
	end
	if not union:GetMemberInfo(roleId) then
		return ERROR.TipMsg(connection, "相同联盟的玩家才能查看协防情况。")
	end
	local function get_back(list)
		local sData = {nick = data}
		sData.list = list
		connection:SendProtocol("c_embassy_viewList", sData)
	end
	local embassy = GetEmbassyObjById(roleId)
	if embassy then
		user:SetFocus("embassy", roleId)
		embassy:SetVisitor(connection.roleId, true)
		embassy:GetViewInfo(connection.roleId, get_back)
	else
		local function call_back(ret, msg)
			local conn = LOGIN.GetConnection(connection.roleId)
			if conn ~= connection then
				return
			end
			if not ret then
				return ERROR.TipMsg(connection, msg)
			end
			local embassy = GetEmbassyObjById(roleId)
			embassy:SetVisitor(connection.roleId, true)
			user:SetFocus("embassy", roleId)
			embassy:GetViewInfo(connection.roleId, get_back)
		end
		RestoreEmbassyData(roleId, call_back)
	end
end

--派遣武将进驻使馆
local s_embassy_send = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if connection.roleId == roleId then
		return ERROR.TipMsg(connection, "您不能在自己的使馆驻扎战魂。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local hero = user:GetHero(data.heroId)
	if not hero then
		return ERROR.TipMsg(connection, "没有指定的战魂信息。")
	elseif hero:GetWorkState() ~= "rest" then
		return ERROR.TipMsg(connection, string.format("指定的战魂%s不在空闲中。", hero:GetName()))
	elseif hero:GetRoleType() == "godHero" then
		return ERROR.TipMsg(connection, "神魂不能被派遣协防。")
	end
	local embassy = GetEmbassyObjById(roleId)
	if not embassy then
		return ERROR.TipMsg(connection, "您不在对方的使馆，不能进行协防。")
	end
	if embassy:HasSendHero(connection.roleId) then
		return ERROR.TipMsg(connection, "您已经有战魂在此驻防")
	end
	data.pos = tonumber(data.pos)
	if not data.pos or data.pos < 1 then
		return ERROR.TipMsg(connection, "无效的指定位置。")
	end
	local info = embassy:GetHeroStay(data.pos)
	if not info then
		return ERROR.TipMsg(connection, "该位置未开放")
	elseif info.heroId then
		return ERROR.TipMsg(connection, "指定的位置已经有别的战魂协防了。")
	end
	embassy:SetHeroStay(data.pos, connection.roleId, data.heroId)
	connection:SendProtocol("c_embassy_send", data)
end

--召回驻扎在使馆的武将
local s_embassy_recall = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	if connection.roleId == roleId then
		return ERROR.TipMsg(connection, "您不能在自己的使馆驻扎战魂。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local embassy = GetEmbassyObjById(roleId)
	if not embassy then
		return ERROR.TipMsg(connection, "您不在对方的使馆，不能进行协防。")
	end
	if not embassy:HasSendHero(connection.roleId) then
		return ERROR.TipMsg(connection, "您没有战魂在此驻防。")
	end
	data.pos = tonumber(data.pos)
	if not data.pos or data.pos < 1 then
		return ERROR.TipMsg(connection, "无效的指定位置。")
	end
	local info = embassy:GetHeroStay(data.pos)
	if not info then
		return ERROR.TipMsg(connection, "该位置未开放。")
	elseif info.owner ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不能操作别人的战魂。")
	end
	embassy:DelHeroStay(data.pos)
	connection:SendProtocol("c_embassy_recall", data)	
end

local s_embassy_leave = function(connection, data)
	if data ~= "leave" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if focus.type ~= "embassy" then
		return ERROR.TipMsg(connection, "您当前不在使馆。")
	end
	local embassy = GetEmbassyObjById(focus.id)
	user:SetFocus()
	if embassy then
		embassy:SetVisitor(connection.roleId)
	end
	connection:SendProtocol("c_embassy_leave", "true")
end

local s_embassy_kick = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local embassy = tribe:GetStruct("embassy")
	if not embassy then
		return ERROR.TipMsg(connection, "您还没有建立使馆")
	end
	local info = embassy:GetHeroStay(data)
	if not info or not info.heroId then
		return ERROR.TipMsg(connection, "该位置没有驻扎战魂")
	end
	embassy:DelHeroStay(data)
	connection:SendProtocol("c_embassy_kick", data)
end

function __init__()
	CALLOUT.CallFre(CheckOnlineEmbassy, 60)
	
	PROTOCOL.s_embassy_stayList = s_embassy_stayList
	PROTOCOL.s_embassy_kick = s_embassy_kick
	
	PROTOCOL.s_embassy_viewList = s_embassy_viewList
	PROTOCOL.s_embassy_send = s_embassy_send
	PROTOCOL.s_embassy_recall = s_embassy_recall
	PROTOCOL.s_embassy_leave = s_embassy_leave
end
