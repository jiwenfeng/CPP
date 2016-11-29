clsMessageClient = SOCKET.clsConnection:Inherit()
local EnvTbl = {}
setmetatable(EnvTbl, {__index = _G})
local function RunCmd( Cmd )
	local Fun, ErrMsg = loadstring(Cmd)
	assert(Fun, ErrMsg)
	return { setfenv(Fun, EnvTbl) () }
end

function clsMessageClient:ReadCB(Message)
--[[	if self.ClientIp ~= "127.0.0.1" then
		_RUNTIME_ERROR("base/debug.lua: INVALID IP from", self.ClientIp)
		return self:Close()
	end]]
	Cmd = string.strip(Message)
	local Ok, Result = pcall(RunCmd, Cmd )
	Result = sys.dump(Result)
	if Ok then
		return self:WritePolicy(Result .. "\n>")
	end
	self:WritePolicy("ERROR:" .. Result .. "\n>")
end

clsMessageServer = SOCKET.clsServer:Inherit()
function clsMessageServer:OnAccept()
	local ClientObj, Ip = self:Accept(clsMessageClient)
	ClientObj.ClientIp = Ip
end

ServerObj = clsMessageServer:New("192.168.0.20", PORT_DEBUG)
