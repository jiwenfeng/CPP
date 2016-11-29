
local UNION_OPEN_LV = 20				--联盟开放等级
local CREATE_UNION_COST = 200000	--建立联盟的花费
local GOLD_TO_DEVOTE = 10000	--钱币对贡献
local INGOT_TO_DEVOTE = 1		--元宝对贡献

local MIN_WAIT_TIMELONG = 24 * 60 * 60	--加入公会冷却时间
local MAX_APPLY_NUM = 5					--玩家可同时申请联盟数量
local DELATE_NEED_OFFLINETIME = 3 * 24 * 3600	--弹劾要求离线时间
local DELATE_NEED_INGOT = 100			--弹劾盟主花费
local INVITE_NEED_INGOT = 20			--邀请花费

local ExchangeList = {}

local AllUnionList = {}
local path_union = "chaos.union"
local SAVE_UNION_TIME_LONG = 5 * 60		--保存联盟数据时间间隔

local PosList = {"盟主", "副盟主", "精英", "成员"}

local INVITE_TIMES = 5

function GetTotalInviteTimes()
	return INVITE_TIMES
end

function GetDataBase()
	return path_union
end

function GetAllUnionList()
	return AllUnionList
end

function GetUnionById(id)
	return AllUnionList[id]
end

function AddUnionObj(id, obj)
	AllUnionList[id] = obj
end

function AddAllUnionToRank()
	for k, v in pairs(GetAllUnionList()) do
		RANK.AddToRank("unionLv", v:GetId())
		RANK.AddToRank("unionForce", v:GetId())
	end
end

function ClearAllUnion()
	for k, v in pairs(GetAllUnionList()) do
		v:Dismiss()
	end
end

function GetAllExchangeList()
	return ExchangeList
end

function GetExchangeInfoById(id)
	return ExchangeList[id]
end

function GetUnionByName(name)
	for k, v in pairs(GetAllUnionList()) do
		if v:GetName() == name then
			return v
		end
	end
end

function FormateExchangeList()
	local sData = {}
	local list = GetAllExchangeList()
	for k, v in pairs(list) do
		local buf = {}
		buf.goodsId = k
		buf.needDevote = v.devote
		buf.needUnionLv = v.Lv
		table.insert(sData, buf)
	end
	table.sort(sData, function (a, b) return a.needUnionLv < b.needUnionLv
			or (a.needUnionLv == b.needUnionLv and a.goodsId < b.goodsId)
		end)
	return sData
end

--刷新玩家联盟信息
function RefreshUnionInfo(user)
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if union then
		union:RefreshInfo("member", user)
	else
		for _, v in pairs(GetAllUnionList()) do 
			v:RefreshInfo("apply", user)
		end
	end
end

function CalUserApplyCount(roleId)
	local num = 0
	for _, v in pairs(GetAllUnionList()) do
		if v:IsInList("apply", roleId) then
			num = num + 1
		end
	end
	return num
end

function RemoveUserApplyInfo(roleId)
	for _, v in pairs(GetAllUnionList()) do 
		if v:IsInList("apply", roleId) then
			v:RmFromList("apply", roleId)
		end
	end
end

function CreateUnion(user, info, announce)
	local roleId = user:GetRoleId()
	local id = STATS.GenId("union")
	local obj = CLSUNION.clsUnion:New()
	AllUnionList[id] = obj
	obj:initUnion(id, roleId, user:GetPhyle(), info, announce or "欢迎来到上古战歌~")
	
	obj:AddMember(roleId, CLSUNION.LEADER, true)
	RANK.AddToRank("unionLv", id)
	RANK.AddToRank("unionForce", id)
	local buff = obj:GetSaveData()
	buff._id = id
	DATA.UpdateData(path_union, {_id = id}, buff)
end

function RemoveUnion(id)
	AllUnionList[id] = nil
	DATA.DBRemove(path_union, {_id = id})
	RANK.RmFromRank("unionLv", "unionId", id)
	RANK.RmFromRank("unionForce", "unionId", id)
end

function IsUnionNameExist(name)
	for k, v in pairs(GetAllUnionList()) do
		if name == v:GetName() then
			return true
		end
	end
	return false
end

