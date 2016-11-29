--场景处理模块

local NextSceneId = 0
local SCENE_FIND_POS_CORRECT_Y = 50			--自动寻路时玩家 Y 坐标偏移
local UserActionList ={["stand"] = 1, ["zazen"] = 2, ["run"] = 1, ["start"] = 1}

local AllSceneObjList = {}			--场景数据表

local SceneInfoList = {}


function GetSceneInfoById(type, id)
	return SceneInfoList[type][id]
end

function GetSceneInfoList(type)
	return SceneInfoList[type]
end
--[[获取地图配置信息中的npc信息
function GetSceneNpcInfo(template, npcId)
	local mapInfo = GetSceneInfoById(template)
	for _, v in ipairs(mapInfo.npcList) do
		if v.npcId == npcId then
			return v
		end
	end
end]]

function GetNextSceneId()
	NextSceneId = NextSceneId + 1
	return NextSceneId
end
--[[即将删除
function GetSceneObjByMapId(template)
	for k, v in pairs(GetAllSceneObjList()) do
		if v:GetTemplate() == template and v:GetType() == "town" then
			return v
		end
	end
end
]]

function GetSceneObjById(id)
	return AllSceneObjList[id]
end

function GetAllSceneObjList()
	return AllSceneObjList
end
--场景类别列表
local SceneClassList = {
	["town"] = TOWNSCENE.clsScene,
	["normal"] = COPYSCENE.clsScene,
	["uniRelic"] = UNIRELICSCENE.clsScene,
	["uniSecret"] = UNISECRETSCENE.clsScene,
	["retreat"] = COPYSCENE.clsScene,
	["worldBoss"] = WORLDBOSSSCENE.clsScene,
	["empty"] = EMPTYSCENE.clsScene,
	["hero"] = HEROSCENE.clsScene,
	["collect"] = COLLECTSCENE.clsCollectScene,
}

--新建一个场景
function NewScene(type, template, autoRelease)	
	local sceneData = GetSceneInfoById(type, template)
	if not sceneData then
		return
	end
	local scene = SceneClassList[type]:New()
	local sceneId = GetNextSceneId()
	scene:InitScene(sceneId, type, template, sceneData, autoRelease)
	AllSceneObjList[sceneId] = scene
	return scene
end

--销毁场景
function DestroyScene(sceneObj)
	local sceneId = sceneObj:GetId()
	sceneObj:Destroy()
	AllSceneObjList[sceneId] = nil
end

--定期检查，销毁无人的副本场景
function ResetScene()
	for k, v in pairs(GetAllSceneObjList()) do
		if not v:GetInitState() and v:IsAutoRelease() then
			if v:IsNoneUserExist() then
				DestroyScene(v)
			end
		end
	end
end

function GetCharById(roleId)
	if IsUserId(roleId) then
		return USER.GetOnlineUser(roleId)
	else
		return NPC.GetNpcObjById(roleId)
	end
end

--根据城镇id获取城镇场景
function GetTownScene(template)
	for k, v in pairs(GetAllSceneObjList()) do
		if v:GetTemplate() == template and v:GetType() == "town" then
			return v
		end
	end
end

--从副本返回城镇
function ReturnToTown(user)
	local lastLocate = user:GetLastLocateInfo()
	local scene = user:GetScene()
	if scene then
		user:LeaveScene()
	end
	local town = GetTownScene(lastLocate.scene or USERBASE.INIT_SCENE)
	if town then
		town:Enter(user, lastLocate.x, lastLocate.y)
		local con = LOGIN.GetConnection(user:GetRoleId())
		if con and con.roleId == user:GetRoleId() then
			con:SendProtocol("c_scene_enter", {sceneId = town:GetId(), mapId = town:GetMapId(), name = town:GetName(), posX = lastLocate.x, posY = lastLocate.y, type = town:GetType(), transInfo = town:GetTransPos()})
		end
	end
end

--获取即将返回的城镇的 sceneId
function GetReturnedTownId(user)
	local lastLocate = user:GetLastLocateInfo()
	local town = GetTownScene(lastLocate.scene or USERBASE.INIT_SCENE)
	return town:GetId()
end

--获取空场景（kind: copyMopup 副本扫荡 retreatMopup 修仙塔扫荡 tribe 部落）
function GetEmptyScene(kind)
	if not kind then return end
	for k, v in pairs(GetAllSceneObjList()) do
		if v:GetType() == "empty" then
			if kind == v:GetKind() then
				return v
			end
		end
	end
