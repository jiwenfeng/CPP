local ITEM_UP_LIMIT = 99			--物品叠加上限

local INIT_HERO_MAX = 3				--初始最大武将槽数量
local INGOT_SLOT_NUM = 27			--元宝可解锁武将槽数量
--local NIMBUS_SLOT_NUM = 6			--灵气可解锁武将槽数量
local AUTO_SLOT_NUM = 5				--等级自动解锁武将槽数量

local INIT_GOLD_STORAGE = 500000
local INIT_VIGOUR_MAX = 200			--初始最大精力值
local VIGOUR_RESET_HOUR = 6			--精力重置时辰
INIT_SCENE = "697001"
local HERO_COPY_TOWN_ID = "697015"
local INIT_POS_X = 200
local INIT_POS_Y = 475
local USER_SIGHT_WIDTH = 675	--玩家视野半径

local INIT_SELL_NUM = 5				--初始可上架数量
local MAX_SELL_NUM = 10				--最大可上架数量

PACKAGE = 1
DEPOT = 2
BUYBACK = 3
EQUIP = 4

--背包类型
local bagTypeList = {
	[PACKAGE] = {max = 108, init = 24, unLock = {base = 1, add = 1}},
	[DEPOT] = {max = 49, init = 14, unLock = {base = 1, add = 1}},
	[BUYBACK] = {max = 99999, init = 99999},
}


local MAX_FAIRY_PANEL_GRID = 18		--炼器界面最大格子数
local MAX_FAIRY_BAG_GRID = 24		--仙器背包界面最大格子数
local INIT_FAIRY_BAG_GRID = 8		--仙器背包界面初始解锁格子数
local MAX_FAIRY_EQUIP_GRID = 8		--可装备仙器的最大格子数
local FIRST_FAIRY_GRID_COST = 5		--第一个可解锁格子价格
local FAIRY_GRID_UNLOCK_PRICE_ADD = 5	--格子价格增长量

local ItemUpLimit = {["weapon"] = 1, ["armor"] = 1, ["jewelry"] = 1, ["heroCard"] = 1, ["vote"] = 9999}
--local ItemUpLimit = {["equip"] = 1, ["heroCard"] = 1, ["vote"] = 9999}

local UserExpList = {}    --用户升级经验表


local FIRST_COST = 1

function GetUserExpList()
	return UserExpList
end

function GetUserUpgradeInfo(Lv)
	return UserExpList[Lv]
end

function GetUpgradeExp(lv)   --获得升级经验
	return UserExpList[lv].uplimit
end

local itemList = BASEITEM.GetAllItemList()
---------------------------------------------------------------------
--玩家类
clsUser = CHAR.clsChar:Inherit()

function clsUser:__init__()
	Super(clsUser).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsUser:initsavedata()
	self.__save.login = {}				--本次登录的时间和IP地址
	self.__save.offlineTime = nil		--上次离线时间
	self.__save.userId = nil			--用户数字ID
	self.__save.username = ""        --用户名
	self.__save.name = ""            --昵称
	self.__save.regTime	= os.time()    --注册时间

	self.__save.Lv = 0              --等级
	self.__save.exp = 0  	--用户经验
	self.__save.gold = 0            --金币
	self.__save.nimbus = 0			--灵气
	self.__save.voucher = 0			--礼券
	self.__save.ingot = 0			--元宝
	self.__save.locate = {scene = nil, x = nil, y = nil}		--玩家当前所在场景及位置
	
	self.__save.info = {
			headId = nil,  			--头像ID
			job = nil,      	--身份/职业
			phyle = nil,			--种族
			status = nil,			--仙职
			repute = 0,				--声望			
		}
	self.__save.point = {}			--升级属性点数
	self.__save.attrib = {}
	self.__save.special = {}

	self.__save.friend = {}         --好友列表
	self.__save.mail = {}			--邮件列表
	self.__save.mail.list = {}			--邮件列表
	self.__save.mail.count = 0
	
	self.__save.mission = {}		--任务列表				--[missionId] = misObj:GetSave();
	self.__save.missionOver = {}	--已完成的任务列表		--[missionId] = timestamp;
	
	self.__save.skill = {}
	self.__save.equip = {}
	self.__save.stats = {}			--统计数据

	self.__save.upgradeList = {}		--升级队列
end

function clsUser:inittempdata()
	self.__temp.loginTime = nil
	self.__temp.lastLogin = {}			--上次登录的时间和IP
	self.__temp.timestamp = nil  		--数据存储时间戳
	self.__temp.focus = {}

	self.__temp.item = {[PACKAGE] = {}, [DEPOT] = {}, [BUYBACK] = {}}
	self.__temp.mission = {}		--任务对象列表			--[missionId] = misObj
	self.__temp.callout = {}
	self.__temp.equip = {}
	self.__temp.fairyDev = {}
	self.__temp.upgradeList = {}			--升级队列
end


function clsUser:InitUser(username, name, userId, headId, job, sex, modelId, temp)
	self:SetUserName(username)
	self:SetName(name)
	self:SetRoleId(userId)
	self:SetHeadId(headId)
	self:SetSex(sex)
	self:SetRegTime(os.time())
	self:SetJob(job)
	self:SetLv(1)
	self:InitPoint()			--初始化玩家一级属性
	self:InitAttrib()			--初始化玩家二级属性
	self:InitSpecialAttrib()	--初始化玩家精英属性
