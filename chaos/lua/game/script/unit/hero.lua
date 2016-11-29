--武将系统
local FRESH_HERO_NUM = 5			--每次刷新的最大武将数量
local FRESH_MAX_FAMOUS_NUM = 3		--每次最多刷出的英魂数量
local IGNORE_HERO_INGOT = 100
local NORMAL_FRESH_GOLD = 50000		--普通招聘所需钱币
local ADVANCE_FRESH_INGOT = 20		--高级招聘
local FreshLockFamous = {		--刷新时锁定英魂
	[1] = {rate = 0.4, needIngot = 50}, [2] = {rate = 0.6, needIngot = 200}, [3] = {rate = 0.8, needIngot = 500},
}

local NORMAL_FRESH_COLD_DOWN = 4 * 60 * 60
local MAX_UNLIMIT_HERO_APT = 3		--不受限制的最大武将资质
local HIGH_APT_HERO_NUM = 2			--高品质武将每次最多生成2个
local FIRE_HERO_COST = 50000		--解雇武将所需钱币
local HERO_UN_SHUFFLE = 1		--武将牌未翻面
local HERO_SHUFFLED = 0			--武将牌已翻面
local HERO_CHOSEN = 2			--武将牌已选中
local HERO_UN_CHOSEN = 3		--武将未被选中
local AllHeroList = {}
local HERO_ONLINE_TIME = 60 * 2
local AllHeroInLoading = {}
local EMP_BUY_VIP_LV = 5
local EmployeeDirect = {
	[4] = 888,
	[5] = 2888,
	[6] = 28888,
}

function SetOnlineHero(heroId, hero)
	AllHeroList[heroId] = hero
	if hero then
		hero:SetOnlineTimestamp(os.time())
	end
end

function GetOnlineHero(heroId)
	return AllHeroList[heroId]
end

function GetAllOnlineHero()
	return AllHeroList
end

function GetHeroInLoading(heroId)
	return AllHeroInLoading[heroId]
end

function SetHeroInLoading(heroId, call_back)
	AllHeroInLoading[heroId] = AllHeroInLoading[heroId] or {}
	if call_back then
		table.insert(AllHeroInLoading[heroId], call_back)
	end
end

function RemoveHeroInLoading(heorId)
	if not heroId then
		return
	end
	AllHeroInLoading[heroId] = nil
end

function ReadOfflineHeroData(heroId, call_back)
	if type(call_back) ~= "function" and type(call_back) ~= "nil" then
		return
	end
	if GetHeroInLoading(heroId) then		--判断是否已在恢复当中
		return SetHeroInLoading(heroId, call_back)
	end
	SetHeroInLoading(heroId, call_back)	--设置处于恢复当中
	local function read_back(Data, msg)
		Data = UnSerialize(Data)
		if not Data or not Data[1] then
			DATA.log(string.format("没有找到指定的数据信息：%s。", heroId), "hero.log")
			for _, func in ipairs(GetHeroInLoading(heroId)) do
				func(false, string.format("没有找到指定的数据信息：%s。", heroId))
			end
			return
		end
		local heroObj = CLSHERO.clsHero:New()
		heroObj:Restore(Data[1])
		SetOnlineHero(heroId, heroObj)
		for _, func in ipairs(GetHeroInLoading(heroId)) do
			func(true)
		end
		RemoveHeroInLoading(heroId)		--恢复完毕
	end
	USERBASE.ReadHeroFromDB("_id", heroId, read_back)
end


function CheckOnlineHeroList()
	for k, v in pairs(GetAllOnlineHero()) do
		local timestamp = v:GetOnlineTimestamp()
		if os.time() - timestamp >= HERO_ONLINE_TIME then
			SetOnlineHero(k)
		end
	end
end

--英魂生成概率
local FamousHeroRate = {
	[1] = 0.2, [2] = 0.4, [3] = 0.6,
}
--资质生成概率表
local AptRateList = {			
	[1] = {[1] = 40, [2] = 30, [3] = 15, [4] = 10, [5] = 5, [6] = 0},	--普通招募		
	[2] = {[1] = 10, [2] = 20, [3] = 30, [4] = 30, [5] = 9, [6] = 1},	--高级招募
--	[3] = {[1] = 0.05, [2] = 0.25, [3] = 0.4, [4] = 0.20, [5] = 0.1, [6] = 0},
}

