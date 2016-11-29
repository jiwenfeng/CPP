--场景怪物类

local RANDOM_MOVE_RANGE_X = 200		--随机移动的X轴距离
local RANDOM_MOVE_RANGE_Y = 120		--随机移动的Y轴距离

local RANDOM_MOVE_TIMELONG = 5		--随机移动时间间隔
local CORRECT_DISTANCE = 50			--修正距离

local PLOT_HAPPEN_DISTANCE = 600	--对话剧情发生距离
local GO_BACK_MAX_WIDTH = 300		--放弃追踪的x距离
------------------------------------------
clsFoe = CLSNPC.clsNpc:Inherit()

function clsFoe:__init__()
	Super(clsFoe).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsFoe:initsavedata()
end

function clsFoe:inittempdata()
	self.__temp.id = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
	
	self.__temp.callout = {}
	self.__temp.move = {}
end

function clsFoe:GetCallOut()
	return self.__temp.callout
end

function clsFoe:GetRoleType()
	return "foe"
end

function clsFoe:GetName()
	local info = FIGHT.GetMonsterTeamInfoById(self:GetId())
	return info.name
end

function clsFoe:GetType()
	local info = FIGHT.GetMonsterTeamInfoById(self:GetId())
	return info.isBoss and "boss" or "npc"
end


function clsFoe:IsDead()
	return self.__temp.dead
end

function clsFoe:SetDead(flag)
	self.__temp.dead = flag
end

function clsFoe:SetFightable(flag)
	self.__temp.fightable = flag
end

function clsFoe:IsFightable()
	return self.__temp.fightable
end

--目的地
function clsFoe:SetDestination(data)
	self.__temp.destination = data
end

function clsFoe:GetDestination()
	return self.__temp.destination
end


function clsFoe:OnDestroy()

end

function clsFoe:OnTalk(user)

end

function clsFoe:EnterScene(SceneObj, x, y)
	local curScene = self:GetScene()
	if curScene ~= SceneObj then
		local born = SceneObj:GetBornPoint()
		local posX, posY = x or born.x, y or born.y
		self:LeaveScene()
		SceneObj:Enter(self, posX, posY)
		self:SetInitPos(posX, posY)
		self:SetMoveState("rest")
	else
		if x and y then
			self:Goto(x, y)
		end
	end
end











--判断是否到达目的地
function clsFoe:IsArriveDest(posX, posY)
	local locate = self:GetLocateInfo()
	local dest = self:GetMoveDest()
	local destX, destY = posX or dest.x, posY or dest.y
	if math.sqrt((locate.x - destX) ^ 2 + (locate.y - destY) ^2) <= CORRECT_DISTANCE then
		return true
	end
	return false
end

function clsFoe:StartRandomMoveCallOut()
	self.__temp.ranCallId = CALLOUT.ObjCallOut(self:GetCallOut(), clsFoe.RandomMove, RANDOM_MOVE_TIMELONG, self)
end

function clsFoe:RemoveRandomMoveCallOut()
	if self.__temp.ranCallId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.ranCallId)
		self.__temp.ranCallId = nil
	end
end

function clsFoe:GetIdentity()
	return self.__temp.identity
end

function clsFoe:SetFollowTarget(roleId)
	self.__temp.follow = roleId
end

function clsFoe:GetFollowTarget()
	return self.__temp.follow
end


function clsFoe:UserMoving(target)
	if self:GetState() == "fighting" then
		return
	end
	local posX, posY = target:GetCurPos()
	local initPos = self:GetInitPos()
	if initPos.x - posX <= CORRECT_DISTANCE then
		return self:BeginFight(target)
	end
	if self:GetType() == "boss" then
		if (initPos.x - posX <= PLOT_HAPPEN_DISTANCE) and not self:IsTalked() then		--发生剧情对话
			local sceneObj = self:GetScene()
			if sceneObj and sceneObj:GetType() == "normal" then
				self:SetTalked()
				local copy = target:GetCurCopy()
			--	local info = BASECOPY.GetCopyInfoById(copy:GetId())
				local info = CLSCOPY.GetCopyInfoById(copy:GetId())
				local misObj = target:GetMissionById(info.needMissionId)
				if misObj and misObj:GetState() == CLSMISS.DOING then
					local missInfo = CLSMISS.GetMissionInfoById(info.needMissionId)
					if missInfo and missInfo.bossPlot then
						if missInfo.bossPlot.monsterId == self:GetId() then
							target:SendProtocol("c_copy_dialog", {type = "boss", dialog = missInfo.bossPlot.plotId})
						end
					end
				end
			end
		end
		return
	end
	if self:IsArriveDest(posX, posY) then
		return self:BeginFight(target)
	end
	if initPos.x - posX <= RANDOM_MOVE_RANGE_X then
		if self:GetMoveState() == "rest" then
			--移除随即移动定时器
			self:RemoveRandomMoveCallOut()
			self:RemoveMoveCallOut()
			--开始追踪
			self:SetMoveState("track")
			self:SetTrackTargetId(target:GetRoleId())
			self:BeginMove(posX, posY, "track")
		elseif self:GetMoveAction() == "run" then
			self:SetMoveDest(posX, posY)
		end
	elseif initPos.x - posX >= GO_BACK_MAX_WIDTH then
		--停止追踪
		if self:GetMoveState() == "track" then
			self:GoBack()
		end
	end
