--玩家统计数据扩展

local MEDITATE_LAST_TIME = 24 * 3600
local MEDITATE_UPDATE_TIMELONG = 30
local BUFFER_RESTORE_HP = "19"
local FIGHT_CD_TIME = 10

local clsUser = USERBASE.clsUser


--------------------------------------------------------------------------------
--新手引导
function clsUser:SetUserGuideStep(data)
	self.__save.guideStep = data
end

--已经完成的引导
function clsUser:GetUserGuideStep()
	return self.__save.guideStep or GUIDE.MAX_GUIDE_STEP
end

---------------------------------------------------------------------------------

--设置注册时间
function clsUser:SetRegTime(time)
	self.__save.regTime = time
end

function clsUser:GetRegTime()
	return self.__save.regTime
end
--上次登录的情况
function clsUser:SetLastLogin(ip, time)
	self.__temp.lastLogin = {ip = ip, time = time}
end

function clsUser:GetLastLogin()
	return self.__temp.lastLogin or {}
end
--本次登录的情况
function clsUser:SetLogin(ip)
	local info = self.__save.login
	if info and info.ip and info.time then		--记录上次登录的情况
		self:SetLastLogin(info.ip, info.time)
	end
	self.__save.login = {ip = ip, time = os.time()}
	self:SetTimeStamp(os.time())
	self:SetLoginTimeStamp(os.time())
	local offlineTime = self:GetOfflineTime()
	if offlineTime then
		self:SetLastOfflineTime(offlineTime)	--记录上次离线的时间
	end
	self:UpdateOnlinePrizeInfo()				--登录时刷新在线奖励
	STATS.SaveLoginHistory(self)				--保存登陆历史
end

function clsUser:GetLoginIp()
	if not self.__save.login then
		return nil
	end
	return self.__save.login.ip
end

function clsUser:GetLoginTime()
	if not self.__save.login then
		return nil
	end
	return self.__save.login.time
end
--设置离线时间
function clsUser:SetOfflineTime(time)
	self.__save.logout = self.__save.logout or {}
	self.__save.logout.time = time
end

function clsUser:GetOfflineTime()
	self.__save.logout = self.__save.logout or {}
	return self.__save.logout.time
end
--设置上次离线时间
function clsUser:SetLastOfflineTime(time)
	self.__save.logout = self.__save.logout or {}
	self.__save.logout.last = time or os.time()
end

function clsUser:GetLastOfflineTime()
	self.__save.logout = self.__save.logout or {}
	return self.__save.logout.last
end

--登录状态
function clsUser:SetLoginState(data)
	self.__temp.loginstate = data
end

function clsUser:GetLoginState()
	return self.__temp.loginstate
end

function clsUser:SetLoginInitDone(state)
	self.__temp.loginInit = state
end

function clsUser:IsLoginInitDone()
	return self.__temp.loginInit
end

--在线时长统计时间点
function clsUser:SetLoginTimeStamp(data)
	self.__temp.logintime = data or os.time()
end

function clsUser:GetLoginTimeStamp()
	return self.__temp.logintime or os.time()
end

function clsUser:SetTimeStamp(time)
	self.__temp.timestamp = time
end

function clsUser:GetTimeStamp()
	return self.__temp.timestamp
end

--累积在线时长
function clsUser:GetTotalOnlineTime()
	return self.__save.stats.onlineTime or 0
end
--下线时统计在线时间
function clsUser:SetTotalOnlineTime(time)
	local timelong = time or (os.time() - self:GetLoginTimeStamp())
	self.__save.stats.onlineTime = (self.__save.stats.onlineTime or 0) + timelong
	self:SetLoginTimeStamp(os.time())
	self:SetDailyOnlineTime(timelong)
	self:SetAntiAddictOnlineTime(timelong)
end

--每日在线时长统计
function clsUser:SetDailyOnlineTime(timelong)
	self.__save.stats.dailyOnline = self.__save.stats.dailyOnline or {}
	local info = self.__save.stats.dailyOnline
	if not info.timestamp then
		info.timelong = timelong
	else
		if CalDiffDays(info.timestamp, os.time()) ~= 0 then
			info.timelong = timelong
		else
			info.timelong = info.timelong + timelong
		end
	end
	info.timestamp = os.time()
end

function clsUser:GetDailyOnlineTime()
	self.__save.stats.dailyOnline = self.__save.stats.dailyOnline or {}
	local info = self.__save.stats.dailyOnline
	if not info.timestamp or CalDiffDays(info.timestamp, os.time()) ~= 0 then
		return o
	end
	return info.timelong
end


--连续登陆天数
function clsUser:GetDurativeLogin()
	self.__save.stats.duratLogin = self.__save.stats.duratLogin or {}
	return self.__save.stats.duratLogin.days
end
--最大连续登陆天数
function clsUser:GetMaxDurativeLogin()
	self.__save.stats.duratLogin = self.__save.stats.duratLogin or {}
	return self.__save.stats.duratLogin.max
end

