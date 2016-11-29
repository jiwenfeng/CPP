--野外矿藏

local FRESH_TIME_LONG = 5 * 60
local MAX_MINE_NUM = 2

local AllWildMineInfoList = {}		--矿藏地列表
local WildMineLvInfoList = {}		--矿藏等级信息表

function GetAllWildMineInfoList()
	return AllWildMineInfoList
end

function GetWildMineInfoById(id)
	return AllWildMineInfoList[id]
end

function GetMineLvInfo(lv)
	return WildMineLvInfoList[lv]
end

local AllWildMineObjList = {}
local path_wildMine = "chaos.wildMine"

function GetAllWildMineObjList()
	return AllWildMineObjList
end

function GetWildMineObjById(id)
	return AllWildMineObjList[id]
end

function SetWildMineById(id, obj)
	AllWildMineObjList[id] = obj
end

function SetWildMineFreshTime(time)
	AllWildMineObjList.__fresh = AllWildMineObjList.__fresh or {}
	AllWildMineObjList.__fresh.time = time or os.time()
end

function GetWildMineFreshTime()
	AllWildMineObjList.__fresh = AllWildMineObjList.__fresh or {}
	return AllWildMineObjList.__fresh.time
end

local MineLvList = {30, 40, 50}
local MineTypeList = {"gold", "spar"}
local nameList = {["gold"] = "金矿", ["spar"] = "晶石矿"}
local modelList = {gold = 360002, spar = 360003}
local MINE_FRESH_TIMELONG = 12 * 3600
-------------------------------------------------------------

clsMine = clsObject:Inherit()

function clsMine:__init__()
	Super(clsMine).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsMine:initsavedata()
	self.__save.Lv = 0
end

function clsMine:inittempdata()
end

function clsMine:InitMine(id, type, lv)
	self:SetId(id)
	self:SetResType(type)
	self:SetLv(lv)
	
	self:SetResTimeStamp(os.time())
end


function clsMine:SetId(data)
	self.__save.id = data
end

function clsMine:GetId()
	return self.__save.id
end

function clsMine:SetResType(data)
	self.__save.type = data
end

function clsMine:GetResType()
	return self.__save.type
end

function clsMine:SetLv(lv)
	self.__save.Lv = lv
end

function clsMine:GetLv()
	return self.__save.Lv
end

function clsMine:GetOwner()
	return self.__save.owner
end

function clsMine:SetOwner(data)
	self.__save.owner = data
end

function clsMine:GetResIncome()
	local info = GetMineLvInfo(self:GetLv())
	if self:GetResType() == "gold" then
		return info.goldIncome
	end
	return info.sparIncome
end

function clsMine:GetLocation()
	local info = GetWildMineInfoById(self:GetId())
	return info.location
end

function clsMine:GetName()
--	return nameList[self:GetResType()] .. string.format(" %d级", self:GetLv())
	return nameList[self:GetResType()]
end

function clsMine:GetModelId()
	return modelList[self:GetResType()]
end

function clsMine:GetType()
	return "mine"
end

function clsMine:GetLeftTime()
	return GetWildMineFreshTime() + MINE_FRESH_TIMELONG - os.time()
end

function clsMine:SetResNum(data)
	self.__save.resouce = data
end

function clsMine:GetResNum()
	return math.floor(self.__save.resouce or 0)
end

function clsMine:ModResNum(data)
	self.__save.resouce = (self.__save.resouce or 0) + data
end

function clsMine:SetResTimeStamp(data)
	self.__save.timestamp = data or os.time()
end

function clsMine:GetResTimeStamp()
	return self.__save.timestamp or os.time()
end

function clsMine:ProResIncome()
	if self:GetOwner() then
		local timelong = os.time() - self:GetResTimeStamp()
		local income = self:GetResIncome()
		local num = income * timelong / 3600
		self:ModResNum(num)
	end
	self:SetResTimeStamp(os.time())
