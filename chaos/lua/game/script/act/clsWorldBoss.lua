--世界boss

local WorldBossInfoList = {}

local ACT_READY_TIME = 5 * 60		--活动准备时间
local OVER_STAY_TIMGLONG = 60		--活动结束后停留时间
local FIGHT_LAST_TIME = 30 * 60		--活动持续时间
local REVIVE_TIMELONG = 30			--复活时长
local REVIVE_COST = 10
local BOSS_COLUMN_X = 850		--boss栏杆大小
local BOSS_COLUMN_Y = 350		--boss栏杆大小

local KILLER_REWARD = 500000		--补刀奖励
local MAX_REWARD_RANK = 20			--最大奖励名次
local UPDATE_FREQUENCY = 3			--排行榜刷新间隔
local TOP_RANK_NUM = 10
local DYING_HP_PERCET = 0.1			--boss即将死亡的提示血量

local MIN_ONLINE_USER_NUM = 20
local MIN_USER_AVG_LV = 25
local DAMAGE_NOTICE_NUM = 5			--需要公告的伤害排名人数

--排名奖励
local RankReword = {[1] = 500000, [2] = 200000, [3] = 100000, [4] = 50000}
local BufferList = {
	["1"] = {type = "ingot", num = 5,rate = 0.3, max = 1, promote = 0.1, name = "战神祝福"},
	["2"] = {type = "nimbus", num = 5000, rate = 0.5,max = 0.5, promote = 0.05, name = "灵动九天"}
}

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
	if not lv then
		return 0
	end
	for k, v in pairs(damageList) do
		if v.min <= lv and lv < v.max then
			return k
		end
	end
end

function CalBossHpMax()
	local sumLv, num = 0, 0 
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:GetLv() >= 20 then
			sumLv = sumLv + v:GetLv()
			num = num + 1
		end
	end
	local avgLv = math.ceil(sumLv / num)
	num = num < MIN_ONLINE_USER_NUM and MIN_ONLINE_USER_NUM or num
	local damage = GetDamageByLv(avgLv) or 0
	return 36 * num * damage
end

function GetRewardByRank(rank)
	if rank > MAX_REWARD_RANK then
		return
	elseif rank >= 4 then
		return RankReword[4]
	else
		return RankReword[rank]
	end
end

function GetAllWorldBossList()
	return WorldBossInfoList
end

function GetWorldBossInfoById(id)
	return WorldBossInfoList[id]
end

function GetBufferMax(bufferId)
	return BufferList[bufferId].max
end

function GetBufferInfo(bufferId)
	return BufferList[bufferId]
end

function GetRevivalCost()
	return REVIVE_COST
end

----------------------------------------------
clsBossWar = clsObject:Inherit()


function clsBossWar:__init__()
	Super(clsBossWar).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
	self:initsavedata()
end

function clsBossWar:inittempdata()
	self.__temp.userList = {}
	self.__temp.callout = {}
end

function clsBossWar:initsavedata()
end

function clsBossWar:InitBossWar(id)
	self:SetId(id)
	self:SetState("ready")
	self:SetBossActTimestamp(os.time() + ACT_READY_TIME)
	
	local info = GetWorldBossInfoById(id)
	local sceneObj = SCENE.NewScene("worldBoss", info.mapId)
	sceneObj:InitNpcList(true)
	self:SetScene(sceneObj)
	self:InitBossData()
	self:InitRankList()
	self:SendWorldBossMsg("ready")
	self:RunActBeginCallOut()
end

--战斗id
function clsBossWar:SetId(id)
	self.__temp.id = id
end

function clsBossWar:GetId()
	return self.__temp.id
end
--状态
function clsBossWar:SetState(state)
	self.__temp.state = state
end

function clsBossWar:GetState()
	return self.__temp.state
end

function clsBossWar:GetBossName()
	local info = GetWorldBossInfoById(self:GetId())
	local monster = FIGHT.GetMonsterInfoById(info.npcId)
	return monster.name
end

function clsBossWar:GetActName()
	local info = GetWorldBossInfoById(self:GetId())
	return info.name
end

function clsBossWar:SetScene(obj)
	self.__temp.scene = obj
end

function clsBossWar:GetScene()
	return self.__temp.scene
end

function clsBossWar:GetBoss()
	local sceneObj = self:GetScene()
	return sceneObj:GetKeyNpc()
end