end

function clsFoe:GoBack()
	local initPos = self:GetInitPos()
	self:StopMove(true)
	self:SetMoveState("rest")
	self:SetTrackTargetId()
	--返回原点
	self:BeginMove(initPos.x, initPos.y)
end

function clsFoe:SetState(state)
	self.__temp.state = state
end

function clsFoe:GetState()
	return self.__temp.state
end


--随机移动
function clsFoe:RandomMove()
	if self:GetType() == "boss" then		--boss怪不移动
		return
	end
	local action = self:GetMoveAction()
	if action == "run" then
		return self:StartRandomMoveCallOut()
	end
	self:SetMoveInfo("rest")
	local initPos = self:GetInitPos()
	local posX = math.random(initPos.x - RANDOM_MOVE_RANGE_X, initPos.x + RANDOM_MOVE_RANGE_X)
	local posY = math.random(initPos.y - RANDOM_MOVE_RANGE_Y, initPos.y + RANDOM_MOVE_RANGE_Y)
	local scene = self:GetScene()
	if not scene then
		return
	end
	local dest = scene:AdjustPos(posX, posY)
	self:BeginMove(dest.x, dest.y)
end

function clsFoe:GetTrackTargetId()
	return self.__temp.trackId
end

function clsFoe:SetTrackTargetId(roleId)
	self.__temp.trackId = roleId
end


--停止后的操作
function clsFoe:OnMoveStop(flag)
	if flag then
		return
	end
	if self:GetMoveState() ~= "track" then
		self:StartRandomMoveCallOut()
	else
		local roleId = self:GetTrackTargetId()
		local user = assert(USER.GetOnlineUser(roleId))
		if user:GetScene() == self:GetScene() then
			self:BeginFight(user)
		end
	end
end


--处理战斗
function clsFoe:BeginFight(user)
	self:GoBack()
	local sceneObj = user:GetScene()
	local sceneType = sceneObj:GetType()
	if user:GetWorkState() == "fighting" then
		return
	end
	if sceneType == "normal" or sceneType == "hero" then
		local copy = user:GetCurCopy()
		assert(copy, string.format("Foe follow error: user = %s, sceneType = %s, mapId = %s.", user:GetName(), sceneObj:GetType(), sceneObj:GetMapId()))
		copy:ProFight(self:GetRoleId())
	elseif sceneType == "retreat" then
		local tribe = user:GetTribe()
		local retreat = tribe:GetStruct("retreat")
		retreat:BeginChallenge(self:GetRoleId())
	elseif sceneType == "uniRelic" then
		local union = UNION.GetUnionById(user:GetUnionId())
		if union then
			local uniRelic = union:GetUnionAct("uniRelic")
			if uniRelic:GetState() ~= CLSUNION.ACT_BEGIN then
				user:SendProtocol("c_show_tips", "战斗尚未开始。")
			else
				uniRelic:StartFight(user)
			end
		end
	elseif sceneType == "worldBoss" then
		local focus = user:GetFocus()
		if focus.type == "worldBoss" then
			local act = WORLDBOSS.GetWorldBossActById(focus.id)
			if act:GetState() ~= "begin" then
				user:SendProtocol("c_show_tips", "活动尚未开始。")
			else
				act:BeginFight(user)
			end
		end
	end
end

function clsFoe:OnEnterAOI(target)
	-- Target进入我的视野
	if target:GetRoleType() == "user" then
		self:StartRandomMoveCallOut()
	end
end

function clsFoe:OnLeftAOI(target)
	-- Target离开我的视野
	if target:GetRoleType() == "user" then
		self:RemoveRandomMoveCallOut()
	end
end


function clsFoe:IsTalked()
	return self.__temp.talked
end

function clsFoe:SetTalked()
	self.__temp.talked = true
end

return clsFoe
