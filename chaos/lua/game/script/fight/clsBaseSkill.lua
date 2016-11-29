--技能基类

local AllSkillInfoList = {}

function GetAllSkillList()
	return AllSkillInfoList
end

function GetSkillInfo(skillId)
	return AllSkillInfoList[skillId]
end

local SkillEffectList = {}			--技能效果列表

function GetSkillEffectInfoById(effectId)
	return SkillEffectList[effectId]
end

--纵行攻击范围
local VertList = {
	["1"] = {"1", "2", "3"}, ["2"] = {"1", "2", "3"}, ["3"] = {"1", "2", "3"},
	["4"] = {"4", "5", "6"}, ["5"] = {"4", "5", "6"}, ["6"] = {"4", "5", "6"},
	["7"] = {"7", "8", "9"}, ["8"] = {"7", "8", "9"}, ["9"] = {"7", "8", "9"},
}
--横行攻击范围
local HorizList = {
	["1"] = {"1", "4", "7"}, ["2"] = {"2", "5", "8"}, ["3"] = {"3", "6", "9"},
	["4"] = {"1", "4", "7"}, ["5"] = {"2", "5", "8"}, ["6"] = {"3", "6", "9"},
	["7"] = {"1", "4", "7"}, ["8"] = {"2", "5", "8"}, ["9"] = {"3", "6", "9"},
}

--获取攻击范围
function GetAttackRangeByPos(type, pos)
	local list = {}
	if type == 1 or type == 4 or type == 7 then
		table.insert(list, pos)
	elseif type == 2 or type == 3 then
		for i = 1, 9 do
			table.insert(list, tostring(i))
		end
	elseif type == 5 then
		list = HorizList[pos]
	elseif type == 6 then
		list = VertList[pos]
	end
	return list
end
----------------------------------------------------------
clsSkill = clsObject:Inherit()

function clsSkill:__init__()
	Super(clsSkill).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end


function clsSkill:initsavedata()
	self.__save.id = 0
end

function clsSkill:inittempdata()	
	self.__temp.list = {}
end

function clsSkill:InitSkill(id, Lv)
	self:SetId(id)
	self:SetLv(Lv)
end

function clsSkill:SetId(id)
	self.__temp.id = id
end

function clsSkill:GetId(id)
	return self.__temp.id
end

function clsSkill:GetClass()

end

function clsSkill:SetLv(lv)
	self.__temp.Lv = lv
	local info = GetSkillInfo(self:GetId())
	if lv > #info.list then
		self.__temp.Lv = #info.list
	elseif lv < 1 then
		self.__temp.Lv = 1
	end
end

function clsSkill:GetLv()
	return self.__temp.Lv
end



function clsSkill:GetType()
	local info = GetSkillInfo(self:GetId())
	return info.type
end

function clsSkill:GetName()
	local info = GetSkillInfo(self:GetId())
	return info.name
end

function clsSkill:IsNonMiss()
	local info = GetSkillInfo(self:GetId())
	return info.nonMiss
end

function clsSkill:GetTarget()
	local info = GetSkillInfo(self:GetId())
	return info.target
end

function clsSkill:GetTrigger()
	local info = GetSkillInfo(self:GetId())
	return info.trigger
end

function clsSkill:GetRange()
	local info = GetSkillInfo(self:GetId())
	return info.range
end

function clsSkill:GetMethod()
	local info = GetSkillInfo(self:GetId())
	return info.method
end

function clsSkill:GetAtkType()
	local info = GetSkillInfo(self:GetId())
	return info.atkType
end

function clsSkill:GetAtkAction()
	local info = GetSkillInfo(self:GetId())
	return info.atkAction
end

function clsSkill:GetEffectLastTime()
	local info = GetSkillInfo(self:GetId())
	return info.lastTimes
end

function clsSkill:GetCdTime()
	local info = GetSkillInfo(self:GetId())
	return info.list[self:GetLv()].cdTime
end

function clsSkill:GetLvInfo()
	local info = GetSkillInfo(self:GetId())
	return info.list[self:GetLv()]
end



function clsSkill:GetAttackRange(pos)
	return GetAttackRangeByPos(self:GetRange(), pos)
end

--获取技能效果
function clsSkill:GetSkillEffect()

end



function __init__()
	
	local SKILLLIST = Import("autocode/skillList.lua")
	AllSkillInfoList = SKILLLIST.GetTable()
	
	local SKILLEFFECT = Import("autocode/skillEffect.lua")
	SkillEffectList = SKILLEFFECT.GetTable()
	
end
