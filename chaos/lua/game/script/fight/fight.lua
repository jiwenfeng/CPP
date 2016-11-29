local AllFightList = {}
local CallBackFuncList = {}
local AllMonsterInfoList = {}				--NPC战斗数据列表
local AllMonsterTeamList = {}				--NPC队伍数据列表

local FightBackGroundList = {}				--战斗背景场景

function GetFightBGByType(type)
	return FightBackGroundList[type]
end


function GetAllMonsterInfoList()
	return AllMonsterInfoList
end
--获取npc战斗数据
function GetMonsterInfoById(id)
	return AllMonsterInfoList[id] or TESTFIGHT.GetSaveNpcInfoById(id)
end

function GetAllMonsterTeamList()
	return AllMonsterTeamList
end

function GetMonsterTeamInfoById(id)
	return AllMonsterTeamList[id]
end

-----------------------------------------------------------------------

--玩家战斗数据

local AllFightDataList = {}
local path_fightData = "chaos.fightData"
local fightDataInLoading = {}
local FIGHT_DATA_ONLINE_TIME = 5 * 60
local NONE_FORM_ID = "321001"
local NONE_FORM_LV = 1


function GetFightDataById(id)
	UpdateUserFightData(id)
	return AllFightDataList[id]
end

function SetFightDataById(id, buff)
	if buff then
		buff.timestamp = os.time()
	end
	AllFightDataList[id] = buff
end

function SaveUserFightData(id)
	DATA.SaveData(path_fightData, AllFightDataList, id)
end

function UpdateUserFightData(roleId)
	local user = USER.GetOnlineUser(roleId)
	if user then
		local lineup = user:GetCurFightLineup()
		local buff = {lineup = lineup}
		local form = user:GetCurForm()
		buff.formId = form:GetId()
		buff.formLv = form:GetLv()
		local tribe = user:GetTribe()
		if tribe then
			local academy = tribe:GetStruct("academy")
			if academy then
				local lv = academy:GetTechLv("359009")
				local info = ACADEMY.GetTechById("359009")
				buff.linkRate = info.list[lv].attr.linkRate
			end
		end
		buff.list = {}
		--buff.list["user"] = user:GetFightAttr()
		buff.list[roleId] = user:GetFightAttr()
		for k, v in pairs(user:GetHeroList()) do
			local state = v:GetWorkState()
			if state == "standby" or state == "stay" then
				buff.list[k] = v:GetFightAttr()
			end
		end
		local pet = user:GetPet()
		buff.petGuard = pet:GetGuardRate()
		SetFightDataById(roleId, buff)
		SaveUserFightData(roleId)
	end
end


--正在加载中的战斗数据
function SetFightDataInLoading(roleId, call_back)
	fightDataInLoading[roleId] = fightDataInLoading[roleId] or {}
	if call_back then
		table.insert(fightDataInLoading[roleId], call_back)
	end
end

function GetFightDataInLoading(roleId)
	return fightDataInLoading[roleId]
end

function RemoveFightDataInLoading(roleId)
	fightDataInLoading[roleId] = nil
end


function CheckOnlineFightData()
	for k, v in pairs(AllFightDataList) do
		if os.time() - v.timestamp >= FIGHT_DATA_ONLINE_TIME then
			SetFightDataById(k)
		end
	end
end

--恢复战斗数据
function RestoreUserFightData(roleId, call_back)
	if type(call_back) ~= "function" then
		return
	end
	local function read_back(Data)
		Data = UnSerialize(Data)
		if not Data or not Data[1] then
			DATA.log(string.format("没有找到该玩家的战斗信息：%s。", roleId), "fightData.log")
			for _, func in ipairs(GetFightDataInLoading(roleId)) do
				func(false)
			end
			return
		end
		SetFightDataById(roleId, Data[1])
		for _, func in ipairs(GetFightDataInLoading(roleId)) do
			func(true)
		end
		RemoveFightDataInLoading(roleId)		--战斗数据恢复完毕
	end
	if GetFightDataById(roleId) then
		return call_back(true)
	end
	if GetFightDataInLoading(roleId) then
		return SetFightDataInLoading(roleId, call_back)
	end
	SetFightDataInLoading(roleId, call_back)
	local ret, msg = DATA.Read(path_fightData, {_id = roleId}, read_back)
	if not ret then
		DATA.log(string.format("read data failed on %s:%s.", roleId, msg or "connection error"), "fightData.log")
	end
