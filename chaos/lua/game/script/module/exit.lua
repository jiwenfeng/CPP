function UserDataIsUsed(user)
	if LOGIN.GetConnection(user:GetRoleId()) then
		return true
--	elseif table.size(user:GetSubscibe()) > 0 then
--		return true
	end
	return false
end

function ExitUserIsUsed(user)
	if LOGIN.GetConnection(user:GetRoleId()) then
		return true
--	elseif table.size(user:GetSubscibe()) > 1 then
--		return true
--	elseif table.size(user:GetSubscibe()) == 1 and user:GetSubscibe()[user:GetRoleId()] then
--		return false
--	elseif table.size(user:GetPKTbl()) > 0 then
--		return true
	end
	return false
end

--退出存储用户
function UserExit(user)
	local roleId = user:GetRoleId()
	local function User_Exit_Back(data, errmsg)
		if not data then
			return DATA.log(string.format("%s exit save error:%s", roleId, errmsg), "exit.log")
		end
	end
	if not user then
		return
	end
	ExitPrepare(user)
	local con = LOGIN.GetConnection(user:GetRoleId())
	if not USER.GetOnlineUser(user:GetRoleId()) then
		print("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeexit error:", user:GetUserName(), os.date(), "testbug = ", con and con.testBug)
	end
	if con then
		con.username = nil
		con.roleId = nil
		con.user = nil
		con.session = nil
		con.type = nil
		con:Close()
	end
	USER.SaveUserData(user, User_Exit_Back)				--保存数据
	USER.SetOnlineUser(roleId)
	DATA.log(string.format("%s退出了游戏，IP：%s，当前在线玩家人数：%d", roleId, user:GetLoginIp() or "", LOGIN.GetOnlineUserNum()), "login.log")
	user:Destroy()
end



--退出前的准备
function ExitPrepare(user)
--[[	user:ClearMail()
	RANK.UpdateUserRank(user)]]
	local roleId = user:GetRoleId()
	user:OfflineProFight()
	USER.UpdateUserInfo(user)
	USER.UpdateRankInfo(roleId)
	FIGHT.UpdateUserFightData(roleId)
	CITY.RefreshUserCityInfo(roleId)
	EMBASSY.RenewStayHeroInfo(user)
	local focus = user:GetFocus()
	if focus.type == "embassy" then
		local embassy = EMBASSY.GetEmbassyObjById(focus.id)
		if embassy then
			embassy:SetVisitor(roleId)
		end
	end
	user:SetTotalOnlineTime()
	user:DestroyTeam()
	user:SetOfflineTime(os.time())
	user:ExitRemoveCallOut()
	USER.ClearUserFocusOn(user)
	if user:GetMergeFairyDev() then
		FAIRYDEV.KeepMerged(user, true)
	end
	local copyObj = user:GetCurCopy()
	if copyObj then
		user:LeaveCopy(copyObj)
	end
	user:LeaveScene()
end



