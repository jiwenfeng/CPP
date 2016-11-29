--VIP系统
local WEEK_DAYS = 7

--VIP功能
VIP_BUY_VIGOUR = 1			--补充精力次数				1
VIP_TRAIN_LIMIT_UP = 2		--培养值上限增加
VIP_AUTO_REFINE = 3			--一键炼器					
VIP_INDENT_NUM_ADD = 4		--交易行上架物品增加 		1
VIP_USE_TOP_TRIPOD = 5		--直接使用高级炼器鼎		
VIP_WBOSS_REVIVAL_FREE = 6	--世界boss战免费复活		
VIP_UPGRADE_LIST_OPEN = 7	--免费开启建筑队列			
VIP_FAST_FIGHT = 8		--副本可跳过战斗			1
VIP_DAILY_MISS_NUM_ADD = 9	--每日任务可完成数量		1
VIP_DAILY_ROCK_TREE = 10	--摇钱树次数				1
VIP_TEAMCOPY_BUY_TIMES = 11	--组队副本可购买次数		1
VIP_FRIEND_MEMBER_NUM = 12	--好友最大数量				1

VIP_HERO_COPY_ADD_TIMES = 13	--英魂副本可增加次数
VIP_OPEN_EXTRA_CARD = 14		--副本额外翻牌
VIP_MONALBUM_AUTO = 15			--一键炼化功能
VIP_NORMAL_COPY_MOPUP_FINISH = 16			--普通副本扫荡立即完成
VIP_UPGRADELIST_NO_CD = 17		--建筑科技无冷却时间
VIP_HERO_COPY_MOPUP_FINISH = 18				--英魂副本扫荡立即完成

local VipInfoList = {}			--vip等级信息列表
local AllVipFuncList = {}		--vip功能信息列表

function GetVipInfoList()
	return VipInfoList
end

function GetVipInfoByLv(Lv)
	return VipInfoList[Lv]
end

function GetAllVipFuncList()
	return AllVipFuncList
end

function GetVipFuncInfoById(id)
	return AllVipFuncList[id]
end


------------------------------------------------------------------------------
local clsUser = USERBASE.clsUser

--累计充值数量
function clsUser:GetTotalRecharge()
	self.__save.recharge = self.__save.recharge or {}
	return self.__save.recharge.total or 0
end

--充值记录
function clsUser:SetTotalRecharge(data)
	self.__save.recharge = self.__save.recharge or {}
	self.__save.recharge.total = math.max((self.__save.recharge.total or 0) + data, 0)
	self:SetIngot(data)
	local Lv = GetVipLvByIngot(self.__save.recharge.total)
	if Lv >= self:GetVipLv() then
		self:SetVipLv(Lv)
	end
	self:SetFirstWeekRecharge(data)
	self:SetRechargeTimes(1)
	RECHARGE.RecordUserRecharge(self:GetRoleId(), data)
end
	
--累计首周充值数量
function clsUser:SetFirstWeekRecharge(data)
	if CalDiffDays(_G.SERVER_START_TIME, os.time()) > WEEK_DAYS then
		return
	end
	self.__save.recharge = self.__save.recharge or {}
	self.__save.recharge.weekly = math.max((self.__save.recharge.weekly or 0) + data)
end

function clsUser:GetFirstWeekRecharge()
	self.__save.recharge = self.__save.recharge or {}
	return self.__save.recharge.weekly
end

--累计充值次数
function clsUser:SetRechargeTimes(data)
	self.__save.recharge = self.__save.recharge or {}
	self.__save.recharge.times = math.max((self.__save.recharge.times or 0) + data)
end

function clsUser:GetRechargeTimes()
	self.__save.recharge = self.__save.recharge or {}
	return self.__save.recharge.times
end
------------------------------------------------------------------------------

function GetVipLvByIngot(ingot)
	local Lv = 0
	local list = GetVipInfoList()
	local max = table.size(list) - 1
	for k, v in ipairs(list) do
		if ingot >= (v.exp or math.huge) then
			if k >= Lv then
				Lv = math.min(k + 1, max)
			end
		end
	end
	return Lv
end

--vip功能可用次数
function GetVipFuncValidTimes(Lv, id)
	local funcInfo = GetVipFuncInfoById(id)
	if not funcInfo then
		return
	end
	if not funcInfo.isLimited then
		return
	end
	local info = GetVipInfoByLv(Lv)
	if not info then
		return 0
	end
	return info.funcList[id]
end

--vip功能是否可用
function IsVipFuncValid(Lv, id)
	local info = GetVipInfoByLv(Lv)
	if not info then
		return
	end
	return info.funcList[id]
end

--获取格式化的vip信息列表
function GetFormatedVipInfo(user)
	local Lv = user:GetVipLv()
	local sData = {vipLv = Lv}
	local info = GetVipInfoByLv(Lv)
	sData.needIngot = info.exp
	sData.recharged = user:GetTotalRecharge()
	sData.maxLv = table.size(GetVipInfoList()) - 1
	local function get_info(lv)
		local buff = {}
		local info = GetVipInfoByLv(lv)
		for k in ipairs(GetAllVipFuncList()) do
			if info.funcList[k] then
				table.insert(buff, {funcId = k, times = GetVipFuncValidTimes(lv, k)})
			end
		end
		return buff
	end
	sData.now = get_info(Lv)
	if GetVipInfoByLv(Lv + 1) then
		sData.next = get_info(Lv + 1)
	end
	return sData
end



local s_vip_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetFormatedVipInfo(user)
	connection:SendProtocol("c_vip_info", sData)
end




function __init__()
	local VIPLIST = Import("autocode/vipList.lua")
	VipInfoList = VIPLIST.GetTable()
	
	local VIPFUNC = Import("autocode/vipFuncList.lua")
	AllVipFuncList = VIPFUNC.GetTable()
	
	PROTOCOL.s_vip_info = s_vip_info
end
