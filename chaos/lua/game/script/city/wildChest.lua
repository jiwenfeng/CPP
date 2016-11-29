--野地宝藏

local FRESH_TIME_LONG = 10 * 60

local AllWildMonsterInfoList = {}
local WildMonsterLvInfo = {}

function GetAllWildMonsterInfoList()
	return AllWildMonsterInfoList
end

function GetWildMonsterInfoById(id)
	return AllWildMonsterInfoList[id]
end

function GetMonsterLvInfo(type, Lv)
	return WildMonsterLvInfo[type][Lv]
end

local AllWildMonsterObjList = {}
local path_wildMonster = "chaos.wildMonster"

function GetAllWildMonsterObjList()
	return AllWildMonsterObjList
end

function GetWildMonsterObjById(id)
	return AllWildMonsterObjList[id]
end

function SetWildMonsterObjById(id, obj)
	AllWildMonsterObjList[id] = obj
end

local MonsterTypeList = {"monster", "chest"}
local nameList = {monster = "游荡的怪物", chest = "神秘的宝箱"}
local modelList = {
	monster = {[30] = 820010, [40] = 820013, [50] = 820015},
	chest = {[30] = 360001, [40] = 360001, [50] = 360001},
}
local MonsterLvList = {30, 40, 50}
------------------------------------------

clsMonster = clsObject:Inherit()

function clsMonster:__init__()
	Super(clsMonster).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsMonster:initsavedata()
	self.__save.Lv = 0
end

function clsMonster:inittempdata()
end