function GetFormatUnionList(roleId)
	local sData = {}
	for k, v in pairs(GetAllUnionList()) do
		local buff = v:GetUnionInfo()
		local info = v:GetApplyInfo(roleId)
		if info and CalDiffDays(os.time(), info.timestamp) >= 3 then		--超过三天的取消申请
			v:RmFromList("apply", roleId)
		end
		buff.joinState = v:IsInList("apply", roleId) and 1 or 0
		table.insert(sData, buff)
	end
	table.sort(sData, function(a, b) return
		a.rank < b.rank
		or (a.rank == b.rank and a.force > b.force)
		or (a.rank == b.rank and a.force == b.force and a.num > b.num)
	end)
	return sData
end

function GetUnionDetail(union)
	local sData = {}
	local unionId = union:GetId()
	local rankObj = RANK.GetRankObjByType("real", "unionLv")
	sData.rank = rankObj:IsDataExist({unionId = unionId}) or -1
	sData.unionId =unionId
	sData.name = union:GetName()
	sData.leader = USER.GetUserNickById(union:GetLeader())
	sData.Lv = union:GetLv()
	sData.num = union:GetMemberNum()
	sData.max = union:GetMaxMemberNum()
	sData.phyle = union:GetPhyle()
	sData.unionId = union:GetId()
	sData.force = union:GetTotalForce()
	sData.wealth = union:GetWealth()
	sData.announce = union:GetAnnounce()
	return sData
end

--弹劾
function DelateLeader(union, user)
	local leader = union:GetLeader()
	if USER.GetOnlineUser(leader) then
		return false, "盟主在线期间，不能进行弹劾。"
	end
	local info = union:GetMemberInfo(leader)
	if os.time() - info.timestamp < DELATE_NEED_OFFLINETIME then
		return false, string.format("盟主离线时间少于%d天，无法弹劾。", math.ceil(DELATE_NEED_OFFLINETIME / 3600 / 24))
	end
	if user:GetTotalIngot() < DELATE_NEED_INGOT then
		return false, string.format("弹劾盟主需要%d元宝，您元宝数量不足。", DELATE_NEED_INGOT)
	end
	user:SetTotalIngot(-DELATE_NEED_INGOT)
	union:ModMemberIdentity(leader, CLSUNION.ELITE)
	union:ModMemberIdentity(user:GetRoleId(), CLSUNION.LEADER)

	local mail = {roleId = SYSTEM_ID, time = os.time(), type = "sys"}
	mail.title = "弹劾通知"
	mail.content = string.format("在您的离线期间，%s通过弹劾取代了您的盟主位置。\n", user:GetName())
	MAIL.SendSysMail(leader, mail)
	return true
end

--恢复所有联盟数据
function RestoreAllUnionData()
	local function CallBack(Data, ErrMsg)
		Data = UnSerialize(Data)
		for _, v in pairs(Data) do
			local unionObj = CLSUNION.clsUnion:New()
			unionObj:Restore(v)
			AllUnionList[v._id] = unionObj
			unionObj:CheckDelateInfo()
		end
	end
	DATA.Read(path_union, {}, CallBack)
end

--[[
--保存联盟数据
function SaveAllUnionData()
	for k, v in pairs(AllUnionList) do
		local buff = v:GetSaveData()
		buff._id = k
		DATA.UpdateData(path_union, {_id = k}, buff)
	end
end]]

function SaveAllUnionData()
	for k, v in pairs(AllUnionList) do
		if v:GetMemberNum() <= 0 then
			v:Dismiss()
		else
			RANK.AddToRank("unionLv", k)
			RANK.AddToRank("unionForce", k)
			v:SaveUnionData()
		end
	end
end

--[[
function DismissAllUnion()
	for k, v in pairs(AllUnionList) do
		v:Dismiss()
	end
end]]

--关闭服务器时保存联盟数据
function ShutdownSaveAllUnion()
	for k, v in pairs(AllUnionList) do
		for k, v in pairs(v:GetUniActList()) do
			v:ShutDownProAct()
		end
		local buff = v:GetSaveData()
		buff._id = k
		DATA.UpdateData(path_union, {_id = k}, buff)
	end
end


