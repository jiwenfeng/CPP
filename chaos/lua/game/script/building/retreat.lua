--修仙塔

local MAX_CHALLENGE_TIMES = 1	--每日挑战次数
local SELECT_BASE_COST = 2
local EXP_DOUBLE_ITEM_ID = 101033	--聚气丹id

local MOPUP_NEED_GOLD = 1000	--扫荡所需钱币
local MOPUP_TIMELONG = 60		--每层扫荡时长
local FINISH_MOPUP_COST = 1		--每层立即结束所需元宝

local STATE_DOING = 1
local STATE_FINISH = 0

local PrizeList = {}

function GetPrizeByLevel(level)
	return PrizeList[level]
end

function GetRetreatInfoByLv(level)
	return STRUCT.GetUpLvInfo("retreat", level)
end

--直接选择某层开始
function GetSelectNeedIngot(tail, begin)
	begin = begin or 1
	return (tail - begin) * SELECT_BASE_COST
end

--扫荡所需钱币
function GetMopUpNeedGold(tail, begin)
	begin = begin or 1
	return (tail - begin + 1) * MOPUP_NEED_GOLD
end

--快速结束扫荡所需元宝
function GetFinishMopUpCost(tail, begin)
	begin = begin or 1
	return (tail - begin + 1) * FINISH_MOPUP_COST
end

--lv之前所有楼层的经验和
function GetTotalExp(tail, begin)
	begin = begin or 1
	local total = 0
	for i = begin, tail do
		local lvInfo = GetRetreatInfoByLv(i)
		total = total + lvInfo.exp
	end
	return total
end

function GetRetreatBossLvList()
	local sData = {}
	local n = #STRUCT.GetUpLvInfoList("retreat")
	for i = 1, math.floor(n / 10) do
		local lvInfo = GetRetreatInfoByLv(i * 10)
		local monsterInfo =FIGHT.GetMonsterTeamInfoById(lvInfo.monsterId)
		local buff = {}
		buff.Lv = i * 10
		buff.name = monsterInfo.name
		buff.headId = monsterInfo.headId
		table.insert(sData, buff)
	end
	return sData
end

-----------------------------------
clsRetreat = STRUCT.clsStruct:Inherit()

function clsRetreat:__init__()
	Super(clsRetreat).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsRetreat:initsavedata()
	self.__save.Lv = 0
	self.__save.open = 0
end

function clsRetreat:inittempdata()
	self.__temp.cur = 0
	self.__temp.callout = {}
end

function clsRetreat:InitStruct(owner, type, structType)
	self:SetOwner(owner)
	self:SetType(type)
	self:SetStructType(structType)
end

--当前等级
function clsRetreat:SetLv(data)
	self.__save.Lv = (self.__save.Lv or 0) + data
	if self.__save.Lv <= 1 then
		self.__save.Lv = 1
	elseif self.__save.Lv > self:GetMaxLv() then
		self.__save.Lv = self:GetMaxLv()
	end
end

function clsRetreat:GetLv()
	return self.__save.Lv or 1
end

function clsRetreat:Upgrade()
	self:SetLv(1)
end

--当前开放层数
function clsRetreat:GetOpenLv()
	return self:GetLv()
end

--已经通过的最高位置
function clsRetreat:SetTopLv(data)
	self.__save.top = data
	local lv = self:GetOpenLv()
	if self.__save.top < 0 then
		self.__save.top = 0
	elseif self.__save.top > lv then
		self.__save.top = lv
	end
end

function clsRetreat:GetTopLv()
	return self.__save.top or 0
end
--当日挑战次数
function clsRetreat:SetChallengeTimes(data)
	self.__save.challStat = self.__save.challStat or {}
	local info = self.__save.challStat
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		info.num = data
	else
		info.num = info.num + data
	end
	info.time = os.time()
end

