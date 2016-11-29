--场景Npc类

local NPC_SIGHT_WIDTH = 250		--npc视野半径

-----------------------------
clsNpc = ROLE.clsRole:Inherit()

function clsNpc:__init__()
	Super(clsNpc).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsNpc:initsavedata()
end

function clsNpc:inittempdata()
	self.__temp.id = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
	self.__temp.move = {}
end

function clsNpc:SetHeadId(id)
	self.__temp.headId = id
end

function clsNpc:GetHeadId()
	return self.__temp.headId
end

function clsNpc:SetModelId(id)
	self.__temp.modelId = id
end

function clsNpc:GetModelId()
	return self.__temp.modelId
end

function clsNpc:SetInitPos(x, y)
	self.__temp.initPos = {x = x, y = y}
end

function clsNpc:GetInitPos()
	return self.__temp.initPos
end

function clsNpc:SetTarget(roleId)
	self.__temp.target = roleId
end

function clsNpc:GetTarget()
	return self.__temp.target
end

function clsNpc:InitNpc(roleId, id, townId, posX, posY, headId, modelId)
	self:SetRoleId(roleId)
	self:SetId(id)
	self:SetHeadId(headId)
	self:SetModelId(modelId)
	if townId then
		self:SetLocateInfo(townId, posX, posY)
	end
end

--Id
function clsNpc:SetId(data)
	self.__temp.id = data
end

function clsNpc:GetId()
	return self.__temp.id
end

function clsNpc:GetName()
	local info = NPC.GetNpcInfoById(self:GetId())
	return info.name
end

function clsNpc:GetType()
	local info = NPC.GetNpcInfoById(self:GetId())
	return info.type
end

function clsNpc:GetRoleType()
	return "npc"
end



function clsNpc:GetMenu(user, list, isSub)
	local menu = {}
	if list then
		for k, v in ipairs(list) do
			table.insert(menu, {index = k, type = v.type, name = v.name})
		end
	end
	local msg
	if not isSub then
		local missList = MISSION.GetMissionListByNpc(self:GetId(), user)
		for k, v in ipairs(missList) do
			if v.state == CLSMISS.FINISHED then
				local missInfo = CLSMISS.GetMissionInfoById(v.missionId)
				if missInfo then
					menu = {}
					table.insert(menu, {type = SETTING.FUNC_MISSION, missionId = v.missionId, missType = v.type, name = missInfo.endA, index = #menu + 1, state = v.state})
					msg = missInfo.endQ
					break
				end
			else
				table.insert(menu, {index = #menu + 1, type = SETTING.FUNC_MISSION, missionId = v.missionId, missType = v.type, name = v.name, state = v.state})
			end
		end
	end
	return menu, msg
end


function clsNpc:ProMission(user, menu, index)
	local info = menu[index]
	if info and info.type == SETTING.FUNC_MISSION then
		local missInfo = CLSMISS.GetMissionInfoById(info.missionId)
		if missInfo then
			local roleId = user:GetRoleId()
			if info.state == CLSMISS.FINISHED then
	--			local which = self:SendMenu(user, missInfo.endQ, {{type = SETTING.FUNC_MISSION, name = missInfo.endA, index = 1}})
	--			if which == 1 then
					local con = LOGIN.GetConnection(roleId)
					if con and con.roleId == roleId then
						PROTOCOL.s_mission_prize(con, info.missionId)
						local scene = user:GetScene()
						scene:RefreshSceneNpcState(user)
					end
	--			end
				local list = MISSION.GetMissionListByNpc(self:GetId(), user, true)
				if #list > 0 then
					local con = LOGIN.GetConnection(roleId)
					if con and con.roleId == roleId then
						PROTOCOL.s_npc_click(con, self:GetRoleId())
					end
				else
					user:SendProtocol("c_npc_shutUp", "任务完成")
				end
			elseif info.state == CLSMISS.DOING then
				local which = self:SendMenu(user, missInfo.doingQ, {{type = SETTING.FUNC_MISSION, name = missInfo.doingA, index = 1}})
				if which == 1 then
					local con = LOGIN.GetConnection(roleId)
					if con and con.roleId == roleId then
						PROTOCOL.s_scene_find(con, {missionId = info.missionId})
					end
				end
				user:SendProtocol("c_npc_shutUp", "任务进行中")
			elseif info.state == CLSMISS.UNSTARTED then
				local prize = CLSMISS.GetFormatedPrize(user, missInfo.prize)
				local which = self:SendMenu(user, missInfo.startQ, {{type = SETTING.FUNC_MISSION, name = missInfo.startA, index = 1, prize = prize}})
				if which == 1 then
					local con = LOGIN.GetConnection(roleId)
					if con and con.roleId == roleId then
						PROTOCOL.s_mission_accept(con, info.missionId)
						local scene = user:GetScene()
						scene:RefreshSceneNpcState(user)
						PROTOCOL.s_scene_find(con, {missionId = info.missionId})
					end
				end
				user:SendProtocol("c_npc_shutUp", "任务领取")
			end
		end
	else
		user:SendProtocol("c_npc_click", {content = "无效的对话索引。"})
	end
end


function clsNpc:GetState(user)
	local missList = MISSION.GetMissionListByNpc(self:GetId(), user)
	if #missList >= 1 then
		return missList[1].state
	end
end

function clsNpc:GetRandomDialog()
	local info = NPC.GetNpcInfoById(self:GetId())
	return info.dialog[math.random(1, #info.dialog)]
end

function clsNpc:OnTalk(user)
	MISSION.CheckMissionEvent(user, "dialog", self:GetId(), 1)
	local menu, msg = self:GetMenu(user)
	local index = self:SendMenu(user, msg, menu)
	if index then
		user:SetNpcCort()
		self:ProMission(user, menu, index)
	end
end

function clsNpc:SendMenu(user, msg, menu)
	local co = coroutine.running()
	user:SetNpcCort(co)
	local sData = {}
	sData.content = msg or self:GetRandomDialog()
	sData.menu = menu
	user:SendProtocol("c_npc_click", sData)
	return coroutine.yield(co)
end

function clsNpc:ProMenu(index, user)
	local co = user:GetNpcCort()
	if co and coroutine.status(co) == "suspended" then
		coroutine.resume(co, index)
		return
	end
end

function clsNpc:SetCurTitle(data)
	self.__temp.title = data
end

function clsNpc:GetCurTitle()
	return self.__temp.title
end

function clsNpc:OnDestroy()

end
