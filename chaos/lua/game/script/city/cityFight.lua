--城池争夺战

local CityAttackState = {state = "close", timestamp = nil}
local CITY_ATTACK_TIMELONG = 3600
local CITY_ATTACK_TOKEN = 102021			--攻城令
local FORCE_COLD_DOWN_COST = 500			--强制冷却不同势力城池所需元宝
local NONE_FORMATION = "321001"		--【无】阵法id
local AllTacticList = {}

function GetTacticInfoById(id)
	return AllTacticList[id]
end

function GetAllTacticList()
	return AllTacticList
end

function GetCityAttackState()
	return CityAttackState.state
end

function GetCityAttackLeftTime()
	if not CityAttackState.timestamp then return 0 end
	local timelong = CityAttackState.timestamp - os.time()
	return timelong > 0 and timelong or 0
end

function SetCityAttackOpen()
	CityAttackState.state = "open"
	CityAttackState.timestamp = os.time() + CITY_ATTACK_TIMELONG
	for k, v in pairs(CITY.GetAllCityList()) do
		
	end
	CHAT.SendSysNotice("成者为王败者为寇，本势力城池争夺战已开始，鹿死谁手笑看今朝。")
	CALLOUT.CallOut(CloseCityAttack, CITY_ATTACK_TIMELONG)
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:IsFuncOpen(SETTING.FUNC_CITY_FIGHT) then
			FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_CITY_FIGHT, FUNCOPEN.STATE_OK)
		end
	end
end

function CloseCityAttack()
	if CityAttackState.state ~= "open" then
		return
	end
	CHAT.SendSysNotice("本轮城池争夺战已结束，指点江山大展宏图，新的霸业即将展开。")
	CityAttackState.state = "close"
	CityAttackState.timestamp = nil
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:IsFuncOpen(SETTING.FUNC_CITY_FIGHT) then
			FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_CITY_FIGHT, FUNCOPEN.STATE_INVALID)
		end
	end
end

function IsInAttackRange(start, target)
	if not start then
		return false
	end
	local done = {}
	local function cal_path(id)
		done[id] = true
		local city = CITY.GetCityObjById(id)
		for k, v in pairs(city:GetNeighber()) do
			if v == target then
				return true
			end
			local tCity = CITY.GetCityObjById(v)
			if tCity:GetPhyle() == city:GetPhyle() then
				if not done[v] then
					local ret = cal_path(v)
					if ret then
						return true
					end
				end
			end
		end
	end
	return cal_path(start)
end

function GetAttackRange(city)
	local ret = {}
	local done = {}
	local function add_to_list(id)
		local city = CITY.GetCityObjById(id)
		done[id] = true
		for k, v in pairs(city:GetNeighber()) do
			ret[v] = true
			if not done[v] then
				local tCity = CITY.GetCityObjById(v)
				if tCity:GetPhyle() == city:GetPhyle() then
					add_to_list(v)
				end
			end
		end
	end
	add_to_list(city)
	return ret
end

--暗渡陈仓范围判定
function CanTargetBeReached(form, to)
	local city = CITY.GetCityObjById(to)
	for _, v in pairs(city:GetNeighber()) do
		if IsInAttackRange(form, v) then
			return true
		end
	end
	return false
end

function IsCityCanAttack(user, target, flag)
	local roleId = user:GetRoleId()
	local city = CITY.GetCityObjById(target)
	if city:GetOwner() == roleId then
		return false, "不能攻打自己的城池。"
	end
	if city:IsMainCity() then
		return false, "不能向主城发起攻击。"
	end
	if city:GetFightState() then
		return false, "该城池处于战斗状态。"
	end
	local myCityId = user:GetCityId()
	if not flag and not IsInAttackRange(myCityId, target) then
		return false, "仅能攻打路线中相邻的敌对城池。"
	elseif flag and not CanTargetBeReached(myCityId, target) then
		return false, "指定的城池不在可攻打范围内。"
	end
	local tempCityId = user:GetTempCity()
	if tempCityId then
		local tempCity = CITY.GetCityObjById(tempCityId)
		local locate = tempCity:GetLocation()
		return false, string.format("城池【%s(%d, %d)】正处于进驻保护时间。\n请先完成该城池的处置事宜。", tempCity:GetName(), locate.x, locate.y)
	end
	if user:GetPhyle() ~= city:GetPhyle() then
		local myCity = CITY.GetCityObjById(user:GetCityId())
		if roleId ~= myCity:GetOwner() then
			return false, "你不是城主，不能攻打敌对城池。"
		end	
		if user:GetCityAttackTimes() >= user:GetMaxCityAttackTimes() then
			return false, "您今日的进攻次数已经用完。"
		end
	else
		local state = GetCityAttackState()
		local owner = city:GetOwner()
		if state ~= "open" and owner then
			return false, "城池争夺战尚未开启，不能攻打本势力有主的城池。"
		end
		if city:GetTimeStamp() > os.time() then
			return false, "城池处于保护状态，不能进行攻打。"
		end
		if owner and user:GetItemNum(USERBASE.PACKAGE, CITY_ATTACK_TOKEN) <= 0 then
			return false, "您身上没有【攻城令】，不能攻击友方城池。"
		end
	end
	return true
