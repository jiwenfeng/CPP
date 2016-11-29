-- mission基类

INVALID = 100		--不可接
UNSTARTED = 101		--未开始
DOING = 102			--进行中
FINISHED = 103		--任务已达成（未领取奖励）
ALLDONE = 104		--任务已完成（已领取奖励）

local AllMissionInfo = {}
--获取所有的系统任务配置信息
function GetAllMissionInfo()
	return AllMissionInfo
end
--获取单个任务的配置信息
function GetMissionInfoById(id)
	return AllMissionInfo[id]
end

function IsDailyMission(missionId)
	local info = GetMissionInfoById(missionId)
	return info and info.type == "daily"
end

function GetMissionListByType(type)
	local sData = {}
	for k, v in pairs(GetAllMissionInfo()) do
		if v.type == type then
			table.insert(sData, k)
		end
	end
	return sData
end

function GetFormatedPrize(user, prizeList)
	local sData = {}
	sData.gold = prizeList.gold
	sData.exp = prizeList.exp
	sData.nimbus = prizeList.nimbus
	sData.voucher = prizeList.voucher
	if prizeList.skill then
		local skillId = prizeList.skill[user:GetJob()]
		local skillInfo = BASESKILL.GetSkillInfo(skillId)
		if skillInfo then
			sData.skill = {skillId = skillId}
			sData.skill.name = skillInfo.name
			sData.skill.describe = skillInfo.describe
			sData.skill.type = skillInfo.type
			sData.skill.prizeType = "skill"
		end
	end
	if prizeList.jobPrize then
		sData.item = {}
		local id = prizeList.jobPrize[user:GetJob()]
		table.insert(sData.item, {goodsId = id, num = prizeList.jobPrize.num, prizeType = "item"})
	end
	if prizeList.item then
		sData.item = sData.item or {}
		for k, v in pairs(prizeList.item) do
			table.insert(sData.item, {goodsId = v.id, num = v.num, prizeType = "item"})
		end
	end
	if prizeList.fairyDev then
		local devObj = BASEITEM.NewItem(prizeList.fairyDev.id, prizeList.fairyDev.num, {lock = 1})
		if devObj then
			sData.item = sData.item or {}
			if prizeList.fairyDev.Lv then
				devObj:ResetLv(prizeList.fairyDev.Lv)
			end
			local buff = devObj:GetFullAttr()
			buff.prizeType = "fairyDev"
			table.insert(sData.item, buff)
		end
	end
	return sData
end

function GetFormatMissionDesc(missionId)
	local info = GetMissionInfoById(missionId)
	local param = {}
	for k, v in ipairs(info.target) do
		if v.type == "fight" then
			local monsterInfo = FIGHT.GetMonsterInfoById(tostring(v.id))
			table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", monsterInfo.name, 0, v.num))
		elseif v.type == "collect" then
			local itemInfo = BASEITEM.GetItemById(v.id)
			table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", itemInfo.name, 0, v.num))
		elseif v.type == "dialog" then
			local npcInfo = NPC.GetNpcInfoById(tostring(v.id))
			table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", npcInfo.name, 0, v.num))
		end
	end
	if info.needCopyId then
		local copyInfo = CLSCOPY.GetCopyInfoById(tostring(info.needCopyId))
		return string.format(info.shortDesc, string.format("<font color = \"#ffff00\"><u>%s</u></font>", copyInfo.name), unpack(param))
	else
		return string.format(info.shortDesc, unpack(param))
	end
end
--------------------------------------------------------------

clsmission = clsObject:Inherit()
function clsmission:__init__()
	Super(clsmission).__init__(self)
	self.__save = {}
	self.__save.state = nil		--任务状态
	self.__save.missionId = nil
	self.__save.class = nil
--	self.__save.type = nil			--
--	self.__save.label = nil			--
--	self.__save.owner = nil			--任务发布者
--	self.__save.name = nil  --任务名
--	self.__save.aim = nil	--任务目标
--	self.__save.content = nil --任务内容
--	self.__save.needLv = 0			--需求等级
	self.__save.detail = {}		--任务完成进度
--	self.__save.prize = {}		--{exp = 0, gold = 0, item = {}, preGold = nil, repute = nil}
	return true
end

