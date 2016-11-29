
clsRank = BASERANK.clsRank:Inherit()

function clsRank:GetSortFunc()
	return  function(data1, data2)
		return data1.force > data2.force
	end
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	local buf = {}
	buf.roleId = data.roleId
	buf.nick = USER.GetUserNickInfo(data.roleId, "nick")
	buf.phyle = USER.GetUserNickInfo(data.roleId, "phyle")
	buf.vipLv = USER.GetUserNickInfo(data.roleId, "vipLv")
	buf.force = data.force
	buf.rank = rank
	buf.quality = data.quality
	buf.goodsId = data.goodsId
	return buf
end

function clsRank:GetTopRank(num, roleId)
	local list = self:GetRankList()
	local sData = {}
	sData.list = {}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData.list, self:GetFormatRankData(i))
	end
	local index = self:IsDataExist({roleId = roleId})
	sData.rank = index or -1
	if index then
		local info = self:GetDataByRank(index)
		sData.goodsId = info.goodsId
		sData.force = info.force
	end
	return sData
end

return clsRank