end


function GetUserFormList(user)
	local city = CITY.GetCityObjById(user:GetCityId())
	if not city then return end
	local formList = user:GetFightFormList()
	local fightForm = city:GetFightFormList()
	local sData = {formNum = table.size(FORMATION.GetAllFormationList()) - 1, list = {}, curFormId = city:GetFormationActive()}
	for k, v in pairs(FORMATION.GetAllFormationList()) do
		if k ~= NONE_FORMATION then
			local buff = {formId = k, name = v.name, posList = v.posList} --, maxLv = #v.list}
			buff.restraintDesc = v.restraintDesc
			if formList[k] then
				if not fightForm[k] then
					city:InitOneFormation(k, formList[k].Lv)
				end
				buff.formLv = formList[k].Lv
				buff.explain = v.explain
				buff.describe = v.list[buff.formLv].describe
		--[[		if v.list[buff.formLv + 1] then
					buff.nextDesc = v.list[buff.formLv + 1].describe
				end]]
				buff.lineup = fightForm[k].list
				buff.state = "unlock"
			else
				buff.formLv = 0
				buff.explain = v.explain
				buff.state = "lock"
			end
			table.insert(sData.list, buff)
		end
	end
	table.sort(sData.list, function(a, b) return tonumber(a.formId) < tonumber(b.formId) end)
	return sData
end

function GetFormatedTacticList()
	local sData = {}
	for k, v in pairs(GetAllTacticList()) do
		local buff = {}
		buff.tacticId = k
		buff.name = v.name
		buff.describe = v.describe
		buff.needIngot = v.needIngot
		buff.target = v.target
		table.insert(sData, buff)
	end
	return sData
end

-------------------------------------------------------------
local clsCity = CLSCITY.clsCity

function clsCity:InitTacticList()
	self.__save.tacticList = {}
	self.__temp.tacticList = {}
end

function clsCity:GetTacticList()
	return self.__save.tacticList
end

function clsCity:IsTacticExist(id)
	return self.__save.tacticList[id]
end

function clsCity:AddTactic(id)
	self.__save.tacticList[id] = self.__save.tacticList[id] or {}
	local tacticInfo = GetTacticInfoById(id)
	local info = self.__save.tacticList[id]
	info.timestamp = (info.timestamp or os.time()) + tacticInfo.timelong
	if info.timestamp - os.time() >= tacticInfo.uplimit then
		info.timestamp = os.time() + tacticInfo.uplimit
	end
	self:StartTacticTimer(id)
	self:SendTacticUpdate(id)
end

function clsCity:DelTactic(id)
	self.__save.tacticList[id] = nil
	self:RemoveTacticTimer(id)
	self:SendTacticUpdate(id)
end

function clsCity:SendTacticUpdate(id)
	local sData = {tacticId = id}
	local info = self:IsTacticExist(id)
	if info then
		sData.timelong = info.timestamp - os.time()
	else
		sData.timelong = 0
	end
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_city_tacticUpdate", sData)
		end
	end
end

function clsCity:StartTacticTimer(id)
	if self.__temp.tacticList[id] then
		self:RemoveTacticTimer(id)
	end
	local info = self:IsTacticExist(id)
	local timelong = info.timestamp - os.time()
	if timelong <= 0 then
		self:DelTactic(id)
	else
		self.__temp.tacticList[id] = CALLOUT.ObjCallOut(self.__temp.callout, clsCity.DelTactic, timelong, self, id)
	end
end

function clsCity:RemoveTacticTimer(id)
	if self.__temp.tacticList[id] then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.tacticList[id])
	end
end

--城池初始化时启动计谋定时器
function clsCity:StartAllTacticTimer()
	for k in pairs(self:GetTacticList()) do
		self:StartTacticTimer(k)
	end
end


function clsCity:GetFightParam(type, city, tacticId)
	local buff = {}
	if not self:IsMainCity() and not self:GetOwner() then		--无城主且不是主城
		return buff
	end
	local linkSkill = self:GetLinkSkill()
	if CLSCITY.IsFightBufferValid("linkSkill", type, linkSkill) then
		if (type == "attack" and not city:IsTacticExist("390001")) or (type == "defend" and tacticId ~= "390003") then 
			buff.stuntId = linkSkill
			buff.stuntLv = self:GetLinkSkillLv()
		end
	end
	for id in pairs(self:GetTacticList()) do
		if CLSCITY.IsFightBufferValid("tactic", type, id) then
			local tacticInfo = GetTacticInfoById(id)
			for k, v in pairs(tacticInfo.attr) do
				buff[k] = (buff[k] or 0) + v
			end
		end
	end
	return buff