function clsUser:SetDurativeLogin(data)
	assert(tonumber(data))
	self.__save.stats = self.__save.stats or {}
	self.__save.stats.duratLogin = self.__save.stats.duratLogin or {}
	local info = self.__save.stats.duratLogin
	if not info.time then
		self.__save.stats.duratLogin.days = data
		self.__save.stats.duratLogin.max = data
	else
		local ndays = CalDiffDays(info.time, os.time())
		if ndays == 1 then
			self.__save.stats.duratLogin.days = self.__save.stats.duratLogin.days + data
		elseif ndays > 1 then
			self.__save.stats.duratLogin.days = data
		end
	end
	if (info.max or 0) < self.__save.stats.duratLogin.days then
		self.__save.stats.duratLogin.max = self.__save.stats.duratLogin.days	
	end
	self.__save.stats.duratLogin.time = os.time()
end

--连续登陆奖励领取状态
function clsUser:SetDuratLoginPrizeDayState()
	self.__save.prize = self.__save.prize or {}
	self.__save.prize.durative = os.time()
end

function clsUser:GetDuratLoginPrizeDayState()
	self.__save.prize = self.__save.prize or {}
	if not self.__save.prize.durative or CalDiffDays(self.__save.prize.durative, os.time()) >= 1 then
		local day = self:GetDurativeLogin()
--		if WELFARE.GetLoginPrizeInfo("durative", day) then
			return day
--		end
	end
	return false
end
--每日登陆奖励领取状态
function clsUser:SetDailyLoginPrizeState()
	self.__save.prize = self.__save.prize or {}
	self.__save.prize.daily = os.time()
end

function clsUser:GetDailyLoginPrizeState()
	self.__save.prize = self.__save.prize or {}
	if not self.__save.prize.daily then
		return true
	elseif CalDiffDays(self.__save.prize.daily, os.time()) >= 1 then
		return true
	end
	return false
end

--[[累计充值
function clsUser:SetTotalRecharge(data)
	self.__save.stats.ingot = math.max((self.__save.stats.ingot or 0) + data, 0)
	local lv = VIP.GetVipLvByIngot(self.__save.stats.ingot)
	if lv >= self:GetVipLv() then
		self:SetVipLv(lv)
	end
	self:SetFirWeekRecharge(data)
end

function clsUser:GetTotalRecharge()
	return self.__save.stats.ingot or 0
end

--累计首周充值数量
function clsUser:SetFirWeekRecharge(data)
	if CalDiffDays(_G.SERVER_START_TIME, os.time()) > 7 then
		return
	end
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	self.__save.stats.recharge.ingot = math.max((self.__save.stats.recharge.ingot or 0) + data)
end

function clsUser:GetFirWeekRecharge()
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	return self.__save.stats.recharge.ingot
end

-- state 0 已经完成 1 已经领奖
function clsUser:SetRechargePrizeGot(k, state)
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	self.__save.stats.recharge.list = self.__save.stats.recharge.list or {}
	self.__save.stats.recharge.list[k] = state
end

-- 2 未完成
function clsUser:GetRechargeState(k)
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	self.__save.stats.recharge.list = self.__save.stats.recharge.list or {}
	return self.__save.stats.list[k] or 2
end

function clsUser:GetAllRechargeList()
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	return self.__save.stats.recharge.list or {}
end

function clsUser:HasAllRechargePrizeGot()
	self.__save.stats.recharge = self.__save.stats.recharge or {}
	return table.size(self.__save.stats.recharge.list or {})
end

--0 已完成 1 已领奖励 2 未完成
function clsUser:SetRechargeTimes(data)
	self.__save.stats.times = self.__save.stats.times or {}
	self.__save.stats.times.times = math.max((self.__save.stats.times.times or 0) + data)
	self.__save.stats.times.state = 0 
end

function clsUser:ModRechargeTimesState(state)
	self.__save.stats.times = self.__save.stats.times or {}
	self.__save.stats.times.state = state
end

--2 为完成 0 已完成 1 已领奖
function clsUser:GetRechargeTimesState()
	self.__save.stats.times = self.__save.stats.times or {}
	return self.__save.stats.times.state or 2
end

function clsUser:GetRechargeTimes()
	return self.__save.stats.times or 0
end
]]

--每日购买精力次数
function clsUser:SetDailyVigourBuyTimes(data)
	self.__save.stats.buyVigour = self.__save.stats.buyVigour or {}
	local info = self.__save.stats.buyVigour
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = 0
	end
	info.num = math.max(info.num + data, 0)
	info.time = os.time()
end

function clsUser:GetDailyVigourBuyTimes()
	self.__save.stats.buyVigour = self.__save.stats.buyVigour or {}
	local info = self.__save.stats.buyVigour
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = 0
	end
	return info.num
end

function clsUser:GetDailyVigourBuyMaxNum()
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_BUY_VIGOUR) or 0
end

--每日跳过战斗次数
function clsUser:SetDailyFastFightTimes(data)
	self.__save.stats.fastFight = self.__save.stats.fastFight or {}
	local info = self.__save.stats.fastFight
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsUser:GetDailyFastFightTimes()
	self.__save.stats.fastFight = self.__save.stats.fastFight or {}
	local info = self.__save.stats.fastFight
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end

function clsUser:GetDailyFastFightMaxNum()
	return VIP.GetVipFuncValidTimes(self:GetVipLv(), VIP.VIP_FAST_FIGHT) or 0
end