end

function IsEmptyScene(townId)
	for k, v in pairs(GetAllSceneObjList()) do
		if v:GetType() == "empty" then
			if townId == v:GetTemplate() then
				return v
			end
		end
	end
end

function GetTownInfoByCopyId(copyId)
	print("copyId:", copyId)
	copyId = tonumber(copyId)
	for k, v in pairs(GetSceneInfoList("town")) do
		if v.minCopyId <= copyId and copyId <= v.maxCopyId then
			return v
		end
	end
end

-------------------------------------------------

--模块加载时初始化公用场景
function StartUpInitPublicScene()
	for k, v in pairs(GetSceneInfoList("empty")) do
		NewScene(v.type, k)
	end
	for k, v in pairs(GetSceneInfoList("town")) do
		NewScene(v.type, k)
	end
	for k, v in pairs(GetSceneInfoList("collect") or {}) do
		NewScene(v.type, k)
	end
end

function ShutDownDestroyScene()
	for k, v in pairs(GetAllSceneObjList()) do
	
		v:Destroy()
	end
end

function GetTownSceneByCopyId(copyId)
	local townId
	local id = tonumber(copyId)
	for k, v in pairs(GetSceneInfoList("town")) do
		if id >= v.minCopyId and id <= v.maxCopyId then
			townId = k
			break
		end
	end
	return GetTownScene(townId)
end

function FindMissionPosition(missionId, user)
	local misObj = user:GetMissionById(missionId)
	local state = misObj and misObj:GetState() or CLSMISS.UNSTARTED
	local template = missionId
	if misObj and misObj:GetType() == "daily" then
		template = misObj:GetTemplate()
	end
	local missInfo = CLSMISS.GetMissionInfoById(template)
	if not missInfo then
		return false, string.format("没有指定的任务信息：%s。", missionId)
	end
	local sData = {}
	if state == CLSMISS.UNSTARTED then
		if missInfo.startNpcId then
			return FindNpcPosition(missInfo.startNpcId)
		end
	elseif state == CLSMISS.FINISHED then
		if missInfo.endNpcId then
			return FindNpcPosition(missInfo.endNpcId)
		else
			return user:FinishMission(missionId)
		end
	elseif state == CLSMISS.DOING then
		if missInfo.needCopyId then
			local scene = GetTownSceneByCopyId(missInfo.needCopyId)
			if not scene then
				return false, string.format("没有指定的副本信息：%s。", missInfo.needCopyId)
			end
			local transInfo = scene:GetTransPos()
			sData.sceneId = scene:GetId()
			sData.posX = transInfo.posX
			sData.posY = transInfo.posY
			sData.type = transInfo.type
			sData.copyId = missInfo.needCopyId
		end
	end
	return sData
end

function FindNpcPosition(npcId)
	local npcObj = NPC.GetNpcObjByTemplate(npcId)
	if not npcObj then
		return false, string.format("没有指定的npc信息:%s。", npcId)
	end
	local scene = npcObj:GetScene()
	if not scene then
		return false, string.format("npc信息有误：%s。", npcId)
	end
	local sData = {}
	sData.sceneId = scene:GetId()
	local locate = npcObj:GetLocateInfo()
	sData.posX = locate.x
	sData.posY = locate.y + SCENE_FIND_POS_CORRECT_Y
	sData.roleId = npcObj:GetRoleId()
	return sData
end

--玩家离开场景
function QuitScene(scene, user)
	if not scene then
		return
	end
	local sceneId, msg
	local type = scene:GetType()
	if type == "normal" or type == "hero" then
		sceneId, msg = COPY.QuitCopy(user)
	elseif type == "uniRelic" then
		sceneId, msg = UNIRELIC.QuitUniRelic(user)
	elseif type == "uniSecret" then
		sceneId, msg = UNISECRET.QuitSecret(user)
	elseif type == "retreat" then
		sceneId, msg = RETREAT.QuitRetreat(user, scene)
	elseif type == "worldBoss" then
		sceneId = WORLDBOSS.QuitWorldBoss(user)
	elseif type == "empty" then
		if scene:GetKind() == "tribe" then
			sceneId = TRIBE.QuitTribeScene(user)
		else
			sceneId = GetReturnedTownId(user)
		end
	else
		user:LeaveScene()
		sceneId = scene:GetId()
	end
	return sceneId, msg
