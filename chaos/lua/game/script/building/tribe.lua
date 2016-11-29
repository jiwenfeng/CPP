--部落系统
local SPEEDUP_COST_PER_SEC = 1
local RESOURCE_FRESH_TIME = 5 * 60		--资源刷新间隔

local SeparateStruct = {["embassy"] = 1}		--长期在线的建筑

local resList = {
	gold = {type = 1, max = 500000, init = 0, struct = "market"}, 
	spar = {type = 1, max = 20000, init = 1000, struct = "spar"}, 
	people = {type = 0, max = 4000, init = 4000, struct = "house"}, 
	soldier = {type = 0, max = 0, init = 1000, struct = "barrack"}
}	--资源种类

local upgradeList = {
	["1"] = {type = "building", state = UPGRADELIST.LIST_UNLOCK}, 
	["2"] = {type = "building", state = UPGRADELIST.LIST_LOCK},
}

local AllTribeList = {}			--所有玩家部落数据
local path_tribe = "chaos.tribe"

function GetTribeDataById(id)
	return AllTribeList[id]
end

function GetAllTribeDataList()
	return AllTribeList
end

function SaveTribeData(id)
	local buff = GetTribeDataById(id)
	buff._id = id
	DATA.UpdateData(path_tribe, {_id = id}, buff)
end

function SetTribeDataById(id, data)
	AllTribeList[id] = data
	SaveTribeData(id)
end

--计算当前资源存量
function GetTribeResourceNum(id, res)
	local tribeInfo = GetTribeDataById(id)
	local n = math.floor((os.time() - tribeInfo.timestamp) / RESOURCE_FRESH_TIME)
	local num = tribeInfo.resource[res].num + (tribeInfo.resource[res].speed * n * RESOURCE_FRESH_TIME / 3600)
	return num - tribeInfo.resource[res].lost
end

function SaveAllTribeData()
	for k in pairs(GetAllTribeDataList()) do
		SaveTribeData(k)
	end
end

function DelTribeDataById(id)
	AllTribeList[id] = nil
	DATA.DBRemove(path_tribe, {_id = id})
end

function GetSpeedUpCost()
	return SPEEDUP_COST_PER_SEC
end

function RestoreStruct(type, data)
	local obj
	if type == "mainHall" then
		obj = MAINHALL.clsMainHall:New()
	elseif type == "retreat" then
		obj = RETREAT.clsRetreat:New()
	elseif type == "house" then
		obj = HOUSE.clsHouse:New()
	elseif type == "market" then
		obj = MARKET.clsMarket:New()
	elseif type == "barrack" then
		obj = BARRACK.clsBarrack:New()
	elseif type == "academy" then
		obj = ACADEMY.clsAcademy:New()
	elseif type == "embassy" then
		obj = EMBASSY.clsEmbassy:New()
	elseif type == "furnace" then
		obj = FURNACE.clsFurnace:New()
	elseif type == "spar" then
		obj = SPARMINE.clsSpar:New()
	elseif type == "godAltar" then
		obj = GODALTAR.clsGodAltar:New()
	end
	if data and obj then
		obj:Restore(data)
	end
	return obj
end

--资源刷新间隔
function GetResourceFreshTimelong()
	return RESOURCE_FRESH_TIME
end
-------------------------------------------------
clsTribe = clsObject:Inherit()

function clsTribe:__init__()
	Super(clsTribe).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsTribe:initsavedata()	
	self.__save.locate = {}
	self.__save.stat = {}
	self.__save.wild = {}
	self.__save.struct = {}
	self.__save.levy = {}		--征收
end

function clsTribe:inittempdata()
	self.__temp.struct = {}
	self.__temp.callout = {}
end

function clsTribe:InitTribe(owner, type, posX, posY)
	self:SetOwner(owner)
	self:SetType(type)
	self:InitStructList(owner, type)
	self:SetLocate(posX, posY)
	self:InitResource()
	self:CalTribeLv()
	self:InitUpgradeList()
	self:InitOnlineTribeData()
end

function clsTribe:InitResource()
	for k, v in pairs(resList) do
		self.__save.stat[k] = {num = v.init, speed = 0, max = v.max}
		if v.type == 1 then
			self.__save.wild[k] = {num = v.init, speed = 0, max = v.max}
		end
	end
