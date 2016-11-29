--Npc类

local AllNpcInfoList = {}		--所有npc静态数据列表

function GetAllNpcList()
	return AllNpcInfoList
end

function GetNpcInfoById(id)
	return AllNpcInfoList[id]
end
-------------------------------------------------------------
local AllNpcObjList = {}		--所有npc对象列表
local npcIndex = 0

function SetNpcObjById(roleId, obj)
	AllNpcObjList[roleId] = obj
end

function GetNpcObjById(roleId)
	return AllNpcObjList[roleId]
end

function GetAllNpcObjList()
	return AllNpcObjList
end

--获取npcId
function GetNextNpcObjIndex()
	npcIndex = npcIndex + 1
	return string.format("%d%04d%s", npcIndex, SERVER_ID, NPC_ID_ENDING)
end

--通过npc的模版id（静态id）查询npc对象
function GetNpcObjByTemplate(id)
	for _, v in pairs(GetAllNpcObjList()) do
		if v:GetId() == id then
			return v
		end
	end
end

---------------------------------------------------------------
local NpcClassList = {
	["prop"] = require "npc/clsPropNpc",		--道具商店类
	["equip"] = require "npc/clsEquipNpc",	--装备商店类
	["bourse"] = require "npc/clsBourseNpc",	--交易行类
	["tavern"] = require "npc/clsTavernNpc",	--酒馆
	["altar"] = require "npc/clsAltarNpc",		--祭坛
--	["official"] = require "npc/clsOfficialNpc",	--官职查看
	["phyDevote"] = require "npc/clsPhyDevote",	--首都建设
	["phyExchg"] = require "npc/clsPhyExchg",	--军功兑换
	
	["uniDeity"] = require "npc/clsUniDeity",	--联盟秘境npc-玄冥
	["uniBoss"] = require "npc/clsUniBoss",	--联盟遗迹Boss-饕餮
	["admin"] = require "npc/clsAdmin",		--测试admin类
	
	["feo"] = require "npc/clsFoe",		--怪物类
}

local NpcTitleList = {
	["equip"] = "391040", ["tavern"] = "391041", ["prop"] = "391042", ["bourse"] = "391043",
	["phyDevote"] = "391044", ["official"] = "391045", ["altar"] = "391046",
}

function NewNpc(id)
	local info = GetNpcInfoById(id)
	local type
	if not info then
		info = FIGHT.GetMonsterTeamInfoById(id)
		type = "feo"
	else
		type = info.type
	end
	local npc
	if NpcClassList[type] then
		local class = NpcClassList[type]
		npc = class:New()
	else
		npc = CLSNPC.clsNpc:New()
	end
	local roleId = GetNextNpcObjIndex()
	npc:InitNpc(roleId, id, info.mapId, info.posX, info.posY, info.headId, info.modelId)
	npc:SetCurTitle(NpcTitleList[type])
	SetNpcObjById(roleId, npc)
	return npc
end

function NpcLeave(npcObj)
	if npcObj:GetScene() then
		npcObj:LeaveScene()
		if npcObj:GetRoleType() == "foe" then
			npcObj:RemoveMoveCallOut()
		end
	end
	local roleId = npcObj:GetRoleId()
	SetNpcObjById(roleId)
	npcObj:Destroy()
end
----------------------------------------------------------------





function __init__()
	local NPCLIST = Import("autocode/sceneNpcList.lua")
	AllNpcInfoList = NPCLIST.GetTable()

end