function UniRelicBossReset()
	for k, v in pairs(GetAllUnionList()) do
		local uniRelic = v:GetUnionAct("uniRelic")
		if uniRelic then
			uniRelic:ResetBoss()
		end
	end
end

function UniRelicBossActOpen()
	for k, v in pairs(GetAllUnionList()) do
		local uniRelic = v:GetUnionAct("uniRelic")
		if uniRelic and not uniRelic:IsBossReleased() then
			uniRelic:BossActReady()
		end
	end
end

-----------------------------------------------
--获取联盟列表
local s_union_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatUnionList(connection.roleId)
	connection:SendProtocol("c_union_list", sData)
end

local s_union_view = function(connection, data)
	local union = GetUnionById(data)
	if not union then
		return ERROR.TipMsg(connection, "没有指定的联盟信息。")
	end
	local sData = GetUnionDetail(union)
	connection:SendProtocol("c_union_view", sData)
end
--联盟信息
local s_union_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "无效的联盟编号")
	end
	local sData = union:GetUnionInfo(connection.roleId)
	connection:SendProtocol("c_union_info", sData)
end

--获取成员列表
local s_union_memberList = function(connection, data)
	if data ~= "member" and data ~= "apply" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "无效的联盟编号")
	end
	local sData = {type = data}
	sData.list = union:FormatMemberList(data)
	if data == "apply" then
		sData.joinable = union:IsJoinable() and 1 or 0
	end
	connection:SendProtocol("c_union_memberList", sData)
end
--创建联盟
local s_union_create = function(connection, data)
	--if not data or data == "" then
	if type(data) ~= "table" or not data.name or not data.picId then
		return ERROR.TipMsg(connection, "请输入联盟名称。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能建立联盟。")
	end
	if user:GetLv() < UNION_OPEN_LV then
		return ERROR.TipMsg(connection, string.format("您的等级还未达到%d级，不能创建联盟。", UNION_OPEN_LV))
	end
	if user:GetGold() < CREATE_UNION_COST then
		return ERROR.TipMsg(connection, string.format("建立联盟需要%d钱币。", CREATE_UNION_COST))
	end
	if user:GetUnionId() then
		return ERROR.TipMsg(connection, "您已经加入联盟了。")
	end
	if NEWUSER.NameIsIllegal(data.name) then
		return ERROR.TipMsg(connection, "该名字含有敏感字符。")
	end
	if IsUnionNameExist(data.name) then
		return ERROR.TipMsg(connection, "该联盟名称已经存在。")
	end
	user:SetGold(-CREATE_UNION_COST)
	CreateUnion(user, data)
	connection:SendProtocol("c_union_create", user:GetUnionId())
end

--加入联盟
local s_union_join = function(connection, data)
	local union = GetUnionById(data)
	if not union then
		return ERROR.TipMsg(connection, "没有指定的联盟信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能加入联盟。")
	end
	if user:GetLv() < UNION_OPEN_LV then
		return ERROR.TipMsg(connection, string.format("只有%d级以上玩家才能加入联盟。", UNION_OPEN_LV))
	end
	if GetUnionById(user:GetUnionId()) then
		return ERROR.TipMsg(connection, "您已经加入联盟了。")
	end
	if union:GetPhyle() ~= user:GetPhyle() then
		return ERROR.TipMsg(connection, "您不能加入其他势力的联盟。")
	end
	if not union:IsJoinable() then
		return ERROR.TipMsg(connection, "该联盟已经关闭了加入申请。")
	end
	if union:GetMemberNum() >= union:GetMaxMemberNum() then
		return ERROR.TipMsg(connection, "该联盟人数已满。")
	end
	if union:IsInList("apply", connection.roleId) then
		return ERROR.TipMsg(connection, "您已经在该联盟的申请列表了。")
	end
	if union:GetApplyNum() >= union:GetApplyMax() then
		return ERROR.TipMsg(connection, "该联盟的申请人数已经达到上限。")
	end
	if CalUserApplyCount(connection.roleId) >= MAX_APPLY_NUM then
		return ERROR.TipMsg(connection, "您发出的申请已经达到上限。")
	end
	local quitTime = user:GetUnionQuitTime()
	if quitTime and (os.time() - quitTime <= MIN_WAIT_TIMELONG) then
		return ERROR.TipMsg(connection, string.format("您退出联盟时间未满%d小时，不能再次加入联盟", MIN_WAIT_TIMELONG / 3600))
	end
	union:AddToApplyList(connection.roleId)
	connection:SendProtocol("c_union_join", data)