function clsmission:InitSaveData(missionId, template, quality, prizeList)
	self:SetMissionId(missionId)
	local mis
	if not template then
		mis = GetMissionInfoById(missionId)
	else
		mis = GetMissionInfoById(template)
	end
	if not mis then
		return
	end
	self:SetState(DOING)
	self:SetTemplate(template)
	self:SetQuality(quality)
	self:InitMissionDetail()
	if prizeList then
		self:SetPrize(prizeList)
	end
	self:SetTimeStamp(os.time())
end

function clsmission:GetSave()
	return table.deepcopy(self.__save)
end

function clsmission:Restore(data)
	self.__save = data
end



--任务状态
function clsmission:GetState()
	return self.__save.state
end

function clsmission:SetState(data)
	self.__save.state = data
end

--任务id
function clsmission:GetMissionId()
	return self.__save.missionId
end

function clsmission:SetMissionId(data)
	self.__save.missionId = data
end
--任务模版
function clsmission:SetTemplate(template)
	self.__save.template = template
end

function clsmission:GetTemplate()
	return self.__save.template
end
--任务品质
function clsmission:SetQuality(quality)
	self.__save.quality = quality
end

function clsmission:GetQuality()
	return self.__save.quality
end

--任务类型
function clsmission:GetType()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.type
end
--任务模版
function clsmission:GetClass()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.class
end

function clsmission:GetLabel()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.label
end

function clsmission:GetName()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.name
end

function clsmission:GetAim()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.aim
end


function clsmission:GetContent()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.content
end

function clsmission:GetNeedLv()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.needLv
end

function clsmission:GetTarget()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.target
end

function clsmission:GetShortDesc()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.shortDesc
end

function clsmission:SetPrize(prizeList)
	assert(type(prizeList) == "table")
	self.__save.prize = prizeList
end

function clsmission:GetPrize()
	if self.__save.prize then
		return self.__save.prize
	end
	local info = GetMissionInfoById(self:GetMissionId())
	return info.prize
end

function clsmission:GetEndNpcInfo()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	if info.endNpcId then
		local sData = {}
		local npcInfo = NPC.GetNpcInfoById(info.endNpcId)
		sData.name = npcInfo.name
		sData.npcId = info.endNpcId
		return sData
	end
end

function clsmission:GetStartNpcInfo()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	if info.startNpcId then
		local sData = {}
		local npcInfo = NPC.GetNpcInfoById(info.startNpcId)
		sData.name = npcInfo.name
		sData.npcId = info.startNpcId
		return sData
	end
end

--获取所需通关副本id
function clsmission:GetNeedPassedCopyId()
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	return info.needCopyId
end

function clsmission:SetTimeStamp(time)
	self.__save.timestamp = time
end

function clsmission:GetTimeStamp()
	return self.__save.timestamp
end


--初始化任务进度
function clsmission:InitMissionDetail()
	local target = self:GetTarget()
	for k, v in ipairs(target) do
		table.insert(self.__save.detail, {type = v.type, id = v.id, needNum = v.num, curNum = 0})
	end
end

--获取任务完成进度
function clsmission:GetMissionDetail()
	return self.__save.detail
end
--修改任务完成进度
function clsmission:ModMissionDetail(user, id, num)			--子类重载
	if self:GetState() ~= DOING then
		return
	end	
	for k, v in ipairs(self.__save.detail or {}) do
		if v.id == id then
			v.curNum = math.min(v.needNum, v.curNum + num)
			self:SendMissionDetailUpdate(user)
		end
	end
end
--任务完成时清理完成进度
function clsmission:ClearMissionDetail()
	self.__save.detail = {}
end

--获取奖励物品数量
function clsmission:GetPrizeItemNum()
	local prizeList = self:GetPrize()
	local num = 0
	if prizeList.item then
		num = num + #prizeList.item
	end
	if prizeList.jobPrize then
		num = num + 1
	end
	return num
end

--发送任务奖励
function clsmission:SendReward(user)
	local msg_money = ""
	local prizeList = self:GetPrize()
	if prizeList.gold then
		user:SetGold(prizeList.gold)
		msg_money = msg_money .. string.format("钱币%d", prizeList.gold)
--		user:SendProtocol("c_show_tips", string.format("钱币 + %d", prizeList.gold))
	end
	if prizeList.exp then			--给出战队伍增加经验
		user:SetTeamExp(prizeList.exp, self:GetTemplate() or self:GetMissionId())
		msg_money = msg_money .. string.format("经验%d ", prizeList.exp)
