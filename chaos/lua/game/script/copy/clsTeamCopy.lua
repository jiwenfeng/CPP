--组队副本 teamCopy


function GetTeamCopyPrize(copyId, expAdd)
	local sData = {}
	local info = BASECOPY.GetCopyInfoById(copyId)
	sData.exp = info.exp * (1 + (expAdd or 0))
	sData.gold = info.gold
	sData.nimbus = info.nimbus
	return sData
end

--------------------------------------------------------

clsCopy = BASECOPY.clsCopy:Inherit()

function clsCopy:__init__()
	Super(clsCopy).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
end

function clsCopy:initsavedata()

end

function clsCopy:inittempdata()
	self.__temp.monsterList = {}
	self.__temp.clicked = {}
	self.__temp.prizeList = {}
	self.__temp.memberList = {}		--参战成员
	
	self.__temp.callout = {}
end

function clsCopy:InitCopy(copyId, owner)
	self:SetId(copyId)
	self:SetOwner(owner)
	
	self:SetType("team")
end


function clsCopy:GetNeedLv()
	local info = BASECOPY.GetCopyInfoById(self:GetId())
	return info.needLv
end

function clsCopy:SetTeam(teamObj)
	self.__temp.team = teamObj
end

function clsCopy:GetTeam()
	return self.__temp.team
end

--组队副本通关
function clsCopy:PassCopy(user)
	self:SetCopyPassed()
	self:SendCopyPassed()		--发送副本通关提示
	user:SetFuncStatById(SETTING.FUNC_TEAM_COPY, 1)
	user:SetDailyTeamCopyNum(1)
end

function clsCopy:SendCopyPassed()
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_copy_passed", {copyId = self:GetId(), type = "team"})
	end
end


function clsCopy:BeginFight(user)
	local info = BASECOPY.GetCopyInfoById(self:GetId())
	local teamObj = self:GetTeam()
	local memberList = teamObj:GetMemberFightData()
	local function call_back(fightId, winner, prize)
		user:SetWorkInfo("standby")
		if winner == "attack" then
			self:PassCopy(user)
		end
		if prize.exp then
			user:SetTeamExp(prize.exp, "teamCopy")
		end
		if prize.gold then
			user:SetGold(prize.gold)
		end
		if prize.nimbus then
			user:SetNimbus(prize.nimbus)
		end
		user:SendProtocol("c_teamCopy_fight", {result = winner == "attack"})
	end
	local _, expAdd = teamObj:GetTeamFriendAdd(self:GetOwner())
	local param = {reinforce = {defend = {}}, Lv = self:GetId(), expAdd = expAdd}
	for i = 2, #info.monsterList do
		table.insert(param.reinforce.defend, info.monsterList[i])
	end
	teamObj:ModFriendFavor()
	user:SetWorkInfo("fighting")
	local fightId = FIGHT.BeginFight("teamCopy", memberList, info.monsterList[1], teamObj:GetUserList(), call_back, param, {mapId = info.mapId, posX = 0, posY = 0})
end

function clsCopy:DestroyCopy()
	local teamObj = self:GetTeam()
	self:SetTeam()
	self:Destroy()
end

function __init__()
	
end
