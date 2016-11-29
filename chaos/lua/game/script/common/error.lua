--错误处理函数
function ErrorMsg(connection, errmsg)
	return connection:SendProtocol("c_show_msg", errmsg)
end
--浮动消息提示
function TipMsg(connection, msg)
	return connection:SendProtocol("c_show_tips", msg)
end


