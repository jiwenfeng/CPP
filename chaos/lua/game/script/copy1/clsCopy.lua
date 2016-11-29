--副本状态
COPY_CLOSE = 0
COPY_OPEN = 1
COPY_PASSED = 2

local AUTO_CHOOSE_TIMELONG = 5		--自动领取奖励时间
local NORMAL_CLICK_TIMES = 1		--普通玩家可领取奖励次数
local VIP_CLICK_TIMES = 2			--vip玩家可领取奖励次数
MAX_PRIZE_NUM = 5				--最大生成奖励数量

local prizeRate = {["show"] = {35, 20, 20, 25}, ["got"] = {50, 35, 14, 1}}		--奖励物品品质概率

local CopyTable = {}

local scoreList = {
	[1] = {min = 1, max = 60, rate = 0},
	[2] = {min = 61, max = 80, rate = 0.05},
	[3] = {min = 81, max = 90, rate = 0.1},
	[4] = {min = 91, max = 100, rate = 0.15},
}

function GetCopyInfoById(id)
	return CopyTable[id]
end

function GetCopyList()
	return CopyTable
end

function GetAddRateByScore(score)
	for k, v in ipairs(scoreList) do
		if score >= v.min and score <= v.max then
			return v.rate
		end
	end
end

--攻击得分
function GetAttackScore(hp, damage)
	local score = math.ceil((1 - hp / damage) * 60 + 20)
	return math.min(score, 50)
end

--防守得分
function GetDefenceScore(hp, hpMax)
	if hp > hpMax then
		hp = hpMax
	end
	return math.ceil((hp / hpMax) * 30 + 20)
end

--生成奖励物品品质
function GenPrizeQuality(type, flag)
	local list = {}
	local rateList = prizeRate[type]
	local sum = 0
	for k, v in ipairs(rateList) do 
		if not flag or k < #rateList then
			sum = sum + v
			table.insert(list, v)
		end
	end
	local ran = math.random(0, sum)
	local rate = 0
	for k, v in ipairs(list) do
		rate = rate + v
		if rate >= ran then
			return k
		end
	end
end

function GetCopyFightPrize(copyId, teamId, roleId)
	local info = GetCopyInfoById(copyId)
	local teamInfo
	for k, v in pairs(info.monsterList) do
		if v.teamId == teamId then
			teamInfo = v
			break
		end
	end
	if not teamInfo then return end
	local prize = {}
	prize.gold = teamInfo.gold
	prize.exp = teamInfo.exp
	prize.nimbus = teamInfo.nimbus
	prize.item = {}
	local user = USER.GetOnlineUser(roleId)
	if user then
		for k, v in pairs(teamInfo.itemList) do
			local misObj = user:GetMissionById(v.needMissionId)
			if not v.needMissionId or (misObj and misObj:GetState() ~= CLSMISS.FINISHED) then
			--	local num = math.random(v.min, v.max)
				local num = 1
				if num > 0 then
					table.insert(prize.item, {goodsId = v.id, num = num, missionId = v.needMissionId})
				end
			end
		end
	end
	return prize
end

function GetPrizeNum()
	return MAX_PRIZE_NUM
end

