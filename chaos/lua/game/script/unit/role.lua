--角色基类

local DEFALT_SIGHT_WIDTH = 675

local BASE_MOVE_TIME = 1
local NORMAL_MOVE_SPEED = 200
local FIGHT_MOVE_SPEED = 400

------------------------------
clsRole = clsObject:Inherit()

function clsRole:__init__()
	Super(clsRole).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsRole:initsavedata()
	self.__save.id = nil
	self.__save.name = nil
	self.__save.locate = {}
end

function clsRole:inittempdata()
	self.__save.roleId = nil
end

--角色唯一编号
function clsRole:GetId()
	return self.__save.id
end

function clsRole:SetId(data)
	self.__save.id = data
end

--角色在场景中的编号
function clsRole:GetRoleId()
	return self.__temp.roleId
end

function clsRole:SetRoleId(data)
	self.__temp.roleId = data
end

function clsRole:GetRoleType()
	return self.__save.role
end

function clsRole:SetRoleType(data)
	self.__save.role = data
end

function clsRole:SetLocateSence(data)
	self.__save.locate = self.__save.locate or {}
	self.__save.locate.scene = data
end

function clsRole:GetLocateSence()
	self.__save.locate = self.__save.locate or {}
	return self.__save.locate.scene
end

function clsRole:GetLocatePosX()
	self.__save.locate = self.__save.locate or {}
	return self.__save.locate.x
end

function clsRole:GetLocatePosY()
	self.__save.locate = self.__save.locate or {}
	return self.__save.locate.y
end



function clsRole:SetCurPos(x, y)
	self.__save.locate = self.__save.locate or {}
	self.__save.locate.x = x
	self.__save.locate.y = y
end

function clsRole:GetCurPos()
	local info = self.__save.locate
	return info.x, info.y
end

function clsRole:GetAOI()
	return DEFALT_SIGHT_WIDTH
end
--当前位置信息
function clsRole:SetLocateInfo(mapId, x, y)
	self.__save.locate = {scene = mapId, x = x, y = y}
end

function clsRole:GetLocateInfo()
	self.__save.locate = self.__save.locate or {}
	return self.__save.locate
end
--之前的位置信息
function clsRole:SetLastLocateInfo(mapId, x, y)
	self.__save.lastLocate = {scene = mapId, x = x, y = y}
end

function clsRole:GetLastLocateInfo()
	return self.__save.lastLocate
end
--计算角色进入点
function clsRole:GetEnterLocate(sceneObj)
	local posX, posY
	local scene = self:GetScene()
	if scene then
		if scene == sceneObj then
			local info = self:GetLocateInfo()
			posX, posY = info.x, info.y
		else
			self:LeaveScene()
		end
	end
	if not posX or not posY then
		local locate = self:GetLastLocateInfo()
		if locate and locate.scene == sceneObj:GetTemplate() then
			posX, posY = locate.x, locate.y
		else
			local born = sceneObj:GetBornPoint()
			posX, posY = born.x, born.y
		end
	end
	return posX, posY
end

function clsRole:SetScene(SceneObj)
	self.__temp.scene = SceneObj
end

function clsRole:GetScene()
   return self.__temp.scene
end
--角色姿势
function clsRole:GetPose()
	return self.__temp.pose or "stand"
end

function clsRole:SetPose(data)
	self.__temp.pose = data
end

--移动速度
function clsRole:GetMoveSpeed(type)
	self.__temp.speed = self.__temp.speed or {}
	if type then
		return self.__temp.speed[type]
	end
	return self.__temp.speed
end

function clsRole:SetMoveSpeed(x, y)
	self.__temp.speed = self.__temp.speed or {}
	self.__temp.speed = {x = x, y = y}
end

function clsRole:SetMoveRunNum(data)
	self.__temp.runNum = data
end

function clsRole:GetMoveRunNum()
	return self.__temp.runNum or 0
end

function clsRole:OnEnterAOI(target)
	-- Target进入我的视野
	local scene = self:GetScene()
	if not scene then
		return
	end
end

function clsRole:OnLeftAOI(target)
	-- Target离开我的视野
	if self:GetRoleType() == "foe" then
		self:RemoveCallOut()
	end
	local scene = self:GetScene()
	if not scene then
		return
	end
end

function clsRole:IsPlayer()
	return self:GetRoleType() == "user"
