
clsRank = BASERANK.clsRank:Inherit()

function clsRank:GetSortFunc()
	return  function(data1, data2)
		return data1.force > data2.force 
			or (data1.force == data2.force and data1.vipLv > data2.vipLv) 
	end
end

function clsRank:GetRankData(roleId)
	local user = assert(USER.GetOnlineUser(roleId))
	local data = {}
	data.roleId = roleId
	data.vipLv = user:GetVipLv()
	data.force = user:GetFightForce()
	self:AddToRank(data, self:GetSortFunc())
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	if data then
		local buf = {}
		buf.nick = USER.GetUserNickInfo(data.roleId, "nick")
		buf.phyle = USER.GetUserNickInfo(data.roleId, "phyle")
		buf.job = USER.GetUserNickInfo(data.roleId, "job")
		buf.rank = rank
		buf.force = data.force
		buf.vipLv = data.vipLv
		buf.roleId = data.roleId
		return buf
	end
end

function clsRank:GetTopRank(num, roleId)
	local list = self:GetRankList()
	local sData = {}
	sData.list = {}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData.list, self:GetFormatRankData(i, sum))
	end
	local index = self:IsDataExist({roleId = roleId})
	sData.rank = index or -1
	if index then
		local info = self:GetDataByRank(index)
		sData.force = info.force
	end
	return sData
end

return clsRank

