--联盟遗迹boss：饕餮

clsUniBoss = CLSNPC.clsNpc:Inherit()

function clsUniBoss:__init__()
	Super(clsUniBoss).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsUniBoss:initsavedata()	
end

function clsUniBoss:inittempdata()
	self.__temp.id = nil
	self.__temp.name = nil
	self.__temp.locate = {scene = nil, x = nil, y = nil}
end

--设置boss可战斗
function clsUniBoss:SetFightable(data)
	self.__temp.fightable = data
end

function clsUniBoss:IsFightable()
	return self.__temp.fightable
end

function clsUniBoss:OnTalk(user)
	local roleId = user:GetRoleId()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		PROTOCOL.s_uniSecret_sacrifice(con, "set")
	end
end


return clsUniBoss