end

function clsTribe:InitStructList(owner, type)
	local user = assert(USER.GetOnlineUser(owner))
	local flag = false
	local cityId = user:GetCityId()
	if cityId then
		local city = CITY.GetCityObjById(cityId)
		if city:GetOwner() == owner and city:HasGodHero() then
			flag = true
		end
	end
	for k, v in pairs(STRUCT.GetStructList()) do
		if v.state == 1 or (k == "godAltar" and flag) then
			local obj = RestoreStruct(k)
			if obj then
				obj:InitStruct(owner, type, k)
				self.__temp.struct[k] = obj
				if k == "embassy" then
					EMBASSY.SetEmbassyObjById(self:GetOwner(), obj)
				end
				obj:Upgrade()
			end
		end
	end
end

function clsTribe:SetOwner(data)
	self.__save.owner = data
end

function clsTribe:GetOwner()
	return self.__save.owner
end

function clsTribe:SetType(data)
	self.__save.type = data
end

function clsTribe:GetType()
	return self.__save.type
end

function clsTribe:OpenStruct()
	for k, v in pairs(STRUCT.GetStructList()) do
		if v.tribeLv <= self.__save.Lv and not self:GetStruct(k) and k ~= "godAltar" then
			self:AddStruct(k)
		end
	end
end

--计算部落等级
function clsTribe:CalTribeLv()
	local total = 0
	local num = 0
	for k, v in pairs(self:GetAllStructList()) do
		if k ~= "godAltar" and k ~= "furnace" then
			total = total + v:GetLv()
			num = num + 1
		end
	end
	self.__save.Lv = math.max((self.__save.Lv or 0), math.floor(total / num))
	self:OpenStruct()
end

function clsTribe:GetTribeLv()
	return self.__save.Lv
end

--部落资源状况		--(type: num, max)
function clsTribe:SetResourceStat(res, type, data)
	if not self.__save.stat[res] then
		return
	end
	self.__save.stat[res][type] = data
	if self.__save.stat[res].num or 0 < 0 then
		self.__save.stat[res].num = 0
	end
end

function clsTribe:ModResourceStat(res, type, data)
	if res == "gold" and type == "num" then
		local user = assert(USER.GetOnlineUser(self:GetOwner()))
		return user:SetGold(data)
	end
	if not self.__save.stat[res] then
		return
	end
	self.__save.stat[res][type] = (self.__save.stat[res][type] or 0) + data
	if self.__save.stat[res].num < 0 then
		self.__save.stat[res].num = 0
	end
end

--(type: num, max, speed)
function clsTribe:GetResourceStat(res, type)
	if res == "gold" and type == "num" then
		local user = assert(USER.GetOnlineUser(self:GetOwner()))
		return user:GetGold()
	end
	if not self.__save.stat[res] then
		return 0
	end
	if type == "speed" then
		local struct = self:GetStruct(resList[res].struct)
		if struct then
			return struct:GetTotalResIncomeRate()
		end
		return 0
	end
	return self.__save.stat[res][type] or 0
end

--野地资源状况
function clsTribe:SetWildResourse(res, type, data)
	if not self.__save.wild[res] then
		return
	end
	self.__save.wild[res][type] = data
	if self.__save.wild[res].num > self.__save.wild[res].max then
		self.__save.wild[res].num = self.__save.wild[res].max
	elseif self.__save.wild[res].num < 0 then
		self.__save.wild[res].num = 0
	end
end

function clsTribe:ModWildResourse(res, type, data)
	if not self.__save.wild[res] then
		return
	end
	self.__save.wild[res][type] = self.__save.wild[res][type] + data
	if self.__save.wild[res].num > self.__save.wild[res].max then
		self.__save.wild[res].num = self.__save.wild[res].max
	elseif self.__save.wild[res].num < 0 then
		self.__save.wild[res].num = 0
	end
end

function clsTribe:GetWildResourse(res, type)
	if not self.__save.wild[res] then
		return
	end
	return self.__save.wild[res][type]
end