end

--批量加载战斗数据
function LoadFightDataList(list, call_back)
	local dataList = {}
	local needRead = {}
	for _, v in pairs(list) do
		local buff = GetFightDataById(v)
		if buff then
			buff.timestamp = os.time()
			dataList[v] = buff
		else
			table.insert(needRead, v)
		end
	end
	local function read_back(Data)
		Data = UnSerialize(Data)
		for _, v in ipairs(Data) do
			dataList[v._id] = v
			SetFightDataById(v._id, v)
		end
		call_back(dataList)
	end
	if #needRead <= 0 then
		return call_back(dataList)
	end
	DATA.BatchRead(path_fightData, "_id", needRead, nil, read_back)
end

--格式化单人战斗数据
--	fightData = {formId = , formLv = , lineup = {{v.id = , v.pos = , v.isMain = }, {}, ...}, list = {["user"] = , [heroId] = , ...}}	
function FormatSingleUserData(fightData)
	local sData = {list = {}, formation = {}}
	sData.formation.formId = fightData.formId or NONE_FORM_ID
	sData.formation.formLv = fightData.formLv or NONE_FORM_LV
	for k, v in ipairs(fightData.lineup) do
		if fightData.list[v.id] then
			local buff = table.deepcopy(fightData.list[v.id])
			buff.pos = v.pos
			--if v.id == "user" then
			if IsUserId(v.id) then
				buff.isMain = 1
			end
			table.insert(sData.list, buff)
		end
	end
	sData.petGuard = fightData.petGuard
	return sData
end

--格式化组队战斗数据(玩家队伍 / 玩家npc混编队伍)
--	teamInfo = {formId = , formLv = , lineup = {{v.roleId = , v.id = , v.pos = , v.isMain = }, {}, ...}}

local attrList = {
	hpMax = 1, physcAtkMin = 1, physcAtkMax = 1, speed = 1, 
	magicAtkMin = 1, magicAtkMax = 1, physcDef = 1, magicDef = 1
}

function FormatTeamUserData(teamInfo, dataList)
	local sData = {list = {}, formation = {}}
	sData.formation.formId = teamInfo.formId or NONE_FORM_ID
	sData.formation.formLv = teamInfo.formLv or NONE_FORM_LV
	for k, v in ipairs(teamInfo.lineup) do
		if GetMonsterInfoById(v.id) then
			local buff = FormatNpcFightData(v.id)
			buff.pos = v.pos
			buff.isMain = v.isMain
			table.insert(sData.list, buff)
		else
			local userData = dataList[v.roleId]
			if userData then
				local buff = table.deepcopy(userData.list[v.id])
				buff.pos = v.pos
				buff.isMain = v.isMain
				if v.attrAdd then			--单体属性增益
					for k in pairs(attrList) do
						buff[k] = (buff[k] or 0) * (1 + v.attrAdd)
					end
				end
				table.insert(sData.list, buff)
			end
		end
	end
	local formInfo = FORMATION.GetFormationInfoById(sData.formation.formId)
	if formInfo then
		for k, v in pairs(formInfo.list[sData.formation.formLv].attr) do
			for _, buff in pairs(sData.list) do
				if k == "physcAtk" or k == "magicAtk" then
					buff[k .. "Min"] = (buff[k .. "Min"] or 0) + v
					buff[k .. "Max"] = (buff[k .. "Max"] or 0) + v
				else
					buff[k] = (buff[k] or 0) + v
				end
			end
		end
	end
	return sData
end