--初始化boss数据
function clsBossWar:InitBossData()
	local boss = self:GetBoss()
	local hpMax = CalBossHpMax()
	self.__temp.boss = {}
	self.__temp.boss.hpMax = hpMax
	self.__temp.boss.hp = hpMax
	self:SetBossKilled(false)
end

function clsBossWar:GetBossHeadId()
	local info = GetWorldBossInfoById(self:GetId())
	local monster = FIGHT.GetMonsterInfoById(info.npcId)
	return monster.headId
end

function clsBossWar:GetBossLv()
	local info = GetWorldBossInfoById(self:GetId())
	local monster = FIGHT.GetMonsterInfoById(info.npcId)
	return monster.Lv
end

function clsBossWar:GetBossHpMax()
	return self.__temp.boss.hpMax or 0
end

function clsBossWar:GetBossCurHp()
	return self.__temp.boss.hp
end

function clsBossWar:SetBossCurHp(data)
	local old = self.__temp.boss.hp
	self.__temp.boss.hp = self.__temp.boss.hp + data
	if self.__temp.boss.hp < 0 then
		self.__temp.boss.hp = 0
	end
	local flag = self:GetBossHpMax() * DYING_HP_PERCET
	if old > flag and self.__temp.boss.hp <= flag then
		self:SendWorldBossMsg("dying")
	end
end

function clsBossWar:SetBossKilled(flag)
	self.__temp.bossKilled = falg
end

function clsBossWar:IsBossKilled()
	return self.__temp.bossKilled
end


function clsBossWar:InitRankList()
	self.__temp.rank = RANK.NewRank("real", "bossDmg")
end

function clsBossWar:GetRank()
	return self.__temp.rank
end

function clsBossWar:SetRank(obj)
	self.__temp.rank = obj
end

--更新排行榜
function clsBossWar:RenewRank(roleId, damage)
	local rankObj = self:GetRank()
	rankObj:AddToRank({roleId = roleId, damage = damage})
end
--获取排行榜排名
function clsBossWar:GetTopRank(roleId)
	local rankObj = self:GetRank()
	return rankObj:GetTopRank(TOP_RANK_NUM, roleId, self:GetBossHpMax())
end

--玩家参战信息
function clsBossWar:GetAllRoleList()
	return self.__temp.userList
end

function clsBossWar:GetRoleInfo(roleId)
	return self.__temp.userList[roleId]
end

function clsBossWar:AddRole(roleId)
	if not self.__temp.userList[roleId] then
		self.__temp.userList[roleId] = {timestamp = os.time(), damage = 0, bufferList = {}}
	end
end
--角色死亡
function clsBossWar:SetRoleDead(roleId)
	local info = self.__temp.userList[roleId]
	if not info then return false end
	info.timestamp = os.time() + REVIVE_TIMELONG
	self:SetRoleState(roleId, "dead")
	local vipLv = USER.GetUserNickInfo(roleId, "vipLv")
	if VIP.IsVipFuncValid(vipLv, VIP.VIP_WBOSS_REVIVAL_FREE) then
		return self:SetRoleRevive(roleId)
	end
	self:StartReviveCallOut(roleId)
end

function clsBossWar:IsRoleDead(roleId)
	local info = self.__temp.userList[roleId]
	if not info then return false end
	return info.timestamp > os.time()
end

function clsBossWar:SetRoleRevive(roleId)
	local info = self.__temp.userList[roleId]
	if not info then return end
	self:RemoveReviveCallOut(roleId)
	info.timestamp = os.time()
	self:SetRoleState(roleId)
end

function clsBossWar:GetRoleReviveTime(roleId)
	local info = self.__temp.userList[roleId]
	if not info then return end
	return info.timestamp
end

function clsBossWar:ReviveAllRole()
	for k, v in pairs(self:GetAllRoleList()) do
		v.state = nil
		self:RemoveReviveCallOut(k)
	end
end

--启动复活定时器
function clsBossWar:StartReviveCallOut(roleId)
	local roleInfo = self:GetRoleInfo(roleId)
	if not roleInfo then
		return
	end
	local timelong = roleInfo.timestamp - os.time()
	if timelong <= 0 then
		self:SetRoleRevive(roleId)
	else
		roleInfo.reviveTimer = CALLOUT.CallOut(clsBossWar.SetRoleRevive, timelong, self, roleId)
	end
end

function clsBossWar:RemoveReviveCallOut(roleId)
	local roleInfo = self:GetRoleInfo(roleId)
	if not roleInfo then
		return
	end
	if roleInfo.reviveTimer then
		CALLOUT.RemoveCallOut(getfenv(2), roleInfo.reviveTimer)
	end
