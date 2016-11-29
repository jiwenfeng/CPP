--buffer增益

local AllBufferList = {}

function GetBufferList()
	return AllBufferList
end

function GetBufferInfoById(id)
	return AllBufferList[id]
end

--获取buffer效果列表
function GetUserBufferList(user)
	local sData = {}
	local list = user:GetBufferEffectList()
	for k, v in pairs(list) do
		local info = GetBufferInfoById(k)
		if info then
			local buff = {}
			buff.buffId = k
			buff.name = info.name
--			buff.describe = info.describe
			buff.left = v.left
			buff.type = info.type
			buff.describe = string.format(info.describe, v.left)
			if v.endTime then
				buff.timelong = math.max(v.endTime - os.time(), 0)
			end
			table.insert(sData, buff)
		end
	end
	table.sort(sData, function(a, b) return list[a.buffId].timestamp < list[b.buffId].timestamp end)
	return sData
end

--更新buffer效果
function SendBufferUpdate(user, id, flag)
	local roleId = user:GetRoleId()
	local info = GetBufferInfoById(id)
	if not info then
		return
	end
	local sData = {buffId = id}
	local buffInfo = user:GetBufferEffect(id)
	sData.type = info.type
	sData.left = buffInfo.left
	if buffInfo.endTime then
		sData.timelong = buffInfo.endTime - os.time()
	end
	if flag then
		sData.name = info.name
	end
	if flag or buffInfo.left then
		sData.describe = string.format(info.describe, buffInfo.left)
	end
	local con = LOGIN.GetConnection(roleId)
	if con and con.roleId == roleId then
		con:SendProtocol("c_buffer_update", sData)
	end	
end

function IsSaveBufferExist(user, id)
	local info = GetBufferInfoById(id)
	for k, v in pairs(user:GetBufferEffectList()) do
		local buffInfo = GetBufferInfoById(k)
		if info.label == buffInfo.label then
			return k
		end
	end
end

--增加一个增益效果
function AddOneBuffer(user, id, num)
	local info = GetBufferInfoById(id)
	if not info then
		return
	end
	local same = IsSaveBufferExist(user, id)
	if same and not info.canAdd then
		RemoveOneBuffer(user, same)
	end
	user:SetBufferEffect(id, num or info.num, info.timelong)
	user:FreshTeamTotalAttr()		--buffer影响全部参战人员属性
	SendBufferUpdate(user, id, true)
end

--修改buffer耐久数值（仅对耐久型buffer有用）
function ModBufferEffectNum(user, id, num)
	local info = user:GetBufferEffect(id)
	if not info or not info.left then
		return
	end
	info.left = info.left + num
	if info.left <= 0 then
		return RemoveOneBuffer(user, id)
	end
	SendBufferUpdate(user, id)
end

function RemoveOneBuffer(user, id, flag)
	local info = user:GetBufferEffect(id)
	if not info then
		return
	end
	local buffInfo = GetBufferInfoById(id)
	if not buffInfo then
		return
	end
	if not flag then			--登录时移除buffer不用向客户端更新
		SendBufferUpdate(user, id)
		user:FreshTeamTotalAttr()		--buffer影响全部参战人员属性
	end
	user:DelBufferEffect(id)
end


local s_buffer_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, string.format("协议错误：无效的请求类型%s。", data or ""))
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = GetUserBufferList(user)
	return connection:SendProtocol("c_buffer_list", sData)
end

function __init__()

	local BUFFERLIST = Import("autocode/bufferList.lua")
	AllBufferList = BUFFERLIST.GetTable()

	PROTOCOL.s_buffer_list = s_buffer_list
end