function clsRetreat:GetChallengeTimes()
	self.__save.challStat = self.__save.challStat or {}
	local info = self.__save.challStat
	if not info.time or CalDiffDays(info.time, os.time()) ~= 0 then
		return 0
	end
	return info.num
end

--挑战过程记录
function clsRetreat:InitChallenge(expDouble)
	self.__save.challenge = {}
	self.__save.challenge.curLv = 1
	self.__save.challenge.passed = false
	self.__save.challenge.expDouble = expDouble
	self.__save.challenge.expGot = 0
	self.__save.challenge.itemGot = {}
end

function clsRetreat:GetCurChallenge()
	return self.__save.challenge
end
--重新开始/清除挑战记录
function clsRetreat:ClearCurChallenge()
	if self:GetMopup() then
		self:FinishMopUp("quiet")
	end
	self.__save.challenge = nil
end
--当前层数
function clsRetreat:GetCurLv()
	if not self.__save.challenge then
		return 1
	end
	return self.__save.challenge.curLv
end

function clsRetreat:SetCurLv(data)
	if not self.__save.challenge then
		return
	end
	self.__save.challenge.curLv = self.__save.challenge.curLv + data 
	if self.__save.challenge.curLv >= self:GetMaxLv() then
		self.__save.challenge.curLv = self:GetMaxLv()
	end
end

--当前层数是否已经胜利
function clsRetreat:SetLvPassed(flag)
	if not self.__save.challenge then
		return
	end
	self.__save.challenge.passed = flag
end

function clsRetreat:IsLvPassed()
	if not self.__save.challenge then
		return
	end
	return self.__save.challenge.passed
end

--累计经验
function clsRetreat:SetExpGot(data)
	self.__save.challenge.expGot = self.__save.challenge.expGot + data
end

function clsRetreat:GetExpGot()
	return self.__save.challenge.expGot
end

--累计获得物品
function clsRetreat:SetItemGot(goodsId, num)
	local itemList = self.__save.challenge.itemGot
	for k, v in ipairs(itemList) do
		if v.goodsId == goodsId then
			v.num = v.num + num
			return
		end
	end
	table.insert(itemList, {goodsId = goodsId, num = num})
end

function clsRetreat:GetItemGot()
	return self.__save.challenge.itemGot
end

function clsRetreat:BeginChallenge(id)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local challenge = self:GetCurChallenge()
	if not self:GetCurChallenge() then
		return false, "您没有进入修仙塔场景。"
	end
	local scene = self:GetCurScene()
	if not scene then
		return false, "您已经不在修仙塔场景中。"
	end
	local npcObj = scene:GetSceneRole(id)
	if not npcObj then
		return false, "没有指定的怪物。"
	end
	local roleId = user:GetRoleId()
	local lvInfo = GetRetreatInfoByLv(challenge.curLv)
	local function callback(fightId, winner)
		npcObj:SetState()
		user:SetWorkInfo("standby")
		if winner == "attack" then
			NPC.NpcLeave(npcObj)
		end
		if winner == "attack" then
			self:ChallengeWin()
		else
			local bornPoint = scene:GetBornPoint()
			user:SetLocateInfo(scene:GetTemplate(), bornPoint.x, bornPoint.y)
		end
	end
	local param = {saveHp = true, restoreHp = true, Lv = challenge.curLv, double = challenge.expDouble}
	local locate = user:GetLocateInfo()
	local scene = user:GetScene()
	user:SetWorkInfo("fighting", nil, nil, nil)
	npcObj:SetState("fighting")
	local fightId = FIGHT.BeginFight("retreat", roleId, lvInfo.monsterId, {roleId}, callback, param, {mapId = scene:GetMapId(), posX = locate.x, posY = locate.y})
	return true
end