end

function clsBossWar:SetRoleState(roleId, state)
	local roleInfo = self:GetRoleInfo(roleId)
	if not roleInfo then
		return
	end
	roleInfo.state = state
	local scene = self:GetScene()
	local user = scene:GetSceneRole(roleId)
	if user then
		local locate = user:GetLocateInfo()
		user:Broad2ExNeighbor("stand", locate.x, locate.y, true)
	end
end

function clsBossWar:GetRoleState(roleId)
	local roleInfo = self:GetRoleInfo(roleId)
	if not roleInfo then
		return
	end
	return roleInfo.state or "normal"
end

--角色伤害统计
function clsBossWar:SetRoleDamage(roleId, damage)
	local info = self.__temp.userList[roleId]
	if not info then return end
	info.damage = (info.damage or 0) + damage
	self:RenewRank(roleId, info.damage)
end

function clsBossWar:GetRoleDamage(roleId)
	local info = self.__temp.userList[roleId]
	if not info then return 0 end
	return info.damage
end

function clsBossWar:SetRoleBuffer(roleId, bufferId)
	local bufferInfo = GetBufferInfo(bufferId)
	if not bufferInfo then return end
	local info = self.__temp.userList[roleId]
	if not info then return end
	info.bufferList = info.bufferList or {}
	info.bufferList[bufferId] = (info.bufferList[bufferId] or 0) + bufferInfo.promote
	if info.bufferList[bufferId] > bufferInfo.max then
		info.bufferList[bufferId] = bufferInfo.max
	elseif info.bufferList[bufferId] < 0 then
		info.bufferList[bufferId] = 0
	end
end

function clsBossWar:GetRoleBufferEffect(roleId)
	local info = self:GetRoleInfo(roleId)
	local rate = 0
	if info and info.bufferList then
		for k in pairs(BufferList) do
			rate = rate + (info.bufferList[k] or 0)
		end
	end
	return rate
end


function clsBossWar:SetKiller(roleId)
	self.__temp.killer = roleId
end

function clsBossWar:GetKiller()
	return self.__temp.killer
end

--活动时间戳
function clsBossWar:SetBossActTimestamp(time)
	self.__temp.timestamp = time
end

function clsBossWar:GetBossActTimestamp()
	return self.__temp.timestamp
end

function clsBossWar:IsUserAutoFight(roleId)
	local info = self:GetRoleInfo(roleId)
	return info.autoFight and info.autoFight == 1
end

--boss战开启
function clsBossWar:ActBegin()
	if self:GetState() ~= "ready" then
		return
	end
	self:ClearActCallOut()
	self:SetState("begin")
	self:SetBossActTimestamp(os.time() + FIGHT_LAST_TIME)
	local info = GetWorldBossInfoById(self:GetId())
	local boss = self:GetBoss()
	boss:SetFightable(true)
	self:UpdateBossWarInfo()
	self:SendBossInfoUpdate()
	
	self:SendWorldBossMsg("begin")
	self:RunActOverCallOut()
	self:RunRankUpdateCallOut()
end

--活动结束
function clsBossWar:ActOver()
	if self:GetState() ~= "begin" then
		return
	end
	self:ClearActCallOut()
	self:ClearRankUpdateCallOut()
	self:SetState("over")
	self:SetBossActTimestamp(os.time() + OVER_STAY_TIMGLONG)
	local sceneObj = self:GetScene()
	local boss = self:GetBoss()
	if boss then
		sceneObj:SetKeyNpc()
		NPC.NpcLeave(boss)
	end
	self:UpdateBossWarInfo()
	self:SendBossInfoUpdate()
	self:SendRankReward()
	local scene = self:GetScene()
	for k, v in pairs(scene:GetSceneRoleList()) do
		if v:GetRoleType() == "user" then
		--	SCENE.ReturnToTown(v)
			self:SendBossWarStat(k)
		end
	end
	local killer = self:GetKiller()
	if killer then
		self:SendKillerReward()
		self:SendWorldBossMsg("kill", killer)
		self:CheckBossActAchv()
	else
		self:SendWorldBossMsg("timeout")
	end
	self:SendFightResult()
	self:RunClearActCallOut()
	
	for k, v in pairs(USER.GetAllOnlineUser()) do
		FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_WORLD_BOSS_1, FUNCOPEN.STATE_INVALID)
	end

	STATS.SaveWordBossHistory(self:GetId(), table.size(self:GetAllRoleList()), killer, self:GetTopDamageRole())