--免费开启建筑队伍2
function clsUser:HasFreeUpgradeList()
	local vipLv = self:GetVipLv()
	return VIP.IsVipFuncValid(self:GetVipLv(), VIP.VIP_UPGRADE_LIST_OPEN)
end

--免费冷却次数
function clsUser:GetMaxSpeedUpTimes()
	return 0
end

--加速次数
function clsUser:SetSpeedUpTimes(data)
	self.__save.speedup = self.__save.speedup or {}
	if not self.__save.speedup.timestamp or CalDiffDays(os.time(), self.__save.speedup.timestamp) ~= 0 then
		self.__save.speedup.times = 0
	end
	self.__save.speedup.times = math.max((self.__save.speedup.times or 0 + data), 0)
	self.__save.speedup.timestamp = os.time()
end

function clsUser:GetSpeedUpTimes()
	self.__save.speedup = self.__save.speedup or {}
	if CalDiffDays(os.time(), self.__save.speedup.timestamp or 0) ~= 0 then
		self.__save.speedup.times = 0
	end
	return self.__save.speedup.times
end

--累积使用资金
function clsUser:GetTotalGoldUsed()
	return self.__save.stats.goldUsed or 0
end

function clsUser:SetTotalGoldUsed(data)
	assert(tonumber(data))
	self.__save.stats.goldUsed = self.__save.stats.goldUsed + data
end
--累积使用商币
function clsUser:GetTotalShGoldUsed()
	return self.__save.stats.shGoldUsed or 0
end

function clsUser:SetTotalShGoldUsed(data)
	assert(tonumber(data))
	self.__save.stats.shGoldUsed = self.__save.stats.shGoldUsed + data
end
--累积使用礼金
function clsUser:GetTotalPreGoldUsed()
	return self.__save.stats.preGoldUsed or 0
end

function clsUser:SetTotalPreGoldUsed(data)
	assert(tonumber(data))
	self.__save.stats.preGoldUsed = self.__save.stats.preGoldUsed + data
end


--等级连续排名第一
function clsUser:GetMaxGradeOne()
	self.__save.stats.maxGradeOne = self.__save.stats.maxGradeOne or {}
	return self.__save.stats.maxGradeOne.num or 0
end

function clsUser:SetMaxGradeOne(data, time)
	self.__save.stats.maxGradeOne = self.__save.stats.maxGradeOne or {}
	if not self.__save.stats.maxGradeOne.time then
		self.__save.stats.maxGradeOne.num = data
	else
		local ndays = CalDiffDays(self.__save.stats.maxGradeOne.time, time or os.time())
		if ndays == 1 then
			self.__save.stats.maxGradeOne.num = self.__save.stats.maxGradeOne.num + data
		elseif ndays > 1 then
			self.__save.stats.maxGradeOne.num = data
		end
	end
	self.__save.stats.maxGradeOne.time = time or os.time()
end

--势力战统计信息 （sum:参加次数 win:胜利次数）
function clsUser:SetPhyleFightStats(isWin)
	self.__save.stats.phyleFight = self.__save.stats.phyleFight or {}
	local info = self.__save.stats.phyleFight
	info.sum = (info.sum or 0) + 1
	if isWin then
		info.win = (info.win or 0) + 1
	end
	self:CheckAchvState("phyleFight")
end

function clsUser:GetPhyleFightStats()
	self.__save.stats.phyleFight = self.__save.stats.phyleFight or {}
	return self.__save.stats.phyleFight
end
--联盟献祭次数
function clsUser:SetUnionSacrificeTimes(data)
	self.__save.stats.union = self.__save.stats.union or {}
	self.__save.stats.union.sacrificeTimes = (self.__save.stats.union.sacrificeTimes or 0) + data
	self:CheckAchvState("sacrifice")
end

function clsUser:GetUnionSacrificeTimes()
	self.__save.stats.union = self.__save.stats.union or {}
	return self.__save.stats.union.sacrificeTimes or 0
end
--送花统计
function clsUser:SetSendFlowerTimes(data)
	self.__save.stats.sendflower = (self.__save.stats.sendflower or 0) + data
	self:CheckAchvState("sendFlower")
end

function clsUser:GetSendFlowerTimes()
	return self.__save.stats.sendflower or 0
end
--收花统计
function clsUser:SetReceiveFlowerTimes(data)
	self.__save.stats.receiveflower = (self.__save.stats.receiveflower or 0) + data
	self:CheckAchvState("receiveFlower")
end

function clsUser:GetReceiveFlowerTimes()
	return self.__save.stats.receiveflower or 0
end

--攻打城池统计 (true / false)
function clsUser:SetCityAttackTimes(result)
	self.__save.stats.cityAttack = self.__save.stats.cityAttack or {}
	self.__save.stats.cityAttack.times = (self.__save.stats.cityAttack.times or 0) + 1
	if result then
		self.__save.stats.cityAttack.win = (self.__save.stats.cityAttack.win or 0) + 1
	end
end

function clsUser:GetCityAttackTimes()
	self.__save.stats.cityAttack = self.__save.stats.cityAttack or {}
	return self.__save.stats.cityAttack
end

function clsUser:SetPetTransformInfo(formId)
	self.__save.stats.petForm = self.__save.stats.petForm or {}
	local info = self.__save.stats.petForm
	info.times = (info.times or 0) + 1
	info.list = info.list or {}
	info.list[tostring(formId)] = 1
	self:CheckAchvState("petForm")