--格式化单个Npc战斗数据
function FormatNpcFightData(npcId)
	local buff = table.deepcopy(GetMonsterInfoById(npcId))
	for _, skill in pairs(buff.skillList or {}) do
		local skillInfo = BASESKILL.GetSkillInfo(skill.id)
		if not skillInfo then
			print("\n\n------------------------------Error on Fight Skill: ", teamId, skill.id)
		else
			if skillInfo.trigger == 2 and skillInfo.list[skill.Lv] then			--预先处理被动技能
				for k, v in pairs(skillInfo.list[skill.Lv].buffer) do
					local attr = SKILL.GetPassiveSkillAttrType(v.id)
					if attr == "physcAtk" or attr == "magicAtk" then
						buff[attr .. "Min"] = (buff[attr .. "Min"] or 0) + v.num
						buff[attr .. "Max"] = (buff[attr .. "Max"] or 0) + v.num
					else
						buff[attr] = (buff[attr] or 0) + v.num
					end
				end
			end
		end
	end
	return buff
end

--格式化npc队伍战斗数据
function FormatNpcTeamData(teamId)
	local teamData = GetMonsterTeamInfoById(teamId)
	if not teamData then return end
	local sData = {list = {}, formation = {}}
	sData.formation.formId = teamData.formId or NONE_FORM_ID
	sData.formation.formLv = teamData.formLv or NONE_FORM_LV
	for _, v in pairs(teamData.list) do
		local buff = FormatNpcFightData(v.id)
		buff.pos = v.pos
		buff.boss = v.isBoss
		table.insert(sData.list, buff)
	end
	local formInfo = FORMATION.GetFormationInfoById(sData.formation.formId)
	if formInfo then
		local openNum = #formInfo.list[sData.formation.formLv].posList
		local takenNum = #teamData.list
		for k, v in pairs(formInfo.list[sData.formation.formLv].attr) do
			local add = math.ceil(v * takenNum / openNum)
			for _, buff in pairs(sData.list) do
				if k == "physcAtk" or k == "magicAtk" then
					buff[k .. "Min"] = (buff[k .. "Min"] or 0) + add
					buff[k .. "Max"] = (buff[k .. "Max"] or 0) + add
				else
					buff[k] = (buff[k] or 0) + add
				end
			end
		end
	end
	return sData
end

--------------------------------------------------------------------------

function GetOneFight(fightId)
	return AllFightList[fightId]
end

function GetAllFightList()
	return AllFightList
end

function RemoveOneFight(fightId)
	local fight = GetOneFight(fightId)
	if not fight then
		return
	end
	if fight:GetState() == "send_wait" then
		fight:SendFightDataUpdate()
	end
	fight:Destroy()
	AllFightList[fightId] = nil
end

function FightOverCallBack(fightId, winner, prize, stats)
	local callback = CallBackFuncList[fightId]
	if not callback then
		return
	end
	callback(fightId, winner, prize, stats)
	RemoveOneFight(fightId)
	CallBackFuncList[fightId] = nil
end


function GetFightLocate(fightType, roleId, locate)
	locate = locate or {}
	if not locate.mapId then
		local bgType = fightType
		if fightType == "pillage" or fightType == "cityAttack" then
	------------------------------------ 尚未实现 -----------------------------------------
			local tribeForm = USER.GetUserNickInfo(roleId, "tribeForm")		
			if tribeForm then
				bgType = fightType .. roleInfo.tribeForm
			end
		end
		local info = GetFightBGByType(bgType)
		if info then
			locate.mapId = info.mapId
			locate.posX = locate.posX or info.posX
			locate.posY = locate.posY or info.posY
		end
	end
	return locate
end

