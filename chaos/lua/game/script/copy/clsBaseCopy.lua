--副本类

local AllCopyList = {["normal"] = {}, ["hero"] = {}, ["team"] = {}}

COPY_CLOSE = 0
COPY_OPEN = 1
COPY_PASSED = 2

local CopyIdList = {
	{type = "normal", min = 600001, max = 601999},
	{type = "team", min = 602001, max = 602999},
	{type = "hero", min = 604001, max = 604999},
}
local prizeRate = {["show"] = {35, 20, 20, 25}, ["got"] = {50, 35, 14, 1}}		--奖励物品品质概率
local AUTO_CHOOSE_TIMELONG = 5		--自动领取奖励时间
local NORMAL_CLICK_TIMES = 1		--普通玩家可领取奖励次数
local VIP_CLICK_TIMES = 2			--vip玩家可领取奖励次数
local MAX_PRIZE_NUM = 5				--最大生成奖励数量


function GetCopyTypeById(id)
	local numId = tonumber(id)
	if not numId then return end
	for _, v in pairs(CopyIdList) do
		if numId >= v.min and numId <= v.max then
			return v.type
		end
	end
end

--获取副本信息
function GetCopyInfoById(id)
	local type = GetCopyTypeById(id)
	if not AllCopyList[type] then
		return
	end
	return AllCopyList[type][id]
end

function GetCopyInfoList(type)
	return AllCopyList[type]
end

function GetAllCopyList()
	return AllCopyList
end

function GetPrizeNum()
	return MAX_PRIZE_NUM
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

function AddCopyItem(user, itemList, copyName)
	local list = {}
	for _, v in ipairs(itemList) do
		local item = BASEITEM.NewItem(v.goodsId, v.num, {missionId = v.missionId})
		if user:IsItemCanAdd(USERBASE.PACKAGE, item) then
			user:AddItem(USERBASE.PACKAGE, item)
		else
			table.insert(list, {id = v.goodsId, num = v.num, missionId = v.missionId})
		end
	end
	if #list > 0 then
		local mail = {}
		mail.title = "副本掉落物品"
		mail.content = string.format("亲，由于背包已满，您在副本【%s】中获得的奖励已发送到邮箱，请及时领取。", copyName)
		mail.attach = {}
		mail.attach.item = list
		MAIL.SendSysMail(user:GetRoleId(), mail)
		user:SendProtocol("c_show_tips", string.format("由于背包已满，您在副本【%s】中获得的奖励已发送到邮箱，请及时领取。", copyName))
	end
end
-------------------------------------------------
clsCopy = clsObject:Inherit()

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
	
	self.__temp.callout = {}
end

function clsCopy:InitCopy(id, owner)
	self:SetId(id)
	self:SetOwner(owner)
	local info = GetCopyInfoById(id)
	self:SetType(info.type)
	if type == "normal" then
		self:InitGroundList()
	end
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

function clsCopy:SetId(id)
	self.__temp.id = id
end

function clsCopy:GetId()
	return self.__temp.id
end

function clsCopy:SetOwner(owner)
	self.__temp.owner = owner
end

function clsCopy:GetOwner()
	return self.__temp.owner
end

function clsCopy:SetScene(data)
	self.__temp.scene = data
end

function clsCopy:GetScene()
	return self.__temp.scene
end

function clsCopy:SetType(type)
	self.__temp.type = type
end

function clsCopy:GetType()
	return self.__temp.type
end

function clsCopy:GetName()
	local info = GetCopyInfoById(self:GetId())
	return info.name
end

function clsCopy:SetCopyMonster(teamId, roleId)
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

--是否剧情对话已经发生过
function clsCopy:SetDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	self.__temp.dialogOver[data] = 1
end

function clsCopy:IsDialogOver(data)
	self.__temp.dialogOver = self.__temp.dialogOver or {}
	return self.__temp.dialogOver[data]
end


function clsCopy:IsPrizeTaken(roleId)
	if not self:IsCopyPassed() then
		return false
	end
	return self:GetClickTimes(roleId) >= self:GetClickMaxTimes(roleId)
end

function clsCopy:SetClickPos(roleId, pos)
	self.__temp.clicked[roleId] = self.__temp.clicked[roleId] or {}
	self.__temp.clicked[roleId][pos] = true
end

function clsCopy:IsPosClicked(roleId, pos)
	self.__temp.clicked[roleId] = self.__temp.clicked[roleId] or {}
	return self.__temp.clicked[roleId][pos]
end

function clsCopy:GetClickTimes(roleId)
	self.__temp.clicked[roleId] = self.__temp.clicked[roleId] or {}
	return table.size(self.__temp.clicked[roleId])
end

