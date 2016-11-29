local DEFAULT_FORM_ID = "321005"
local DEFAULT_FORM_LV = 10
local ENTER_NEED_VIGOUR = 10
local posList = {"4", "2", "6", "8"}
local STUNT_POS = "4"
local MAX_FRIEND_ADD = 0.3			--好友最大加成比率
local TEAM_COPY_ADD_FAVOR = 1		--组队副本增加好感度

function GetTeamCopyPrize(copyId, expAdd)
	local sData = {}
	local info = CLSCOPY.GetCopyInfoById(copyId)
	sData.exp = info.exp * (1 + (expAdd or 0))
	sData.gold = info.gold
	sData.nimbus = info.nimbus
	return sData
end


--队伍
clsTeam = clsObject:Inherit()

function clsTeam:__init__()
	Super(clsTeam).__init__(self)
	self.__temp = {}
	self:inittempdata()
end

function clsTeam:inittempdata()
end

function clsTeam:InitTeam(owner)
	self:SetOwner(owner)
	local form = FORMATION.NewFormation(DEFAULT_FORM_ID, DEFAULT_FORM_LV, FORMATION.FORM_USE)
	local grid = form:GetGrid("4")	--初始主角在4号位置
	grid:SetRoleId(owner)
	self:SetForm(form)
end

function clsTeam:SetOwner(id)
	self.__temp.id = id
end

function clsTeam:GetOwner()
	return self.__temp.id
end

function clsTeam:SetForm(form)
	self.__temp.form = form
end

function clsTeam:GetForm()
	return self.__temp.form
end

function clsTeam:RemoveMember(pos)
	local form = self:GetForm()
	local grid = form:GetGrid(pos)
	grid:SetRoleId()
end

function clsTeam:IsUserInList(roleId)
	local form = self:GetForm()
	for k, v in pairs(form:GetGridList()) do
		if v:GetRoleId() == roleId then
			return true
		end
	end
end

function clsTeam:FormLineup()
	local form = self:GetForm()
	local sData
	for k, v in pairs(form:GetGridList()) do
		local buf = {}
		buf.id = v:GetId()
		buf.modelId = USER.GetUserNickInfo(v:GetRoleId(), "modelId")
		table.insert(sData, buf)
	end
	return sData
end

function clsTeam:AddMember(roleId)
	local form = self:GetForm()
	for k, v in ipairs(posList) do
		local grid = form:GetGrid(v)
		if not grid:GetRoleId() then
			grid:SetRoleId(roleId)
			return true
		end
	end
	return false, "队伍已满"
end

function clsTeam:GetFormInfo()
	local sData = {}
	local form = self:GetForm()
	local num = 0
	for k, v in pairs(form:GetGridList()) do
		local roleId = v:GetRoleId()
		if roleId then
			sData[roleId] = 1
			num = num + 1
		end
	end
	return sData, num
end