function clsTribe:InitUpgradeList()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	for k, v in pairs(upgradeList) do
		local obj = UPGRADELIST.NewList(v.type, k, v.state, self:GetOwner())
		user:AddUpgradeList(v.type, k, obj)
	end
	if user:HasFreeUpgradeList() then
		user:SetUpgradeListState("building", "2", UPGRADELIST.LIST_UNLOCK)
	end
end

--部落位置
function clsTribe:SetLocate(posX, posY)
	self.__save.locate = {x = posX, y = posY}
end

function clsTribe:GetLocate()
	return self.__save.locate
end

function clsTribe:GetCurPos()
	local info = self.__save.locate
	return info.x, info.y
end

--保存部落数据
function clsTribe:GetSaveData()
	self:UpdateOnlineTribeData()
	self.__save.struct = {}
	for k, v in pairs(self.__temp.struct) do
		if not SeparateStruct[k] then
			self.__save.struct[k] = v:GetSaveData()
		else
			self.__save.struct[k] = {}
		end
	end
	return table.deepcopy(self.__save)
end

function clsTribe:GetOnlineData()
	local sData = {}
	sData.posX, sData.posY = self:GetCurPos()
	sData.owner = self:GetOwner()
	return sData
end

function clsTribe:Restore(data)
	self.__save = data
	for k, v in pairs(self.__save.struct) do
		if not SeparateStruct[k] then
			local structObj = RestoreStruct(k, v)			--生成建筑对象
			self.__temp.struct[k] = structObj
		elseif k == "embassy" then
			local obj = EMBASSY.GetEmbassyObjById(self:GetOwner())
			if obj then
				self.__temp.struct[k] = obj
			else
				local function call_back(ret)
					if ret then
						local embassy = EMBASSY.GetEmbassyObjById(self:GetOwner())
						self.__temp.struct[k] = embassy
					else
						DATA.log(string.format("login restore embassy failed on %s:data error.", self:GetOwner()), "tribe.log")
					end
				end
				EMBASSY.RestoreEmbassyData(self:GetOwner(), call_back)
			end
		end
	end
end

function clsTribe:GetCallOut()
	return self.__temp.callout
end

function clsTribe:InitOnlineTribeData()
	local sData = {}
	sData.resource = {}
	for k, v in pairs(resList) do
		if v.type == 1 then
			local buff = {}
			local obj
			if k == "gold" then
				obj = self:GetStruct("market")
			elseif k == "spar" then
				obj = self:GetStruct("spar")
			end
			buff.speed = obj and obj:GetTotalResIncomeRate() or 0
			buff.num = self:GetResourceStat(k, "num")
			buff.lost = 0
			sData.resource[k] = buff
		end
	end
	sData.timestamp = os.time()
	sData.struct = {}
	for k, v in pairs(self:GetAllStructList()) do
		sData.struct[k] = v:GetLv()
	end
	SetTribeDataById(self:GetOwner(), sData)
end

function clsTribe:UpdateOnlineTribeData()
	local tribeInfo = GetTribeDataById(self:GetOwner())
	if not tribeInfo then
		return self:InitOnlineTribeData()
	end
	for k, v in pairs(resList) do
		if v.type == 1 then
			local obj
			if k == "gold" then
				obj = self:GetStruct("market")
			elseif k == "spar" then
				obj = self:GetStruct("spar")
			end
			if obj then
				tribeInfo.resource[k].speed = obj and obj:GetTotalResIncomeRate() or 0
			end
			tribeInfo.resource[k].num = self:GetResourceStat(k, "num")
		end
	end
	tribeInfo.timestamp = os.time()
end

function clsTribe:ProOfflineTribeData()
	local tribeInfo = GetTribeDataById(self:GetOwner())
	if tribeInfo then
		self:ModResourceStat("gold", "num", -tribeInfo.resource.gold.lost)
		self:ModResourceStat("spar", "num", -tribeInfo.resource.spar.lost)
	end
	self:InitOnlineTribeData()
end

function clsTribe:LoginStartCallOut()
	self.__temp.callId = CALLOUT.ObjCallOut(self:GetCallOut(), clsTribe.LoginStartCallOut, RESOURCE_FRESH_TIME, self)
	self:ProStructFunc()
	self:ClearOverrunResource()
	for k, v in pairs(self.__temp.struct) do
		v:LoginStartCallOut()
	end
