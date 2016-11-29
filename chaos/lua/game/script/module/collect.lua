--采集

local AllCollectionList = {}

function GetAllCollection()
	return AllCollectionList
end

function GetCollectionInfoById(id)
	return AllCollectionList[id]
end

-- [id] = {name = , timelong = , harvest = }

---------------------------------------------------

clsCollect = clsObject:Inherit()


function clsCollect:__init__()
	Super(clsCollect).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end


function clsCollect:initsavedata()
	self.__save.id = 0
end

function clsCollect:inittempdata()	
	self.__temp.list = {}
end

function clsCollect:InitCollect(id, owner)
	self:SetId(id)
	self:SetOwner(owner)
end

function clsCollect:SetId(id)
	self.__temp.id = id
end

function clsCollect:GetId()
	return self.__temp.id
end

function clsCollect:SetOwner(owner)
	self.__temp.owner = owner
end

function clsCollect:GetOwner()
	return self.__temp.owner
end

function clsCollect:GetNeedTime()
	local info = GetCollectionInfoById(self:GetId())
	return info.timelong
end

function clsCollect:GetName()
	local info = GetCollectionInfoById(self:GetId())
	return info.name
end

function clsCollect:GetHarvest()
	local info = GetCollectionInfoById(self:GetId())
	return info.harvest
end

--------------------------------------------------------------

function NewCollection(id, owner)
	local collect = clsCollect:New()
	collect:InitCollect(id, owner)
	return collect
end

---------------------------------------------------------------

local clsUser = USERBASE.clsUser

--开始采集
function clsUser:BeginCollect(id)
	local collect = NewCollection(id)
	self:SetCurCollect(collect)
	self:StartCollectCallOut(collect:GetNeedTime())
	return collect
end

function clsUser:SetCurCollect(obj)
	self.__temp.collect = obj
end

function clsUser:GetCurCollect()
	return self.__temp.collect
end

function clsUser:StartCollectCallOut(timelong)
	self.__temp.collectTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.FinishCollect, timelong, self)
end

function clsUser:RemoveCollectCallOut()
	if self.__temp.collectTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.collectTimer)
	end
end

--完成采集
function clsUser:FinishCollect()
	local collect = self:GetCurCollect()
	if not collect then
		return
	end
	self:RemoveCollectCallOut()
	local list = collect:GetHarvest()
	for k, v in pairs(list) do
		local item = BASEITEM.NewItem(v.id, v.num, {lock = 1, missionId = v.missionId})
		if item then
			self:AddItem(USERBASE.PACKAGE, item)
		end
		MISSION.CheckMissionEvent(self, "collect", v.id, v.num)
	end
	self:SendProtocol("c_collect_finish", collect:GetId())
	self:SetCurCollect()
	collect:Destroy()
end

--取消采集
function clsUser:ClearCollect()
	local collect = self:GetCurCollect()
	if not collect then
		return
	end
	self:RemoveCollectCallOut()
	self:SetCurCollect()
end




local s_collect_begin = function(connection, data)
	local info = GetCollectionInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的收集物。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local scene = user:GetScene()
	if not scene then
		return ERROR.TipMsg(connection, "您不在场景中，不能进行采集。")
	elseif scene:GetType() ~= "collect" then
		return ERROR.TipMsg(connection, "您尚未进入采集场。")
	end
	if user:GetCurCollect() then
		return ERROR.TipMsg(connection, "您正在进行采集。")
	end
	local collect = user:BeginCollect(data)
	connection:SendProtocol("c_collect_begin", {collectId = data, timelong = collect:GetNeedTime()})
end

local s_collect_cancel = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if not user:GetCurCollect() then
		return ERROR.TipMsg(connection, "您当前没有进行采集。")
	end
	user:ClearCollect()
	connection:SendProtocol("c_collect_cancel", "true")
end



function __init__()

	local COLLECTION = Import("autocode/collectionList.lua")
	AllCollectionList = COLLECTION.GetTable()
	
	PROTOCOL.s_collect_begin = s_collect_begin
	PROTOCOL.s_collect_cancel = s_collect_cancel
	
end