--	local modelList = JOB.GetModelList(job, sex)
--	local modelId = modelList[math.random(1, #modelList)]
	self:SetModelId(modelId)
	self:InitItemBagGrid()		--初始化背包
	if not temp then
		
		self:InitSkillList()
		local list = SKILL.GetSkillListByJob(job, true)
		local skillId = list[4][math.random(1, #list[4])]
		self:SetSkill("4", skillId, 1)
--[[		
		self:InitFairyDev()			--初始化仙器系统
		self:InitAchvList()
		self:InitUserMission()	
		self:AddPet()]]
		self:InitFightForm()		--初始化战斗阵型
		FUNCOPEN.CheckFuncOpen(self)
		
		self:SetLocateInfo(INIT_SCENE, INIT_POS_X, INIT_POS_Y)
		self:RecommendPoint()
		
		self:InitVigour()
		self:InitHeroList()	
		self:InitFuncStat()				--初始化功能统计
		self:InitCopy()
		self:InitStatus()
		self:InitUserMail()			--初始化邮件系统
			
		self:FreshTotalAttr(true)
	end
end

function clsUser:Destroy()
	Super(clsUser).Destroy(self)
end
--保存玩家数据
function clsUser:GetSaveData()
	self:SaveItemData()
	self:SaveFairyDevData()
	self:SaveEquipData()
	self:SaveStatusData()
	self:SaveFriendData()
--	self:SaveSecData()
	self:SaveMissionData()
	self:SaveUpgradeList()
	self:SaveTribeData()
	self:SaveHeroData()
	self:SavePetData()
--	self:SaveMailData()
	self:SaveFormData()
	self:SaveMonAlbumData()
	return table.deepcopy(self.__save)
end
--恢复玩家数据
function clsUser:Restore(data)
	self.__save = data
	self:RestoreHeroData()
	self:RestorePetData()
	self:RestoreItemData()
	self:RestoreFairyDevData()
	self:RestoreStatusData()
	self:RestoreUpgradeList()
	self:RestoreTribeData()
	self:RestoreEquipData()
	self:RestoreFriendData()
--	self:RestoreMailData()
--[[	self:RestoreCompData()
	self:RestoreSecData()]]
	self:RestoreFormData()
	self:RestoreMissionData()
	self:RestoreMonAlbum()
end
--设置处理离线操作状态
function clsUser:SetOfflineProState(state)
	self.__temp.offlineState = state
end

function clsUser:GetOfflineProState()
	return self.__temp.offlineState
end

--处理玩家离线数据
function clsUser:ProOfflineData()
	self:LeaveScene()
	self:OfflineProFight()
end

--处理玩家上线数据计算
function clsUser:LoginProUserData()
	self:LoginRestoreVigour()
	self:LoginClearMail()
	EMBASSY.RenewStayHeroInfo(self)
	self:UpdateOnlinePrizeInfo()
--	self:ProUnionApply()
--	self:ProUnionMsg()
	USER.UpdateUserInfo(self)
	USER.UpdateRankInfo(self:GetRoleId())
	FIGHT.UpdateUserFightData(self:GetRoleId())
	CITY.RefreshUserCityInfo(self:GetRoleId())
	self:InitUserInSight(true)	--上线之后初始化同屏玩家人数
end
--检查玩家登录后提示信息
function clsUser:LoginCheckAct()
	
end

--设置用户名
function clsUser:SetUserName(data)
	self.__save.username = data
	USER.SetAllUserNick(self:GetRoleId(), data, self:GetName(), self:GetHeadId(), self:GetJob(), self:GetVipLv())
end

function clsUser:GetUserName()
	return self.__save.username
end
--设置昵称
function clsUser:SetName(data)
	self.__save.name = data
	USER.SetAllUserNick(self:GetRoleId(), self:GetUserName(), data, self:GetHeadId(), self:GetJob(), self:GetVipLv())
end

--设置玩家数字ID
function clsUser:SetRoleId(data)
	self.__save.userId = data
	USER.SetAllUserNick(data, self:GetUserName(), self:GetName(), self:GetHeadId(), self:GetJob(), self:GetVipLv())
end

function clsUser:GetRoleId()
	return self.__save.userId
end
--设置头像Id
function clsUser:SetHeadId(data)
	self.__save.info.headId = data
	USER.SetAllUserNick(self:GetRoleId(), self:GetUserName(), self:GetName(), data, self:GetJob(), self:GetVipLv())
end

--设置玩家职业身份
function clsUser:SetJob(data)
	self.__save.info.job = data
	USER.SetAllUserNick(self:GetRoleId(), self:GetUserName(), self:GetName(), self:GetHeadId(), data, self:GetVipLv())
end

function clsUser:SetVipLv(data, timelong)
	self.__save.vip = self.__save.vip or {}
	self.__save.vip.Lv = data
	self:RemoveVipCallOut()
	if tonumber(timelong) then
		self.__save.vip.endTime = (self.__save.vip.endTime or os.time()) + timelong
		self:StartVipCallOut()
	else
		self.__save.vip.endTime = nil
	end
	if self:HasFreeUpgradeList() then
		self:SetUpgradeListState("building", "2", UPGRADELIST.LIST_UNLOCK)
	end
	USER.ModUserNickInfo(self:GetRoleId(), "vipLv", data)
	self:SyncData("vip")
	self:CheckAchvState("vip")
end

function clsUser:ResetVipLv()
	local Lv = VIP.GetVipLvByIngot(self:GetTotalRecharge())
	self:SetVipLv(Lv)
end

function clsUser:GetVipLv()
	self.__save.vip = self.__save.vip or {}
	if self.__save.vip.endTime then
--		return VIP.GetVipLvByIngot(self:GetTotalRecharge())
	end
	return self.__save.vip.Lv or 0
end

function clsUser:StartVipCallOut()
	if self.__save.vip.endTime then
		local timelong = self.__save.vip.endTime - os.time()
		if timelong <= 0 then
			self:ResetVipLv()
		else
			self.__temp.vipTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.ResetVipLv, timelong, self)
		end
	end
end

function clsUser:RemoveVipCallOut()
	if self.__temp.vipTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.vipTimer)
	end
end

function clsUser:SetId(data)
	self.__temp.id = data
end

function clsUser:GetId()
	return "user"
end

function clsUser:SetSex(data)
	self.__save.sex = data
	USER.ModUserNickInfo(self:GetRoleId(), "sex", data)
end

function clsUser:SetCharm(data)
	self.__save.info.charm = math.max((self.__save.info.charm or 0) + data, 0)
end

function clsUser:GetCharm()
	return self.__save.info.charm or 0
end

--设置声望
function clsUser:SetRepute(data)
	data = assert(tonumber(data))
	self.__save.info.repute = data
	self:SyncData("repute")
end

function clsUser:GetRepute()
	return self.__save.info.repute
end

function clsUser:ModRepute(data, flag)
	assert(tonumber(data))
	self.__save.info.repute = self.__save.info.repute + data
	if not flag then
		self:SyncData("repute")
	end
end

--设置成就
function clsUser:SetAchvNum(data)
	self.__save.achv = data
	self:SyncData("achv")
end

--获取成就点数
function clsUser:GetAchvNum()
	return self.__save.achv or 0
end

function clsUser:ModAchvNum(data)
	assert(tonumber(data))
	self.__save.achv = self.__save.achv + data
	self:SyncData("achv")
end

----------------------------------------------------------------------------------------
--[[获取仙职
function clsUser:InitStatus()
	local status = STATUS.clsStatus:New()
	status:InitStatus(self:GetRoleId())
	self.__temp.status = status
end

function clsUser:GetStatus()
	return self.__temp.status
end

function clsUser:GetStatusLv()
	local status = self:GetStatus()
	if status then
		return status:GetLv()
	end
	return 0
end

function clsUser:SaveStatusData()
	local status = self:GetStatus()
	if status then
		self.__save.status = status:GetSaveData()
	end
end

function clsUser:RestoreStatusData()
	local statusData = self.__save.status
	if statusData then
		local status = STATUS.clsStatus:New()
		status:Restore(statusData)
		self.__temp.status = status
	end
end]]

--获取官职
function clsUser:SetOfficialInfo(officialId, pos)
	self.__save.official = {id = officialId, pos = pos}
	self:SyncData("official")
end

function clsUser:GetOfficialInfo()
	self.__save.official = self.__save.official or {}
	return self.__save.official
end

function clsUser:GetOfficialId()
	self.__save.official = self.__save.official or {}
	return self.__save.official.id
end

function clsUser:GetOfficialName()
--	local officialInfo = OFFICIAL.GetOfficialInfoById(self:GetOfficialId())
--	if not officialInfo then return end
--	return officialInfo.name
	return ""
end

--------------------------------------------------------------------------------
--最大订单数量
--[[
function clsUser:SetMaxIndentNum(data)
	--self.__save.indent = self.__save.indent or {}
	--self.__save.indent.max = data or INIT_SELL_NUM
end]]

function clsUser:GetMaxIndentNum()
	local times = VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_INDENT_NUM_ADD)
	return INIT_SELL_NUM + (times or 0)
end
--当前订单数量
function clsUser:SetIndentCount(data)
	self.__save.indent = self.__save.indent or {}
	self.__save.indent.num = (self.__save.indent.num or 0) + data
end

function clsUser:GetIndentCount()
	self.__save.indent = self.__save.indent or {}
	return self.__save.indent.num or 0
end

function clsUser:IsIndentCanBeAdd()
	return self:GetIndentCount() < self:GetMaxIndentNum()
end

------------------------------------------------------------------
function clsUser:SetUnionInfo(unionId, pos, time)
	self.__save.union = self.__save.union or {}
	if unionId then
		self.__save.union.id = unionId
		self.__save.union.pos = pos
		if not self.__save.union.enterTime then
			self.__save.union.enterTime = time or os.time()
			self:SyncData("union")
		end
	else
		self.__save.union = {quitTime = time or os.time()}
		self:SyncData("union")
	end
	USER.ModUserNickInfo(self:GetRoleId(), "unionId", unionId)
	self:Broad2ExNeighbor("union", nil, nil, true)
	CITY.RefreshUserCityInfo(self:GetRoleId(), true)		--联盟信息改变时，更新城池对应的信息
end

function clsUser:GetUnionInfo()
	return self.__save.union or {}
end

function clsUser:GetUnionQuitTime()
	return self.__save.union.quitTime
end

function clsUser:GetUnionId()
	self.__save.union = self.__save.union or {}
	return self.__save.union.id
end

function clsUser:GetUnionPos()
	self.__save.union = self.__save.union or {}
	return self.__save.union.pos
end

function clsUser:GetUnionName()
	local unionId = self:GetUnionId()
	local union = UNION.GetUnionById(unionId)
	if union then
		return union:GetName()
	end
	return ""
end


function clsUser:QuitUnion(timestamp)
	local unionId = self:GetUnionId()
	local union = UNION.GetUnionById(unionId)
--[[	if union then
		local delateInfo = union:GetDelateInfo()
		if delateInfo and delateInfo.roleId == roleId then
			union:CancelDelate(true)
		end
	end]]
	if union then
		union:RmFromList("member", self:GetRoleId())
	end
	self:SetUnionInfo(nil, nil, timestamp or os.time())
end
--发出的个人联盟邀请 coroutine
function clsUser:SetUnionInviteCort(roleId, co)
	self.__temp.unionInvite = self.__temp.unionInvite or {}
	self.__temp.unionInvite[roleId] = co
end

function clsUser:GetUnionInviteCort(roleId)
	return self.__temp.unionInvite
end


----------------------------------------------------------------------
-- 玩家一级属性，具备 set、get、mod方法

--分配升级点数
function clsUser:SetPoint(strength, intellect, agility, captain)
	self:ModStrength(strength, true)
	self:ModIntellect(intellect, true)
	self:ModAgility(agility, true)
	self:ModCaptain(captain, true)
	local sum = strength + intellect + agility + captain
	self:ModLeftPoint(-sum)
--	self:SyncData("strength", "intellect", "agility", "captain", "point")
end

--推荐加点
function clsUser:RecommendPoint()
	local info = JOB.GetJobInfoById(self:GetJob())
	local left = self:GetLeftPoint()
	local list = {}
	while left > 0 do
		for k, v in ipairs(info.recommend) do
			local num = math.min(left, v.num)
			list[v.type] = (list[v.type] or 0) + num
			left = left - num
			if left <= 0 then
				break
			end
		end
	end
	self:SetPoint(list.strength or 0, list.intellect or 0, list.agility or 0, list.captain or 0)
	self:SetLeftPoint(0)
end
-----------------------------------------------------------------
-- 玩家二级属性： Get时无参数表示获取总和
--人物气势		--type: "base" / "add"

function clsUser:InitAttrib()
	self.__save.attrib = {}
	self.__save.attrib.hp = 150
	self.__save.attrib.hpMax = 150
	self.__save.attrib.physcAtk = {min = 50, max = 50, add = 0}
	self.__save.attrib.physcDef = {base = 0, add = 0}
	self.__save.attrib.magicAtk = {min = 50, max = 50, add = 0}
	self.__save.attrib.magicDef = {base = 0, add = 0}
	self.__save.attrib.speed = 100
end

--生成属性培养数值
function clsUser:GenAttrTrain(type)
	local vipLv = self:GetVipLv()
	local uplimit = USER.GetAttrTrainUpLimit(self:GetStatusLv(), vipLv + 1)
	local cur = {}
	cur.physcAtkAdd = self:GetPhyscAtk("add")
	cur.physcDefAdd = self:GetPhyscDef("add")
	cur.magicAtkAdd = self:GetMagicAtk("add")
	cur.magicDefAdd = self:GetMagicDef("add")
	local flag = true
	for k, v in pairs(cur) do
		if v < uplimit then
			flag = false
			break
		end
	end
	if flag then
		return
	end
	local function gen_num(cur)
		local num = 0
		if type == "nimbus" then
			if not self:GetStatusLv() or math.random() <= 0.8 then
				num = math.random(1, 5)
			else
				num = -math.random(1, 3)
			end
		elseif type == "ingot" then
			if math.random() <= 0.8 then
				num = math.random(4, 7)
			else
				num = math.random(1, 3)
			end
		end
		return math.min(math.max(0, num + cur), uplimit)
	end
	local info = {}
	for k, v in pairs(cur) do
		info[k] = gen_num(v)
	end
	self.__temp.attrTrain = info
	return self.__temp.attrTrain
end
------------------------------------------------------------------------------
--精英属性

-------------------------------------------------------------------------------
function clsUser:GetMaxLv()
	return SERVER_ROLE_MAX_LV
end

function clsUser:GetUpgradeExp()
	return GetUpgradeExp(self:GetLv())
end

--打坐经验(每分钟)
function clsUser:GetMeditateExp()
	return UserExpList[self:GetLv()].meditate
end
--设置等级
function clsUser:SetLv(data)
	if tonumber(data) == 0 then
		return
	end
	self.__save.Lv = self.__save.Lv + tonumber(data)
	if self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	elseif self.__save.Lv < 1 then
		self.__save.Lv = 1
	end
	USER.ModUserNickInfo(self:GetRoleId(), "Lv", self:GetLv())
end

--设置经验
function clsUser:SetExp(data, type, flag)	
	self.__save.exp = math.max(math.floor(self.__save.exp + data), 0)
	self:CheckUpgrade()
	DATA.log(string.format("%s获得经验%s,for %s", self:GetName(), data, type or ""), "userexp.log")
	if not flag then		
		self:SyncData("exp")
	end
end

--检查升级
function clsUser:CheckUpgrade()
	local expMax = GetUpgradeExp(self:GetLv())
	if self.__save.exp >= expMax then
		if self:GetLv() < self:GetMaxLv() then
			self.__save.exp = self.__save.exp - expMax
			return self:Upgrade()
		else
			self.__save.exp = expMax
		end
	end
end

function clsUser:Upgrade()
	self:SetLv(1)
	self:ModLeftPoint(5)
	self:SetHpMax(10)
	self:RecommendPoint()
	if self:GetLv() % 2 == 0 then
		self:SetSkillPoint(1)
	end
	if self:GetLv() >= 100 then
		TITLE.CheckHonorTitleRecord(self, "391103")
	end
	
	local status = self:GetStatus()
	if status then
		status:CheckUpgradeState()
	end
	if self:GetHeroUnLockNeed("auto") and self:GetHeroUnLockNeed("auto") == self:GetLv() then
		self:SetHeroUnLockNum("auto", 1)
		self:SetHeroMax(1)
	end
	FUNCOPEN.CheckFuncOpen(self)
	self:CheckUpgrade()
	if self:IsFuncOpen(SETTING.FUNC_ACHIEVE) then
		self:CheckAchvState("userLv")
	end
	local pet = self:GetPet()
	pet:CheckPetForm()
	self:FreshTotalAttr()
	self:SyncData("Lv")
	STATS.SaveUpgradeHistory(self)
end
-------------------------------------------------------------------------------
--[[
local equipPosList = {
	[1] = "roleHead",	[2] = "roleHand",	[3] = "upperBody",
	[4] = "roleFace", 	[5] = "roleParts",	[6] = "lowerBody",
}

]]

--卸下装备
function clsUser:UnfixEquip(pos, flag)
	local equip = self:GetEquipByPos(pos)
	if not equip then
		return
	end
	equip:SetInUse()		--设置装备未使用
	self.__temp.equip[pos] = nil
	self:AddItem(PACKAGE, equip, true)
	if not flag then
		self:FreshTotalAttr()
	end
end

function clsUser:GenAllEquipShowList()
	local sData = {}
	local index = 0
	local function get_info(equip)
		local buff = {}
		buff.roleId = self:GetRoleId()
		buff.goodsId = equip:GetId()
		buff.force = equip:CalFightForce()
		buff.quality = equip:GetQuality()
		index = index + 1
		buff.index = index
		return buff
	end
	local function add_quip_to_list(obj)
		for _, equip in pairs(obj:GetEquipList()) do
			table.insert(sData, get_info(equip))
		end
	end
	add_quip_to_list(self)
	for _, heroObj in pairs(self:GetHeroList()) do
		add_quip_to_list(heroObj)
	end
	for _, v in pairs(self:GetAllItemList(PACKAGE)) do
		if v:GetClass() == "equip" then
			table.insert(sData, get_info(v))
		end
	end
	table.sort(sData, function(a, b)
		return a.force > b.force
	end)
	self.__temp.allEquip = sData
end

function clsUser:GetAllEquipShowList()
	return self.__temp.allEquip
end

function clsUser:AddEquipToRank(index)
	local list = self:GetAllEquipShowList()
	for _, v in pairs(list) do
		if v.index == index then
			return v
		end
	end
end

--------------------------------------------------------------------------------------------------


function GetPackageGridNum(type, kind)
	if not bagTypeList[type] then return 0 end
	return bagTypeList[type][kind]
end

function GetPackageGridUnlockCost(type, kind)
	if not bagTypeList[type] then return end
	return bagTypeList[type].unLock[kind]
end

function GetUnlockNeedTime(num)
	return ((num + 1) ^ 2 - (num + 1) + 1) * 30 + 300
end

--当前开放的
function clsUser:GetGridMax(type)
	self.__save.bagGrid = self.__save.bagGrid or {}
	self.__save.bagGrid[type] = self.__save.bagGrid[type] or {max = bagTypeList[type].init, unLock = {}}
	return self.__save.bagGrid[type].max or bagTypeList[type].init
end

function clsUser:SetGridMax(type, data)
	self.__save.bagGrid = self.__save.bagGrid or {}
	self.__save.bagGrid[type] = self.__save.bagGrid[type] or {max = bagTypeList[type].init, unLock = {}}
	self.__save.bagGrid[type].max = (self.__save.bagGrid[type].max or bagTypeList[type].init) + data
	if self.__save.bagGrid[type].max < bagTypeList[type].init then
		self.__save.bagGrid[type].max = bagTypeList[type].init
	elseif self.__save.bagGrid[type].max > bagTypeList[type].max then
		self.__save.bagGrid[type].max = bagTypeList[type].max
	end
end

function clsUser:SetUnLockNum(type, method, num)
	self.__save.bagGrid = self.__save.bagGrid or {}
	self.__save.bagGrid[type] = self.__save.bagGrid[type] or {max = bagTypeList[type].init, unLock = {}}
	self.__save.bagGrid[type].unLock[method] = self.__save.bagGrid[type].unLock[method] or {}
	local info = self.__save.bagGrid[type].unLock[method]
	info.num = (info.num or 0) + num
	if method == "auto" then
		info.recordTime = self:GetTotalOnlineTime()
	end
end

function clsUser:GetUnLockNum(type, method)
	self.__save.bagGrid = self.__save.bagGrid or {}
	self.__save.bagGrid[type] = self.__save.bagGrid[type] or {max = bagTypeList[type].init, unLock = {}}
	self.__save.bagGrid[type].unLock[method] = self.__save.bagGrid[type].unLock[method] or {}
	local info = self.__save.bagGrid[type].unLock[method]
	info.num = info.num or 0
	if method == "auto" then
		info.recordTime = info.recordTime or self:GetTotalOnlineTime()
	end
	return info.num, info.recordTime
end

function clsUser:GetAutoUnlockLeftTime(type)
	if type ~= PACKAGE then return end
	local unLockNum, recordTime = self:GetUnLockNum(type, "auto")
	if self:GetGridMax(type) >= GetPackageGridNum(type, "max") then
		return
	end
	local unLockNeedTime = GetUnlockNeedTime(unLockNum)
	return unLockNeedTime + recordTime - self:GetTotalOnlineTime()
end

function clsUser:StartUnLockPackageCallOut()
	local timelong = self:GetAutoUnlockLeftTime(PACKAGE)
	if not timelong then
		return
	end
	if timelong <= 0 then
		self:AutoUnlockPackage()
	else
		self.__temp.unlockPackageTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.AutoUnlockPackage, timelong, self)
	end