end

function clsUser:GetPetTransformInfo()
	self.__save.stats.petForm = self.__save.stats.petForm or {}
	return self.__save.stats.petForm
end


function clsUser:GetWorldBossTimes(id)
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.worldBoss = self.__save.daily.worldBoss or {}
	return CalDiffDays(self.__save.daily.worldBoss[id] or 0, os.time()) == 0 and 1 or 0
end

function clsUser:SetWorldBossTimes(id)
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.worldBoss = self.__save.daily.worldBoss or {}
	self.__save.daily.worldBoss[id] = os.time()
end

function clsUser:SetTeamCopyTimes(data)
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.teamcopy = self.__save.daily.teamcopy or {}
	if CalDiffDays(os.time(), self.__save.daily.teamcopy.timestamp) == 0 then
		self.__save.daily.teamcopy.times = 0
	end
	self.__save.daily.teamcopy.times = math.max((self.__save.daily.teamcopy.times or 0) + data, 0)
	self.__save.daily.tempcopy.timestamp = os.time()
end

function clsUser:GetTeamCopyTimes()
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.teamcopy = self.__save.daily.teamcopy or {}
	if CalDiffDays(os.time(), self.__save.daily.teamcopy.timestamp) == 0 then
		self.__save.daily.teamcopy.times = 0
	end
	return self.__save.daily.teamcopy.times or 0
end

function clsUser:SetDailyMissionTimes(data)
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.mission = self.__save.daily.mission or {}
	if CalDiffDays(os.time(), self.__save.daily.mission.timestamp) == 0 then
		self.__save.daily.mission.times = 0
	end
	self.__save.daily.mission.times = math.max((self.__save.daily.mission.times or 0) + data, 0)
	self.__save.daily.mission.timestamp = os.time()
end

function clsUser:GetDailyMissionTimes()
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.mission = self.__save.daily.mission or {}
	if CalDiffDays(os.time(), self.__save.daily.mission.timestamp) == 0 then
		self.__save.daily.mission.times = 0
	end
	return self.__save.daily.mission.times or 0
end
--[[
function clsUser:SetArenaFightTimes(data)
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.arena = self.__save.daily.arena or {}
	if CalDiffDays(os.time(), self.__save.daily.arena.timestamp) == 0 then
		self.__save.daily.arena.times = 0
	end
	self.__save.daily.arena.times = math.max((self.__save.daily.arena.times or 0) + data, 0)
	self.__save.daily.arena.timestamp = os.time()
end

function clsUser:GetArenaFightTimes()
	self.__save.daily = self.__save.daily or {}
	self.__save.daily.arena = self.__save.daily.arena or {}
	if CalDiffDays(os.time(), self.__save.daily.arena.timestamp) == 0 then
		self.__save.daily.arena.times = 0
	end
	return self.__save.daily.arena.times or 0
end
]]
---------------------------------------------- buffer -----------------------------
--buffer效果
function clsUser:SetBufferEffect(id, num, timelong)
	self.__save.buffer = self.__save.buffer or {}
	local buffInfo = self:GetBufferEffect(id)
	if buffInfo then
		buffInfo.left = buffInfo.left + num
		if timelong then
			buffInfo.num = num
			buffInfo.endTime = buff.endTime + buffInfo.timelong
		end
	else
		buffInfo = {timestamp = os.time()}
		if timelong then
			buffInfo.num = num
			buffInfo.endTime = os.time() + timelong
		else
			buffInfo.left = num
		end
		self.__save.buffer[id] =  buffInfo
	end
	if (buffInfo.left and buffInfo.left <= 0) or (buffInfo.endTime and buffInfo.endTime <= os.time()) then
		self:DelBufferEffect(id)
	else
		if buffInfo.callout then
			CALLOUT.RemoveCallOut(self:GetCallOut(), buffInfo.callout)
		end
		if timelong then		
			buffInfo.callout = CALLOUT.ObjCallOut(self:GetCallOut(), BUFFER.RemoveOneBuffer, timelong, self, id)
		end
	end
end

function clsUser:GetBufferEffect(id)
	self.__save.buffer = self.__save.buffer or {}
	return self.__save.buffer[id]
end

function clsUser:DelBufferEffect(id)
	local buff = self:GetBufferEffect(id)
	if not buff then
		return
	end
	if buff.callout then
		CALLOUT.RemoveCallOut(self:GetCallOut(), buff.callout)
	end
	self.__save.buffer[id] = nil
end


function clsUser:GetBufferEffectList()
	self.__save.buffer = self.__save.buffer or {}
	return self.__save.buffer
end

function clsUser:StartBufferCallOut()
	for k, v in pairs(self:GetBufferEffectList()) do
		if v.endTime then
			local timelong = v.endTime - os.time()
			if timelong <= 0 then
				BUFFER.RemoveOneBuffer(self, k, true)
			else
				v.callout = CALLOUT.ObjCallOut(self:GetCallOut(), BUFFER.RemoveOneBuffer, timelong, self, k)
			end
		end
	end
end

function clsUser:ClearBufferCallOut()
	for k, v in pairs(self:GetBufferEffectList()) do
		if v.callout then
			CALLOUT.RemoveCallOut(self:GetCallOut(), v.callout)
			v.callout = nil
		end
	end
