
local MainCityList = {["phoenix"] = "611015", ["dragon"] = "610013", ["tiger"] = "612015"}
local CityStateList = {["normal"] = 1, ["wait"] = 1, ["protect"] = 1}
ID_MASTER = 1
ID_ATTACH = 2
local CITY_MAX_LV = 15
local CITY_MIN_LV = 1
local INIT_CITY_NAME = "城池"
local RENAME_FREE_TIMES = 1
local RENAME_NEED_INGOT = 50
local MAX_ATTACH_VIEW_NUM = 100			--显示的依附人员数量上限
local PILLAGE_INCOME_RATE = 0.05					--掠夺收益
local PILLAGE_PROTECT_TIME = 3600		--被掠夺保护时间

local INIT_FORMATION = "321002"				--初始阵型
local SAME_PHYLE_WAIT_TIME = 5 * 60			--同势力进驻等待时间
local DIFF_PHYLE_WAIT_TIME = 3 * 60			--不同势力进驻等待时间
local CITY_PROTECT_TIMELONG = 60 * 60		--不同势力入驻后的保护时间
local CITY_ATTACK_LOSS_RATE = 0.04			--城池升级进度损失比率
local MAIN_CITY_RES_ADD = 0.02				--主城资源加成
local MAIN_CITY_PIC_ID = 619006				--主城的模型id

--城池等级限制
local CityUpLimitList = {
	[6] = {Lv = 9, needIngot = 50},
	[9] = {Lv = 12, needIngot = 100},
	[12] = {Lv = 15, needIngot = 150},
}

function GetMainCityByPhyle(phyle)
	return MainCityList[phyle]
end

--城池升级信息
local CityUpgradeList = {}

function GetCityUpgradeInfo(lv)
	return CityUpgradeList[lv]
end

function GetCityUpLimitInfo(lv)
	return CityUpLimitList[lv]
end

--城池静态信息
local AllCityInfoList = {}

function GetAllCityInfoList()
	return AllCityInfoList
end

function GetCityInfoById(id)
	return AllCityInfoList[id]
end

local CityFightBufferType = {}		--战斗效果（包括连携属性、策略）

function GetValidFightBufferList(type, kind)
	return CityFightBufferType[type][kind]
end

function IsFightBufferValid(type, kind, id)
	return CityFightBufferType[type][kind][id]
end

-----------------------------------------------------------
clsCity = clsObject:Inherit()

function clsCity:__init__()
	Super(clsCity).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsCity:initsavedata()
	self.__save.member = {}
	self.__save.apply = {}
	self.__save.strategies = {}
	self.__save.joinable = 1
	self.__save.Lv = 0
	self.__save.spar = 0
	
	self.__save.formation = {}
end

function clsCity:inittempdata()
	self.__temp.callout = {}
	self.__temp.tacticList = {}
	self.__temp.formation = {}
end

function clsCity:InitCity(id, owner, phyle, name, initLv, limitLv)
	local info = GetCityInfoById(id)
	self:SetId(id)
	self:SetPhyle(phyle)
	self:SetOwner(owner)
	self:SetState("normal")
	self:SetLimitLv(limitLv)
	self:SetLv(initLv)
	
	self:SetName(name or INIT_CITY_NAME)
	if self:HasGodHero() then
		self:InitGodHeroList()
	end
	self:InitTacticList()
end

function clsCity:SetId(id)
	self.__save.id = id
end

function clsCity:GetId(id)
	return self.__save.id
end

function clsCity:SetName(data)
	self.__save.name = data
end

function clsCity:GetName()
	return self.__save.name
end
--当前所属势力
function clsCity:SetPhyle(data)
	self.__save.phyle = data
end

function clsCity:GetPhyle()
	return self.__save.phyle
end
--之前所属势力
function clsCity:GetLastPhyle()
	return self.__save.lastPhyle
end

function clsCity:SetLastPhyle(data)
	self.__save.lastPhyle = data
end

function clsCity:SetOwner(data)
	self.__save.owner = data
end

function clsCity:GetOwner()
	return self.__save.owner
end


function clsCity:GetCallOut()
	return self.__temp.callout
end


function clsCity:GetMaxLv()
	return CITY_MAX_LV 
end

function clsCity:GetLimitLv()
	return self.__save.limitLv
end

function clsCity:SetLimitLv(Lv)
	self.__save.limitLv = Lv
end

function clsCity:GetLv()
	return self.__save.Lv or 1
end

function clsCity:SetLv(lv)
	local maxLv = self:GetLimitLv()
	self.__save.Lv = (self.__save.Lv or 1) + lv
	if self.__save.Lv > maxLv then
		self.__save.Lv = maxLv
	elseif self.__save.Lv < 1 then
		self.__save.Lv = 1
	end
end

--突破等级限制
function clsCity:BreakLimit()
	local lv = self:GetLv()
	local limitInfo = GetCityUpLimitInfo(lv)
	if not limitInfo then
		return
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if user:GetTotalIngot() < limitInfo.needIngot then
		return
	end
	user:SetTotalIngot(-limitInfo.needIngot)
	self:SetLimitLv(limitInfo.Lv)
end


function clsCity:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsCity:Restore(data)
	self.__save = data
end


function clsCity:GetMemberMax()
	return CityUpgradeList[self:GetLv()].num
end

function clsCity:GetMemberNum()
	return table.size(self.__save.member)
end

function clsCity:HasGodHero()
	return GetCityInfoById(self:GetId()).godHero
end


function clsCity:GetNeighber()
	return GetCityInfoById(self:GetId()).neighber
end

function clsCity:GetLocation()
--	return GetCityInfoById(self:GetId()).location
	local location = {}
	location.x = GetCityInfoById(self:GetId()).posX
	location.y = GetCityInfoById(self:GetId()).posY
	return location
end
--基本的资源加成比率
function clsCity:GetBaseResAddRate()
	if self:IsMainCity() then
		return MAIN_CITY_RES_ADD
	end
	local info = CityUpgradeList[self:GetLv()]
	if self:GetInitPhyle() == "npc" then
		return info.neutralResRate
	end
	return info.normalResRate
