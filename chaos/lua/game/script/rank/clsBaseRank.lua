--排行榜基类

local path_rank = {real = "chaos.realRank", show = "chaos.showRank"}

function GetRankPath(label)
	return path_rank[label]
end
----------------------------------
clsRank = clsObject:Inherit()

function clsRank:__init__()
	Super(clsRank).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsRank:initsavedata()
	self.__save.list = {}
end

function clsRank:inittempdata()

end

--初始化排行榜
function clsRank:InitRank(label, type, needSave, upLimit)
	self:SetLabel(label)
	self:SetType(type)
	self:SetNeedSave(needSave)
	if upLimit then
		self:SetUplimit(upLimit)
	end
end

function clsRank:SetLabel(label)
	self.__save.label = label
end

function clsRank:GetLabel()
	return self.__save.label
end

--排行榜类型
function clsRank:SetType(type)
	self.__save.type = type
end

function clsRank:GetType()
	return self.__save.type
end

--排名上限
function clsRank:SetUplimit(data)
	self.__save.uplimit = data
end

function clsRank:GetUplimit()
	return self.__save.uplimit
end

--是否需要保存
function clsRank:SetNeedSave(flag)
	self.__save.needSave = flag
end

function clsRank:IsNeedSave()
	return self.__save.needSave
end

--排行榜数据
function clsRank:GetRankList()
	return self.__save.list
end

function clsRank:GetDataByRank(index)
	return self.__save.list[index]
end

function clsRank:RmFromRank(index)
	table.remove(self.__save.list, index)
	local buf = self:GetSaveData()
	buf._id = self:GetType()
	DATA.UpdateData(path_rank["real"], {_id = self:GetType()}, buf)
end

function clsRank:IsDataExist(data)
	for k, v in ipairs(self:GetRankList()) do
		if v.roleId == data.roleId then
			return k
		end
	end
	return false
end

--检查能否被添加到排行榜
function clsRank:CanBeAddToRank(data, sortFunc)
	local uplimit = self:GetUplimit()
	if not uplimit or #self:GetRankList() <= 0 then
		return true
	end
	local func = sortFunc or self:GetSortFunc()
	return func(data, self:GetDataByRank(#self:GetRankList()))
end

function clsRank:GetSortFunc()
	return function (data1, data2)
			return  data1.Lv > data2.Lv
		end
end

function clsRank:AddToRank(data, sortFunc)
	local ret = self:IsDataExist(data)
	if ret then
		self:RmFromRank(ret)
	end
	table.binsert(self:GetRankList(), data, sortFunc or self:GetSortFunc())		--二分插入法
	local uplimit = self:GetUplimit()
	if uplimit then
		local rankList = self:GetRankList()
		while (rankList[uplimit + 1]) do
			table.remove(rankList, uplimit + 1)
		end
	end
end

function clsRank:GetFormatRankData(index)
	return self:GetDataByRank(index)
end

--获取前num名
function clsRank:GetTopRank(num)
	local list = self:GetRankList()
	local sData = {}
	for i = 1, num do
		table.insert(sData, self:GetFormatRankData(i))
	end
	return sData
end

function clsRank:GetRankNo(roleId)
	return self:IsDataExist({roleId = roleId})
end

function clsRank:RmUserRank(roleId)
	for k, v in pairs(self:GetRankList()) do
		if v.roleId == roleId then
			self:RmFromRank(k)
			break
		end
	end
end

function clsRank:RmRankData(key, id)
	for k, v in pairs(self:GetRankList()) do
		if v[key] == id then
			self:RmFromRank(k)
			return
		end
	end
end

function clsRank:GetFormatRankList()
	return self:GetRankList()
end

function clsRank:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsRank:Restore(data)
	self.__save = data
end

function clsRank:UpdateRank(data)
	self:ClearTitle()			--刷新前清理旧的称号
	local buff = {}
	local limit = math.min(self:GetUplimit() or math.huge, #data)
	for i = 1, limit do
		buff[i] = data[i]
	end
	self.__save.list = buff
	self:UpdateTitle()			--刷新后添加新的称号
	local buf = self:GetSaveData()
	buf._id = self:GetType()
	DATA.UpdateData(path_rank["show"], {_id = self:GetType()}, buf)
end

function clsRank:GetRankData()

end

--删除称号
function clsRank:ClearTitle()
	local type = self:GetType()
	if not TITLE.IsValidTitleType(type) then
		return
	end
	local upLimit = self:GetUplimit()
	for k, v in ipairs(self:GetRankList()) do
		--[[if k > upLimit then
			break
		end]]
		TITLE.ClearTitleInfo(v.roleId, type, k)
	end
end

--更新称号
function clsRank:UpdateTitle()
	local type = self:GetType()
	if not TITLE.IsValidTitleType(type) then
		return
	end
	local upLimit = self:GetUplimit()
	for k, v in ipairs(self:GetRankList()) do
	--[[	if k > upLimit then
			break
		end]]
		TITLE.SetTitleInfo(v.roleId, type, k)
	end
end


function clsRank:GetMyRank(roleId)
	for k, v in ipairs(self:GetRankList()) do
		if v.roleId == roleId then
			return k
		end
	end
	return -1
end

function __init__()
end