function clsTeam:AutoAddMember()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local exceptList, num = self:GetFormInfo()
	if #posList == num then
		return false, "阵型已满"
	end
	local list = USER.ClassifyFightDataListByLv(user:GetLv(), #posList - num, exceptList)
	if not list or #list <= 0 then
		return false, "没有符合条件的玩家"
	end
	local form = self:GetForm()
	for k, v in ipairs(posList) do
		local grid = form:GetGrid(v)
		if grid and not grid:GetRoleId() then
			if #list <= 0 then
				break
			end
			local index = math.random(1, #list)
			grid:SetRoleId(list[index])
			table.remove(list, index)
		end
	end
	return true
end

function clsTeam:GetMemberInfo(pos)
	local form = self:GetForm()
	local buff = {pos = pos, state = 0}
	local grid = form:GetGrid(pos)
	local roleId = grid:GetRoleId()
	if roleId then
		buff.state = 1
		buff.roleId = roleId
		buff.name = USER.GetUserNickById(roleId)
		buff.headId = USER.GetUserNickInfo(roleId, "headId")
		buff.job = USER.GetUserNickInfo(roleId, "job")
		buff.aptLv = 5
		buff.modelId = USER.GetUserNickInfo(roleId, "modelId")
		local user = USER.GetOnlineUser(roleId)
		if user then
			buff.Lv = user:GetLv()
			buff.force = user:GetFightForce()
			buff.skillList = {}
			local skillData = user:GetSkill(1)
			if skillData and skillData.id then
				table.insert(buff.skillList, {type = 1, skillId = skillData.id, Lv = skillData.Lv})
			end
			local pet = user:GetPet()
			if pet and pet:GetForm() ~= 0 then
				local skillId = pet:GetSkillId()
				table.insert(buff.skillList, {type = 5, skillId = skillId, Lv = pet:GetSkillLv()})
			end
		 	buff.hp = math.ceil(user:GetTotalAttr("hpMax"))
			buff.hpMax = math.ceil(user:GetTotalAttr("hpMax"))
			buff.physcAtkMin = math.ceil(user:GetTotalAttr("physcAtkMin"))
			buff.physcAtkMax = math.ceil(user:GetTotalAttr("physcAtkMax"))
			buff.physcDef = math.ceil(user:GetTotalAttr("physcDef"))
			buff.magicAtkMin = math.ceil(user:GetTotalAttr("magicAtkMin"))
			buff.magicAtkMax = math.ceil(user:GetTotalAttr("magicAtkMax"))
			buff.magicDef = math.ceil(user:GetTotalAttr("magicDef"))
			buff.speed = math.ceil(user:GetTotalAttr("speed"))
		else
			local fightData = FIGHT.GetFightDataById(roleId)
			if fightData then
				local userData = fightData.list[roleId]
				buff.Lv = userData.Lv
				buff.force = userData.force
				buff.skillList = {}
				if userData.skillList["1"] and userData.skillList["1"].id then
					table.insert(buff.skillList, {type = 1, skillId = userData.skillList["1"].id, Lv = userData.skillList["1"].Lv})
				end
				if userData.stuntId then
					table.insert(buff.skillList, {type = 5, skillId = userData.stuntId, Lv = userData.stuntLv})
				end
				buff.hp = math.ceil(userData.hpMax)
				buff.hpMax = math.ceil(userData.hpMax)
				buff.physcAtkMin = math.ceil(userData.physcAtkMin)
				buff.physcAtkMax = math.ceil(userData.physcAtkMax)
				buff.physcDef = math.ceil(userData.physcDef)
				buff.magicAtkMin = math.ceil(userData.magicAtkMin)
				buff.magicAtkMax = math.ceil(userData.magicAtkMax)
				buff.magicDef = math.ceil(userData.magicDef)
				buff.speed = math.ceil(userData.speed)
			end
		end
	end
	return buff
end

function clsTeam:GetFormatMemberList(call_back)
	local form = self:GetForm()
	local userList = {}
	for _, v in pairs(posList) do
		local grid = form:GetGrid(v)
		local roleId = grid:GetRoleId()
		if roleId then
			table.insert(userList, roleId)
		end
	end	
	local function read_back(ret)
		local sData = {}
		for _, v in ipairs(posList) do
			local buff = self:GetMemberInfo(v)
			table.insert(sData, buff)
		end
		call_back(sData)
	end
	FIGHT.LoadFightDataList(userList, read_back)
end

function clsTeam:Switch(src, dst)
	local form = self:GetForm()
	local gsrc = form:GetGrid(src)
	local gdst = form:GetGrid(dst)
	if not gsrc or not gdst then
		return false, "无效的位置"
	end
	local temp = gsrc:GetRoleId()
	if not temp then
		return false, "没有需要移动的角色"
	end
	gsrc:SetRoleId(gdst:GetRoleId())
	gdst:SetRoleId(temp)
	return true
end

function clsTeam:GetTeamFriendAdd(roleId)
	local attrAdd, expAdd
	local user = USER.GetOnlineUser(roleId)
	local form = self:GetForm()
	if user then
		local friend = user:GetFriendList()
		for k, v in pairs(form:GetGridList()) do
			local r = v:GetRoleId()
			if r then
				local info = friend:IsInMemberList("friend", r)
				if info and info.type == 2 then
					local favorInfo = FRIEND.GetFavorLvInfo(info.favorLv)
					if favorInfo then
						attrAdd = (attrAdd or 0) + favorInfo.attrAdd
						expAdd = (expAdd or 0) + favorInfo.expAdd
					end
				end
			end
		end
	end
	if attrAdd and attrAdd > MAX_FRIEND_ADD then
		attrAdd = MAX_FRIEND_ADD
	end
	if expAdd and expAdd > MAX_FRIEND_ADD then
		expAdd = MAX_FRIEND_ADD
	end
	return attrAdd, expAdd
end

function clsTeam:GetMemberFightData()
	local sData = {}
	sData.lineup = {}
	local form = self:GetForm()
	sData.formId = form:GetId()
	sData.formLv = form:GetLv()
	for k, v in pairs(form:GetGridList()) do
		local buff = {}
		local roleId = v:GetRoleId()
		if roleId then
			buff.roleId = roleId
			buff.id = roleId
			buff.pos = v:GetId()
			buff.attrAdd = self:GetTeamFriendAdd(roleId)
			if v:GetId() == STUNT_POS then
				buff.isMain = 1
			end
			table.insert(sData.lineup, buff)
		end
	end
	return sData
end

function clsTeam:ModFriendFavor()
	local user = USER.GetOnlineUser(self:GetOwner())
	local form = self:GetForm()
	if user then
		local friend = user:GetFriendList()
		for k, v in pairs(form:GetGridList()) do
			local roleId = v:GetRoleId()
			if roleId then
				local info = friend:IsInMemberList("friend", roleId)
				if info and info.type == 2 then
					FRIEND.ModUserFavor(user, k, TEAM_COPY_ADD_FAVOR)
				end
			end
		end
	end
end

function NewTeam(roleId)
	local obj = clsTeam:New()
	obj:InitTeam(roleId)
	return obj
end

--组队副本
clsTeamCopy = CLSCOPY.clsCopy:Inherit()
function clsTeamCopy:__init__()
	Super(clsTeamCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:inittempdata()
end

function clsTeamCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.callout = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
	self.__temp.stats = {}
end

function clsTeamCopy:GetType()
	return "team"
end

function clsTeamCopy:InitCopyInfo(id, owner, flag)
	self:SetId(id)
	self:SetOwner(owner)
	self:ClassifyPrizeList()
	local obj = SCENE.GetEmptyScene("team")
	self:SetScene(obj)
end

function clsTeamCopy:GetNeedLv()
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	return info.needLv
end

function clsTeamCopy:GetNeedVigour()
	return ENTER_NEED_VIGOUR
end

function clsTeamCopy:ClassifyPrizeList()
	self.__temp.prize = {}
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	for k, v in ipairs(info.itemList) do
		local info = BASEITEM.GetItemById(v.id)
		self.__temp.prize[info.quality] = self.__temp.prize[info.quality] or {}
		table.insert(self.__temp.prize[info.quality], v)
	end
end

function clsTeamCopy:GetPrizeListByQuality(quality)
	return self.__temp.prize[quality]
end

function clsTeamCopy:GenPrizeList(num)
	local qualityList = {[1] = 0.35, [2] = 0.35, [3] = 0.15, [4] = 0.1, [5] = 0.05} 
	local ran = math.random()
	local rate = 0
	local list
	local prize = {}
	for i = 1, num or 1 do
		for k, v in ipairs(qualityList) do
			rate = v + rate
			if ran <= rate then
				list = self:GetPrizeListByQuality(k)
				table.insert(prize, list[math.random(1, #list)])
				break
			end
		end
	end
	return prize 
end

function clsTeamCopy:GenPrizeShown()
	for i = 1, CLSCOPY.MAX_PRIZE_NUM do
		if not self:IsPosClicked(i) then
			local prize = self:GenPrizeList()
			self:SetPrizeGot(i, {goodsId = prize[1].id, num = prize[1].num})
			self:SetClickPos(i)
		end
	end
end

function clsTeamCopy:SendClickReward(pos)
	local roleId = self:GetOwner()
	self:SetClickPos(pos)
	local prize = self:GenPrizeList()
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

function clsTeamCopy:Enter()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if user:GetLv() < self:GetNeedLv() then
		return false, "等级不足"
	end
	if user:GetDailyTeamCopyNum() >= user:GetDailyTeamCopyMax() then
		return false, "今日进入次数已用完"
	end
	local team = user:GetTeam()
	if not team then
		return false, "您还没有创建队伍"
	end
	local form = team:GetForm()
	local num = 0
	for k, v in pairs(form:GetGridList()) do
		if v:GetRoleId() then
			num = num + 1
		end
	end
	if num < 4 then
		return false, "队伍未满，不能进入副本"
	end
	if user:GetVigour() < self:GetNeedVigour() then
		return false, "您精力不足，不能进入副本"
	end
	user:SetVigour(-self:GetNeedVigour())
	user:SetDailyTeamCopyNum(1)
	user:SetCurCopy(self)
	return true
end

function clsTeamCopy:SetFoeHpSum(hp)
	self.__temp.stats.foeHp = (self.__temp.stats.foeHp or 0) + hp
end

function clsTeamCopy:GetFoeHpSum()
	return self.__temp.stats.foeHp or 0
end
--伤害总和
function clsTeamCopy:SetDamageSum(damage)
	self.__temp.stats.damage = (self.__temp.stats.damage or 0) + damage
end

function clsTeamCopy:GetDamageSum()
	return self.__temp.stats.damage or 0
end

--我方血量总和
function clsTeamCopy:SetMyHpSum(hp)
	self.__temp.stats.myHpSum = (self.__temp.stats.myHpSum or 0) + hp
end

function clsTeamCopy:GetMyHpSum()
	return self.__temp.stats.myHpSum or 0
end

--我方剩余血量
function clsTeamCopy:SetMyHpLeft(hp)
	self.__temp.stats.myHpLeft = (self.__temp.stats.myHpLeft or 0) + hp
end

function clsTeamCopy:GetMyHpLeft()
	return self.__temp.stats.myHpLeft or 0
end

function clsTeamCopy:CalCopyScore()
	local ret = {}
	ret.atkScore = CLSCOPY.GetAttackScore(self:GetFoeHpSum(), self:GetDamageSum())
	ret.defScore = CLSCOPY.GetDefenceScore(self:GetMyHpLeft(), self:GetMyHpSum())
	ret.rate = CLSCOPY.GetAddRateByScore(ret.atkScore + ret.defScore)
	return ret
end

function clsTeamCopy:GetPassCopyExp()
	local ret = self:CalCopyScore()
	local sData = {}
	sData.atkScore = ret.atkScore
	sData.defScore = ret.defScore
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	sData.baseExp = info.exp
	sData.addExp = math.ceil(info.exp * (ret.rate or 1))
	return sData
end

--组队副本战斗
function clsTeamCopy:ProFight()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local info = CLSCOPY.GetCopyInfoById(self:GetId())
	local teamObj = user:GetTeam()
	local memberList = teamObj:GetMemberFightData()
	local function call_back(fightId, winner, prize, stats)
		user:SetWorkInfo("standby")
		self:SetFoeHpSum(stats.defHpSum)
		self:SetDamageSum(stats.atkDamage)
		self:SetMyHpSum(stats.atkHpSum)
		self:SetMyHpLeft(stats.atkHp)
		if prize.exp then
			user:SetTeamExp(prize.exp, "teamCopy")
		end
		if prize.gold then
			user:SetGold(prize.gold)
		end
		if prize.nimbus then
			user:SetNimbus(prize.nimbus)
		end
		if winner == "attack" then
			self:PassCopy()
		end
	end
	local _, expAdd = teamObj:GetTeamFriendAdd(self:GetOwner())
	local param = {reinforce = {defend = {}}, Lv = self:GetId(), expAdd = expAdd}
	for i = 2, #info.monsterList do
		table.insert(param.reinforce.defend, info.monsterList[i])
	end
	teamObj:ModFriendFavor()
	user:SetWorkInfo("fighting")
	local fightId = FIGHT.BeginFight("teamCopy", memberList, info.monsterList[1], {self:GetOwner()}, call_back, param, {mapId = info.mapId, posX = 0, posY = 0})
end

function clsTeamCopy:PassCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	self:SetCopyPassed()
	self:SendCopyPassed()
end

function clsTeamCopy:SendCopyStats()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = self:GetPassCopyExp()
	user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
	self:SetPassExpGot(true)
	user:SendProtocol("c_copy_stats", sData)
end

function clsTeamCopy:DestroyCopy()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	if self:IsCopyPassed() then
		if not self:IsPassExpGot() then
			local sData = self:GetPassCopyExp()
			user:SetTeamExp(sData.baseExp + sData.addExp)		--通关经验
			self:SetPassExpGot(true)
		end
		local itemList = self:GenPrizeList(self:GetClickMaxTimes(self:GetOwner()) - self:GetClickTimes(self:GetOwner()))
		for k, v in pairs(item or {}) do
			local item = BASEITEM.NewItem(v.id, v.num)
			user:AddItem(USERBASE.PACKAGE, item)
		end
	end
	local scene = self:GetScene()
	local npcList = scene:GetSceneRoleList()
	for k, v in pairs(npcList) do
		if v:GetRoleType() == "user" then
			v:LeaveScene()
		else
			NPC.NpcLeave(v)
		end
	end
	self:Destroy()
end