end
--实际的资源加成比率
function clsCity:GetTotalResAddRate()
	local baseRate = self:GetBaseResAddRate()
	local info = self:IsTacticExist("390008")
	if info then
		baseRate = baseRate + info.resRate
	end
	info = self:IsTacticExist("390009")
	if info then
		baseRate = baseRate + info.resRate
	end
	return baseRate
end

function clsCity:GetInitPhyle()
	return GetCityInfoById(self:GetId()).phyle
end

function clsCity:GetPicId()
	if self:IsMainCity() then
		return MAIN_CITY_PIC_ID
	end
	local info = GetCityUpgradeInfo(self:GetLv())
	local initType = self:GetInitPhyle() == "npc" and "npc" or "player"
	return info.picIdList[initType]
end





--捐献
function clsCity:Donate(data)
	self:SetSparNum(data)
end

function clsCity:GetSparNum()
	return self.__save.spar
end

function clsCity:SetSparNum(data)
	self.__save.spar = (self.__save.spar or 0) + data
	local need = self:GetSparMax()
	if self.__save.spar >= need then
		self.__save.spar = self.__save.spar - need
		self:Upgrade()
	elseif self.__save.spar < 0 then
		self.__save.spar = math.max(self.__save.spar + need, 0)
	--	self:Degrade()
	else
		self:SendMemberUpdate("donate")
	end
end

function clsCity:GetSparMax()
	return CityUpgradeList[self:GetLv()].needSpar
end
--城市等级提升
function clsCity:Upgrade()
	self:SetLv(1)
	local need = self:GetSparMax()
	if self:GetSparNum() >= need then
		if self:GetLv() < self:GetLimitLv() then
			self.__save.spar = self.__save.spar - need
			return self:Upgrade()
		else
			self.__save.spar = 0
		end
	end
	self:SendMemberUpdate("upgrade")
	self:SendCityUpdate("Lv")
end
--[[城市降级
function clsCity:Degrade()
	self:SetLv(-1)
	if self:GetSparNum() < 0 then
		if self:GetLv() > self:GetMinLv() then
			self.__save.spar = self.__save.spar + self:GetSparMax()
			return self:Degrade()
		else
			self.__save.spar = 0
		end
	end
	self:SendCityUpdate("Lv")
end]]

function clsCity:GetRenameTimes()
	return self.__save.renameTms or 0
end

function clsCity:SetRenameTimes(data)
	self.__save.renameTms = math.max((self.__save.renameTms or 0) + data, 0)
end

function clsCity:GetRenameCost()
	if self:GetRenameTimes() < RENAME_FREE_TIMES then
		return 0
	end
	return RENAME_NEED_INGOT	
end

function clsCity:IsJoinable()
	return self.__save.joinable == 1
end

function clsCity:SetJoinable(data)
	self.__save.joinable = data
end
--列表维护(type: member, apply)
function clsCity:AddToList(type, roleId, data)
	self.__save[type] = self.__save[type] or {}
	self.__save[type][roleId] = data
end

function clsCity:IsInList(type, roleId)
	return self.__save[type][roleId]
end

function clsCity:RmFromList(type, roleId)
	self.__save[type] = self.__save[type] or {}
	self.__save[type][roleId] = nil
end

function clsCity:GetListByType(type)
	self.__save[type] = self.__save[type] or {}
	return self.__save[type]
end

function clsCity:ModInfoInList(type, roleId, key, value)
	local info = self:IsInList(type, roleId)
	if not info then return end
	info[key] = value
end

function clsCity:AddToApplyList(user)
	local buf = {}
	buf.Lv = user:GetLv()
	buf.force = user:GetFightForce()
	buf.timestamp = os.time()
	buf.unionId = user:GetUnionId()
	buf.proTimestamp = user:GetPillageProtectTime()		--掠夺保护时间
	buf.curCityId = user:GetCityId()
	self:AddToList("apply", user:GetRoleId(), buf)
	local roleId = self:GetOwner()
	local owner = USER.GetOnlineUser(roleId)
	if owner then
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			CHAT.SendSysMsg("system", "收到新的依附申请。", {roleId})
		end
	end
end

function clsCity:FormatMemberList()
	local sData = {}
	local list = self:GetListByType("member")
	for k, v in pairs(list) do
		local buf = {}
		buf.nick = USER.GetUserNickById(k)
		if k == self:GetOwner() then
			buf.isMaster = 1
		else
			buf.inForm = self:IsMemberInLineup(k) and 1 or 0
		end
		local union = UNION.GetUnionById(v.unionId)
		if union then
			buf.union = union:GetName()
			buf.unionId = v.unionId
		else
			buf.union = ""
			buf.unionId = -1
		end
		buf.Lv = v.Lv
		local timelong =  v.proTimestamp - os.time()
		buf.proTime = timelong > 0 and timelong or 0
		table.insert(sData, buf)
	end
	table.sort(sData, function(a, b)
			if a.isMaster then return true end
			if b.isMaster then return false end
			return a.Lv > b.Lv 
		end)
	if self:IsMainCity() then
		for i = MAX_ATTACH_VIEW_NUM + 1, #sData do
			sData[i] = nil
		end
	end
	return sData
end


function clsCity:CancelApply(roleId)
	if self:IsInList("apply", roleId) then
		self:RmFromList("apply", roleId)
	end
end
--[[
function clsCity:AddToMemberList(roleId, identity, userInfo)
	local user = USER.GetOnlineUser(roleId)
	if identity == ID_MASTER then
		self:SetOwner(roleId)
	end
	if user then
		user:SetCity(self:GetId(), identity or ID_ATTACH)
		CITY.SendMyStatUpdate(user)
	else
		OFFLINE.SetOfflineData(roleId, "setCity", self:GetId(), identity or ID_ATTACH, os.time())
	end
	local buff = {timestamp = os.time()}
	if user then
		buff.unionId = user:GetUnionId()
		buff.force = user:GetFightForce()
		buff.Lv = user:GetLv()
		buff.proTimestamp = user:GetPillageProtectTime()		--掠夺保护时间
	else
		local info = userInfo or self:IsInList("apply", roleId)
		if not info then
			return
		end
		buff.Lv = info.Lv
		buff.unionId = info.unionId
		buff.force = info.force
		buff.proTimestamp = info.proTimestamp
		local oldCity = CITY.GetCityObjById(info.curCityId)
		if oldCity then
			oldCity:RmFromList("member", roleId)
		end
	end
	self:AddToList("member", roleId, buff)
	self:SendMemberUpdate("appear", roleId)
	CITY.ClearAllAttachApply(roleId)
end]]

