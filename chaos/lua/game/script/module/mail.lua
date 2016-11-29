--邮件系统

local path_mail = "chaos.mail"

local MAIL_SAVE_TIMELONG = 15 * 24 * 3600		--邮件保存时间
local MAIL_SAVE_TOTAL_NUM = 30					--邮件保存数量上限
MAIL_ATTACH_NUM = 5						--最大附件数量

local MAIL_TITLE_LEN = 30
local MAIL_CONTENT_LEN = 300

--初始化邮件数据库(建立索引)
function InitMailDataBase()
	DATA.EnsureIndex(path_mail, {to = 1})		--用于读取离线邮件
	DATA.EnsureIndex(path_mail, {time = 1})		--用于定时清理过期邮件(saved/script/base/DBManage.lua)
end


--添加离线邮件
function AddOfflineMail(roleId, mail)
	local id = STATS.GenId("mail")			--统一Id生成器
	local buff = {}
	buff.to = roleId
	buff.title = mail.title
	buff.time = mail.time
	buff.roleId = mail.roleId
	buff.type = mail.type
	buff.content = mail.content
	buff.attach = mail.attach
	buff._id = id
	if not DATA.Write(path_mail, {_id = id}, buff) then
		DATA.log(string.format("Save offline mail %s failed: connect saved failed!", id), "mail.log")
		return
	end
end

function DelOfflineMail(id)
	DATA.DBRemove(path_mail, {_id = id})
end

--玩家发送邮件
function SendMail(from, to, title, content)
	local newMail = {state = 1}
	newMail.type = "person"
	newMail.title = title
	newMail.time = os.time()
	newMail.roleId = from
	newMail.content = NEWUSER.TranslateLegalStr(content)
	local user = USER.GetOnlineUser(to)
	if user and user:GetLoginState() then
		user:AddNewMail(newMail)
	else
		AddOfflineMail(to, newMail)
	end
end

