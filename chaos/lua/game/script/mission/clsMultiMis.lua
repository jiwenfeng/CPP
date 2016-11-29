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

--初始化任务进度
function clsMission:InitMissionDetail()
	local target = self:GetTarget()
	for k, v in ipairs(target) do
		table.insert(self.__save.detail, {type = v.type, id = v.id, needNum = v.num, curNum = 0, limit = v.limit})
	end
end

--修改任务完成进度
function clsMission:ModMissionDetail(user, id, num, limit)
	if self:GetState() ~= CLSMISS.DOING then
		return
	end	
	for k, v in ipairs(self.__save.detail or {}) do
		if v.id == id then
			if not v.limit or limit > v.limit then
				v.curNum = math.min(v.needNum, v.curNum + num)
				self:SendMissionDetailUpdate(user)
			end
		end
	end
end


return clsMission
