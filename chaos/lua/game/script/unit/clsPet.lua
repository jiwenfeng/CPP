--本命精灵类

local EGG_PET_ID = 200011		--初始id
local PET_TRANSFORM_LV = 7			--变形所需要的等级
local ADULT_NEED_LV = 30			--长大所需等级
local MAX_PET_LV = 60

local FORM_EGG = 0
local FORM_YOUNG = 1
local FORM_ADULT = 2

local PetExpList = {}			--本命精灵经验表
local AllPetInfoList = {}		--本命精灵列表
local PetTalentList = {}		--本命精灵天赋技能表
local PetAptLvList = {}			--本命精灵品级对应关系表
local ExtraItemList = {}		--商城意外之喜物品列表


local REFRESH_FREQUENCY	= 4 * 3600 	--刷新频率
local MAX_REFRESH_TIMES = 10		--最多刷新次数
local EXTRA_ITEM_RATE = 0.02		--挖宝时意外之喜概率

local HeadingList = {
	[1] = {"主人","表哥","哥哥"},
	[0] = {"主人","姐姐","亲亲"},
}
local FoodEffectRate = {
	[101097] = {new = {rate = 0.8, num = 0.1}, add = {rate = 0.8, num = 0.1}},
	[101098] = {new = {rate = 0.5, num = 0.05}, add = {rate = 0.5, num = 0.05}},
}

function GetPetExpByLv(Lv)
	return PetExpList[Lv]
end

--本命精灵列表
function GetAllPetInfoList()
	return AllPetInfoList
end

function GetPetInfoById(id)
	return AllPetInfoList[id]
end

--本命精灵天赋
function GetPetTalentList()
	return PetTalentList
end

function GetPetTalentById(id)
	return PetTalentList[tonumber(id)]
end

function GetPetTalentArray()
	local buff = {}
	for k in pairs(GetPetTalentList()) do
		table.insert(buff, k)
	end
	return buff
end

--本命精灵资质信息
function GetPetAptInfoByLv(aptLv)
	return PetAptLvList[aptLv]
end

function GetPetAptLv(Lv)
	local aptLv = 1
	for k, v in ipairs(PetAptLvList) do
		if Lv < v.needLv then
			break
		end
		aptLv = k
	end
	return aptLv
end

function GetHeadingBySex(sex)
	return HeadingList[sex]
end

function CalRandomQuality()
	local ran = math.random()
	local rate = 0
	for k, v in ipairs(QualityRate) do
		rate = rate + v.rate
		if rate >= ran then
			return v.quality
		end
	end
end

function GetExtraItemRate()
	return EXTRA_ITEM_RATE
end
------------------------------------------------------
clsPet = clsObject:Inherit()

function clsPet:__init__()
	Super(clsPet).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end

function clsPet:initsavedata()
	self.__save.Lv = 1
	self.__save.exp = 0
	self.__save.itemList = {}
	self.__save.talent = {}
	self.__save.freshTime = nil
	self.__save.refresh = {times = nil, timestamp = nil}
	self.__save.luck = {}
end

function clsPet:inittempdata()
	self.__temp.callout = {}
end


function clsPet:InitPet(roleId, id, owner)
	self:SetRoleId(roleId)
	self:SetId(id or EGG_PET_ID)
	self:SetOwner(owner)
	
	self:SetForm(FORM_EGG)
	
	local info = GetPetInfoById(self:GetId())
	local formInfo = info.modelList[self:GetForm()]
	self:SetModelId(formInfo.modelId)
	self:SetHeadId(formInfo.headId)
end


function clsPet:SetOwner(owner)
	self.__save.owner = owner
end

function clsPet:GetOwner()
	return self.__save.owner
end

function clsPet:SetId(id)
	self.__save.id = id
end

function clsPet:GetId()
	return self.__save.id
end

function clsPet:SetRoleId(roleId)
	self.__save.roleId = roleId
end

function clsPet:GetRoleId()
	return self.__save.roleId
end

--模型
function clsPet:GetModelId()
	return self.__save.modelId
end

function clsPet:SetModelId(modelId)
	self.__save.modelId = modelId
end
--头像
function clsPet:GetHeadId()
	return self.__save.headId
end

function clsPet:SetHeadId(headId)
	self.__save.headId = headId
end

