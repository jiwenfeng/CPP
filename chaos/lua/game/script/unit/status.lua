--玩家仙职系统


----------------------------------------------------------------
local clsUser = USERBASE.clsUser

--获取仙职
function clsUser:InitStatus()
	local status = CLSSTATUS.clsStatus:New()
	status:InitStatus(self:GetRoleId())
	self.__temp.status = status
end

function clsUser:GetStatus()
	return self.__temp.status
end

function clsUser:GetStatusLv()
	local status = self:GetStatus()
	if not status then
		return 0
	end
	return status:GetLv()
end

function clsUser:SaveStatusData()
	local status = self:GetStatus()
	if status then
		self.__save.status = status:GetSaveData()
	end
end

function clsUser:RestoreStatusData()
	local statusData = self.__save.status
	if statusData then
		local status = CLSSTATUS.clsStatus:New()
		status:Restore(statusData)
		self.__temp.status = status
	end
end




local s_status_info = function(conn, data)
	if data ~= "get" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local status = user:GetStatus()
	if not status then
		return ERROR.TipMsg(conn, "您尚未拥有仙职。")
	end
	local sData = status:GetStatusInfo()
	conn:SendProtocol("c_status_info", sData)
end

local s_status_attr = function(conn, data)
	if data ~= "get" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local status = user:GetStatus()
	if not status then
		return ERROR.TipMsg(conn, "您尚未拥有仙职。")
	end
	local sData = status:GetAttrAdd()
	conn:SendProtocol("c_status_attr", sData)
end

local s_status_gemList = function(conn, data)
	if data ~= "get" then
		return ERROR.TipMsg(conn, "协议错误，无效的请求类型。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local status = user:GetStatus()
	if not status then
		return ERROR.TipMsg(conn, "您尚未拥有仙职。")
	end
	local sData = status:GetFormatHoleList()
	conn:SendProtocol("c_status_gemList", sData)
end

local s_status_inject = function(conn, data)
	data = tonumber(data)
	if not data or data <= 0 then
		return ERROR.TipMsg(conn, "请输入要注入的灵气数量。")
	end
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local status = user:GetStatus()
	if not status then
		return ERROR.TipMsg(conn, "您尚未拥有仙职。")
	end
	local statusInfo = CLSSTATUS.GetStatusInfoByLv(status:GetLv())
	if status:GetNimbus() >= statusInfo.needNimbus then
		return ERROR.TipMsg(conn, "当前灵气已经注满。")
	end
	if status:GetLv() >= status:GetMaxLv() then
		return ERROR.TipMsg(conn, "当前仙印已经到达最高等级。")
	end
	user:SetNimbus(-data)
	status:SetNimbus(data)
	conn:SendProtocol("c_status_inject", status:GetNimbus())
	status:CheckUpgradeState()
end

local s_status_enchase = function(conn, data)
	local user = assert(USER.GetOnlineUser(conn.roleId))
	local status = user:GetStatus()
	if not status then
		return ERROR.TipMsg(conn, "您尚未拥有仙职。")
	end
	data = tonumber(data)
	local holeInfo = status:GetHoleInfo(data)
	if not holeInfo then
		return ERROR.TipMsg(conn, "请选择要镶嵌或升级宝石的孔位。")
	end
	local ret, msg
	if not holeInfo.id then
		ret, msg = status:Enchase(data)
	else
		ret, msg = status:UpgradeEnchased(data)
	end
	if not ret then
		return ERROR.TipMsg(conn, msg)
	end
	conn:SendProtocol("c_status_enchase", status:GetFormatHoleInfo(data))
--	PROTOCOL.s_status_gemList(conn, "get")
	PROTOCOL.s_status_attr(conn, "get")
end


function __init__()
	
	PROTOCOL.s_status_info = s_status_info
	PROTOCOL.s_status_attr = s_status_attr
	PROTOCOL.s_status_inject = s_status_inject
	
	PROTOCOL.s_status_gemList = s_status_gemList
	PROTOCOL.s_status_enchase = s_status_enchase

end