function GenPrize(Lv)
	local prizeInfo = GetPrizeByLevel(Lv)
	if not prizeInfo then
		return 
	end
	local len = #prizeInfo.list
	local sData = {}
	for i = 1, prizeInfo.num do
		local index = math.random(1, len)
		local info = prizeInfo.list[index]
		if not info.type then
			table.insert(sData, {goodsId = info.id, num = 1})
		else
			local list = BASEITEM.GetItemListByType(info.type, info.Lv)
			local index = math.random(1, #list)
			table.insert(sData, {goodsId = list[index], num = 1})
		end
	end
	return sData
end

--计算修仙塔奖励
function GetRetreatPrize(Lv, double)
	local lvInfo = GetRetreatInfoByLv(Lv)
	local prize = {}
	prize.exp = lvInfo.exp
	if double then
		prize.exp = prize.exp * 2
	end
	prize.item = GenPrize(Lv)
	return prize
end

function clsRetreat:ChallengeWin()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local challenge = self:GetCurChallenge()
	local prize = GetRetreatPrize(challenge.curLv, challenge.expDouble)
	user:SetExp(prize.exp, "retreat")
	self:SetExpGot(prize.exp)
	local itemToMail = {}
	for k, v in pairs(prize.item or {}) do
		local item = BASEITEM.NewItem(v.goodsId, v.num, {lock = 1})
		if item then
			if not user:IsItemCanAdd(USERBASE.PACKAGE, item) then
				table.insert(itemToMail, {id = v.goodsId, num = v.num})	
			else
				user:AddItem(USERBASE.PACKAGE, item)
				self:SetItemGot(v.goodsId, v.num)
			end
		else
			DATA.log(string.format("Invalid retreat prize:id = %s, num = %s.", v.goodsId, v.num), "retreat.log")
		end
	end
	if #itemToMail > 0 then
		local mail = {}
		mail.title = string.format("修仙塔第%d层掉落奖励", challenge.curLv)
		mail.content = "您的背包已满，请及时整理背包并领取附件中的奖励，以避免不必要的损失。"
		mail.attach = {item = itemToMail}
		MAIL.SendSysMail(self:GetOwner(), mail)
	end	
	self:SetLvPassed(true)
	if challenge.curLv > self:GetTopLv() then
		self:SetTopLv(challenge.curLv)
	end
	return prize.exp, prize.item
end

--进入下一层
function clsRetreat:EnterNextLevel()
	self:SetCurLv(1)
	self:SetLvPassed(false)
end

function clsRetreat:ChallengeSelected(level)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if self:GetCurChallenge() then
		self:ClearCurChallenge()
	end
	self:InitChallenge(user:GetSetting(SETTING.RETREAT_EXP_DOUBLE))
	self:SetChallengeTimes(1)
--	while(self:GetCurLv() <= level and not self:IsLvPassed()) do
	while(self:GetCurLv() < level) do
		self:ChallengeWin()
		self:EnterNextLevel()
	end
end

function clsRetreat:CreateScene(type)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if not self:GetCurChallenge() then
		self:InitChallenge(user:GetSetting(SETTING.RETREAT_EXP_DOUBLE))
		self:SetChallengeTimes(1)
	end
	if type == "next" and self:IsLvPassed() then
		self:EnterNextLevel()
	end
	local challenge = self:GetCurChallenge()
	local level = challenge.curLv
	local lvInfo = GetRetreatInfoByLv(level)
	local sceneObj = SCENE.NewScene("retreat", lvInfo.mapId, true)
	if not challenge.passed then
		local npcObj = NPC.NewNpc(lvInfo.monsterId)
		npcObj:EnterScene(sceneObj, lvInfo.monsterPos.x, lvInfo.monsterPos.y)
		npcObj:SetTarget(self:GetOwner())
	end
	self:SetCurScene(sceneObj)
	user:SetFuncStatById(SETTING.FUNC_RETREAT, 1)
	return sceneObj
end

function clsRetreat:SetMopupState(state)
	if not self.__save.mopup then
		return
	end
	self.__save.mopup.state = state
end

function clsRetreat:GetMopupState()
	if not self.__save.mopup then
		return
	end
	return self.__save.mopup.state
end

function clsRetreat:SetMopupGot(exp, itemList)
	if not self.__save.mopup then
		return
	end
	self.__save.mopup.got = self.__save.mopup.got or {}
	local info = self.__save.mopup.got
	info.exp = (info.exp or 0) + exp
	info.itemList = info.itemList or {}
	if itemList then
		for k, v in pairs(itemList) do
			local flag = false
			for _, val in pairs(info.itemList) do
				if val.goodsId == v.goodsId then
					val.num = val.num + v.num
					flag = true
				end
			end
			if not flag then
				table.insert(info.itemList, v)
			end
		end
	end
end

function clsRetreat:GetMopupGot()
	if not self.__save.mopup then
		return
	end
	self.__save.mopup.got = self.__save.mopup.got or {}
	return self.__save.mopup.got
end

--开始扫荡
function clsRetreat:BeginMopUp(begin, tail)
--	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self.__save.mopup = {}
	self.__save.mopup.begin = begin
	self.__save.mopup.tail = tail
	self.__save.mopup.state = STATE_DOING
	self.__save.mopup.got = {}
	self:SetMopupTimestamp(os.time() + MOPUP_TIMELONG)
	self:StartMopupCallOut()
end

function clsRetreat:ProMopUp(kind)
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	user:SetGold(-MOPUP_NEED_GOLD)
	user:SetFuncStatById(SETTING.FUNC_RETREAT, 1)
	local exp, itemList = self:ChallengeWin()
	self:SetMopupGot(exp, itemList)
	local sData = {done = {}}
	sData.done.level = self:GetCurLv()
	sData.done.exp = exp
	sData.done.itemList = {}
	for k, v in pairs(itemList or {}) do
		table.insert(sData.done.itemList, {goodsId = v.goodsId, num = v.num})
	end
	self:EnterNextLevel()
	if self:GetCurLv() <= self:GetMopUpTail() then
--		self:EnterNextLevel()
		self:SetMopupTimestamp(os.time() + MOPUP_TIMELONG)
		if not kind then
			self:StartMopupCallOut()
		end
		sData.curLv = self:GetCurLv()
		sData.timelong = MOPUP_TIMELONG
	else
		self:SetMopupState(STATE_FINISH)
--		self:ClearMopUp()
	end
	sData.state = self:GetMopupState()
	if kind ~= "quiet" then
		user:SendProtocol("c_retreat_mopupUpdate", sData)
		if self:GetMopupState() == STATE_FINISH then
			local mopupGot = self:GetMopupGot()
			user:SendProtocol("c_retreat_finish", {curLv = self:GetCurLv(), expGot = mopupGot.exp, itemGot = mopupGot.itemList, state = STATE_FINISH})
		end
	end
end

function clsRetreat:FinishMopUp(kind)
	self:RemoveMopupCallOut()
	while (self:GetCurLv() <= self:GetMopUpTail() and not self:IsLvPassed()) do
		self:ProMopUp(kind)
	end
	self:SetMopupTimestamp(os.time())
end

--取消扫荡
function clsRetreat:ClearMopUp()
	self:RemoveMopupCallOut()
	self.__save.mopup = nil
end

--扫荡信息
function clsRetreat:GetMopup()
	return self.__save.mopup
end

--扫荡终止层数
function clsRetreat:GetMopUpTail()
	if not self.__save.mopup then
		return
	end
	return self.__save.mopup.tail
end

--扫荡处理时间戳
function clsRetreat:GetMopupTimestamp()
	if not self.__save.mopup then
		return
	end
	return self.__save.mopup.timestamp
end

function clsRetreat:SetMopupTimestamp(data)
	if not self.__save.mopup then
		return
	end
	self.__save.mopup.timestamp = data
end

function clsRetreat:StartMopupCallOut()
	if self:GetMopupState() == STATE_DOING then
		local timelong = self:GetMopupTimestamp() - os.time()
		self.__temp.mopupTimer = CALLOUT.ObjCallOut(self.__temp.callout, clsRetreat.ProMopUp, timelong, self)
	end
end

function clsRetreat:RemoveMopupCallOut()
	if self.__temp.mopupTimer then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.mopupTimer)
	end
