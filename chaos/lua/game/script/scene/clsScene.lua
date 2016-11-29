--场景类

local INIT_KEEP_TIME = 60			--场景保留最小时间(防止自释放场景在创建后立即被释放)
-----------------------------------------------------
clsScene = clsObject:Inherit()

function clsScene:__init__()
	Super(clsScene).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsScene:initsavedata()	
end

function clsScene:inittempdata()
	self.__temp.id = 0
	self.__temp.callout = {}
	self.__temp.roleList = {}
end

function clsScene:InitScene(sceneId, type, mapId, sceneData, autoRelease)

end

function clsScene:InitNpcList(flag)
	local info = self:GetSceneData()
	
	setmetatable(self.__temp.roleList, {__mode = "k"})			--设置 roleList 对 roleObj的引用为弱引用
	for _, v in ipairs(info.npcList) do
		if flag or v.init then
			local npcObj = NPC.NewNpc(v.npcId)
			npcObj:EnterScene(self, v.x, v.y)
		end
	end
end

--场景动态id
function clsScene:SetId(data)
	self.__temp.id = data
end

function clsScene:GetId()
	return self.__temp.id
end

--场景静态id
function clsScene:SetTemplate(template)
	self.__temp.template = template
end

function clsScene:GetTemplate()
	return self.__temp.template
end
--[[场景地图编号
function clsScene:SetMapId(mapId)
	self.__temp.mapId = mapId
end]]

function clsScene:GetMapId()
	local sceneData = self:GetSceneData()
	return sceneData.mapId
end

function clsScene:SetTransPos(type, x, y)
	self.__temp.trans = {type = type, posX = x, posY = y}
end

function clsScene:GetTransPos()
	return self.__temp.trans
end

function clsScene:SetSceneData(data)
	self.__temp.sceneData = data
end

function clsScene:GetSceneData()
	return self.__temp.sceneData
end
--[[
function clsScene:SetName(data)
	self.__temp.name = data
end]]

function clsScene:GetName()
	local sceneData = self:GetSceneData()
	return sceneData.name
end

function clsScene:SetType(data)
	self.__temp.type = data
end

function clsScene:GetType()
	return self.__temp.type
end

function clsScene:GetKind()
	local sceneData = self:GetSceneData()
	return sceneData.kind
end
--[[
function clsScene:SetKind(kind)
	self.__temp.kind = kind
end
]]
--[[出生点
function clsScene:SetBornPoint(data)
	assert(type(data) == "table")
	self.__temp.born = {}
	self.__temp.born.x = data.x
	self.__temp.born.y = data.y
end]]

function clsScene:GetBornPoint()
	local sceneData = self:GetSceneData()
	return sceneData.born
end
--场景范围
function clsScene:SetSceneRange(x, y)
	self.__temp.range = {x = x, y = y}
end

function clsScene:GetSceneRange()
	return self.__temp.range
end

function clsScene:IsValidPos(x,y)
	local range = self:GetSceneRange()
	if x < 0 or y < 0 or x > range.x or y > range.y then
		return false
	end
	return true
end

function clsScene:GetMovableRange()
	local sceneData = self:GetSceneData()
	local range = self:GetSceneRange()
	local buff = {}
	buff.x = sceneData.validX or 0
	buff.y = sceneData.validY or range.y
	return buff
end

--调整位置
function clsScene:AdjustPos(x, y)
	local range = self:GetSceneRange()
	local size = self:GetMovableRange()
	local function adjust_pos(type, pos)
		if pos < size[type] then
			pos = size[type] + 10
		elseif pos >= range[type] then
			pos = size[type] - 10
		end
		return pos
	end
	local ret = {}
	ret.x = adjust_pos("x", x)
	ret.y = adjust_pos("y", y)
	return ret
end

function clsScene:SetInitState()
	self.__temp.init = os.time() + INIT_KEEP_TIME
end

function clsScene:GetInitState()
	return self.__temp.init > os.time()
end


function clsScene:GetSceneRole(roleId)
	return self.__temp.roleList[roleId]
end

function clsScene:GetSceneRoleList()
	return self.__temp.roleList
end

function clsScene:GetSceneRoleState(roleId)

end

--通过npcId查询npc
function clsScene:GetNpcRoleByNpcId(npcId)
	for k, v in pairs(self:GetSceneRoleList()) do
		if v:GetId() == npcId then
			return v
		end
	end
end

function clsScene:IsNoneUserExist()
	for k, v in pairs(self.__temp.roleList) do
		if v:GetRoleType() == "user" then
			return false
		end
	end
	return true
end

--是否自动释放，返回true将会在reset时
--检查场景内是否有玩家，没有就会释放场景对象
function clsScene:SetAutoRelease(Value)
	self.__temp.autoRelease = Value
end

function clsScene:IsAutoRelease()
	return self.__temp.autoRelease
end