--添加到成员列表
function clsCity:AddToMemberList(roleId, identity, userInfo)
	local user = USER.GetOnlineUser(roleId)
	if identity == ID_MASTER then
		self:SetOwner(roleId)
	end
	local buff = {timestamp = os.time()}
	if user then
		buff.unionId = user:GetUnionId()
		buff.force = user:GetFightForce()
		buff.Lv = user:GetLv()
		buff.proTimestamp = user:GetPillageProtectTime()		--掠夺保护时间
	else
		local info = userInfo or self:IsInList("apply", roleId)
		if not info then
			return
		end
		buff.Lv = info.Lv
		buff.unionId = info.unionId
		buff.force = info.force
		buff.proTimestamp = info.proTimestamp
	end
	buff.identity = identity
	self:AddToList("member", roleId, buff)
	self:SendMemberUpdate("appear", roleId)
end

--修改成员信息
function clsCity:ModMemberInList(roleId, identity)
	local info = self:IsInList("member", roleId)
	if not info then
		return
	end
	if identity == ID_MASTER then
		self:SetOwner(roleId)
	end
	info.identity = identity
	self:SendMemberUpdate("identity", roleId)
end


function clsCity:SendCityMsg(type, roleId, tRoleId)
	local locate = self:GetLocation()
	local cityId = self:GetId()
	local flag = false
	local msg = ""
	local tips
	local title = ""
	local nick = USER.GetUserNickById(tRoleId)
	if type == "apply" then
		title = "依附申请通过"
		msg = string.format("您对【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】的依附申请已经通过。", cityId, self:GetName(), locate.x, locate.y)
	elseif type == "occupancy" then
		title = "城池占领成功"
		msg = string.format("您已经成为城池【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】的城主，请及时对禁卫队进行编制。", cityId, self:GetName(), locate.x, locate.y)
	elseif type == "demise" then
		title = "城池转让信息"
		msg = string.format("【<a href=\"event:person_%s\"><u>%s</u></a>】已将城池【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】的城主之位托付于您，请及时对禁卫队进行编制。", nick, nick, cityId, self:GetName(), locate.x, locate.y)
	elseif type == "remove" then
		title = "城池脱离通知"
		msg = string.format("您已被城主【<a href=\"event:person_%s\"><u>%s</u></a>】从【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】中移除。", nick, nick, cityId, self:GetName(), locate.x, locate.y)
	elseif type == "leave" then
		title = "城池脱离通知"
		msg = string.format("您已经脱离了【%s(%d, %d)】。", self:GetName(), locate.x, locate.y)
	elseif type == "def_lose" then
		title = "城池防守失败"
		msg = string.format("人有失手马有失蹄，您在城池争夺战中不幸落败，失去了城池【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】。", cityId, self:GetName(), locate.x, locate.y)
	elseif type == "def_win" then
		title = "城池防守成功"
		msg = string.format("天上地下唯我独尊，您成功地击败了前来争夺城池的挑战者【<a href=\"event:person_%s\"><u>%s</u></a>】。", nick, nick)
	elseif type == "atk_win" then
		title = "城池争夺成功"
		msg = string.format("叱咤风云舍我其谁，恭喜你打下新的城池【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】。", cityId, self:GetName(), locate.x, locate.y)
	elseif type == "atk_lose" then
		title = "城池争夺失败"
		msg = string.format("人外有人天外有天，您在城池【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】的争夺战中不幸战败。", cityId, self:GetName(), locate.x, locate.y)
	elseif type == "refuse" then
		title = "城池申请被拒绝"
		msg = string.format("您对【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】的依附申请已被城主【<a href=\"event:person_%s\"><u>%s</u></a>】拒绝。", cityId, self:GetName(), locate.x, locate.y, nick, nick)
	elseif type == "teamLeave" and roleId ~= tRoleId then
		title = "依附变动"
		msg = string.format("禁卫队员【<a href=\"event:person_%s\"><u>%s</u></a>】已从【<a href=\"event:phyleMap_%s\"><u>%s(%d, %d)</u></a>】中脱离，请及时对禁卫队进行编制。", nick, nick, self:GetName(), self:GetName(), locate.x, locate.y)
		tips = string.format("禁卫队员【<a href=\"event:person_%s\"><u>%s</u></a>】已脱离城池，请及时对禁卫队进行编制。", nick, nick)
		flag = true
	end
	local channel = "system"
	local type = "fight"
	if flag then 
		channel = "phyle"
		type = "sys"
	end
	if roleId then
		if USER.GetOnlineUser(roleId) then
			CHAT.SendSysMsg(channel, tips or msg, {roleId})
		else
			local mail = {type = type}
			mail.title = title
			mail.content = msg
			MAIL.SendSysMail(roleId, mail)
		end
	end
end


function clsCity:RefreshInfo(type, roleId)
	local user = USER.GetOnlineUser(roleId)
	local info = self:IsInList(type, roleId)
	if info then
		info.Lv = USER.GetUserNickInfo(roleId, "Lv")
		info.unionId = USER.GetUserNickInfo(roleId, "unionId")
		if user then
			info.force = user:GetFightForce()
			info.proTimestamp = user:GetPillageProtectTime()		--掠夺保护时间
			if type == "member" then
				info.timestamp = os.time()
			elseif type == "apply" then
				info.curCityId = user:GetCityId()
			end
		end
	end
end
--拒绝申请
function clsCity:Refuse(roleId)
	self:RmFromList("apply", roleId)
	self:SendCityMsg("refuse", roleId, self:GetOwner())