end

local s_union_retrieve = function(connection, data)
	local union = GetUnionById(data)
	if not union then
		return ERROR.TipMsg(connection, "没有指定的联盟信息。")
	end
	union:RmFromList("apply", connection.roleId)
	connection:SendProtocol("c_union_retrieve", data)
end

--捐献
local s_union_donate = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data.gold = tonumber(data.gold)
	data.ingot = tonumber(data.ingot)
	if not data.gold and not data.ingot then
		return ERROR.TipMsg(connection, "请输入要捐献的钱币或元宝数量。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local devote = 0
	if data.gold then
		if data.gold < GOLD_TO_DEVOTE then
			return ERROR.TipMsg(connection, string.format("捐赠钱币的数量不能小于%d", GOLD_TO_DEVOTE))
		elseif user:GetGold() < data.gold then
			return ERROR.TipMsg(connection, "您的钱币不足。")
		end
		devote = math.floor(data.gold / GOLD_TO_DEVOTE)
	end
	if data.ingot then
		if data.ingot < INGOT_TO_DEVOTE then
			return ERROR.TipMsg(connection, string.format("捐赠元宝的数量不能少于%d", INGOT_TO_DEVOTE))
		elseif user:GetTotalIngot() < data.ingot then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
		end
		devote = devote + math.floor(data.ingot / INGOT_TO_DEVOTE)
	end
	union:Donate(connection.roleId, devote)
	if data.gold then
		user:SetGold(-data.gold)
	end
	if data.ingot then
		user:SetTotalIngot(-data.ingot)
	end
	data.devote = devote
	connection:SendProtocol("c_union_donate", data)
end
--退出
local s_union_quit = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您还没有加入联盟")
	end
	if union:GetLeader() == connection.roleId then
		return ERROR.TipMsg(connection, "您是盟主，不能退出联盟")
	end
	user:QuitUnion()
	local delateInfo = union:GetDelateInfo()
	if delateInfo and delateInfo.roleId == roleId then
		union:CancelDelate(true)
	end
	union:SendUnionMsg(string.format("%s离开联盟", user:GetName()), true)
	connection:SendProtocol("c_union_quit", union:GetDelateInfo() and 1 or 0)
end

--弹劾
local s_union_delate = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
--[[	if CalDiffDays(info.joinTime, os.time()) < 3 then
		return ERROR.TipMsg(connection,"只有加入联盟超过3天的成员才能弹劾盟主")
	end]]
	local leader = union:GetLeader()
	if USER.GetOnlineUser(leader) then
		return ERROR.TipMsg(connection, "盟主在线期间，不能进行弹劾。")
	end
	if union:GetDelateInfo() then
		return ERROR.TipMsg(connection, "盟主已经处于弹劾状态")
	end
	local info = union:GetMemberInfo(leader)
--[[	if os.time() - info.timestamp < DELATE_NEED_OFFLINETIME then
		return ERROR.TipMsg(connection, string.format("盟主离线时间少于%d天，无法弹劾。", math.ceil(DELATE_NEED_OFFLINETIME / 3600 / 24)))
	end]]
	if user:GetTotalIngot() < DELATE_NEED_INGOT then
		return ERROR.TipMsg(connection, string.format("弹劾盟主需要%d元宝，您元宝数量不足。", DELATE_NEED_INGOT))
	end
	user:SetTotalIngot(-DELATE_NEED_INGOT)
	union:BeginDelate(connection.roleId)
	connection:SendProtocol("c_union_delate", "true")
end

--同意申请
local s_union_agree = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作")
	end
	if not union:IsJoinable() then
		return ERROR.TipMsg(connection, "联盟不允许加入成员")
	end
	if union:GetMemberNum() >= union:GetMaxMemberNum() then
		return ERROR.TipMsg(connection, "联盟人数已达上限。")
	end
	local msg
	if data ~= "all" then
		local roleId = USER.GetRoleIdByNick(data)
		if not roleId then
			return ERROR.TipMsg(connection, "没有该玩家信息。")
		end
		local unionId = USER.GetUserUnionById(roleId)
		if GetUnionById(unionId) then
			return ERROR.TipMsg(connection, "该玩家已经加入其他联盟了。")
		end
		union:AddMember(roleId, CLSUNION.ELITE)
	else
		for k in pairs(union:GetListByType("apply")) do 
			if union:GetMemberNum() >= union:GetMaxMemberNum() then
				msg = "联盟人数已达上限。"
				break
			end
			local unionId = USER.GetUserUnionById(k)
			if GetUnionById(unionId) then
				union:RmFromList("apply", k)
				ERROR.TipMsg(connection, string.format("玩家【%s】已经加入其他联盟了。", USER.GetUserNickById(k)))
			else
				union:AddMember(k, CLSUNION.ELITE)
			end
		end
	end
	connection:SendProtocol("c_union_agree", {type = data, msg = msg})
	if data == "all" then
		PROTOCOL.s_union_memberList(connection, "apply")
	end
end
--拒绝申请
local s_union_refuse = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作")
	end
	if data ~= "all" then
		local roleId = USER.GetRoleIdByNick(data)
		if not roleId then
			return ERROR.TipMsg(connection, "没有该玩家信息。")
		end
		union:RmFromList("apply", roleId)
	else
		for k in pairs(union:GetListByType("apply")) do 
			union:RmFromList("apply", k)
		end
	end
	connection:SendProtocol("c_union_refuse", data)
	if data == "all" then
		PROTOCOL.s_union_memberList(connection, "apply")
	end
end

--修改公告
local s_union_announce = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	data = tostring(data)
	if string.len(data) == 0 then
		return ERROR.TipMsg(connection, "公告内容不能为空")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作")
	end
	union:SetAnnounce(data)
	connection:SendProtocol("c_union_announce", data)
end

--升级
local s_union_upgrade = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作")
	end
	local lv = union:GetLv()
	if lv ~= tonumber(data) then
		return ERROR.TipMsg(connection, string.format("请求错误，当前联盟等级为%d级。", union:GetLv()))
	end
	if lv >= union:GetMaxLv() then
		return ERROR.TipMsg(connection, "联盟已经达到最高等级。")
	end
	local lvInfo = CLSUNION.GetUnionInfoByLv(lv)
	if union:GetWealth() < lvInfo.wealth then
		return ERROR.TipMsg(connection, "联盟财富值不够，无法升级")
	end
	union:SetLv(1)
	union:SetWealth(-lvInfo.wealth)
	union:SendUnionMsg(string.format("联盟已经升到%d级", union:GetLv()))
	connection:SendProtocol("c_union_upgrade", union:GetLv())
	union:SendUnionUpdate("Lv")
end


-- 改变职位
local s_union_changePos = function(connection, data)
	if type(data) ~= "table" or not data.nick or not data.identity then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "该用户不存在")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "不能对自己进行此操作")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local myInfo = union:GetMemberInfo(connection.roleId)
	if myInfo.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此项操作")
	end
	if myInfo.identity ~= CLSUNION.LEADER and myInfo.identity >= data.identity then
		return ERROR.TipMsg(connection, "你没有权力任命该职位。")
	end
	local hisInfo = union:GetMemberInfo(roleId)
	if not hisInfo then
		return ERROR.TipMsg(connection, "不能操作联盟以外的玩家。")
	elseif hisInfo.identity == data.identity then
		return ERROR.TipMsg(connection, string.format("成员已经是%s了。", PosList[data.identity]))
	end
	if myInfo.identity >= hisInfo.identity then
		return ERROR.TipMsg(connection, "您没有权力操作该职位的玩家。")
	end
	local msg
	if data.identity == CLSUNION.LEADER then
		union:ModMemberIdentity(connection.roleId, CLSUNION.ELITE)
		user:SetUnionInfo(unionId, CLSUNION.ELITE)
		msg = string.format("恭喜【%s】成为新的盟主。", data.nick)
	else
		if data.identity == CLSUNION.VICELEADER and union:GetViceLeaderNum() >= union:GetViceLeaderMax() then
			return ERROR.TipMsg(connection, "副盟主人数已满。")
		end
		msg = string.format("%s将%s%s成为%s。", user:GetName(), data.nick, data.identity < hisInfo.identity and "提升" or "降职", PosList[data.identity])
	end
	union:ModMemberIdentity(roleId, data.identity)
	local tData = {opNick = user:GetName(), identity = data.identity, act = data.identity < hisInfo.identity and 1 or 0}
	local tUser = USER.GetOnlineUser(roleId)
	if tUser then
		tUser:SetUnionInfo(unionId, data.identity)
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_union_posChaged", tData)
		end
	else
		OFFLINE.SetOfflineData(roleId, "setUnionInfo", unionId, data.identity)
	end
	union:SendUnionMsg(msg)
	local sData = {nick = data.nick, identity = data.identity}
	if data.identity == CLSUNION.LEADER then
		sData.curIdentt = CLSUNION.ELITE
		union:SendUnionUpdate("leader")
		--OFFICIAL.AssignOfficialPos(user, roleId)		--转让盟主的时候同时转让官职
	end
	connection:SendProtocol("c_union_changePos", sData)