end


function clsRole:GetNeighbor()
	local SceneObj = self:GetScene()
	if not SceneObj then
		return {}
	end
	local SceneId = SceneObj:GetId()
	local AOI = self:GetAOI()
	if not AOI or AOI <= 0 then
		return {}
	end
	local x, y = self:GetCurPos()
	local NeighborList = lengine.scene.get_neighbor(self:GetRoleId(), SceneId, x, y, AOI)
	return NeighborList
end

function clsRole:GetExNeighbor()
	local SceneObj = self:GetScene()
	if not SceneObj then
		return {}
	end
	local SceneId = SceneObj:GetId()
	local x, y = self:GetCurPos()
	local ExNeighborList = lengine.scene.get_ex_neighbor(self:GetRoleId(), SceneId, x, y)
	return ExNeighborList
end


function clsRole:EnterScene(SceneObj, x, y)
	local curScene = self:GetScene()
	if curScene ~= SceneObj then
		local born = SceneObj:GetBornPoint()
		local posX, posY = x or born.x, y or born.y
		self:LeaveScene()
		SceneObj:Enter(self, posX, posY)
		self:SetPose("stand")
		self:InitUserInSight()
	else
		if x and y then
			self:Goto(x, y)
		end
	end
end

function clsRole:LeaveScene()
	local scene = self:GetScene()
	if scene then
		if scene:GetType() == "town" then
			local info = self:GetLocateInfo()
			self:SetLastLocateInfo(info.scene, info.x, info.y)
		end
		scene:Leave(self)
	end
	self:SetSceneEnterDone()
end

--客户端发起的角色移动
function clsRole:Move(x, y, action, spX, spY)
--function clsRole:Move(x, y, action, runNum)
	local SceneObj = self:GetScene()
	if not SceneObj then
		return false
	end
	if not SceneObj:IsValidPos(x, y) then
		return false
	end
	local src_x, src_y = self:GetCurPos()
	self:SetPose(action)
	self:SetMoveSpeed(spX, spY)
--	self:SetMoveRunNum(runNum)
	lengine.scene.move(SceneObj:GetId(), self:GetRoleId(), src_x, src_y, x, y, self:GetAOI())
	self:SetCurPos(x, y)
	self:Broad2ExNeighbor(action, src_x, src_y)
	return true
end

--向周围人更新自己的位置
function clsRole:Broad2ExNeighbor(action, oldX, oldY, toSelf)
	local SceneObj = self:GetScene()
	if not SceneObj then
		return
	end
	local ExNeighborList = self:GetExNeighbor()
	if toSelf then
		table.insert(ExNeighborList, self:GetRoleId())
	end
	local speedInfo = self:GetMoveSpeed()
	local locate = self:GetLocateInfo()
	for k, v in pairs(ExNeighborList) do
		local role = SceneObj:GetSceneRole(v)
		if role and role:GetRoleType() == "foe" then
	--		if role:GetTarget() == self:GetRoleId() and (speedInfo.x ~= 0 or speedInfo.y ~= 0) then		--NPC看见自己并且自己已经达到目的地
			if speedInfo.x ~= 0 or speedInfo.y ~= 0 then		--NPC看见自己并且自己已经达到目的地
				role:UserMoving(self)
			end
		end
		if role and role:GetRoleType() == "user" then
			SceneObj:SendSceneUpdate(role:GetRoleId(), self:GetRoleId(), action, locate.x, locate.y, oldX, oldY, speedInfo)
		end
	end
end

--服务器发起的角色移动
function clsRole:Goto(x, y, SceneObj)
--[[	if self:IsPlayer() then
		self:SetSyncEndX(nil)
		self:SetSyncEndY(nil)
	end

	if self:SetPos( x, y ) then
		if self:IsPlayer() then
			self:SendProtocol("c_nobj_goto", self:GetRoleId(), self:Getx(), self:Gety())
		end
		self:Broad2Neighbor("c_nobj_goto", self:GetRoleId(), self:Getx(), self:Gety()) 
	end]]
	local SceneObj = SceneObj or self:GetScene()
	if not SceneObj then
		return
	end
