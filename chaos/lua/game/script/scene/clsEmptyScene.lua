--无人场景

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

function clsScene:InitScene(sceneId, type,template, sceneData, autoRelease)
	self:SetId(sceneId)
	self:SetTemplate(template)
	self:SetSceneData(sceneData)
	self:SetInitState()
	self:SetType(type)
	self:InitNpcList()
end

function clsScene:InitNpcList()
end

function clsScene:GetType()
	return "empty"
end


function clsScene:AddRole(obj, x, y)
end

function clsScene:DelRole(obj)
end


--发送场景信息更新
function clsScene:SendSceneUpdate(tRoleId, roleId, action, curX, curY, oldX, oldY, speedInfo)
end

function clsScene:GetSceneInfo(user)
	local info = user:GetLocateInfo()
	local sData = {}
	sData.sceneId = self:GetId()
	sData.type = self:GetKind()
	if sData.type == "copyMopup" then
		local mopup = user:GetCurMopup()
		if mopup then
			local copyInfo = CLSCOPY.GetCopyInfoById(mopup.copyId)
			sData.kind = copyInfo.type
		else
			sData.kind = "normal"
		end
	end
	return sData
end