end
--[[移除成员
function clsCity:RemoveMember(roleId)
	local info = self:IsInList("member", roleId)
	if not info then
		return 
	end
	local buff = {}
	buff.timestamp = os.time()
	buff.unionId = info.unionId
	buff.force = info.force
	buff.proTimestamp = info.timestamp
	buff.Lv = info.Lv
	if self:IsMemberInLineup(roleId) then
		self:SendCityMsg("teamLeave", self:GetOwner(), roleId)
	end
	self:DelUserFromAllLineup(roleId)
	self:RmFromList("member", roleId)
	self:SendMemberUpdate("disappear", roleId)
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:QuitCity()
		CITY.SendMyStatUpdate(user)
	else
		OFFLINE.SetOfflineData(roleId, "quitCity")
	end
end	]]

--从成员列表中移除
function clsCity:RemoveMember(roleId)
	local info = self:IsInList("member", roleId)
	if not info then
		return
	end
	local owner = self:GetOwner()
	if owner == roleId then
		self:SetOwner()
	else
		if self:IsMemberInLineup(roleId)  then
			self:SendCityMsg("teamLeave", owner, roleId)
		end
		self:DelUserFromAllLineup(roleId)
	end
	self:RmFromList("member", roleId)
	self:SendMemberUpdate("disappear", roleId)
end


function clsCity:GetApplyNum()
	return table.size(self:GetListByType("apply"))
end

--是否主城
function clsCity:IsMainCity()
	return self:GetId() == MainCityList[self:GetPhyle()]
end

function clsCity:GetState()
	return self.__save.state
end

function clsCity:SetState(state)
	self.__save.state = state
end

function clsCity:GetTimeStamp()
	return self.__save.timestamp or os.time()
end

function clsCity:SetTimeStamp(time)
	self.__save.timestamp = time
end

function clsCity:SetInviteTimes(data)
	self.__save.invite = self.__save.invite or {}
	if not self.__save.invite.timestamp or CalDiffDays(self.__save.invite.timestamp, os.time()) ~= 0 then
		self.__save.invite.times = 0
	end
	self.__save.invite.times = math.max(self.__save.invite.times + data, 0)
	self.__save.invite.timestamp = os.time()
end

function clsCity:GetInviteTimes()
	self.__save.invite = self.__save.invite or {}
	if not self.__save.invite.timestamp or CalDiffDays(self.__save.invite.timestamp, os.time()) ~= 0 then
		self.__save.invite.times = 0
	end
	return self.__save.invite.times
end

--连携
function clsCity:GetAllLinkCity()
	return GetCityInfoById(self:GetId()).linker or {}
end

function clsCity:GetLinkCityList()
	local list = self:GetAllLinkCity()
	local buff = {}
	for _, v in pairs(list) do
		local cityObj = CITY.GetCityObjById(v)
		if cityObj:GetPhyle() == self:GetPhyle() then
			table.insert(buff, v)
		end
	end
	return buff
end

function clsCity:GetLinkSkill()
	return GetCityInfoById(self:GetId()).linkSkill
end

function clsCity:GetLinkSkillLv()
	local buff = self:GetLinkCityList()
	return #buff
end


function clsCity:GetCityInfo(roleId)
	local sData = {}
	sData.cityId = self:GetId()
	sData.name = self:GetName()
	sData.resRate = self:GetBaseResAddRate()
	sData.phyle = self:GetPhyle()
	sData.isMain = self:IsMainCity()
	if not sData.isMain then
		sData.Lv = self:GetLv()
		sData.limitLv = self:GetLimitLv()
		local info = GetCityUpLimitInfo(self:GetLimitLv())
		if info then
			sData.breakNeed = info.needIngot
		end
	--	sData.maxLv = self:GetMaxLv()
		sData.owner = USER.GetUserNickById(self:GetOwner()) or ""
		sData.memberNum = self:GetMemberNum()
		sData.memberMax = self:GetMemberMax()
		sData.expRate = math.floor(self:GetSparNum() / self:GetSparMax() * 100)
		if self:GetOwner() == roleId then
			sData.renameCost = self:GetRenameCost()
		end
	end
	local skillId = self:GetLinkSkill()
	local skillLv = self:GetLinkSkillLv()
	if skillLv > 0 then
		sData.link = {name = SKILL.GetSkillNameById(skillId), Lv = skillLv}
	end
	if self:HasGodHero() then
		sData.godHero = 1
	end
	return sData
end

function clsCity:SetTempOwner(roleId)
	if roleId and self:GetOwner() then
		return
	end
	if roleId then
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetTempCity(self:GetId())
		else
			OFFLINE.SetOfflineData(roleId, "tempCity", self:GetId())
		end
	else
		local tempRoleId = self.__save.tempOwner
		if tempRoleId then
			local user = USER.GetOnlineUser(tempRoleId)
			if user then
				user:SetTempCity()
			else
				OFFLINE.SetOfflineData(tempRoleId, "tempCity")
			end
		end
	end
	self.__save.tempOwner = roleId
end

function clsCity:GetTempOwner()
	return self.__save.tempOwner
end

function clsCity:GetFightState()
	return self.__temp.fightState
end

function clsCity:SetFightState(data)
	self.__temp.fightState = data
end