end

function clsBossWar:ClearAct()
	local scene = self:GetScene()
--[[	for k, v in pairs(scene:GetSceneRoleList()) do
		if v:GetRoleType() == "user" then
		--	SCENE.ReturnToTown(v)
			self:SendBossWarStat(k)
		end
	end]]
--	SCENE.DestroyScene(scene)
	scene:SetAutoRelease(true)			--设置为无人时自动释放
	RANK.DestroyRank(self:GetRank())
	self:SetRank()
	self:Destroy()
end

function clsBossWar:SendBossWarStat(roleId)
	local roleInfo = self:GetRoleInfo(roleId)
	local rank = self:GetRank()
	local rankNo = rank:GetRankNo(roleId)
	if not rankNo then
		return
	end
	local msg = string.format("您在本次%s的活动中，累计对%s造成%d伤害，获得钱币%d，排名第%d位。", self:GetActName(), self:GetBossName(), roleInfo.damage, math.floor(roleInfo.damage * 0.1), rankNo)
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		ERROR.ErrorMsg(con, msg)
	end
end

function clsBossWar:SendRankReward()
	local rank = self:GetRank()
	local rankList = rank:GetRankList()
	for i = 1, MAX_REWARD_RANK do
		local roleId = rankList[i] and rankList[i].roleId
		if roleId then
			local reward = GetRewardByRank(i)
			local mail = {}
			mail.title = "世界BOSS排名奖励"
			mail.content = string.format("亲，鉴于你在抵抗%s的行动中表现杰出，荣获第%d名，特奖励钱币%d。请及时提取附件。", self:GetBossName(), i, reward)
			mail.attach = {gold = reward}
			MAIL.SendSysMail(roleId, mail)
			local user = USER.GetOnlineUser(roleId)
			if user then
				user:SendProtocol("c_show_tips", string.format("获得了%d金钱", reward))
			end
		end
	end
end

function clsBossWar:SendKillerReward()
	local roleId = self:GetKiller()
	local mail = {}
	mail.title = "世界BOSS击杀奖励"
	mail.content = string.format("亲，鉴于你在抵抗%s的行动中表现杰出，在最后关头给予其致命一击，特奖励钱币%d。请及时提取附件。", self:GetBossName(), KILLER_REWARD)
	mail.attach = {gold = KILLER_REWARD}
	MAIL.SendSysMail(roleId, mail)
end