clsCopy = clsObject:Inherit()
function clsCopy:__init__()
	Super(clsCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
end

function clsCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.callout = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
end

--
--flag true 不创建副本场景
--
function clsCopy:InitCopyInfo(id, owner, flag)
	self:SetId(id)
	self:SetOwner(owner)
	print("InitCopyInfo:", id, owner)
	if flag then 
		return
	end
	local info = GetCopyInfoById(id)
	if info.mapId then
		local sceneObj = SCENE.NewScene(info.type, info.mapId, true)
		for _, v in ipairs(info.monsterList) do
			local npcObj = NPC.NewNpc(v.teamId)
			npcObj:EnterScene(sceneObj, v.posX, v.posY)
			npcObj:SetTarget(owner)
			self:SetCopyMonster(v.teamId, npcObj:GetRoleId())
		end
		self:SetScene(sceneObj)
	end
end

function clsCopy:SetOwner(owner)
	self.__temp.owner = owner
end

function clsCopy:GetOwner()
	return self.__temp.owner
end

function clsCopy:SetId(id)
	self.__temp.id = id
end

function clsCopy:GetId()
	return self.__temp.id
end

function clsCopy:SetCopyMonster(teamId, roleId)
	if not teamId or not roleId then
		return
	end
	table.insert(self.__temp.monsterList, {roleId = roleId, teamId = teamId})
end

function clsCopy:GetCopyMonster(teamId)
	for k, v in pairs(self.__temp.monsterList) do
		if v.teamId == teamId then
			return v.roleId
		end
	end
end

function clsCopy:DelCopyMonster(roleId)
	for k, v in pairs(self.__temp.monsterList) do
		if v.roleId == roleId then
			return table.remove(self.__temp.monsterList, k)			
		end
	end
end

function clsCopy:GetCopyMonsterList()
	return self.__temp.monsterList
end

function clsCopy:GetCopyMonsterList()
	return self.__temp.monsterList
end

function clsCopy:IsCopyPassed()
	return self.__temp.isPass
end

function clsCopy:SetCopyPassed()
	self.__temp.isPass = true
end

function clsCopy:SendCopyPassed()
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_copy_passed", {copyId = self:GetId(), type = self:GetType()})
	end
end

function clsCopy:GetDescribe()
	local info = GetCopyInfoById(self:GetId())
	return info.describe
end

function clsCopy:GetName()
	local info = GetCopyInfoById(self:GetId())
	return info.name
end

function clsCopy:SetScene(data)
	self.__temp.scene = data
end

function clsCopy:GetScene()
	return self.__temp.scene
end

function clsCopy:DestroyCopy()
	local scene = self:GetScene()
	local npcList = scene:GetSceneRoleList()
	for k, v in pairs(npcList) do
		if v:GetRoleType() == "user" then
			v:LeaveScene()
		else
			NPC.NpcLeave(v)
		end
	end
	SCENE.DestroyScene(scene)
	self:Destroy()
end

local function InitCopyTable(tb)
	for k, v in pairs(tb) do
		CopyTable[k] = v
	end
end

function clsCopy:ProFight(id)
	local scene = self:GetScene()
	if not scene then
		return false, "无效的副本"
	end
	local npcObj = scene:GetSceneRole(id)
	if not npcObj then
		return false, "指定的敌人不存在。"
	end
	local roleId = self:GetOwner()
	local user = assert(USER.GetOnlineUser(roleId))
	local function callback(fightId, winner, prize, stats)
		self:ProFightResult(npcObj, fightId, winner, prize, stats)		--处理战斗结果
	end
	local param = {saveHp = true, restoreHp = true, Lv = self:GetId(), teamId = npcObj:GetId()}
	local copyInfo = GetCopyInfoById(self:GetId())
	if user:GetMissionById(copyInfo.needMissionId) then
		local misInfo = CLSMISS.GetMissionInfoById(copyInfo.needMissionId)
		if misInfo.remark then
			param.maxRage = true
		end
	end
	local locate = user:GetLocateInfo()
	local scene = self:GetScene()
	user:SetWorkInfo("fighting")
	npcObj:SetState("fighting")
	local fightId = FIGHT.BeginFight("copy", roleId, npcObj:GetId(), {roleId}, callback, param, {mapId = scene:GetMapId(), posX = locate.x, posY = locate.y})
	return true
end

function clsCopy:ProFightResult(npcObj, fightId, winner, prize, stats)		--处理战斗结果
end

function clsCopy:PassCopy()
end

function clsCopy:IsPrizeTaken(roleId)
	if not self:IsCopyPassed() then
		return false
	end
	return self:GetClickTimes() > self:GetClickMaxTimes()
end

function clsCopy:SetClickPos(pos)
	self.__temp.clicked[pos] = true
end

function clsCopy:IsPosClicked(pos)
	return self.__temp.clicked[pos]
end

function clsCopy:GetClickTimes()
	return table.size(self.__temp.clicked)
end

function clsCopy:GetClickMaxTimes()
	local vipLv = USER.GetUserNickInfo(self:GetOwner(), "vipLv")
	if VIP.IsVipFuncValid(vipLv, VIP.VIP_OPEN_EXTRA_CARD) then
		return VIP_CLICK_TIMES
	end
	return NORMAL_CLICK_TIMES
end

function clsCopy:SendClickReward(pos)
	local roleId = self:GetOwner()
	self:SetClickPos(pos)
--	local quality = GenPrizeQuality("got")
	local info = GetCopyInfoById(self:GetId())
	local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
	local ret = {pos = pos, goodsId = prize[1].id, num = prize[1].num}
	local moneyType = MONEY.GetGoldTypeByPicId(ret.goodsId)
	local user = USER.GetOnlineUser(roleId)
	if user then
		if moneyType then
			local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
			USERBASE.clsUser[moneyInfo.set](user, ret.num)
		else
			user:AddCopyItem({ret}, self:GetName())
		end
	else
		if moneyType then
			OFFLINE.SetOfflineData(roleId, moneyType, ret.num)
		else
			OFFLINE.SetOfflineData(roleId, "item", USERBASE.PACKAGE, ret.goodsId, ret.num)
		end
	end
	self:SetPrizeGot(pos, ret, true)
	return ret
end

function clsCopy:SetPrizeGot(pos, item, flag)
	local buf = {goodsId = item.goodsId, num = item.num , pos = pos}
	if flag then
		buf.owner = self:GetOwner()
	end
	table.insert(self.__temp.prizeList, buf)
end

function clsCopy:GenPrizeShown()
	local info = GetCopyInfoById(self:GetId())
	for i = 1, MAX_PRIZE_NUM do
		if not self:IsPosClicked(i) then
			local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
			self:SetPrizeGot(i, {goodsId = prize[1].id, num = prize[1].num})
			self:SetClickPos(i)
		end
	end
end

function clsCopy:GetPrizeList()
	self:GenPrizeShown()
	return self.__temp.prizeList
end

function clsCopy:AutoChoose()
	self:RemoveChooseCallOut()
	local roleId = self:GetOwner()
	local list = {}
	for i = 1, MAX_PRIZE_NUM do
		table.insert(list, i)
	end
	local times = self:GetClickMaxTimes() - self:GetClickTimes()
	for i = 1, times do
		local index = math.random(1, #list)
		self:SendClickReward(list[index])
		table.remove(list, index)
	end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_copy_prizeShow", self:GetPrizeList())
	end
end

function clsCopy:GetPrizeChooseTimestamp()
	return self.__temp.chooseTms or os.time()
end

function clsCopy:StartChooseCallOut()
	if not self.__temp.timer then
		self.__temp.chooseTms = os.time() + AUTO_CHOOSE_TIMELONG
		self.__temp.timer = CALLOUT.ObjCallOut(self.__temp.callout, clsCopy.AutoChoose, AUTO_CHOOSE_TIMELONG, self)
	end
end

function clsCopy:RemoveChooseCallOut()
	if self.__temp.timer then
		CALLOUT.RemoveCallOut(self.__temp.callout, self.__temp.timer)
	end
end

function clsCopy:SendCopyStats()
end

--副本通关经验发放
function clsCopy:SetPassExpGot(state)
	self.__temp.passExpGot = state
end

function clsCopy:IsPassExpGot()
	return self.__temp.passExpGot
end

function clsCopy:Enter()
end

function clsCopy:GetGroundItemList()
end

function clsCopy:pickup(data)
	return false, "本副本没有可拾取的物品"
end

function clsCopy:CheckMopupCondt()
	return false, "本副本没有扫荡功能"
end

function clsCopy:FastMopup()
	return false, "本副本没有扫荡功能"
end

function clsCopy:StopMopup()
	return false, "本副本没有扫荡功能"
end

function __init__()
	local normal = Import("autocode/normalCopyList.lua")
	InitCopyTable(normal.GetTable())
	local team = Import("autocode/teamCopyList.lua")
	InitCopyTable(team.GetTable())
	local hero = Import("autocode/heroCopyList.lua")
	InitCopyTable(hero.GetTable())
end
