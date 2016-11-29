--城镇类场景

-------------------------------------------------------------
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

function clsScene:InitScene(sceneId, type, template, sceneData)
	self:SetId(sceneId)
--	self:SetName(sceneData.name)
	self:SetTemplate(template)
	self:SetSceneData(sceneData)
	self:SetType(type)
--	self:SetMapId(sceneData.mapId)
--	self:SetBornPoint(sceneData.born)
	self:SetSceneRange(sceneData.size.x, sceneData.size.y)
	if sceneData.transType then
		self:SetTransPos(sceneData.transType, sceneData.transX, sceneData.transY)
	end
	if sceneData.copyListId then
		self:SetCopyListId(sceneData.copyListId)
		self:SetCopyListName(sceneData.copyListName)
	end
--	self:SetKind(sceneData.kind)
	self:SetInitState()
	lengine.scene.new_scene(sceneId, sceneData.size.x, sceneData.size.y)
	self:InitNpcList()
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
	sData.cityId = self:GetTemplate()
	local validRange = self:GetMovableRange()
	sData.rangeY = validRange.y
	return sData
end

function clsScene:SetCopyListId(id)
	self.__temp.copyListId = id
end

function clsScene:GetCopyListId()
	return self.__temp.copyListId
end

function clsScene:SetCopyListName(name)
	self.__temp.copyListName = name
end

function clsScene:GetCopyListName()
	return self.__temp.copyListName
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
	if Object:GetRoleType() == "user" and self:GetTemplate() == "697004" then
		TITLE.CheckHonorTitleRecord(Object, "391102")
	end
end