end

function clsRetreat:GetCurScene()
	return self.__temp.scene
end

function clsRetreat:SetCurScene(sceneObj)
	self.__temp.scene = sceneObj
end

--处理离线扫荡
function clsRetreat:ProStructFunc()
	local mopup = self:GetMopup()
	while (mopup and mopup.state == STATE_DOING) do
		if os.time() < self:GetMopupTimestamp() then
			break
		end
		self:ProMopUp("quiet")
	end
end

function clsRetreat:LoginStartCallOut()
	self:StartMopupCallOut()
end

function clsRetreat:OfflineRemoveCallOut()
	self:RemoveMopupCallOut()
end


function clsRetreat:GetRetreatStat()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	sData.openLv = self:GetOpenLv()
	sData.totalLv = self:GetTopLv()
	sData.times = MAX_CHALLENGE_TIMES - self:GetChallengeTimes()
	local challenge = self:GetCurChallenge()
	sData.inProcess = challenge and 1 or 0
	sData.curLv = self:GetCurLv()
--	sData.isPassed = self:IsLvPassed() and 1 or 0
	sData.isSelected = user:GetSetting(SETTING.RETREAT_EXP_DOUBLE) and 1 or 0
	sData.selBaseCost = SELECT_BASE_COST
	sData.mopupGold = MOPUP_NEED_GOLD
	sData.mopupTime = MOPUP_TIMELONG
	return sData
