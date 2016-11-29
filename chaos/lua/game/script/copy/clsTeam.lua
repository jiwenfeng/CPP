--组队队伍

local posList = {"4", "2", "6", "8"}	--组队位置
local STUNT_POS = "4"		--必杀技位置
local COPY_FORM_ID = "321005"		--组队副本默认阵型
local COPY_FORM_LV = 10				--组队副本默认阵型等级
local MAX_FRIEND_ADD = 0.3			--好友最大加成比率
local TEAM_COPY_ADD_FAVOR = 1		--组队副本增加好感度


function IsValidPos(pos)
	for _, v in pairs(posList) do
		if v == pos then
			return true
		end
	end
	return false
end
-------------------------------------------------

clsTeam = clsObject:Inherit()

function clsTeam:__init__()
	Super(clsTeam).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsTeam:initsavedata()

end

function clsTeam:inittempdata()
	
	self.__temp.memberList = {}		--参战成员
	self.__temp.callout = {}
	
	self.__temp.fightDataList = {}	--战斗数据引用表
end

function clsTeam:InitCopy(index, owner)
	self:SetIndex(index)
	self:SetOwner(owner)
	
	local pos = self:GetNextFreePos()
	self:AddToList(pos, owner, true)
end

function clsTeam:SetIndex(index)
	self.__temp.index = index
end

function clsTeam:GetIndex()
	return self.__temp.index
end

function clsTeam:SetOwner(owner)
	self.__temp.owner = owner
end

function clsTeam:GetOwner()
	return self.__temp.owner
end


function clsTeam:GetPosMax()
	return #posList
end


function clsTeam:IsPosTaken(pos)
	return self.__temp.memberList[pos]
end

function clsTeam:GetNextFreePos()
	for _, v in ipairs(posList) do
		if not self:IsPosTaken(v) then
			return v
		end
	end
end

function clsTeam:GetMemberList()
	return self.__temp.memberList
end

function clsTeam:RemoveMemberByPos(pos)
	self.__temp.memberList[pos] = nil
end

function clsTeam:AddToList(pos, roleId, flag)
	self.__temp.memberList[pos] = {roleId = roleId, isUser = flag}
	local function read_back(ret)
		if not ret then
			return
		end
		local fightData = FIGHT.GetFightDataById(roleId)
		if roleId ~= self:GetOwner() then
			self:SendMemberUpdate(pos)
		end
	end
	FIGHT.RestoreUserFightData(roleId, read_back)
end

function clsTeam:IsInList(roleId)
	for k, v in pairs(self.__temp.memberList) do
		if v.roleId == roleId then
			return k
		end
	end
end

function clsTeam:GetUserList()
	local buff = {}
	for k, v in pairs(self.__temp.memberList) do
		if v.isUser then
			table.insert(buff, v.roleId)
		end
	end
	return buff
end


function clsTeam:GetMemberInfo(pos)
	local buff = {pos = pos, state = 0}
	local info = self:IsPosTaken(pos)
	if info and info.roleId then
		buff.state = 1
		buff.roleId = info.roleId
		buff.name = USER.GetUserNickById(info.roleId)
		buff.headId = USER.GetUserNickInfo(info.roleId, "headId")
		buff.job = USER.GetUserNickInfo(info.roleId, "job")
		buff.aptLv = 5
		local user = USER.GetOnlineUser(info.roleId)
		if user then
			buff.modelId = user:GetModelId()
			buff.Lv = user:GetLv()
			buff.force = user:GetFightForce()
			buff.skillList = {}
			local skillData = user:GetSkill(1)
			if skillData and skillData.id then
	--			buff.skillName = SKILL.GetSkillNameById(skillData.id)
				table.insert(buff.skillList, {type = 1, skillId = skillData.id, Lv = skillData.Lv})
			end
			local pet = user:GetPet()
			if pet and pet:GetForm() ~= 0 then
				local skillId = pet:GetSkillId()
	--			buff.stuntName = SKILL.GetSkillNameById(skillId)
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
			local fightData = FIGHT.GetFightDataById(info.roleId)
			if fightData then
				local userData = fightData.list[info.roleId]
				buff.modelId = userData.modelId
				buff.Lv = userData.Lv
				buff.force = userData.force
				buff.skillList = {}
				if userData.skillList["1"] and userData.skillList["1"].id then
			--		buff.skillName = SKILL.GetSkillNameById(userData.skillList[1].id)
					table.insert(buff.skillList, {type = 1, skillId = userData.skillList["1"].id, Lv = userData.skillList["1"].Lv})
				end
				if userData.stuntId then
			--		buff.stuntName = SKILL.GetSkillNameById(userData.stuntId)
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
	local list = self:GetMemberList()
	local userList = {}
	for _, v in pairs(posList) do
		local info = self:IsPosTaken(v)
		if info and info.roleId then
			table.insert(userList, info.roleId)
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

