--防沉迷系统 antiAddict

local REST_TIME_LONG = 5 * 3600			--离线休息5小时后解除
local NOTICE_TIME_LONG = 1800			--默认提醒间隔
local NORMAL_INCOMING_RATE = 1			--正常收益

local STATE_NO_VERIFY = -1
local STATE_YOUNG = 0
local STATE_ADULT = 1
--local VERIFY_URL = "http://192.168.0.20/chaosweb/temp/status.php?username=%s"
local VERIFY_URL = "http://www.shangguzhange.com/temp/status.php?username=%s"

local AntiAddictInfoList = {}

--获取当前防沉迷警告等级
function GetAntiAddictLvByTimelong(timelong)
	local Lv = 0
	for k, v in ipairs(AntiAddictInfoList) do
		if v.timelong <= timelong then
			Lv = k
		elseif v.timelong > timelong then
			break
		end
	end
	return Lv
end

function GetAntiAddictInfoByLv(Lv)
	return AntiAddictInfoList[Lv]
end

function AskUserAntiAddictState(roleId, callback)
	local username = USER.GetUserNameById(roleId)
	local function verify_request()
		local content = HTTP.CoRequest(string.format(VERIFY_URL, username))
		if not content then
			return
		end
		local result = string.match(content, "([%w_-]+)")
		result = tonumber(result)
		
		if result ~= STATE_NO_VERIFY and result ~= STATE_YOUNG and result ~= STATE_ADULT then
			return
		end
		local user = USER.GetOnlineUser(roleId)
		if user then
			callback(result)
		end
	end
	local co = coroutine.create(verify_request)
	coroutine.resume(co)
--[[	
		local user = USER.GetOnlineUser(roleId)
		if user then
			callback(user:GetAntiAddictState())
		end]]
end

local clsUser = USERBASE.clsUser

--获取防沉迷状态 (state: 0 未满18岁 1 已满18岁 -1 未验证 )
function clsUser:SetAntiAddictState(state)
	self.__save.antiAddict = self.__save.antiAddict or {}
	self.__save.antiAddict.state = state
end

function clsUser:GetAntiAddictState()
	self.__save.antiAddict = self.__save.antiAddict or {}
	return self.__save.antiAddict.state or -1
end


--防沉迷累计时间
function clsUser:SetAntiAddictOnlineTime(timelong)
	self.__save.antiAddict = self.__save.antiAddict or {}
	local info = self.__save.antiAddict
	if info.state == 1 then
		return
	end
	if not info.statTimestamp or CalDiffDays(info.statTimestamp, os.time()) ~= 0 then
		info.onlintTime = timelong
	else
		info.onlineTime = (info.onlineTime or 0) + timelong
	end
	info.statTimestamp = os.time()
end

function clsUser:GetAntiAddictOnlineTime()
	self.__save.antiAddict = self.__save.antiAddict or {}
	local info = self.__save.antiAddict
	if info.state == 1 then
		return 0
	end
	if not info.statTimestamp or CalDiffDays(info.statTimestamp, os.time()) ~= 0 then
		return 0
	end
	return info.onlineTime or 0
end

function clsUser:ResetAntiAddictOnlineTime()
	self.__save.antiAddict = self.__save.antiAddict or {}
	local info = self.__save.antiAddict
	info.onlineTime = 0
	info.statTimestamp = os.time()
end

--上线时启动防沉迷定时器
function clsUser:LoginStartAntiAddictCallOut()
	local function call_back(result)
		if result == 1 then
			RelieveAntiAddict(self)
		else
			self:SetAntiAddictState(result)
			local offTimelong = os.time() - (self:GetLastOfflineTime() or os.time())
			if offTimelong >= REST_TIME_LONG then
				self:ResetAntiAddictOnlineTime()
				self:SetIncomingRate(NORMAL_INCOMING_RATE)
			end
			local onlineTime = self:GetAntiAddictOnlineTime()
			local Lv = GetAntiAddictLvByTimelong(onlineTime)
			self:StartAntiAddictCallOut(Lv + 1)
		end
	end
	AskUserAntiAddictState(self:GetRoleId(), call_back)
