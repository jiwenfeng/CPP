clsRank = BASERANK.clsRank:Inherit()


function clsRank:GetSortFunc()
	return  function(data1, data2)
		return data1.Lv > data2.Lv 
			or (data1.Lv == data2.Lv and data1.exp > data2.exp) 
			or (data1.Lv == data2.Lv and data1.exp == data2.exp and data1.vipLv > data2.vipLv)
	end
end

function clsRank:GetRankData(roleId)
	local data = {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		data.roleId = roleId
		data.Lv = user:GetLv()
		data.exp = user:GetExp()
		data.vipLv = user:GetVipLv()
		self:AddToRank(data, self:GetSortFunc())
	end
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	if data then
		local buf = {}
		buf.nick = USER.GetUserNickInfo(data.roleId, "nick")
		buf.lv = data.Lv
		buf.exp = data.exp
		buf.phyle = USER.GetUserNickInfo(data.roleId, "phyle")
		buf.job = USER.GetUserNickInfo(data.roleId, "job")
		buf.rank = rank
		buf.vipLv = data.vipLv
		return buf
	end
end

function clsRank:GetTopRank(num, roleId)
	local list = self:GetRankList()
	local sData = {}
	sData.list = {}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData.list, self:GetFormatRankData(i))
	end
	local index = self:IsDataExist({roleId=roleId})
	sData.rank = index or -1
	if index then
		local info = self:GetDataByRank(index)	
		sData.lv = info.Lv
		sData.exp = info.exp
	end
	return sData
end

return clsRank

