
clsRank = BASERANK.clsRank:Inherit()

function clsRank:GetSortFunc()
	return  function(data1, data2)
		return data1.unionForce > data2.unionForce
			or (data1.unionForce == data2.unionForce and data1.unionLv > data2.unionLv) 
			or (data1.unionForce == data2.unionForce and data1.unionLv == data2.unionLv and data1.createTime > data2.createTime)
	end
end

function clsRank:IsDataExist(data)
	for k, v in pairs(self:GetRankList()) do
		if v.unionId == data.unionId then
			return k
		end
	end
end

function clsRank:GetRankData(unionId)
	local union = UNION.GetUnionById(unionId)
	local data = {}
	data.unionId = unionId
	data.unionForce = union:GetTotalForce()
	data.unionLv = union:GetLv()
	data.createTime = union:GetCreateTime()
	data.name = union:GetName()
	data.phyle = union:GetPhyle()
	self:AddToRank(data, self:GetSortFunc())
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	local buf = {}
	local union = UNION.GetUnionById(data.unionId)
		buf.unionId = data.unionId
		buf.unionLv = data.unionLv
		buf.rank = rank
		buf.unionForce = data.unionForce
	if union then
		buf.name = union:GetName()
		buf.phyle = union:GetPhyle()
	else
		buf.name = data.name
		buf.phyle = data.phyle
	end
	return buf
end

function clsRank:GetTopRank(num, roleId)
	local list = self:GetRankList()
	local sData = {}
	sData.list = {}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData.list, self:GetFormatRankData(i, sum))
	end
	local user = assert(USER.GetOnlineUser(roleId))
	local unionId = user:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	local index = self:IsDataExist({unionId = unionId})
	sData.rank = index or -1
	if union and index then
		local info = self:GetDataByRank(index)
		sData.unionForce = info.unionForce
		sData.unionLv = info.unionLv
		sData.unionName = union:GetName()
	end
	return sData
end

return clsRank

