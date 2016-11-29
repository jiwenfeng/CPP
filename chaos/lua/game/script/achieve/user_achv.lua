--玩家成就扩展

local clsUser = USERBASE.clsUser

local MAX_ACHIEVE_LV = 20

function clsUser:InitAchvList()
	self.__save.achv = {}
	self.__save.achv.list = {}
	self.__save.achv.point = 0
	self.__save.achv.Lv = 0
end

function clsUser:GetAchvList()
	if not self.__save.achv then return end
	return self.__save.achv.list
end

--成就项目
function clsUser:SetAchvById(id, state, time)
	if not self.__save.achv then return end
	self.__save.achv.list[id] = self.__save.achv.list[id] or {}
	local info = self.__save.achv.list[id]
	info.state = state
	if time then
		info.time = time
	else
		info.time = info.time or os.time()
	end
end

function clsUser:GetAchvById(id)
	if not self.__save.achv then return end
	return self.__save.achv.list[id]
end

function clsUser:ModAchvInfo(id, key, val)
	if not self.__save.achv then return end
	local info = self.__save.achv.list[id]
	if not info then return end
	info[key] = val
end

function clsUser:GetAchvState(id)
	if not self.__save.achv then return end
	if not self.__save.achv.list[id] then return end
	return self.__save.achv.list[id].state
end

function clsUser:IsAchvGot(id)
	if not self.__save.achv then return false end
	if not self.__save.achv.list[id] then return false end
	return true
end

--成就点
function clsUser:SetAchvPoint(point)
	if not self.__save.achv then return end
	self.__save.achv.point = (self.__save.achv.point or 0) + point
	self:CheckBadgeUpgrade()
end

function clsUser:GetAchvPoint()
	if not self.__save.achv then return 0 end
	return self.__save.achv.point or 0
end

function clsUser:CheckBadgeUpgrade()
	local point = self:GetAchvPoint()
	local info = ACHV.GetBadgeInfoByLv(self:GetBadgeLv())
	if point >= info.needPoint then
		self.__save.achv.Lv = ACHV.GetBadgeLvByPoint(point)
	end
end

function clsUser:GetBadgeLv()
	if not self.__save.achv then return 0 end
	return self.__save.achv.Lv
end

function clsUser:GetBadgeAdd()
	local lv = self:GetBadgeLv()
	local info = ACHV.GetBadgeInfoByLv(lv)
	return info.attr
end

--检查成就是否达成
function clsUser:CheckAchvState(type, param)
	return ACHV.CheckAchvState(self, type, param)
end

function clsUser:SetAchvGot(achvId)
	if self:IsAchvGot(achvId) then
		return
	end
	ACHV.SetUserAchvGot(self, achvId)
end