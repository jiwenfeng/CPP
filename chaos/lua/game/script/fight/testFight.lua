local NPCList = {}
local GroupList = {}

local path_npcList = "chaos.npcList"
local path_groupList = "chaos.groupList"

function GetAllSavedNPCList()
	return NPCList
end

function GetSaveNpcInfoById(name)
	return NPCList[name]
end

function ClearAllSaveNpc()
	for k in pairs(TESTFIGHT.GetAllSavedNPCList()) do 
		DATA.DBRemove(path_npcList, {_id = k})
		NPCList[k] = nil 
	end
end

function SetLogin(connection)
	local roleId = STATS.GenId("testFight")
	connection.roleId = tostring(roleId)
	connection.type = CONNECT_TESTER
	LOGIN.SetConnection(roleId, connection)
end

function SaveNpcList(name)
	local npcInfo = GetSaveNpcInfoById(name)
	if not npcInfo then
		return
	end
	npcInfo._id = name
	if not DATA.Write(path_npcList, {_id = name}, npcInfo) then
		return DATA.log(string.format("Save indent %s data failed: connect server Saved error!", indentId), "market.log")
	end
end

function CheckNpcIdExist(id)
	return FIGHT.GetMonsterInfoById(id)
end

function GetSkillIdByName(name)
	for k, v in pairs(BASESKILL.GetAllSkillList()) do
		if v.name == name then
			return k
		end
	end
end

function GetJobIdByName(name)
	for k, v in pairs(JOB.GetAllJobType()) do 
		if v.name == name then
			return k
		end
	end
end

function GetJobNameById(id)
	return JOB.GetJobInfoById(id).name
end

function GetSkillNameById(id)
	return BASESKILL.GetSkillInfo(id).name
end

function SaveNpcInfo(data)
	NPCList[data.name] = {}
	NPCList[data.name].staffId = data.name
	for k, v in pairs(data) do 
		if k == "job" then
			local jobId = GetJobIdByName(v)
			if not jobId then	
				return false, "请选择职业"
			end
			NPCList[data.name][k] = jobId
		elseif k == "skillList" then
			for _, val in pairs(v) do
				local skillId = GetSkillIdByName(val.id)
				if skillId then
					NPCList[data.name].skillList = NPCList[data.name].skillList or {}
					local buf = {}
					buf.id = skillId
					buf.Lv = val.Lv or 0
					table.insert(NPCList[data.name].skillList, buf)
				end
			end
		elseif k == "hp" then
			NPCList[data.name][k] = v or 0
			NPCList[data.name].hpMax = v or 0
		elseif k == "resist" then
			NPCList[data.name].physcResist = v or 0
			NPCList[data.name].magicResist = v or 0
		else
			NPCList[data.name][k] = v
		end
	end
	SaveNpcList(data.name)
	return true
end

function GetFormationIdByName(name)
	for k, v in pairs(FORMATION.GetAllFormationList()) do 
		if v.name == name then
			return k
		end
	end
	return false, "无效的阵型"
end

function GetFormationOpenPosByName(name)
	for k, v in pairs(FORMATION.GetAllFormationList()) do 
		if v.name == name then
			return v.posList
		end
	end
	return false, "无效的阵型"
end

function FormatMonsterData(monsterId, pos)
	local info = FIGHT.GetMonsterInfoById(monsterId)
	local sData = table.deepcopy(info)
	for _, skill in pairs(info.skillList or {}) do
		local skillInfo = BASESKILL.GetSkillInfo(skill.id)
		if skillInfo.trigger == 2 and skillInfo.list[skill.Lv] then			--预先处理被动技能
			for k, v in pairs(skillInfo.list[skill.Lv].buffer) do
				local attr = SKILL.GetPassiveSkillAttrType(v.id)
				if attr == "physcAtk" or attr == "magicAtk" then
					sData[type .. "Min"] = (sData[type .. "Min"] or 0) + v.num
					sData[type .. "Max"] = (sData[type .. "Max"] or 0) + v.num
				else
					sData[attr] = (sData[attr] or 0) + v.num
				end
			end
		end
	end
	sData.pos = pos
	return sData