end


function clsUser:AddOneBuffer(id, num)
	local info = BUFFER.GetBufferInfoById(id)
	if not info then
		return
	end
	local same = BUFFER.IsSaveBufferExist(self, id)
	if same and not info.canAdd then
		BUFFER.RemoveOneBuffer(self, same)
	end
	self:SetBufferEffect(id, num or info.num, info.timelong)
	self:FreshTeamTotalAttr()		--buffer影响全部参战人员属性
end
-----------------------------------------------------------------------------

function clsUser:DelHeroFromAllLineup(heroId, flag)
	for k, v in pairs(self:GetFightFormList()) do
		for id, grid in pairs(v:GetGridList()) do
			if grid:GetRoleId() == heroId then
				grid:SetRoleId()
				if not flag then
					self:SyncData("teamHp")
				end
				break
			end
		end
	end
end

function clsUser:GetFormatedCurLineup()
	local sData = {}
	local list = self:GetCurFightLineup()
	for k, v in ipairs(list) do
		local buf = {}
		buf.heroId = v.id
		buf.pos = v.pos
		local obj
		if IsUserId(v.id) then
			obj = self	
			buf.aptLv = self:GetAptLv()
			buf.type = "user"
		else
			local hero = self:GetHero(v.id)
			if hero then
				obj = hero
				buf.aptLv = CLSHERO.GetAptLvByAptitude(hero:GetAptitude())
				buf.type = hero:GetType()
			end
		end
		if obj then
			buf.name = obj:GetName()
			buf.Lv = obj:GetLv()
			buf.headId = obj:GetHeadId()
			buf.job = obj:GetJob()
		end
		table.insert(sData, buf)
	end
	return sData
end

function clsUser:GetCurLineupTotalHp()
	local hp = 0
	for k, v in pairs(self:GetCurFightLineup()) do
		if IsUserId(v.id) then
			hp = hp + self:GetHp()
		else
			local hero = self:GetHero(v.id)
			if hero then
				hp = hp + hero:GetHp()
			end
		end
	end
	return hp
end

function clsUser:GetStayHeroHp()
	local hp = 0
	for k, v in pairs(self:GetHeroList()) do
		if v:GetWorkState() == "stay" then
			hp = hp + v:GetHp()
		end
	end
	return hp
end

------------------------------------------------------


--刷新角色全部属性值
function clsUser:FreshTotalAttr(flag)
	local Lv = self:GetLv()
	self:InitTotalAttrList()						--人物自身属性
	local suitList = {}
	for k, v in pairs(self:GetEquipList()) do		--装备附加属性
		if Lv >= v:GetNeedLv() then
			for k, v in pairs(v:GetEquipAttrAdd()) do
				self:SetTotalAttr(k, v)
			end
			local suitId = v:GetSuitId()
			if suitId then
				suitList[suitId] = (suitList[suitId] or 0) + 1
			end
		end
	end
	for k, v in pairs(suitList) do					--套装效果
		local list = SUIT.GetSuitAttrById(k, v)
		for _, table in ipairs(list or {}) do
			for name, num in pairs(table) do 
				self:SetTotalAttr(name, num)
			end
		end
	end
	for k, v in pairs(self:GetSkillList()) do			--技能附加效果
		for key, val in pairs(SKILL.GetPassiveSkillEffect(v.id, v.Lv) or {}) do
			self:SetTotalAttr(key, val)
		end
	end
--[[	local tribe = self:GetTribe()
	if tribe then
		local academy = tribe:GetStruct("academy")
		if academy then
			for k, v in pairs(academy:GetTechList()) do
				local info = ACADEMY.GetTechById(k)
				if info and info.type == 2 then
					for attrType, num in pairs(info.list[v.Lv].attr) do
						self:SetTotalAttr(attrType, num)
					end
				end
			end
		end
	end]]
	local status = self:GetStatus()		--仙印增加属性
	if status then
		for k, v in pairs(status:GetAttrAdd()) do
			self:SetTotalAttr(k, v)
		end
	end
	for k, v in pairs(self:GetBufferEffectList()) do	--buffer附加属性
		local buffInfo = BUFFER.GetBufferInfoById(k)
		for key, val in pairs(buffInfo.attr or {}) do
			self:SetTotalAttr(key, val)
		end
	end
	local form = self:GetCurForm()		--阵法属性加成
	if form then
		for k, v in pairs(form:GetAttr()) do
			self:SetTotalAttr(k, v)
		end
	end
	local pet = self:GetPet()		--本命精灵
	local rate = pet:GetTalentAdd(311012)
	self:ModAttrByRate("physcAtk", rate)
	self:ModAttrByRate("magicAtk", rate)
	local rate = pet:GetTalentAdd(311013)
	self:ModAttrByRate("physcDef", rate)
	self:ModAttrByRate("magicDef", rate)
	if self:IsFuncOpen(SETTING.FUNC_FAIRYDEV) then				--仙器系统加成
		local list, skillLv = self:GetFairyDevAttrAdd()
		for k, v in pairs(list.single) do		--单体属性
			self:SetTotalAttr(k, v)
		end
		for k, v in pairs(list.range) do			--全体属性
			self:SetTotalAttr(k, v)
		end	
	end
	if pet then
		pet:SetSkillLv(1 + (skillLv or 0))
	end
	local rate = 0
	if self:IsFuncOpen(SETTING.FUNC_TRIBE) then
		local tribe = self:GetTribe()
		local soldierInfo = BARRACK.GetUserSoldierInfo(self)
		rate = rate + (soldierInfo and soldierInfo.add or 0)
	end
	if self:IsFuncOpen(SETTING.FUNC_MON_ALBUM) then
		for k, v in pairs(self:GetMonAlbumAttr()) do
			self:SetTotalAttr(k, v)
		end
	end
	self:SetTotalAttrAddRate(2, rate)
	self:CalFightForce()
	self:RestoreHp(self, flag)
	self:CheckAchvState("attr")