--开始战斗
function BeginFight(fightType, attacker, defender, updateList, callback, param, locate)
	local fightId = STATS.GenId("fight")
	local fight = CLSFIGHT.clsFight:New()
	AllFightList[fightId] = fight
	CallBackFuncList[fightId] = callback
	
	local userList = {}
	local function get_user(data)
		if type(data) == "string" then
			if USER.GetUserNickById(data) then
				table.insert(userList, data)
			end		
		elseif type(data) == "table" then
			for k, v in pairs(data.lineup) do
				if USER.GetUserNickById(v.roleId) then
					table.insert(userList, v.roleId)
				end
			end
		end
	end
	get_user(attacker)
	get_user(defender)
	param = param or {}
	if param.reinforce then
		for k, v in pairs(param.reinforce.attack or {}) do
			get_user(v)
		end
		for k, v in pairs(param.reinforce.defend or {}) do
			get_user(v)
		end
	end
	local function begin_fight(userDataList)
		fight:InitFight(fightId, fightType, attacker, defender, updateList, param, userDataList, GetFightLocate(fightType, defender, locate))
	end
	LoadFightDataList(userList, begin_fight)
	return fightId
end

function ShutDownFinishAllFight()
	for k, v in pairs(GetAllFightList()) do
		v:SetFastFightState(true)
	end
end
------------------------------------------------------------------------

local s_fight_drawEnd = function(connection, data)
	data.round = tonumber(data.round)
	if type(data) ~= "table" or not data.fightId or not data.round then
		return
	end
	local fight = GetOneFight(data.fightId)
	if fight and fight:GetState() ~= "over" then
		if fight:GetRound() ~= data.round then
			return
		end
		fight:UpdateUserState(connection.roleId, 1)
	else
		local recObj = FIGHTREC.GetFightRecObj(connection.roleId)
		if not recObj or recObj:GetId() ~= data.fightId or recObj:GetRound() ~= (data.round + 1) then
			return
		end
		recObj:SetUserState(1)
	end
end

local s_fight_fastFight = function(connection, data)
--[[	local fight = GetOneFight(data)
	if not fight then 
--		return ERROR.TipMsg(connection, "没有该次战斗信息")
		return
	end
	if table.size(fight:GetUserList()) > 1 then
		return ERROR.TipMsg(connection, "该场战斗不能使用快速战斗选项。")	
	end]]
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetDailyFastFightTimes() >= user:GetDailyFastFightMaxNum() then
		return ERROR.TipMsg(connection, "您今日可跳过战斗的次数已经用完。提升vip等级可获得更多次数。")
	end
	user:SetDailyFastFightTimes(1)
	user:SetWorkInfo("standby")
--	fight:SetFastFightState(true)
end
--[[
local s_fight_stunt = function(connection, data)
	if not data.fightId or not data.type then
		return ERROR.TipMsg(connection, "协议错误，请选择战斗场次及阵营。")
	end
	local fight = GetOneFight(data.fightId)
	if not fight then
		ERROR.TipMsg(connection, "没有该次战斗信息")
	end
	local rage = fight:GetRage(data.type)
	if rage < fight:GetMaxRage() then
		return ERROR.TipMsg(connection, "怒气值不足，不能释放绝技。")
	end
	
	
	connection:SendProtocol("c_fight_stunt", data.fightId)
end
]]

local s_fight_ready = function(connection, data)
	if type(data) ~= "table" or not data.fightId or not data.type then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local fight = GetOneFight(data.fightId)
	if fight then
		if data.type ~= "start" and data.type ~= "reinforce" then
			return ERROR.TipMsg(connection, "无效的类型。")
		end
		fight:SendFightDataUpdate()
	end
	local fightRec = FIGHTREC.GetFightRecObj(connection.roleId)
	if fightRec then
		fightRec:NewRound()
	end
end


function __init__()
	
	local MONSTER_LIST = Import("autocode/monsterList.lua")
	AllMonsterInfoList = MONSTER_LIST.GetTable()
	
	local MONSTER_TEAM = Import("autocode/monsterTeamList.lua")
	AllMonsterTeamList = MONSTER_TEAM.GetTable()
	
	local FIGHTBG = Import("autocode/fightBG.lua")
	FightBackGroundList = FIGHTBG.GetTable()
	
	CALLOUT.CallFre(CheckOnlineFightData, FIGHT_DATA_ONLINE_TIME)
	
	PROTOCOL.s_fight_ready = s_fight_ready
	PROTOCOL.s_fight_drawEnd = s_fight_drawEnd
	PROTOCOL.s_fight_fastFight = s_fight_fastFight
end
