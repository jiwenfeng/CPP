--联盟秘境npc：玄冥


clsUniDeity = CLSNPC.clsNpc:Inherit()

function clsUniDeity:__init__()
	Super(clsUniDeity).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsUniDeity:initsavedata()	
end

function clsUniDeity:inittempdata()
	self.__temp.id = nil
	self.__temp.name = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
end


function clsUniDeity:OnTalk(user)
	local roleId = user:GetRoleId()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		PROTOCOL.s_uniSecret_sacrifice(con, "set")
	end
end


return clsUniDeity