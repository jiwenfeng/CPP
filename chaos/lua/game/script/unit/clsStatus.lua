--仙职系统

local MAX_STATUS_LV = 10		--最大仙职等级
local STATE_CAN_ENCHASE = 1		--可镶嵌
local STATE_CAN_UPGRADE = 2		--可升级
local STATE_INVALID = 0			--不可操作
local STATE_IDLE = -1			--未镶嵌

local AllStatusInfoList = {}		--仙职信息列表
local statusHoleList = {}			--仙职孔位信息

function GetAllStatusInfo()
	return AllStatusInfoList
end

function GetStatusInfoByLv(lv)
	return AllStatusInfoList[lv]
end

function GetStatusHoleInfoList()
	return statusHoleList
end

function GetStatusHoleInfoById(pos)
	return statusHoleList[pos]
end

function GetStatusHoleNum()
	return #statusHoleList
end
-----------------------------------------------------

clsStatus = clsObject:Inherit()

function clsStatus:__init__()
	Super(clsStatus).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsStatus:initsavedata()
	self.__save.Lv = 0
	self.__save.nimbus = 0
	self.__save.list = {}
end

function clsStatus:inittempdata()
	
end

function clsStatus:InitStatus(owner)
	self:SetLv(0)
	self:SetOwner(owner)
	
	self:InitHoleList()
end


function clsStatus:GetOwner()
	return self.__save.owner
end

function clsStatus:SetOwner(data)
	self.__save.owner = data
end

function clsStatus:GetLv()
	return self.__save.Lv
end

function clsStatus:SetLv(data)
	data = assert(tonumber(data))
	self.__save.Lv = self.__save.Lv + data
	if self.__save.Lv < 0 then
		self.__save.Lv = 0
	elseif self.__save.Lv > MAX_STATUS_LV then
		self.__save.Lv = MAX_STATUS_LV
	end
end

function clsStatus:GetMaxLv()
	return MAX_STATUS_LV
end

function clsStatus:GetNimbus()
	return self.__save.nimbus
end

function clsStatus:ResetNimbus()
	self.__save.nimbus = 0
end

function clsStatus:SetNimbus(data)
	data = assert(tonumber(data))
	self.__save.nimbus = self.__save.nimbus + data
	local info = GetStatusInfoByLv(self:GetLv())
	if self.__save.nimbus < 0 then
		self.__save.nimbus = 0
	elseif self.__save.nimbus > info.needNimbus then
		self.__save.nimbus = info.needNimbus
	end
end

--仙印升级
function clsStatus:Upgrade()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:SetLv(1)
	self:ResetNimbus(0)
	
	user:SyncData("status")
	TITLE.SetTitleInfo(self:GetOwner(), "status", self:GetLv())
	user:SetFairyGridMax(USERBASE.FAIRY_EQUIP, 1)
	user:CheckAchvState("status")
	if self:GetLv() >= 8 then
		TITLE.CheckHonorTitleRecord(user, "391104")
		if self:GetLv() >= 9 then
			TITLE.CheckHonorTitleRecord(user, "391105")
			if self:GetLv() >= 10 then
				TITLE.CheckHonorTitleRecord(user, "391106")
			end
		end
	end
end

--初始化宝石孔列表
function clsStatus:InitHoleList()
	self.__save.list = {}
	for k, v in ipairs(GetStatusHoleInfoList()) do
		self.__save.list[k] = {}
	end
end


function clsStatus:EnchaseOneGem(pos, gemId)
	self.__save.list[pos] = {id = gemId}
end

function clsStatus:PickOneGem(pos)
	local gemData = self.__save.list[pos]
	self.__save.list[pos] = {}
	return gemData
end

function clsStatus:IsHoleEnchased(pos)
	if not self.__save.list[pos] then
		return
	end
	return self.__save.list[pos].id
end

function clsStatus:GetHoleInfo(pos)
	return self.__save.list[pos]
end


function clsStatus:IsGemCanbeUpgrade(gemId)
	local gemInfo = BASEITEM.GetItemById(gemId)
	if not gemInfo.complex then
		return false
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local statusInfo = GetStatusInfoByLv(self:GetLv())
	if gemInfo.Lv + 1 > statusInfo.maxGemLv then
		return false
	end
	if user:GetItemNum(USERBASE.PACKAGE, gemInfo.complex) <= 0 then
		return false
	end
	return true
end

--获取孔位状态
function clsStatus:GetPosState(pos)
	local holeInfo = GetStatusHoleInfoById(pos)
	if not holeInfo then
		return STATE_IDLE
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local statusInfo = GetStatusInfoByLv(self:GetLv())
	local holeData = self:GetHoleInfo(pos)
	if not holeData.id then
		local gemInfo = BASEITEM.GetItemById(holeInfo.gemId)
		if gemInfo.Lv > statusInfo.maxGemLv then
			return STATE_IDLE
		end
		if user:GetItemNum(USERBASE.PACKAGE, holeInfo.gemId) <= 0 then
			return STATE_IDLE
		end
		return STATE_CAN_ENCHASE
	end
	if not self:IsGemCanbeUpgrade(holeData.id) then
		return STATE_INVALID
	end
	return STATE_CAN_UPGRADE
