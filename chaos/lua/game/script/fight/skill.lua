--技能


local RESET_SKILL_COST = 5		--重置技能所需元宝
local RESET_POINT_COST = 100	--重置技能点所需元宝

local AllSkillList = {}

--技能攻击范围类型(1单体 2横行 3纵列 4全体)
local skillRange = {[1] = 1, [2] = 4, [3] = 4, [4] = 1, [5] = 2, [6] = 3, [7] = 1}
local SKILL_POINT_PILL_ID = 101044

------------------------------------------------------
local ResetableSkillList = {}

function GetAllResetableSkillList()
	return ResetableSkillList
end

function GetResetableSkillListByjob(job)
	return ResetableSkillList[job]
end
------------------------------------------------------

function GetSkillInfoByLv(skillId, Lv)
	local info = AllSkillList[skillId]
	if not info then return end
	return info.list[Lv]
end

function GetSkillMaxLv()
	local info = AllSkillList[skillId]
	return #info.list
end

function GetSkillInfo(skillId)
	return AllSkillList[skillId]
end

function GetSkillNameById(skillId)
	return AllSkillList[skillId].name
end

--获取某种类型的技能列表（包含玩家与怪物的）
function GetSkillListByJob(job, classify)
	local list = {}
	for k, v in pairs(BASESKILL.GetAllSkillList()) do
		if v.validJob[job] then
			if classify then
				list[v.type] = list[v.type] or {}
				table.insert(list[v.type], k)
			else
				table.insert(list, k)
			end
		end
	end
	return list
end