function clsScene:GetFormatedRoleList(user, x, y)
	local roleId = user:GetRoleId()
	local list = lengine.scene.get_neighbor(roleId, self:GetId(), x, y, user:GetAOI())
	table.insert(list, roleId)
	local sData = {}
	for k, v in pairs(list) do
		local role = self:GetSceneRole(v)
		if role then
			local type = role:GetRoleType()
			if type ~= "user" or v == roleId or user:IsUserInSight(v) then
				local buff = {}
				buff.roleType = type
				buff.title = role:GetCurTitle()
				buff.posX, buff.posY = role:GetCurPos()
				buff.roleId = role:GetRoleId()
				buff.picId = role:GetModelId()
				buff.nick = role:GetName()
				buff.headId = role:GetHeadId()
				if type == "user" then
					buff.Lv = role:GetLv()
					buff.action = role:GetPose()
					buff.phyle = role:GetPhyle()
					buff.unionName = role:GetUnionName()
					local pet = role:GetPet()
					buff.petModelId = pet:GetModelId()
					buff.state = self:GetSceneRoleState(role:GetRoleId())
				else
					if type == "foe" and role:GetType() == "boss" then
						buff.roleType = "boss"
					end
					buff.state, buff.isMaster = role:GetState(user)
				end
				table.insert(sData, buff)
			end
		end
	end
	return sData
end

function clsScene:Enter(Object, x, y)
	local posX, posY = x, y
	if not posX or not posY then
		if Object:GetRoleType() == "user" then
			posX, posY = Object:GetEnterLocate(self)
		else
			local position = self:GetBornPoint()
			posX, posY = position.x, position.y
		end
	end
	Object:SetLocateInfo(self:GetTemplate(), posX, posY)
	self:AddRole(Object, posX, posY)
	Object:SetScene(self)
end

function clsScene:Leave(Object)
	self:DelRole(Object)
	Object:SetScene(nil)
end

function clsScene:AddRole(obj, x, y)
	local roleId = obj:GetRoleId()
	lengine.scene.add_object(self:GetId(), roleId, x, y, obj:GetAOI())	--添加
	self.__temp.roleList[roleId] = obj
	DATA.log(string.format("%s add obj :\t%s (%s)\t%d\t%d", self:GetId(), obj:GetName(), obj:GetRoleType(), x, y), "scene.log")
end

function clsScene:DelRole(obj)
	local locate = obj:GetLocateInfo()
	local roleId = obj:GetRoleId()
	lengine.scene.remove_object(self:GetId(), roleId, locate.x, locate.y, obj:GetAOI())	--删除
	self.__temp.roleList[roleId] = nil
	DATA.log(string.format("%s del obj :\t%s (%s)\t%d\t%d", self:GetId(), obj:GetName(), obj:GetRoleType(), locate.x, locate.y), "scene.log")
end

--刷新场景npc状态
function clsScene:RefreshSceneNpcState(user)
	local roleId = user:GetRoleId()
	for k, v in pairs(user:GetNeighbor()) do
		local role = self:GetSceneRole(v)
		if role:GetRoleType() == "npc" then
			self:SendSceneUpdate(roleId, v, "npcState")
		end
	end
end

--发送场景信息更新
function clsScene:SendSceneUpdate(tRoleId, roleId, action, curX, curY, oldX, oldY, speedInfo)
	local role = self:GetSceneRole(roleId)
	local roleType = role:GetRoleType()
	local buff = {roleId = roleId}
--	buff.id = STATS.GenId("sceneTest")		--场景测试用
--	buff.nick = role:GetName()
	buff.action = action
	if action == "title" then
		buff.title = role:GetCurTitle() or ""
	elseif action == "union" then
		buff.unionName = role:GetUnionName()
		buff.action = "title"
	elseif action == "phyle" then
		buff.phyle = role:GetPhyle() or ""
		buff.action = "title"
	elseif action == "rename" then
		buff.nick = role:GetName()
		buff.action = "title"
	elseif action == "pet" then
		local pet = role:GetPet()
		buff.petModelId = pet:GetModelId()
		buff.petHeadId = pet:GetHeadId(pet:GetForm())
		buff.action = "title"
	elseif action == "npcState" and roleType == "npc" then
		buff.state = role:GetState(self:GetSceneRole(tRoleId))
	else
		buff.roleType = roleType
		buff.posX = curX
		buff.posY = curY
		buff.oldX = oldX
		buff.oldY = oldY
		buff.speedX = speedInfo.x
		buff.speedY = speedInfo.y
		buff.state = self:GetSceneRoleState(role:GetRoleId())
	end
	local con = LOGIN.GetConnection(tRoleId)
	if con and con.roleId == tRoleId then
		con:SendProtocol("c_scene_update", buff)
	end
end

--隐藏其他玩家
function clsScene:HideOther(user, choice)
	user:SetHideOther(choice > 0 and choice)
	for _, v in pairs(user:GetNeighbor()) do
		local obj = self:GetSceneRole(v)
		if obj:GetRoleType() == "user" then
			if choice == 1 then
				user:OnLeftAOI(obj)
			else
				user:OnEnterAOI(obj)
			end
		end
	end
end

function clsScene:GetSceneInfo(user)
	local info = user:GetLocateInfo()
	local sData = {}
	sData.sceneId = self:GetId()
	sData.type = self:GetType()
	sData.mapId = self:GetMapId()
	sData.name = self:GetName()
	sData.posX, sData.posY = info.x, info.y
	sData.transInfo = self:GetTransPos()
	local validRange = self:GetMovableRange()
	sData.rangeY = validRange.y
	return sData
end