end

function clsUser:RemoveUnLockPackageCallOut()
	if self.__temp.unlockPackageTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.unlockPackageTimer)
	end
end

function clsUser:AutoUnlockPackage()
	self:SetGridMax(PACKAGE, 1)
	self:SetUnLockNum(PACKAGE, "auto", 1)
	self:SendBagGridUpdate(PACKAGE)
	if self:GetGridMax(PACKAGE) < bagTypeList[PACKAGE].max then
		self:StartUnLockPackageCallOut()
	end
end

function clsUser:SendBagGridUpdate(type)
	local buff = {}
	buff.stoType = type
	buff.unlockNum = self:GetGridMax(type)
	buff.firstCost = self:CalUnlockPrice(type, 1)
	buff.unlockTime = self:GetAutoUnlockLeftTime(type)
	self:SendProtocol("c_bagGrid_update", buff)
end

function clsUser:CalUnlockPrice(type, num)
	if num <= 0 then return 0 end
	local bagInfo = bagTypeList[type]
	local firstCost = bagInfo.unLock.base
	local function get_sum(n)
		return n * bagInfo.unLock.base + n * (n - 1) * bagInfo.unLock.add / 2
	end
	local unLockNum = self:GetUnLockNum(type, "ingot")
	return get_sum(unLockNum + num) - get_sum(unLockNum)
end


function clsUser:InitItemBagGrid()
	self.__save.bagGrid = {}
	for k, v in pairs(bagTypeList) do
		self.__save.bagGrid[k] = {max = v.init, unLock = {}}
	end
end

---------------------------------------------------------------------------------------------------

local itemType = {}
--获取物品叠加上限
function GetItemUpLimit(id)
	local info = BASEITEM.GetItemById(id)
	local uplimit = ItemUpLimit[info.type]
	if uplimit then
		return uplimit
	end
	return ITEM_UP_LIMIT
end

function clsUser:GetItemAttr(type, areaId, show)
	local sData = {}
	local item = self:GetItemByAreaId(type, areaId)
	if item then
		sData = item:GetFullAttr()
		if not show then
			sData.areaType = item:GetAreaType()
			sData.areaId = areaId
			sData.num = item:GetNum()
		end
		sData.missionId = item:GetMissionId()
	end
	return sData
end

--发送新物品信息
function SendNewItem(user, type, areaId)
	local sData = user:GetItemAttr(type, areaId)
	sData.stoType = type
	user:SendProtocol("c_item_new", sData)
end

--更新物品信息
function SendUpdateItem(user, type, areaId, num)
	user:SendProtocol("c_item_update", {stoType = type, areaId = tostring(areaId), num = num})
end

function clsUser:SwitchItem(type, from, to)
	local list = self.__temp.item[type]
	if not list or not list[from] then
		return
	end
	if not to or tonumber(to) <= 0 or tonumber(to) > self:GetGridMax(type) then
		return
	end
	if list[to] and list[from]:GetId() == list[to]:GetId() and list[from]:GetMissionId() == list[to]:GetMissionId() then
		local upLimit = GetItemUpLimit(list[to]:GetId())
		local num = math.min(upLimit - list[to]:GetNum(), list[from]:GetNum())
		list[from]:ModNum(-num)
		list[to]:ModNum(num)
		SendUpdateItem(self, type, from, list[from]:GetNum())
		SendUpdateItem(self, type, to, list[to]:GetNum())
		if list[to]:GetNum() <= 0 then
			list[to]:Destroy()
			self.__temp.item[type][to] = nil
		end
	else
		list[to], list[from] = list[from], list[to]
		SendNewItem(self, type, to)
		if list[from] then
			SendNewItem(self, type, from)
		else
			SendUpdateItem(self, type, from, 0)
		end
	end
end


function clsUser:AddItemById(type, id, num, param, save)
	param = param or {}
	local item = BASEITEM.NewItem(id, num, param)
	if not item then
		DATA.log(string.format("AddItemById: valid item id %s on user %s.", id, self:GetUserName()), "user.log")
		return
	end
	if save then
		item:Restore(save)
		item:SetNum(num)
	end
	return self:AddItem(type, item)
end

--添加物品
function clsUser:AddItem(type, item, flag)				--这里的参数是一个item对象，可以由NewItem方法生成  flag 发送邮件
	local id = item:GetId()
	local itemType = BASEITEM.GetItemAreaType(id)
	if not BASEITEM.GetItemAreaTypeList()[itemType] then
		DATA.log(string.format("%s添加物品失败：无效的物品，id:%d, areaType:%s。", self:GetName(), id, type or ""), "user.log")
		return	
	end
	if not self:IsItemCanAdd(PACKAGE, item) then
		local mail = {}
		mail.title = "新物品提示"
		mail.content = "由于背包已经满，物品发送到邮件"
		mail.attach = {item = {}}
		table.insert(mail.attach.item, {id = item:GetId(), num = item:GetNum(), lock = item:IsLocked(), missionId = item:GetMissionId()})
		return MAIL.SendSysMail(self:GetRoleId(), mail)
	end
	MISSION.CheckMissionEvent(self, "collect", id, item:GetNum())
	local function add_item(item)
		for k, v in pairs(self.__temp.item[type] or {}) do
			if v:GetId() == id then
				if v:IsSameItem(item) and v:GetNum() < GetItemUpLimit(id) then				--相同物品
					v:ModNum(item:GetNum())
					if v:GetNum() > GetItemUpLimit(id) then			--叠加上限
						SendUpdateItem(self, type, k, GetItemUpLimit(id))
						item:SetNum(v:GetNum() - GetItemUpLimit(id))
						v:SetNum(GetItemUpLimit(id))
						return add_item(item)
					else
						SendUpdateItem(self, type, k, v:GetNum())
						return k
					end
				end
			end
		end
		local MaxNum = self:GetGridMax(type)
		for i = 1, MaxNum do
			if not self.__temp.item[type][tostring(i)] then		--空位置
				if item:GetNum() > GetItemUpLimit(id) then
					local newItem = table.deepcopy(item)
					newItem:SetNum(GetItemUpLimit(id))
					self.__temp.item[type][tostring(i)] = newItem
					SendNewItem(self, type, tostring(i))
					item:ModNum(-GetItemUpLimit(id))
					return add_item(item)
				else
					self.__temp.item[type][tostring(i)] = item
					SendNewItem(self, type, tostring(i))
					return tostring(i)
				end
			end
		end
		DATA.log(string.format("%s添加物品失败：背包空间已满，多余物品将被丢弃，id:%d, num:%d。", self:GetName(), id, item:GetNum()), "user.log")
	end
	return add_item(item)
end
--获取物品区域Id
--type == 0 背包
--type == 1 仓库
function clsUser:GetItemAreaId(type , id, num, param)
	param = param or {}
	for k, v in pairs(self.__temp.item[type]) do
		if v:GetId() == id and v:GetNum() >= num then
			if param.missionId and param.missionId == v:GetMissionId() then
				return k
			elseif not v:GetMissionId() and param.lock == v:IsLocked() then
				return k
			end
		end
	end
	return false
end

--删除物品
--type == 0 背包
--type == 1 仓库
function clsUser:DelItem(type, id, num, param)			-- param= {lock = , missionId = ,}
	if num <= 0 then
		return
	end
	local function del_item(num, lock)
		param = param or {}
		param.lock = lock
		local areaId = self:GetItemAreaId(type, id, 1, param)
		if not areaId then
			return false, num
		end
		local item = self:GetItemByAreaId(type, areaId)
		local count = item:GetNum()
		if count < num then
			self:DelItemByAreaId(type, areaId, count)
			return del_item(num - count, lock)
		else
			self:DelItemByAreaId(type, areaId, num)
			return true
		end
	end
	if not param then					--对于普通物品，删除时优先删除绑定物品
		local rst, left = del_item(num, 1)
		if not rst then
			return del_item(left)
		end
		return rst
	else
		return del_item(num)
	end	
end

--通过位置删除物品
function clsUser:DelItemByAreaId(type, areaId, num, flag)
	if not num or num <= 0 then
		return false
	end
	local item = self:GetItemByAreaId(type, areaId)
	if not item then
		return false	
	end
	item:ModNum(-num)
	SendUpdateItem(self, type, areaId, item:GetNum())
	if item:GetNum() <= 0 then
		item:Destroy()
		self.__temp.item[type][areaId] = nil
	end
	return true
end