--[[
function clsTeam:SendTeamMsg(msg)
	local list = self:GetMemberList()
	local memberList = {}
	for _, v in ipairs(list) do
		if v.roleId and v.isUser then
			table.insert(memberList, v.roleId)
		end
	end
	CHAT.SendSysMsg("system", msg, memberList)
end]]

function clsTeam:NewMember(roleId, flag)
	local pos = self:GetNextFreePos()
	if pos then
--		self:SendTeamMsg(string.format("【%s】加入队伍", USER.GetUserNickInfo(roleId, "nick")))
		self:AddToList(pos, roleId, flag)
	end
end

function clsTeam:DelMember(roleId)
	local pos = self:IsInList(roleId)
	if pos then
		local info = self:IsPosTaken(pos)
		self:RemoveMemberByPos(pos, roleId)
		self:SendMemberUpdate(pos)
--		self:SendTeamMsg(string.format("【%s】离开队伍", USER.GetUserNickInfo(roleId, "nick")))
		if info.isUser then
			local user = USER.GetOnlineUser(roleId)
			if user then
				user:SetCurCopy()
			end
		end
		if roleId == self:GetOwner() then
			self:TransferOwner()
		end
	end
end
--队长转让
function clsTeam:TransferOwner()
	local flag = false
	for i = 1, #posList do
		local pos = posList[i]
		local info = self:IsPosTaken(pos)
		if info and info.isUser then
			self:SetOwner(info.roleId)
			flag = true
		end
	end
	if not flag then
		TEAMCOPY.DestroyCopy(self:GetId())
	end
end

function clsTeam:AutoInvite()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local list = USER.ClassifyFightDataListByLv(user:GetLv(), #posList - 1)
	local function get_member()
		for k, v in ipairs(list) do
			if #v > 0 then
				local index = math.random(1, #v)
				local roleId = v[index]
				table.remove(list[k], index)
				if self:IsInList(roleId) then
					return get_member()
				end
				return roleId
			end
		end
	end
	for i = 1, #posList do
		if #list <= 0 then
			break
		end
		local pos = posList[i]
		if not self:IsPosTaken(pos) then
			local roleId = get_member()
			if roleId then
				self:AddToList(pos, roleId)
			end
		end
	end
end

--副本成员的战斗数据列表
function clsTeam:GetMemberFightData()
	local sData = {}
	sData.formId = COPY_FORM_ID
	sData.formLv = COPY_FORM_LV
	sData.lineup = {}
	for k, v in pairs(self:GetMemberList()) do
		local buff = {}
		buff.roleId = v.roleId
		buff.id = v.roleId
		buff.pos = k
		buff.attrAdd = self:GetTeamFriendAdd(v.roleId)
		if k == STUNT_POS then
			buff.isMain = 1
		end
		table.insert(sData.lineup, buff)
	end
	return sData
end

function clsTeam:IsUserInTeam(roleId)
	for k, v in pairs(self:GetMemberList()) do
		if v.roleId == roleId then
			return true
		end
	end
end

function clsTeam:SendMemberUpdate(pos)
	local buff = self:GetMemberInfo(pos)
	self:SendTeamUpdate("c_teamCopy_update", buff)
end

--向队伍里所有人发送更新
function clsTeam:SendTeamUpdate(pro, data)
	for k, v in pairs(self:GetMemberList()) do
		if v.isUser then
			local con = LOGIN.GetConnection(v.roleId)
			if con and con.roleId == v.roleId then
				con:SendProtocol(pro, data)
			end
		end
	end
end

function clsTeam:SwitchMember(src, dest)
	self.__temp.memberList[src], self.__temp.memberList[dest] = self.__temp.memberList[dest], self.__temp.memberList[src]
end

function clsTeam:IsTeamEmpty()
	for k, v in pairs(self:GetMemberList()) do
		if v.isUser then
			return false
		end
	end
	return true
end

function clsTeam:GetTeamFriendAdd(roleId)
	local attrAdd, expAdd
	local user = USER.GetOnlineUser(roleId)
	if user then
		local friend = user:GetFriendList()
		for k, v in pairs(self:GetMemberList()) do
			local info = friend:IsInMemberList("friend", k)
			if info and info.type == 2 then
				local favorInfo = FRIEND.GetFavorLvInfo(info.favorLv)
				if favorInfo then
					attrAdd = (attrAdd or 0) + favorInfo.attrAdd
					expAdd = (expAdd or 0) + favorInfo.expAdd
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

function clsTeam:ModFriendFavor()
	local user = USER.GetOnlineUser(self:GetOwner())
	if user then
		local friend = user:GetFriendList()
		for k, v in pairs(self:GetMemberList()) do
			local info = friend:IsInMemberList("friend", k)
			if info and info.type == 2 then
				FRIEND.ModUserFavor(user, k, TEAM_COPY_ADD_FAVOR)
			end
		end
	end
end
