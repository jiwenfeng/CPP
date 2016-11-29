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

return clsMission
