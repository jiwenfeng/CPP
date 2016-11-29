--联盟秘境类

local MAX_DEITY_LV = 6					--最大等级
local MAX_SACRIFICE_TIMES = 50			--最大献祭次数
local DEITY_LAST_TIMELONG = 30 * 60		--秘境开启时长
local UPDATE_EXP_FREQUENCE = 15			--秘境中更新经验频率
local SECRET_MAP_ID = "697101"			--秘境地图id
local DEITY_NPC_ID = "210009"


--玄冥升级信息
local DeityInfoList = {}

function GetDeityInfoList()
	return DeityInfoList
end

function GetDeityInfoByLv(lv)
	return DeityInfoList[lv]
end

function GetMaxSacrificeTimes()
	return MAX_SACRIFICE_TIMES
end

function GetDeityLastTimelong()
	return DEITY_LAST_TIMELONG
end

--献祭次数加成经验
function GetSacrificeExpAddRate(times)
	return math.floor(times / 5) * 0.2
end

-------------------------------------------------------------
clsUniSecret = clsObject:Inherit()

CLSUNION.SetActClassById("uniSecret", clsUniSecret)

function clsUniSecret:__init__()
	Super(clsUniSecret).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsUniSecret:initsavedata()
	self.__save.deityLv = 1
	self.__save.summonNum = 0
end

function clsUniSecret:inittempdata()
	self.__temp.callout = {}
end

function clsUniSecret:InitUniAct(unionId)
	self:SetOwner(unionId)
	self:SetDeityLv(1)
	self:SetState(CLSUNION.ACT_OVER)
	
	local sceneObj = SCENE.NewScene("uniSecret", SECRET_MAP_ID)
	self:SetSecretScene(sceneObj)
end

function clsUniSecret:SetOwner(data)
	self.__save.owner = data
end

function clsUniSecret:GetOwner()
	return self.__save.owner
end

--秘境场景
function clsUniSecret:SetSecretScene(data)
	self.__temp.scene = data
end

function clsUniSecret:GetSecretScene()
	return self.__temp.scene
end

--玄冥等级
function clsUniSecret:SetDeityLv(data)
	self.__save.deityLv = (self.__save.deityLv or 0) + data
	if self.__save.deityLv >= MAX_DEITY_LV then
		self.__save.deityLv = MAX_DEITY_LV
	elseif self.__save.deityLv < 1 then
		self.__save.deityLv = 1
	end
end

function clsUniSecret:GetDeityLv()
	return self.__save.deityLv or 1
end
--基础经验加成
function clsUniSecret:GetBaseExpRate()
	local Lv = self:GetDeityLv()
	return GetDeityInfoByLv(Lv).expAdd
end

--玄冥召唤状态
function clsUniSecret:SetState(data)
	self.__save.state = data
end

function clsUniSecret:GetState()
	local state = self.__save.state
	if state == CLSUNION.ACT_BEGIN then
		return state
	end
	local timestamp = self:GetSummonTimestamp()
	if not timestamp or CalDiffDays(timestamp, os.time()) ~= 0 then
		return CLSUNION.ACT_UNSTART
	end
	return CLSUNION.ACT_OVER
end
--召唤次数
function clsUniSecret:GetUpgradeSummNum()
	return GetDeityInfoByLv(self:GetDeityLv()).needTimes
end

function clsUniSecret:GetSummonNum()
	return self.__save.summonNum or 0
end

function clsUniSecret:SetSummonNum(data)
	self.__save.summonNum = (self.__save.summonNum or 0) + data
	if self:GetDeityLv() >= MAX_DEITY_LV then
		self.__save.summonNum = self:GetUpgradeSummNum()
	else
		while (self.__save.summonNum >= self:GetUpgradeSummNum() and self:GetDeityLv() < MAX_DEITY_LV) do
			self:SetDeityLv(1)
		end
	end
end
--玄冥召唤时间
function clsUniSecret:SetSummonTimestamp(data)
	self.__save.summonTime = data
end

function clsUniSecret:GetSummonTimestamp()
	return self.__save.summonTime
end

--初始化召唤的临时信息
function clsUniSecret:InitSummonInfo()
	self.__temp.roleList = {}
	self.__temp.sacrificeTms = 0
end

function clsUniSecret:SummonDeity()
	self:SetState(CLSUNION.ACT_BEGIN)
	self:SetSummonNum(1)
	self:SetSummonTimestamp(os.time())
	self:InitSummonInfo()
	local sceneObj = self:GetSecretScene()
	
	sceneObj:InitNpcList(true)			--添加npc进入场景
	self:StartAddExpCallOut()
	self:StartActOverCallOut()
end


function clsUniSecret:StartAddExpCallOut()
	self.__temp.expTimer = CALLOUT.CallOutInRange(clsUniSecret.SecretAddExp, os.time(), os.time() + DEITY_LAST_TIMELONG, UPDATE_EXP_FREQUENCE, self)
end

function clsUniSecret:RemoveAddExpCallOut()
	if self.__temp.expTimer then
		CALLOUT.RemoveCallOut(getfenv(2), self.__temp.expTimer)
	end
end

function clsUniSecret:GetCallOut()
	return self.__temp.callout
end