end

--刷新队伍成员属性
function clsUser:FreshTeamTotalAttr()
	local form = self:GetCurForm()
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId()
		if IsUserId(roleId) then
			self:FreshTotalAttr()
		else
			local hero = self:GetHero(roleId)
			if hero then
				hero:FreshTotalAttr()
			end
		end
	end
end
--刷新所有成员属性
function clsUser:FreshAllHeroTotalAttr()
	self:FreshTotalAttr()
	for k, v in pairs(self:GetHeroList()) do
		v:FreshTotalAttr()
	end
end
--给队伍增加经验
function clsUser:SetTeamExp(exp, from)
	local form = self:GetCurForm()
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId()
		if IsUserId(roleId) then
			self:SetExp(exp, from)
		else
			local hero = self:GetHero(roleId)
			if hero then
				hero:SetExp(exp, from)
			end
		end
	end
end

function clsUser:RestoreTeamHp()
	for k, v in pairs(self:GetCurFightLineup()) do
		if IsUserId(v.id) then
			self:RestoreHp(self, true)
		else
			local hero = user:GetHero(v.id)
			if hero then
				self:RestoreHp(hero, true)
			end
		end
	end
end

--isFree 不计算损失，直接恢复到满血
function clsUser:ProHpAfterFight(heroId, curHp, needRestore, isFree)
	local obj = self
	if heroId and heroId ~= self:GetRoleId() then
		local hero = self:GetHero(heroId)
		if not hero then return end
		obj = hero
	end
	obj:SetHp(curHp - obj:GetHp())
	if needRestore then
		self:RestoreHp(obj, isFree)
	end
end

--isFree 直接恢复到满血
function clsUser:RestoreHp(obj, isFree)
	obj = obj or self
	if isFree then
		return obj:SetHp(obj:GetTotalAttr("hpMax"))
	end
	local lostHp = obj:GetCurHpMax() - obj:GetHp()
	local buffHp, restoreHp = 0, 0
	local buffInfo = self:GetBufferEffect(BUFFER_RESTORE_HP)
	if buffInfo then
		buffHp = buffInfo.left
	end
	if buffHp > 0 then
		restoreHp = math.min(buffHp, lostHp)
		lostHp = lostHp - restoreHp
		self:SetBufferEffect(BUFFER_RESTORE_HP, -restoreHp)
	end
	if lostHp > 0 then
		local soldier, restored = 0, 0
		local tribe = self:GetTribe()
		if tribe then
			soldier = tribe:GetResourceStat("soldier", "num")
		end
		if soldier > 0 then
			restored = math.min(soldier, lostHp)
			tribe:ModResourceStat("soldier", "num", -restored)
			restoreHp = restoreHp + restored
		end
	end
	obj:SetHp(restoreHp)
end

function clsUser:CalHpInfo()
	local formationList = self:GetCurFightLineup()
	local total, cur = 0, 0
	local obj
	for _, val in pairs(formationList) do
		if IsUserId(val.id) then
			obj = self
		else
			obj = self:GetHero(val.id)
		end
		if obj then
			total = total + obj:GetCurHpMax()
			cur = cur + obj:GetHp()
		end
	end
	return total, cur
end

--部队战斗力
function clsUser:GetlineUpForce()
	local force = 0
	local form = self:GetCurForm()
	if not form then
		self:InitFightForm()
		form = self:GetCurForm()
	end
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId()
		local obj
		if IsUserId(roleId) then
			obj = self
		else
			local hero = self:GetHero(roleId)
			if hero then
				obj = hero
			end
		end
		if obj then
			force = force + obj:GetFightForce()
		end
	end
	return math.ceil(force)
end


function clsUser:GetFightData()
	local form = self:GetCurForm()
	local list = {}
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId()
		local obj
		if IsUserId(roleId) then
			obj = self
		else
			local hero = self:GetHero(roleId)
			if hero then
				obj = hero
			end
		end
		if obj then
			local buf = table.deepcopy(obj:GetFightAttr())
			buf.pos = k
			table.insert(list, buf)
		end
	end
	return list
end