end

function clsRetreat:SendRetreatUpdate()
	local sData = {}
	sData.times = MAX_CHALLENGE_TIMES - self:GetChallengeTimes()
	sData.inProcess = self:GetCurChallenge() and 1 or 0
	sData.curLv = self:GetCurLv()
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_retreat_update", sData)
	end
end

function QuitRetreat(user, scene)
	local tribe = user:GetTribe()
	if not tribe then
		return false, "你还没有建立部落"
	end
	local retreat = tribe:GetStruct("retreat")
	local sceneObj = retreat:GetCurScene()
	if scene == sceneObj then
		retreat:SetCurScene()
		sceneObj:Destroy()
		if retreat:GetCurLv() < retreat:GetOpenLv() and retreat:IsLvPassed() then
			retreat:EnterNextLevel()
		end
	end
	local tribeScene = SCENE.GetEmptyScene("tribe")
	return tribeScene:GetId()
end

--界面信息
local s_retreat_stat = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if not retreat then
		return ERROR.TipMsg(connection, "您还没有修仙塔")
	end
	local sData = retreat:GetRetreatStat()
	connection:SendProtocol("c_retreat_stat", sData)
end

--指定楼层开始
local s_retreat_select = function(connection, data)
	data = tonumber(data)
	if not data or data < 1 then
		return ERROR.TipMsg(connection, "请选择要直接到达的层数。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if data > retreat:GetOpenLv() then
		return ERROR.TipMsg(connection, "不能选择未开放的层数。")
	end
	if retreat:GetMopup() then
		return ERROR.TipMsg(connection, "您当前正在扫荡中。")
	end
	local cost = GetSelectNeedIngot(data)
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "您的元宝不足，请充值。")
	end
	if retreat:GetChallengeTimes() >= MAX_CHALLENGE_TIMES then
		return ERROR.TipMsg(connection, "您今日的挑战次数已经用完。")
	end
	if user:GetSetting(SETTING.RETREAT_EXP_DOUBLE) then
		if not user:DelItem(USERBASE.PACKAGE, EXP_DOUBLE_ITEM_ID, 1) then
			return ERROR.TipMsg(connection, "聚气丹数量不足。")
		end
	end
	user:SetTotalIngot(-cost)
	retreat:ChallengeSelected(data)
	local challenge = retreat:GetCurChallenge()
	local sData = {curLv = challenge.curLv, expGot = challenge.expGot}
	connection:SendProtocol("c_retreat_select", sData)
