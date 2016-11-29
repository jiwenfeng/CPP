--联盟遗迹

local FEED_NUM_PER_LV = 50			--每级最大可喂食次数
local MAX_BOSS_LV = 6				--最大boss等级
local TOP_RANK_NUM = 10				--伤害排名榜前列
local REVIVE_TIME_LONG = 30			--复活等该时间
local FIHGT_READY_TIME = 3 * 60	--战斗准备时间
local FIGHT_LAST_TIME = 60 * 60		--饕餮战持续时间
local RANK_FRESH_TIME = 10			--排行榜刷新间隔
local MAX_EMBRAVE_ADD = 0.5			--鼓舞加成上限
local DAMAGE_TO_GOLD_RATE = 0.1		--伤害与金钱获得成正比

local RELIC_MAP_ID = "697102"		--遗迹场景mapId
local RELIC_BOSS_ID = "260465"		--遗迹场景bossId
local BOSS_COLUMN_X = 850		--boss栏杆大小
local BOSS_COLUMN_Y = 350		--boss栏杆大小

local READY_AREA_X = 300			--准备区域右侧坐标
local MAX_FIGHT_ROUND = 3			--最大战斗回合

local damageList = {
	[1820] = {min = 20, max = 30}, 
	[2490] = {min = 30, max = 40},
	[3470] = {min = 40, max = 50},
	[4240] = {min = 50, max = 60},
	[5550] = {min = 60, max = 70},
	[6450] = {min = 70, max = 80},
	[7460] = {min = 80, max = 90},
	[8100] = {min = 90, max = 100},
	[9270] = {min = 100, max = 110},
	[10140] = {min = 110, max = 120},
	[11370] = {min = 120, max = 130},
	[12300] = {min = 130, max = 140},
	[14040] = {min = 140, max = 150},
	[14380] = {min = 150, max = math.huge},
}

function GetDamageByLv(lv)
	for k, v in pairs(damageList) do
		if lv >= v.min and lv < v.max then
			return k
		end
	end
end

--鼓舞类型
local embraveList = {
	["ingot"] = {cost = 5, rate = 0.5, add = 0.02},
	["devote"] = {cost = 5, rate = 0.4, add = 0.02},
}

function GetEmbraveInfoByType(type)
	return embraveList[type]
end

--------------------------------------------

clsUniRelic = clsObject:Inherit()

CLSUNION.SetActClassById("uniRelic", clsUniRelic)

function clsUniRelic:__init__()
	Super(clsUniRelic).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsUniRelic:initsavedata()
	self.__save.bossLv = 0
	self.__save.feedNum = 0
	self.__save.state = nil
	self.__save.iskilled = false
	self.__save.feedList = self.__save.feedList or {}
end

function clsUniRelic:inittempdata()
	self.__temp.callout = {}
end

function clsUniRelic:InitUniAct(unionId)
	self:SetOwner(unionId)
	self:ResetBossLv(1)
	self:SetFeedNum(0)
	self:SetState(CLSUNION.ACT_UNSTART)
	
	self:SetBossKilled(false)
	
	local sceneObj = SCENE.NewScene("uniRelic", RELIC_MAP_ID)
	self:SetRelicScene(sceneObj)
	sceneObj:InitNpcList(true)
end

--重置boss
function clsUniRelic:ResetBoss()
	local sceneObj = self:GetRelicScene()
	local boss = sceneObj:GetKeyNpc()
	if boss then
		NPC.NpcLeave(boss)
	end
	self:ResetBossLv(1)
	self:SetFeedNum(0)
	self:SetState(CLSUNION.ACT_UNSTART)
	self:SetBossReleased()
	self:SetBossKilled(false)
	sceneObj:InitNpcList(true)
	self:SendBossInfoUpdate()
end

function clsUniRelic:GetBossHeadId()
	local sceneObj = self:GetRelicScene()
	local boss = sceneObj:GetKeyNpc()
	if boss then
		local info = FIGHT.GetMonsterTeamInfoById(boss:GetId())
		return info.headId
	end
end

function clsUniRelic:SetOwner(unionId)
	self.__save.owner = unionId
end

function clsUniRelic:GetOwner()
	return self.__save.owner
end

function clsUniRelic:SetState(state)
	self.__save.state = state
end

function clsUniRelic:GetState()
	return self.__save.state