--[[转让
function clsCity:Demise(ownerObj, roleId, oldCityId, abandonFlag)
	local owner = ownerObj:GetRoleId()
	local ownerInfo = self:IsInList("member", owner)
	if ownerInfo then
		if abandonFlag then
			self:RemoveMember(owner)
		elseif self:GetOwner() == owner then
			ownerInfo.identity = ID_ATTACH
			ownerObj:SetCity(self:GetId(), ID_ATTACH)
			ownerObj:DelStruct("godAltar")
			CITY.SendMyStatUpdate(ownerObj)
			ownerObj:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
		end
	end
	self:RemoveCallOut()
	if roleId then
		local cityObj = CITY.GetCityObjById(oldCityId)
		local userInfo = cityObj:IsInList("member", roleId)
		cityObj:DelUserFromAllLineup(roleId)
		cityObj:RmFromList("member", roleId)
		cityObj:SendMemberUpdate("disappear", roleId)
		self:SendCityMsg("demise", roleId, owner or self:GetTempOwner())
		self:AddToMemberList(roleId, ID_MASTER, userInfo)
		self:SetJoinable(1)
		self:ProCallOutTimeOut()
		local user = USER.GetOnlineUser(roleId)
		if self:HasGodHero() then
			self:InitGodHeroList()			
			if user then
				user:AddStruct("godAltar")
				user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
			else
				OFFLINE.SetOfflineData(roleId, "addStruct", "godAltar")
			end
		end
	else
		self:SetJoinable()
		self:SetState("normal")
		self:SetTimeStamp()
	end
--	self:ModMemberInList(roleId, ID_MASTER)
	self:SetOwner(roleId)
	self:SetTempOwner()
	if self:IsInList("member", owner) then
		self:SendMemberUpdate("identity", owner)
	end
	self:SendCityUpdate("owner")
	self:SendCityUpdate("state")
	self:SendMemberUpdate("owner")
	self:InitFightForm()
end

--占领
function clsCity:Occupancy(user)
	local roleId = user:GetRoleId()
	self:SetTempOwner()
	self:SetOwner(roleId)
	self:AddToMemberList(roleId, ID_MASTER)
	self:SendCityMsg("occupancy", roleId)
	self:SetJoinable(1)
	self:InitFightForm()
	self:SendCityUpdate("owner")
	self:RemoveCallOut()
	self:ProCallOutTimeOut()
	user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
	if self:HasGodHero() then
		user:AddStruct("godAltar")		--神魂祭坛
		self:InitGodHeroList()
	end
	CITY.ClearAllAttachApply(roleId)
end

function clsCity:ChangeOwner(user)
	local roleId
	local lv = 0
	for k, v in pairs(self:GetListByType("member")) do
		if v.Lv > lv and k ~= self:GetOwner() then
			roleId = k
			lv = v.Lv
		end
	end
	self:Demise(user, roleId, self:GetId(), true)
end

--遗弃
function clsCity:Abandon(user, flag)
	local roleId = user:GetRoleId()
	if roleId == self:GetOwner() then
		self:ChangeOwner(user)
		user:DelStruct("godAltar")		--神魂祭坛
		user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
	else
		self:RemoveMember(roleId)
--		self:SendCityMsg("leave", self:GetOwner(), roleId)
	end
	if not flag then
		user:SetCityQuitTime(os.time())
	end
end]]

--占领
function clsCity:Occupancy(user)
	user:QuitCity()
	local roleId = user:GetRoleId()
	user:SetCity(self:GetId(), ID_MASTER)
	self:AddToMemberList(roleId, ID_MASTER)
	self:ReInitCity(roleId)
	self:SendCityMsg("occupancy", roleId)
	user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
	if self:HasGodHero() then
		user:AddStruct("godAltar")		--神魂祭坛
		self:InitGodHeroList()
	end
	CITY.ClearAllAttachApply(roleId)
	TITLE.CheckHonorTitleRecord(user, "391101")
end

--重新初始化城池
function clsCity:ReInitCity(owner)
	self:SetTempOwner()
	self:SetOwner(owner)
	self:SetJoinable(owner and 1 or 0)
	self:InitFightForm()
	self:RemoveCallOut()
	self:ProCallOutTimeOut()
	self:SendCityUpdate("owner")
	self:SendCityUpdate("state")
	self:SendMemberUpdate("owner")
end

--转让当前城池给某个依附者 (原城主并未依附回主城)
function clsCity:DemiseCurCity(ownerObj, roleId, isLeave)
	local owner = self:GetOwner()
	self:SetOwner()
	if isLeave then
		self:RemoveMember(owner)
	else
		self:ModMemberInList(owner, ID_ATTACH)
		ownerObj:SetCity(self:GetId(), ID_ATTACH)
	end
	ownerObj:DelStruct("godAltar")
	ownerObj:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
	
	self:ReInitCity(roleId)
	if roleId then
		self:ModMemberInList(roleId, ID_MASTER)
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetCity(self:GetId(), ID_MASTER)
			user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
		else
			OFFLINE.SetOfflineData(roleId, "setCity", self:GetId(), ID_MASTER)
		end
		if self:HasGodHero() then
			self:InitGodHeroList()
			if user then				
				user:AddStruct("godAltar")				
			else				
				OFFLINE.SetOfflineData(roleId, "addStruct", "godAltar")
			end
		end
		self:SendCityMsg("demise", roleId, ownerObj:GetRoleId())
	end
end

--转让攻打下的新城池给某个依附者
function clsCity:DemiseNewCity(ownerObj, roleId, newCity)
	local info = self:IsInList("member", roleId)
	if not info then
		return
	end
	newCity:ReInitCity(roleId)
	newCity:AddToMemberList(roleId, ID_MASTER, info)
	self:RemoveMember(roleId)
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:SetCity(newCity:GetId(), ID_MASTER)
		user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
	else
		OFFLINE.SetOfflineData(roleId, "setCity", newCity:GetId(), ID_MASTER)
	end
	if newCity:HasGodHero() then
		newCity:InitGodHeroList()
		if user then
			user:AddStruct("godAltar")			
		else
			OFFLINE.SetOfflineData(roleId, "addStruct", "godAltar")
		end
	end
	newCity:SendCityMsg("demise", roleId, ownerObj:GetRoleId())
end


function clsCity:ChangeOwner(user)
	local roleId
	local lv = 0
	for k, v in pairs(self:GetListByType("member")) do
		if v.Lv > lv then
			roleId = k
			lv = v.Lv
		end
	end
	self:DemiseCurCity(user, roleId, true)
end