--保存物品数据
function clsUser:SaveItemData()
	self.__save.item = {[PACKAGE] = {}, [DEPOT] = {}, [BUYBACK] = {}}			--先置空
	for type, value in pairs(self.__temp.item) do
		if type ~= BUYBACK then
			self.__save.item[type] = self.__save.item[type] or {}
			for k, v in pairs(value) do
				self.__save.item[type][k] = v:GetSaveData()
			end
		end
	end
end

--恢复物品数据
function clsUser:RestoreItemData()
	for type, value in pairs(self.__save.item) do
		if type == PACKAGE or type == DEPOT then
			self.__temp.item[type] = self.__temp.item[type] or {}
			for k, v in pairs(value) do
				if itemList[v.id] then
					local item = BASEITEM.NewItem(v.id, v.num, {missionId = v.missionId, lock = v.lock, time = v.time, from = v.from})
					if item then
						item:Restore(v)
						self.__temp.item[type][tostring(k)] = item
					else
						DATA.log(string.format("RestoreItem: valid item id %s on user %s.", id, self:GetUserName()), "user.log")
					end
				end
			end
		end
	end	
end

--获取回购物品列表
function clsUser:GetBuybackList()
	local sData = {}
	sData.list = {}
	for k, v in pairs(self.__temp.item[BUYBACK]) do
		table.insert(sData.list, self:GetItemAttr(BUYBACK, tostring(k)))
	end
	sData.stoType = BUYBACK
	return sData
end

--获取背包/仓库物品列表
function clsUser:GetItemList(type)
	local sData = {}
	sData.list = {}
	local itemTbl = {}
	if type == BUYBACK then
		return 
	end
	for k, v in pairs(self.__temp.item[type]) do
		table.insert(sData.list, self:GetItemAttr(type, tostring(k)))
	end
	sData.stoType = type
	sData.priceAdd = GetPackageGridUnlockCost(type, "add")
	sData.unlockNum = self:GetGridMax(type)
	sData.maxNum = GetPackageGridNum(type, "max")
	sData.firstCost = self:CalUnlockPrice(type, 1)
	sData.unlockTime = self:GetAutoUnlockLeftTime(type)
	sData.unlockNeedTime = GetUnlockNeedTime(self:GetUnLockNum(type, "auto"))
	return sData
end
--通过存储位置查询物品
function clsUser:GetItemByAreaId(type, areaId)
	return self.__temp.item[type][areaId]
end
--通过物品id和属性查询物品
function clsUser:GetItemById(type, id, num, param)
	local areaId = self:GetItemAreaId(type, id, num or 1, param)
	return self:GetItemByAreaId(type, areaId)
end
--获取某种物品的总数量
function clsUser:GetItemNum(type, id, param)
	param = param or {}
	local count = 0
	for k, v in pairs(self.__temp.item[type]) do
		if v:GetId() == id and (not param.lock or param.lock == v:IsLocked()) and (not param.missionId or param.missionId == v:GetMissionId()) then
			count = count + v:GetNum()
		end
	end
	return count