end

function BatFightTest(connection, attack, defend, times)
	if not FIGHT.GetMonsterInfoById(attack) then
		return ERROR.ErrorMsg(connection, "无效的攻击方。")
	end
	if not FIGHT.GetMonsterInfoById(defend) then
		return ERROR.ErrorMsg(connection, "无效的防御方。")
	end
	times = tonumber(times)
	if times <= 0 then
		return ERROR.ErrorMsg(connection, "无效的战斗次数。")
	end
	local ret = {win = 0, lose = 0}
	for i = 1, times do
		local function callback(fightId, winner)
			if winner == "attack" then
				ret.win = ret.win + 1
			else
				ret.lose = ret.lose + 1
			end
			if ret.win + ret.lose >= times then
				return ERROR.ErrorMsg(connection, string.format("%s与%s战斗%d次：\n胜利 %d 次，失败 %d 次。", attack, defend, times, ret.win, ret.lose))
			end
		end
		local atList = {formation = {formId = "321001", formLv = 1}, list = {FormatMonsterData(attack, "5")}}
		local defList = {formation = {formId = "321001", formLv = 1}, list = {FormatMonsterData(defend, "5")}}
		FIGHT.BeginFight("test", atList, defList, {}, callback, {fastFight = true})
	end
end



local s_testFight_userExist = function(connection, data)
	if not data then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	local info = CheckNpcIdExist(data)
	if not info then
		return ERROR.ErrorMsg(connection, "该用户不存在，请添加")
	end
	local sData = {}
	sData.skillList = {}
	for k, v in pairs(info) do
		if type(v) ~= "table" then
			if k == "job" then
				sData[k] = GetJobNameById(v)
			else
				sData[k] = v
			end
		else
			for _, val in pairs(v) do
				local buf = {}
				buf.skillName = GetSkillNameById(val.id)
				buf.Lv = val.Lv
				table.insert(sData.skillList, buf)
			end
		end
	end
	connection:SendProtocol("c_testFight_userExist", sData)
end

local s_testFight_getNpcList = function(connection, data)
	if data ~= "get" then
		return ERROR.ErrorMsg(connection, "获取NPC列表失败，无效的协议")
	end
	local sData = {}
	local npcList
	npcList = FIGHT.GetAllMonsterInfoList()
	for k, v in pairs(NPCList) do 
		table.insert(sData, k)
	end
	for k, v in pairs(npcList) do 
		table.insert(sData, k)
	end
	connection:SendProtocol("c_testFight_getNpcList", sData)
end

local s_testFight_beginFight = function(connection, data)
	if not data or type(data) ~= "table" then
		return ERROR.ErrorMsg(connection, "无效的数据")
	end
	local attacker = {}
	attacker.list = {}
	local defender = {}
	defender.list = {}
	for k, v in pairs(data.attack) do
	--	attacker.type = "npc"
		if k == "formation" then
			attacker.formId = GetFormationIdByName(v.formation)
		end
		if k == Lv then
			attacker.formLv = v or 1
		end
		if type(v) == "table" then
			for _, val in pairs(v) do
			--	local buff = FIGHT.GetMonsterData(val.name)
			--	buff.pos = tostring(val.pos)
				local buff = FormatMonsterData(val.name, tostring(val.pos))
				buff.isMain = true
				buff.roleId = val.name
				table.insert(attacker.list, buff)
			end
		end
	end
	for k, v in pairs(data.defend) do
	--	defender.type = "npc"
		if k == "formation" then
			defender.formId = GetFormationIdByName(v.formation)
		end
		if k == Lv then
			defender.formLv = v or 1
		end
		if type(v) == "table" then
			for _, val in pairs(v) do
			--	local buff = FIGHT.GetMonsterData(val.name)
			--	buff.pos = tostring(val.pos)
				local buff = FormatMonsterData(val.name, tostring(val.pos))
				buff.isMain = true
				buff.roleId = val.name
				table.insert(defender.list, buff)
			end
		end
	end
	if table.size(attacker.list) == 0 or table.size(defender.list) == 0 then
		return ERROR.ErrorMsg(connection, "请选择需要参战的NPC")
	end
	local function callback(fightId, winner)
	end
	FIGHT.BeginFight("test", attacker, defender, {connection.roleId}, callback)