--发送系统邮件
--mail.attach = {gold = , nimbus = , ingot = , voucher = , exp = , item = {{}, {}, ...}}
function SendSysMail(roleId, mail)
	local newMail = {state = 1}
	newMail.title = mail.title
	newMail.time = mail.time or os.time()
	newMail.roleId = mail.roleId or SYSTEM_ID
	newMail.type = mail.type or "sys"	--默认为系统邮件  fight/sys
	newMail.content = mail.content
	if mail.attach then
		if table.size(mail.attach) > MAIL_ATTACH_NUM then
			return false, string.format("一封邮件最多携带%d个附件", MAIL_ATTACH_NUM)
		end
		newMail.attach = {}
		local index = 0
		for k, v in pairs(mail.attach) do
			index = index + 1
			local moneyInfo = MONEY.GetGoldInfoByType(k)
			if moneyInfo then
				newMail.attach[tostring(index)] = {id = moneyInfo.picId, num = v}
			--	table.insert(newMail.attach, {index = #newMail.attach + 1, id = moneyInfo.picId, num = v})
			elseif k == "item" then
				for key, val in pairs(v) do
					local buff = table.copy(val)
				--	buff.index = #newMail.attach + 1
					buff.lock = val.lock or 0
					buff.missionId = val.missionId
					if val.save then
						buff.save = val.save
					end
					newMail.attach[tostring(index)] = buff
			--		table.insert(newMail.attach, buff)
				end
			end
		end
	end
    local user = USER.GetOnlineUser(roleId)
    if user and user:GetLoginState() then
		user:AddNewMail(newMail)
	else
		AddOfflineMail(roleId, newMail)
	end
	return true
end

function FormatMail(id, mail)
	local sData = {}
	sData.title = mail.title
	sData.type = mail.type
	sData.state = mail.state
	sData.id = id
	sData.time = mail.time
	sData.nick = USER.GetUserNickById(mail.roleId)
	local index, attach = next(mail.attach or {})
	if attach then
		sData.picId = attach.id
	end
	return sData
end
--获取邮件内容及附件
function GetMailDetail(user, id)
	local mail = user:GetMailById(id)
	if not mail then
		return
	end
	local sData = {id = id, state = mail.state, content = mail.content}
	if mail.attach then
		sData.attach = {}
		for k, v in pairs(mail.attach) do
			if not v.save then
				table.insert(sData.attach, {index = tonumber(k), goodsId = v.id, num = v.num})
			else
				local item = BASEITEM.NewItem(v.id, v.num, {save = v.save})
				local buff = item:GetFullAttr()
			--	buff.lock = nil
				buff.index = tonumber(k)
				buff.num = v.num
				table.insert(sData.attach, buff)
				item:Destroy()
			end
		end
	end
	return sData
end

--玩家登录时获取离线邮件
function GetOfflineMail(user)
	local roleId = user:GetRoleId()
	local function add_mail_back(data, msg)
		if not data then
			DATA.log(string.format("%s读取离线邮件失败：%s", roleId, msg or ""), "mail.log")
			return
		end
		data = UnSerialize(data)
		local needNotice = false
		for k, v in ipairs(data) do
			user:AddNewMail({title = v.title, time = v.time, roleId = v.roleId, content = v.content, type = v.type, state = 1, attach = v.attach})
			needNotice = true
			DATA.DBRemove(path_mail, {_id = v._id})
		end
		local sData = {uplimit = MAIL_SAVE_TOTAL_NUM, list = {}}
		for k, v in pairs(user:GetMailList()) do
			table.insert(sData.list, FormatMail(k, v))
		end
		table.sort(sData.list, function(a, b) return a.time < b.time end)
		local con = LOGIN.GetConnection(roleId)
		if con and con.roleId == roleId then
			con:SendProtocol("c_mail_list", sData)
			if needNotice then
				con:SendProtocol("c_mail_new", {})
			end
		end
		user:SetClientMailInit(true)	--设定客户端邮件初始化完毕
	end
	if not DATA.Read(path_mail, {to = roleId}, add_mail_back) then
		DATA.log(string.format("Read user %s offlineMail failed: connect server Saved error!", roleId), "mail.log")
	end
end


--------------------------------------------------------
local clsUser = USERBASE.clsUser

function clsUser:SetClientMailInit(state)
	self.__temp.mail = self.__temp.mail or {}
	self.__temp.mail.clientInit = state
end

function clsUser:GetClientMailInit()
	self.__temp.mail = self.__temp.mail or {}
	return self.__temp.mail.clientInit
end

function clsUser:InitUserMail()
	self.__save.mail = {}
	self.__save.mail.count = 0
	self.__save.mail.list = {}
end

function clsUser:GenMailId()
	self.__save.mail.count = self.__save.mail.count + 1
	return tostring(self.__save.mail.count)
end

function clsUser:AddNewMail(mail)
	local id = self:GenMailId()
	self.__save.mail.list[id] = mail
	self:SendNewMail(id, mail)
end


function clsUser:GetMailNum()
	return table.size(self.__save.mail.list)
end

function clsUser:SendNewMail(id, mail)
	local state = self:GetClientMailInit()
	if not state then
		return
	end
	local con = LOGIN.GetConnection(self:GetRoleId())
	if con and con.roleId == self:GetRoleId() then
		local sData = FormatMail(id, mail)
		con:SendProtocol("c_mail_new", sData)
		if self:GetMailNum() > MAIL_SAVE_TOTAL_NUM then
			ERROR.TipMsg(con, string.format("您的邮件数目已达上限%d封, 多余的邮件会在下线的时候被系统自动清理。", MAIL_SAVE_TOTAL_NUM))
		end
	end
end

function clsUser:GetMailList()
	return self.__save.mail.list or {}
end

function clsUser:GetMailById(id)
	id = tostring(id)
	return self.__save.mail.list[id]
end

function clsUser:DelMailById(id)
	id = tostring(id)
	local mail = self:GetMailById(id)
	if not mail then
		return
	end
	self.__save.mail.list[id] = nil
end

--上线清理邮件
function clsUser:LoginClearMail()
	local curTime = os.time()
	local withAttach = {}
	local withoutAttach = {}
	for k, v in pairs(self.__save.mail.list or {}) do
		if curTime - v.time >= MAIL_SAVE_TIMELONG then
			self.__save.mail.list[k] = nil
		else
			if v.attach then
				table.insert(withAttach, k)
			else
				table.insert(withoutAttach, k)
			end
		end
	end	
	if #withAttach > MAIL_SAVE_TOTAL_NUM then
		local rmList = {}
		table.sort(withAttach, function(a, b) return self.__save.mail.list[a].time > self.__save.mail.list[b].time end)
		for i = MAIL_SAVE_TOTAL_NUM + 1, #withAttach do
			self:DelMailById(withAttach[i])
		end
		for k in pairs(withoutAttach) do
			self:DelMailById(k)
		end
	elseif #withAttach + #withoutAttach > MAIL_SAVE_TOTAL_NUM then
		table.sort(withoutAttach, function(a, b) return self.__save.mail.list[a].time > self.__save.mail.list[b].time end)
		for i = MAIL_SAVE_TOTAL_NUM - #withAttach + 1, #withoutAttach do
			self:DelMailById(withoutAttach[i])
		end
	end
end

--提取
function clsUser:FetchAttach(mail)
	mail.state = 0
	for k in pairs(mail.attach or {}) do
		local ret, msg = self:FetchOneElement(mail, k)
		if not ret then
			return false, msg
		end
	end
	return true
end

--提取单个附件
function clsUser:FetchOneElement(mail, index)
	index = tostring(index)
	local elemet = mail.attach[index]
	if not elemet then
		return false, "指定的附件不存在。"
	end
	local moneyType = MONEY.GetGoldTypeByPicId(elemet.id)
	if moneyType then
		local moneyInfo = MONEY.GetGoldInfoByType(moneyType)
		clsUser[moneyInfo.set](self, elemet.num)
	else
		local info = BASEITEM.GetItemById(elemet.id)
		if info then
			local item = BASEITEM.NewItem(elemet.id, elemet.num, {lock = elemet.lock, missionId = elemet.missionId, save = elemet.save})
			if not self:IsItemCanAdd(USERBASE.PACKAGE, item) then
				return false, "背包容量不足，无法完成提取。"
			end
			self:AddItem(USERBASE.PACKAGE, item)
		end
	end
	mail.attach[index] = nil
	if #mail.attach then
		mail.attach = nil
	end
	return true
end

--------------------------------------------------------
local s_mail_list = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	return GetOfflineMail(user)
end

local s_mail_detail = function(connection, data)
	if not data then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local mail = user:GetMailById(data)
	if not mail then
		return ERROR.TipMsg(connection, "没有指定的邮件信息。")
	end
	mail.state = 0
	local sData = GetMailDetail(user, data)
	connection:SendProtocol("c_mail_detail", sData)
end

--提取
local s_mail_fetch = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "无效的请求")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local sData = {list = {}}
	for k, v in ipairs(data) do
		local mail = user:GetMailById(v)
		if mail then
			local ret, msg = user:FetchAttach(mail)
			if ret then
				table.insert(sData.list, v)
			else
				sData.id = v
				sData.msg = msg
				local _, attach = next(mail.attach or {})
				if attach then
					sData.picId = attach.id
				end
				break
			end
		end
	end
	connection:SendProtocol("c_mail_fetch", sData)
end

local s_mail_fetchOne = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local mail = user:GetMailById(data.id)
	if not mail then
		return ERROR.TipMsg(connection, "指定的邮件不存在。")
	end
	local ret, msg = user:FetchOneElement(mail, data.index)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	local sData = {index = data.index, id = data.id}
	local index, attach = next(mail.attach or {})
	if attach then
		sData.picId = attach.id
	end
	connection:SendProtocol("c_mail_fetchOne", sData)
end

--删除
local s_mail_del = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求")
	end
	if #data <= 0 then
		return ERROR.TipMsg(connection, "请选择要删除的邮件。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	for _, v in ipairs(data) do
		user:DelMailById(v)
	end
	connection:SendProtocol("c_mail_del", data)
end

local s_mail_send = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的请求。")
	end
	if not data.nick then
		return ERROR.TipMsg(connection, "请选择收件人。")
	end
	local roleId = USER.GetRoleIdByNick(data.nick)
	if not roleId then
		return ERROR.TipMsg(connection, "该收件人不存在。")
	end
	if not data.title or not data.content then
		return ERROR.TipMsg(connection, "请输入邮件标题及内容。")
	end
	if string.len(data.title) > MAIL_TITLE_LEN then
		return ERROR.TipMsg(connection, string.format("邮件标题不能超过%d个字符。", MAIL_TITLE_LEN))
	end
	if string.len(data.content) > MAIL_CONTENT_LEN then
		return ERROR.TipMsg(connection, string.format("邮件内容不能超过%d个字符。", MAIL_CONTENT_LEN))
	end
	SendMail(connection.roleId, roleId, data.title, data.content)
	connection:SendProtocol("c_mail_send", data.nick)
end

function __init__()
	
	PROTOCOL.s_mail_list = s_mail_list
	PROTOCOL.s_mail_detail = s_mail_detail
	PROTOCOL.s_mail_fetch = s_mail_fetch
	PROTOCOL.s_mail_fetchOne = s_mail_fetchOne
	PROTOCOL.s_mail_del = s_mail_del
	
	PROTOCOL.s_mail_send = s_mail_send
end