function clsUniSecret:StartActOverCallOut()
	self.__temp.actTimer = CALLOUT.ObjCallOut(self:GetCallOut(), UNISECRET.EndSummon, DEITY_LAST_TIMELONG, self:GetOwner())
end


function clsUniSecret:RemoveActOverCallOut()
	if self.__temp.actTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.actTimer)
	end
end


function clsUniSecret:EndSummon()
	self:SetState(CLSUNION.ACT_OVER)
	local sceneObj = self:GetSecretScene()
	local npcObj = sceneObj:GetKeyNpc()
	if npcObj then
		NPC.NpcLeave(npcObj)
	end
	self:InitSummonInfo()
end


function clsUniSecret:InitRoleInfo(roleId)
	self.__temp.roleList = self.__temp.roleList or {}
	if not self.__temp.roleList[roleId] then
		self.__temp.roleList[roleId] = {}
	end
end

--获取玩家在秘境的信息
function clsUniSecret:GetRoleInfo(roleId)
	self.__temp.roleList[roleId] = self.__temp.roleList[roleId] or {}
	return self.__temp.roleList[roleId]
end
--献祭次数
function clsUniSecret:SetSacrificeTimes(data)
	self.__temp.sacrificeTms = (self.__temp.sacrificeTms or 0) + data
	if self.__temp.sacrificeTms > GetMaxSacrificeTimes() then
		self.__temp.sacrificeTms = GetMaxSacrificeTimes()
	elseif self.__temp.sacrificeTms < 0 then
		self.__temp.sacrificeTms = 0
	end
end

function clsUniSecret:GetSacrificeTimes()
	return self.__temp.sacrificeTms or 0
end
--玩家献祭
function clsUniSecret:ProSacrifice(roleId)
	local info = self:GetRoleInfo(roleId)
	info.sacrifice = 1
	self:SetSacrificeTimes(1)
end

function clsUniSecret:IsSacrificed(roleId)
	local info = self:GetRoleInfo(roleId)
	return info.sacrifice
end
--累计获得经验统计
function clsUniSecret:SetExpGot(roleId, exp)
	local info = self:GetRoleInfo(roleId)
	info.exp = (info.exp or 0) + exp
end

function clsUniSecret:GetExpGot(roleId, exp)
	local info = self:GetRoleInfo(roleId)
	return info.exp
end

--献祭增加额外的经验加成
function clsUniSecret:GetAdditionExpRete()
	local times = self:GetSacrificeTimes()
	return GetSacrificeExpAddRate(times)
end

--在场景中定时获得经验
function clsUniSecret:SecretAddExp()
	local baseRate = self:GetBaseExpRate()
	local extRate = self:GetAdditionExpRete()
	local sceneObj = self:GetSecretScene()
	if sceneObj then
		for k, v in pairs(sceneObj:GetSceneRoleList()) do
			if v:GetRoleType() == "user" then
				local exp = v:GetMeditateExp() * UPDATE_EXP_FREQUENCE / 60
				exp = exp * (1 + baseRate) * (1 + extRate)
				v:SetExp(exp)
			end
		end
	end
end

local updateFuncList = {
	state = function(uniSecret)
		local sData = {}
		sData.state = uniSecret:GetState()
		return sData
	end,
	sacrifice = function(uniSecret)
		local sData = {}
		sData.sacrifNum = uniSecret:GetSacrificeTimes()
		sData.extRate = uniSecret:GetAdditionExpRete()
		return sData
	end,
}


function clsUniSecret:SendUniSecretUpdate(type, list)
	local updateList = list
	if not updateList then
		updateList = {}
		local sceneObj = self:GetSecretScene()
		for roleId in pairs(sceneObj:GetSceneRoleList()) do
			table.insert(updateList, roleId)
		end
	end
	local sData = updateFuncList[type](self)
	for _, v in pairs(updateList) do
		local con = LOGIN.GetConnection(v)
		if con and con.roleId == v then
			con:SendProtocol("c_uniSecret_update", sData)
		end
	end
end





function clsUniSecret:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsUniSecret:Restore(data)
	self.__save = data
	if not self:GetSecretScene() then
		local sceneObj = SCENE.NewScene("uniSecret", SECRET_MAP_ID)
		self:SetSecretScene(sceneObj)
	end
end

function clsUniSecret:ShutDownProAct()
	local state = self:GetState()
	if state == CLSUNION.ACT_BEGIN then
		self:RemoveAddExpCallOut()
		self:RemoveActOverCallOut()
		UNISECRET.EndSummon(self:GetOwner())
	end
end

function clsUniSecret:Destroy()
	self:ShutDownProAct()
	local sceneObj = self:GetSecretScene()
--[[	for k, v in pairs(sceneObj:GetSceneRoleList()) do
		v:LeaveScene()
	end
	sceneObj:Destroy()]]
	sceneObj:SetAutoRelease(true)
	self:SetSecretScene()
	Super(clsUniSecret):Destroy()
end

--联盟开启秘境功能
function SetUniSecretOpen(union)
	local uniSecret = clsUniSecret:New()
	uniSecret:InitUniAct(union:GetId())
	union:SetUnionAct("uniSecret", uniSecret)
end

function __init__()
	local EXPLIST = Import("autocode/deityExp.lua")
	DeityInfoList = EXPLIST.GetTable()
end