end

--boss喂养次数
function clsUniRelic:GetMaxFeedNum()
	local Lv = self:GetBossLv()
	return Lv * FEED_NUM_PER_LV
end

function clsUniRelic:GetFeedNum()
	return self.__save.feedNum
end

function clsUniRelic:SetFeedNum(num)
	self.__save.feedNum = (self.__save.feedNum or 0) + num
	if self:GetBossLv() >= MAX_BOSS_LV then
		self.__save.feedNum = self:GetMaxFeedNum()
	else
		while (self.__save.feedNum >= self:GetMaxFeedNum() and self:GetBossLv() < MAX_BOSS_LV) do
			self:BossUpgrade()
		end
	end
end
--boss等级
function clsUniRelic:GetBossLv()
	return self.__save.bossLv
end

function clsUniRelic:SetBossLv(lv)
	self.__save.bossLv = (self.__save.bossLv or 0) + lv
	if self.__save.bossLv > MAX_BOSS_LV then
		self.__save.bossLv = MAX_BOSS_LV
	elseif self.__save.bossLv < 1 then
		self.__save.bossLv = 1
	end	
end

function clsUniRelic:ResetBossLv(data)
	self.__save.bossLv = data
end

function clsUniRelic:BossUpgrade()
	self:SetBossLv(1)
end
--boss是否被击杀
function clsUniRelic:IsBosskilled()
	return self.__save.iskilled
end

function clsUniRelic:SetBossKilled(iskill)
	self.__save.iskilled = iskill
end
--喂养boss
function clsUniRelic:SetBossFeed(roleId, data)
	self.__save.feedList[roleId] = self.__save.feedList[roleId] or {}
	local info = self.__save.feedList[roleId]
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
	self:SetFeedNum(data)
end

function clsUniRelic:IsBossFeed(roleId)
	self.__save.feedList[roleId] = self.__save.feedList[roleId] or {}
	local info = self.__save.feedList[roleId]
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return
	end
	return info.num
end
--遗迹场景
function clsUniRelic:GetRelicScene()
	return self.__temp.scene
end

function clsUniRelic:SetRelicScene(sceneObj)
	self.__temp.scene = sceneObj
end

------------------------------------------------------

--活动时间戳
function clsUniRelic:SetBossActTimestamp(data)
	self.__temp.actTimestamp = data
end

function clsUniRelic:GetBossActTimestamp()
	return self.__temp.actTimestamp
end

function clsUniRelic:IsBossReleased()
	return self.__save.bossReleased
end

function clsUniRelic:SetBossReleased(state)
	self.__save.bossReleased = state
end

--释放boss
function clsUniRelic:BossActReady(roleId)
	self:SetState(CLSUNION.ACT_READY)
	self:InitBossAct()
	self:SetBossActTimestamp(os.time() + FIHGT_READY_TIME)
	self:UpdateUniRelicInfo()
	self:SendBossInfoUpdate()
	self:SetBossReleased(1)
	
	local union = UNION.GetUnionById(self:GetOwner())	
	local msg
	if roleId then
		msg = string.format("%s释放了凶兽饕餮，各位成员速速前往讨伐，击杀后将获得丰厚的奖励。", USER.GetUserNickById(roleId))
	else
		msg = "凶兽饕餮挣出牢笼，正在帮派遗迹大肆破坏。各位成员速速前往讨伐，击杀后将获得丰厚的奖励。"
	end
	CHAT.SendSysMsg("union", msg, union:GetListByLevel(CLSUNION.COMMON), {kind = "uniRelic"})
	self:RunActBeginCallOut()
end


function clsUniRelic:BossActBegin()
--	self:SetBossActTimestamp(os.time())
--	self:UpdateUniRelicInfo()
	if self:GetState() ~= CLSUNION.ACT_READY then
		return
	end
	self:ClearActBeginCallOut()
	self:SetState(CLSUNION.ACT_BEGIN)
	self:SetBossActTimestamp(os.time() + FIGHT_LAST_TIME)
	local sceneObj = self:GetRelicScene()
	local npcObj = sceneObj:GetKeyNpc()
	npcObj:SetFightable(true)
	self:UpdateUniRelicInfo()
	self:SendBossInfoUpdate()
	
	self:RunActOverCallOut()
	self:RunRankUpdateCallOut()
end

