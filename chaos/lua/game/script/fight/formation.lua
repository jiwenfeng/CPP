--阵法系统

local AllFormationList = {}
local NONE_FORMATION = "321001"		--【无】阵法id
local INIT_FORMATION = "321002"		--玩家初始默认阵法id

function GetAllFormationList()
	return AllFormationList
end

function GetFormationInfoById(id)
	return AllFormationList[id]
end

---------------------------------------------grid--------------------------------------------------
clsGrid = clsObject:Inherit()

GRID_CLOSE = 0		--不可用
GRID_LOCK = 1		--可解锁
GRID_OPEN = 2		--已开启

function clsGrid:__init__()
	Super(self).__init__(self)
	self.__save = {}
	self.__temp = {}
	return true
end

function clsGrid:InitGrid(id, state, openLv)
	self:SetId(id)
	self:SetState(state)
	self:SetOpenLv(openLv)
end

function clsGrid:SetId(id)
	self.__save.id = id
end

function clsGrid:GetId()
	return self.__save.id
end

-- state: 0 close / 1 lock / 2 open
function clsGrid:SetState(state)
	self.__save.state = state
end

function clsGrid:GetState()
	return self.__save.state
end

function clsGrid:SetRoleId(roleId)
	self.__save.roleId = roleId
end

--此位置的武将
function clsGrid:GetRoleId()
	return self.__save.roleId
end

function clsGrid:SetRoleOwner(userId)
	self.__save.roleOwner = userId
end

function clsGrid:GetRoleOwner()
	return self.__save.roleOwner
end


function clsGrid:SetOpenLv(lv)
	self.__temp.openLv = lv
end

function clsGrid:GetOpenLv()
	return self.__temp.openLv
end

function clsGrid:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsGrid:Restore(data)
	assert(type(data) == "table")
	self.__save = data
end

function NewGrid(id, state, openLv)
	local grid = clsGrid:New()
	grid:InitGrid(id, state)
	grid:SetOpenLv(openLv)
	return grid
end
---------------------------------------------formation---------------------------------------------
clsFormation = clsObject:Inherit()

FORM_LOCK = 0	--阵型锁定
FORM_OPEN = 1	--阵型开启
FORM_USE = 2	--正在使用

function clsFormation:__init__()
	Super(clsFormation).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsFormation:initsavedata()
	self.__save.Lv = 0
	self.__save.list = {}
end

function clsFormation:inittempdata()
	self.__temp.list = {}
end

function clsFormation:InitForm(id, Lv, state, roleId, roleOwner)
	self:SetId(id)
	self:SetState(state or FORM_LOCK)
	
	self:SetLv(Lv or 1)
	self:InitGridList()
	if roleId then
		local pos = self:GetInitPos()
		local grid = self:GetGrid(pos)
		grid:SetRoleId(roleId)
		grid:SetRoleOwner(roleOwner)
	end
end

function clsFormation:GetId()
	return self.__save.id
end

function clsFormation:SetId(id)
	id = tostring(id)
	self.__save.id = id
end

function clsFormation:AddGrid(pos, grid)
	pos = tostring(pos)
	self.__temp.list[pos] = grid
end

function clsFormation:GetGrid(pos)
	pos = tostring(pos)
	return self.__temp.list[pos]
end

function clsFormation:GetGridList()
	return self.__temp.list
end

function clsFormation:InitGridList()
	local Lv = self:GetLv() or 1
	for i = 1, 9 do
		local pos = tostring(i)
		local openLv = self:GetGridOpenLv(pos) or -1
		local state = GRID_LOCK
		if openLv == -1 then
			state = GRID_CLOSE
		elseif openLv <= Lv then
			state = GRID_OPEN
		end
		local grid = NewGrid(pos, state, openLv)
		self:AddGrid(pos, grid)
	end
end

function clsFormation:Upgrade()
	self:SetLv(1)
	local info = GetFormationInfoById(self:GetId())
	for k, v in pairs(self:GetPosList(self:GetLv())) do
		local grid = self:GetGrid(v)
		grid:SetState(GRID_OPEN)
	end
end

function clsFormation:SetLv(data)
	self.__save.Lv = (self.__save.Lv or 0) + data
	if self.__save.Lv < 1 then
		self.__save.Lv = 1
	elseif self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	end