end

--进入场景
local s_retreat_enter = function(connection, data)
	if data ~= "enter" and data ~= "next" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if not retreat:GetCurChallenge() then
		if retreat:GetChallengeTimes() >= MAX_CHALLENGE_TIMES then
			return ERROR.TipMsg(connection, "您今日的挑战次数已经用完。")
		end
		if user:GetSetting(SETTING.RETREAT_EXP_DOUBLE) then
			if not user:DelItem(USERBASE.PACKAGE, EXP_DOUBLE_ITEM_ID, 1) then
				return ERROR.TipMsg(connection, "聚气丹数量不足。")
			end
		end
		retreat:InitChallenge(user:GetSetting(SETTING.RETREAT_EXP_DOUBLE))
		retreat:SetChallengeTimes(1)
	end
	if data == "next" then
		if retreat:GetLv() > retreat:GetMaxLv() then
			return ERROR.TipMsg(connection, "恭喜您，你已经到达顶层。")
		elseif retreat:GetLv() == retreat:GetCurLv() then
			return ERROR.TipMsg(connection, "下一层未开放，请升级修仙塔")
		elseif not retreat:IsLvPassed() then
			return ERROR.TipMsg(connection, "您还未通关本层，不能进入下一层。")
		end
	end
	local sceneObj = retreat:CreateScene(data)
	connection:SendProtocol("c_retreat_enter", sceneObj:GetId())
end

--退出场景
local s_retreat_leave = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if user:GetScene() ~= retreat:GetCurScene() then
		return ERROR.TipMsg(connection, "您没有在修仙塔场景中。")
	end
	retreat:SetCurScene()
	if retreat:GetCurLv() < retreat:GetOpenLv() and retreat:IsLvPassed() then
		retreat:EnterNextLevel()
	end
	connection:SendProtocol("c_retreat_leave", "true")
end

--重新开始
local s_retreat_reset = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if not retreat:GetCurChallenge() then
		return ERROR.TipMsg(connection, "您没有进入修仙塔场景。")
	end
	if retreat:GetMopup() then
		return ERROR.TipMsg(connection, "您当前正在扫荡中。")
	end
	retreat:ClearCurChallenge()
	retreat:SendRetreatUpdate()
	connection:SendProtocol("c_retreat_reset", "true")
end