function clsCopy:GetClickMaxTimes(roleId)
	local vipLv = USER.GetUserNickInfo(roleId, "vipLv")
	if vipLv > 0 then
		return VIP_CLICK_TIMES
	end
	return NORMAL_CLICK_TIMES
end


function clsCopy:SendClickReward(roleId, pos)
	self:SetClickPos(roleId, pos)
	local quality = GenPrizeQuality("got")
	local info = BASECOPY.GetCopyInfoById(self:GetId())
	local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
	local ret = {pos = pos, goodsId = prize[1].id, num = prize[1].num}
	local moneyType = MONEY.GetGoldTypeByPicId(ret.goodsId)
	local user = USER.GetOnlineUser(roleId)
	if user then
		if moneyType then
			local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
			USERBASE.clsUser[moneyInfo.set](user, ret.num)
		else
			AddCopyItem(user, {ret}, self:GetName())
		end
	else
		if moneyType then
			OFFLINE.SetOfflineData(roleId, moneyType, ret.num)
		else
			OFFLINE.SetOfflineData(roleId, "item", USERBASE.PACKAGE, ret.goodsId, ret.num)
		end
	end
	self:SetPrizeGot(roleId, pos, ret, true)
	return ret
end



function clsCopy:SetPrizeGot(roleId, pos, item, flag)
	self.__temp.prizeList[roleId] = self.__temp.prizeList[roleId] or {}
	local buff = {goodsId = item.goodsId, num = item.num, pos = pos}
	if flag then
		buff.owner = roleId
	end
	table.insert(self.__temp.prizeList[roleId], buff)
end

function clsCopy:GenPrizeShown(roleId)
	local info = BASECOPY.GetCopyInfoById(self:GetId())
	for i = 1, MAX_PRIZE_NUM do
		if not self:IsPosClicked(roleId, i) then
			local prize = BASEITEM.GenRandomItemByNum(info.itemList, 1)
			self:SetPrizeGot(roleId, i, {goodsId = prize[1].id, num = prize[1].num})
			self:SetClickPos(roleId, i)
		end
	end
end

function clsCopy:GetPrizeList(roleId)
	self:GenPrizeShown(roleId)
	return self.__temp.prizeList[roleId]
end

function clsCopy:AutoChoose()
	self:RemoveChooseCallOut()
	local roleId = self:GetOwner()
	local list = {}
	for i = 1, MAX_PRIZE_NUM do
		table.insert(list, i)
	end
	local vipLv = USER.GetUserNickInfo(roleId, "vipLv")
	local times = self:GetClickMaxTimes(roleId) - self:GetClickTimes(roleId)
	for i = 1, times do
		local index = math.random(1, #list)
		self:SendClickReward(roleId, list[index])
		table.remove(list, index)
	end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_copy_prizeShow", self:GetPrizeList(roleId))
	end
--	if self:GetType() == "team" then			--发完奖励后清理组队副本
	--	local user = USER.GetOnlineUser(roleId)
	--	if user then
	--		user:SetCurCopy()
	--	end
	--	self:DestroyCopy()
--	end
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

--副本通关经验发放
function clsCopy:SetPassExpGot(state)
	self.__temp.passExpGot = state
end

function clsCopy:IsPassExpGot()
	return self.__temp.passExpGot
end

function clsCopy:InitGroundList()
	self.__temp.ground = {}
	local num = math.random(1, 3)
	local locateList = {{x = 700, y = 500}, {x = 1500, y = 500}, {x = 2050, y = 500}}
	local lv = USER.GetUserNickInfo(self:GetOwner(), "Lv")
	for i = 1, num do
		local pos = math.random(1, #locateList)
		self.__temp.ground[i] = {type = math.random(0, 1) == 1 and "gold" or "nimbus", locate = locateList[i], num = (lv + 20) * 2 * math.random(6, 11)}
		table.remove(locateList, pos)
	end
end

--"{[id] = {type = type, num = num}}"
function clsCopy:SetGroundItem(item)
	self.__temp.ground = item
end

function clsCopy:RemoveGroundItem(id)
	self.__temp.ground = self.__temp.ground or {}
	self.__temp.ground[id] = nil
end

function clsCopy:GetGroundItemById(id)
	self.__temp.ground = self.__temp.ground or {}
	return self.__temp.ground[id]
end

function clsCopy:GetGoundItemList()
	return self.__temp.ground
end

function __init__()
	local NORMAL = Import("autocode/normalCopyList.lua")
	AllCopyList["normal"] = NORMAL.GetTable()
	
	local TEAM = Import("autocode/teamCopyList.lua")
	AllCopyList["team"] = TEAM.GetTable()
	
	local HERO = Import("autocode/heroCopyList.lua")
	AllCopyList["hero"] = HERO.GetTable()
	
end
