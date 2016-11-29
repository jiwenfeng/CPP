--城池编制

local NONE_FORMATION = "321001"		--【无】阵法id


local clsCity = CLSCITY.clsCity

function clsCity:AddFormation(formId, formObj)
	formId = tostring(formId)
	self.__temp.formation = self.__temp.formation or {}
	self.__temp.formation[formId] = formObj
end

function clsCity:GetFormation(formId)
	formId = tostring(formId)
	self.__temp.formation = self.__temp.formation or {}
	return self.__temp.formation[formId]
end

function clsCity:GetFightFormList()
	return self.__temp.formation or {}
end

function clsCity:GetCurForm()
	for k, v in pairs(self:GetFightFormList()) do
		if v:GetState() == FORMATION.FORM_USE then
			return v
		end
	end
end

function clsCity:SaveFormData()
	self.__save.formation = {}
	for k, v in pairs(self:GetFightFormList()) do
		self.__save.formation[k] = v:GetSaveData()
	end
end

function clsCity:RestoreFormData()
	self.__temp.formation = {}
	for k, v in pairs(self.__save.formation or {}) do
		local form = FORMATION.NewFormation(k)
		form:Restore(v)
		self:AddFormation(k, form)
	end
end

function clsCity:IsFightFormInited()
	return next(self.__temp.formation or {})
end

--初始化阵型列表
function clsCity:InitFightForm()
	local user = USER.GetOnlineUser(self:GetOwner())
	if user then
		for k, v in pairs(user:GetFightFormList()) do
			local form = FORMATION.NewFormation(k, v:GetLv(), v:GetState(), self:GetOwner(), self:GetOwner())
			if v:GetState() == FORMATION.FORM_USE then
				for k, v in pairs(v:GetGridList()) do
					local grid = form:GetGrid(k)
					if v:GetRoleId() then
						grid:SetRoleId(v:GetRoleId())
						grid:SetRoleOwner(self:GetOwner())
					end
				end
			end
			self:AddFormation(k, form)
		end
	end
end

--初始化一个阵型
function clsCity:InitOneFormation(formId, formLv, state)
	local formObj = FORMATION.NewFormation(formId, formLv or 1, state or FORM_OPEN, self:GetOwner(), self:GetOwner())
	self:AddFormation(data, formObj)
end


function clsCity:SetFormationActive(formId)
	for k, v in pairs(self:GetFightFormList()) do
		if k == formId then
			v:SetState(FORMATION.FORM_USE)
		elseif v:GetState() == FORMATION.FORM_USE then
			v:SetState(FORMATION.FORM_OPEN)
		end
	end
end



--获取依附者战斗属性列表
function clsCity:GetMemberFightInfoList(call_back)
	local userList = {}
	for k in pairs(self:GetListByType("member")) do
		table.insert(userList, k)
	end
	local function read_back(dataList)
		local sData = {}
		for k, v in pairs(dataList) do
			local userInfo = v.list[k]
			local buff = {}
			buff.name = userInfo.name
		--	buff.roleId = k
			buff.heroId = k
			if self:GetOwner() == k then
				buff.isMaster = 1
			else
				buff.isMaster = 0
			end
			buff.headId = userInfo.headId
			buff.modelId = userInfo.modelId
			buff.Lv = userInfo.Lv
			buff.job = userInfo.job
			buff.aptLv = 5
			buff.skillList = {}
			local skillData = userInfo.skillList and userInfo.skillList["1"]
			if skillData then
				table.insert(buff.skillList, {type = 1, skillId = skillData.id, Lv = skillData.Lv})
			end
			buff.hp = userInfo.hpMax
			buff.hpMax = userInfo.hpMax
			buff.physcAtkMin = userInfo.physcAtkMin
			buff.physcAtkMax = userInfo.physcAtkMax
			buff.physcDef = userInfo.physcDef
			buff.magicAtkMin = userInfo.magicAtkMin
			buff.magicAtkMax = userInfo.magicAtkMax
			buff.magicDef = userInfo.magicDef
			buff.speed = math.roundedto(userInfo.speed)
			table.insert(sData, buff)
		end
		table.sort(sData, function(a, b) return
				a.isMaster > b.isMaster or (a.isMaster == b.isMaster and a.Lv > b.Lv) 
			end)
		call_back(sData)
	end
	FIGHT.LoadFightDataList(userList, read_back)
end



function clsCity:IsMemberInLineup(roleId)
	for k, v in pairs(self:GetFightFormList()) do
		for _, grid in pairs(v:GetGridList()) do
			if grid:GetRoleId()	== roleId then
				return true
			end
		end
	end
end


function clsCity:DelUserFromAllLineup(roleId)
	local function del_user(list)
		for _, grid in pairs(list) do
			if grid:GetRoleId() == roleId then
				grid:SetRoleId()
				grid:SetRoleOwner()
				return
			end
		end
	end
	for k, v in pairs(self:GetFightFormList()) do
		del_user(v:GetGridList())
	end