end



--防沉迷提醒定时器
--[[
function clsUser:StartAntiAddictCallOut()
	local onlineTime = self:GetAntiAddictOnlineTime()
	local timelong = NOTICE_TIME_LONG
	local Lv = GetAntiAddictLvByTimelong(onlineTime)
	local info = GetAntiAddictInfoByLv(Lv + 1)
	if info then
		timelong = info.timelong - onlineTime
	end
	self.__temp.antiAddictTimer = CALLOUT.ObjCallOut(self:GetCallOut(), ProAntiAddictNotice, timelong, self:GetRoleId(), Lv)	
end
]]
function clsUser:StartAntiAddictCallOut(Lv)		--Lv 下一级防沉迷警告等级
	local onlineTime = self:GetAntiAddictOnlineTime()
	local timelong = 0
	local info = GetAntiAddictInfoByLv(Lv)
	if info then
		timelong = info.timelong - onlineTime
	else
		timelong = NOTICE_TIME_LONG - (info.timelong % NOTICE_TIME_LONG)
	end
	self.__temp.antiAddictTimer = CALLOUT.ObjCallOut(self:GetCallOut(), ProAntiAddictNotice, timelong, self:GetRoleId(), Lv)	
end



function clsUser:RemoveAntiAddictCallOut()
	if self.__temp.antiAddictTimer then
		CALLOUT.RemoveCallOut(self:GetCallOut(), self.__temp.antiAddictTimer)
	end
end



--发送防沉迷提示信息
function SendAnitAddictNotice(user, state, Lv)
	local info = GetAntiAddictInfoByLv(Lv)
	user:SendProtocol("c_show_tips", info.notice)
	CHAT.SendSysNotice(info.notice, {user:GetRoleId()})
	user:SendProtocol("c_antiAddict_notice", {state = state, Lv = Lv})
end


function ProAntiAddictNotice(roleId, Lv)
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return
	end
	local state = user:GetAntiAddictState()
	local info = GetAntiAddictInfoByLv(Lv)
	SendAnitAddictNotice(user, state, Lv)
	user:SetIncomingRate(info.rate)
	user:StartAntiAddictCallOut(Lv + 1)
end


function RelieveAntiAddict(user)
	user:RemoveAntiAddictCallOut()
	user:SetAntiAddictState(1)
	user:ResetAntiAddictOnlineTime()
	user:SetIncomingRate(NORMAL_INCOMING_RATE)
end


function RelieveAllOnlineUser()
	for k, v in pairs(USER.GetAllOnlineUser()) do
		if v:GetAntiAddictState() ~= 1 then
			RelieveAntiAddict(v)
			v:StartAntiAddictCallOut(1)
			SendAntiAddictNotice(v, "防沉迷计时已经重置。")
		end
	end
end


local s_antiAddict_info = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {}
	sData.state = user:GetAntiAddictState()
	if sData.state ~= 1 then
		local timelong = user:GetAntiAddictOnlineTime()
		sData.Lv = GetAntiAddictLvByTimelong(timelong)
		if sData.Lv > 0 then
			local msg = string.format("您已经累计在线%d分钟，请合理安排游戏时间。", math.floor(timelong / 60))
			CHAT.SendSysNotice(msg, {user:GetRoleId()})
		end
	end
	connection:SendProtocol("c_antiAddict_info", sData)
end

local s_antiAddict_relieve = function(connection, data)
	data = tonumber(data)
	if data ~= 1 and data ~= 0 then
		return ERROR.TipMsg(connection, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	RelieveAntiAddict(user)
	user:SetAntiAddictState(data)
	connection:SendProtocol("c_antiAddict_relieve", data)
end


function __init__()
	ANTIADDICT = Import("autocode/antiAddict.lua")
	AntiAddictInfoList = ANTIADDICT.GetTable()
	
	PROTOCOL.s_antiAddict_info = s_antiAddict_info
	PROTOCOL.s_antiAddict_relieve = s_antiAddict_relieve

	CALLOUT.Daily(getfenv(2), 0, 0, RelieveAllOnlineUser)
end