--主动脱离城池
function clsCity:Abandon(user)
	local roleId = user:GetRoleId()
	local isOwner = false
	if roleId == self:GetOwner() then
		isOwner = true
	end
	self:RemoveMember(roleId)
	CITY.AttachToMainCity(roleId)
	if isOwner then
		if self:HasGodHero() then
			user:DelStruct("godAltar")		--神魂祭坛
		end
		user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
		self:ChangeOwner(user)
		self:SendCityUpdate("owner")
	end
	user:SetCityQuitTime(os.time())
end

--脱离城池
function clsCity:LeaveCity(roleId)
	local isOwner = false
	if roleId == self:GetOwner() then
		isOwner = true
	end
	self:RemoveMember(roleId)
	CITY.AttachToMainCity(roleId)
	local user = USER.GetOnlineUser(roleId)
	if isOwner and self:HasGodHero() then
		if user then
			user:DelStruct("godAltar")		--神魂祭坛
		else
			OFFLINE.SetOfflineData(roleId, "delStruct", "godAltar")
		end
	end
	if user then
		if isOwner then			
			user:FreshSimpleStatList(SETTING.FUNC_PHYLE_CHOOSE)		--刷新左侧状态队列
		end
	end
end

--依附到普通城池
function clsCity:AttachToCity(roleId)
	local cityId = USER.GetUserNickInfo(roleId, "cityId")
	local cityObj = CITY.GetCityObjById(cityId)
	if cityObj then
		cityObj:RemoveMember(roleId)
	end
	self:AddToMemberList(roleId, ID_ATTACH)
	local user = USER.GetOnlineUser(roleId)
	if user then
		user:SetCity(self:GetId(), ID_ATTACH)
		CITY.SendMyStatUpdate(user)
	else
		OFFLINE.SetOfflineData(roleId, "setCity", self:GetId(), ID_ATTACH)
	end
end
------------------------------------------------------------------------------------------
--初始化神将系统
function clsCity:InitGodHeroList()
	self.__save.godHeroList = {}
end

function clsCity:SetGodHeroList(data)
	self.__save.godHeroList = data
end

function clsCity:GetGodHeroList()
	return self.__save.godHeroList
end

function clsCity:IsGodHeroExist(index)
	if not self.__save.godHeroList then
		return
	end
	return self.__save.godHeroList[index]
end

function clsCity:SetHeroEmploy(index)
	local info = self:IsGodHeroExist(index)
	if not info then return end
	info.state = 1
end
------------------------------------------------------------------------------------------
local PillageGotFeat = {["lose"] = 10, ["win"] = 30, ["lose2"] = 20}

function CalPillageGot(roleId, result)
	local tribeInfo = TRIBE.GetTribeDataById(roleId)
	local prize = {}
	if result == "win" and tribeInfo then
		prize.gold = TRIBE.GetTribeResourceNum(roleId, "gold") * PILLAGE_INCOME_RATE
		prize.spar = TRIBE.GetTribeResourceNum(roleId, "spar") * PILLAGE_INCOME_RATE
	else
		prize.gold = 0
		prize.spar = 0
	end
	prize.feat = PillageGotFeat[result]
	return prize
end


function clsCity:ProPillage(user, tRoleId)
	local roleId = user:GetRoleId()
	local myCityId = user:GetCityId()
	local city = CITY.GetCityObjById(myCityId)
	if city then
		city:ModInfoInList("member", roleId, "proTimestamp", os.time())
		city:SendMemberUpdate("state", roleId)
		user:SetPillageProtectTime(os.time())
	end
	user:SetFuncStatById(SETTING.FUNC_PILLAGE, 1)
	self:ModInfoInList("member", tRoleId, "fightState", 1)
	local function fight_call_back(fightId, winner, prize)
		self:ModInfoInList("member", tRoleId, "fightState")
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetWorkInfo("standby")
		else
			OFFLINE.SetOfflineData(roleId, "workInfo", "standby")
		end
		if winner == "attack" then
			local gold = prize.gold
			local spar = prize.spar
			self:ModInfoInList("member", tRoleId, "proTimestamp", os.time() + PILLAGE_PROTECT_TIME)
			self:SendMemberUpdate("state", tRoleId)
			if user then
				user:SetGold(gold)
				user:SetSpar(spar)
			else
				OFFLINE.SetOfflineData(roleId, "gold", gold)
				OFFLINE.SetOfflineData(roleId, "spar", spar)
			end
			local tUser = USER.GetOnlineUser(tRoleId)
			if tUser then
				tUser:SetGold(-gold)
				tUser:SetSpar(-spar)
				tUser:SetPillageProtectTime(os.time() + PILLAGE_PROTECT_TIME)
			else
				OFFLINE.SetOfflineData(tRoleId, "gold", -gold)
				OFFLINE.SetOfflineData(tRoleId, "spar", -spar)
				OFFLINE.SetOfflineData(tRoleId, "pillagePro", os.time() + PILLAGE_PROTECT_TIME)
			end
			user:SetPhyleFeatGot(prize.feat or 0)
			if prize.petGuardState then
				local msg = string.format("%s刚欺负到咱们头上来了，我发动了舍命守护才抢回了%d个钱币和%d个晶石。", USER.GetUserNickInfo(roleId, "nick"), gold, spar)
				if tUser then
					tUser:SendPetPrompt("pillageLose", msg)
				else
					OFFLINE.SetOfflineData(tRoleId, "petPrompt", "pillageLose", msg)
				end
			end
		else
			local tUser = USER.GetOnlineUser(tRoleId)
			if prize.petGuardState then
				local msg = string.format("%s刚欺负到咱们头上来了，我发动了守护祝福将他们赶跑了。", USER.GetUserNickInfo(roleId, "nick"))
				if tUser then
					tUser:SendPetPrompt("pillageWin", msg)
				else
					OFFLINE.SetOfflineData(tRoleId, "petPrompt", "pillageWin", msg)
				end
			end
		end
		SendPillageAttackMail(roleId, winner == "attack", tRoleId, self:GetId(), prize.gold, prize.spar)
		SendPillageDefendMail(tRoleId, winner ~= "attack", roleId, myCityId, prize.gold, prize.spar, prize.petGuardState)
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_city_pillage", {nick = USER.GetUserNickById(tRoleId), result = winner == "attack"})
--			PROTOCOL.s_city_myStat(con, "get")
		end
	end
	local param = {restoreHp = true, saveHp = true, Lv = tRoleId}
	param.attack = city:GetFightParam("attack", self)
	param.defend = self:GetFightParam("defend", city)
	
	local function read_back(ret)
		if ret then
			local embassy = EMBASSY.GetEmbassyObjById(tRoleId)
			param.reinforce = {defend = {[1] = embassy:GetStayHeroFightData()}}
		end
		user:SetWorkInfo("fighting", fightId, nil, nil)
		local fightId = FIGHT.BeginFight("pillage", roleId, tRoleId, {roleId}, fight_call_back, param)
	end
	EMBASSY.RestoreEmbassyData(tRoleId, read_back)