--boss脱逃
function clsUniRelic:BossEscape()
	if self:GetState() ~= CLSUNION.ACT_BEGIN then
		return
	end
	self:SetState(CLSUNION.ACT_OVER)
	local sceneObj = self:GetRelicScene()
	local npcObj = sceneObj:GetKeyNpc()
	if npcObj then
		NPC.NpcLeave(npcObj)
	end
	self:SetBossKilled(true)
	self:ReviveAllFighter()
	self:UpdateUniRelicInfo()
	
	local union = UNION.GetUnionById(self:GetOwner())
	if not union then
		union:SendUnionMsg("饕餮已遁形逃脱， 击杀失败")
		local rankObj = self:GetRank()
		if rankObj then
			RANK.DestroyRank(rankObj)
		end
		self:SetRank()
	end
end

--初始化boss战数据
function clsUniRelic:InitBossAct()
	self.__temp.boss = {}
	local hpMax = self:CalBossHp()
	self.__temp.boss.hpMax = hpMax
	self.__temp.boss.hp = hpMax
	self.__save.killer = false
	self.__temp.fighter = {}
	
	self:InitRank()
end

--计算boss血量
function clsUniRelic:CalBossHp()
	local union = UNION.GetUnionById(self:GetOwner())
	local avgLv, num = union:GetMemberAvgLv()
	print("ping ju deng ji shi :", avgLv)
	local avgDamage = GetDamageByLv(avgLv)
	return 180 * num * avgDamage
end

function clsUniRelic:GetBossHpMax()
	self.__temp.boss = self.__temp.boss or {}
	return self.__temp.boss.hpMax
end

function clsUniRelic:GetBossCurHp()
	self.__temp.boss = self.__temp.boss or {}
	return self.__temp.boss.hp
end

function clsUniRelic:SetBossCurHp(data)
	if not self.__temp.boss then
		return
	end
	data = assert(tonumber(data))
	self.__temp.boss.hp = self.__temp.boss.hp + data
	if self.__temp.boss.hp < 0 then
		self.__temp.boss.hp = 0
	end
end
--boss杀手
function clsUniRelic:SetBossKiller(roleId)
	self.__save.killer = roleId
end

function clsUniRelic:GetBossKiller()
	return self.__save.killer
end
--玩家参战信息
function clsUniRelic:GetFighterList()
	self.__temp.fighter = self.__temp.fighter or {}
	return self.__temp.fighter
end

function clsUniRelic:GetFighterInfo(roleId)
	self.__temp.fighter = self.__temp.fighter or {}
	self.__temp.fighter[roleId] = self.__temp.fighter[roleId] or {}
	return self.__temp.fighter[roleId]
end

function clsUniRelic:GetBeatDamage(roleId)
	local info = self:GetFighterInfo(roleId)
	return info.damage or 0
end

function clsUniRelic:SetBeatDamage(roleId, damage)
	local info = self:GetFighterInfo(roleId)
	info.damage = (info.damage or 0) + damage
	self:RenewRank(roleId, info.damage)
end


--初始化伤害排行榜
function clsUniRelic:InitRank()
	local rankObj = RANK.NewRank("real", "bossDmg")
	self.__temp.rank = rankObj
end

function clsUniRelic:GetRank()
	return self.__temp.rank
end

function clsUniRelic:SetRank(obj)
	self.__temp.rank = obj
end

--更新排行榜
function clsUniRelic:RenewRank(roleId, damage)
	local rankObj = self:GetRank()
	rankObj:AddToRank({roleId = roleId, damage = damage})
end
--获取排行榜排名
function clsUniRelic:GetTopRank(roleId)
	local rankObj = self:GetRank()
	if rankObj then
		return rankObj:GetTopRank(TOP_RANK_NUM, roleId, self:GetBossHpMax())
	end
end

