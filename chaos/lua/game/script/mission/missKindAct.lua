--日行一善

local path_missKind = "chaos.missKindAct"
local DAY = 24 * 3600
local STATE_UNSTART = 0
local STATE_DOING = 1
local STATE_OVER = 2
local prizeNameList = {"上善若水", "大德善人", "古道热肠", "乐善好施", "乐于助人"}

local missKindObj = false

function SetMissKindAct(obj)
	missKindObj = obj
end

function GetMissKindAct()
	return missKindObj
end
---------------------------------------------------

clsAct = clsObject:Inherit()

function clsAct:__init__()
	Super(clsAct).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end


function clsAct:initsavedata()
	self.__save.id = 0
	self.__save.list = {}
end

function clsAct:inittempdata()	
	self.__temp.callout = {}
end

function clsAct:InitAct(id, startTime, lastDays, describe, link)
	self:SetId(id)
	self:SetState(STATE_UNSTART)
	
	self:SetStartTime(startTime)
	self:SetEndTime(startTime + lastDays * DAY)
	self:SetDescribe(describe)
	self:SetLink(link)
	
	self:StartActCallOut()
end

function clsAct:SetId(id)
	self.__save.id = id
end

function clsAct:GetId()
	return self.__save.id
end

function clsAct:SetState(state)
	self.__save.state = state
end

function clsAct:GetState()
	return self.__save.state
end

function clsAct:SetStartTime(time)
	self.__save.startTime = time
end

function clsAct:GetStartTime()
	return self.__save.startTime
end

function clsAct:SetEndTime(time)
	self.__save.endTime = time
end

function clsAct:GetEndTime()
	return self.__save.endTime
end

function clsAct:SetDescribe(desc)
	self.__save.describe = desc
end

function clsAct:GetDescribe()
	return self.__save.describe
end

function clsAct:SetLink(link)
	self.__save.link = link
end

function clsAct:GetLink()
	return self.__save.link
end
--奖励所需参加天数
function clsAct:SetPrizeNeedTimes(days)
	self.__save.needTimes = days
end

function clsAct:GetPrizeNeedTimes()
	return self.__save.needTimes
end

--奖励信息
function clsAct:SetPrize(prizeList)
	self.__save.prize = prizeList
end

function clsAct:GetPrize()
	return self.__save.prize
end

--奖励得主
function clsAct:SetPrizeOwner(ownerList)
	self.__save.prizeOwner = ownerList
end

function clsAct:GetPrizeOwner()
	return self.__save.prizeOwner
end

function clsAct:SetRoleJoinInfo(roleId, data)
	self.__save.list[roleId] = self.__save.list[roleId] or {}
	self.__save.list[roleId].times = (self.__save.list[roleId].times or 0) + data
	self.__save.list[roleId].timestamp = os.time()
end

function clsAct:GetRoleJoinInfo(roleId)
	return self.__save.list[roleId]
end

function clsAct:GetRoleJoinTimes(roleId)
	if not self.__save.list[roleId] then return 0 end
	return self.__save.list[roleId].times
end

--今日是否已经参加
function clsAct:IsTodayJoined(roleId)
	local roleInfo = self:GetRoleJoinInfo(roleId)
	if not roleInfo or CalDiffDays(roleInfo.timestamp, os.time()) ~= 0 then
		return false
	end
	return true
end

function clsAct:GetJoinedRoleList()
	return self.__save.list
end


function clsAct:GetCallOut()
	return self.__temp.callout
end

--启动活动定时器
function clsAct:StartActCallOut()
	local state = self:GetState()
	if state == STATE_UNSTART then
		local timelong = self:GetStartTime() - os.time()
		if timelong <= 0 then
			return self:ActBegin()
		end
		self.__temp.timer = CALLOUT.ObjCallOut(self:GetCallOut(), clsAct.ActBegin, timelong, self)
	elseif state == STATE_DOING then	
		local timelong = self:GetEndTime() - os.time()
		if timelong <= 0 then
			return self:ActOver()
		end
		self.__temp.timer = CALLOUT.ObjCallOut(self:GetCallOut(), clsAct.ActOver, timelong, self)
	end
end

function clsAct:GetSaveData()
	return self.__save
end

function clsAct:Restore(data)
	assert(type(data) == "table")
	self.__save = data
end

function clsAct:ActBegin()
	self:RemoveActCallOut()
	
	self:SetState(STATE_DOING)
	self:StartActCallOut()
end

--移除活动定时器
function clsAct:RemoveActCallOut()
	if self.__temp.timer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.timer)
	end
end


function clsAct:SendActPrize(rank, roleId)
	local mail = {}
	mail.title = "【积善成德】奖励"
	mail.content = string.format("昔日积善因，今朝得善果。恭喜您在本次【积善成德】的活动中高中【%s】大奖，", prizeNameList[rank])
	local prizeList = self:GetPrize()
	local itemInfo = BASEITEM.GetItemById(prizeList[rank].goodsId)
	if prizeList[rank].goodsId == 104004 then
		mail.content = mail.content .. string.format("获得了 %s × %d。奖励已经随附件进行发放，请及时领取。", itemInfo.name, prizeList[rank].num)
		mail.attach = {item = {{id = prizeList[rank].goodsId, num = prizeList[rank].num}}}
	else
		mail.content = mail.content .. string.format("获得了 %s × %d。请联系活动举办方进行领取。", itemInfo.name, prizeList[rank].num)
	end
	MAIL.SendSysMail(roleId, mail)