--本命精灵状态(0 蛋形 1 幼年 2 成年)
function clsPet:SetForm(data)
	self.__save.form = data
end

function clsPet:GetForm()
	return self.__save.form
end


function clsPet:SendFormUpdate()
	local sData = {}
	sData.id = self:GetId()
	sData.modelId = self:GetModelId()
	sData.headId = self:GetHeadId()
--	sData.form = self:GetForm()
	local owner = assert(USER.GetOnlineUser(self:GetOwner()))
	owner:Broad2ExNeighbor("pet", nil, nil)
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_pet_formUpdate", sData)
	end
end

function clsPet:Transform(id)
	local info = GetPetInfoById(id)
	if not info then
		return
	end
	if self:GetForm() == FORM_EGG then
		self:SetForm(FORM_YOUNG)
	end
	self:SetId(id)
	local formInfo = info.modelList[self:GetForm()]
	self:SetModelId(formInfo.modelId)
	self:SetHeadId(formInfo.headId)
	self:SetSkillLv(1)
	self:SendFormUpdate()
	self:ClearRefreshCallOut()
	self:StartRefreshCallOut()
end


function clsPet:GetCallOut()
	return self.__temp.callout
end

function clsPet:GetName()
	local info = GetPetInfoById(self:GetId())
	return info.name
end

function clsPet:GetSkillId()
	local info = GetPetInfoById(self:GetId())
	if not info then
		return
	end
	return info.skillId
end
--必杀技等级
function clsPet:SetSkillLv(lv)
	assert(tonumber(lv))
	self.__save.skillLv = lv
	if self.__save.skillLv < 1 then
		self.__save.skillLv = 1
	end
	local skillInfo = BASESKILL.GetSkillInfo(self:GetSkillId())
	if skillInfo then
		if self.__save.skillLv > #skillInfo.list then
			self.__save.skillLv = #skillInfo.list
		end
	end
end


function clsPet:GetSkillLv()
	return self.__save.skillLv or 1
end


--本命精灵经验（直接使用主角经验）
function clsPet:SetExp(data)

end

function clsPet:GetExp()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	return user:GetExp(), user:GetUpgradeExp()
end

function clsPet:Upgrade()
end
--本命精灵等级（直接使用主角等级）
function clsPet:SetLv(data)	
end

function clsPet:GetLv()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local userLv = user:GetLv()
	return math.min(userLv, MAX_PET_LV)
end

function clsPet:GetAptLv()
	return GetPetAptLv(self:GetLv())
end