end
--整理物品
function clsUser:SortItem(type)
	local buff = {}
	for k, v in pairs(self.__temp.item[type] or {}) do
		if v:GetNum() > GetItemUpLimit(v:GetId()) then
			local newItem = table.deepcopy(v)			
			newItem:ModNum(-GetItemUpLimit(v:GetId()))
			v:SetNum(GetItemUpLimit(v:GetId()))
			self:AddItem(type, newItem, true)
		elseif v:GetNum() <= 0 then
			self.__temp.item[type][k] = nil
		end
	end
	local function sort_item(itemList, item)
		local function pro_same_id_item(item, index, newItem)	--处理同id的物品插入顺序
			if item:GetNum() < GetItemUpLimit(item:GetId()) and item:IsSameItem(newItem) then		--相同物品进行合并
				item:ModNum(newItem:GetNum())
				if item:GetNum() > GetItemUpLimit(item:GetId()) then	--超出叠加上限					
					newItem:SetNum(item:GetNum() - GetItemUpLimit(item:GetId()))
					item:SetNum(GetItemUpLimit(item:GetId()))
					return sort_item(itemList, newItem)
				end
			elseif (tonumber(newItem:GetMissionId() or 0)) > (tonumber(item:GetMissionId() or 0)) then	--有任务id的排在前面
				table.insert(itemList, index, newItem)
			elseif (newItem:IsLocked() or 0) > (item:IsLocked() or 0) then		--有lock字段的排在前面
				table.insert(itemList, index, newItem)
			else
				if not itemList[index + 1] or itemList[index + 1]:GetId() ~= newItem:GetId() then
					table.insert(itemList, index + 1, newItem)
				else
					pro_same_id_item(itemList[index + 1], index + 1, newItem)
				end
			end
		end
		local function add_item(item, head, tail)		--使用折半查找法寻找合适的插入位置
			if tail == 0 then
				return table.insert(itemList, item)
			end
			if head >= tail then
				if item:GetId() == itemList[tail]:GetId() then
					return pro_same_id_item(itemList[tail], tail, item)
				elseif item:GetId() > itemList[tail]:GetId() then
					return table.insert(itemList, tail + 1, item)
				else
					return table.insert(itemList, tail, item)
				end
			end
			local mid = math.ceil((tail + head) / 2)
			if item:GetId() == itemList[mid]:GetId() then
				return pro_same_id_item(itemList[mid], mid, item)
			elseif item:GetId() < itemList[mid]:GetId() then
				return add_item(item, head, mid - 1)
			else
				return add_item(item, mid + 1, tail)
			end
		end
		add_item(item, 1, #itemList)
	end
	for _, itemObj in pairs(self.__temp.item[type]) do
		sort_item(buff, itemObj)
	end
	self.__temp.item[type] = {}
	for k, v in ipairs(buff) do
		self.__temp.item[type][tostring(k)] = v
	end
end

--获取物品栏已经使用的物品格子数
function clsUser:GetItemGridNum(type)
	return table.size(self.__temp.item[type]) or 0
end

--判断是否还能存放某个物品，并返回可存放的最大数量
function clsUser:IsItemCanAdd(type, item)
	local nGrid = 0
	for k, v in pairs(self.__temp.item[type]) do
		if v:IsSameItem(item) then
			nGrid = nGrid + GetItemUpLimit(item:GetId()) - v:GetNum()
		end
	end
	nGrid = nGrid + (self:GetGridMax(type) - self:GetItemGridNum(type)) * GetItemUpLimit(item:GetId())
	if nGrid <= 0 then
		return false
	else
		return true, nGrid
	end
end

--获取玩家背包中所有的物品对象
function clsUser:GetAllItemList(type)
	return self.__temp.item[type]
end

function clsUser:GetUserItemByClass(type, class)
	local buf = {}
	for k, v in pairs(self:GetAllItemList(type)) do 
		if v:GetClass() == class then 
			table.insert(buf, v)
		end
	end
	return buf
end

--判断物品是否满格
function clsUser:IsBagFull(type)
	return self:GetItemGridNum(type) >= self:GetGridMax(type)
end

function clsUser:SellItem(type, areaId, num)
	if type ~= PACKAGE then
		return false, "只能出售背包中的物品。"
	end
	local item = self:GetItemByAreaId(type, areaId)
	if not item then
		return false, "没有指定的物品信息。"
	end
	local info = BASEITEM.GetItemById(item:GetId())
	if not info.recycleGold then
		return false, "该道具不能出售。"
	end
	if item:GetNum() < num then
		return false, "指定的物品数量不足。"
	end
	if item:GetMissionId() then
		return false, "任务物品不能出售"
	end
	local price = num * info.recycleGold
	self:DelItemByAreaId(type, areaId, num)
	self:SetGold(price)
	local newItem = table.deepcopy(item)
	newItem:SetNum(num)
	self:AddItem(BUYBACK, newItem)
	return price, newItem:GetId()
end

function clsUser:ClearMissionItem(missionId)
	local function clear_item(type)
		for k, v in pairs(self:GetAllItemList(type)) do
			if v:GetMissionId() == missionId then
				self:DelItemByAreaId(type, k, v:GetNum())
			end
		end
	end
	clear_item(PACKAGE)
	clear_item(DEPOT)
end

---------------------------------------------------------------------------------------------------------------------------
--资金
function clsUser:SetGold(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.gold = math.floor(self.__save.gold + data)
	if self.__save.gold <= 0 then
		self.__save.gold = 0	
	end
	if not flag then
		self:SyncData("gold")
	end
end

function clsUser:GetGoldMax()
	local tribe = self:GetTribe()
	if tribe then
		return tribe:GetResourceStat("gold", "max")
	end
	return INIT_GOLD_STORAGE
end

function clsUser:GetGold()
	return self.__save.gold or 0
end
--元宝
function clsUser:SetIngot(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.ingot = math.floor(self.__save.ingot + data)
	if self.__save.ingot < 0 then
		self.__save.ingot = 0
	end
	if not flag then
		self:SyncData("ingot")
	end
end

function clsUser:GetIngot()
	return self.__save.ingot or 0
end
--灵气
function clsUser:SetNimbus(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.nimbus = math.floor(self.__save.nimbus + tonumber(data))
	if self.__save.nimbus < 0 then
		self.__save.nimbus = 0
	end
	if not flag then
		self:SyncData("nimbus")
	end
end

function clsUser:GetNimbus()
	return self.__save.nimbus or 0
end
--礼券
function clsUser:SetVoucher(data, flag)
	data = assert(tonumber(data))
	if data == 0 then return end
	self.__save.voucher = self.__save.voucher + data
	if self.__save.voucher < 0 then
		self.__save.voucher = 0
	end
	if not flag then
		self:SyncData("voucher")
	end
end

function clsUser:GetVoucher()
	return self.__save.voucher
end

--晶石
function clsUser:SetSpar(data)
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	tribe:ModResourceStat("spar", "num", data)
end

function clsUser:GetSpar()
	local tribe = self:GetTribe()
	if not tribe then
		return 0
	end
	return tribe:GetResourceStat("spar", "num")
end



function clsUser:GetTotalIngot(isOnlyIngot)
	local voucher = self:GetVoucher()
	local ingot = self:GetIngot()
	if isOnlyIngot then
		return ingot
	end
	return voucher + ingot
end

--玩家消费元宝（先扣除绑定，后扣除不绑定）
function clsUser:SetTotalIngot(newIngot)
	local voucher = self:GetVoucher()
	if newIngot >= 0 then
		self:SetVoucher(newIngot)
	else
		local tempInfo = -newIngot
		if voucher >= tempInfo then
			self:SetVoucher(-tempInfo)
		else
			self:SetVoucher(-voucher)
			self:SetIngot(voucher - tempInfo)
		end
	end
end

--初始化精力值
function clsUser:InitVigour()
	if not self.__save.vigour then
		self.__save.vigourMax = INIT_VIGOUR_MAX
	end
	self.__save.vigour = self.__save.vigourMax
	self:SyncData("vigour")
end

function clsUser:SetVigourMax(data)
	data = assert(tonumber(data))
	self.__save.vigourMax = self.__save.vigourMax + data
end

function clsUser:GetVigourMax()
	return self.__save.vigourMax
end

function clsUser:GetVigour()
	return self.__save.vigour
end

function clsUser:SetVigour(data, flag)
	data = assert(tonumber(data))
	local former = self.__save.vigour
	local vigourBuffer = self:GetBufferEffect("21")
	if data < 0 and vigourBuffer then
		if vigourBuffer.left >= -data then
			BUFFER.ModBufferEffectNum(self, "21", data)
		else
			BUFFER.ModBufferEffectNum(self, "21", -vigourBuffer.left)
			self.__save.vigour = self.__save.vigour + (data + vigourBuffer.left)
		end
	else
		self.__save.vigour = self.__save.vigour + data
	end
	if self.__save.vigour < 0 then
		self.__save.vigour = 0
	end
	self:SyncData("vigour")
	if data < 0 then
		self:SetVigourUsed(-data)
	end
end

function clsUser:GetVigourPrizeTimes()
	self.__save.use = self.__save.use or {}
	if not self.__save.use.timestamp or CalDiffDays(self.__save.use.timestamp, os.time()) ~= 0 then
		self.__save.use.vigour = 0
		self.__save.use.times = 0
	end
	return self.__save.use.times
end

function clsUser:SetVigourUsed(data)
	self.__save.vigourStat = self.__save.vigourStat or {}
	self.__save.vigourStat.used = self.__save.vigourStat.used or {}
	local info = self.__save.vigourStat.used
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
		info.prizeGot = 0
	else
		info.num = info.num + data
		if info.num >= 100 and info.prizeGot ~= 1 then
			self:SendVigourConsumePrize()
			info.prizeGot = 1
		end
	end
	info.time = os.time()
end

--消耗100精力赠送
function clsUser:SendVigourConsumePrize()
	local ATTACK_ID = 102021
	local item = BASEITEM.NewItem(ATTACK_ID, 1, {lock = 1})
	if self:IsItemCanAdd(PACKAGE, item) then
		self:AddItem(PACKAGE, item)
	else
		local mail = {title = "您获得新物品。", attach = {item = {}}}
		mail.content = "您出色的表现得到了势力的认可，获得【攻城令】×1 作为奖励。"
		table.insert(mail.attach.item, {id = ATTACK_ID, num = 1, lock = 1})
		MAIL.SendSysMail(self:GetRoleId(), mail)
	end
end

function clsUser:SetVigourBufferTimestamp(time)
	self.__save.vigourStat = self.__save.vigourStat or {}
	self.__save.vigourStat.bufferTime = time or os.time()
end

function clsUser:SetVigourRestoreTimestamp(time)
	self.__save.vigourStat = self.__save.vigourStat or {}
	self.__save.vigourStat.restoreTime = time or os.time()
end

function clsUser:GetVigourRestoreTimestamp()
	self.__save.vigourStat = self.__save.vigourStat or {}
	self.__save.vigourStat.restoreTime = self.__save.vigourStat.restoreTime or os.time()
	return self.__save.vigourStat.restoreTime
end

--上线时检查是否增加精力buffer
function clsUser:LoginAddVigourBuffer()
	local tm = os.date("*t")
	if tm.hour == 12 or tm.hour == 18 then
		if self.__save.vigourStat.bufferTime then
			local temp = os.date("*t", self.__save.vigourStat.bufferTime)
			if temp.day == tm.day or temp.hour == tm.hour then
				return
			end
		end
		BUFFER.AddOneBuffer(self, "21")
		self:SetVigourBufferTimestamp(os.time())
	end
end

--[[
function clsUser:LoginRestoreVigour()
	local offlineTime = self:GetOfflineTime()
	if not offlineTime then
		return
	end
	local tm = os.date("*t")
	if tm.hour < VIGOUR_RESET_HOUR then
		tm.day = tm.day - 1
	end
	tm.hour, tm.min, tm.sec = VIGOUR_RESET_HOUR, 0, 0
	if offlineTime < os.time(tm) then
		self:InitVigour()
	end
end]]

function clsUser:LoginRestoreVigour()
	local lastTime = self:GetVigourRestoreTimestamp()
	local times = math.floor((os.time() - lastTime) / (30 * 60))
	local vigour_diff = self:GetVigourMax() - self:GetVigour()
	if vigour_diff > 0 then
		local vigour = math.min(vigour_diff, 5 * times)
		self:SetVigour(vigour)
	end
	self:SetVigourRestoreTimestamp(lastTime + times * 30 * 60)
	self:LoginAddVigourBuffer()			--上线时检查是否增加精力buffer
end


function clsUser:GetRank()
	return RANK.GetRankNoByType("show", "userLv", self:GetRoleId())
end

-------------------------------------------------------------------------------------------------------------------------
function clsUser:GetUserData()
	local sData = {}
	sData.username = self:GetUserName()
	sData.picId = self:GetModelId()
	sData.roleId = self:GetRoleId()
	sData.nick = self:GetName()
	sData.job = self:GetJob()
	sData.Lv = self:GetLv()
	sData.maxLv = self:GetMaxLv()
	sData.sex = self:GetSex()
	sData.exp = self:GetExp()
	sData.expMax = GetUpgradeExp(self:GetLv())
	sData.headId = self:GetHeadId()
	sData.goldMax = self:GetGoldMax()
	sData.gold = self:GetGold()
	sData.ingot = self:GetIngot()
	sData.voucher = self:GetVoucher()
	sData.nimbus = self:GetNimbus()
	sData.vigour = self:GetVigour()
	sData.vigourMax = self:GetVigourMax()
	sData.rank = self:GetRank()
	sData.guide = 100
	sData.guideMax = 100
	sData.vipLv = self:GetVipLv()
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.status = self:GetStatusLv()
	sData.phyle = self:GetPhyle()
	sData.unionId = self:GetUnionId() or 0
	sData.unionPos = self:GetUnionPos()
--	sData.officialId = self:GetOfficialId() or 0
	sData.teamForce = self:GetlineUpForce()
	sData.hasTribe = self:GetTribe() and 1 or 0
	
	local sceneObj
	local locate = self:GetLocateInfo()
	if locate.scene then
		sceneObj = SCENE.IsEmptyScene(locate.scene)
	end
	local backToTown = false
	if sceneObj and sceneObj:GetKind() == "tribe" then		--下线前停留在部落，如果没有进行扫荡的话，则上限后返回城镇
		local tribe = self:GetTribe()
		if tribe then
			local retreat = tribe:GetStruct("retreat")
			if not retreat or not retreat:GetMopup() then
				backToTown = true
			end
		end
	end
	if not sceneObj or backToTown then
		local lastLocate = self:GetLastLocateInfo()
		sceneObj = SCENE.GetTownScene(lastLocate and lastLocate.scene or INIT_SCENE)
		if not sceneObj then
			sceneObj = SCENE.GetTownScene(INIT_SCENE)
		end
	end
--	sData.mapId = sceneObj:GetMapId()
	sData.sceneId = sceneObj:GetId()
	sData.worldChat = self:GetSetting(SETTING.WORLD_CHAT_NO_REMIND) or 0
	sData.hearsayChat = self:GetSetting(SETTING.HEARSAY_CHAT_NO_REMIND) or 0
	sData.unionInvite = self:GetSetting(SETTING.UNION_INVITE_NO_REMIND) or 0
	sData.buyAptPill = self:GetSetting(SETTING.BUY_HERO_APT_PILL_NO_REMIND) or 0
--	sData.newMail = self:HasOfflineMail()
--	sData.posX, sData.posY = self:GetCurPos()
	return sData
end

function clsUser:GetUserGlory()
	local sData = {}
	sData.unionName = self:GetUnionName() or ""
	sData.title = self:GetCurTitle() or -1
--	sData.position = self:GetOfficialName() or ""
	sData.teamForce = self:GetlineUpForce()
	return sData
end


function clsUser:GetUserInfo()
	local sData = {}
	sData.teamForce = self:GetlineUpForce()
--	sData.officialId = self:GetOfficialId() or 0
	return sData
end

function clsUser:GetUserDetail()
	local sData = {}
	sData.roleId = self:GetRoleId()
	sData.heroId = self:GetHeroId()
	sData.Lv = self:GetLv()
	sData.job = self:GetJob()
	sData.exp = self:GetExp()
	sData.expMax = GetUpgradeExp(self:GetLv())
	sData.force = self:GetFightForce()
	sData.state = self:GetWorkState()
	sData.modelId = self:GetModelId()
	
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(self:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(self:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(self:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(self:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(self:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(self:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(self:GetTotalAttr("speed"))
	local soldierInfo = BARRACK.GetUserSoldierInfo(self)
	sData.soldierType = soldierInfo.name
	local badgeInfo = ACHV.GetBadgeInfoByLv(self:GetBadgeLv())
	sData.badgeName = badgeInfo.name
	sData.badgeAdd = badgeInfo.attr
	return sData
end

function clsUser:GetDetailAttr()
	local sData = {}
	sData.roleId = self:GetRoleId()
	sData.strength = math.ceil(self:GetTotalAttr("strength"))
	sData.intellect = math.ceil(self:GetTotalAttr("intellect"))
	sData.agility = math.ceil(self:GetTotalAttr("agility"))
	sData.captain = math.ceil(self:GetTotalAttr("captain"))
	sData.aptLv = self:GetAptLv()
	sData.jink = self:GetTotalAttr("jink")
	sData.crush = self:GetTotalAttr("crush")
	sData.fatal = self:GetTotalAttr("fatal")
	sData.harmDeep = self:GetTotalAttr("harmDeep")
	sData.physcResist = self:GetTotalAttr("physcResist")
	sData.magicResist = self:GetTotalAttr("magicResist")
	return sData
end

function clsUser:GetSimpleInfo(flag)
	local sData = {}
	sData.name = self:GetName()
	sData.heroId = self:GetRoleId()
	sData.headId = self:GetHeadId()
	sData.modelId = self:GetModelId()
	sData.Lv = self:GetLv()
	sData.job = self:GetJob()
	sData.aptLv = 5
	sData.hp = self:GetHp()
	sData.hpMax = math.ceil(self:GetTotalAttr("hpMax"))
	sData.physcAtkMin = math.ceil(self:GetTotalAttr("physcAtkMin"))
	sData.physcAtkMax = math.ceil(self:GetTotalAttr("physcAtkMax"))
	sData.physcDef = math.ceil(self:GetTotalAttr("physcDef"))
	sData.magicAtkMin = math.ceil(self:GetTotalAttr("magicAtkMin"))
	sData.magicAtkMax = math.ceil(self:GetTotalAttr("magicAtkMax"))
	sData.magicDef = math.ceil(self:GetTotalAttr("magicDef"))
	sData.speed = math.ceil(self:GetTotalAttr("speed"))
	sData.force = self:GetFightForce()
	sData.type = "user"	
	if flag then
		sData.skillList = {}
		for k, v in pairs(self:GetSkillList()) do
			if flag == 0 or flag == tonumber(k) then
				if v.id then
					table.insert(sData.skillList, {type = tonumber(k), skillId = v.id, Lv = v.Lv})
				end
			end
		end
	end
	return sData
end


local ModUserTbl = {
	Lv = "SetLv",
	gold = "SetGold",
	nimbus = "SetNimbus",
	voucher = "SetVoucher",
	ingot = "SetIngot",
	exp = "SetExp",
	strength = "ModStrength",
	intellect = "ModIntellect",
	agility = "ModAgility",
	captain = "ModCaptain",
}

function GetModUserTbl()
	return ModUserTbl
end

--修改玩家属性			--{gold = 0, exp = 0, expType = "", ...}
function clsUser:ModUserData(list)
	local syncList = {}
	for k, v in pairs(list) do
		if ModUserTbl[k] and v ~= 0 then
			if k == "exp" then
				clsUser[ModUserTbl[k]](self, v, list["expType"], 1)
			else
				clsUser[ModUserTbl[k]](self, v, 1)
			end
			table.insert(syncList, k)
		end
	end
	self:SyncData(unpack(syncList))
end

--数据刷新列表
local SyncTbl = {
	Lv = function (user)
		local sData = {}
		sData.Lv = user:GetLv()
		return sData
	end,
	hp = function(user)
		local sData = {}
		sData.hp = user:GetHp()
		sData.hpMax = math.ceil(user:GetTotalAttr("hpMax"))
		return sData
	end,
	rank = function(user)
		local sData = {}
		sData.rank = user:GetRank()
		return sData
	end,
	exp = function(user)
		local sData = {}
		sData.exp = user:GetExp()
		sData.expMax = GetUpgradeExp(user:GetLv())
		return sData
	end,
--[[	point = function(user)
		local sData = {}
		sData.leftPoint = user:GetLeftPoint()
		return sData
	end,]]
	achv = function(user)
		local sData = {}
		sData.achv = user:GetAchvPoint()
		return sData
	end,
	badge = function(user)
		local sData = {}
		sData.badge = ACHV.GetBadgeNameByLv(self:GetBadgeLv())
		return sData
	end,
	repute = function(user)
		local sData = {}
		sData.repute = user:GetRepute()
		return sData
	end,
	status = function(user)
		local sData = {}
		sData.status = user:GetStatusLv()
		return sData
	end,
	union = function(user)
		local sData = {}
		sData.unionId = user:GetUnionId() or 0
		if user:GetUnionId() then
			sData.unionPos = user:GetUnionPos()
		end
		return sData
	end,
	official = function(user)
		local sData = {}
		sData.officialId = user:GetOfficialId() or 0
		return sData
	end,
	phyle = function(user)
		local sData = {}
		sData.phyle = user:GetPhyle()
		return sData
	end,
--[[	strength = function(user)
		local sData = {}
		sData.strength = user:GetStrength()
		return sData
	end,
	intellect = function(user)
		local sData = {}
		sData.intellect = user:GetIntellect()
		return sData
	end,
	agility = function(user)
		local sData = {}
		sData.agility = user:GetAgility()
		return sData
	end,
	captain = function(user)
		local sData = {}
		sData.captain = user:GetCaptain()
		return sData
	end,]]
	gold = function(user)
		local sData = {}
		sData.gold = user:GetGold()
		return sData
	end,
	voucher = function(user)
		local sData = {}
		sData.voucher = user:GetVoucher()
		return sData
	end,
	ingot = function(user)
		local sData = {}
		sData.ingot = user:GetIngot()
		return sData
	end,
	nimbus = function(user)
		local sData = {}
		sData.nimbus = user:GetNimbus()
		return sData
	end,
	vigour = function(user)
		local sData = {}
		sData.vigour = user:GetVigour()
		return sData
	end,
	teamHp = function(user)
		local sData = {}
		sData.cur, sData.total = user:CalHpInfo()
		return sData
	end,
	name = function(user)
		local sData = {}
		sData.nick = user:GetName()
		return sData
	end,
	tribe = function(user)
		local sData = {}
		sData.hasTribe = user:GetTribe() and 1 or 0
		return sData
	end,
	vip = function(user)
		local sData = {}
		sData.vipLv = user:GetVipLv()
		return sData
	end,
}

--刷新玩家数据
function clsUser:SyncData(...)
	local sData = {}
	for _, attr in ipairs(arg) do
		if SyncTbl[attr] then
			local buff = SyncTbl[attr](self)
			for k, v in pairs(buff) do
				sData[k] = v
			end
		end
	end
	if table.size(sData) > 0 then
		self:SendProtocol("c_user_update", sData)
	end
end

--登录时运行定时器
function clsUser:LoginStartCallOut()
	local tribe = self:GetTribe()
	if tribe then
		tribe:LoginStartCallOut()
	end
	local pet = self:GetPet()
	if pet then
		pet:LoginStartCallOut()
	end
	self:StartVipCallOut()
	self:RenewDailyMission()		--上线时刷新昨日的日常任务
	self:StartCopyMopupCallOut()
	self:ProMeditate()
	self:StartBufferCallOut()
	self:StartUnLockPackageCallOut()		--自动解锁背包
	self:StartFreshEmplListCallOut()		--定时刷新可招募列表
	self:LoginStartAntiAddictCallOut()		--防沉迷定时器
	self:StartFightCDTimer()
end
--退出时移除定时器
function clsUser:ExitRemoveCallOut()
	local tribe = self:GetTribe()
	if tribe then
		tribe:OfflineRemoveCallOut()
	end
	local pet = self:GetPet()
	if pet then
		pet:OfflineRemoveCallout()
	end
	self:RemoveVipCallOut()
	self:RemoveCopyMopupCallOut()
	self:RemoveMeditateCallOut()
	self:RemoveUnLockPackageCallOut()
	self:ClearFreshEmplListCallOut()
	self:RemoveAntiAddictCallOut()
	self:RemoveFightCDTimer()
end

-------------------------------------------------------------------------------------------------------------------------
local path_heroList = "chaos.heroList"

--初始化武将数据库
function InitHeroDB()
	DATA.EnsureIndex(path_heroList, {owner = 1})		--以 owner 为关键字建立索引
end
--保存武将数据
function SaveHeroToDB(hero, call_back)
	local heroId = hero:GetId()
	local buff = hero:GetSaveData()
	buff._id = heroId
	if not DATA.UpdateData(path_heroList, {_id = heroId}, buff, call_back) then
		DATA.log(string.format("Save hero data failed: heroId %s, owner %s.", heroId, hero:GetOwner()), "user.log")
	end
end
--移除武将数据
function RmHeroFromDB(heroId)
	DATA.DBRemove(path_heroList, {_id = heroId})
end
--读取武将数据
function ReadHeroFromDB(type, match, call_back)
	if type ~= "_id" and type ~= "owner" then
		return false, "无效的查询条件。"
	end
	if not DATA.Read(path_heroList, {[type] = match}, call_back) then
		return false, "连接错误"
	end
	return true
end


function clsUser:InitHeroList()
	self.__save.heroList = {}
	self.__save.heroMax = INIT_HERO_MAX
	
	self.__temp.heroList = {}
end
--武将最大数量
function clsUser:SetHeroMax(data)
	self.__save.heroMax = self.__save.heroMax + data
end

function clsUser:GetHeroMax(data)
	return self.__save.heroMax
end
--获取武将元宝或灵气解锁次数
function clsUser:GetHeroUnLockNum(type)
	self.__save.heroUnlock = self.__save.heroUnlock or {}
	return self.__save.heroUnlock[type] or 0
end

function clsUser:SetHeroUnLockNum(type, data)
	self.__save.heroUnlock = self.__save.heroUnlock or {}
	self.__save.heroUnlock[type] = (self.__save.heroUnlock[type] or 0) + data
end
--获取武将解锁花费
function clsUser:GetHeroUnLockNeed(type)
	local num = self:GetHeroUnLockNum(type)
	num = num + 1
	if type == "ingot" then
		if num > INGOT_SLOT_NUM then return end
		return num * 10
--[[	elseif type == "nimbus" then
		if num > NIMBUS_SLOT_NUM then return end
		return num * 1000 + 5000]]
	elseif type == "auto" then
		if num > AUTO_SLOT_NUM then return end
		return 10 + 5 * num
	end
end

function clsUser:GetHeroSlotMax()
	return INGOT_SLOT_NUM + INIT_HERO_MAX
end

--武将招募列表
function clsUser:SetHeroFreshList(data)
	assert(type(data or {}) == "table")
	self.__save.heroFreshList = data
end

function clsUser:GetHeroFreshList()
	return self.__save.heroFreshList or {}
end

function clsUser:GetHero(heroId)
	return self.__temp.heroList[heroId]
end

function clsUser:SetHeroExp(heroId, data, type)
	local hero = self.__temp.heroList[heroId]
	if hero then
		hero:SetExp(data, type)
	end
end

function clsUser:SetHeroState(heroId, state, place, startTime, endTime)
	local hero = self:GetHero(heroId)
	if hero then
		hero:SetWorkInfo(state, place, startTime, endTime)
	end
end

--招募一名武将
function clsUser:AddHero(type, heroData)
	local heroId
	local heroObj
	if type == "godHero" then
		heroId = STATS.GenId("godHero")		--神将
		heroObj = GODHERO.clsGodHero:New()
	else
		heroId = STATS.GenId("hero")
		if type == "famous" then
			heroObj = FAMOUSHERO.clsHero:New()
		else
			heroObj = CLSHERO.clsHero:New()
		end
	end
	heroObj:InitHero(type, heroId, self:GetRoleId(), heroData)
	self.__temp.heroList[heroId] = heroObj
	heroObj:FreshTotalAttr(true)
	self:CheckAchvState("heroNum")
	self:CheckAchvState("famousHero")
	return heroObj
end
--通过保存数据来增加一名武将
function clsUser:AddHeroBySave(saveData)
	local heroId
	local heroObj
	if saveData.type == "godHero" then
		heroObj = GODHERO.clsGodHero:New()
		heroId = STATS.GenId("godHero")		--神将
	else		
		heroId = STATS.GenId("hero")
		if saveData.type == "famous" then
			heroObj = FAMOUSHERO.clsHero:New()
		else
			heroObj = CLSHERO.clsHero:New()
		end
	end
	heroObj:Restore(saveData)
	heroObj:SetId(heroId)
	heroObj:SetOwner(self:GetRoleId())
	self.__temp.heroList[heroId] = heroObj
	heroObj:FreshTotalAttr(true)
	self:CheckAchvState("heroNum")
	self:CheckAchvState("famousHero")
end

--删除一名武将
function clsUser:DelHero(heroId)
	local hero = self:GetHero(heroId)
	if not hero then
		return
	end
	hero:Dimission()
	hero:Destroy()
	self.__temp.heroList[heroId] = nil
	RmHeroFromDB(heroId)
end

function clsUser:GetHeroList()
	return self.__temp.heroList
end

function clsUser:GetHeroNumByAptLv(aptLv, match)
	local aptInfo = CLSHERO.GetHeroAptInfoByLv(aptLv)
	local num = 0
	for k, v in pairs(self:GetHeroList()) do
		local aptitude = v:GetAptitude()
		if ((not match or match == "=") and aptitude == aptInfo.atp.min) or (match == ">" and aptitude >= aptInfo.atp.min) or (match == "<" and aptitude <= aptInfo.atp.min) then
			num = num + 1
		end
	end
	return num
end

function clsUser:SaveHeroData()
	self.__save.heroList = {}
	for k, v in pairs(self.__temp.heroList) do
		SaveHeroToDB(v)
		table.insert(self.__save.heroList, k)
	end
end

function clsUser:RestoreHeroData()
	self.__temp.heroList = {}	
	local function call_back(data, msg)
		data = UnSerialize(data)
		if not data then
			DATA.log(string.format("Restore hero data failed: owner %s.", self:GetRoleId()), "user.log")
			return
		end
		for k, v in pairs(data) do
			local heroObj
			if v.type == "godHero" then
				heroObj = GODHERO.clsGodHero:New()
			else
				heroObj = CLSHERO.clsHero:New()
			end
			heroObj:Restore(v)
			self.__temp.heroList[v.id] = heroObj
		end
	end
	ReadHeroFromDB("owner", self:GetRoleId(), call_back)
end


--名将招募次数
function clsUser:GetFamousHeroFreshMax()
--	local vipLv = self:GetVipLv()
--	local times = VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_FAMOUS_FRESH)
--	return 1 + (times or 0)
	return 1
end

--type:adv 高级招募/fam 名将招募/dir 直接招募
function clsUser:GetEmployeeTimes(type)
	self.__save.employee = self.__save.employee or {}
	self.__save.employee[type] = self.__save.employee[type] or {}
	if CalDiffDays(os.time(), (self.__save.employee[type].timestamp or 0)) ~= 0 then
		return 0
	end
	return self.__save.employee[type].times
end

function clsUser:SetEmployeeTimes(type, data)
	self.__save.employee = self.__save.employee or {}
	self.__save.employee[type] = self.__save.employee[type] or {}
	if CalDiffDays(os.time(), (self.__save.employee[type].timestamp or 0)) ~= 0 then
		self.__save.employee[type].times = 0
	end
	self.__save.employee[type].times = math.max(self.__save.employee[type].times + data, 0)
	self.__save.employee[type].timestamp = os.time()
end

function clsUser:SetHeroFreshTimestamp(data)
	self.__save.heroFreshTime = data
end

function clsUser:GetHeroFreshTimestamp()
	return self.__save.heroFreshTime or os.time()
end

--启动刷新招募列表定时器
function clsUser:StartFreshEmplListCallOut()
	local timelong = self:GetHeroFreshTimestamp() - os.time()
	if timelong <= 0 then
		HERO.FreeFreshEmplList(self)
	else
		self.__temp.heroFreshTimer = CALLOUT.CallOut(HERO.FreeFreshEmplList, timelong, self)
	end
end

function clsUser:ClearFreshEmplListCallOut()
	if self.__temp.heroFreshTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.heroFreshTimer)
	end
end

-----------------------------------------------------------------------------
function clsUser:AddPet(id)
	local pet = self:GetPet()
	if pet then
		pet:Destroy()
	end
	self.__temp.pet = PET.NewPet(self:GetRoleId(), id)
end

function clsUser:GetPet()
	return self.__temp.pet
end

function clsUser:SendPetPrompt(type, msg)
	local pet = self:GetPet()
	pet:SendPetPrompt(type, msg)
end

function clsUser:SetPetPrompt(type, msg)
	local pet = self:GetPet()
	pet:SetPetPrompt(type, msg)
end

function clsUser:SavePetData()
	local pet = self:GetPet()
	if pet then
		self.__save.pet = pet:GetSaveData()		
	end
end

function clsUser:RestorePetData()
	local petData = self.__save.pet
	if petData then
		local pet = CLSPET.clsPet:New()
		pet:Restore(petData)
		self.__temp.pet = pet
	end
end
-------------------------------------------------------------------------------------------------------------------------




-------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------

function clsUser:GetCallOut()
	return self.__temp.callout
end

--角色收益指数(防沉迷系统 antiAddict)
function clsUser:SetIncomingRate(data)
	self.__save.incomeRate = data
end

function clsUser:GetIncomingRate()
	return self.__save.incomeRate or 1
end

--------------------------------------------------------------------------------------
function clsUser:InitTribe()
	self:InitBuildingUpgradeList()
	local tribe = TRIBE.clsTribe:New()
	self.__temp.tribe = tribe
	tribe:InitTribe(self:GetRoleId(), self:GetPhyle())
	tribe:LoginStartCallOut()
	self:SyncData("tribe")
end

function clsUser:InitBuildingUpgradeList()
	self.__temp.upgradeList["building"] = {}
	self.__temp.upgradeList["enlist"] = {}
	self.__temp.upgradeList["levy"] = {}
	self.__temp.upgradeList["academy"] = {}
end

function clsUser:GetTribe()
	return self.__temp.tribe
end

function clsUser:SetTribe(data)
	self.__temp.tribe = data
end


function clsUser:SaveTribeData()
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	self.__save.tribe = tribe:GetSaveData()
end

function clsUser:RestoreTribeData()
	if self.__save.tribe then
		local tribe = TRIBE.clsTribe:New()
		tribe:Restore(self.__save.tribe)
		self.__temp.tribe = tribe
	end
end

function clsUser:RetreatChallWin()
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	local retreat = tribe:GetStruct("retreat")
	retreat:ChallangeWin()
end

function clsUser:AddStruct(structType)
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	tribe:AddStruct(structType)
end

function clsUser:DelStruct(structType)
	local tribe = self:GetTribe()
	if not tribe then
		return
	end
	tribe:DelStruct(structType)
end

--记录需要提醒的建筑功能
function clsUser:SetStructNeedNotice(funcId, type, state)
	self.__save.structNeedNotice = self.__save.structNeedNotice or {}
	table.insert(self.__save.structNeedNotice, {funcId = funcId, type = type, state = state})
end

function clsUser:GetStructNeedNotice()
	self.__save.structNeedNotice = self.__save.structNeedNotice or {}
	return self.__save.structNeedNotice
end

function clsUser:ClearStructNeedNotice()
	self.__save.structNeedNotice = {}
end

--------------------------------------------------------------------------------------
--炼器界面	/ 仙器背包	/	炼器装备
FAIRY_PANEL = 1		--炼器界面
FAIRY_BAG = 2		--仙器背包
FAIRY_EQUIP = 3		--炼器装备
local FairyDevBagList = {[FAIRY_PANEL] = 1, [FAIRY_BAG] = 1, [FAIRY_EQUIP] = 1}

function clsUser:InitFairyDev()
	self.__save.fairyDev = {}
	self.__temp.fairyDev = {}
	self.__temp.fairyDev.list = {}
	for k in pairs(FairyDevBagList) do
		self.__temp.fairyDev.list[k] = {}
	end
	self.__save.fairyDev.debris = 0
	self.__save.fairyDev.bagMax = INIT_FAIRY_BAG_GRID
	self.__save.fairyDev.equipMax = 0
	self.__save.fairyDev.tripod = 1
--	self.__save.fairyDev.tripod[1] = 1
end


--获取仙器列表
function clsUser:GetFairyDevList(type)
	return self.__temp.fairyDev.list[type]
end

function clsUser:SendFairyDevUpdate(type, pos, kind)
	local sData = {type = type, pos = pos}
	local devObj = self:GetFairyDevByPos(type, pos)
	if not devObj then
		sData.num = 0
	else
		sData.exp = devObj:GetExp()
		sData.expMax = devObj:GetNeedExp()
--		sData.addExp = devObj:GetAddExp()
		sData.Lv = devObj:GetLv()
	end
	self:SendProtocol("c_fairy_update", sData)
end

function clsUser:SendNewFairyDev(type, pos)
	local devObj = self:GetFairyDevByPos(type, pos)
	local sData = devObj:GetFullAttr()
	sData.pos = pos
	sData.type = type
	sData.price = nil
	self:SendProtocol("c_fairy_new", sData)
end
--仙器附加属性
function clsUser:GetFairyDevAttrAdd()
	local sData = {single = {}, range = {}}
	local skillId, skillLv = nil, 0
	local pet = self:GetPet()
	if pet then
		skillId = pet:GetSkillId()
	end
	for k, v in pairs(self:GetFairyDevList(FAIRY_EQUIP) or {}) do
		if v:IsRange() then
			for key, val in pairs(v:GetAttrList()) do
				sData.range[key] = (sData.range[key] or 0) + val
			end
		else
			for key, val in pairs(v:GetAttrList()) do
				sData.single[key] = (sData.single[key] or 0) + val
			end
		end
		local info = v:GetSkill()
		if info and info.id == skillId then
			skillLv = skillLv + info.lv
		end
	end
	return sData, skillLv
end

--获取指定仙器
function clsUser:GetFairyDevByPos(type, pos)
	return self.__temp.fairyDev.list[type][pos]
end

function clsUser:AddFairyDev(type, obj, flag)
	local list = self:GetFairyDevList(type)
	for i = 1, self:GetFairyGridMax(type) do
		local pos = tostring(i)
		if not self:GetFairyDevByPos(type, pos) then
			self:AddFairyDevByPos(type, pos, obj, flag)
			return pos
		end
	end
	return false
end

function clsUser:DelFairyDevByPos(type, pos, flag)
	self.__temp.fairyDev.list[type][pos] = nil
	if not flag then
		self:SendFairyDevUpdate(type, pos)
	end
end

function clsUser:AddFairyDevByPos(type, pos, obj, flag)
	self.__temp.fairyDev.list[type][pos] = obj
	if not flag then
		self:SendNewFairyDev(type, pos)
	end
end
--装备仙器
function clsUser:WearFairyDev(targetPos, pos)
	local fairyDev = self:GetFairyDevByPos(FAIRY_BAG, pos)
	if not fairyDev then
		return false, "指定的背包位置没有仙器。"
	end
	if not targetPos then
		if not self:AddFairyDev(FAIRY_EQUIP, fairyDev) then
			return false, "没有空余位置可以装备仙器。"
		else
			self:DelFairyDevByPos(FAIRY_BAG, pos)
			self:FreshTeamTotalAttr()
			return true
		end
	end
	self:DelFairyDevByPos(FAIRY_BAG, pos)
	local devObj = self:GetFairyDevByPos(FAIRY_EQUIP, targetPos)
	if devObj then
		self:AddFairyDev(FAIRY_BAG, devObj)
	end
	self:AddFairyDevByPos(FAIRY_EQUIP, targetPos, fairyDev)
	self:FreshTeamTotalAttr()
	return true
end

function clsUser:UnfixFairyDev(targetPos, pos)
	local devObj = self:GetFairyDevByPos(FAIRY_EQUIP, targetPos)
	if not devObj then
		return false, "指定的位置没有装备仙器。"
	end
	if not pos then
		self:AddFairyDev(FAIRY_BAG, devObj)
	else
		if self:GetFairyDevByPos(FAIRY_BAG, pos) then
			return false, "指定的背包位置已经有仙器了。"
		end
		self:AddFairyDevByPos(FAIRY_BAG, pos, devObj)
	end
	self:DelFairyDevByPos(FAIRY_EQUIP, targetPos)
	self:FreshTeamTotalAttr()
	return true
end

--碎片数量
function clsUser:SetDebris(data)
	self.__save.fairyDev.debris = math.max((self.__save.fairyDev.debris or 0) + data, 0)
end

function clsUser:GetDebris()
	return self.__save.fairyDev.debris or 0
end

--炼器鼎
function clsUser:SetCurTripod(index)
--	self.__save.fairyDev.tripod[index] = state
	self.__save.fairyDev.tripod = index
end

function clsUser:GetTripodState(index)
--	return self.__save.fairyDev.tripod[index]
	index = tonumber(index)
--	if not self.__save.fairyDev.tripod then
--		self.__save.fairyDev.tripod = 1
--	end
	return self.__save.fairyDev.tripod == index
end

function clsUser:ResetTripodState()
	self.__save.fairyDev = nil
end

function clsUser:GetTripodList()
	return self.__save.fairyDev.tripod
end

function clsUser:GetCurTripod()
	return self.__save.fairyDev.tripod
end

function clsUser:GetFairyDevGridUnlockAdd()
	return FAIRY_GRID_UNLOCK_PRICE_ADD
end

function clsUser:GetFairyGridMax(type, flag)
	if type == FAIRY_PANEL then
		return MAX_FAIRY_PANEL_GRID
	elseif type == FAIRY_BAG then
	--	if not flag then
	--		return self.__save.fairyDev.bagMax or INIT_FAIRY_BAG_GRID
	--	else
			return MAX_FAIRY_BAG_GRID
	--	end
	elseif type == FAIRY_EQUIP then
		if not flag then
			return self.__save.fairyDev.equipMax or 1
		else
			return MAX_FAIRY_EQUIP_GRID
		end
	end
end

function clsUser:SetFairyGridMax(type, data)
	if type == FAIRY_BAG then
		self.__save.fairyDev.bagMax = (self.__save.fairyDev.bagMax or INIT_FAIRY_BAG_GRID) + data
		if self.__save.fairyDev.bagMax > MAX_FAIRY_BAG_GRID then
			self.__save.fairyDev.bagMax = MAX_FAIRY_BAG_GRID
		elseif self.__save.fairyDev.bagMax < INIT_FAIRY_BAG_GRID then
			self.__save.fairyDev.bagMax = INIT_FAIRY_BAG_GRID
		end
	elseif type == FAIRY_EQUIP then
		self.__save.fairyDev.equipMax = (self.__save.fairyDev.equipMax or 0) + data
		if self.__save.fairyDev.equipMax > MAX_FAIRY_EQUIP_GRID then
			self.__save.fairyDev.equipMax = MAX_FAIRY_EQUIP_GRID
		elseif self.__save.fairyDev.equipMax < 1 then
			self.__save.fairyDev.equipMax = 1
		end
	end
end

function clsUser:SetFairyEquipUnlockNum()
	local Lv = self:GetLv()
	self.__save.fairyDev.equipMax = 1 + math.floor(Lv / 10)
end

function clsUser:IsFairyBagFull(type)
	for i = 1, self:GetFairyGridMax(type) do
		if not self:GetFairyDevByPos(type, tostring(i)) then
			return false
		end
	end
	return true
end

function clsUser:CalFairyGridUnLockPrice(type, num)
	if num <= 0 then return 0 end
	local nGrid = self:GetFairyGridMax(type)
	local begin
	if type == FAIRY_BAG then
		local curNum = (nGrid - INIT_FAIRY_BAG_GRID + 1)
		begin = FIRST_FAIRY_GRID_COST + (curNum - 1) * FAIRY_GRID_UNLOCK_PRICE_ADD
	end
	return num * begin + num * (num - 1) * FAIRY_GRID_UNLOCK_PRICE_ADD / 2
end

function clsUser:GetDevourCort()
	return self.__temp.devourCort
end

function clsUser:SetDevourCort(co)
	self.__temp.devourCort = co
end

function clsUser:SaveFairyDevData()
	if not self.__temp.fairyDev.list then
		return
	end
	self.__save.fairyDev.list = {}
	for type, list in pairs(self.__temp.fairyDev.list) do
		self.__save.fairyDev.list[type] = {}
		for k, v in pairs(list) do
			self.__save.fairyDev.list[type][k] = v:GetSaveData()
		end
	end
end

function clsUser:RestoreFairyDevData()
	if not self.__save.fairyDev then
		return
	end
	self.__temp.fairyDev.list = {}
	for type, list in pairs(self.__save.fairyDev.list or {}) do
		self.__temp.fairyDev.list[type] = {}
		for k, v in pairs(list) do
			local fairyDevObj = BASEITEM.NewItem(v.id, v.num)
			if fairyDevObj then
				fairyDevObj:Restore(v)
				self.__temp.fairyDev.list[type][k] = fairyDevObj
			end
		end
	end
end
--仙器融合
function clsUser:GetMergeFairyDev()
	return self.__temp.fairyDev.fairyMerge
end

function clsUser:SetMergeFairyDev(obj, quality)
	if not obj then
		self.__temp.fairyDev.fairyMerge = nil
	else
		self.__temp.fairyDev.fairyMerge = {fairyDev = obj, quality = quality}
	end	
end

--------------------------------------------------------------------------------------
function clsUser:GetRoleType()
	return "user"
end

function clsUser:GetHeroId()
	return "user"
end

function clsUser:GetCurPos()
	local info = self.__save.locate
	return info.x or INIT_POS_X, info.y or INIT_POS_Y
end

function clsUser:GetAOI()
	return USER_SIGHT_WIDTH
end

function clsUser:SendProtocol(pro, data)
	local roleId = self:GetRoleId()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol(pro, data)
	end
end

function clsUser:OnEnterAOI(target)
	-- Target进入我的视野
	local scene = self:GetScene()
	if not scene then
		return
	end
	if not self:IsSceneEnterDone() then
		return
	end
	local buff = {}
	local type = target:GetRoleType()
	buff.roleType = type
	buff.posX, buff.posY = target:GetCurPos()
	buff.nick = target:GetName()
	buff.roleId = target:GetRoleId()
	buff.picId = target:GetModelId()
	buff.headId = target:GetHeadId()
	buff.title = target:GetCurTitle()
	if type == "user" then
		buff.Lv = target:GetLv()
		buff.action = target:GetPose()
		local pet = target:GetPet()
		buff.petModelId = pet:GetModelId()
		buff.phyle = target:GetPhyle()
		buff.unionName = target:GetUnionName()
		buff.state = scene:GetSceneRoleState(target:GetRoleId())
	else
		if type == "foe" and target:GetType() == "boss" then
			buff.roleType = "boss"
		end
		buff.state, buff.isMaster = target:GetState(self)
	end
	self:SendProtocol("c_player_apear", buff)
end

function clsUser:OnLeftAOI(target)
	-- Target离开我的视野
	local scene = self:GetScene()
	if not scene then
		return
	end
	if not self:IsSceneEnterDone() then
		return
	end
	local targetRoleId = target:GetRoleId()
	self:DelUserInSight(targetRoleId)
	self:SendProtocol("c_role_disapear", targetRoleId)
end

function clsUser:SetSceneEnterDone(state)
	self.__temp.sceneEnter = state
end

function clsUser:IsSceneEnterDone()
	return self.__temp.sceneEnter
end

function clsUser:IsHideOther()
	return self:GetSetting(SETTING.SHOW_SCENE_USER_NUM)
end

--设置场景同屏人数(0显示全部 1只显示自己 >1显示指定数目)
function clsUser:SetHideOther(data)
	self:SetSetting(SETTING.SHOW_SCENE_USER_NUM, data)
end

function clsUser:InitUserInSight(flag)
	if flag then
		if self:GetSetting(SETTING.SHOW_SCENE_USER_NUM) == 1 then
			self:SetSetting(SETTING.SHOW_SCENE_USER_NUM, 0)
		end
	end
	self.__temp.userInSight = {}
	local num = self:IsHideOther()
	if not num or type(num) == "boolean" then
		num = 1
	end
	local count = 0	
	local scene = self:GetScene()
	for _, roleId in pairs(self:GetNeighbor()) do
		local target = scene:GetSceneRole(roleId)
		if target and target:GetRoleType() == "user" then
			self.__temp.userInSight[roleId] = true
			count = count + 1
			if count >= num then
				break
			end
		end
	end
end

--判断玩家是否可以出现在自己的视野范围
function clsUser:IsUserInSight(roleId)
	if not self:IsHideOther() then
		return true
	end
	return self.__temp.userInSight[roleId]
end

function clsUser:AddUserInSight(roleId)
	local num = self:IsHideOther()
	if not num or num <= 1 then return end
	if table.size(self.__temp.userInSight) < num then
		self.__temp.userInSight[roleId] = true
	end
end

function clsUser:DelUserInSight(roleId)
	self.__temp.userInSight[roleId] = nil
end


--在玩家身上挂一个npc对话的 coroutine
function clsUser:SetNpcCort(co)
	self.__temp.npcCort = co 
end

function clsUser:GetNpcCort()
	return self.__temp.npcCort
end

---------------------------------------------------------

function clsUser:IsFuncOpen(id)
	id = tostring(id)
	self.__save.funcOpen = self.__save.funcOpen or {}
	return self.__save.funcOpen[id]
end

function clsUser:SetFuncOpen(id)
	id = tostring(id)
	self.__save.funcOpen = self.__save.funcOpen or {}
	self.__save.funcOpen[id] = 1
end

-------------------------------------------------------------------
--功能使用次数统计
function clsUser:InitFuncStat()
	self.__save.funcStat = self.__save.funcStat or {}
end
--funcStat (times:次数 now:最新记录时间 last:最新领取奖励时间 canFind:是否可以找回昨天奖励)
function clsUser:SetFuncStatById(funcId, data)
	local id = tostring(funcId)
	self.__save.funcStat[id] = self.__save.funcStat[id] or {}
	local info = self.__save.funcStat[id]
	if not info or not info.now then
		info.times = data
		info.last = os.time() - 24 * 3600
	else
		if CalDiffDays(info.now, os.time()) ~= 0 then
			info.times = data
			info.canFind = nil
		else
			info.times = info.times + data
		end
	end
	info.now = os.time()
	self:CheckWelfareFinish(funcId)
end

function clsUser:GetFuncStatById(funcId)
	local id = tostring(funcId)
	self.__save.funcStat[id] = self.__save.funcStat[id] or {}
	local info = self.__save.funcStat[id]
	if not info or not info.now then
		info.times = 0
		info.last = os.time() - 24 * 3600
	else
		if CalDiffDays(info.now, os.time()) ~= 0 then
			info.times = 0
			info.canFind = nil
		end
	end
	info.now = os.time()
	return info
end
----------------------------------------------------------
--左侧状态队列
local SimpleStatList = {
	[SETTING.FUNC_ARENA] = 1, [SETTING.FUNC_TEAM_COPY] = 1, [SETTING.FUNC_PHYLE_CHOOSE] = 1,
	[SETTING.FUNC_TRIBE] = 1, [SETTING.FUNC_ST_ACADEMY] = 1, 
}

function clsUser:FreshSimpleStatList(funcId)
	if not SimpleStatList[funcId] then
		return
	end
	local roleId = self:GetRoleId()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		PROTOCOL.s_tribe_upgradeList(con, "get")
	end
end

function clsUser:GetUpgradeTimestamp(type, index)
	local obj = self:GetOneUpgradeList(type, index)
	local tribe = self:GetTribe()
	local times, timelong, maxTimes, state
	if not obj then
		timelong = 0
	else
		timelong = math.max(obj:GetEndTimestamp() - os.time(), 0)
		state = obj:GetState()
	end
	if type == "levy" then
		local hall = tribe:GetStruct("mainHall")
		times = hall:GetLevyCount()
		maxTimes = hall:GetLevyMaxTimes()
	elseif type == "enlist" then
		local barrack = tribe:GetStruct("barrack")
		times = barrack:GetEnlistCount()
		maxTimes = barrack:GetEnlistMax()
	elseif type == "arena" then
		times = self:GetArenaTimes()
		maxTimes = ARENA.GetMaxArenaTimes()
		state = UPGRADELIST.LIST_UNLOCK
	elseif type == "teamcopy" then
		times = self:GetDailyTeamCopyNum()
		maxTimes= self:GetDailyTeamCopyMax()
		state = UPGRADELIST.LIST_UNLOCK
	elseif type == "pillage" then
		times = self:GetPillageTimes() 
		maxTimes = self:GetMaxPillageTimes()
		state = UPGRADELIST.LIST_UNLOCK
	elseif type == "cityAttack" then
		times = self:GetCityAttackTimes()
		state = UPGRADELIST.LIST_UNLOCK
	end
	return timelong, times, maxTimes, state
end

function clsUser:SendUpgradeListUpdate(type, index)
	local sData = {index = index, type = type}
	local timelong, times, maxTimes, state = self:GetUpgradeTimestamp(type, index)
	sData.state = state
	sData.timelong = timelong
	sData.times = times
	sData.maxTimes = maxTimes
	self:SendProtocol("c_upgrade_update", sData)
end

function clsUser:GetSLGInfo()
	local sData = {}
	local cityId = self:GetCityId()
	local city = CITY.GetCityObjById(cityId)
	if city then
--		sData.cityId = cityId
		sData.picId = city:GetPicId()
		sData.cityName = city:GetName()
		sData.Lv = city:GetLv()
		sData.identity = city:GetOwner() == self:GetRoleId() and CLSCITY.ID_MASTER or CLSCITY.ID_ATTACH
--[[		sData.locate = city:GetLocation()
		local skillId = city:GetLinkSkill()
		local skillLv = city:GetLinkSkillLv()
		if skillLv > 0 then
			sData.link = SKILL.GetSkillNameById(skillId)
		end
		local godHero = self:GetGodHero()
		if godHero then
			sData.godHero = godHero:GetName()
		end]]
	end
	local tribe = self:GetTribe()
	if tribe then
--[[		local mainHall = tribe:GetStruct("mainHall")
		local market = tribe:GetStruct("market")
		local spar = tribe:GetStruct("spar")
		local house = tribe:GetStruct("house")
		local barrack = tribe:GetStruct("barrack")
		local embassy = tribe:GetStruct("embassy")]]
		sData.gold = self:GetGold()
		sData.goldSpd = tribe:GetResourceStat("gold", "speed")
		sData.sparNum = tribe:GetResourceStat("spar", "num")
		sData.sparSpd = tribe:GetResourceStat("spar", "speed")
		sData.peopleNum = tribe:GetResourceStat("people", "num")
		sData.peopleSpd = tribe:GetResourceStat("people", "speed")
		sData.soldierNum = tribe:GetResourceStat("soldier", "num")
		sData.soldierSpd = tribe:GetResourceStat("soldier", "speed")
--[[		if embassy then
			sData.levy = mainHall:GetLevyGot()
		end
		if market then
			sData.money = market:GetTotalResIncomeRate()
		end
		if spar then
			sData.spar = spar:GetTotalResIncomeRate()
		end
		if house then
			sData.people = house:GetTotalResIncomeRate()
		end
		if barrack then
			sData.enlist = barrack:GetTotalResIncomeRate()
			sData.enlistMax = barrack:GetEnlistMax()
		end
		if embassy then
			sData.stayMaxNum = embassy:GetMaxStayNum()
			sData.stayNum = embassy:GetStayHeroNum()
		end]]
	end
	return sData
end

----------------------------------------------------------
function __init__()
	
	local USEREXPLIST = Import("autocode/userExpList.lua")
	UserExpList = USEREXPLIST.GetTable()
end