end

function SendPillageDefendMail(roleId, result, tRoleId, cityId, gold, spar, guardType)
	local nick = USER.GetUserNickInfo(tRoleId, "nick")
	local tLv = USER.GetUserNickInfo(tRoleId, "Lv")
	local city = CITY.GetCityObjById(cityId)
	local locate = city:GetLocation()
	local mail = {type = "fight"}
	local msg = string.format("【<a href=\"event:phyleMap_%s\"><u><font color = \"#FFff00\">%s Lv%d</font>(%d, %d)</u></a>】向您发起掠夺，", cityId, nick, tLv, locate.x, locate.y)
	if result then
		if guardType == "defend_up" then
			msg = msg .. "由于本命精灵为您的队伍加持了守护祝福，您在本次战斗中获得了胜利。"
		else
			msg = msg .. "您在战斗中获胜，成功地守卫了自己的资源。"
		end
	else
		msg = msg .. "您在战斗中不幸落败。"
		if guardType == "lost_down" then
			msg = msg .. "由于本命精灵的舍命守护，损失降低50%，"
		end
		msg = msg .. string.format("您损失了%d钱币，%d晶石。", gold, spar)
	end
	mail.title = string.format("%s向您发起掠夺", nick)
	mail.content = msg
	MAIL.SendSysMail(roleId, mail)
end

function SendPillageAttackMail(roleId, result, tRoleId, cityId, gold, spar)
	local nick = USER.GetUserNickInfo(tRoleId, "nick")
	local tLv = USER.GetUserNickInfo(tRoleId, "Lv")
	local city = CITY.GetCityObjById(cityId)
	local locate = city:GetLocation()
	local mail = {type = "fight"}
	local msg = string.format("【<a href=\"event:phyleMap_%s\"><u><font color = \"#FFff00\">%s Lv%d</font>(%d, %d)</u></a>】遭到您的讨伐，", cityId, nick, tLv, locate.x, locate.y)if result then
		msg = msg .. string.format("您在战斗中获胜，获得%d钱币，%d晶石。", gold, spar)
	else
		msg = msg .. string.format("您在战斗中不幸落败，未能获得对方资源。")
	end
	mail.title = string.format("您向%s发起掠夺", nick)
	mail.content = msg
	MAIL.SendSysMail(roleId, mail)
end

local memberUpdate = {
	identity = function(city, roleId)
		local userInfo = city:IsInList("member", roleId)
		if userInfo then
			local sData = {}
			sData.action = "identity"
			sData.nick = USER.GetUserNickById(roleId)
			sData.isMaster = 0
			if roleId == city:GetOwner() then
				sData.isMaster = 1
			end
			return sData
		end
	end,
	state = function(city, roleId)
		local sData = {}
		sData.action = "state"
		sData.nick = USER.GetUserNickById(roleId)
		local userInfo = city:IsInList("member", roleId)
		if userInfo then
			sData.proTime = (userInfo.proTimestamp <= os.time()) and 0 or (userInfo.proTimestamp - os.time())
		end
		return sData
	end,
	appear = function(city, roleId)
		local sData = {}
		sData.nick = USER.GetUserNickById(roleId)
		sData.action = "appear"
		local userInfo = city:IsInList("member", roleId)
		if roleId == city:GetOwner() then
			sData.isMaster = 1
		end
		local union = UNION.GetUnionById(userInfo.unionId)
		if union then
			sData.union = union:GetName()
			sData.unionId = userInfo.unionId
		else
			sData.union = ""
			sData.unionId = -1
		end
		sData.Lv = userInfo.Lv
		local timelong = userInfo.proTimestamp - os.time()
		sData.proTime = timelong > 0 and timelong or 0
		return sData
	end,
	disappear = function(city, roleId)
		local sData = {}
		sData.nick = USER.GetUserNickById(roleId)
		sData.action = "disappear"
		return sData
	end,
	donate = function(city)
		local sData = {}
		sData.action = "donate"
		sData.expRate = math.floor(city:GetSparNum() / city:GetSparMax() * 100)
		return sData
	end,
	upgrade = function(city)
		local sData = {}
		sData.action = "upgrade"
		sData.Lv = city:GetLv()
		sData.expRate = math.floor(city:GetSparNum() / city:GetSparMax() * 100)
		sData.resRate = city:GetBaseResAddRate()
		sData.memberNum = city:GetMemberNum()
		sData.memberMax = city:GetMemberMax()
		return sData
	end,
	union = function(city, roleId)
		local sData = {}
		sData.nick = USER.GetUserNickById(roleId)
		sData.action = "union"
		local unionId = USER.GetUserNickInfo(roleId, "unionId")
		local union = UNION.GetUnionById(unionId)
		if union then
			sData.union = union:GetName()
			sData.unionId = unionId
		else
			sData.union = ""
			sData.unionId = -1
		end
		sData.Lv = USER.GetUserNickInfo(roleId, "Lv")
		return sData
	end,
	owner = function(city)
		local sData = {}
		sData.action = "owner"
		sData.owner = USER.GetUserNickById(city:GetOwner()) or ""
		return sData
	end,
}

function clsCity:SendMemberUpdate(type, roleId)
	if not memberUpdate[type] then
		return
	end
	local sData = memberUpdate[type](self, roleId)
	if not sData then
		return
	end
	for k in pairs(USER.GetUserFocusListByType(self:GetId())) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_city_update", sData)
		end
	end