function clsBossWar:SendFightResult()
	local rank = self:GetRank()
	local rankList = rank:GetRankList()
	if #rankList <= 0 then
		return
	end
	local rankNum = math.min(DAMAGE_NOTICE_NUM, #rankList)
	local msg = string.format("在本次抵抗%s的行动中伤害最高的前%d名为\n", self:GetBossName(), rankNum)
	for i = 1, rankNum do
		if rankList[i] then
			local rate = rankList[i].damage / self:GetBossHpMax() * 100
			rate = rate - rate % 0.01
			msg = msg .. string.format("第%d名  %-12s  %.2f%%伤害\n", i, USER.GetUserNickById(rankList[i].roleId), rate)
		end
	end
	msg = msg .. "他们分别获得神秘奖励一份。"
	CHAT.SendSysMsg("system", msg)
	local killer = self:GetKiller()
	if killer then
		CHAT.SendSysMsg("system", string.format("玩家%s给予Boss最后一击，获得神秘奖励一份。", USER.GetUserNickById(killer)))
	end
end

function clsBossWar:GetTopDamageRole()
	local rank = self:GetRank()
	local rankList = rank:GetRankList()
	if rankList and next(rankList) then
		return rankList[1].roleId
	end
end

function clsBossWar:BeginFight(user)
	local roleId = user:GetRoleId()
	local sceneObj = self:GetScene()
	local boss = sceneObj:GetKeyNpc()
	if not boss then
		return
	elseif self:IsRoleDead(roleId) then
		return
	end
	local function call_back(fightId, winner, prize, stats)
		user:SetWorkInfo("standby")		
		self:SetRoleDamage(roleId, stats.atkDamage or 10000)
		self:SetBossCurHp(-(stats.atkDamage or 10000))
		user:SetGold(prize.gold or 0)
		if not self:IsBossKilled() and self:GetBossCurHp() <= 0 then
			self:SetBossKilled(true)
			self:SetKiller(roleId)
			user:CheckAchvState("bossActKill", self)
			self:ActOver()
		end
		if not self:IsBossKilled() and winner ~= "attack" then
			self:SetRoleDead(roleId)
			local sceneObj = self:GetScene()
			local revivalPos = sceneObj:GetBornPoint()
			user:Goto(revivalPos.x, revivalPos.y)
		else
			self:SetRoleState(roleId)
		end
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			local roleInfo = self:GetRoleInfo(roleId)
			local timelong = roleInfo.timestamp - os.time()
			con:SendProtocol("c_bossWar_fight", {result = winner == "attack", timelong = timelong > 0 and timelong or 0})
		end
	end
	local roleInfo = self:GetRoleInfo(roleId)
	local rate = self:GetRoleBufferEffect(roleId)
	local param = {attack = {physcAtkRate = rate, magicAtkRate = rate, hpMax = rate, physcDefRate = rate, magicDefRate = rate, speed = rate}, bossCurHp = self:GetBossCurHp(), bossHpMax = self:GetBossHpMax()}
	local locate = user:GetLocateInfo()
	user:SetWorkInfo("fighting", fightId)
	self:SetRoleState(roleId, "fighting")
	local fightId = FIGHT.BeginFight("bossFight", roleId, boss:GetId(), {roleId}, call_back, param, {mapId = sceneObj:GetMapId(), posX = locate.x, posY = locate.y})
end

function clsBossWar:GetBossInfo(flag)
	local state = self:GetState()
	local sData = {}
--	if state == "ready" or state == "begin" then
		if not flag then
			sData.name = self:GetBossName()
			sData.bossLv = self:GetBossLv()
			sData.hpMax = self:GetBossHpMax()
			sData.headId = self:GetBossHeadId()
		end
		sData.hp = self:GetBossCurHp()
--	end
	if state == "begin" then
		sData.timelong = self:GetBossActTimestamp() - os.time()
	end
	return sData
end

--更新boss信息
function clsBossWar:SendBossInfoUpdate(roleId, flag)
	if roleId then
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_bossWar_bossInfo", self:GetBossInfo())
		end
	else
		local sceneObj = self:GetScene()
		local list = sceneObj:GetSceneRoleList()
		for k in pairs(list) do
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_bossWar_bossInfo", self:GetBossInfo(flag))
			end
		end
	end
end
--更新boss战状态信息
function clsBossWar:UpdateBossWarInfo()
	local sceneObj = self:GetScene()
	for k, v in pairs(sceneObj:GetSceneRoleList()) do
		if v:GetRoleType() == "user" then
			local con = LOGIN.GetConnection(k)
			if con and con.roleId == k then
				con:SendProtocol("c_bossWar_info", self:GetBossWarInfo(k))
			end
		end
	end
end

function clsBossWar:GetBossWarInfo(roleId)
	local state = self:GetState()
	local sData = {state = state}
	if state == "begin" then
		local vipLv = USER.GetUserNickInfo(roleId, "vipLv")		
		sData.revivalCost = VIP.IsVipFuncValid(vipLv, VIP.VIP_WBOSS_REVIVAL_FREE) and 0 or REVIVE_COST
		sData.reviveTime = math.max(self:GetRoleReviveTime(roleId) - os.time(), 0)
		sData.embrave = {}
		for k, v in pairs(BufferList) do
			local buff = {}
			buff.bufferId = k
			buff.name = v.name
			buff.promote = v.promote
			if v.type == "ingot" then
				buff.needIngot = v.num
			else
				buff.needNimbus = v.num
			end
			buff.max = v.max
			table.insert(sData.embrave, buff)
		end
	end
	local roleInfo = self:GetRoleInfo(roleId)
	if roleInfo and roleInfo.bufferList then
		sData.buffer = {}
		for k, v in pairs(roleInfo.bufferList) do
			table.insert(sData.buffer, {bufferId = k, bufferRate = v})
		end
	end
	if state == "ready" or state == "over" then
		sData.timelong = self:GetBossActTimestamp() - os.time()
	end
	return sData
end


--更新伤害排名
function clsBossWar:SendRankUpdate()
	if self:GetState() ~= "open" then
		return
	end
	local sceneObj = self:GetScene()
	local list = sceneObj:GetSceneRoleList()
	for k in pairs(list) do
		local con = LOGIN.GetConnection(k)
		if con and con.roleId == k then
			local sData = self:GetTopRank(k)
			con:SendProtocol("c_bossWar_rank", sData)
		end
	end
	self:SendBossInfoUpdate(nil, true)
end



--boss战信息提示
function clsBossWar:SendWorldBossMsg(type, roleId)
	local msg = ""
	if type == "ready" then
		msg = string.format("活动[%s]即将开始，各位大侠赶快准备行动吧。", self:GetActName())
	elseif type == "begin" then
		msg = string.format("活动[%s]正式开始了，各位大侠快冲吧。", self:GetActName())
	elseif type == "dying" then
		msg = string.format("%s已经被打的奄奄一息，大家快冲啊！", self:GetBossName())
	elseif type == "timeout" then
		msg = string.format("%s突然狂吼一声，化作一阵黑烟消失不见了。", self:GetBossName())
	elseif type == "kill" then
		msg = string.format("玩家%s突然大吼一声，王霸之气外漏，%s受气场震慑，惨叫一声当场身亡。", USER.GetUserNickById(roleId), self:GetBossName())
	end
	CHAT.SendSysNotice(msg)
end


--boss战开启定时器
function clsBossWar:RunActBeginCallOut()
	local timelong = self:GetBossActTimestamp() - os.time()
	if timelong <= 0 then
		self:ActBegin()
	else
		self.__temp.actTimerId = CALLOUT.ObjCallOut(self.__temp.callout, clsBossWar.ActBegin, timelong, self)
	end
end

--活动结束定时器
function clsBossWar:RunActOverCallOut()
	local timelong = self:GetBossActTimestamp() - os.time()
	if timelong <= 0 then
		self:ActOver()
	else
		self.__temp.actTimerId = CALLOUT.ObjCallOut(self.__temp.callout, clsBossWar.ActOver, FIGHT_LAST_TIME, self)
	end
end

function clsBossWar:RunClearActCallOut()
	local timelong = self:GetBossActTimestamp() - os.time()
	if timelong <= 0 then
		WORLDBOSS.ClearAct(self:GetId())
	else
		self.__temp.actTimerId = CALLOUT.ObjCallOut(self.__temp.callout, WORLDBOSS.ClearAct, OVER_STAY_TIMGLONG, self:GetId())
	end
end

function clsBossWar:ClearActCallOut()
	if self.__temp.actTimerId then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.actTimerId)
	end