end

function clsTribe:OfflineRemoveCallOut()
	if self.__temp.callId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.callId)
	end
	for k, v in pairs(self.__temp.struct) do
		v:OfflineRemoveCallOut()
	end
end

function clsTribe:ProStructFunc()
	for k, v in pairs(self.__temp.struct) do
		if not USER.GetOnlineUser(self:GetOwner()) then
			return
		end
		if not SeparateStruct[k] then
			v:ProStructFunc()
		end
	end
	for k in pairs(USER.GetUserFocusListByType("tribe") or {}) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			PROTOCOL.s_tribe_info(con, "get")
		end
	end
end

--增加新的建筑
function clsTribe:AddStruct(structType)
	local obj = RestoreStruct(structType)
	obj:InitStruct(self:GetOwner(), self:GetType(), structType)
	self.__temp.struct[structType] = obj
	obj:Upgrade()
	if structType == "embassy" then
		EMBASSY.SetEmbassyObjById(self:GetOwner(), obj)
	end
	self:CalTribeLv()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:SendProtocol("c_tribe_new", {type = structType, lv = obj:GetLv()})
end

--移除新的建筑
function clsTribe:DelStruct(structType)
	local obj = self:GetStruct(structType)
	if not obj then
		return
	end
	obj:OfflineRemoveCallOut()
	obj:Destroy()
	self.__temp.struct[structType] = nil
end

function clsTribe:GetStruct(type)
	return self.__temp.struct[type]
end

function clsTribe:GetAllStructList()
	return self.__temp.struct
end

--扣除升级消耗
function clsTribe:CalUpgradeConsume(type, lv)
	local user = USER.GetOnlineUser(self:GetOwner())
	local info = STRUCT.GetUpLvInfo(type, lv)
	local pet = user:GetPet()
	local rate = pet:GetTalentAdd(311019)
	if info.condt.needGold then
		local gold = info.condt.needGold * (1 - rate)
		user:SetGold(-gold)
	end
	if info.condt.needSpar then
		local spar = info.condt.needSpar * (1 - rate)
		self:ModResourceStat("spar", "num", -spar)
	end
end

--获取部落的基本属性信息
function clsTribe:GetResourceInfo()
	local sData = {}
	sData.list = {}
	local user = USER.GetOnlineUser(self:GetOwner())
	for k, v in pairs(resList) do
		local buff = {}
		buff.type = k
		if k ~= "gold" then
			buff.num = self:GetResourceStat(k, "num")
		end
		if k == "soldier" then
			buff.totalHp = user:GetCurLineupTotalHp()
			buff.stayHp = user:GetStayHeroHp()
		end	
		buff.max = self:GetResourceStat(k, "max")
		buff.speed = self:GetResourceStat(k, "speed")
		if v.type == 1 then
			buff.wildAdd = self:GetWildResourse(k, "speed")
			buff.wildNum = self:GetWildResourse(k, "num")
		end
		table.insert(sData.list, buff)
	end
	sData.tribeLv = self:GetTribeLv()
	local id = user:GetCityId()
	local city = CITY.GetCityObjById(id)
	if city then
		sData.cityName = city:GetName()
	end
	return sData
end

--清理超出上限的资源
function clsTribe:ClearOverrunResource()
	local user = USER.GetOnlineUser(self:GetOwner())
	if not user then
		return
	end
	for k, v in pairs(resList) do
		local upLimit = self:GetResourceStat(k, "max")
		if k == "gold" then
			if user:GetGold() > upLimit then
				user:SetGold(upLimit - user:GetGold())
			end
		else
			if self:GetResourceStat(k, "num") > upLimit	then
				self:SetResourceStat(k, "num", upLimit)
			end
		end
	end
end

function clsTribe:GetStructList()
	local sData = {}
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	for k, v in pairs(self.__temp.struct) do
		local buff = v:GetUpgradeInfo()
		if k == "godAltar" or k == "furnace" then
			buff.state = true
		else
			buff.state = v:GetLv() > 0
		end
		table.insert(sData, buff)
	end
	return sData
