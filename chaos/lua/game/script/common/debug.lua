
clsDebug = SOCKET.clsConnection:Inherit()


local EnvTbl = {}			--创建一个新的环境
setmetatable(EnvTbl, {__index = _G})		--新的环境继承旧环境_G的数据

function clsDebug:ReadCB(Stream)
	local function RunCmd( Cmd )
      local Fun, ErrMsg = loadstring(Cmd)
		assert(Fun, ErrMsg)
		return { setfenv(Fun, EnvTbl) () }			--设置Fun的运行环境为新环境EnvTbl
	end

	local Ok, Result = pcall(RunCmd, Stream )
	self:WritePolicy(string.format("ret = %s!\n%s\n", tostring(Ok), FormatSerialize(Result)))
end


clsServer = SOCKET.clsServer:Inherit()
function clsServer:OnAccept()
	local Connector, IP = self:Accept(clsDebug)
	Connector.IP = IP
end

--Server = clsServer:New("220.162.247.140", PORT_DEBUG)
Server = clsServer:New("*", PORT_DEBUG)