--职业的特性技能
function GetJobFeatureSkill(job)
	local basic = JOB.GetBasicJob(job)
	local list = {}
	for k, v in pairs(BASESKILL.GetAllSkillList()) do
		if v.validJob[basic] and v.type == 4 then
			table.insert(list, k)
		end
	end
	if #list < 1 then
		return
	end
	return list[math.random(1, #list)]
end

function GenRandomSkill(job, type)
	local basicJob = JOB.GetBasicJob(job)
	local skillList = GetResetableSkillListByjob(basicJob)
	local valid = skillList[type]
	if #valid < 1 then
		return
	end
	return valid[math.random(1, #valid)]
end

--格式化技能信息
function GetFormatedSkillInfo(skillId, curLv)
	local info = BASESKILL.GetSkillInfo(skillId)
	local buff = {}
	buff.skillId = skillId
	buff.type = info.type
	buff.name = info.name
	buff.quality = info.quality
	buff.skillLv = curLv
	buff.range = skillRange[info.range]
	buff.maxLv = #info.list
	local curInfo = info.list[curLv]
	if curInfo then
		buff.curInfo = {lastTimes = curInfo.lastTimes, describe = curInfo.explain}
		if curInfo.cdTime and curInfo.cdTime > 1 then
			buff.curInfo.cdTimes = curInfo.cdTime - 1
		end
	end
	local nextInfo = info.list[curLv + 1]
	if nextInfo then
		buff.nextInfo = {lastTimes = nextInfo.lastTimes, describe = nextInfo.explain, needLv = nextInfo.needLv}
		if nextInfo.cdTime and nextInfo.cdTime > 1 then
			buff.nextInfo.cdTimes = nextInfo.cdTime - 1
		end
	end
	return buff
end




--获取目标阵营类型
function GetAttackTargetType(attackType, skillId)
	local targetType = attackType
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	if skillInfo.target == 3 then
		targetType = attackType == "attack" and "defend" or "attack"
	end
	return targetType
end


----------------------------------------------------------------

local SkillClassList = {	
	["atk"] = require "fight/skill/clsAtk",
	["atk_buff"] = require "fight/skill/clsAtkBuff",
	["restore"] = require "fight/skill/clsRestore",
	["buff"] = require "fight/skill/clsBuff",
	["stunt"] = require "fight/skill/clsStunt",
}

function NewSkill(skillId, skillLv)
	local info = BASESKILL.GetSkillInfo(skillId)
	local class = assert(SkillClassList[info.class], "NewSkill Error on:" .. skillId)
	local skillObj = class:New()
	skillObj:InitSkill(skillId, skillLv)
	return skillObj
end


--获取主动技能释放效果
function GetSkillEffect(fightObj, attacker, skillId, skillLv, targetType, targetPos, isLinked)
	local skillObj = NewSkill(skillId, skillLv)
	local buff = skillObj:GetSkillEffect(fightObj, attacker, targetType, targetPos, isLinked)
	return buff
end

--处理持续性技能效果
function ProSkillEffectBuffer(target, effectId, times, num, rate, damage, isMyRound)
	local effectInfo = BASESKILL.GetSkillEffectInfoById(effectId)
	if not effectInfo.IsBuffer then
		return
	end
	if effectInfo.state == "bleed" or effectInfo.state == "poison" then			--持续流血
		num = math.ceil(damage * rate)
	end
	target:AddState(effectInfo.state, times, num, rate, isMyRound)
end

--处理被动性技能效果
function GetPassiveSkillAttrType(effectId)
	local effectInfo = BASESKILL.GetSkillEffectInfoById(effectId)
	if not effectInfo.IsPassive then
		return
	end
	return effectInfo.attr
end

function GetPassiveSkillEffect(skillId, skillLv)
	local skillInfo = BASESKILL.GetSkillInfo(skillId)
	if not skillInfo then
		print("skill.lua:GetPassiveSkillEffect, skillId error!", skillId)
	end
	if skillInfo.trigger ~= 2 then
		return
	end
	local sData = {}
	for k, v in ipairs(skillInfo.list[skillLv].buffer) do
		local attr = GetPassiveSkillAttrType(v.id)
		sData[attr] = v.num
	end
	return sData
end



--[[
function Reset(user)
	ResetSkill(user)
	for k, v in pairs(user:GetHeroList()) do
		ResetSkill(v)
	end
end

function ResetSkill(user)
	local skillList = GetResetableSkillListByjob(user:GetJob())
	local list = {}
	for k, v in ipairs(skillList) do
		local id = v[math.random(1, #v)]
		table.insert(list, {type = tostring(k), skillId = id})
	end
	for _, v in ipairs(list) do
		local info = user:GetSkill(v.pos)
		user:SetSkill(v.type, v.skillId, info and info.Lv or 1)
	end
	user:FreshTotalAttr()				--刷新全部属性for k, v in pairs(user:GetHeroList()) do
end
]]



--获取可重置的技能列表
local s_skill_resetList = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user
	if data ~= "user" then
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	local list = GetResetableSkillListByjob(obj:GetJob())
	local sData = {heroId = data, list = {}, resetCost = RESET_SKILL_COST}
	for _, skillList in ipairs(list) do
		for _, v in ipairs(skillList) do
			local info = BASESKILL.GetSkillInfo(v)		
			local buff = GetFormatedSkillInfo(v, #info.list)
			table.insert(sData.list, buff)
		end
	end
	connection:SendProtocol("c_skill_resetList", sData)
end

local s_skill_resetGen = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local moneyInfo = MONEY.GetGoldInfoByType("ingot")
	if USERBASE.clsUser[moneyInfo.get](user) < RESET_SKILL_COST then
		return ERROR.TipMsg(connection, string.format("您%s的数量不足", moneyInfo.name))
	end
	local obj = user
	if data ~= "user" then
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	local list = GetResetableSkillListByjob(obj:GetJob())
	for k, v in ipairs(list) do
		if k < 4 then
			local id = v[math.random(1, #v)]
			local info = obj:GetSkill(k)
			obj:SetSkill(k, id, info and info.Lv or 1)
		end
	end
	USERBASE.clsUser[moneyInfo.set](user, -RESET_SKILL_COST)
	obj:FreshTotalAttr()				--刷新全部属性
	connection:SendProtocol("c_skill_resetGen", data)
	PROTOCOL.s_skill_list(connection, {roleId = connection.roleId, heroId = data.heroId})
end


local s_skill_resetSave = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user
	if data ~= "user" then
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	local list = obj:GetTempSkillList()
	for _, v in ipairs(list) do
		local info = obj:GetSkill(v.type)
		obj:SetSkill(v.type, v.skillId, info and info.Lv or 1)
	end
	obj:FreshTotalAttr()				--刷新全部属性
	connection:SendProtocol("c_skill_resetSave", data)
	local buff = {roleId = connection.roleId}
	if data ~= "user" then
		buff.heroId = data
	end
	PROTOCOL.s_skill_list(connection, buff)
end

local s_skill_resetPoint = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user
	if data ~= "user" then
		local hero = user:GetHero(data)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	if user:GetTotalIngot() < RESET_POINT_COST then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	user:SetTotalIngot(-RESET_POINT_COST)
	obj:ResetSkillPoint()
	connection:SendProtocol("c_skill_resetPoint", data)
	PROTOCOL.s_skill_list(connection, {roleId = connection.roleId, heroId = data})
end

local s_skill_upgrade = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user
	if data.heroId and data.heroId ~= "user" then
		local hero = user:GetHero(data.heroId)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	data.type = tostring(data.type)
	local info = obj:GetSkill(data.type)
	local skillInfo = BASESKILL.GetSkillInfo(info.id)
	if info.Lv >= #skillInfo.list then
		return ERROR.TipMsg(connection, "该技能已经达到最高等级。")
	end
	if data.skillLv ~= info.Lv then
		return ERROR.TipMsg(connection, "技能等级错误。")
	end
	if obj:GetSkillPoint() < 1 then
		return ERROR.TipMsg(connection, "技能点不足，无法完成升级。")
	end
	if obj:GetLv() < skillInfo.list[info.Lv + 1].needLv then
		return ERROR.TipMsg(connection, "等级不足，无法完成升级")
	end
	obj:SkillUpgrade(data.type)
	local sData = {heroId = data.heroId, type = data.type, skillLv = info.Lv, point = obj:GetSkillPoint()}
	sData.explain = skillInfo.list[info.Lv].explain
	if skillInfo.list[info.Lv + 1] then
		sData.next = skillInfo.list[info.Lv + 1].explain
	end
	connection:SendProtocol("c_skill_upgrade", sData)
end

--使用帝脉丹
local s_skill_addPoint = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local obj = user
	if data.heroId ~= "user" then
		local hero = user:GetHero(data.heroId)
		if not hero then
			return ERROR.TipMsg(connection, "没有找到该武将信息。")
		end
		obj = hero
	end
	local num = user:GetItemNum(USERBASE.PACKAGE, SKILL_POINT_PILL_ID)
	if num <= 0 then
		return ERROR.TipMsg(connection, string.format("您背包中的【%s】不足。", BASEITEM.GetItemName(SKILL_POINT_PILL_ID)))
	end
	user:DelItem(USERBASE.PACKAGE, SKILL_POINT_PILL_ID, 1)
	obj:SetSkillPoint(1)
	connection:SendProtocol("c_skill_addPoint", {heroId = data.heroId, point = obj:GetSkillPoint()})
end



function __init__()
	local SKILLLIST = Import("autocode/skillList.lua")
	AllSkillList = SKILLLIST.GetTable()
	
	local SKILLEFFECT = Import("autocode/skillEffect.lua")
	SkillEffectList = SKILLEFFECT.GetTable()
	
	local RESETLIST = Import("autocode/resetSkillList.lua")
	ResetableSkillList = RESETLIST.GetTable()
	
--	PROTOCOL.s_skill_resetList = s_skill_resetList		--可重置技能列表
	PROTOCOL.s_skill_resetGen = s_skill_resetGen		--生成新技能
--	PROTOCOL.s_skill_resetSave = s_skill_resetSave		--保存新技能
	
	PROTOCOL.s_skill_resetPoint = s_skill_resetPoint	--重置技能点
	PROTOCOL.s_skill_upgrade = s_skill_upgrade			--升级技能
	PROTOCOL.s_skill_addPoint = s_skill_addPoint		--使用帝脉丹
end