end

--获取武将列表
function clsTribe:GetAllHeroList(type)
	local user = USER.GetOnlineUser(self:GetOwner())
	local heroList = user:GetHeroList()
	local sData = {}
	for k, v in pairs(heroList) do 
		if v:GetType() ~= "godHero" then
			local buf = {}
			buf.heroId = v:GetId()
			buf.name = v:GetName()
			buf.aptLv = v:GetAptLv()
			buf.Lv = v:GetLv()
			buf.job = v:GetJob()
			buf.force = v:GetFightForce()
			buf.headId = v:GetHeadId()
			buf.physcAtkMin, buf.physcAtkMax = v:GetPhyscAtk()
			buf.magicAtkMin, buf.magicAtkMax = v:GetMagicAtk()
			buf.physcDef = v:GetPhyscDef()
			buf.magicDef = v:GetMagicDef()
			buf.speed = v:GetSpeed()
			if type == "attr" then
				buf.strength = v:GetStrength()
				buf.intellect = v:GetIntellect()
				buf.agility = v:GetAgility()
				buf.captain = v:GetCaptain()
			elseif type == "hp" then
				buf.hp = v:GetHp()
				buf.hpMax = v:GetHpMax()
			end
			buf.state = v:GetWorkState()
			table.insert(sData, buf)
		end
	end
	return sData
end

function QuitTribeScene(user)
	user:LeaveScene()
	USER.SetUserFocusOn("tribe", user:GetRoleId())
	user:SetFocus()
	return SCENE.GetReturnedTownId(user)
end

--建筑列表
local s_tribe_structList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local sData = {}
	sData.list = tribe:GetStructList()
	sData.shape = user:GetCityIdentity() == CLSCITY.ID_MASTER
	connection:SendProtocol("c_tribe_structList", sData)
end

local s_struct_upgradeInfo = function(connection, data)
	if not STRUCT.GetStructList()[data] then
		return ERROR.TipMsg(connection, string.format("无效的建筑类型：%s。", data))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local struct = tribe:GetStruct(data)
	if not struct then
		return ERROR.TipMsg(connection, "您还没有建立该建筑。")
	end
	local sData = struct:GetUpgradeInfo()
	connection:SendProtocol("c_struct_upgradeInfo", sData)
end

--部落信息
local s_tribe_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local sData = tribe:GetResourceInfo()
	connection:SendProtocol("c_tribe_info", sData)
end

local s_tribe_upgradeList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.list = user:GetUpgradeList()
	sData.freeTimes = math.max(0, user:GetMaxSpeedUpTimes() - user:GetSpeedUpTimes())
	connection:SendProtocol("c_tribe_upgradeList", sData)
end

--升级
local s_struct_upgrade = function(connection, data)
	if not STRUCT.GetStructList()[data.type] then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的建筑类型：%s。", data.type or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local struct = tribe:GetStruct(data.type)
	if not struct then
		return ERROR.TipMsg(connection, "你还没有建立该建筑")
	end	
	local obj = user:GetIdleUpgradeList("building")
	if not obj then
		return ERROR.TipMsg(connection, "没有空闲队列可供升级使用")
	end
	local ret, msg = struct:CheckUpgrade(struct:GetLv())
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	tribe:CalUpgradeConsume(data.type, data.Lv)
	obj:SetEndTimestamp(os.time() + struct:GetUpgradeCdTime(data.Lv))
	struct:Upgrade()
	tribe:CalTribeLv()
	user:SendUpgradeListUpdate(obj:GetType(), obj:GetId())
	connection:SendProtocol("c_struct_upgrade", {type = data.type, Lv = data.Lv + 1, tribeLv = tribe:GetTribeLv()})
end

local s_tribe_heroList = function(connection, data)
	if data ~= "attr" and data ~= "hp" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
	local sData = tribe:GetAllHeroList(data)
	connection:SendProtocol("c_tribe_heroList", sData)
end