end
--解散
local s_union_dismiss = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	if union:GetLeader() ~= connection.roleId then
		return ERROR.TipMsg(connection, "只有盟主才能解散联盟")
	end
	if union:GetMemberNum() > 3 then
		return ERROR.TipMsg(connection, "联盟人数少于3人的时候才能解散联盟")
	end
	union:SendUnionMsg(string.format("联盟【%s】已被解散", union:GetName()))
	union:Dismiss()
	connection:SendProtocol("c_union_dismiss", "true")
end

--修改申请许可
local s_union_permission = function(connection, data)
	if data ~= 0 and data ~= 1 then
		return ERROR.TipMsg(connection, "无效请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您还没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主能进行此操作")
	end
	union:SetJoinable(data == 1)
	if data == 0 then		--清空申请列表
		for k in pairs(union:GetListByType("apply")) do 
			union:RmFromList("apply", k)
		end
	end
	connection:SendProtocol("c_union_permission", data)
end

--移除
local s_union_remove = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求。")
	end
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "没有该玩家信息。")
	end
	if roleId == connection.roleId then
		return ERROR.TipMsg(connection, "不能对自己执行此操作。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您还没有加入联盟。")
	end
	if not union:IsInList("member", roleId) then
		return ERROR.TipMsg(connection, "没有该成员信息。")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有盟主和副盟主才能进行此操作。")
	end
	local tUser = USER.GetOnlineUser(roleId)
	if tUser then
		tUser:QuitUnion(0)
	else
		union:RmFromList("member", roleId)
		OFFLINE.SetOfflineData(roleId, "quitUnion", 0)
	end
	connection:SendProtocol("c_union_remove", data)
	local delateInfo = union:GetDelateInfo()
	if delateInfo and delateInfo.roleId == roleId then
		union:CancelDelate(true)
	end
	union:SendUnionMsg(string.format("%s被%s%s移出了联盟。", data, PosList[info.identity], user:GetName()), true)