--天赋
function clsPet:InitTalent()
	local talentArray = GetPetTalentArray()
	local id = talentArray[math.random(1, #talentArray)]
	local info = GetPetTalentById(id)
	self:SetTalentInfo(id, info.init)
end

function clsPet:GetTalentList()
	return self.__save.talent
end

function clsPet:GetTalentInfo(id)
	id = tostring(id)
	return self.__save.talent[id]
end

function clsPet:GetTalentAdd(id)
	local info = self.__save.talent[id]
	if not info then
		return 0
	end
	return info.num
end

function clsPet:SetTalentInfo(id, num)
	local talentInfo = GetPetTalentById(id)
	id = tostring(id)
	self.__save.talent[id] = self.__save.talent[id] or {}
	local info = self.__save.talent[id]
	if not info.num then
		info.num = num
		info.timestamp = os.time()
	else
		info.num = info.num + num
	end
	if info.num <= 0 then
		self.__save.talent[id] = nil
	elseif info.num > talentInfo.uplimit[self:GetAptLv()] then
		info.num = talentInfo.uplimit[self:GetAptLv()]
	end
	local roleId = self:GetOwner()
	local user = assert(USER.GetOnlineUser(roleId))
	user:FreshTeamTotalAttr()
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		PROTOCOL.s_pet_attrDesc(con, "get")
	end
end


function clsPet:GetAttrDescList()
	local sData = {}
	local temp = {}
	for k, v in pairs(self:GetTalentList()) do
		local info = GetPetTalentById(k)
		local str = string.format(info.descrip, v.num < 1 and v.num * 100 or v.num)
		table.insert(sData, str)
		temp[str] = v.timestamp
	end
	table.sort(sData, function(a, b) return temp[a] < temp[b] end)
	return sData
end

--幸运值
function clsPet:GetLuckyNum()
	local info = self.__save.luck or {}
	if not info.timestamp or CalDiffDays(info.timestamp, os.time()) ~= 0 then
		info.number = math.random(1, 10)
		info.timestamp = os.time()
	end
	return info.number
end

--设置称谓
function clsPet:GetHeading()
	return self.__save.appellation or 1
end

function clsPet:SetHeading(data)
	self.__save.appellation = data
end

function clsPet:GetHeadingName()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local headingList = GetHeadingBySex(user:GetSex())
	return headingList[self:GetHeading()]
end



function clsPet:SetExchangeList(list)
	self.__save.itemList = list
end

--当前兑换列表
function clsPet:GetExchangeList()
	return self.__save.itemList
end

function clsPet:ModExchgItemById(id, num)
	for k, v in ipairs(self:GetExchangeList()) do
		if v.id == id then
			v.num = v.num + num
			if v.num <= 0 then
				self:RemoveItemByIndex(k)
			end
			break
		end
	end
end

function clsPet:ModExchgItemByIndex(index, num)
	local info = self:GetExchgItemByIndex(index)
	if not info then
		return
	end
	info.num = info.num + num
	if info.num <= 0 then
		self:RemoveItemByIndex(index)
	end
end

function clsPet:GetExchgItemByIndex(index)
	for k, v in ipairs(self.__save.itemList) do
		if v.index == index then
			return v
		end
	end
end


function clsPet:RemoveItemByIndex(index)
	for k, v in ipairs(self.__save.itemList) do
		if v.index == index then
			v = nil
			return
		end
	end
end


--刷新时间
function clsPet:SetAutoRefreshTimestamp(time)
	self.__save.refresh = self.__save.refresh or {}
	self.__save.refresh.autoFresh = time
end

--刷新时间
function clsPet:GetAutoRefreshTimestamp()
	self.__save.refresh = self.__save.refresh or {}
	return self.__save.refresh.autoFresh or os.time()
end

--元宝刷新次数
function clsPet:SetRefreshTimes(data)
	local info = self.__save.refresh or {}
	if not info.timestamp or CalDiffDays(info.timestamp, os.time()) ~= 0 then
		info.times = data
	else
		info.times = (info.times or 0) + data
	end
	if info.times > MAX_REFRESH_TIMES then
		info.times = MAX_REFRESH_TIMES
	elseif info.times < 0 then
		info.times = 0
	end
	info.timestamp = os.time()
end

function clsPet:GetRefreshMaxTimes()
	return MAX_REFRESH_TIMES
end

function clsPet:GetRefreshTimes()
	local info = self.__save.refresh or {}
	if not info.timestamp or CalDiffDays(info.timestamp, os.time()) ~= 0 then
		info.times = 0
	end
	return info.times or 0				--之前错误导致次数数据有误。故返回0
end


--刷新物品定时器
function clsPet:StartRefreshCallOut()
	if self:GetAutoRefreshTimestamp() <= os.time() then
		PET.GenExchangeItemList(self)
	end
	local timelong = self:GetAutoRefreshTimestamp() - os.time()
	self.__temp.freshCallId = CALLOUT.ObjCallOut(self:GetCallOut(), PET.GenExchangeItemList, timelong, self)
end

function clsPet:ClearRefreshCallOut()
	if self.__temp.freshCallId then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.freshCallId)
	end
end



function clsPet:LoginStartCallOut()
	if self.__save.id ~= EGG_PET_ID then
		self:StartRefreshCallOut()
	end
end

function clsPet:OfflineRemoveCallout()
	self:ClearRefreshCallOut()
end

function clsPet:GetSaveData()
	return table.deepcopy(self.__save)
end

function clsPet:Restore(data)
	self.__save = data
end

local PetPromptList = {
	["pillageWin"] = 1, ["pillageLose"] = 2
}

--本命精灵日常提醒
function clsPet:SendPetPrompt(type, msg)
	local con = LOGIN.GetConnection(self:GetOwner())
	if con and con.roleId == self:GetOwner() then
		con:SendProtocol("c_pet_prompt", {type = PetPromptList[type], msg = msg})
	end
end

--离线时设置本命精灵提醒
function clsPet:SetPetPrompt(type, msg)
	if type then
		self.__temp.prompt = {type = type, msg = msg}
	else
		self.__temp.prompt = nil
	end
end

function clsPet:GetPetPrompt()
	return self.__temp.prompt
end


--意外之喜
function clsPet:GetRandomItem()
	if self:GetForm() ~= FORM_EGG then
		local aptInfo = GetPetAptInfoByLv(self:GetAptLv())
	--	if math.random() <= aptInfo.extra then
		if math.random() <= 0.5 then
			return ExtraItemList[math.random(1, #ExtraItemList)]
		end
	end
end

--打折概率
function clsPet:GetPetDiscount()
	if self:GetForm() ~= FORM_EGG then
		local aptInfo = GetPetAptInfoByLv(self:GetAptLv())
	--	if math.random() <= aptInfo.discount then
		if math.random() <= 0.5 then
			return math.random(88, 95) / 100
		end
	end
end

--舍命守护概率
function clsPet:GetGuardRate()
	if self:GetForm() ~= FORM_EGG then
		local aptInfo = GetPetAptInfoByLv(self:GetAptLv())
		return aptInfo.guard
	end
	return 0
end

--发送几率完成日常任务请求
function clsPet:SendDailyMissionAsk(missionId)
	if self:GetForm() == FORM_EGG then
		return
	end
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local misObj = user:GetMissionById(missionId)
	if not misObj or misObj:GetType() ~= "daily" then
		return
	end
	local info = GetPetAptInfoByLv(self:GetAptLv())
--	if math.random() <= info.mission then
	if math.random() <= 1 then
		local function func()
			local msg = "您的本命精灵一时兴起，想要帮你完成任务。是否继续？"
			user:SendProtocol("c_pet_dailyMissAsk", msg)
			local chosen = coroutine.yield(co)
			if chosen == 1 then
				misObj:SetMissionFinish(user)
			else
				self:SetDailyMissCort()
			end
		end
		local co = coroutine.create(func)
		self:SetDailyMissCort(co)
		return coroutine.resume(co)
	end
end

function clsPet:ProDailyMissionAsk(chosen)
	local co = self:GetDailyMissCort()
	if co and coroutine.status(co) == "suspended" then
		coroutine.resume(co, chosen)
	end
end


function clsPet:SetDailyMissCort(co)
	self.__temp.dailyMissCort = co
end

function clsPet:GetDailyMissCort()
	return self.__temp.dailyMissCort
end




function clsPet:IsTalentCanPromote(id)
	local talentInfo = GetPetTalentById(id)
	id = tostring(id)
	self.__save.talent[id] = self.__save.talent[id] or {}
	local info = self.__save.talent[id]
	return (info.num or 0) < talentInfo.uplimit[self:GetAptLv()]
end

function clsPet:GenNewTalent()
	local list = {}
	for k in pairs(GetPetTalentList()) do
		if not self:GetTalentInfo(k) then
			table.insert(list, k)
		end
	end
	if #list <= 0 then
		return
	end
	return list[math.random(1, #list)]
end

function clsPet:Feed(foodId)
	local info = BASEITEM.GetItemById(foodId)
	local aptLv = self:GetAptLv()
	local aptInfo = GetPetAptInfoByLv(aptLv)
	local foodEffect = FoodEffectRate[foodId]
	local talentList = self:GetTalentList()
	local flag
	local buff = {}
	for k, v in pairs(talentList) do
		local talentInfo = GetPetTalentById(k)
		if v.num < talentInfo.uplimit[aptLv] then
			flag = true
		end
		table.insert(buff, k)
	end
	local talentId
	if #buff < aptInfo.talentNum then
		if math.random() > foodEffect.new.rate then
			return true, string.format("%s拍了拍肚子，满意地打了一个饱嗝。", self:GetName())
		end
		local talentId = self:GenNewTalent()
		local talentInfo = GetPetTalentById(talentId)
		local num = talentInfo.max * foodEffect.new.num
		self:SetTalentInfo(talentId, num)
		return true, string.format("%s领悟了新的天赋：" .. talentInfo.descrip, self:GetName(), num < 1 and num * 100 or num)
	elseif flag then
		if math.random() > foodEffect.add.rate then
			return true, string.format("%s拍了拍肚子，满意地打了一个饱嗝。", self:GetName())
		end
		local talentId = buff[math.random(1, #buff)]
		if not self:IsTalentCanPromote(talentId) then
			return true, string.format("%s拍了拍肚子，满意地打了一个饱嗝。", self:GetName())
		end
		local talentInfo = GetPetTalentById(talentId)
		local num = talentInfo.max * foodEffect.add.num
		self:SetTalentInfo(talentId, num)
		return true, string.format("%s服用仙果成功，天赋获得提升：" .. talentInfo.descrip, self:GetName(), num < 1 and num * 100 or num)
	end
	return false, "本命精灵当前所有天赋都已达到瓶颈。"
end


function clsPet:GetPetDetailInfo()
	local user = assert(USER.GetOnlineUser(self:GetOwner()))
	local sData = {}
	sData.id = self:GetId()
	sData.Lv = self:GetLv()
	sData.name = self:GetName()
	sData.aptLv = self:GetAptLv()
	sData.exp = self:GetExp()
	sData.expMax = GetPetExpByLv(self:GetLv())
	local sex = user:GetSex()
	sData.headingList = GetHeadingBySex(sex)
	sData.chosen = self:GetHeading()
	sData.luckyNum = self:GetLuckyNum()
	sData.skillId = self:GetSkillId()
	local skillLv = self:GetSkillLv()
	sData.skillLv = skillLv
	local skillInfo = BASESKILL.GetSkillInfo(sData.skillId)
	if skillInfo and skillLv > 0 then
		sData.skillName = skillInfo.name
		sData.skillDesc = skillInfo.describe
		sData.explain = skillInfo.list[skillLv].explain
		if skillInfo.list[skillLv + 1] then
			sData.nextExplain = skillInfo.list[skillLv + 1].explain
		end
	end
	sData.form = self:GetForm()
	sData.modelId = self:GetModelId()
	sData.headId = self:GetHeadId(sData.form)
	return sData
end

function clsPet:Destory()
	CALLOUT.RemoveAll(self:GetCallOut())
	self:Destory()
end


function clsPet:CheckPetTransForm(id)
	if id == EGG_PET_ID then
		return false, "不能把本名精灵变换为初始形态。"
	end
	if self:GetForm() ~= 0 then
		return false, "您的本命精灵已经变换过形态了。"
	end
	if self:GetId() == id then
		return false, "本命精灵已经是该形态了。"
	end
	if self:GetLv() < PET_TRANSFORM_LV then
		return false, string.format("本命精灵等级未达到%d级，不能转换形态。", PET_TRANSFORM_LV)
	end
	return true
end

--等待玩家选择形态
function clsPet:IsWaitForChoice()
	if self:GetForm() ~= FORM_EGG then
		return false
	end
	local Lv = self:GetLv()
	if Lv < PET_TRANSFORM_LV then
		return false
	end
	return true
end

function clsPet:CheckPetForm()
	local Lv = self:GetLv()
	if self:IsWaitForChoice() then
		return self:SendPetFormSelect()
	elseif Lv >= ADULT_NEED_LV and self:GetForm() == FORM_YOUNG then
		self:SetForm(FORM_ADULT)
		self:Transform(self:GetId())
	end
end


function clsPet:SendPetFormSelect()
	local user = USER.GetOnlineUser(self:GetOwner())
	local sData = {}
	for k, v in pairs(GetAllPetInfoList()) do
		if k ~= EGG_PET_ID then
			local buf = {}
			buf.id = k
			buf.describe = v.describe
			buf.modelId = v.modelList[1]
			table.insert(sData, buf)
		end
	end
	user:SendProtocol("c_pet_select", sData)
end


-----------------------------------


function __init__()
	
	local EXPLIST = Import("autocode/petExp.lua")
	PetExpList = EXPLIST.GetTable()
	
	local PETLIST = Import("autocode/petList.lua")
	AllPetInfoList = PETLIST.GetTable()
	
	local TALENTLIST = Import("autocode/petTalentList.lua")
	PetTalentList = TALENTLIST.GetTable()
	
	local APTLVINFO = Import("autocode/petAptLv.lua")
	PetAptLvList = APTLVINFO.GetTable()
	
	local PETEXTRAITEM = Import("autocode/petExtraItemList.lua")
	ExtraItemList = PETEXTRAITEM.GetTable()
	
end
