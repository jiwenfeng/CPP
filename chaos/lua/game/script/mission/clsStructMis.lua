--建筑类任务

clsMission = CLSMISS.clsmission:Inherit()

function clsMission:__init__()
	Super(clsMission).__init__(self)
	self.__save = {}
	self.__save.state = nil		--任务状态
	self.__save.missionId = nil
	self.__save.class = nil
	self.__save.detail = {}		--任务完成进度
	return true
end

--修改任务完成进度
function clsMission:ModMissionDetail(user, id, num)
	if self:GetState() ~= CLSMISS.DOING then
		return
	end	
	for k, v in ipairs(self.__save.detail or {}) do
		if v.id == id then
			v.curNum = num
			self:SendMissionDetailUpdate(user)
		end
	end
end


return clsMission