local ClickRateList = {
	[1] = {[1] = 50, [2] = 40, [3] = 6, [4] = 3, [5] = 1, [6] = 0},		--普通招募
	[2] = {[1] = 10, [2] = 20, [3] = 30, [4] = 30, [5] = 9, [6] = 1},	--高级招募
--	[3] = {[1] = 10, [2] = 15, [3] = 40, [4] = 25, [5] = 10, [6] = 0},
}

local HeroMaxAptList = {["famous"] = 6, ["normal"] = 5}			--生成战魂的最大品质

--生成资质
function GenHeroApt(type, heroType, flag)
	local list = AptRateList[type]
	if not list then
		return
	end
	local maxApt = HeroMaxAptList[heroType]
	if flag then
		maxApt = MAX_UNLIMIT_HERO_APT
	end
	local sum = 0
	for i = 1, maxApt do
		sum = sum + list[i]
	end
	local ran = math.random(0, sum)
	local rate = 0
	for k, v in ipairs(list) do
		rate = rate + v
		if rate >= ran then		--可生成的最高武将品质为 MAX_UNLIMIT_HERO_APT
			return k
		end
	end
end


--生成招募列表
function FreshHeroList(user, type, copyId, rate)
	local typeList = {}
	local famousCount, targetNum = 0, 0		--刷出英魂数量 / 刷出指定副本英魂数量
	for i = 1, FRESH_HERO_NUM do
		local famousRate = famousCount >= FRESH_MAX_FAMOUS_NUM and 0 or FamousHeroRate[type]
		if math.random() <= famousRate then
			local targetCopy
			if copyId and math.random() <= rate then
				targetCopy = copyId
				targetNum = targetNum + 1
			end
			table.insert(typeList, {heroType = "famous", copyId = targetCopy})
			famousCount = famousCount + 1
		else
			table.insert(typeList, {heroType = "normal"})
		end
	end
	if copyId and targetNum <= 0 then		--确保至少刷出1个指定副本的英魂
		if famousCount <= 0 then
			local index = math.random(1, #typeList)
			typeList[index].copyId = copyId
		else
			for k, v in pairs(typeList) do
				if v.heroType == "famous" then
					v.copyId = copyId
				end
			end
		end
	end
	local sData = {}
	local exception = {}
	local count = 0
	for i = 1, FRESH_HERO_NUM do
		local index = math.random(1, #typeList)
		local chosen = typeList[index]
		local aptLv = GenHeroApt(type, chosen.heroType, count >= HIGH_APT_HERO_NUM)
		if aptLv > MAX_UNLIMIT_HERO_APT then
			count = count + 1
		end
		local buff = {index = i, state = HERO_UN_SHUFFLE}
		local template
		if chosen.heroType == "famous" then
			template = FAMOUSHERO.GenFamousHeroByAptLv(user, aptLv, chosen.copyId, exception)
		end
		if template then
			buff.template = template
			exception[template] = true
			local info = FAMOUSHERO.GetFamousHeroInfoById(template)
			buff.aptLv = CLSHERO.GetAptLvByAptitude(info.aptitude)
			buff.skillId = SKILL.GenRandomSkill(info.job, 1)
		else
			local sex = math.random(0, 1)
			buff.name = NAME.GenName(sex)
			buff.aptLv = aptLv
			local jobList = {}
			for k, v in pairs(JOB.GetJobTypeList()) do
				table.insert(jobList, k)
			end
			buff.job = jobList[math.random(1, #jobList)]
			local modelList = JOB.GetModelList(buff.job, sex)
			buff.modelId = modelList[math.random(1, #modelList)]
			buff.headId = JOB.GenHeroHeadIdByModel(buff.modelId)
		--	local atkType = JOB.GetJobAttackType(buff.job)
			buff.growing = CLSHERO.GenHeroGrowingRate(buff.job)
			for k, v in pairs(CLSHERO.GenHeroAttrList(buff.growing, aptLv)) do
				buff[k] = v
			end
			buff.skillId = SKILL.GenRandomSkill(buff.job, 1)
		end
		table.insert(sData, buff)
		table.remove(typeList, index)
	end
	return sData
end

--定时免费刷新
function FreeFreshEmplList(user)
	user:ClearFreshEmplListCallOut()
	local list = FreshHeroList(user, 1)
	user:SetHeroFreshList({list = list, type = 1, period = "fresh"})
	user:SetHeroFreshTimestamp(os.time() + NORMAL_FRESH_COLD_DOWN)
	user:StartFreshEmplListCallOut()
end

--普通招募
function ProNormalFresh(user)
	if user:GetGold() < NORMAL_FRESH_GOLD then
		return false, "您的钱币不足。"
	end
	user:SetGold(-NORMAL_FRESH_GOLD)
	local list = FreshHeroList(user, 1)
	user:SetHeroFreshList({list = list, type = 1, period = "fresh"})
	return true
end

--高级招募
function ProAdvancedFresh(user, copyId, index)
	local needIngot, rate = ADVANCE_FRESH_INGOT, 0
	if copyId then
		if not FAMOUSHERO.GetFamousCopyById(copyId) then
			return false, "没有指定的英魂副本。"
		end
		index = tonumber(index)
		local info = FreshLockFamous[index]
		if not info then
			return false, "请选择正确的锁定类型。"
		end
		needIngot = needIngot + info.needIngot
		rate = info.rate
	end
	if user:GetTotalIngot() < needIngot then
		return false, "您的元宝不足。"
	end
	user:SetIngot(-needIngot)
	local list = FreshHeroList(user, 2, copyId, rate)
	user:SetHeroFreshList({list = list, type = 2, period = "fresh"})
	return true
end

--格式化输出招募列表
function GetFormatedEmplList(user)
	local sData = {}
	local freshList = user:GetHeroFreshList()
	for k, v in pairs(freshList.list or {}) do
		local buff = {}
		buff.index = v.index
		buff.state = v.state
		buff.pos = v.pos
		if v.state == HERO_CHOSEN or v.state == HERO_UN_SHUFFLE then
			if v.template then
				buff = FAMOUSHERO.GetFamousHeroShownData(v.template)
				buff.index = v.index
				buff.state = v.state
				buff.pos = v.pos
			else
				if v.state ~= HERO_SHUFFLED then
					buff.modelId = v.modelId
					buff.name = v.name
					buff.aptLv = v.aptLv
					buff.job = v.job
					buff.Lv = 1
					
					buff.hpMax = v.hpMax
					buff.hp = v.hpMax
					buff.physcAtkMin, buff.physcAtkMax = v.physcAtk, v.physcAtk
					buff.physcDef = v.physcDef
					buff.magicAtkMin, buff.magicAtkMax = v.magicAtk, v.magicAtk
					buff.magicDef = v.magicDef
					buff.speed = math.ceil(v.speed)
				end
				if v.skillId then
					buff.skillList = buff.skillList or {}
					table.insert(buff.skillList, {type = 1, skillId = v.skillId, Lv = 1})
				end
				buff.force = CHAR.CalFightForce(buff)
			end
		end
		table.insert(sData, buff)
	end
	return sData
end
--忽略指定武将
function RmHeroFromFreshList(user, index)
	local freshList = user:GetHeroFreshList()
	if not freshList.type then
		return
	end
	for k, v in pairs(freshList.list) do
		if v.index == index then
			table.remove(freshList.list, k)
			break
		end
	end
end

function ShuffleHeroEmplList(user)
	local freshList = user:GetHeroFreshList()
	if not freshList.list then
		return
	end
	for k, v in pairs(freshList.list) do
		v.state = HERO_SHUFFLED
	end
	freshList.period = "shuffle"
end

function SetHeroEmployeed(user)
	local freshList = user:GetHeroFreshList()
	if not freshList.list then
		return
	end
	for k, v in pairs(freshList.list) do
		if v.state ~= HERO_CHOSEN then
			v.state = HERO_UN_SHUFFLE
		end
	end
	freshList.period = "done"
end

--翻出可招募武将
function GenHeroIndexFromList(user)
	local freshList = user:GetHeroFreshList()
	if not freshList.type then
		return
	end
	local rateList = ClickRateList[freshList.type]
	local sum, list = 0, {}
	for k, v in ipairs(freshList.list) do
		sum = sum + rateList[v.aptLv]
		table.insert(list, {index = v.index, rate = rateList[v.aptLv]})
	end
	local ran = math.random(0, sum)
	local rate = 0
	for k, v in ipairs(list) do
		rate = rate + v.rate
		if rate >= ran then
			return v.index
		end
	end
end

function BuyHero(user, index)
	if user:GetVipLv() < EMP_BUY_VIP_LV then
		return false, "您VIP等级不足，不能直接购买武将"
	end
	if user:GetEmployeeTimes("dir") > 1 then
		return false, "您今天直接招募的次数已经用完"
	end
	if table.size(user:GetHeroList()) >= user:GetHeroMax() - 1 then
		return false, "您的武将已经达到可招募的最大数量。"
	end
	local freshList = user:GetHeroFreshList()
	if freshList.period ~= "fresh" then
		return false, "当前状态不能直接购买武将"
	end
	local heroData = freshList.list[index]
	if not heroData then
		return false, "无效的编号"
	end
	local cost = EmployeeDirect[heroData.aptLv]
	if not cost then
		return false, "只能招募紫色以上的武将"
	end
	if user:GetTotalIngot() < cost then
		return false, "您元宝数量不足"
	end
	user:SetTotalIngot(-cost)
	user:AddHero(heroData.template and "famous" or "normal", heroData)
	RmHeroFromFreshList(user, index)
	user:SetEmployeeTimes("dir", 1)
	return true
end

--修改武将翻面状态
function SetEmplHeroClicked(user, index, pos)
	local emplList = user:GetHeroFreshList()
	for k, v in pairs(emplList.list) do
		if v.index == index then
			v.state = HERO_CHOSEN
			v.index = pos
		elseif v.index == pos then
			v.index = index
		end
		if v.state ~= HERO_CHOSEN then
			v.state = HERO_UN_CHOSEN
		end
	end
	emplList.period = "click"
end

function GetExampleHeroInfo()
	local sData = {}
	local list = {"赵云", "关羽", "张飞", "黄忠", "马超"}
	for i = 1, MAX_HERO_SHOWN do
		local buff = {}
		buff.index = i
		buff.state = math.random(0, 1)
		buff.headId = math.random(101, 108)
		buff.name = list[i]
		buff.aptLv = 4
		buff.Lv = 1
		buff.job = "warrior"
		buff.hpMax = math.random(100, 300)
		buff.physcAtkMin, buff.physcAtkMax = 20, 21
		buff.physcDef = 20
		buff.magicAtkMin, buff.magicAtkMax = 10, 11
		buff.magicDef = 10
		buff.speed = 100
		if buff.state == 1 then
			buff.owner = "xxx1"
		end
		table.insert(sData, buff)
	end
	return sData
end



local s_empl_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.heroNum = table.size(user:GetHeroList()) + 1
	sData.heroMax = user:GetHeroMax()
	sData.norGold = NORMAL_FRESH_GOLD
	sData.advIngot = ADVANCE_FRESH_INGOT
--	sData.ignIngot = IGNORE_HERO_INGOT
	sData.lockFresh = {}
	for k, v in ipairs(FreshLockFamous) do
		table.insert(sData.lockFresh, {index = k, needIngot = v.needIngot})
	end
	sData.advNoRemind = user:GetSetting(SETTING.ADV_EMPL_NO_REMIND) and 1 or 0
--[[	local timestamp = user:GetHeroFreshTimestamp()
	if not timestamp or timestamp - os.time() <= 0 then		--第一次打开或者刷新时间到时候刷新列表
		local list = FreshHeroList(user, 1)
		user:SetHeroFreshList({list = list, type = 1, period = "fresh"})
		user:SetHeroFreshTimestamp(os.time() + NORMAL_FRESH_COLD_DOWN)
	end]]
	sData.coldTime = user:GetHeroFreshTimestamp() - os.time()
	connection:SendProtocol("c_empl_info", sData)
end



--刷新招募列表
local s_empl_fresh = function(connection, data)
	if type(data) ~= "table" or not data.type then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	data.type = tonumber(data.type)
	if not AptRateList[data.type] then
		return ERROR.TipMsg(connection, "请选择招募类型。")
	end
	if data.copyId and data.type == 1 then
		return ERROR.TipMsg(connection, "只有【庆典宴请】才能指定英魂副本类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local ret, msg
	if data.type == 1 then
		ret, msg = ProNormalFresh(user)
	else
		ret, msg = ProAdvancedFresh(user, copyId, data.index)
	end
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	user:SetFuncStatById(SETTING.FUNC_TAVERN, 1)
	local sData = {type = "fresh", period = "fresh"}
	sData.list = GetFormatedEmplList(user)
	connection:SendProtocol("c_empl_list", sData)
end

--获取招募中的武将列表
local s_empl_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误，无效的请求类型：%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local freshList = user:GetHeroFreshList()
	local sData = {type = "list", period = freshList and freshList.period or "done"}
	sData.list = GetFormatedEmplList(user)
	return connection:SendProtocol("c_empl_list", sData)
end

--[[忽略武将
local s_empl_ignore = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local freshList = user:GetHeroFreshList()
	if not freshList.list or #freshList.list <= 0 then
		return ERROR.TipMsg(connection, "当前可招募列表为空。")
	end
	if freshList.period ~= "fresh" then
		return ERROR.TipMsg(connection, "当前状态不允许忽略武将。")
	end
	if #freshList.list <= FRESH_HERO_NUM - IGNORE_HERO_NUM then
		return ERROR.TipMsg(connection, "最多只能忽略两名武将。")
	end
	local cost = (FRESH_HERO_NUM - #freshList.list + 1) * IGNORE_HERO_INGOT
	if user:GetTotalIngot() < cost then
		return ERROR.TipMsg(connection, "您的元宝不足，不能完成操作。")
	end
	data = tonumber(data)
	local flag = false
	for k, v in pairs(freshList.list) do
		if v.index == data then
			flag = true
		end
	end
	if not flag then
		return ERROR.TipMsg(connection, "无效的指定武将信息。")
	end
	user:SetTotalIngot(-cost)
	RmHeroFromFreshList(user, data)
	connection:SendProtocol("c_empl_ignore", data)
end]]
--武将翻面
local s_empl_shuffle = function(connection, data)
	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local freshList = user:GetHeroFreshList()
	if not freshList.list or #freshList.list <= 0 then
		return ERROR.TipMsg(connection, "当前可招募列表为空。")
	end
	if freshList.period ~= "fresh" then
		return ERROR.TipMsg(connection, "当前状态不允许抽选武将。")
	end
	ShuffleHeroEmplList(user)
	connection:SendProtocol("c_empl_shuffle", "true")
end

--选择翻面的武将
local s_empl_click = function(connection, data)
	data = tonumber(data)
	if not data then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local freshList = user:GetHeroFreshList()
	if not freshList.list or #freshList.list <= 0 then
		return ERROR.TipMsg(connection, "当前可招募列表为空。")
	end
	if freshList.period ~= "shuffle" then
		return ERROR.TipMsg(connection, "当前状态不允许选择武将。")
	end
	if freshList.period == "done" then
		return ERROR.TipMsg(connection, "您已经招募了武将。如果要继续招募，请刷新列表。")
	end
	if data <= 0 or data > FRESH_HERO_NUM then
		return ERROR.TipMsg(connection, "无效的指定武将信息。")
	end
	local index = GenHeroIndexFromList(user)
	SetEmplHeroClicked(user, index, data)
	connection:SendProtocol("c_empl_click", "true")
	PROTOCOL.s_empl_list(connection, "get")
end

--招募武将
local s_hero_empl = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local freshList = user:GetHeroFreshList()
	if not freshList.list or #freshList.list <= 0 then
		return ERROR.TipMsg(connection, "当前可招募列表为空。")
	end
	if freshList.period ~= "click" then
		return ERROR.TipMsg(connection, "当前状态不允许招募武将。")
	end
	data = tonumber(data)
	local heroData
	for k, v in pairs(freshList.list) do
		if v.index == data then
			if v.state ~= HERO_CHOSEN then
				return ERROR.TipMsg(connection, "指定的武将未被选中。")
			end
			heroData = v
			break
		end
	end
	if not heroData then
		return ERROR.TipMsg(connection, "指定的武将信息不存在。")
	end
	if table.size(user:GetHeroList()) >= user:GetHeroMax() - 1 then
		return ERROR.TipMsg(connection, "您的武将已经达到可招募的最大数量。")
	end
	local heroObj = user:AddHero(heroData.template and "famous" or "normal", heroData)
	if heroData.aptLv >= 4 then
		local heroType, heroName = "武魂", heroData.name
		if heroData.template then
			local info = FAMOUSHERO.GetFamousHeroInfoById(heroData.template)
			heroType, heroName = "英魂", info.name
		end
		local item = BASEITEM.NewItem(BASEITEM.GetHeroCarIdByAptLv(heroData.aptLv), 1, {save = heroObj:GetSaveData()})
		CHAT.SendSysMsg("system", string.format("%s 招募武将时遇到了%s[%s]，将其招入麾下。", CHAT.GetFormatedNick(user:GetName()), heroType, heroName), nil, {goodsAttr = {[1] = item:GetTipMsg()}})
	end
	SetHeroEmployeed(user)
--	freshList.period = "done"
	MISSION.CheckMissionEvent(user, "func", SETTING.FUNC_TAVERN, 1)
	connection:SendProtocol("c_hero_empl", {index = data, heroMax = user:GetHeroMax()})
end


local s_hero_fire = function(connection, data)
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local hero = user:GetHero(data)
	if not hero then
		return ERROR.TipMsg(connection, "请选择要解雇的战魂。")
	elseif not hero:IsNoEquipWorn() then
		return ERROR.TipMsg(connection, "请先卸下战魂的装备。")
	end
	local template = hero:GetTemplate()
	if template then
		FAMOUSHERO.SetFamousHeroDataById(template)
		if hero:GetLv() >= 100 then
			local tempItem = BASEITEM.NewItem(BASEITEM.GetHeroCarIdByAptLv(hero:GetAptLv()), 1, {save = hero:GetSaveData()})
			CHAT.SendSysMsg("system", string.format("玩家%s与英魂[%s]感情破裂，已解除关系。", CHAT.GetFormatedNick(user:GetName()), hero:GetName()), nil, {kind = "employee", goodsAttr = {[1] = tempItem:GetTipMsg()}})
		end
	end
	user:DelHero(data)
	connection:SendProtocol("c_hero_fire", data)
end

local s_empl_giveup = function(connection, data)
--[[	if data ~= "set" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	user:SetHeroFreshList()]]
	connection:SendProtocol("c_empl_giveup", "true")
end

function __init__()
	PROTOCOL.s_empl_info = s_empl_info
	PROTOCOL.s_empl_list = s_empl_list
--	PROTOCOL.s_hero_showList = s_hero_showList
	PROTOCOL.s_empl_fresh = s_empl_fresh
--	PROTOCOL.s_empl_ignore = s_empl_ignore
	PROTOCOL.s_empl_shuffle = s_empl_shuffle
	PROTOCOL.s_empl_click = s_empl_click
	PROTOCOL.s_hero_empl = s_hero_empl
--	PROTOCOL.s_empl_giveup = s_empl_giveup
	PROTOCOL.s_hero_fire = s_hero_fire
--	PROTOCOL.s_empl_employee = s_empl_employee
end