--		user:SendProtocol("c_show_tips", string.format("经验 + %d", prizeList.exp))
	end
	if prizeList.nimbus then
		user:SetNimbus(prizeList.nimbus)
		msg_money = msg_money .. string.format("灵气%d ", prizeList.nimbus)
--		user:SendProtocol("c_show_tips", string.format("灵气 + %d", prizeList.nimbus))
	end
	if prizeList.voucher then
		user:SetVoucher(prizeList.voucher)
		msg_money = msg_money .. string.format("礼券%d", prizeList.voucher)
--		user:SendProtocol("c_show_tips", string.format("礼券 + %d", prizeList.voucher))
	end
	if #msg_money > 0 then
		user:SendProtocol("c_show_tips", string.format("您获得了%s。", msg_money))
	end
	if prizeList.skill then
		local skillId = prizeList.skill[user:GetJob()]
		local skillInfo = BASESKILL.GetSkillInfo(skillId)
		if skillInfo then
			user:SetSkill(skillInfo.type, skillId, 1)
			user:SendProtocol("c_show_tips", string.format("您习得新技能 %s。", skillInfo.name))
		end
	end
	local msg_item = ""
	if prizeList.jobPrize then
		local id = prizeList.jobPrize[user:GetJob()]
		local newItem = BASEITEM.NewItem(id, prizeList.jobPrize.num, {lock = 1, rate = 1})
		if newItem then
			user:AddItem(USERBASE.PACKAGE, newItem)
			msg_item = msg_item .. string.format("%s×%d ", newItem:GetName(), newItem:GetNum())
		--	user:SendProtocol("c_show_tips", string.format("您获得物品 %s", newItem:GetName()))
		end
	end
	if prizeList.item then
		for k, v in pairs(prizeList.item) do
			local newItem = BASEITEM.NewItem(v.id, v.num, {lock = 1})
			if newItem then
				if newItem:GetClass() == "equip" then
					newItem:SetGrade(1)
				end
				user:AddItem(USERBASE.PACKAGE, newItem)
				msg_item = msg_item .. string.format("%s×%d ", newItem:GetName(), newItem:GetNum())
	--			user:SendProtocol("c_show_tips", string.format("获得物品 %s", newItem:GetName()))
			end
		end
	end
	if #msg_item > 0 then
		user:SendProtocol("c_show_tips", string.format("您获得物品%s。", msg_item))
	end
	if prizeList.fairyDev and user:IsFuncOpen(SETTING.FUNC_FAIRYDEV) then
		local devObj = BASEITEM.NewItem(prizeList.fairyDev.id, prizeList.fairyDev.num, {lock = 1})
		if devObj then
			if prizeList.fairyDev.Lv then
				devObj:ResetLv(prizeList.fairyDev.Lv)
			end
			user:AddFairyDev(USERBASE.FAIRY_BAG, devObj, true)
			user:SendProtocol("c_show_tips", string.format("您获得仙器【%s】。", devObj:GetName()))
		end
	end
end

function clsmission:IsFinished(user)
	for _, v in ipairs(self:GetMissionDetail()) do
		if v.curNum < v.needNum then
			return false
		end
	end
--[[	if self:GetType() == "master" then
		local needCopyId = self:GetNeedPassedCopyId()
		if needCopyId then
			local type = CLSCOPY.GetCopyTypeById(needCopyId)
			if not user:IsCopyPassed(type, needCopyId) then
				return false
			end
		end
	end]]
	return true
end

function clsmission:SetMissionFinish(user)
	for _, v in ipairs(self:GetMissionDetail()) do
		v.curNum = v.needNum
	end
	self:CheckMissionOver(user)
end

--完成任务
function clsmission:Over(user)
	if self:GetState() ~= FINISHED then
		return
	end
	self:SendReward(user)
	user:FinishMission(self:GetMissionId())
end

function clsmission:CheckMissionOver(user)
	if self:GetState() ~= DOING then
		return
	end
	if not self:IsFinished(user) then
		return
	end
	self:SetState(FINISHED)
	if self:GetType() == "daily" then
		user:ModDailyMissionState(FINISHED)
	end
	local con = LOGIN.GetConnection(user:GetRoleId())
	if con and con.roleId == user:GetRoleId() then
		local sData = {}
		sData.missionId = self:GetMissionId()
	--	sData.label = self:GetLabel()
		sData.type = self:GetType()
		sData.state = self:GetState()
		con:SendProtocol("c_mission_finish", sData)
	end