--[[	local action = "stand"
	local oldX, oldY = self:GetCurPos()
	if self:IsPlayer() then		
		SceneObj:SendSceneUpdate(self:GetRoleId(), self:GetRoleId(), action, x, y, oldX, oldY, self:GetMoveSpeed())
	end
	lengine.scene.move(SceneObj:GetId(), self:GetRoleId(), oldX, oldY, x, y, self:GetAOI())
	self:SetCurPos(x, y)
	self:Broad2ExNeighbor(action, oldX, oldY)]]
	
--	self:LeaveScene()
--	self:EnterScene(SceneObj, x, y)
	
	local scene = self:GetScene()
	if scene and scene == SceneObj then			--同场景内瞬移
		local oldX, oldY = self:GetCurPos()
		self:SetCurPos(x, y)
		if self:IsPlayer() then		
			SceneObj:SendSceneUpdate(self:GetRoleId(), self:GetRoleId(), "goto", x, y, oldX, oldY, self:GetMoveSpeed())
		end
		lengine.scene.move(SceneObj:GetId(), self:GetRoleId(), oldX, oldY, x, y, self:GetAOI())
	else
		self:EnterScene(SceneObj, x, y)
	end
end

function clsRole:IsInTown()
	local scene = self:GetScene()
	if not scene or scene:GetType() ~= "town" then
		return false
	end
	return true
end

function clsRole:IsHideOther()
	return false
end

function clsRole:SetHideOther(data)
end

function clsRole:InitUserInSight()
end

function clsRole:IsUserInSight(roleId)
	return true
end

function clsRole:AddUserInSight(roleId)
end

function clsRole:DelUserInSight(roleId)
end

--
function clsRole:SetSceneEnterDone()
end

function clsRole:IsSceneEnterDone()
	return true
end

----------------------------------------------------------------
--服务器端发起移动

--设置移动信息		--action: run, stand
function clsRole:SetMoveInfo(action, timestamp)
	self.__temp.move = self.__temp.move or {}
	self.__temp.move.action = action
	self.__temp.move.timestamp = timestmap or os.time()
end

function clsRole:GetMoveInfo()
	return self.__temp.move
end

function clsRole:GetMoveAction()
	return self.__temp.move.action
end

function clsRole:GetMoveState()
	return self.__temp.move.state
end

function clsRole:SetMoveState(state)
	self.__temp.move.state = state
end
--移动处理时间戳
function clsRole:SetMoveTimestamp(time)
	self.__temp.move.timestamp = time or os.time()
end

function clsRole:GetMoveTimestamp()
	return self.__temp.move.timestamp
end
--移动目的地
function clsRole:SetMoveDest(posX, posY)
	self.__temp.move.dest = {x = posX, y = posY}
end

function clsRole:GetMoveDest()
	return self.__temp.move.dest
end

function clsRole:StartMoveCallOut(time)
	self.__temp.move.timerId = CALLOUT.ObjCallOut(self:GetCallOut(), clsRole.ProMoving, time or 1, self)
end

function clsRole:RemoveMoveCallOut()
	if self.__temp.move.timerId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.move.timerId)
		self.__temp.move.timerId = nil
	end
end

--计算移动速度
function clsRole:CalMoveSpeed(posX, posY, type)
	local locate = self:GetLocateInfo()
	local xLen, yLen = posX - locate.x, posY - locate.y
	if xLen == 0 and yLen == 0 then		--NPC和user在同一个点
		return 0
	end
	local baseSpeed = type ~= "track" and NORMAL_MOVE_SPEED or FIGHT_MOVE_SPEED
	local time = math.sqrt(math.pow(xLen, 2) + math.pow(yLen, 2)) / baseSpeed
	local speedX = xLen / time / 30
	local speedY = yLen / time / 30
	local spX = speedX > 0 and math.ceil(speedX) or math.floor(speedX)
	local spY = speedY > 0 and math.ceil(speedY) or math.floor(speedY)
	self:SetMoveSpeed(spX, spY)
	return time
end