end

function clsStatus:GetFormatHoleInfo(pos)
	local buff = {}
	buff.pos = pos
	buff.state = self:GetPosState(pos)
	local holeData = self:GetHoleInfo(pos)
	if holeData and holeData.id then
		local gemInfo = BASEITEM.GetItemById(holeData.id)
		buff.goodsId = holeData.id
		buff.attr = gemInfo.attr
	else
		local holeInfo = GetStatusHoleInfoById(pos)
		buff.goodsId = holeInfo.gemId
	end
	return buff
end


function clsStatus:GetFormatHoleList()
	local holeNum = GetStatusHoleNum()
	local sData = {}
	for i = 1, holeNum do
		table.insert(sData, self:GetFormatHoleInfo(i))
	end
	return sData
end

function clsStatus:GetAttrAdd()
	local holeNum = GetStatusHoleNum()
	local sData = {}
	for i = 1, holeNum do
		local info = self:GetHoleInfo(i)
		if info and info.id then
			local gemInfo = BASEITEM.GetItemById(info.id)
			for k, v in pairs(gemInfo.attr) do
				sData[k] = (sData[k] or 0) + v
			end
		end
	end
	return sData
end

--镶嵌宝石
function clsStatus:Enchase(pos)
	local holeInfo = GetStatusHoleInfoById(pos)
	if not holeInfo then
		return false, "无效的孔位信息。"
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local holeData = self:GetHoleInfo(pos)
	if not holeData or holeData.id then
		return false, "该位置已经镶嵌过宝石了。"
	end
	local statusInfo = GetStatusInfoByLv(self:GetLv())
	local gemInfo = BASEITEM.GetItemById(holeInfo.gemId)
	if gemInfo.Lv > statusInfo.maxGemLv then
		return false, "您的仙职等级不足。"
	end
	if user:GetItemNum(USERBASE.PACKAGE, holeInfo.gemId) <= 0 then
		return false, "您背包中没有指定类型的宝石。"
	end
	self:EnchaseOneGem(pos, holeInfo.gemId)
	user:DelItem(USERBASE.PACKAGE, holeInfo.gemId, 1)
	user:FreshAllHeroTotalAttr()			--刷新仙印对主角及所有武将的属性影响
	return true
end

--升级已经镶嵌的宝石
function clsStatus:UpgradeEnchased(pos)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local holeData = self:GetHoleInfo(pos)
	if not holeData or not holeData.id then
		return false, "该位置并未镶嵌宝石。"
	end
	local gemInfo = BASEITEM.GetItemById(holeData.id)
	if not gemInfo or not gemInfo.complex then
		return false, "该宝石不能进行升级。"
	end
	local statusInfo = GetStatusInfoByLv(self:GetLv())
	if (gemInfo.Lv + 1) > statusInfo.maxGemLv then
		return false, "您的仙职等级不足，不能升级指定的宝石。"
	end
	if user:GetItemNum(USERBASE.PACKAGE, gemInfo.complex) <= 0 then
		return false, "您的背包中没有所需的宝石。"
	end
	self:EnchaseOneGem(pos, gemInfo.complex)
	user:DelItem(USERBASE.PACKAGE, gemInfo.complex, 1)
	user:FreshAllHeroTotalAttr()			--刷新仙印对主角及所有武将的属性影响
	return true
end



function clsStatus:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsStatus:Restore(data)
	assert(type(data) == "table")
	self.__save = data
end


function clsStatus:GetStatusInfo()
	local Lv = self:GetLv()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	local info = GetStatusInfoByLv(Lv)
	sData.statusLv = Lv
	sData.name = info.name
	sData.describe = info.describe
	sData.maxGemLv = info.maxGemLv
	
	sData.needLv = info.needLv
	sData.maxNimbus = info.needNimbus
	sData.nimbus = self:GetNimbus()
	if info.needMissionId then
		sData.missionId = info.needMissionId
		local missInfo = CLSMISS.GetMissionInfoById(info.needMissionId)
		sData.missName = missInfo and missInfo.name or ""
		sData.missState = user:IsMissionOver(info.needMissionId) and 1 or 0
	end
	local nextInfo = GetStatusInfoByLv(Lv + 1)
	if nextInfo then
		sData.nextName = nextInfo.name
	end
	return sData
end

function clsStatus:CheckUpgradeState()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local info = GetStatusInfoByLv(self:GetLv())
	if user:GetLv() < info.needLv then
		return false
	end
	if self:GetNimbus() < info.needNimbus then
		return false
	end
	if not user:IsMissionOver(info.needMissionId) then
		return false
	end
	if self:GetLv() >= MAX_STATUS_LV then
		return false
	end
	self:Upgrade()
	return true
end

function __init__()
	
	local STATUS_LIST = Import("autocode/statusList.lua")
	AllStatusInfoList = STATUS_LIST.GetTable()
	
	local STATUS_HOLE = Import("autocode/statusHoleList.lua")
	statusHoleList = STATUS_HOLE.GetTable()

end