end

--邀请信息
local s_union_inviteMsg = function(connection, data)
	--if type(data) ~= "table" or not data.type or not data.msg then
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您还没有加入联盟。")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.VICELEADER then
		return ERROR.TipMsg(connection, "只有副盟主及以上的成员才能邀请")
	end	
	if union:GetInviteTimes() >= INVITE_TIMES then
		return ERROR.TipMsg(connection, "联盟今天的邀请次数已经用完")
	end
	if not union:IsJoinable() then
		return ERROR.TipMsg(connection, "联盟目前已经关闭了入盟申请。")
	end
	if union:GetMemberNum() >= union:GetMaxMemberNum() then
		return ERROR.TipMsg(connection, "联盟人数已满。")
	end
	union:SetInviteTimes(1)
	local msg = string.format("联盟【%s】即将大展宏图制霸一方，诚邀天下能人异士相助", union:GetName())
	CHAT.SendSysMsg("world", msg, nil, {kind = "unionInvite", id = unionId})
	connection:SendProtocol("c_union_inviteMsg", math.max(INVITE_TIMES - union:GetInviteTimes(), 0))
end

local s_union_invite = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, "该用户不存在")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity > CLSUNION.ELITE then
		return ERROR.TipMsg(connection, "只有精英及以上的成员才能邀请")
	end
	if not union:IsJoinable() then
		return ERROR.TipMsg(connection, "联盟拒绝加入，无法邀请")
	end
	if union:GetMemberNum() >= union:GetMaxMemberNum() then
		return ERROR.TipMsg(connection, "联盟人数已满")
	end
	if union:GetApplyNum() >= union:GetApplyMax() then
		return ERROR.TipMsg(connection, "联盟申请列表已满。")
	end
	local tUser = USER.GetOnlineUser(roleId)
	if not tUser then
		return ERROR.TipMsg(connection, "该玩家已经下线")
	end
	if tUser:GetUnionId() then
		return ERROR.TipMsg(connection, "对方已经加入联盟。")
	end
	if tUser:GetPhyle() ~= union:GetPhyle() then
		return ERROR.TipMsg(connection, "不能邀请其他势力的玩家。")
	end
	if info.identity <= CLSUNION.VICELEADER and union:IsInList("apply", roleId) then
		union:AddMember(roleId, CLSUNION.ELITE)
	else
		local function invite_ask()
			local co = user:GetUnionInviteCort(roleId)			
			local sData = {nick = user:GetName(), unionName = union:GetName()}
			local con = LOGIN.GetConnection(roleId)
			if con and con.roleId == roleId then
				con:SendProtocol("c_union_inviteAsk", sData)
			end
			local chosen = coroutine.yield(co)
			if chosen == 1 then
				if info.identity <= CLSUNION.VICELEADER then
					if union:GetMemberNum() >= union:GetMaxMemberNum() then
						return
					end
					union:AddMember(roleId, CLSUNION.ELITE)
				else
					if union:GetApplyNum() >= union:GetApplyMax() then
						return
					end
					union:AddToApplyList(roleId)
				end
			end
			user:SetUnionInviteCort(roleId)
		end
		local co = coroutine.create(invite_ask)
		user:SetUnionInviteCort(roleId, co)
		coroutine.resume(co)
	end
	connection:SendProtocol("c_union_invite", "true")