function clsMonster:InitMonster(id, type, lv)
	self:SetId(id)
	self:SetMonsterType(type)
	self:SetLv(lv)
	
	if type == "monster" then
		local lvInfo = GetMonsterLvInfo(type, lv)
		local teamId = lvInfo.monsterList[math.random(1, #lvInfo.monsterList)]
	end
	self:SetTemplate(teamId)
end


function clsMonster:SetId(data)
	self.__save.id = data
end

function clsMonster:GetId()
	return self.__save.id
end

function clsMonster:SetMonsterType(data)
	self.__save.type = data
end

function clsMonster:GetMonsterType()
	return self.__save.type
end

function clsMonster:SetTemplate(template)
	self.__save.template = template
end

function clsMonster:GetTemplate()
	return self.__save.template
end

function clsMonster:SetLv(lv)
	self.__save.Lv = lv
end

function clsMonster:GetLv()
	return self.__save.Lv
end


function clsMonster:GetType()
	return "monster"
end

function clsMonster:GetLocation()
	return GetWildMonsterInfoById(self:GetId()).location
end

function clsMonster:GetName()
--	return nameList[self:GetMonsterType()] .. string.format(" %d级", self:GetLv())
	return nameList[self:GetMonsterType()]
end

function clsMonster:GetModelId()
	local template = self:GetTemplate()
	if not template then
		return modelList[self:GetMonsterType()][self:GetLv()]
	end
	local info = FIGHT.GetMonsterTeamInfoById(template)
	return info.modelId
end

function clsMonster:GetPrize()
	local lvInfo = GetMonsterLvInfo(self:GetMonsterType(), self:GetLv())
	return lvInfo.prize
end


function clsMonster:GetMonsterInfo()
	local buff = {}
	buff.monsterId = self:GetId()
	local locate = self:GetLocation()
	buff.posX = locate.x
	buff.posY = locate.y
	buff.name = self:GetName()
	buff.Lv = self:GetLv()
	buff.modelId = self:GetModelId()
	buff.state = self:GetFightState() and 1 or 0
	buff.type = self:GetMonsterType()
	return buff
end

function clsMonster:BeginFight(user)
	local roleId = user:GetRoleId()
	self:SetFightState(1)
	self:SendMonsterUpdate("state")
	local function call_back(fightId, winner, prize)
		self:SetFightState()
		user:SetWorkInfo("standby")
		if winner == "attack" then
			self:SendMonsterUpdate("disappear")
			SetWildMonsterObjById(self:GetId())
			self:Destroy()
			if prize.gold then
				user:SetGold(prize.gold)
			end
			if prize.nimbus then
				user:SetNimbus(prize.nimbus)
			end
			if prize.spar then
				user:SetSpar(prize.spar)
			end
			if prize.item then
				for k, v in pairs(prize.item) do
					local item = BASEITEM.NewItem(v.goodsId, v.num, {lock = v.lock})
					if item then
						user:AddItem(USERBASE.PACKAGE, item)
					end
				end
			end
		else
			self:SendMonsterUpdate("state")
		end
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_wildMonster_fight", {result = winner == "attack", monsterId = self:GetId()})
		end
	end
	local lvInfo = GetMonsterLvInfo(self:GetMonsterType(), self:GetLv())
	local teamId = lvInfo.monsterList[math.random(1, #lvInfo.monsterList)]
	local param = {restoreHp = true, saveHp = true, id = self:GetId()}
	user:SetWorkInfo("fighting")
	local fightId = FIGHT.BeginFight("wildChest", roleId, self:GetTemplate() or teamId, {roleId}, call_back, param)
end

function clsMonster:GetFightState()
	return self.__temp.fightState
end

function clsMonster:SetFightState(flag)
	self.__temp.fightState = flag
end

local UpdateList = {
	state = function(monster)
		local sData = {}
		sData.monsterId = monster:GetId()
		sData.state = monster:GetFightState() and 1 or 0
		return sData
	end,
	appear = function(monster)
		local sData = {}
		sData.monsterId = monster:GetId()
		sData.type = monster:GetMonsterType()
		sData.name = monster:GetName()
		sData.Lv = monster:GetLv()
		sData.modelId = monster:GetModelId()
		local locate = monster:GetLocation()
		sData.posX = locate.x
		sData.posY = locate.y
		sData.action = "appear"
		return sData
	end,
	disappear = function(monster)
		local sData = {}
		sData.monsterId = monster:GetId()
		sData.action = "disappear"
		return sData
	end,
}

function clsMonster:SendMonsterUpdate(type)
	if not UpdateList[type] then
		return 
	end
	local sData = UpdateList[type](self)
	for k in pairs(CITY.GetAllUserInCityMap()) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_wildMonster_update", sData)
		end
	end
end


function clsMonster:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsMonster:Restore(data)
	self.__save = data
end
-----------------------------------------
function FreshAllWildMonster()
	for k, v in pairs(GetAllWildMonsterInfoList()) do
		if not GetWildMonsterObjById(k) then
			local type = MonsterTypeList[math.random(1, #MonsterTypeList)]
			local Lv = MonsterLvList[math.random(1, #MonsterLvList)]
			local monsterObj = clsMonster:New()
			monsterObj:InitMonster(k, type, Lv)
			SetWildMonsterObjById(k, monsterObj)
			monsterObj:SendMonsterUpdate("appear")
		end
	end
end

function SaveAllWildMonster()
	for k, v in pairs(GetAllWildMonsterObjList()) do
		local buff = v:GetSaveData()
		buff._id = k
		DATA.UpdateData(path_wildMonster, {_id = k}, buff)
	end
end

function RestoreAllWildMonster()
	local function callback(Data, ErrMsg)
		Data = UnSerialize(Data)
		if not Data or #Data <= 0 then
			FreshAllWildMonster()
		else
			for _, v in pairs(Data) do
				local monsterObj = clsMonster:New()
				monsterObj:Restore(v)
				monsterObj:SetId(tostring(v.id))
				SetWildMonsterObjById(tostring(v._id), monsterObj)
			end
		end
	end
	DATA.Read(path_wildMonster, {}, callback)
end

function CalFightPrize(id)
	local wildMonster = GetWildMonsterObjById(id)
	if not wildMonster then
		return
	end
	local lvInfo = GetMonsterLvInfo(wildMonster:GetMonsterType(), wildMonster:GetLv())
	if not lvInfo then
		return
	end
	local prize = {}
	if wildMonster:GetMonsterType() == "monster" then
		local ran = math.random()
		local rate = 0
		for _, v in ipairs(lvInfo.prize) do
			rate = rate + v.rate
			if rate >= ran then
				prize.gold = v.gold
				prize.nimbus = v.nimbus
				prize.spar = v.spar
				break
			end
		end
	else
		local list = BASEITEM.GenRandomItemByNum(lvInfo.prize)
		prize.item = {}
		for k, v in pairs(list) do
			table.insert(prize.item, {goodsId = v.id, num = v.num})
		end
	end
	return prize
end



local s_wildMonster_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = {}
	for k, v in pairs(GetAllWildMonsterObjList()) do
		table.insert(sData, v:GetMonsterInfo())
	end
	connection:SendProtocol("c_wildMonster_list", sData)
end

local s_wildMonster_fight = function(connection, data)
	local wildMonster = GetWildMonsterObjById(data)
	if not wildMonster then
		return ERROR.TipMsg(connction, "该野生怪已经被消灭了。")
	end
	if wildMonster:GetFightState() then
		return ERROR.TipMsg(connction, "这个野生怪正在战斗中。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	wildMonster:BeginFight(user)
end


function __init__()
	local MONSTER = Import("autocode/wildMonsterList.lua")
	AllWildMonsterInfoList = MONSTER.GetTable()
	
	local LVINFO = Import("autocode/wildMonsterLvInfo.lua")
	WildMonsterLvInfo["monster"] = LVINFO.GetTable()
	
	LVINFO = Import("autocode/wildChestLvInfo.lua")
	WildMonsterLvInfo["chest"] = LVINFO.GetTable()
	
	RestoreAllWildMonster()
	
	PROTOCOL.s_wildMonster_list = s_wildMonster_list
	PROTOCOL.s_wildMonster_fight = s_wildMonster_fight
	
	CALLOUT.CallFre(SaveAllWildMonster, 60 * 2)
	CALLOUT.CallFre(FreshAllWildMonster, FRESH_TIME_LONG)
end