--战斗开始
function clsUniRelic:StartFight(user)
	local roleId = user:GetRoleId()
	local sceneObj = self:GetRelicScene()
	local boss = sceneObj:GetKeyNpc()
	if not boss then
		return
	elseif self:IsRoleDead(roleId) then
		return
	end
	local function callback(fightId, winner, prize, stats)
		user:SetWorkInfo("standby")
		local damage = stats.atkDamage
		self:SetBossCurHp(-damage)
		self:SetBeatDamage(roleId, damage)
		local gold = math.floor(damage * DAMAGE_TO_GOLD_RATE)
		user:SetGold(gold)
		if winner == "attack" then
			if not self:GetBossKiller() then
				self:SetBossKiller(roleId)
				self:SetBossKilled(true)
				self:BossActOver(roleId)
			end
			self:SetRoleState(roleId, "normal")
	--	elseif not self:IsBosskilled() then
		else
			self:SetRoleDead(roleId)
			local sceneObj = self:GetRelicScene()
			local revivalPos = sceneObj:GetBornPoint()			
			user:Goto(revivalPos.x, revivalPos.y)
		end
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			local result = winner == "attack" and 1 or 0
			local sData = {result = result}
			if not result and not self:IsBosskilled() then
				sData.reviveTime = self:GetFightColdTime() - os.time()
			end
			con:SendProtocol("c_uniRelic_fight", sData)
		end
	end
	local param = {bossCurHp = self:GetBossCurHp(), bossHpMax = self:GetBossHpMax()}
	local locate = user:GetLocateInfo()
	user:SetWorkInfo("fighting", nil, nil, nil)
	self:SetRoleState(roleId, "fighting")
	local fightId = FIGHT.BeginFight("uniBoss", roleId, boss:GetId(), {roleId}, callback, param, {posX = locate.x, posY = locate.y})
end

function clsUniRelic:IsUserAutoFight(roleId)
	local info = self:GetFighterInfo(roleId)
	return info and info.autoFight == 1
end

