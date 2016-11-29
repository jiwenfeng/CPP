--管理员账号
--权限等级1-3, -1为非许可

local ADMIN_GROUP = {
--	["admin"] = 3,		--原来是100
	["ilflyf"] = 3,
	["CCCC"] = 10,
	["xxxx"] = 100,
	["xxx1"] = 100,
	["xxx2"] = 100,
	["wwww"] = 100,
	["dddd"] = 10,
	["ssss"] = 10,
	["huanghaha"] = 10,
}  --权限等级3级


--分解字符串
function AnalyzeStr(str)
	local list = {}
	local i, j = 0, 0, 0
	while string.find(str, ",") do
		j = string.find(str, ",")
		local s = string.sub(str, i, j-1)
		list[#list + 1] = s
		str = string.sub(str, j + 1, -1)
	end
	list[#list + 1] = str
	return list[1], list[2], list[3], list[4]
end

--可修改的玩家属性表
local mod_user_tbl = {
	["gold"] = "SetGold",
	["ingot"] = "SetIngot",
	["voucher"] = "SetVoucher",
	["nimbus"] = "SetNimbus",
	["exp"] = "SetExp",
	["lv"] = "SetLv",
--	["point"] = "ModLeftPoint",
	["strength"] = "ModStrength",
	["intellect"] = "ModIntellect",
	["agiltiy"] = "ModAgility",
	["captain"] = "ModCaptain",
	["vigour"] = "SetVigour",
}

local function ModUser(connection, data)
	local nick, type, num = AnalyzeStr(data)
	if not type or not nick or not num then
		return ERROR.ErrorMsg(connection, "输入有误，请重新输入")
	end
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.ErrorMsg(connection, string.format("没有该用户信息:%s", nick))
	end
	local function mod_user(user)
		if type == "name" then
			NAME.DelNameExist(user:GetName())
			user:SetName(num)
			NAME.SetNameExist(num)
		elseif mod_user_tbl[type] then
			num = tonumber(num)
			if not num then
				return ERROR.ErrorMsg(connection, "无效的修改数量num：%s", num or "")
			end
			user[mod_user_tbl[type]](user, num)
		end
	end
	local user = USER.GetOnlineUser(roleId)
	if user and not user:GetOfflineProState() then
		mod_user(user)
	elseif OFFLINE.GetValidProcessTbl()[type] then
		OFFLINE.SetOfflineData(roleId, type, num)
	else
		return ERROR.ErrorMsg(connection, "该玩家已经下线。")
	end
	if type == "name" then
		DATA.log(string.format("%s把用户%s的昵称由%s修改为%s", USER.GetUserNameById(connection.roleId), roleId, nick, num), "admin.log")
		return ERROR.TipMsg(connection, string.format("给用户%s改名成功，当前昵称为%s。", nick, num))
	elseif mod_user_tbl[type] then
		DATA.log(string.format("%s给用户%s增加%s %d", USER.GetUserNameById(connection.roleId), nick, type, num), "admin.log")
		return ERROR.TipMsg(connection, string.format("成功给用户%s增加%s %d。", nick, type, num))
	else
		return ERROR.ErrorMsg(connection, string.format("无效的输入类型：%s", type))
	end
end
--禁言玩家
local function BanUser(connection, data)
	local nick, time = AnalyzeStr(data)
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.ErrorMsg(connection, string.format("没有该用户信息:%s", nick))
	end
	local function gag_user()
		if not time then
			CHAT.SetUserBanned(roleId)
			DATA.log(string.format("%s解禁了用户%s", USER.GetUserNameById(connection.roleId), nick), "admin.log")
			ERROR.TipMsg(connection, string.format("解禁用户%s成功。", nick))
		else
			if not tonumber(time) then
				return ERROR.ErrorMsg(connection, string.format("无效的禁言时间：%s", time))
			end
			time = tonumber(time)
			CHAT.SetUserBanned(roleId, time, connection.roleId)
			DATA.log(string.format("%s禁言了用户%s，State：%d，Time：%d。", USER.GetUserNameById(connection.roleId), nick, (time == 0) and 0 or 1, time or 0), "admin.log")
			ERROR.TipMsg(connection, string.format("禁言用户%s成功，时间%d。", nick, time))
		end
	end
	return gag_user()
end

local mod_hero_tbl = {
	["aptitude"] = "SetAptitude",
	["lv"] = "SetLv",
	["exp"] = "SetExp",
}
local hero_attr = {
	["strength"] = "ModStrength",
	["intellect"] = "ModIntellect",
	["agiltiy"] = "ModAgility",
	["captain"] = "ModCaptain",
}

local function ModHero(connection, data)
	local nick, heroId, type, num = AnalyzeStr(data)
	if not nick or not heroId or not type or not num then
		return ERROR.ErrorMsg(connection, "输入有误，请重新输入")
	end
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.ErrorMsg(connection, string.format("没有该玩家信息:%s", nick))
	end
	local function mod_hero(user)
		local hero = user:GetHero(heroId)
		if not hero then
			return ERROR.ErrorMsg(connection, string.format("没有找到该武将信息:%s", heroId))
		end
		num = tonumber(num)
		if mod_hero_tbl[type] then
			hero[mod_hero_tbl[type]](hero, num, "admin")
			DATA.log(string.format("%s把玩家%s的武将%s的属性%s修改为/增加了%d", USER.GetUserNameById(connection.roleId), nick, heroId, type, num), "admin.log")
			return ERROR.TipMsg(connection, string.format("修改%s的武将%s的属性%s成功，修改值为%d。", nick, heroId, type, num))
		else
			return ERROR.ErrorMsg(connection, string.format("无效的输入类型：%s", type))
		end
	end
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return ERROR.ErrorMsg(connection, string.format("修改玩家%s的武将%s属性失败，该玩家已经离线。", nick, heroId))
	else
		mod_hero(user)
	end
end


local function GetOnline(connection, data)
	local nUser = LOGIN.GetOnlineUserNum()
	return ERROR.ErrorMsg(connection, string.format("当前在线玩家%d人。", nUser))
end

local function GetUserName(connection, data)
	local roleId = data
	local username = USER.GetUserNameById(roleId)
	if not username then
		return ERROR.ErrorMsg(connection, "没有该用户信息")
	end
	return ERROR.ErrorMsg(connection, string.format("你所查询的%s的用户名是%s。", roleId, username))
end

local function GetNick(connection, data)
	local roleId = data
	local nick = USER.GetUserNickById(roleId)
	if not nick then
		return ERROR.ErrorMsg(connection, "没有该用户信息")
	end
	return ERROR.ErrorMsg(connection, string.format("你所查询的%s的昵称是%s。", roleId, nick))
end

local function SendNotice(connection, data)
	local type, msg = AnalyzeStr(data)
	type = tonumber(type)
	if type == 0 then
		CHAT.SendSysNotice(msg)
	elseif type == 1 then
		CHAT.SendSysMsg("system", msg)
	else
		return ERROR.TipMsg(connection, string.format("无效的类型：%d", type or ""))
	end
	DATA.log(string.format("%s发布公告：%s", USER.GetUserNameById(connection.roleId), data), "admin.log")
end

local function SendHearsay(connection, data)
	local pos = string.find(data, ",")
	local nick = string.sub(data, 0, pos - 1)
	local msg = string.sub(data, pos + 1, -1)
	local sData = {type = "hearsay", msg = msg, from = nick}
	for tRoleId, obj in pairs(USER.GetAllOnlineUser()) do
		local con = LOGIN.GetConnection(tRoleId)
		if con and con.roleId == tRoleId then
			con:SendProtocol("c_chat", sData)
		end
	end
	DATA.log(string.format("%s以%s的名义发送喇叭消息：%s", USER.GetUserNameById(connection.roleId), nick, msg), "admin.log")
end

local function SendPrompt(connection, data)
	local pos = string.find(data, ",")
	local nick = string.sub(data, 0, pos - 1)
	local msg = string.sub(data, pos + 1, -1)
	local roleId = USER.GetUserNickById(nick)
	if not LOGIN.GetConnection(roleId) then
		return ERROR.ErrorMsg(connection, string.format("玩家%s已经离线", nick))
	end
	CHAT.SendSysPrompt(roleId, msg)
	DATA.log(string.format("%s向%s发送了系统提示信息：%s", USER.GetUserNameById(connection.roleId), nick, msg), "admin.log")
end

local InValidList = {
	["unit/user.lua"] = 1,	["module/login.lua"] = 1,
}
local DynamicFileList = {
	
}

local function UpdateFile(connection, data)
	if not string.match(data, "^[_%w]+[./_%w]*") then
		return
	end
	if not (io.open(data, "r")) then
		return
	end
	local function update_file()
		if Update(data) then
			CHAT.SendSysPrompt(connection.roleId, string.format("%s: Module \"%s\" Updated !", os.date("%H:%M:%S"), data))
			return DATA.log(string.format("%s更新文件%s", USER.GetUserNameById(connection.roleId), data), "admin.log")
		else
			CHAT.SendSysMsg("system", string.format("%s: Module \"%s\" update failed, please retry !", os.date("%H:%M:%S"), data), {connection.roleId})
		end
	end
	if InValidList[data] then
		return CHAT.SendSysMsg("system", string.format("%s: Module \"%s\" contains dynamic data, update failed ...", os.date("%H:%M:%S"), data), {connection.roleId})
	elseif DynamicFileList[data] then
		DynamicFileList[data]()
		CHAT.SendSysPrompt(connection.roleId, string.format("%s: Module \"%s\" save done !", os.date("%H:%M:%S"), data))
		CALLOUT.CallOut(update_file, 2)
	else
		update_file()
	end
end

local function LoadString(connection, Str)
	if not Str then
		return
	end
	local func = loadstring(Str)
	if not func then
		return
	end
	local env = {
		self = connection
	}
	setmetatable(env, {__index = _G,})
	local result = { setfenv(func, env)() }
	ERROR.ErrorMsg(connection, FormatSerialize(result))
	DATA.log(string.format("%s loadstring %s", USER.GetUserNameById(connection.roleId), Str), "admin.log")
end

--转发协议：协议测试功能
local function TestProtocol(connection, data)
	local nick, pro, str = string.match(data, "^(.+),([%w%_]+),(.+)$")
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.ErrorMsg(connection, "该玩家不存在")
	end
	local con = LOGIN.GetConnection(roleId)
	if not con then
		return ERROR.ErrorMsg(connection, "该玩家已经离线")
	end
	local ProID = PROTOCOL.GetProtocolIdByName(pro)
	if not ProID and not PROTOCOL[pro] then
		return ERROR.ErrorMsg(connection, string.format("协议错误，没有找到该协议:%s", pro))
	end
	str = lengine.jsondecode(str)
	if PROTOCOL[pro] then
		PROTOCOL[pro](con, str)
	else
		con:SendProtocol(pro, str)
	end
	return ERROR.TipMsg(connection, string.format("协议\"%s\"已经成功转发！", pro))
end

local function AddItem(connection, data)
	local nick, id, num, param = AnalyzeStr(data)
	if not id or not num then
		return ERROR.ErrorMsg(connection, "请输入要添加的物品Id及数量")
	end
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("指定的用户%s不存在", nick))
	end
	id = tonumber(id)
	num = tonumber(num)
	local info = BASEITEM.GetItemById(id)
	if not info then
		return ERROR.ErrorMsg(connection, string.format("没有该物品信息：%d", id or 0))
	end
	if param then
		param = UnSerialize(param)
	end
	local user = USER.GetOnlineUser(roleId)
	if not user or user:GetOfflineProState() then
		OFFLINE.SetOfflineData(roleId, "item", id, num, param)
	else
		local item = BASEITEM.NewItem(id, num, param)
		local ret, left = user:IsItemCanAdd(1, item)
		if not ret then
			return ERROR.ErrorMsg(connection, string.format("%s的背包已满，请整理背包或者解锁更多的背包空间。", nick))
		elseif left < num then
			return ERROR.ErrorMsg(connection, string.format("%s的背包最多还能再容纳%d个%s。", nick, left, info.name))
		end
		user:AddItem(1, item)
	end
	local msg = string.format("%s给玩家%s增加了%d个%s", USER.GetUserNameById(connection.roleId), nick, num, info.name)
	if param then
		msg = msg .. string.format(", missionId:%s, lock:%d", param.missionId or "", param.lock or 0)
	end
	DATA.log(msg .. "。", "admin.log")
	return ERROR.ErrorMsg(connection, string.format("成功给玩家%s增加了%d个%s", nick, num, info.name))
end


--禁用用户
local function DelUser(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("指定的用户%s不存在", data))
	end
	local user = assert(USER.GetOnlineUser(roleId))
	if user then
		EXIT.UserExit(user)
	end
	USER.ModUserNickInfo(roleId, "state", 0)
	DATA.log(string.format("%s通过GM协议禁用了用户%s", USER.GetUserNameById(connection.roleId), data), "admin.log")
	return ERROR.TipMsg(connection, string.format("禁用用户%s成功", data))
end
--恢复用户
function RecoverUser(connection, data)
	local roleId = USER.GetRoleIdByNick(data)
	if not roleId then
		return ERROR.TipMsg(connection, string.format("指定的用户%s不存在", data))
	end
	USER.ModUserNickInfo(roleId, "state", 1)
	DATA.log(string.format("%s通过GM协议恢复了用户%s", USER.GetUserNameById(connection.roleId), data), "admin.log")
	return ERROR.TipMsg(connection, string.format("恢复用户%s成功", data))
end


--系统播报
local SysHelp = {list = nil, timer = nil, env = {}}
--setmetatable(SysHelp.list, {__mode = "v"})
local helpFile = "autocode/sysHelp"

function SendSysHelp(src)
	local ran = math.random(1, #src)
	CHAT.SendSysMsg("system", src[ran])
end

local function StartSysHelp(connection, data)
	data = tonumber(data) or 300
	if SysHelp.timer then
		return ERROR.TipMsg(connection, "系统播报已经在运行中了！")
	end
	SysHelp.list = require(helpFile)
	SysHelp.timer = CALLOUT.ObjCallFre(SysHelp.env, SendSysHelp, data, SysHelp.list)
	ERROR.TipMsg(connection, string.format("系统播报运行成功，频率为%d秒/次！", data))
	DATA.log(string.format("%s启动了系统播报。", USER.GetUserNickById(connection.roleId)), "admin.log")
end

local function EndSysHelp(connection, data)
	if not SysHelp.timer then
		return ERROR.TipMsg(connection, "系统播报尚未运行。")
	end
	CALLOUT.RemoveCallOut(SysHelp.env, SysHelp.timer)
	SysHelp.list = nil
	SysHelp.timer = nil
	package.loaded[helpFile] = nil
	ERROR.TipMsg(connection, "系统播报停止成功！")
	DATA.log(string.format("%s停止了系统播报。", USER.GetUserNickById(connection.roleId)), "admin.log")
end


--删除玩家数据(不可恢复，慎用)
local function RemoveUser(username, serverId)
	local roleId = USER.GetRoleIdByUserName(username, serverId or SERVER_ID)
	if not roleId then
		return
	end
	local function remove_user()
		USER.GetAllUserNick()[roleId] = nil
		DATA.DBRemove("chaos.usernick", {_id = roleId})
		DATA.DBRemove("chaos.user", {_id = roleId})
		NAME.DelNameExist(USER.GetUserNickById(roleId))
	end
	local user = USER.GetOnlineUser(roleId)
	if user then
		EXIT.ExitPrepare(user)
		
		USER.SaveUserData(user, remove_user)				--保存数据
		USER.SetOnlineUser(roleId)
		DATA.log(string.format("%s退出了游戏，IP：%s，当前在线玩家人数：%d", username, user:GetLoginIp(), LOGIN.GetOnlineUserNum()), "login.log")
		user:Destroy()
	else
		remove_user()
	end
end


local function testFight(connection, data)
	local attack, defend, times = AnalyzeStr(data)
	if not attack or not defend or not times then
		return ERROR.ErrorMsg(connection, "输入有误，请重新输入")
	end
	TESTFIGHT.BatFightTest(connection, attack, defend, times)
end


local function ClearItem(connection, data)
	local nick, type = AnalyzeStr(data)
	if not nick or not type then
		return ERROR.ErrorMsg(connection, "输入有误，请重新输入。")
	end
	local roleId = USER.GetRoleIdByNick(nick)
	if not roleId then
		return ERROR.TipMsg(connection, "没有指定的玩家信息。")
	end
	type = tonumber(type)
	if not type or type < 1 or type > 3  then
		return ERROR.TipMsg(connection, "没有指定的背包空间类型。")
	end
	local user = USER.GetOnlineUser(roleId)
	if not user then
		return ERROR.TipMsg(connection, "指定的玩家已经离线。")
	end
	for k, v in pairs(user:GetAllItemList(type)) do
		user:DelItemByAreaId(type, k, v:GetNum())
	end
	ERROR.TipMsg(connection, "操作完成。")
end

function BatPro(user)
	USER.UpdateRankInfo(user:GetRoleId())
--	user:SetCurMopup()
--	user:InitCopy()
--	local roleId = user:GetRoleId()
--[[	user.__save.achv = {}
	user:SetLastLocateInfo(697001, 75, 415)
	user:SetHeadId(818001)
	user:SetModelId(808001)
	user:ResetWelfareList()
	user:InitAchvList()
	user:InitCopyInfo()]]
	--user:AddPet()
--	user:InitFuncStat()
--	ARENA.EnterArena(user)
--	user:AddPet()
--	user:Upgrade()
--	user:InitStatus(user:GetRoleId())
--	USER.ModUserNickInfo(user:GetRoleId(), "sex", user:GetSex())
--[[	local tribe = user:GetTribe()
	if tribe then
		user:InitTribe()
	end
	if user:GetLv() >= 15 then
		local obj = UPGRADELIST.NewList("arena", "1", UPGRADELIST.LIST_UNLOCK, user:GetRoleId())
		user:AddUpgradeList("arena", "1", obj)
	end	]]


--	user:InitFairyDev()

--	USER.ModUserNickInfo(user:GetRoleId(), "title", user:GetCurTitle())
--	user:ResetFightForce()
--	for k, v in pairs(user:GetHeroList()) do
--		v:ResetFightForce()
--	end

--	SKILL.Reset(user)
	
--	USER.ModUserNickInfo(roleId, "cityId", user:GetCityId())	
--	user:ResetTripodState()
--	user:ResetFormation()
--	user:InitStatus()
--	user:InitUserMail()
--	user:ClearTechInfo()
end

local function LoginAllUser(connection, data)
	local list = {}
	for k in pairs(USER.GetAllUserNick()) do
		table.insert(list, k)
	end
	local buff = {}
	local function offLineLogin(roleId)
		local function User_Login_Back(data, errmsg)
			if not data then
				DATA.log(string.format("玩家%s登录游戏失败，没有用户数据。", roleId, errmsg or ""), "login.log")
				return
			end
			CHAT.SendSysPrompt(connection.roleId, string.format("%s  Now:%s(%d/%d)..", os.date("%M:%S"), USER.GetUserNameById(roleId), buff[roleId], #list))
			
			data = UnSerialize(data)
			if not data or not data[1] then
				DATA.log(string.format("玩家%s离线登录游戏失败，%s", roleId, errmsg or ""), "login.log")
				return
			end
			local user = USERBASE.clsUser:New()
			user:Restore(data[1])
			user:SetOfflineProState(true)		--进入离线操作处理阶段
			USER.SetOnlineUser(roleId, user)				--设置玩家数据在线
			
			local function AfterPro(ret)
				if not ret then
					return CHAT.SendSysPrompt(connection.roleId, string.format("%s  Failed:%s ", os.date("%M:%S"), USER.GetUserNameById(roleId)))
				end
				user:SetOfflineProState(false)			--离线操作处理阶段结束
				user:ProOfflineData()
				user:LoginProUserData()
				user:SetLoginState(1)					--解除发往客户端的协议屏蔽
				LOGIN.AfterLogin(user)
				BatPro(user)
				
				DATA.log(string.format("%s离线登录了游戏，IP：%s，当前在线玩家人数：%d", USER.GetUserNameById(roleId), "", LOGIN.GetOnlineUserNum()), "login.log")
				FIGHT.UpdateUserFightData(roleId)
				EXIT.UserExit(user)
			end
			OFFLINE.LoginProOffline(user, AfterPro)			--处理离线操作			
		end
		if not USER.ReadUserData(roleId, User_Login_Back) then
			return
		end
	end
	for i = 1, #list do
		local timelong = (i - 1) * 0.2 + 1
		buff[list[i]] = i
	--	if list[i] ~= "500011" then
			local user = USER.GetOnlineUser(list[i])
			if not user then
				CALLOUT.CallOut(offLineLogin, timelong, list[i])
			else
				BatPro(user)
				CHAT.SendSysMsg("system", string.format("%s  %s(%d/%d) is done!", os.date("%M:%S"), USER.GetUserNameById(list[i]), i, #list), {connection.roleId})
			end
	--	end
	end
end



local GM_COMMAND = {
	["updatefile"] = {Lv = 100, func = UpdateFile}, 		--更新权限文件
	["loadstring"] = {Lv = 10, func = LoadString},

	["moduser"] = {Lv = 3, func = ModUser},					--修改玩家数值属性
	["modhero"] = {Lv = 3, func = ModHero},					--修改玩家武将的数值属性
	["additem"] = {Lv = 1, func = AddItem},					--修改物品(nick,id,num,{missionId = ,lock = })

	["getusername"] = {Lv = 1, func = GetUserName},
	["getnick"] = {Lv = 1, func = GetNick},
	["getonline"] = {Lv = 1, func = GetOnline},
	["sendnotice"] = {Lv = 1, func = SendNotice},   		--发公告
	["sendhearsay"] = {Lv = 1, func = SendHearsay},   		--发谣言
	["sendprompt"] = {Lv = 1, func = SendPrompt},  			--发送系统提示
	["testpro"] = {Lv = 3, func = TestProtocol},   			--测试协议
	["loginalluser"] = {Lv = 100, func = LoginAllUser},   			--离线登录玩家

--	["updaterank"] = {Lv = 1, func = UpdateRank},

	["banuser"] = {Lv = 3, func = BanUser},
	["deluser"] = {Lv = 3, func = DelUser},
	["recoveruser"] = {Lv = 3, func = RecoverUser},

	["startsyshelp"] = {Lv = 1, func = StartSysHelp},
	["endsyshelp"] = {Lv = 1, func = EndSysHelp},
	["testFight"] = {Lv = 10, func = testFight},

	["clearItem"] = {Lv = 1, func = ClearItem},
}

function GetCommand(data)
	return GM_COMMAND[data]
end
function GetPermit(roleId)
	local username = USER.GetUserNameById(roleId)
	return tonumber(ADMIN_GROUP[username]) or -1
end


--处理GM指令
function ProGmCommand(connection, command, data)
	if not GetCommand(command) then
		return
	end
	if GetPermit(connection.roleId) < GetCommand(command).Lv then
		return
	end
	return GetCommand(command).func(connection, data)
end

--测试外网http请求
function testRequest(name, pwd)
	local Url = string.format("http://192.168.0.20/chaosweb/proving.php?&username=%s&ytpwd=%s", name, pwd)
	local function co_request()
		local content = HTTP.CoRequest(Url)
		print("request returned:", content)
	end

	local co = coroutine.create(co_request)
	coroutine.resume(co)
end

