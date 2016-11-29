
clsRank = BASERANK.clsRank:Inherit()

function clsRank:inittempdata()
	self.__temp.callout = {}
end

function clsRank:GetCallOut()
	return self.__temp.callout
end

function clsRank:GetSortFunc()
	return  function(data1, data2)
		return data1.heroForce > data2.heroForce 
			or (data1.heroForce == data2.heroForce and data1.heroLv > data2.heroLv) 
	end
end

function clsRank:IsDataExist(data)
	for k, v in ipairs(self:GetRankList()) do
		if v.heroId == data.heroId and v.roleId == data.roleId then
			return k
		end
	end
end

function clsRank:IsMyHeroInRank(data)
	for k, v in ipairs(self:GetRankList()) do
		if v.roleId == data.roleId then
			return k
		end
	end
end

function clsRank:GetRankData(roleId)
	local user = assert(USER.GetOnlineUser(roleId))
	for k, v in pairs(user:GetHeroList()) do
		local data = {}
		data.roleId = roleId
		data.aptLv = v:GetAptLv()
		data.heroId = k
		data.heroLv = v:GetLv()
		data.heroForce = v:GetFightForce()
		data.heroName = v:GetName()
		self:AddToRank(data, self:GetSortFunc())
	end
	local showObj = RANK.GetRankObjByType("show", self:GetType())
	if showObj then
		showObj:UpdateRank(self:GetRankList())
	end
end

function clsRank:GetFormatRankData(rank)
	local data = self:GetDataByRank(rank)
	local buf = {}
	buf.nick = USER.GetUserNickInfo(data.roleId, "nick")
	buf.phyle = USER.GetUserNickInfo(data.roleId, "phyle")
	buf.rank = rank
	buf.vipLv = USER.GetUserNickInfo(data.roleId, "vipLv")
	buf.heroName = data.heroName
	buf.heroForce = data.heroForce
	buf.heroId = data.heroId
	buf.roleId = data.roleId
	buf.aptLv = data.aptLv
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
	local index = self:IsMyHeroInRank({roleId = roleId})
	sData.rank = index or -1
	if index then
		local info = self:GetDataByRank(index)
		sData.heroLv = info.heroLv
		sData.heroForce = info.heroForce
		sData.heroName = info.heroName
	end
	return sData
end


return clsRank