function clsUniRelic:GetFightColdTime(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	return info.timestamp
end

function clsUniRelic:IsRoleDead(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	return info.timestamp and info.timestamp > os.time()
end

function clsUniRelic:SetRoleDead(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	info.timestamp = os.time() + REVIVE_TIME_LONG
	info.state = "dead"			-- 后面的 goto 方法会更新死亡状态，所以这里就不用更新了
	self:StartReviveCallOut(roleId)
end

function clsUniRelic:SetRoleRevive(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	self:RemoveReviveCallOut(roleId)
	info.timestamp = os.time()
	self:SetRoleState(roleId, "normal")
	if info.autoFight then

	end
end

--启动复活定时器
function clsUniRelic:StartReviveCallOut(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then
		return
	end
	local timelong = info.timestamp - os.time()
	if timelong <= 0 then
		self:SetRoleRevive(roleId)
	else
		info.reviveTimer = CALLOUT.CallOut(clsUniRelic.SetRoleRevive, timelong, self, roleId)
	end
end

function clsUniRelic:RemoveReviveCallOut(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then
		return
	end
	if info.reviveTimer then
		CALLOUT.RemoveCallOut(getfenv(2), info.reviveTimer)
	end
end

function clsUniRelic:SetRoleState(roleId, state)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	info.state = state
	local scene = self:GetRelicScene()
	local user = scene:GetSceneRole(roleId)
	if user then
		local locate = user:GetLocateInfo()
		user:Broad2ExNeighbor("stand", locate.x, locate.y, true)
	end
end

function clsUniRelic:GetRoleState(roleId)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	return info.state or "normal"
end

--boss被斩杀
function clsUniRelic:BossActOver(roleId)
	self:SetState(CLSUNION.ACT_OVER)
	local sceneObj = self:GetRelicScene()
	local npcObj = sceneObj:GetKeyNpc()
	if npcObj then
		NPC.NpcLeave(npcObj)
	end
	self:ReviveAllFighter()
	self:UpdateUniRelicInfo()
	self:CheckActAchv()
	
	local union = UNION.GetUnionById(self:GetOwner())
	union:SendUnionMsg("饕餮已被成功击杀，所有参战成员都将获得饕餮礼包")
	self:SendReward()
	self:ClearActOverCallOut()
	self:ClearRankUpdateCallOut()
	RANK.DestroyRank(self:GetRank())
	self:SetRank()
end

--boss战开启定时器
function clsUniRelic:RunActBeginCallOut()
	self.__temp.actBeginTimerId = CALLOUT.ObjCallOut(self.__temp.callout, clsUniRelic.BossActBegin, FIHGT_READY_TIME, self)
end

function clsUniRelic:ClearActBeginCallOut()
	if self.__temp.actBeginTimerId then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.actBeginTimerId)
	end
end

--boss脱逃定时器
function clsUniRelic:RunActOverCallOut()
	self.__temp.actOverTimerId = CALLOUT.ObjCallOut(self.__temp.callout, clsUniRelic.BossEscape, FIGHT_LAST_TIME, self)
end

function clsUniRelic:ClearActOverCallOut()
	if self.__temp.actOverTimerId then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.actOverTimerId)
	end
end
--更新伤害排名
function clsUniRelic:SendRankUpdate()
	local sceneObj = self:GetRelicScene()
	local list = sceneObj:GetSceneRoleList()
	for k in pairs(list) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			local sData = self:GetTopRank(k)
			con:SendProtocol("c_uniRelic_rank", sData)
		end
	end
	self:SendBossInfoUpdate(nil, true)
end

function clsUniRelic:SendBossInfoUpdate(roleId, flag)
	local state = self:GetState()
	local sData = {}
	if not flag then
		sData.bossLv = self:GetBossLv()
		sData.headId = self:GetBossHeadId()
	end
	if state == CLSUNION.ACT_BEGIN then
		if not flag then
			sData.hpMax = self:GetBossHpMax()
			sData.timelong = self:GetBossActTimestamp() - os.time()
		end
		sData.hp = self:GetBossCurHp()
	end
	if roleId then
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_uniRelic_bossInfo", sData)
		end
	else
		local sceneObj = self:GetRelicScene()
		local list = sceneObj:GetSceneRoleList()
		for k in pairs(list) do
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_uniRelic_bossInfo", sData)
			end
		end
	end
end

function clsUniRelic:RunRankUpdateCallOut()
	self.__temp.rankTimerId = CALLOUT.CallOutInRange(clsUniRelic.SendRankUpdate, os.time(), os.time() + FIGHT_LAST_TIME, RANK_FRESH_TIME, self)
end

function clsUniRelic:ClearRankUpdateCallOut()
	if self.__temp.rankTimerId then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.rankTimerId)
	end
end

--鼓舞加成
function clsUniRelic:SetRoleEmbrave(roleId, type, rate)
	local info = self:GetFighterInfo(roleId)
	if not info then return end
	info[type] = (info[type] or 0) + rate
	if info[type] > MAX_EMBRAVE_ADD then
		info[type] = MAX_EMBRAVE_ADD
	elseif info[type] < 0 then
		info[type] = 0
	end
end

function clsUniRelic:GetRoleEmbrave(roleId, type)
	local info = self:GetFighterInfo(roleId)
	if not info then return 0 end
	return info[type] or 0
end

--是否可以释放boss
function clsUniRelic:CanBeRelease(roleId)
	local union = UNION.GetUnionById(self:GetOwner())
	local roleInfo = union:GetMemberInfo(roleId)
	if roleInfo.identity > CLSUNION.VICELEADER then
		return false
	end
	local tm = os.date("*t")
	if tm.wday ~= 6 then
		return false
	end
	local state = self:GetState()
	if state ~= CLSUNION.ACT_UNSTART then
		return false
	end
	return true
end

function clsUniRelic:GetUniRelicInfo(roleId)
	local state = self:GetState()
	local sData = {state = state}
	if state == CLSUNION.ACT_UNSTART then 
		sData.bossLv = self:GetBossLv()
		sData.feedNum = self:GetFeedNum()
		sData.feedMax = self:GetMaxFeedNum()
		sData.isFeed = self:IsBossFeed(roleId) and 1 or 0
		sData.canRelease = self:CanBeRelease(roleId)
	elseif state == CLSUNION.ACT_OVER then
		sData.isFeed = 0
		sData.canRelease = 0
	else
		sData.isFeed = self:IsBossFeed(roleId) and 1 or 0
		sData.canRelease = 0
		sData.ingotTtl = self:GetRoleEmbrave(roleId, "ingot")
		sData.devoteTtl = self:GetRoleEmbrave(roleId, "devote")
		if self:IsRoleDead(roleId) then
			sData.reviveTime = self:GetFightColdTime(roleId) - os.time()
		end
		if state == CLSUNION.ACT_READY then
			sData.timelong = self:GetBossActTimestamp() - os.time()
		end
	end
	local ingEmbrave = GetEmbraveInfoByType("ingot")
	sData.ingotNeed = ingEmbrave.cost
	sData.ingotAdd = ingEmbrave.add
	local devEmbrave = GetEmbraveInfoByType("devote")
	sData.devoteNeed = devEmbrave.cost
	sData.devoteAdd = devEmbrave.add
	sData.autoFight = self:IsUserAutoFight(roleId)
	return sData
end
--向遗迹中所有人更新遗迹信息
function clsUniRelic:UpdateUniRelicInfo()
	local sceneObj = self:GetRelicScene()
	for k, v in pairs(sceneObj:GetSceneRoleList()) do
		if v:GetRoleType() == "user" then
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				local sData = self:GetUniRelicInfo(k)
				con:SendProtocol("c_uniRelic_info", sData)
			end
		end
	end
end
--战斗结束后复活所有玩家
function clsUniRelic:ReviveAllFighter()
	for k, v in pairs(self:GetFighterList()) do
		v.state = "normal"
		self:RemoveReviveCallOut(k)
	end
end


function clsUniRelic:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsUniRelic:Restore(data)
	self.__save = data
	if not self:GetRelicScene() then
		local sceneObj = SCENE.NewScene("uniRelic", RELIC_MAP_ID)
		self:SetRelicScene(sceneObj)
		
		if not self:IsBosskilled() then
			sceneObj:InitNpcList(true)
		end
	end
end


function clsUniRelic:CheckActAchv()
	local function set_achv_got(roleId, achvId)		
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetAchvGot(achvId)
		else
			OFFLINE.SetOfflineData(roleId, "achvGot", achvId)
		end
	end
	local achvId
	for k, v in pairs(ACHV.GetAllAchvList()) do
		if v.label == "unionBoss" then
			achvId = k
			break
		end
	end
	if achvId then
		local rank = self:GetRank()
		for rank, data in pairs(rank:GetRankList()) do
			set_achv_got(data.roleId, achvId)
		end
	end
end


function clsUniRelic:ShutDownProAct()
	local state = self:GetState()
	if state == CLSUNION.ACT_READY then
		self:ClearActBeginCallOut()
		self:SetState(CLSUNION.ACT_BEGIN)
	elseif state == CLSUNION.ACT_BEGIN then
		self:ClearActOverCallOut()
		self:ClearRankUpdateCallOut()		
	end
	self:BossEscape()
end


function clsUniRelic:Destroy()
	self:ShutDownProAct()
	local sceneObj = self:GetRelicScene()
--[[	for k, v in pairs(sceneObj:GetSceneRoleList()) do
		v:LeaveScene()
	end
	sceneObj:Destroy()]]
	sceneObj:SetAutoRelease(true)
	self:SetRelicScene()
	Super(clsUniRelic):Destroy()
end


function clsUniRelic:GetSealBarState()
	local state = self:GetState()
	local sData = {}
	sData.type = state == CLSUNION.ACT_BEGIN and "open" or "close"
	sData.posX = BOSS_COLUMN_X
	sData.posY = BOSS_COLUMN_Y
	return sData
end

function clsUniRelic:SendReward()
	local rewardList = UNIRELIC.GetRewardListByQuality(self:GetBossLv())
	local rankObj = self:GetRank()
	for rank, data in pairs(rankObj:GetRankList()) do
		for k, v in pairs(rewardList.list) do
			if v.min <= rank and rank <= v.max then
				local item = BASEITEM.NewItem(rewardList.chestId, v.num)
				local user = USER.GetOnlineUser(data.roleId)
				if user and user:IsItemCanAdd(USERBASE.PACKAGE, item) then
					user:AddItem(USERBASE.PACKAGE, item)
					user:SendProtocol("c_show_tips", "已获得饕餮礼包")
				else
					local mail = {}
					mail.title = "饕餮战奖励"
					mail.content = "您在击杀饕餮活动中表现优秀，获得饕餮宝箱作为奖励，请及时领取"
					mail.attach = {}
					mail.attach.item = {}
					table.insert(mail.attach.item, {id = rewardList.chestId, num = v.num})
					CHAT.SendSysMail(data.roleId, mail)
					user:SendProtocol("c_show_tips", "饕餮宝箱已通过邮件发送")
				end
			end
		end
	end 
end

--联盟开启秘境功能
function SetUniRelicOpen(union)
	local uniRelic = clsUniRelic:New()
	uniRelic:InitUniAct(union:GetId())
	union:SetUnionAct("uniRelic", uniRelic)
end