--获取玩家的战斗属性
function clsUser:GetFightAttr()
	local buff = table.copy(self:GetFightAttrList())
	buff.roleId = self:GetRoleId()
	buff.identity = "user"
	buff.name = self:GetName()
	buff.headId = self:GetHeadId()
	buff.modelId = self:GetModelId()
	buff.Lv = self:GetLv()
	buff.job = self:GetJob()
	buff.aptLv = self:GetAptLv()
	buff.force = self:GetFightForce()
	
	local pet = self:GetPet()
	if pet and pet:GetForm() ~= 0 then
		local skillId = pet:GetSkillId()
		local skillLv = pet:GetSkillLv()
		if skillId and skillLv > 0 then
			buff.petId = pet:GetId()
			buff.petModelId = pet:GetModelId()
			buff.stuntId = skillId
			buff.stuntLv = skillLv
		end
		buff.morale = pet:GetTalentAdd(311014)
		buff.rage = pet:GetTalentAdd(311015)
	end
	return buff
end

function clsUser:OfflineProFight()
	FIGHTREC.ClearFightRecord(self:GetRoleId())
	local fightId = self:GetLastFight()
	if fightId then
		local fight = FIGHT.GetOneFight(fightId)
		if fight then
			if table.size(fight:GetUserList()) > 1 then
				fight:ModUserList(self:GetRoleId())
				self:SetWorkInfo("standby")
			else
				FIGHT.RemoveOneFight(fightId)
			end
		end
	end
end


function clsUser:SetWorkInfo(state, place, startTime, endTime)
	self.__save.state = self.__save.state or {}
	self.__save.state.state = state
	self.__save.state.place = place
	self.__save.state.startTime = startTime or os.time()
	if state == "fighting" then
		self.__save.state.endTime = os.time() + FIGHT_CD_TIME
		self:StartFightCDTimer()
		self:SetLastFight(place)
	end
	self.__save.state.endTime = endTime
end

function clsUser:StartFightCDTimer()
	local info = self:GetWorkInfo()
	if info.state == "fighting" then
		local timelong = (info.endTime or os.time()) - os.time()
		if timelong <= 0 then
			self:SetWorkInfo("standby")
		else
			self.__temp.fightCdTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.SetWorkInfo, timelong, self, "standby")
		end
	end
end

function clsUser:RemoveFightCDTimer()
	if self.__temp.fightCdTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.fightCdTimer)
	end
end

--记录上一场战斗
function clsUser:SetLastFight(fightId)
	self.__temp.lastFight = fightId
end

function clsUser:GetLastFight()
	return self.__temp.lastFight
end

--------------------------------------------------------------
--交易
function clsUser:SetTrade(tradeObj)
	self.__temp.trade = tradeObj
end

function clsUser:GetTrade()
	return self.__temp.trade
end

------------------------------------------------
--玩家关注点

function clsUser:SetFocus(type, id)
	self.__temp.focus = {type = type, id = id}
end

function clsUser:GetFocus()
	self.__temp.focus = self.__temp.focus or {}
	return self.__temp.focus
end
------------------------------------------------
--设置项	-- ["type"] = {set = 1/0, daily = timestamp}
function clsUser:SetSetting(type, data)
	self.__save.setting = self.__save.setting or {}
	if not data or data == 0 then
		self.__save.setting[tostring(type)] = nil
	else
		self.__save.setting[tostring(type)] = self.__save.setting[tostring(type)] or {}
		local info = self.__save.setting[tostring(type)]
		info.set = data
		if SETTING.IsDailySetting(type) then
			info.daily = os.time()
		end
	end
end

function clsUser:GetSetting(type)
	self.__save.setting = self.__save.setting or {}
	local info = self.__save.setting[tostring(type)]
	if not info or info.set == 0 then
		return
	elseif info.daily and CalDiffDays(info.daily, os.time()) > 0 then
		return
	end
	return info.set
end

function clsUser:GetSettingList()
	self.__save.setting = self.__save.setting or {}
	return self.__save.setting
end

------------------------------------------------
--打坐
function clsUser:SetMeditate()
	local buff = {}
	buff.startTime = os.time()
	buff.endTime = os.time() + MEDITATE_LAST_TIME
	buff.timestamp = os.time()
	buff.expGot = 0
	self.__save.meditate = buff
	self:StartMeditateCallOut()
--	local locate = self:GetLocateInfo()
--	self:Broad2ExNeighbor("zazen", locate.x, locate.y)
end

function clsUser:IsInMeditate()
	return self.__save.meditate
end

function CalMediteExp(baseExp, timelong)
	return math.floor(baseExp * timelong / 60)
end

--处理修炼
function clsUser:ProMeditate()
	local meditate = self:IsInMeditate()
	if not meditate then
		return
	end
	local timelong = math.min(os.time(), meditate.endTime) - meditate.timestamp
	if timelong >= MEDITATE_UPDATE_TIMELONG or (meditate.timestamp + timelong >= meditate.endTime) then
		local exp = CalMediteExp(self:GetMeditateExp(), timelong)
--		self:SetExp(exp, "meditate")
		meditate.expGot = meditate.expGot + exp
		meditate.timestamp = meditate.timestamp + timelong
		self:SendMeditateUpdate("meditate", meditate.expGot)
	end
	if meditate.timestamp < meditate.endTime then
		self:StartMeditateCallOut()
--	else
--		self:CancelMeditate()
	end
end

--领取修炼经验
function clsUser:RecieveMeditateExp()
	local expGot = 0
	local meditate = self:IsInMeditate()
	if meditate then
		expGot = meditate.expGot
		self:RemoveMeditateCallOut()
	end
	self:SetTeamExp(expGot)
	self:SetMeditate()
	self:SendMeditateUpdate("meditate", 0, 0)
	return expGot