end

function clsFormation:GetLv()
	return self.__save.Lv or 0
end

function clsFormation:SetState(state)
	self.__save.state = state
end

function clsFormation:GetState()
	return self.__save.state
end

function clsFormation:GetName()
	local info = GetFormationInfoById(self:GetId())
	return info.name
end

function clsFormation:GetRestraint()
	local info = GetFormationInfoById(self:GetId())
	return info.restraint
end

function clsFormation:GetDescribe(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end
	return info.list[Lv].describe
end

function clsFormation:GetExplain()
	local info = GetFormationInfoById(self:GetId())
	return info.explain
end

function clsFormation:GetRestraintDesc()
	local info = GetFormationInfoById(self:GetId())
	local desc = ""
	for k, v in pairs(info.restraint or {}) do
		local formInfo = GetFormationInfoById(k)
		if formInfo then
			desc = desc == "" and desc or (desc .. "    ")
			desc = desc .. string.format("对【%s】伤害提高%d%%", formInfo.name, v * 100)
		end
	end
	return desc
end

--获取阵型属性
function clsFormation:GetAttr(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return {}
	end
	local open, taken = 0, 0			--上阵人数影响阵型效果数值
	for k, v in pairs(self:GetGridList()) do
		if v:GetState() == GRID_OPEN then
			open = open + 1
		end
		if v:GetRoleId() then
			taken = taken + 1
		end
	end
	local ret = {}
	for k, v in pairs(info.list[Lv].attr) do
		ret[k] = math.ceil(v * taken / open)
	end
	return ret
end

function clsFormation:GetUpgradeGold(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end	
	return info.list[Lv].needGold
end

function clsFormation:GetUpgradeNimbus(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end	
	return info.list[Lv].needNimbus
end


function clsFormation:GetUpgradeItem(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end	
	return info.list[Lv].needItem
end

function clsFormation:GetLinkRate(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end	
	return info.list[Lv].linkRate
end

function clsFormation:GetLinkSkill()
	local info = GetFormationInfoById(self:GetId())
	return info.linkSkill
end

function clsFormation:GetOpenGold()
	local info = GetFormationInfoById(self:GetId())
	return info.needGold
end

function clsFormation:GetOpenNimbus()
	local info = GetFormationInfoById(self:GetId())
	return info.needNimbus
end

function clsFormation:GetOpenLv()
	local info = GetFormationInfoById(self:GetId())
	return info.needLv
end

function clsFormation:IsOpenNeedCity()
	local info = GetFormationInfoById(self:GetId())
	return info.needCity
end

function clsFormation:GetInitPos()
	local info = GetFormationInfoById(self:GetId())
	return info.initPos	
end

function clsFormation:GetPosList(Lv)
	Lv = Lv or self:GetLv()
	local info = GetFormationInfoById(self:GetId())
	if Lv > table.size(info.list) then
		return 
	end	
	return info.list[Lv].posList
end

--获取格子开放等级
function clsFormation:GetGridOpenLv(grid)
	grid = tostring(grid)
	local info = GetFormationInfoById(self:GetId())
	for k, v in ipairs(info.list) do
		for _, id in ipairs(v.posList) do
			if id == grid then
				return k
			end
		end
	end
end

function clsFormation:GetMaxLv()
	local info = GetFormationInfoById(self:GetId())
	return #info.list
end

function clsFormation:IsNeedCity()
	local info = GetFormationInfoById(self:GetId())
	return info.isNeedCity
end

function clsFormation:GetSaveData()
	self.__save.list = {}
	for k, v in pairs(self:GetGridList()) do
		self.__save.list[k] = v:GetSaveData()
	end
	return table.deepcopy(self.__save)
end

function clsFormation:Restore(data)
	self.__save = data
	for k, v in pairs(data.list) do
		local grid = NewGrid(k, v.state)
		grid:Restore(v)
		local openLv = self:GetGridOpenLv(id) or -1
		grid:SetOpenLv(openLv)
		self:AddGrid(k, grid)
	end
end

function clsFormation:FormatGridList()
	local sData = {}
	for k, v in pairs(self:GetGridList()) do
		local grid = self:GetGrid(k)
		local buf = {}
		buf.openLv = grid:GetOpenLv()
		buf.pos = grid:GetId()
		buf.state = grid:GetState()
		buf.id = grid:GetRoleId()
	--	if buf.id then
	--		buf.modelId = USER.GetUserNickInfo(buf.id, "modeldId")
	--	end
		buf.owner = grid:GetRoleOwner()
		table.insert(sData, buf)
	end
	return sData
end

--设置当前出战队形
function clsFormation:SetFormLineUp(lineup)
	for _, v in ipairs(lineup) do
		local grid = self:GetGrid(v.pos)
		if grid:GetState() == GRID_OPEN then
			grid:SetRoleId(v.id)
			grid:SetRoleOwner(v.owner)
		end
	end
end

function NewFormation(id, Lv, state, roleId, roleOwner)
	local form = clsFormation:New()
	form:InitForm(id, Lv, state, roleId, roleOwner)
	return form
end
----------------------------------------------------------------------------------------------------
clsUser = USERBASE.clsUser

function clsUser:ResetFormation()
	self.__save.formation = nil
	self.__save.formation = {}
	self.__temp.formation = {}
	self:InitFightForm()
end

function clsUser:InitOneFormation(formId, formLv, state)
	local formObj = NewFormation(formId, formLv or 1, state or FORM_OPEN, self:GetRoleId())
	self:AddFormation(formId, formObj)
end

--初始化战斗阵型
function clsUser:InitFightForm()
	self:InitOneFormation(INIT_FORMATION, 1, FORM_USE)
	self:SetWorkInfo("standby")
end

function clsUser:AddFormation(formId, formObj)
	formId = tostring(formId)
	self.__temp.formation = self.__temp.formation or {}
	self.__temp.formation[formId] = formObj
end

function clsUser:GetFormation(formId)
	formId = tostring(formId)
	self.__temp.formation = self.__temp.formation or {}
	return self.__temp.formation[formId]
end

function clsUser:GetFightFormList()
	return self.__temp.formation or {}
end

function clsUser:GetCurForm()
	for k, v in pairs(self:GetFightFormList()) do
		if v:GetState() == FORM_USE then
			return v
		end
	end
end


function clsUser:SaveFormData()
	self.__save.formation = {}
	for k, v in pairs(self:GetFightFormList()) do
		self.__save.formation[k] = v:GetSaveData()
	end
end

function clsUser:RestoreFormData()
	self.__temp.formation = {}
	for k, v in pairs(self.__save.formation or {}) do
		local form = NewFormation(k)
		form:Restore(v)
		self:AddFormation(k, form)
	end
end

--战斗队形
function clsUser:GetCurFightLineup()
	local sData = {}
	for k, v in pairs(self:GetFightFormList()) do
		if v:GetState() == FORM_USE then
			for id, grid in pairs(v:GetGridList()) do
				local roleId = grid:GetRoleId()
				if roleId then
					local buf = {}
					buf.pos = grid:GetId()
					buf.id = roleId
					table.insert(sData, buf)
				end
			end
			break
		end
	end
	return sData
end


function GetFormatedFormList(user)
	local form = user:GetCurForm()
	local sData = {list = {}, curFormId = form:GetId()}
	local num = 0
	for k, v in pairs(GetAllFormationList()) do
		if k ~= NONE_FORMATION then
			local form = user:GetFormation(k) or NewFormation(k, 1, FORM_LOCK)
			local buf = {}
			buf.formId = k
			buf.name = form:GetName()
		--	buf.restraintDesc = form:GetRestraintDesc()
			buf.restraint = {}
			for k in pairs(form:GetRestraint() or {}) do
				table.insert(buf.restraint, k)
			end
			buf.linkSkill = form:GetLinkSkill()
			local Lv = form:GetLv()
			local state = form:GetState()
			buf.formLv = Lv
			buf.state = state
			buf.explain = form:GetExplain()
			if state ~= FORM_LOCK then
				buf.describe = form:GetDescribe()
				buf.nextDesc = form:GetDescribe(Lv + 1)
			end
			buf.lineup = form:FormatGridList()
			table.insert(sData.list, buf)
			num = num + 1
		end
	end
	table.sort(sData.list, function(a, b) return tonumber(a.formId) < tonumber(b.formId) end)
	sData.formNum = num
	return sData
end

function GetFormatedFormInfo(user, formId)
	local form = user:GetFormation(formId) or NewFormation(formId, 1, FORM_LOCK)
	local sData = {}
	sData.name = form:GetName()
	sData.formId = formId
	sData.state = form:GetState()
	sData.Lv = form:GetLv()
	sData.maxLv = form:GetMaxLv()
	sData.explain = form:GetExplain()
	if form:GetState() ~= FORM_LOCK then
		sData.describe = form:GetDescribe()
		sData.nextDesc = form:GetDescribe(form:GetLv() + 1)
--		sData.explain = form:GetExplain()
		sData.upgrade = {}
		sData.upgrade.needGold = form:GetUpgradeGold()
		sData.upgrade.needItem = form:GetUpgradeItem()
	else
--		sData.describe = form:GetExplain()
		sData.needGold = form:GetOpenGold()
		sData.needLv = form:GetOpenLv()
		sData.needCity = form:IsNeedCity() and 1 or 0
	end
	return sData
end

local s_form_heroList = function(connection, data)
	return ERROR.TipMsg(connection, "请使用协议：s_hero_myList;{type = form}")
--[[	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData ={}
	for k, v in pairs(user:GetHeroList()) do
		table.insert(sData, v:GetSimpleInfo())
	end
	table.sort(sData, function(a, b) return tonumber(a.heroId) < tonumber(b.heroId) end)
	table.insert(sData, 1, user:GetSimpleInfo())
	connection:SendProtocol("c_form_heroList", sData)]]
end


local s_form_info = function(connection, data)
	local formInfo = GetFormationInfoById(data)
	if not formInfo or data == NONE_FORMATION then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s", data))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatedFormInfo(user, data)
	connection:SendProtocol("c_form_info", sData)
end


local s_form_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatedFormList(user)
	connection:SendProtocol("c_form_list", sData)
end

local s_form_learn = function(connection, data)
	local form = NewFormation(data)
	if not form then
		return ERROR.TipMsg(connection, "无效的阵型")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetFormation(data) then
		return ERROR.TipMsg(connection, "您已经学会了该阵型。")
	end
	local gold = form:GetOpenGold()
	local nimbus = form:GetOpenNimbus()
	if user:GetLv() < form:GetOpenLv() then
		return ERROR.TipMsg(connection, string.format("您的等级不足%d，不能学习该阵法。", form:GetOpenLv()))
	end
	if gold and gold > user:GetGold() then
		return ERROR.TipMsg(connection, string.format("您的钱币不足%d，不能学习该阵法。", gold))
	end
	if nimbus and nimbus > user:GetNimbus() then
		return ERROR.TipMsg(connection, string.format("您的灵气不足%d，不能学习该阵法。", nimbus))
	end
	if form:IsNeedCity() and user:GetCityIdentity() ~= CLSCITY.ID_MASTER then
		return ERROR.TipMsg(connection, "只有城主才能学习该阵法")
	end
	if gold then
		user:SetGold(-gold)
	end
	if nimbus then
		user:SetNimbus(-nimbus)
	end
	user:InitOneFormation(data)
	user:CheckAchvState("formation")
	connection:SendProtocol("c_form_learn", data)
	user:CheckCityForm(data)
end

local s_form_upgrade = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local form = user:GetFormation(data.formId)
	if not form then
		return ERROR.TipMsg(connection, "您还没有学会该阵型")
	end
	local lv = form:GetLv()
	if lv + 1 ~= data.formLv then
		return ERROR.TipMsg(connection, "阵型等级错误。")
	end
	if lv >= form:GetMaxLv() then
		return ERROR.TipMsg(connection, "该阵型已经达到最高等级。")
	end
	local gold = form:GetUpgradeGold()
	if gold and user:GetGold() < gold then
		return ERROR.TipMsg(connection, "钱币不足，不能升级")
	end
	local nimbus = form:GetUpgradeNimbus()
	if nimbus and nimbus > user:GetNimbus() then
		return ERROR.TipMsg(connection, string.format("您的灵气不足%d，不能升级该阵法。", nimbus))
	end
	local item = form:GetUpgradeItem()
	if item then
		if user:GetItemNum(USERBASE.PACKAGE, item.goodsId) < item.num then
			return ERROR.TipMsg(connection, string.format("升级该阵法需要道具【%s】。", BASEITEM.GetItemById(item.goodsId).name))
		end
	end
	form:Upgrade()
	if gold then
		user:SetGold(-gold)
	end
	if nimbus then
		user:SetNimbus(-nimbus)
	end
	if item then
		user:DelItem(USERBASE.PACKAGE, item.goodsId, item.num)
	end
	connection:SendProtocol("c_form_upgrade", data)
end

local s_form_lineup = function(connection, data)
	if not data.formId or type(data.lineup) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local form = user:GetFormation(data.formId)
	if not form then
		return ERROR.TipMsg(connection, "您尚未掌握该阵型，不能对其进行配置。")
	end
	local needFresh = {}
	local flag = false
	for k, v in ipairs(data.lineup) do
		local grid = form:GetGrid(v.pos)
		if not grid or grid:GetState() ~= GRID_OPEN then
			return ERROR.TipMsg(connection, string.format("阵型格%s当前未解锁。", grid:GetId()))
		end
		if IsUserId(v.id) then
			flag = true
		else
			local hero = user:GetHero(v.id)
			if not hero then
				return ERROR.TipMsg(connection, string.format("没有指定的的武将信息：%s。", tostring(v.id)))
			end
		end
		for _, info in ipairs(data.lineup) do
			if info.id == v.id and info.pos ~= v.pos then
				return ERROR.TipMsg(connection, string.format("阵型无效，一个武将不能出现在两个不同的阵型格上"))
			end
		end
		if form:GetState() == FORM_USE then		--上阵人员需要刷新
			needFresh[v.id] = 1
		end
	end
	if not flag then
		return ERROR.TipMsg(connection, "主角必须位于阵型中。")
	end
	for k, v in pairs(form:GetGridList()) do
		local id = v:GetRoleId()
		if id then
			if form:GetState() == FORM_USE then
				if needFresh[id] == 1 then
					needFresh[id] = nil		--始终在阵中的人员不需要刷新
				else
					needFresh[id] = 0		--下阵人员需要刷新
				end
			end
			v:SetRoleId()
		end
	end
	form:SetFormLineUp(data.lineup)
	for k, v in pairs(needFresh) do
		if IsUserId(k) then
			user:FreshTotalAttr()
		else
			local hero = user:GetHero(k)
			if hero then
				hero:SetWorkInfo(v == 1 and "standby" or "rest")
				hero:FreshTotalAttr()
			end
		end
	end
	connection:SendProtocol("c_form_lineup", data)
end


local s_form_activate = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId), connection.roleId)
	data = tostring(data)
	local curForm = user:GetCurForm()
	if curForm:GetId() == data then
		return ERROR.TipMsg(connection, "当前阵型已启用")
	end
	local form = user:GetFormation(data)
	if not form then
		return ERROR.TipMsg(connection, "您尚未掌握该阵型")
	end
	local needFresh = {}
	for k, v in pairs(curForm:GetGridList()) do
		local roleId = v:GetRoleId()
		if roleId then
			needFresh[roleId] = 0
		end
	end
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId() 
		if roleId then
			needFresh[roleId] = 1
		end
	end
	form:SetState(FORM_USE)
	curForm:SetState(FORM_OPEN)
	for k, v in pairs(needFresh) do
		if IsUserId(roleId) then
			user:FreshTotalAttr()
		else
			local hero = user:GetHero(k)
			if hero then
				hero:FreshTotalAttr()
				hero:SetWorkInfo(v == 1 and "standby" or "rest")
			end
		end
	end
	connection:SendProtocol("c_form_activate", data)
end

function __init__()
	FORMATIONLIST = Import("autocode/formationList.lua")
	AllFormationList = FORMATIONLIST.GetTable()
	
	PROTOCOL.s_form_heroList = s_form_heroList
	PROTOCOL.s_form_list = s_form_list
	PROTOCOL.s_form_info = s_form_info
	PROTOCOL.s_form_learn = s_form_learn
	PROTOCOL.s_form_upgrade = s_form_upgrade
	PROTOCOL.s_form_lineup = s_form_lineup
	PROTOCOL.s_form_activate = s_form_activate
end