local s_upgrade_speedup = function(connection, data)
	if type(data) ~= "table" or not data.type or not data.index then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	local obj = user:GetOneUpgradeList(data.type, data.index)
	if not obj then
		return ERROR.TipMsg(connection, "该队列不可用")
	end
	if obj:GetState() == UPGRADELIST.LIST_LOCK then
		return ERROR.TipMsg(connection, "该队列未解锁")
	end
	if obj:GetEndTimestamp() <= os.time() then
		return ERROR.TipMsg(connection, "该队列已处于空闲状态")
	end
	if user:GetSpeedUpTimes() >= user:GetMaxSpeedUpTimes() then
		local cost = math.ceil((obj:GetEndTimestamp() - os.time()) / 100) * SPEEDUP_COST_PER_SEC
		if user:GetTotalIngot() < cost then
			return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
		end
		user:SetTotalIngot(-cost)
	else
		user:SetSpeedUpTimes(1)
	end
	obj:ClearCDTime()
	user:SendUpgradeListUpdate(data.type, data.index)
	data.freeTimes = user:GetSpeedUpTimes()
	connection:SendProtocol("c_upgrade_speedup", data)
end

local s_tribe_leave = function(connection, data)
	--[[if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落")
	end
	USER.SetUserFocusOn("tribe", connection.roleId)
	user:SetFocus()
	connection:SendProtocol("c_tribe_leave", "true")]]
end

--需要提醒的新建筑或功能
local s_tribe_needNotice = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local list = user:GetStructNeedNotice()
	local sData = 0
	if #list > 0 then
		sData = {}
		for k, v in ipairs(list) do
			table.insert(sData, {type = v.type, funcId = v.funcId, state = v.state})
		end
	end
	user:ClearStructNeedNotice()
	connection:SendProtocol("c_tribe_needNotice", sData)
end

local s_tribe_enter = function(connection, data)
	if data ~= "set" then
		return ERROR.ErrorMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "您还没有建立部落。")
	end
--	USER.SetUserFocusOn("tribe", connection.roleId, 1)
--	user:SetFocus("tribe")
	local sceneObj = SCENE.GetEmptyScene("tribe")
	connection:SendProtocol("c_tribe_enter", sceneObj:GetId())
end

local s_view_slgInfo = function(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("没有找到玩家【%s】的信息。", data or ""))
	end
	local function read_back(ret)
		local sData = {}
		sData.nick = data
		local cityId = USER.GetUserNickInfo(roleId, "cityId")
		local cityObj = CITY.GetCityObjById(cityId)
		if cityObj then
			sData.cityId = cityId
			sData.cityName = cityObj:GetName()
			sData.picId = cityObj:GetPicId()
			sData.Lv = cityObj:GetLv()
			sData.identity = cityObj:GetOwner() == roleId and CLSCITY.ID_MASTER or CLSCITY.ID_ATTACH
			sData.location = cityObj:GetLocation()
		end
		local tribeInfo = GetTribeDataById(roleId)
		if tribeInfo then
			sData.tribeLv = tribeInfo.tribeLv or 1
			sData.stayMax = 0
			sData.stayNum = 0
			local embassy = EMBASSY.GetEmbassyObjById(roleId)
			if embassy then
				sData.stayMax = embassy:GetMaxStayNum()
				sData.stayNum = embassy:GetStayHeroNum()
			end
		end
		connection:SendProtocol("c_view_slgInfo", sData)
	end
	EMBASSY.RestoreEmbassyData(roleId, read_back)
end

function __init__()
	DATA.Restore(path_tribe, AllTribeList)
	PROTOCOL.s_tribe_enter = s_tribe_enter
	PROTOCOL.s_tribe_structList = s_tribe_structList
	PROTOCOL.s_tribe_info = s_tribe_info
	PROTOCOL.s_tribe_leave = s_tribe_leave
	PROTOCOL.s_struct_upgradeInfo = s_struct_upgradeInfo
	PROTOCOL.s_tribe_upgradeList = s_tribe_upgradeList
	PROTOCOL.s_struct_upgrade = s_struct_upgrade
	PROTOCOL.s_upgrade_speedup = s_upgrade_speedup
	PROTOCOL.s_tribe_heroList = s_tribe_heroList
	PROTOCOL.s_tribe_needNotice = s_tribe_needNotice
	PROTOCOL.s_view_slgInfo = s_view_slgInfo
end
