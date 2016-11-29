--boss伤害排行榜

clsRank = BASERANK.clsRank:Inherit()

function clsRank:__init__()
	Super(clsRank).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsRank:GetSortFunc()
	return function (data1, data2)
			return  data1.damage > data2.damage
		end
end

function clsRank:AddToRank(data)
	local ret = self:IsDataExist(data)
	if ret then
		self:RmFromRank(ret)
	end
	table.binsert(self:GetRankList(), data, self:GetSortFunc())		--二分插入法
end

function clsRank:GetFormatRankData(rank, sum)
	local data = self:GetDataByRank(rank)
	local buff = {}
	buff.rank = rank
	buff.nick = USER.GetUserNickById(data.roleId)
	buff.damage = data.damage
	buff.roleId = data.roleId
	if sum then
		local rate = data.damage / sum * 100
		buff.rate = rate - rate % 0.01
	end
	return buff
end

function clsRank:GetTopRank(num, roleId, sum)
	local list = self:GetRankList()
	local sData = {list = {}}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData.list, self:GetFormatRankData(i, sum))
	end
	if roleId then
		local index = self:GetRankNo(roleId)
		local data = self:GetDataByRank(index)
		if data then
			sData.rank = index
			sData.damage = data.damage
			local rate = data.damage / sum * 100
			sData.rate = rate - rate % 0.01
		else
			sData.rank = -1
			sData.damage = 0
			sData.rate = 0
		end
	end
	return sData
end


return clsRank
