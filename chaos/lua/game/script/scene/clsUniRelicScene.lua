--联盟秘境类场景

---------------------------------

clsScene = CLSSCENE.clsScene:Inherit()

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

function clsScene:InitScene(sceneId, type, template, sceneData, autoRelease)
	self:SetId(sceneId)
	self:SetType(type)
	self:SetTemplate(template)
	self:SetSceneData(sceneData)
	self:SetSceneRange(sceneData.size.x, sceneData.size.y)
	if sceneData.transType then
		self:SetTransPos(sceneData.transType, sceneData.transX, sceneData.transY)
	end
	self:SetInitState()
	self:SetAutoRelease(autoRelease)
	lengine.scene.new_scene(sceneId, sceneData.size.x, sceneData.size.y)
--	self:InitNpcList()
end

function clsScene:InitNpcList(flag)
	local info = self:GetSceneData()
	
	setmetatable(self.__temp.roleList, {__mode = "k"})			--设置 roleList 对 roleObj的引用为弱引用
	for _, v in ipairs(info.npcList) do
		if flag or v.init then
			local npcObj = NPC.NewNpc(v.npcId)
			npcObj:EnterScene(self, v.x, v.y)
			if v.isBoss then
				self:SetKeyNpc(npcObj)
			end
			break
		end
	end
end

--关键npc
function clsScene:SetKeyNpc(obj)
	self.__temp.keyNpc = obj
end

function clsScene:GetKeyNpc()
	return self.__temp.keyNpc
end

function clsScene:GetSealBarState(unionId)
	local sceneInfo = self:GetSceneData()
	local sData = {}
	local union = UNION.GetUnionById(unionId)
	local uniRelic = union:GetUnionAct("uniRelic")
	if uniRelic then
		local state = uniRelic:GetState()
		sData.type = state == CLSUNION.ACT_BEGIN and "open" or "close"
	else
		sData.type = "close"
	end
	sData.posX, sData.posY = sceneInfo.sealBarX, sceneInfo.sealBarY
	return sData
end


function clsScene:GetSceneRoleState(roleId)
	local unionId = USER.GetUserNickInfo(roleId, "unionId")
	local union = UNION.GetUnionById(unionId)
	print("1", unionId, union)
	if union then
		local uniRelic = union:GetUnionAct("uniRelic")
		print("2", uniRelic)
		if uniRelic then
			return uniRelic:GetRoleState(roleId)
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
	sData.sealBar = self:GetSealBarState(user:GetUnionId())
	local validRange = self:GetMovableRange()
	sData.rangeY = validRange.y
	return sData
end