end

function clsUser:SendMeditateUpdate(type, expGot, timelong)
	local sData = {type = type, expGot = expGot, timelong = timelong}
	self:SendProtocol("c_meditate_update", sData)
end

function clsUser:StartMeditateCallOut()
	self.__temp.meditateTimer = CALLOUT.ObjCallOut(self:GetCallOut(), clsUser.ProMeditate, MEDITATE_UPDATE_TIMELONG, self)
end

function clsUser:RemoveMeditateCallOut()
	if self.__temp.meditateTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.meditateTimer)
	end
end

function clsUser:CancelMeditate()
	self:RemoveMeditateCallOut()
	self.__save.meditate = nil
	self:SendMeditateUpdate("over")
	local locate = self:GetLocateInfo()
	self:Broad2ExNeighbor("stand", locate.x, locate.y, true)
end

----------------------------------------------------

function clsUser:SetChatTimestamp(data)
	self.__temp.chatTmsp = data
end

function clsUser:GetChatTimestamp()
	return self.__temp.chatTmsp
end

function clsUser:SendOnlineMsg()
	local friend = self:GetFriendList()
	local msg = string.format("您的好友%s上线了", self:GetName())
	for k, v in pairs(friend:GetMemberList("friend")) do
		if v.type == 1 then
			local target = USER.GetOnlineUser(k)
			if target and target:GetUnionId() == self:GetUnionId() then
				CHAT.SendSysMsg("world", msg, {k})
			end
		end
	end
	RANK.SendOnlineMsg(self:GetRoleId())
	local union = UNION.GetUnionById(self:GetUnionId())
	if union then
		union:SendUnionMsg(string.format("【%s】上线了", self:GetName()))
	end
end

function clsUser:ClearTechInfo()
	local tribe = self:GetTribe()
	if tribe then
		local academy = tribe:GetStruct("academy")
		if academy then
			academy:ClearTech()
		end
	end
end

-------------------------------------------------------------------------

function clsUser:AddUpgradeList(type, id, obj)
	self.__temp.upgradeList[type] = self.__temp.upgradeList[type] or {}
	self.__temp.upgradeList[type][id] = obj
end

function clsUser:GetUpgradeListByType(type)
	return self.__temp.upgradeList[type] or {}
end

function clsUser:SetUpgradeListState(type, id, state)
	if not self.__temp.upgradeList[type] or not self.__temp.upgradeList[type][id] then
		return
	end
	self.__temp.upgradeList[type][id]:SetState(state)
end

function clsUser:GetOneUpgradeList(type, id)
	self.__temp.upgradeList[type] = self.__temp.upgradeList[type] or {}
	return self.__temp.upgradeList[type][id]
end

function clsUser:GetIdleUpgradeList(type)
	local list = self:GetUpgradeListByType(type)
	for k, v in pairs(list) do
		if v:ListCanUse() then
			return v
		end
	end
end

function clsUser:SaveUpgradeList()
	self.__save.upgradeList = {}
	for k, v in pairs(self.__temp.upgradeList or {}) do
		self.__save.upgradeList[k] = self.__save.upgradeList[k] or {}
		for id, obj in pairs(v) do
			self.__save.upgradeList[k][id] = obj:GetSaveData()
		end
	end
end

function clsUser:RestoreUpgradeList()
	self.__temp.upgradeList = {}
	for k, v in pairs(self.__save.upgradeList or {}) do
		self.__temp.upgradeList[k] = self.__temp.upgradeList[k] or {}
		for id, data in pairs(v) do
			local obj = UPGRADELIST.NewList(data.type, data.id, data.state, self:GetRoleId())
			obj:Restore(data)
			self.__temp.upgradeList[k][id] = obj
		end
	end
end

function clsUser:GetAllUpgradeList()
	return self.__temp.upgradeList
end

function clsUser:GetUpgradeList()
	local sData = {}
	for k, v in pairs(self.__temp.upgradeList or {}) do
		for id, obj in pairs(v) do
			local buf = {}
			local timelong, times, maxTimes, state = self:GetUpgradeTimestamp(obj:GetType(), obj:GetId())
			buf.type = obj:GetType()
			buf.state = state
			buf.index = obj:GetId()
			buf.timelong = timelong 
			buf.times = times
			buf.maxTimes = maxTimes
			table.insert(sData, buf)
		end
	end
	if self:IsFuncOpen(SETTING.FUNC_TEAM_COPY) then
		table.insert(sData, {times = self:GetDailyTeamCopyNum(), maxTimes= self:GetDailyTeamCopyMax(), index = "1", type = "teamcopy"})
	end	
	if self:IsFuncOpen(SETTING.FUNC_PHYLE_CHOOSE) then
		table.insert(sData, {times = self:GetPillageTimes(), maxTimes = self:GetMaxPillageTimes(), index = "1", type = "pillage"})
		local city = CITY.GetCityObjById(self:GetCityId())
		if city and city:GetOwner() == roleId then
			table.insert(sData, {times = self:GetCityAttackTimes(), maxTimes = self:GetMaxCityAttackTimes(), index = "1", type = "cityAttack"})
		end
	end
	return sData
end

