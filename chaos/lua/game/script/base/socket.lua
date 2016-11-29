--$Id: socket.lua 70958 2009-02-11 10:39:53Z tony $
--lua socket封装，create by tony Tue 21 Oct 2008 11:58:33 CST
--用于监听的socket类:clsServer
--用于处理连接对象的socket类:clsConnection
--基类clsBaseSocket不对外开放
--引擎库netlib不建议直接使用，引擎跟脚本使用fd交互，为了简单没有使用userdata

_AllSocketWeakTbl = {}
setmetatable(_AllSocketWeakTbl, {__mode="v"})

function GetTotalSocket()
	return table.size(_AllSocketWeakTbl)
end

clsBaseSocket = clsObject:Inherit()
function clsBaseSocket:GetFd()
	return self.SocketFd
end
--创建socket
function clsBaseSocket:CreateSocket()
	assert(not self.SocketFd)
	local SocketFd, ErrMsg = netlib.socket()
	assert(SocketFd, ErrMsg)
	self.SocketFd = SocketFd
	_AllSocketWeakTbl[SocketFd] = self
end

function clsBaseSocket:CheckSocket()
	if not self:GetFd() then
		return false
	end
	return netlib.IsSocketfd(self:GetFd())
end

function clsBaseSocket:Close()
	if self:CheckSocket() then
		netlib.close(self.SocketFd)
	end
	self.SocketFd = nil
end

--Socket 连接对象
clsConnection = clsBaseSocket:Inherit()
--读入数据
function clsConnection:ReadCB(Data, zip, pro)
end
--write buffer 已经清空
--表示数据已经成功写出去，或者连接已经建立，可以写数据
function clsConnection:WriteCB()
end

function clsConnection:OnError(Event)
	self.Error = Event
end

function clsConnection:IsError()
	return self.Error
end

function clsConnection:Write(Data, zip, pro)
	return netlib.write(self:GetFd(), Data, zip, pro)
end

function clsConnection:WritePolicy(Data)
	return netlib.writePolicy(self:GetFd(), Data)
end
--accept返回的连接对象不应该具有Connect方法，但是实在不好细分了。。。
function clsConnection:Connect(Ip, Port)
	local function _ReadCB(Fd, Data, zip, pro)
		if Fd ~= self:GetFd() then
			_RUNTIME_ERROR(string.format("socket read error: real fd=%d, bad fd:%d", Fd, self:GetFd()))
		end
		return self:ReadCB(Data, zip, pro)
	end
	local function _WriteCB(Fd)
		if Fd ~= self:GetFd() then
			_RUNTIME_ERROR(string.format("socket write error: real fd=%d, bad fd:%d", Fd, self:GetFd()))
		end
		return self:WriteCB()
	end
	local function _OnError(Fd, Event)
		if Fd ~= self:GetFd() then
			_RUNTIME_ERROR(string.format("socket error_hook error: read fd = %d, bad fd:%d", Fd, self:GetFd()))
		end
		return self:OnError(Event)
	end
	return netlib.connect(self:GetFd(), Ip, Port, _ReadCB, _WriteCB, _OnError)
end

--初始化一个监听端口，传入监听IP和端口，IP可以用"*"表示所有IP
clsServer = clsBaseSocket:Inherit()
function clsServer:__init__(Ip, Port)
	Super(clsServer).__init__(self)
	self:CreateSocket()

	local BindOk, ErrMsg = netlib.bind(self:GetFd(), Ip, Port)
	if not BindOk then
		netlib.close(self:GetFd())
	end
	assert(BindOk, string.format("%s:%s", Port, ErrMsg or "OK"))

	local function _DoAccept(Fd)
		return self:OnAccept()
	end
	local ListenOk, ErrMsg = netlib.listen(self:GetFd(), _DoAccept)
	if not ListenOk then
		netlib.close(self:GetFd())
	end
	assert(ListenOk, ErrMsg)
	return true
end

--建立一个连接，传入连接Class，返回连接对象以及客户端IP
function clsServer:Accept(ClientType)
	--创建一个空的clientobj
	local ClientObj = ClientType:New()
	if (ClientObj:GetFd()) then
		ClientObj:Close()
		_RUNTIME_ERROR(string.format("call Accept with a established socket : %s", debug.traceback()))
	end
	local function _ReadCB(Fd, Data, zip, pro)
		if Fd ~= ClientObj:GetFd() then
			_RUNTIME_ERROR(string.format("socket read error: real fd=%d, bad fd:%d", Fd, ClientObj:GetFd()))
		end
		return ClientObj:ReadCB(Data, zip, pro)
	end
	local function _WriteCB(Fd)
		if Fd ~= ClientObj:GetFd() then
			_RUNTIME_ERROR(string.format("socket write error: real fd=%d, bad fd:%d", Fd, ClientObj:GetFd()))
		end
		return ClientObj:WriteCB()
	end
	local function _OnError(Fd, Event)
		if Fd ~= ClientObj:GetFd() then
			_RUNTIME_ERROR(string.format("socket error_hook error: read fd = %d, bad fd:%d", Fd, ClientObj:GetFd()))
		end
		return ClientObj:OnError(Event)
	end

	local ClientFd, ErrMsg = netlib.accept(self:GetFd(), _ReadCB, _WriteCB, _OnError)
	if not ClientFd then
		self:OnError(ErrMsg)
		return nil
	end
	ClientObj.SocketFd = ClientFd
	_AllSocketWeakTbl[ClientFd] = ClientObj
	local ClientIP = ErrMsg
	return ClientObj, ClientIP
end

--当有新的连接时回调，应该在此函数中调用 Accept函数创建连接
function clsServer:OnAccept()
end

function clsServer:OnError(ErrMsg)
	--self:Close()
	print("socket server error:", ErrMsg)
end