end

local s_testFight_addNpcInfo = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.ErrorMsg(connection, "无效的数据")
	end
	if not data.name or data.name == "" then
		return ERROR.ErrorMsg(connection, "请输入NPC的名字")
	end
	local res, msg = SaveNpcInfo(data)
	if not res then
		return ERROR.ErrorMsg(connection, msg)
	end
	connection:SendProtocol("c_testFight_addNpcInfo", "true")
end


local s_test_fight = function(connection, data)
--	local attacker, defender = connection.roleId, "260074"
	local attacker, defender = connection.roleId, USER.GetRoleIdByUserName("xxx2")
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local info = user:GetWorkInfo()
	if info.state == "fighting" then
		local fight = FIGHT.GetOneFight(info.place)
		if fight then
			fight:SetFastFightState(true)
		end
	end
	local function callback(fightId)
		local user = USER.GetOnlineUser(connection.roleId)
		if user then
			user:SetWorkInfo("standby")
		else
			OFFLINE.SetOfflineData(connection.roleId, "workInfo", "standby")
		end
--		FIGHTREC.PlayFightRecord(connection, fightId, connection.roleId)
	end
--	local attacker = {formation = {formId = "321001", formLv = 1}, list = {FormatMonsterData("JW7", "5")}}
--	local defender = {formation = {formId = "321001", formLv = 1}, list = {FormatMonsterData("YL7", "5")}}
	local param = {restoreHp = true, reinforce = {defend = {"260008", "260008"}}}
	local param = {restoreHp = true, reinforce = {defend = {USER.GetRoleIdByUserName("xxx3"), USER.GetRoleIdByUserName("xxx1")}}}
	user:SetWorkInfo("fighting", nil)
	local fightId = FIGHT.BeginFight("test", attacker, defender, {attacker, defender}, callback, param, {mapId = 697001, posX = 500, posY = 475})
end

local s_testFight_getJob = function (connection, data)
	if data ~= "get" then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	local list = JOB.GetAllJobType()
	local sData = {}
	for k, v in pairs(list) do
		table.insert(sData, v.name)
	end
	connection:SendProtocol("c_testFight_getJob", sData)
end

local s_testFight_getSkill = function(connection, data)
	if data ~= "get" then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	local list = BASESKILL.GetAllSkillList()
	local sData = {}
	for k, v in pairs(list) do
		table.insert(sData, v.name)
	end
	connection:SendProtocol("c_testFight_getSkill", sData)
end

local s_testFight_getFormation = function(connection, data)
	if data ~= "get" then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	SetLogin(connection)
	local buf = FORMATION.GetAllFormationList()
	local sData = {}
	for k, v in pairs(buf) do
		table.insert(sData, v.name)
	end
	connection:SendProtocol("c_testFight_getFormation", sData)
end

local s_testFight_formationInfo = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.ErrorMsg(connection, "无效的请求")
	end
	local sData = {}
	sData.type = data.type
	sData.list = GetFormationOpenPosByName(data.name)
	connection:SendProtocol("c_testFight_formationInfo", sData)
end

function __init__()
	DATA.Restore(path_npcList, NPCList)
	DATA.Restore(path_groupList, GroupList)
	PROTOCOL.s_testFight_userExist = s_testFight_userExist
	PROTOCOL.s_testFight_getNpcList = s_testFight_getNpcList
	PROTOCOL.s_testFight_beginFight = s_testFight_beginFight
	
	PROTOCOL.s_test_fight = s_test_fight
	PROTOCOL.s_testFight_addNpcInfo = s_testFight_addNpcInfo
	PROTOCOL.s_testFight_getSkill = s_testFight_getSkill
	PROTOCOL.s_testFight_getJob = s_testFight_getJob
	PROTOCOL.s_testFight_getFormation = s_testFight_getFormation
	PROTOCOL.s_testFight_formationInfo = s_testFight_formationInfo
end