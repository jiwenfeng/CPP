--与中心服务器连接

-------------------------------------------------------

--客户端连接类
clsCon = SOCKET.clsConnection:Inherit()

function clsCon:__init__()
	Super(clsCon).__init__(self)
	return true
end



function clsCon:ReadCB(Stream, zip, pro)
	local SN, Msg = string.match(Stream, "^(%d+);(.*)")
	if tonumber(zip) == 1 then
		Msg = lengine.uncompress(Msg)
	end
	self:Process(SN, Msg, pro)
end

function clsCon:Process(SN, Msg, ProID)
	ProID = ProID and tonumber(ProID)
	if not ProID then
		print("gmSvr:ERROR no cmd", Msg)
		return
	end
--[[	local proName = PROTOCOL.GetProtocolNameById(ProID)
	if not proName then
		print("gmSvr:ERROR no proName", ProID)
		return
	end]]
	local Data = lengine.jsondecode(Msg)
	return GAMEMANAGE.ProGmFuncById(ProId, self, SN, Data)
end


function clsCon:OnError(flag)  --断开连接处理函数
	if not self.roleId then
		return
	end
end



function clsCon:SendProtocol(proId, Arg, debug)
	if not self:CheckSocket() then
		return
	end
	Arg = JSON.encode(Arg)
	CData = lengine.compress(Arg)
	local OutData = CData
	local IsZip = 1
	if string.len(Arg) < string.len(CData) then
		OutData = Arg
		IsZip = 0
	end
	return self:Write(OutData, IsZip, proId)
end





--监听服务器类
clsSer = SOCKET.clsServer:Inherit()
function clsSer:OnAccept()
	local Connector, IP = self:Accept(clsCon)
	Connector.IP = IP
end

Server = clsSer:New("*", PORT_GM_SERVER)

