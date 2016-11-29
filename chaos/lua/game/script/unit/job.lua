--职业类

local JobTypeList = {}

function GetJobInfoById(job)
	return JobTypeList[job]
end

function GetAllJobType()
	return JobTypeList
end

--------------------------------
local ModelHeadList = {}

function GetModelHeadListById(modelId)
	return ModelHeadList[modelId]
end

function GetAllModelHeadList()
	return ModelHeadList
end


-- type：basic 获取所有的基本职业 [job]获取[job]类型的所有职业
function GetJobTypeList(type)
	if not type then
		return JobTypeList		--返回所有职业
	end
	local sData = {}
	for k, v in pairs(JobTypeList) do
		if (type == "basic" and v.basic == k) or (type ~= "basic" and v.basic == type) then
			sData[k] = v
		end
	end
	return sData
end

function GetJobAttackType(job)
	return JobTypeList[job].type
end

function GetJobAttackRange(job)
	return JobTypeList[job].range
end

function GetBasicJob(job)
	return JobTypeList[job].basic
end

function GetJobModelList(job)
	return JobTypeList[job].modelList
end

function GetModelList(job, sex)
	return JobTypeList[job].modelList[sex]
end

--随机生成一个武将headId
function GenHeroHeadIdByModel(modelId)
	local list = GetModelHeadListById(modelId)
	return list[math.random(1, #list)]
end



function __init__()
	
	local JOBTYPE = Import("autocode/jobType.lua")
	JobTypeList = JOBTYPE.GetTable()
	
	local MODELHEAD = Import("autocode/modelHeadList.lua")
	ModelHeadList = MODELHEAD.GetTable()
end