end

function GetTownIdByCopyListId(id)
	for k, v in pairs(GetSceneInfoList("town")) do
		if v.copyListId == id then
			return k
		end
	end
end
---------------------------------------------------------------------
local s_scene_enter = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	data = tonumber(data)
	local scene = GetSceneObjById(data)
	if not scene then
		return ERROR.TipMsg(connection, "没有指定的场景信息。")
	end
	local posX, posY
	local lastInfo = user:GetLastLocateInfo()
	if lastInfo and scene:GetTemplate() == lastInfo.scene then
		posX, posY = lastInfo.x, lastInfo.y
	end
	local curScene = user:GetScene()
	if curScene and curScene ~= scene then
		QuitScene(curScene, user)
	end
	if user:IsLoginInitDone() then		--转换场景时取消打坐状态
		if user:IsInMeditate() then
			user:CancelMeditate()
		end
	end
	user:EnterScene(scene, posX, posY)
	connection:SendProtocol("c_scene_enter", scene:GetSceneInfo(user))
end

local s_scene_quit = function(connection, data)
	data = tonumber(data)
	local scene = GetSceneObjById(data)
	if not scene then
		return ERROR.TipMsg(connection, "没有指定的场景信息。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if scene ~= user:GetScene() then
		return ERROR.TipMsg(connection, "您不在指定的场景中")
	end
	local sceneId, msg = QuitScene(scene, user)	
	if not sceneId then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_scene_quit", sceneId)
end

local s_scene_move = function(connection, data)
--	data.mapId = tonumber(data.mapId)
--	if not data.mapId or not data.curX or not data.curY or not data.newX or not data.newY or not data.action then
	if not data.curX or not data.curY or not data.newX or not data.newY or not data.action then
		return ERROR.TipMsg(connection, "请输入玩家动作及动作发生前后的坐标。")
	end
	if not UserActionList[data.action] then
		return ERROR.TipMsg(connection, "无效的动作类型。")
	end
--[[	if not data.spX or not data.spY then
		return ERROR.TipMsg(connection, "请输入玩家移动的坐标速度。")
	end]]
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "数据错误，您不在场景中。")
	end
	local info = user:GetLocateInfo()
	if data.curX ~= info.x or data.curY ~= info.y then
		return ERROR.TipMsg(connection, "玩家的当前位置数据有误。")
	end
	--data.runNum = tonumber(data.runNum)
	--if not data.runNum then
	--	return ERROR.TipMsg(connection, "请输入runNum")
	--end
--	local ret = user:Move(data.newX, data.newY, data.action, data.spX, data.spY)
	if not data.speedX or not data.speedY then
		return ERROR.TipMsg(connection, "请输入X方向和Y的速度")
	end
--	local ret = user:Move(data.newX, data.newY, data.action, data.runNum)
	local ret = user:Move(data.newX, data.newY, data.action, data.speedX, data.speedY)
	if not ret then
		return ERROR.TipMsg(connection, "移动出错，无效的移动坐标。")
	end
--	connection:SendProtocol("c_scene_move", {posX = data.newX, posY = data.newY, action = data.action, speedX = data.speedX, speedY = data.speedY})
end

local s_scene_userList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))	
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "没有找到指定的场景信息。")
	end
	local info = user:GetLocateInfo()
	local sData = scene:GetFormatedRoleList(user, info.x, info.y)
	connection:SendProtocol("c_scene_userList", sData)
	user:SetSceneEnterDone(true)				--设置玩家的视野初始化完毕
end

local s_npc_click = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "没有找到指定的场景信息。")
	end
	local npcObj = scene:GetSceneRole(data)
	if not npcObj then
		return ERROR.TipMsg(connection, "没有找到指定的npc信息。")
	end
	local function func()
		return npcObj:OnTalk(user)
	end
	local co = coroutine.create(func)
	return coroutine.resume(co)
end

local s_npc_select = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "没有找到指定的场景信息。")
	end
	local npcObj = scene:GetSceneRole(data.roleId)
	if not npcObj then
		return ERROR.TipMsg(connection, "没有找到指定的npc信息。")
	end
	data.index = tonumber(data.index)
	if not data.index then
		return ERROR.TipMsg(connection, "无效的选择。")
	end
	return npcObj:ProMenu(data.index, user)
end