--	self:SendMineUpdate("num")
end

function clsMine:GetMineInfo(flag)
	local buff = {}
	buff.mineId = self:GetId()
	local locate = self:GetLocation()
	buff.posX = locate.x
	buff.posY = locate.y
	buff.speed = self:GetResIncome()
	buff.name = self:GetName()
	buff.modelId = self:GetModelId()
	buff.Lv = self:GetLv()
	buff.timelong = self:GetLeftTime()
	if flag then
		local owner = self:GetOwner()
		if owner then
			buff.owner = USER.GetUserNickById(owner)
			buff.userLv = USER.GetUserNickInfo(owner, "Lv")
			buff.phyle = USER.GetUserNickInfo(owner, "phyle")
			local unionId = USER.GetUserNickInfo(owner, "unionId")
			local union = UNION.GetUnionById(unionId)
			if union then
				buff.unionName = union:GetName()
			end
		end
		buff.state = self:GetFightState() and 1 or 0
	else
		buff.num = self:GetResNum()
	end
	return buff
end

function clsMine:OccupancyFight(user)
	local roleId = user:GetRoleId()
	local function call_back(fightId, winner)
		self:SetFightState()
		if winner == "attack" then
			local owner = self:GetOwner()
			if owner then
				self:SendMailWildMineLost(roleId)
				local tUser = USER.GetOnlineUser(owner)
				if tUser then
					tUser:AbandonWildMine(self:GetId())
				else
					OFFLINE.SetOfflineData(owner, "abandonMine", self:GetId())
				end
			end
			self:SetOwner(roleId)
			self:SendMineUpdate("owner")
			local user = USER.GetOnlineUser(roleId)
			if user then
				user:AddWildMine(self:GetId())
				user:SetWorkInfo("standby")
			else
				OFFLINE.SetOfflineData(roleId, "addMine", self:GetId())
				OFFLINE.SetOfflineData(roleId, "workInfo", "standby")
			end
		else
			self:SendMineUpdate("state")
		end
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_wildMine_fight", {result = winner == "attack", mineId = self:GetId()})
		end
	end
	local lvInfo = GetMineLvInfo(self:GetLv())
	local teamId = lvInfo.monsterList[math.random(1, #lvInfo.monsterList)]
	local param = {restoreHp = true, saveHp = true, id = self:GetId()}
	user:SetWorkInfo("fighting")
	self:SetFightState(true)
	self:SendMineUpdate("state")
	local fightId = FIGHT.BeginFight("wildMine", roleId, self:GetOwner() or teamId, {roleId}, call_back, param)
end

function clsMine:SetFightState(flag)
	self.__temp.fightState = flag
end

function clsMine:GetFightState()
	return self.__temp.fightState
end

function clsMine:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsMine:Restore(data)
	self.__save = data
end


function clsMine:SendMailWildMineLost(roleId)
	local owner = self:GetOwner()
	if owner then
		local mail = {type = "fight"}
		mail.title = "矿藏争夺"
		mail.content = string.format("%s   %s Lv %d\n", os.date("%F %T "), USER.GetUserNickById(roleId), USER.GetUserNickInfo(roleId, "Lv"))
		local locate = self:GetLocation()
		mail.content = mail.content .. string.format("占领了您的 <a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>。", self:GetName(), self:GetName(), locate.x, locate.y)
		MAIL.SendSysMail(owner, mail)
	end
end

local UpdateList = {
	state = function(mineObj)
		local sData = {}
		sData.mineId = mineObj:GetId()
		sData.type = "state"
		sData.state = mineObj:GetFightState() and 1 or 0
		return true, sData
	end,
	owner = function(mineObj)
		local sData = {}
		sData.mineId = mineObj:GetId()
		local owner = mineObj:GetOwner()
		if owner then
			sData.owner = USER.GetUserNickById(owner)
		else
			sData.owner = ""
		end
		sData.state = mineObj:GetFightState() and 1 or 0
		sData.type = "owner"
		return true, sData
	end,
	num = function(mineObj)
		local sData = {}
		local user = USER.GetOnlineUser(mineObj:GetOwner())
		local info = user:GetFocus()
		if info and info.type == "mainHall" then
			sData.mineId = mineObj:GetId()
			sData.num = mineObj:GetResIncome()
			sData.type = "num"
			return true, sData
		end
	end,
	lose = function(mineObj)
		local sData = {}
		local user = USER.GetOnlineUser(mineObj:GetOwner())
		local info = user:GetFocus()
		if info and info.type == "mainHall" then
			sData.mineId = mineObj:GetId()
			sData.type = "lose"
			return true, sData
		end
	end
}

function clsMine:SendMineUpdate(type)
	if not UpdateList[type] then
		return 
	end
	local flag, sData = UpdateList[type](self)
	if not flag then
		return
	end
	for k in pairs(CITY.GetAllUserInCityMap()) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_wildMine_update", sData)
		end
	end
end

--遗弃
function clsMine:Abandon()
	self:SetOwner()
	self:SendMineUpdate("owner")
end

--收获
function clsMine:Havest()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local num = self:GetResNum()
	local resType = self:GetResType()
	self:SetResNum(0)
	if resType == "gold" then
		user:SetGold(num)
	elseif resType == "spar" then
		user:SetSpar(num)
	end
	return num, resType
end
----------------------------------------------------------

local clsUser = USERBASE.clsUser

function clsUser:AddWildMine(id)
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	local mainHall = tribe:GetStruct("mainHall")
	local mineObj = GetWildMineObjById(id)
	mainHall:SetWildById(id, mineObj)
end

function clsUser:AbandonWildMine(id)
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	local mainHall = tribe:GetStruct("mainHall")
	mainHall:SetWildById(id)
	local wildMine = GetWildMineObjById(id)
	if wildMine:GetOwner() == self:GetRoleId() then
		wildMine:Abandon()
	end
end

function clsUser:GetWildMineNum()
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	local mainHall = tribe:GetStruct("mainHall")
	return table.size(mainHall:GetWild())
end

-----------------------------------------------------------

function NewMine(id, type, Lv)
	local mineObj = clsMine:New()
	mineObj:InitMine(id, type, Lv)
	SetWildMineById(id, mineObj)
end

function CalWildMineIncome()
	for k, v in pairs(GetAllWildMineObjList()) do
		if k ~= "__fresh" then
			v:ProResIncome()
		end
	end
end

function DestroyAllWildMine()
	for k, v in pairs(GetAllWildMineObjList()) do
		if k ~= "__fresh" then
			local owner = v:GetOwner()
			if owner then
				local user = USER.GetOnlineUser(owner)
				if user then
					user:AbandonWildMine(k)
				else
					OFFLINE.SetOfflineData(owner, "abandonMine", k)
				end
				v:SetOwner()
			end
			v:Destroy()
		end
	end
end

function FreshAllWildMine()
	DestroyAllWildMine()
	SetWildMineFreshTime(os.time())
	for k, v in pairs(GetAllWildMineInfoList()) do
		local type = MineTypeList[math.random(1, #MineTypeList)]
		local Lv = MineLvList[math.random(1, #MineLvList)]
		local mineObj = clsMine:New()
		mineObj:InitMine(k, type, Lv)
		SetWildMineById(k, mineObj)
		mineObj:SendMineUpdate("new")
	end
	for k in pairs(CITY.GetAllUserInCityMap()) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			PROTOCOL.s_wildMine_list(con, "get")
		end
	end
end

function SaveAllWildMine()
	for k, v in pairs(GetAllWildMineObjList()) do
		local buff = {}
		if k == "__fresh" then
			buff = v
		else
			buff = v:GetSaveData()
		end
		buff._id = tostring(k)
		DATA.UpdateData(path_wildMine, {_id = buff._id}, buff)
	end
end

function RestoreAllWildMine()
	local function callback(Data, ErrMsg)
		Data = UnSerialize(Data)
		if not Data or #Data <= 0 then
			FreshAllWildMine()
		else
			local buff = {}
			for k, v in ipairs(Data) do
				buff[v._id] = v
			end		
			for k in pairs(GetAllWildMineInfoList()) do
				local mineObj = clsMine:New()
				local savedData = buff[k]
				if savedData then
					mineObj:Restore(savedData)
					mineObj:ProResIncome()
				else
					local type = MineTypeList[math.random(1, #MineTypeList)]
					local Lv = MineLvList[math.random(1, #MineLvList)]
					mineObj:InitMine(k, type, Lv)
				end
				SetWildMineById(k, mineObj)
			end
			
			if buff["__fresh"] then
				SetWildMineFreshTime(buff["__fresh"].time)
			else
				SetWildMineFreshTime(os.time())
			end
			if GetWildMineFreshTime() + MINE_FRESH_TIMELONG - os.time() <= 0 then
				FreshAllWildMine()
			end
		end
	end
	DATA.Read(path_wildMine, {}, callback)
end


--野地矿藏列表
local s_wildMine_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = {}
	for k, v in pairs(GetAllWildMineObjList()) do
		if k ~= "__fresh" then
			table.insert(sData, v:GetMineInfo(true))
		end
	end
	connection:SendProtocol("c_wildMine_list", sData)
end
--放弃野地
local s_wildMine_abandon = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	data = tostring(data)
	local wildMine = GetWildMineObjById(data)
	if not wildMine then
		return ERROR.TipMsg(connection, "没有指定的野地矿藏信息。")
	end
	if wildMine:GetOwner() ~= connection.roleId then
		return ERROR.TipMsg(connection, "您没有占领该野地")
	end
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local mainHall = tribe:GetStruct("mainHall")
	if mainHall then
		mainHall:SetWildById(data)
	end
	wildMine:Abandon()
	connection:SendProtocol("c_wildMine_abandon", data)
end
--占领野地
local s_wildMine_fight = function(connection, data)
	local wildMine = GetWildMineObjById(data)
	if not wildMine then
		return ERROR.TipMsg(connection, "没有指定的野地矿藏信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local mainHall = tribe:GetStruct("mainHall")
	if mainHall:GetWildMineNum() >= MAX_MINE_NUM then
		return ERROR.TipMsg(connection, string.format("您最多只能占领%d个矿藏。", MAX_MINE_NUM))
	end
	local mineObj = mainHall:GetWildById(data)
	if mineObj then
		return ERROR.TipMsg(connection, "您已经占领了这个矿藏。")
	end
	if wildMine:GetFightState() then
		return ERROR.TipMsg(connection, "这个矿藏已经处于争夺当中，请稍等。")
	end
	user:SetFuncStatById(SETTING.FUNC_ATK_WILD_MINE, 1)
	wildMine:OccupancyFight(user)
end


function __init__()
	local WILDMINE = Import("autocode/wildMineList.lua")
	AllWildMineInfoList = WILDMINE.GetTable()
	
	local LVINFO = Import("autocode/wildMineLvInfo.lua")
	WildMineLvInfoList = LVINFO.GetTable()
	
	RestoreAllWildMine()
	
	PROTOCOL.s_wildMine_list = s_wildMine_list
	PROTOCOL.s_wildMine_abandon = s_wildMine_abandon
	PROTOCOL.s_wildMine_fight = s_wildMine_fight
	
	CALLOUT.CallFre(SaveAllWildMine, 60 * 2)
	CALLOUT.CallFre(CalWildMineIncome, FRESH_TIME_LONG)
	CALLOUT.Daily(getfenv(2), 8, 0, FreshAllWildMine)
	CALLOUT.Daily(getfenv(2), 20, 0, FreshAllWildMine)
end
