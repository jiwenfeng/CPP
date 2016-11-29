
function LoginUser(roleId, callback, ...)
	function User_Login_Back(data, errmsg)
		if not data then
			DATA.log(string.format("玩家%s登录游戏失败，没有用户数据。", roleId, errmsg or ""), "login.log")
			return
		end
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
			end
			user:SetOfflineProState(false)			--离线操作处理阶段结束
			user:ProOfflineData()
			user:LoginProUserData()
			user:SetLoginState(1)					--解除发往客户端的协议屏蔽
			user:SetLogin("127.0.0.1")
			LOGIN.AfterLogin(user)
			FIGHT.UpdateUserFightData(roleId)
			callback(unpack(arg))
		end
		OFFLINE.LoginProOffline(user, AfterPro)			--处理离线操作			
	end
	if not USER.ReadUserData(roleId, User_Login_Back) then
		return
	end
end

function ExitAllUser()
	for k in pairs(USER.GetAllUserNick()) do
		local user = USER.GetOnlineUser(k)
		if user and user:GetUserName() ~= "wwww" then
			EXIT.UserExit(user)
		end
	end	 
	return "全部用户退出成功"
end

function AutoJoinNodeFight(name, start, tail, nodeId)
	local obj = NODE.GetNodeObjById(nodeId)
	if not obj then
		return "无效的节点编号"
	end
	for i = start, tail do
		local username = name .. tostring(i)
		local roleId = USER.GetRoleIdByUserName(username)
		local user = USER.GetOnlineUser(roleId)
		if user then
			obj:AddMember(roleId)
		else
			LoginUser(roleId, CLSNODE.clsNode["AddMember"], obj, roleId)
		end
	end
	return "全部用户参战完成"
end

function BatNewUser(base, num, phyle)
	num = tonumber(num)
	if num < 0 then
		return
	end
	local list = FormatUserModelList()
	for i = 1, num do
		local username = base .. tostring(i)
		LOGIN.RecordRegInfo(username, username)
		local nick = NAME.GenName()
		local user = USERBASE.clsUser:New()
		local info = list[math.random(1, #list)]
		local userId = STATS.GenId("user")			--统一Id生成器
		user:InitUser(username, nick, userId, info.headId, info.job, info.sex, info.picId)
		InitUserGuideData(user)
		NAME.SetNameExist(nick)
		USER.SetOnlineUser(userId, user)
		user:SetExp(10000000000)
		user:SetPhyle(phyle)
		EXIT.UserExit(user)
	end
	return "全部用户注册完成"
end


