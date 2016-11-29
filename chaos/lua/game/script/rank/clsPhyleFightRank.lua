--势力战个人积分排行榜

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
			return  data1.score > data2.score
		end
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	local buff = {}
	buff.rank = rank
	buff.nick = USER.GetUserNickById(data.roleId)
	buff.roleId = data.roleId
	buff.score = data.score
	return buff
end

function clsRank:GetTopRank(num)
	local list = self:GetRankList()
	local sData = {}
	num = math.min(num, #list)
	for i = 1, num do
		table.insert(sData, self:GetFormatRankData(i))
	end
	return sData
end


return clsRank