--修正移动目的地
function clsRole:CorrectMoveDest(destX, destY, time)
	local sceneObj = self:GetScene()
	if not sceneObj then
		return
	end
	local size = sceneObj:GetSceneRange()
	local speed = self:GetMoveSpeed()
	local locate = self:GetLocateInfo()
	local div_last, recorrect
	local function _correct(type, pos)
		if pos == locate[type] or speed[type] == 0 then
			return locate[type]
		end
		local div = (pos - locate[type]) / speed[type]
		local div_c = math.ceil(div)
		if div + 0.5 > div_c then
			div = div_c
		else
			div = math.floor(div)
		end
		if locate[type] + div * speed[type] >= size[type] or locate[type] + div * speed[type] <= 0 then
			div = div - 1
		end
		if not div_last then
			div_last = div
		elseif div_last ~= div then
			div_last = math.min(div, div_last)
			div = div_last
			recorrect = true		--计算Y时发生div改变，所以需要重新计算X的值
		end
		return locate[type] + div * speed[type]
	end
	local newDestX = _correct("x", destX)
	local newDestY = _correct("y", destY)
	if recorrect then
		newDestX = _correct("x", destX)
	end
--	print("Correct", destX, newDestX, destY, newDestY, (newDestX - locate.x) / speed.x, (newDestY - locate.y) / speed.y)
	self:SetMoveDest(newDestX, newDestY)
end

--由服务器端发起的移动
function clsRole:MoveBySvr(x, y, action, spX, spY)
	local SceneObj = self:GetScene()
	if not SceneObj then
		return false
	end
	if not SceneObj:IsValidPos(x, y) then
		return false
	end
	local src_x, src_y = self:GetCurPos()
	self:SetPose(action)
	self:SetMoveSpeed(spX, spY)
	lengine.scene.move(SceneObj:GetId(), self:GetRoleId(), src_x, src_y, x, y, self:GetAOI())
	self:Broad2ExNeighborSvr(action, x, y, spX, spY)
	self:SetCurPos(x, y)
	self:Broad2ExNeighborSvr("stand", x, y, 0, 0)
	return true
end

function clsRole:Broad2ExNeighborSvr(action, newX, newY, spX, spY)
	local SceneObj = self:GetScene()
	if not SceneObj then
		return
	end
	local ExNeighborList = self:GetExNeighbor()
	local locate = self:GetLocateInfo()
	for k, v in pairs(ExNeighborList) do
		local role = SceneObj:GetSceneRole(v)
		if role and role:GetRoleType() == "user" then
			SceneObj:SendSceneUpdate(role:GetRoleId(), self:GetRoleId(), action, newX, newY, locate.x, locate.y, {x = spX, y = spY})
		end
	end
end

--开始移动
function clsRole:BeginMove(posX, posY, state)
	self:SetMoveInfo("run", os.time())
	local SceneObj = self:GetScene()
	if not SceneObj then
		return
	end
	local adjusted = SceneObj:AdjustPos(posX, posY)
	local time = self:CalMoveSpeed(adjusted.x, adjusted.y, state)
	self:CorrectMoveDest(posX, posY, time)
	
	local info = self:GetMoveInfo()	
	local speed = self:GetMoveSpeed()
	local destX, destY = info.dest.x, info.dest.y
	local timelong = time
	if time > BASE_MOVE_TIME then
		local locate = self:GetLocateInfo()
		destX = locate.x + speed.x * BASE_MOVE_TIME * 30
		destY = locate.y + speed.y * BASE_MOVE_TIME * 30
		timelong = BASE_MOVE_TIME
	end
	local locate = self:GetLocateInfo()
--	print("Correct2", destX, destY, (destX - locate.x) / speed.x, (destY - locate.y) / speed.y)
	self:MoveBySvr(destX, destY, info.action, speed.x, speed.y)
	self:StartMoveCallOut(timelong)
end

--判断是否到达目的地
function clsRole:IsArriveDest()
	local locate = self:GetLocateInfo()
	local dest = self:GetMoveDest()
	if locate.x == dest.x and locate.y == dest.y then
		return true
	end
	return false
end

--处理移动
function clsRole:ProMoving()
	self:RemoveMoveCallOut()
	
	local timelong = os.time() - self:GetMoveTimestamp()
	if timelong <= 0 then
		return
	end
	if self:IsArriveDest() then
		return self:StopMove()
	end
	local dest = self:GetMoveDest()
	self:BeginMove(dest.x, dest.y, self:GetMoveState())
end

function clsRole:StopMove(flag)
	self:RemoveMoveCallOut()
	self:SetMoveDest()
	self:SetMoveSpeed()
	self:OnMoveStop(flag)
	self:SetMoveInfo("stand", os.time())
end

function clsRole:OnMoveStop()
end