end

local s_union_inviteReply = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local me = assert(USER.GetOnlineUser(connection.roleId))
	if not me:GetPhyle() then
		return ERROR.TipMsg(connection, "您尚未选择势力，不能加入联盟。")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "无效的玩家信息。")
	end
	if data.chosen ~= 1 and data.chosen ~= 0 then
		return ERROR.TipMsg(connection, "无效的选择类型。")
	end
	local user = USER.GetOnlineUser(roleId)
	if not user or not user:GetUnionInviteCort(connection.roleId) then
		return ERROR.TipMsg(connection, "该邀请已经失效。")
	end
	if CalUserApplyCount(connection.roleId) >= MAX_APPLY_NUM then
		return ERROR.TipMsg(connection, "您发出的申请已经达到上限。")
	end
	local function pro_invite()
		local co = user:GetUnionInviteCort(connection.roleId)
		if co and coroutine.status(co) == "suspended" then
			coroutine.resume(co, data.chosen)
		end
	end
	return pro_invite()
end

--兑换列表
local s_union_exchgList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local sData = FormateExchangeList()
	connection:SendProtocol("c_union_exchgList", sData)
end

--兑换
local s_union_exchange = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local union = GetUnionById(user:GetUnionId())
	if not union then
		return ERROR.TipMsg(connection, "您没有加入联盟")
	end
	local info = GetExchangeInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有该物品的兑换信息。")
	end
	if union:GetLv() < info.Lv then
		return ERROR.TipMsg(connection, "联盟等级不足，暂时不能兑换该物品。")
	end
	if union:GetDevote(connection.roleId, "enable") < info.devote then
		return ERROR.TipMsg(connection, "您的贡献度不足，不能进行兑换。")
	end
	local item = BASEITEM.NewItem(data, 1, {lock = 1})
	if not user:IsItemCanAdd(USERBASE.PACKAGE, item) then
		return ERROR.TipMsg(connection, "您的背包已满。")
	end
	user:AddItem(USERBASE.PACKAGE, item)
	union:SetDevote(connection.roleId, "enable", -info.devote)
	user:SendProtocol("c_show_tips", string.format("获得%s x %d", item:GetName(), item:GetNum()))
	connection:SendProtocol("c_union_exchange", {goodsId = data, devote = union:GetDevote(connection.roleId, "enable")})