end

--排行榜更新定时器
function clsBossWar:RunRankUpdateCallOut()
	self.__temp.rankTimerId = CALLOUT.CallOutInRange(clsBossWar.SendRankUpdate, os.time(), os.time() + FIGHT_LAST_TIME, UPDATE_FREQUENCY, self)
end

function clsBossWar:ClearRankUpdateCallOut()
	if self.__temp.rankTimerId then
		CALLOUT.RemoveCallOut(getfenv(2), self.__temp.rankTimerId)
	end
end


--检查boss战成就
function clsBossWar:CheckBossActAchv()
	local function set_achv_got(roleId, achvId)		
		local user = USER.GetOnlineUser(roleId)
		if user then
			user:SetAchvGot(achvId)
		else
			OFFLINE.SetOfflineData(roleId, "achvGot", achvId)
		end
	end
	local rank = self:GetRank()
	for roleId, info in pairs(self:GetAllRoleList()) do
		for k, v in pairs(ACHV.GetAllAchvList()) do
			if v.label == "bossActWin" then
				if self:GetId() == tostring(v.num1) then
					set_achv_got(roleId, k)
				end
			elseif v.label == "bossActRank" then
				local rankNo = rank:GetRankNo(roleId)
				if not v.num1 then
					log(string.format("%s, %s", roleId, k), "wberror.log")
				end
				if rankNo and rankNo <= v.num1 then
					set_achv_got(roleId, k)
				end
			elseif v.label == "bossActBuffer" then
				if info.bufferList and info.bufferList[tostring(v.num1)] >= v.num2 then
					set_achv_got(roleId, k)
				end
			end
		end
	end
end

function clsBossWar:GetSealBarState()
	local state = self:GetState()
	local sData = {}
	sData.type = state == "begin" and "open" or "close"
	sData.posX = BOSS_COLUMN_X
	sData.posY = BOSS_COLUMN_Y
	return sData
end


function __init__()
	local WORLD_BOSS = Import("autocode/worldBossList.lua")
	WorldBossInfoList = WORLD_BOSS.GetTable()
end