end

function clsAct:ActOver()
	self:RemoveActCallOut()
	self:SetState(STATE_OVER)
	
	local list = {}
	for k, v in pairs(self:GetJoinedRoleList()) do
		if v.times >= self:GetPrizeNeedTimes() then
			table.insert(list, k)
		end
	end
	local ownerList = {}
	for k, v in ipairs(self:GetPrize()) do
		if not v.owner or not USER.GetUserNickById(v.owner) then
			if #list > 0 then
				local ran = math.random(1, #list)
				table.insert(ownerList, {rank = k, owner = list[ran]})
				table.remove(list, ran)
			end
		else
			table.insert(ownerList, {rank = k, owner = v.owner})
		end
	end
	self:SetPrizeOwner(ownerList)
	for _, v in ipairs(ownerList) do
		self:SendActPrize(v.rank, v.owner)
	end
	ClearMissKindAct()
end


---------------------------------------------------
--发布新活动
function NewAct(startTime, lastDays, describe, link, needDays, prize)
	local actObj = clsAct:New()
	local id = STATS.GenId("missKind")
	actObj:InitAct(id, startTime, lastDays, describe, link)
	actObj:SetPrizeNeedTimes(needDays)
	actObj:SetPrize(prize)
	SetMissKindAct(actObj)
	UpdateMissKindActState(FUNCOPEN.STATE_DOING)
	return actObj
end

--清理当前活动
function ClearMissKindAct()
	local actObj = GetMissKindAct()
	if not actObj then
		return
	end
	local saveData = actObj:GetSaveData()
	saveData._id = actObj:GetId()
	DATA.UpdateData(path_missKind, {_id = actObj:GetId()}, saveData)
	DATA.DBRemove(path_missKind, {_id = "now"})
	SetMissKindAct(false)
	UpdateMissKindActState(FUNCOPEN.STATE_INVALID)
end

--保存活动数据
function SaveMissKindAct()
	local actObj = GetMissKindAct()
	if not actObj then
		return
	end
	local saveData = actObj:GetSaveData()
	saveData._id = "now"
	DATA.UpdateData(path_missKind, {_id = "now"}, saveData)
end

--回复活动数据
function RestoreMissKindAct()
	local function read_back(Data)
		Data = UnSerialize(Data)
		if Data[1] then
			local actObj = clsAct:New()
			actObj:Restore(Data[1])
			actObj:StartActCallOut()
			SetMissKindAct(actObj)
		end
	end
	DATA.Read(path_missKind, {_id = "now"}, read_back)
end

function UpdateMissKindActState(state)
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:IsFuncOpen(SETTING.FUNC_MISS_KIND) then
			FUNCOPEN.SendFuncStateUpdate(v, SETTING.FUNC_MISS_KIND, state)
		end
	end
end

--------------------- 接口示例 非前台用 --------------------

local s_missKind_newAct = function(connection, data)
	if not type(data) ~= "table" or not data.startMonth or not data.startDay or not data.lastDays then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	if not data.describe or not data.link then
		return ERROR.TipMsg(connection, "请输入活动描述及链接。")
	end
	if not data.needDays then
		return ERROR.TipMsg(connection, "请输入可参与抽奖的最小参与天数。")
	end
	if not data.prize or #data.prize <= 0 then
		return ERROR.TipMsg(connection, "请填写活动奖励信息。")
	end
	local buff = {}
	for k, v in ipairs(data.prize) do
		if not data.goodsId or not BASEITEM.GetItemById(data.goodsId) then
			return ERROR.TipMsg(connection, string.format("无效的奖励信息：rank %d.", k))
		end
		if not data.num then
			return ERROR.TipMsg(connection, string.format("请输入奖励的数量。"))
		end
		table.insert(buff, {goodsId = v.goodsId, num = v.num, owner = v.owner})
	end
	if GetMissKindAct() then
		return ERROR.TipMsg(connection, "【积善成德】活动正在进行中，不能发布新的活动。")
	end
	local tm = os.date()
	tm.month = data.startMonth
	tm.day = data.startDay
	tm.hour, tm.min, tm.sec = 0, 0, 0
	NewAct(os.time(tm), data.lastDays, data.describe, data.link, data.needDays, buff)
	connection:SendProtocol("c_missKind_newAct", data)
end

local s_missKind_modPrize = function(connection, data)
	if not type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local actObj = GetMissKindAct()
	if not actObj then
		return ERROR.TipMsg(connection, "当前没有正在进行中的活动。")
	end
	if not data.prize or #data.prize <= 0 then
		return ERROR.TipMsg(connection, "请填写活动奖励信息。")
	end
	local buff = {}
	local prizeList = actObj:GetPrize()
	for k, v in ipairs(data.prize) do
		prizeList[k].owner = v.owner
	end
	actObj:SetPrize(prizeList)
	connection:SendProtocol("c_missKind_modPrize", data)
end


function __init__()

	RestoreMissKindAct()
	CALLOUT.CallFre(SaveMissKindAct, 9 * 60)
end