end


function clsCity:GetCityFightData(type)
	local owner = self:GetOwner()
	if not owner then
		local info = CLSCITY.GetCityUpgradeInfo(self:GetLv())
		local monsterList = info.monsterList
		return monsterList[math.random(1, #monsterList)]
	elseif not self:IsFightFormInited() then
		return owner
	else
		local sData = {}
		if type == "diff" then
			sData = {lineup = {}}
			local form = self:GetCurForm()
			sData.formId = form:GetId()
			sData.formLv = form:GetLv()
			for pos, grid in pairs(form:GetGridList()) do
				local buff = {}
				buff.roleId = grid:GetRoleOwner()
				buff.id = grid:GetRoleId()
				buff.pos = pos
				if buff.id == owner then
					buff.isMain = 1
				end
				table.insert(sData.lineup, buff)
			end
		else
			sData = owner
		end
		return sData
	end	
end

function clsCity:GetFormatedFormList()
	local form = self:GetCurForm()
	local sData = {list = {}, curFormId = form:GetId()}
	local num = 0
	for k, v in pairs(FORMATION.GetAllFormationList()) do
		if k ~= NONE_FORMATION then
			local form = self:GetFormation(k) or FORMATION.NewFormation(k, 1, FORMATION.FORM_LOCK)
			local buf = {}
			buf.formId = k
			buf.name = form:GetName()
			buf.formLv = Lv
			local state = form:GetState()
			buf.state = state
			local Lv = form:GetLv()
			if state ~= FORMATION.FORM_LOCK then
				buf.describe = form:GetDescribe()
				buf.nextDesc = form:GetDescribe(Lv + 1)
			else
				buf.describe = form:GetExplain()
			end
			buf.restraintDesc = form:GetRestraintDesc()
			buf.linkSkill = form:GetLinkSkill()
			buf.lineup = form:FormatGridList()
			table.insert(sData.list, buf)
			num = num + 1
		end
	end
	table.sort(sData.list, function(a, b) return tonumber(a.formId) < tonumber(b.formId) end)
	sData.formNum = num
	return sData
end



local s_cityFight_memberList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "无效的城池信息。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行该操作。")
	end
	local function call_back(sData)
		connection:SendProtocol("c_cityFight_memberList", sData)
	end
	city:GetMemberFightInfoList(call_back)
end


local s_cityFight_formList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "无效的城池信息。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行该操作。")
	end
	if not city:IsFightFormInited() then
		city:InitFightForm()
	end
	local sData = city:GetFormatedFormList()
	connection:SendProtocol("c_cityFight_formList", sData)
end

local s_cityFight_formActive = function(connection, data)
	local formInfo = FORMATION.GetFormationInfoById(data)
	if not formInfo then
		return ERROR.TipMsg(connection, string.format("无效的阵型id：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "无效的城池信息。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行该操作。")
	end
	if not city:GetFormation(data) then
		return ERROR.TipMsg(connection, "您尚未掌握该阵型，请到阵型界面开启")
	end
	city:SetFormationActive(data)
	connection:SendProtocol("c_cityFight_formActive", data)
end

local s_cityFight_lineup = function(connection, data)
	if not data.formId or type(data.lineup) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，请选择阵型及队伍配置。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then
		return ERROR.TipMsg(connection, "无效的城池信息。")
	end
	if city:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您不是城主，不能进行该操作。")
	end
	local form = city:GetFormation(data.formId)
	if not form then
		return ERROR.TipMsg(connection, "您尚未掌握该阵型。")
	end
	local flag = false
	local posList = {}
	for k, v in pairs(data.lineup) do
		if not v.pos or posList[v.pos] then
			return ERROR.TipMsg(connection, "阵型配置有误。")
		end
		local grid = form:GetGrid(v.pos)
		if not grid or grid:GetState() ~= FORMATION.GRID_OPEN then
			return ERROR.TipMsg(connection, string.format("无效的阵型格id：%s。", v.pos))
		end
		if v.id == city:GetOwner() then
			flag = true
		end
		if not city:IsInList("member", v.owner) then
			return ERROR.TipMsg(connection, string.format("玩家%s不是您的依附者，不能列入阵型。", USER.GetUserNickById(v.owner) or "null"))
		end
		posList[v.pos] = true
	end
	if not flag then
		return ERROR.TipMsg(connection, "城主必须位于阵型之中。")
	end	
	for k, v in pairs(form:GetGridList()) do
		if v:GetState() == FORMATION.GRID_OPEN then
			v:SetRoleId()
		end
	end
	form:SetFormLineUp(data.lineup)
	connection:SendProtocol("c_cityFight_lineup", data)
end



function __init__()
	
	PROTOCOL.s_cityFight_memberList = s_cityFight_memberList
	PROTOCOL.s_cityFight_formList = s_cityFight_formList
	PROTOCOL.s_cityFight_formActive = s_cityFight_formActive
	PROTOCOL.s_cityFight_lineup = s_cityFight_lineup
	
end