end

function clsmission:GetFormatedDetail()
	local sData = {}
	for k, v in pairs(self:GetMissionDetail()) do
		local buff = {needNum = v.needNum, curNum = v.curNum}
		table.insert(sData, buff)
	end
	return sData
end

function clsmission:GetMissionShortDesc()
	local str = self:GetShortDesc()	
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	if self:GetState() == FINISHED and info.endNpcId then
		local npcInfo = NPC.GetNpcInfoById(tostring(info.endNpcId))
		return string.format("与<font color = \"#ffff00\"><u>%s</u></font>交谈", npcInfo.name)
	else
		local param = {}
		for k, v in ipairs(self:GetMissionDetail()) do
			if v.type == "fight" then
				local monsterInfo = FIGHT.GetMonsterInfoById(tostring(v.id))			
				table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", monsterInfo.name, v.curNum, v.needNum))
			elseif v.type == "collect" then
				local itemInfo = BASEITEM.GetItemById(v.id)
				table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", itemInfo.name, v.curNum, v.needNum))
			elseif v.type == "dialog" then
				local npcInfo = NPC.GetNpcInfoById(tostring(v.id))
				table.insert(param, string.format("<font color = \"#ffff00\"><u>%s</u></font>[%d/%d]", npcInfo.name, v.curNum, v.needNum))
			end
		end
		if info.needCopyId then
			--local copyInfo = CLSCOPY.GetCopyInfoById(tostring(info.needCopyId))
			local copyInfo = CLSCOPY.GetCopyInfoById(tostring(info.needCopyId))
			return string.format(str, string.format("<font color = \"#ffff00\"><u>%s</u></font>", copyInfo.name), unpack(param))
		else
			return string.format(str, unpack(param))
		end
	end
end

function clsmission:GetMissionData(flag)
	local sData = {}
	sData.missionId = self:GetMissionId()
	sData.name = self:GetName()
	sData.type = self:GetType()
	sData.state =self:GetState()
	if flag then
		sData.content = self:GetMissionShortDesc()
	end
	if self:GetType() == "master" then
		local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
		local chapterInfo = MISSION.GetChapterInfoById(info.chapter)
		sData.chapterId = info.chapter
		sData.chapterName = chapterInfo.name
	end
	return sData
end

function clsmission:GetDetailData(user)
	local sData = {}
	sData.missionId = self:GetMissionId()
	sData.aim = self:GetAim()
	sData.content = self:GetMissionShortDesc()
--	sData.content = self:GetContent()
--	sData.detail = self:GetFormatedDetail()
	sData.prize = GetFormatedPrize(user, self:GetPrize())
	local info = GetMissionInfoById(self:GetTemplate() or self:GetMissionId())
	if info.endNpcId then
		local npcInfo = NPC.GetNpcInfoById(info.endNpcId)
		sData.endNpc = npcInfo.name
	end
	if info.startNpcId then
		local npcInfo = NPC.GetNpcInfoById(info.startNpcId)
		sData.startNpc = npcInfo.name
	end
	return sData
end

--更新任务详情
function clsmission:SendMissionDetailUpdate(user)
	local sData = {}
	sData.missionId = self:GetMissionId()
	sData.content = self:GetMissionShortDesc()
--	sData.detail = self:GetFormatedDetail()
	local roleId = user:GetRoleId()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_mission_update", sData)
	end
end




function __init__()

	local function LoadTable(tb)
		for k, v in pairs(tb) do
			AllMissionInfo[k] = v
		end
	end
	
	--加载任务列表
	local MASTER = Import("autocode/missionMaster.lua")
	LoadTable(MASTER.GetTable())
	
	local BRANCH = Import("autocode/missionBranch.lua")
	LoadTable(BRANCH.GetTable())
	
	local DAILY = Import("autocode/missionDaily.lua")
	LoadTable(DAILY.GetTable())
	
	local STATUS = Import("autocode/missionStatus.lua")
	LoadTable(STATUS.GetTable())
	
	local HERO = Import("autocode/missionHero.lua")
	LoadTable(HERO.GetTable())
	
	local MISSKIND = Import("autocode/missionKind.lua")
	LoadTable(MISSKIND.GetTable())
	
end
