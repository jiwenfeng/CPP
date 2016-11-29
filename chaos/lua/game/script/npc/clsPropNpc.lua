local menuList = {
	[1] = {type = SETTING.FUNC_PROP, name = "道具商店"},
}
------------------------------------------------------

clsPropNpc = CLSNPC.clsNpc:Inherit()

function clsPropNpc:__init__()
	Super(clsPropNpc).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsPropNpc:initsavedata()
	
end

function clsPropNpc:inittempdata()
	self.__temp.id = nil
	self.__temp.name = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
end


function clsPropNpc:OnTalk(user)
	MISSION.CheckMissionEvent(user, "dialog", self:GetId(), 1)
	local menu, msg = self:GetMenu(user, menuList)
	local index = self:SendMenu(user, msg, menu)
	local info = menu[index]
	if info and not info.missionId then
		if index == 1 then
			user:SendProtocol("c_npc_click", {content = "正在紧张开发中，敬请期待。。。"})
		end
	else
		self:ProMission(user, menu, index)
	end
end


return clsPropNpc