local s_scene_hideOther = function(connection, data)
	data = tonumber(data)
	if data ~= 1 and data ~= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "您不在场景中。")
	end
	local state = user:IsHideOther() == 1 and 1 or 0
	if state == data then
		return ERROR.TipMsg(connection, string.format("你已经选择了%s其他玩家。", state == 1 and "隐藏" or "显示"))
	end
	scene:HideOther(user, data)
	connection:SendProtocol("c_scene_hideOther", data)
end

local s_scene_world = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
--	sData.treasure = TREASURE.GetTreasurePlaceList(user)
	sData.treasure = TREASURE.GetTreasurePlace()
	sData.openTown = {}
--	for k in pairs(user:GetOpenedTownList()) do
	for k, v in pairs(GetSceneInfoList("town")) do
--		local townInfo = GetSceneInfoById("town", k)
--		if townInfo then
			local sceneObj = GetTownScene(k)
			if sceneObj then
			--	table.insert(sData.openTown, {sceneId = sceneObj:GetId(), mapId = k, name = v.name, state = user:IsTownOpened(k) and 1 or 0})
				local buf = {}
				buf.sceneId = sceneObj:GetId()
				buf.mapId = k
				buf.name = v.name
				buf.state = user:IsTownOpened(k)
				if user:IsTownOpened(k) then
					buf.copyListId = sceneObj:GetCopyListId()
					buf.copyListName = sceneObj:GetCopyListName()
				end
				table.insert(sData.openTown, buf)
			end
--		end
	end
	local scene = user:GetScene()
	if scene and scene:GetType() == "town" then
		sData.locate = scene:GetId()
	else
		sData.locate = GetReturnedTownId(user)
	end
	USER.SetUserFocusOn("worldmap", connection.roleId, 1)
	user:SetFocus("worldmap", "worldmap")
	connection:SendProtocol("c_scene_world", sData)
end

local s_scene_leaveWorldMap = function(connection, data)
	if data ~= "leave" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	USER.SetUserFocusOn("worldmap", connection.roleId)
	user:SetFocus()
	connection:SendProtocol("c_scene_leaveWorldMap", data)
end

local s_scene_find = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if not data.missionId and not data.npcId then
		return ERROR.TipMsg(connection, "协议错误，无效的请求格式。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg
	if data.missionId then
		ret, msg = FindMissionPosition(data.missionId, user)
	elseif data.npcId then
		ret, msg = FindNpcPosition(data.npcId)
	end
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end	
	connection:SendProtocol("c_scene_find", ret.sceneId and ret or 0)
end

function __init__()
	local TOWNLIST = Import("autocode/townSceneList.lua")
	SceneInfoList["town"] = TOWNLIST.GetTable()
	
	local EMPTYLIST = Import("autocode/emptySceneList.lua")
	SceneInfoList["empty"] = EMPTYLIST.GetTable()
	
	local COPYLIST = Import("autocode/copySceneList.lua")
	SceneInfoList["normal"] = COPYLIST.GetTable()
	
	local HEROCOPY = Import("autocode/heroSceneList.lua")
	SceneInfoList["hero"] = HEROCOPY.GetTable()
	
	local RETREATCOPY = Import("autocode/retreatSceneList.lua")
	SceneInfoList["retreat"] = RETREATCOPY.GetTable()

	local UNISECRET = Import("autocode/uniSecretScene.lua")
	SceneInfoList["uniSecret"] = UNISECRET.GetTable()

	local UNIRELIC = Import("autocode/uniRelicScene.lua")
	SceneInfoList["uniRelic"] = UNIRELIC.GetTable()
	
	local WORLDBOSS = Import("autocode/worldBossScene.lua")
	SceneInfoList["worldBoss"] = WORLDBOSS.GetTable()

	StartUpInitPublicScene()
	CALLOUT.CallFre(ResetScene, 60)
	
	PROTOCOL.s_scene_enter = s_scene_enter
	PROTOCOL.s_scene_quit = s_scene_quit
	PROTOCOL.s_scene_move = s_scene_move
	PROTOCOL.s_scene_userList = s_scene_userList
	
	PROTOCOL.s_npc_click = s_npc_click
	PROTOCOL.s_npc_select = s_npc_select
	PROTOCOL.s_scene_leaveWorldMap = s_scene_leaveWorldMap 
	PROTOCOL.s_scene_world = s_scene_world
	
	PROTOCOL.s_scene_hideOther = s_scene_hideOther
	PROTOCOL.s_scene_find = s_scene_find
end
