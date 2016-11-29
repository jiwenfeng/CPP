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

function clsMission:InitSaveData(missionId, template, quality, prizeList)
	self:SetMissionId(missionId)
	local mis
	if not template then
		mis = CLSMISS.GetMissionInfoById(missionId)
	else
		mis = CLSMISS.GetMissionInfoById(template)
	end
	if not mis then
		return
	end
	self:SetState(CLSMISS.FINISHED)
	self:SetTemplate(template)
	self:SetQuality(quality)
	self:InitMissionDetail()
	if prizeList then
		self:SetPrize(prizeList)
	end
	self:SetTimeStamp(os.time())
end

return clsMission