--开始扫荡
local s_retreat_mopUp = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if not data.begin or not data.tail then
		return ERROR.TipMsg(connection, "请指定要扫荡的起止层数。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	if user:GetCurMopup() then
		return ERROR.TipMsg(connection, "您正在进行副本扫荡。")
	end
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if retreat:GetCurScene() then
		return ERROR.TipMsg(connection, "请先退出修仙塔副本。")
	end
	if retreat:GetMopup() then
		return ERROR.TipMsg(connection, "您当前正在扫荡中。")
	end
	local curLv = retreat:GetCurLv()
	if data.begin ~= curLv or data.tail < data.begin then
		return ERROR.TipMsg(connection, "您的当前楼层有误。")
	end
	if data.tail > retreat:GetTopLv() then
		return ERROR.TipMsg(connection, "不能选择未通过的层数。")
	end
	if data.tail <= curLv and retreat:IsLvPassed() then
		return ERROR.TipMsg(connection, "指定的楼层已经通关。")
	end
	if user:GetGold() < GetMopUpNeedGold(data.tail, data.begin) then
		return ERROR.TipMsg(connection, "您的钱币不足。")
	end
	if not retreat:GetCurChallenge() then
		if retreat:GetChallengeTimes() >= MAX_CHALLENGE_TIMES then
			return ERROR.TipMsg(connection, "您今日的挑战次数已经用完。")
		end
		if user:GetSetting(SETTING.RETREAT_EXP_DOUBLE) then
			if not user:DelItem(USERBASE.PACKAGE, EXP_DOUBLE_ITEM_ID, 1) then
				return ERROR.TipMsg(connection, "聚气丹数量不足。")
			end
		end
		retreat:InitChallenge(user:GetSetting(SETTING.RETREAT_EXP_DOUBLE))
		retreat:SetChallengeTimes(1)
	end
	retreat:BeginMopUp(data.begin, data.tail)
	connection:SendProtocol("c_retreat_mopUp", {curLv = retreat:GetCurLv(), timelong = MOPUP_TIMELONG, state = STATE_DOING, fastIngot = FINISH_MOPUP_COST})
end

--立即结束扫荡
local s_retreat_finish = function(connection, data)
	if data ~= "ingot" and data ~= "voucher" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if not retreat:GetMopup() then
		return ERROR.TipMsg(connection, "您当前没有进行修仙塔扫荡。")
	end
	if user:GetCurMopup() then
		return ERROR.TipMsg(connection, "您正在进行副本扫荡。")
	end
	local cost = GetFinishMopUpCost(retreat:GetCurLv(), retreat:GetMopUpTail())
	local moneyInfo = MONEY.GetGoldInfoByType(data)
	if USERBASE.clsUser[moneyInfo.get](user) < cost then
		return ERROR.TipMsg(connection, string.format("您的%s不足，请充值。", moneyInfo.name))
	end
	USERBASE.clsUser[moneyInfo.set](user, -cost)
	retreat:FinishMopUp("fast")
end

--取消扫荡
local s_retreat_cancel = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local tribe = user:GetTribe()
	if not tribe then
		return ERROR.TipMsg(connection, "你还没有建立部落")
	end
	local retreat = tribe:GetStruct("retreat")
	if retreat:GetMopup() then
		retreat:ClearMopUp()
	end
	connection:SendProtocol("c_retreat_cancel", "true")
end

local s_retreat_mopUpInfo = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = 0
	local tribe = user:GetTribe()
	if tribe then
		local retreat = tribe:GetStruct("retreat")
		if retreat then
			local mopup = retreat:GetMopup()
			if mopup then
				sData = {}
				sData.state = mopup.state
				sData.curLv = retreat:GetCurLv()
				if mopup.state == STATE_DOING then
					sData.timelong = math.max(mopup.timestamp - os.time(), 0)
					sData.tail = mopup.tail
					sData.fastIngot = FINISH_MOPUP_COST
				else
					sData.expGot = mopup.got.exp
				end
				sData.itemGot = mopup.got.itemList
				sData.mopupGold = MOPUP_NEED_GOLD
				sData.mopupTime = MOPUP_TIMELONG
			end
		end
	end
	connection:SendProtocol("c_retreat_mopUpInfo", sData)
end

local s_retreat_lvList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local sData = GetRetreatBossLvList()
	connection:SendProtocol("c_retreat_lvList", sData)
end

function __init__()
	
	local PRIZE = Import("autocode/retreatPrize.lua")
	PrizeList = PRIZE.GetTable()
	
	PROTOCOL.s_retreat_stat = s_retreat_stat
	PROTOCOL.s_retreat_select = s_retreat_select
	PROTOCOL.s_retreat_enter = s_retreat_enter
	PROTOCOL.s_retreat_leave = s_retreat_leave
	
	PROTOCOL.s_retreat_reset = s_retreat_reset
	
	PROTOCOL.s_retreat_mopUpInfo = s_retreat_mopUpInfo
	PROTOCOL.s_retreat_mopUp = s_retreat_mopUp
	PROTOCOL.s_retreat_finish = s_retreat_finish
	PROTOCOL.s_retreat_cancel = s_retreat_cancel

	PROTOCOL.s_retreat_lvList = s_retreat_lvList
end