end

local s_union_actList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local sData = union:GetUnionActList()
	connection:SendProtocol("c_union_actList", sData)
end

local s_union_delateInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local sData = union:FormatDelateInfo()
	connection:SendProtocol("c_union_delateInfo", sData)
end

local s_union_agreeDelate = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	if not union:GetDelateInfo() then
		return ERROR.TipMsg(connection, "当前没有弹劾信息")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity == CLSUNION.LEADER then
		return ERROR.TipMsg(connection, "您是盟主，不能支持弹劾")
	end
	if union:IsRoleJoinDelate(connection.roleId) then
		return ERROR.TipMsg(connection, "您已经参与弹劾盟主")
	end
	union:AddDelateMember(connection.roleId)
	connection:SendProtocol("c_union_agreeDelate", "true")
end

local s_union_cancelDelate = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local unionId = user:GetUnionId()
	local union = GetUnionById(unionId)
	if not union then
		return ERROR.TipMsg(connection, "您尚未加入联盟。")
	end
	local delateInfo = unoin:GetDelateInfo()
	if not delateInfo  then
		return ERROR.TipMsg(connection, "您所在的联盟没有弹劾盟主")
	end
	local info = union:GetMemberInfo(connection.roleId)
	if info.identity ~= CLSUNION.LEADER or delateInfo.roleId ~= connection.roleId then
		return ERROR.TipMsg(connection, "只有盟主或弹劾发起者才能取消弹劾")
	end
	union:CancelDelate()
	connection:SendProtocol("c_union_cancelDelate", "true")
end

function __init__()

	RestoreAllUnionData()
	
	local EXCHANGELIST = Import("autocode/unionExchange.lua")
	ExchangeList = EXCHANGELIST.GetTable()
	
	PROTOCOL.s_union_list = s_union_list
	PROTOCOL.s_union_view = s_union_view
	PROTOCOL.s_union_info = s_union_info
	PROTOCOL.s_union_memberList = s_union_memberList
	
	PROTOCOL.s_union_create = s_union_create
	PROTOCOL.s_union_join = s_union_join
	PROTOCOL.s_union_retrieve = s_union_retrieve
	PROTOCOL.s_union_donate = s_union_donate
	PROTOCOL.s_union_quit = s_union_quit
	
	PROTOCOL.s_union_agree = s_union_agree
	PROTOCOL.s_union_refuse = s_union_refuse
	PROTOCOL.s_union_changePos = s_union_changePos
	PROTOCOL.s_union_announce = s_union_announce
	PROTOCOL.s_union_upgrade = s_union_upgrade
	PROTOCOL.s_union_delate = s_union_delate
	PROTOCOL.s_union_remove = s_union_remove
	PROTOCOL.s_union_dismiss = s_union_dismiss

	PROTOCOL.s_union_permission = s_union_permission
	
	PROTOCOL.s_union_inviteMsg = s_union_inviteMsg
	PROTOCOL.s_union_invite = s_union_invite
	PROTOCOL.s_union_inviteReply = s_union_inviteReply
	
	PROTOCOL.s_union_exchgList = s_union_exchgList
	PROTOCOL.s_union_exchange = s_union_exchange
	PROTOCOL.s_union_actList = s_union_actList

	PROTOCOL.s_union_delateInfo = s_union_delateInfo
	PROTOCOL.s_union_agreeDelate = s_union_agreeDelate
	PROTOCOL.s_union_cancelDelate = s_union_cancelDelate

	CALLOUT.CallFre(SaveAllUnionData, SAVE_UNION_TIME_LONG)
	CALLOUT.Weekly(getfenv(2), 2, 0, 0, UniRelicBossReset)			--每周一 0点重置联盟boss
	CALLOUT.Weekly(getfenv(2), 1, 22, 50, UniRelicBossActOpen)		--每周日 强制释放boss
end
