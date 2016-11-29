--捐献类任务

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


function clsMission:IsFinished(user)
	for k, v in ipairs(self:GetTarget()) do
		if user:GetItemNum(USERBASE.PACKAGE, v.id) < v.needNum then
			return false
		end
	end
	return true
end

return clsMission