end


function GetCityTacticList(city)
	local list = city:GetTacticList()
	local sData = {}
	for k, v in pairs(list) do
		local info = GetTacticInfoById(k)
		if info then
			table.insert(sData, {tacticId = k, timelong = v.timestamp - os.time(), name = info.name, explain = info.explain, needIngot = info.needIngot})
		end
	end
	return sData
end


local s_cityFight_attack = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local city = CITY.GetCityObjById(data.cityId)
	if not city then
		return ERROR.TipMsg(connection, "无效的城池信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg = IsCityCanAttack(user, data.cityId)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	local myCity = CITY.GetCityObjById(user:GetCityId())
	if myCity:GetPhyle() == city:GetPhyle() then
		user:DelItem(USERBASE.PACKAGE, CITY_ATTACK_TOKEN, 1)
	end
	if user:GetPhyle() ~= city:GetPhyle() then
		if city:GetTimeStamp() > os.time() then
			if data.isForce ~= 1 then
				return ERROR.TipMsg(connection, "该城池处于保护状态，不能进行攻打。")
			elseif user:GetTotalIngot() < FORCE_COLD_DOWN_COST then
				return ERROR.TipMsg(connection, "您的元宝不足，不能强制攻打该城池。")
			end
			user:SetTotalIngot(-FORCE_COLD_DOWN_COST)
			city:SetTimeStamp(os.time())
			myCity:SetTimeStamp(os.time())
		end
	end
	city:ProAttack(user)
end

local s_city_tacticList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "city" then
		return ERROR.TipMsg(connection, "您没有关注城池。")
	end
	local city = CITY.GetCityObjById(focus.id)
	local sData = GetCityTacticList(city)
	connection:SendProtocol("c_city_tacticList", sData)
end

local s_city_taticAdd = function(connection, data)
	if type(data) ~= "table" or not data.tacticId or not data.cityId then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local info = GetTacticInfoById(data.tacticId)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的策略信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local myCityId = user:GetCityId()
	if info.target == 1 and data.cityId ~= myCityId then
		return ERROR.TipMsg(connection, "该策略只能用于自己所在的城池。")
	elseif info.target == 2 and data.cityId == myCityId then
		return ERROR.TipMsg(connection, "该策略不能用于自己所在的城池。")
	end
	if user:GetTotalIngot() < info.needIngot then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	local city = CITY.GetCityObjById(data.cityId)
	if info.type == 0 then
		user:SetTotalIngot(-info.needIngot)
		city:AddTactic(data.tacticId)
	else
		if data.tacticId == "390002" then
			local ret, msg = IsCityCanAttack(user, data.cityId, true)
			if not ret then
				return ERROR.TipMsg(connection, msg)
			end
			user:SetTotalIngot(-info.needIngot)
			city:ProAttack(user, data.tacticId)
		end
	end
	connection:SendProtocol("c_city_taticAdd", data)
end

local s_city_tacticDel = function(connection, data)
	local info = GetTacticInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的策略信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local focus = user:GetFocus()
	if not focus or focus.type ~= "city" then
		return ERROR.TipMsg(connection, "您没有关注城池。")
	end
	local city = CITY.GetCityObjById(focus.id)
	local tactic = city:IsTacticExist(data)
	if not tactic then
		return ERROR.TipMsg(connection, string.format("该城池当前并未被施加策略【%s】。", info.name))
	end
	local cost = math.ceil((tactic.timestamp - os.time()) / info.timelong * info.needIngot)
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetTotalIngot(-cost)
	city:DelTactic(data)
	connection:SendProtocol("c_city_tacticDel", data)
end

local s_tactic_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetFormatedTacticList()
	connection:SendProtocol("c_tactic_list", sData)
end


function __init__()

	local TACTIC = Import("autocode/tacticList.lua")
	AllTacticList = TACTIC.GetTable()
	
	CALLOUT.Daily(getfenv(2), 20, 0, SetCityAttackOpen)
	
	PROTOCOL.s_cityFight_attack = s_cityFight_attack

	PROTOCOL.s_city_tacticList = s_city_tacticList
	PROTOCOL.s_tactic_list = s_tactic_list
	PROTOCOL.s_city_taticAdd = s_city_taticAdd
	PROTOCOL.s_city_tacticDel = s_city_tacticDel
end