end


function clsCity:ProAttack(user, tacticId)
	if self:IsMainCity() then
		return
	end
	local roleId = user:GetRoleId()
	local kind
	self:SetFightState(1)
	local phyle = user:GetPhyle()
	if phyle == self:GetPhyle() then
		user:SetFuncStatById(SETTING.FUNC_ATK_CITY_SAME_PHYLE, 1)
		kind = "same"
	else
		user:SetFuncStatById(SETTING.FUNC_ATK_CITY_OHER_PHYLE, 1)
		user:SetCityAttackTimes(1)
		kind = "diff"
	end
	local function fight_call_back(fightId, winner)
		self:SetFightState()
		user:SetWorkInfo("standby")
		user:CheckAchvState("cityAttack")
		local tRoleId = self:GetOwner()
		if winner == "attack" then
			if tRoleId then
				self:LeaveCity(tRoleId)
				self:SendCityMsg("def_lose", tRoleId)
			end
			self:SetLastPhyle(self:GetPhyle())
			self:SetPhyle(phyle)
			self:SetState("wait")
			local timelong = 0
			if phyle == self:GetLastPhyle() then
				timelong = SAME_PHYLE_WAIT_TIME
			else
				timelong = DIFF_PHYLE_WAIT_TIME
				self:SetSparNum(-self:GetSparMax() * CITY_ATTACK_LOSS_RATE)
				self:ClearAllMember()
			end
			self:SetTempOwner(roleId)
			self:SetTimeStamp(os.time() + timelong)
			self:StartCallOut()
			if user:GetCityIdentity() ~= ID_MASTER then	
				self:Occupancy(user)
			end
			self:SendCityMsg("atk_win", roleId)
			self:SendCityUpdate("owner")
		else
			if tRoleId then
				self:SendCityMsg("def_win", tRoleId, roleId)
			end
			self:SendCityMsg("atk_lose", roleId)
		end
		self:SendCityUpdate("state")
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_cityFight_attack", {cityId = self:GetId(), result = winner == "attack"})
		end
	end
	local param = {restoreHp = true, saveHp = true}
	local city = CITY.GetCityObjById(user:GetCityId())
	param.attack = city:GetFightParam("attack", self)
	param.defend = self:GetFightParam("defend", city, tacticId)
	user:SetWorkInfo("fighting")
	local fightId = FIGHT.BeginFight("cityAttack", kind == "same" and roleId or city:GetCityFightData(kind), self:GetCityFightData(kind), {roleId}, fight_call_back, param)
end

function clsCity:StartCallOut()
	if self:GetState() == "normal" then
		return
	end
	local timelong = self:GetTimeStamp() - os.time()
	if timelong <= 0 then
		self:ProCallOutTimeOut(self:GetTimeStamp())
	else
		self.__temp.timerId = CALLOUT.ObjCallOut(self:GetCallOut(), clsCity.ProCallOutTimeOut, timelong, self)
	end
end

function clsCity:RemoveCallOut()
	if self.__temp.timerId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.timerId)
	end
end

function clsCity:ProCallOutTimeOut(timestamp)
	if self:GetState() == "wait" then
		if not self:GetOwner() then
			self:SetTempOwner()
			self:SetState("normal")
			self:ClearAllMember()
			self:SendCityUpdate("owner")
		elseif self:GetPhyle() == self:GetLastPhyle() then
			local timelong = self:GetTimeStamp() - os.time()
			if timelong > 0 then
				self:SetState("protect")
				self:StartCallOut()
			else
				self:SetState("normal")
			end
		else
			self:SetState("protect")
			self:SetTimeStamp((timestamp or os.time()) + CITY_PROTECT_TIMELONG)
			self:StartCallOut()
		end
		self:SendCityUpdate("state")
	elseif self:GetState() == "protect" then
		self:SetState("normal")
		self:SetTimeStamp()
		self:SendCityUpdate("state")
	end
end

--清理所有的依附者和依附申请
function clsCity:ClearAllMember()
	for k in pairs(self:GetListByType("member")) do
		self:RemoveMember(k)
		self:SendCityMsg("leave", k)
	end
	for k in pairs(self:GetListByType("apply")) do
		self:RmFromList("apply", k)
	end
end

local UpdateList = {
	state = function(city)
		local sData = {}
		sData.cityId = city:GetId()
		sData.state = city:GetState()
		local timelong = city:GetTimeStamp() - os.time()
		sData.timelong = timelong > 0 and timelong or 0
		return sData
	end,
	owner = function(city)
		local sData = {}
		sData.cityId = city:GetId()
		local owner = city:GetOwner()
		if owner then
			sData.owner = USER.GetUserNickById(owner)
			sData.vipLv = USER.GetUserNickInfo(owner, "vipLv")
		else
			sData.owner = ""
			if city:GetState() == "wait" then
				sData.tempOwner = USER.GetUserNickById(city:GetTempOwner())
			end
		end
		if city:GetPhyle() ~= city:GetLastPhyle() then
			sData.phyle = city:GetPhyle()
		end
		return sData
	end,
	Lv = function(city)
		local sData = {}
		sData.cityId = city:GetId()
		sData.Lv = city:GetLv()
		sData.limitLv = city:GetLimitLv()
		sData.picId = city:GetPicId()
		return sData
	end,
}


function clsCity:SendCityUpdate(type)
	if not UpdateList[type] then
		return 
	end
	local sData = UpdateList[type](self)
	for k in pairs(CITY.GetAllUserInCityMap()) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			con:SendProtocol("c_cityFight_update", sData)
		end
	end
end



function __init__()
	local UPGRADE = Import("autocode/cityUpgradeList.lua")
	CityUpgradeList = UPGRADE.GetTable()
	
	local CITYLIST = Import("autocode/cityList.lua")
	AllCityInfoList = CITYLIST.GetTable()
	
	local BUFFERTYPE = Import("autocode/cityFightBuffer.lua")
	CityFightBufferType = BUFFERTYPE.GetTable()
end
