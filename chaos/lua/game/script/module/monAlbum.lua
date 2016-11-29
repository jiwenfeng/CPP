--怪物图鉴


local AllMonAlbumItemList = {}

function GetMonAlnumItemList()
	return AllMonAlbumItemList
end

function GetMonAlbumItemById(id)
	return AllMonAlbumItemList[id]
end

-----------------------------------------------
local clsUser = USERBASE.clsUser

function clsUser:InitMonAlbumList()
	self.__temp.monAlbum = {}
	local album = CLSMONALBUM.GetMonAlbumInfoById(1)
	self.__temp.monAlbum[1] = CLSMONALBUM.NewMonAlbum(1, self:GetRoleId())
end

function clsUser:OpenNewMonAlbum()
	local nextIndex = #self:GetMonAlbumList() + 1
	local album = CLSMONALBUM.GetMonAlbumInfoById(nextIndex)
	self.__temp.monAlbum[nextIndex] = CLSMONALBUM.NewMonAlbum(nextIndex, self:GetRoleId())
end

function clsUser:GetMonAlbumList()
	return self.__temp.monAlbum
end

function clsUser:GetMonAlbumById(id)
	return self.__temp.monAlbum[id]
end


--炼妖阵属性加成
function clsUser:GetMonAlbumAttr()
	local sData = {}
	for k, v in pairs(self:GetMonAlbumList()) do
		for attr, num in pairs(v:GetRowAttr()) do
			sData[attr] = (sData[attr] or 0) + num
		end
		for attr, num in pairs(v:GetColAttr()) do
			sData[attr] = (sData[attr] or 0) + num
		end
	end
	return sData
end

function clsUser:SaveMonAlbumData()
	self.__save.monAlbum = {}
	for k, v in ipairs(self:GetMonAlbumList() or {}) do
		self.__save.monAlbum[k] = v:GetSaveData()
	end
end

function clsUser:RestoreMonAlbum()
	self.__temp.monAlbum = {}
	for k, v in pairs(CLSMONALBUM.GetAllMonAlbumList()) do
		local album = CLSMONALBUM.NewMonAlbum(k, self:GetRoleId())
		self.__temp.monAlbum[k] = album
		if self.__save.monAlbum and self.__save.monAlbum[k] then
			album:Restore(self.__save.monAlbum[k])
		end
	end
end

----------------------------------------------



function GetFormatItemList()
	local sData = {}
	for _, v in ipairs(GetMonAlnumItemList()) do
		local buff = {}
		buff.goodsId = v.goodsId
		local info = BASEITEM.GetItemById(v.goodsId)
		buff.goldType = info.needIngot and "ingot" or "gold"
		buff.price = info.needIngot
		table.insert(sData, buff)
	end
	return sData
end






local s_monAlbum_info = function(connection, data)
	local info = CLSMONALBUM.GetMonAlbumInfoById(data)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的图鉴。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local album = user:GetMonAlbumById(data)
	if not album then
		return ERROR.TipMsg(connection, "您尚未开启该图鉴。")
	end
	local sData = album:GetInfo(user)
	connection:SendProtocol("c_monAlbum_info", sData)
end

local s_monAlbum_turn = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议类型。")
	end
	if data.direct ~= "left" and data.direct ~= "right" then
		return ERROR.TipMsg(connection, "请选择要移动的方向。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local album = user:GetMonAlbumById(data.index)
	if not album then
		return ERROR.TipMsg(connection, "您尚未开启该图鉴。")
	end
	data.row = tonumber(data.row)
	if not data.row or data.row < 0 or data.row > album:GetMaxRow() then
		return ERROR.TipMsg(connection, "无效的行数。")
	end
	if data.direct == "left" then
		album:TurnLeft(data.row)
	else
		album:TurnRight(data.row)
	end
	connection:SendProtocol("c_monAlbum_turn", data)
end

local s_monAlbum_material = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议类型。")
	end	
	local info = CLSMONALBUM.GetMonAlbumInfoById(data.index)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的图鉴。")
	end
	data.pos = tonumber(data.pos)
	local monsterId = info[data.pos]
	if not data.pos or not monsterId then
		return ERROR.TipMsg(connection, "无效的指定位置。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local album = user:GetMonAlbumById(data.index)
	if not album then
		return ERROR.TipMsg(connection, "您尚未开启该图鉴。")
	end
	local sData = album:GetMaterialInfo(user, data.pos)
	connection:SendProtocol("c_monAlbum_material", sData)
end

local s_monAlbum_refine = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议类型。")
	end	
	local info = CLSMONALBUM.GetMonAlbumInfoById(data.index)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的图鉴。")
	end
	data.pos = tonumber(data.pos)
	local monsterId = info[data.pos]
	if not data.pos or not monsterId then
		return ERROR.TipMsg(connection, "无效的指定位置。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local album = user:GetMonAlbumById(data.index)
	if not album then
		return ERROR.TipMsg(connection, "您尚未开启该图鉴。")
	end
	local ret, msg = album:ProRefine(user, data.pos)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_monAlbum_refine", data)
end

local s_monAlbum_autoRefine = function(connection, data)
	if type(data) ~= "table" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议类型。")
	end	
	local info = CLSMONALBUM.GetMonAlbumInfoById(data.index)
	if not info then
		return ERROR.TipMsg(connection, "没有指定的图鉴。")
	end
	data.pos = tonumber(data.pos)
	local monsterId = info[data.pos]
	if not data.pos or not monsterId then
		return ERROR.TipMsg(connection, "无效的指定位置。")
	end
	local user = assert(USER.GetOnlineUser(connection.roleId))
	local album = user:GetMonAlbumById(data.index)
	if not album then
		return ERROR.TipMsg(connection, "您尚未开启该图鉴。")
	end
	local ret, msg = album:ProAutoRefine(user, data.pos)
	if not ret then
		return ERROR.TipMsg(connection, msg)
	end
	connection:SendProtocol("c_monAlbum_autoRefine", data)	
end


local s_monAlbum_itemList = function(connection, data)
	if data ~= "get" then
		return ERROR.TipMsg(connection, "协议错误，无效的协议类型。")
	end
	local sData = GetFormatItemList()
	connection:SendProtocol("c_monAlbum_itemList", sData)
end



function __init__()

	local MONALBUMSHOP = Import("autocode/monAlbumShop.lua")
	AllMonAlbumItemList = MONALBUMSHOP.GetTable()
	
	PROTOCOL.s_monAlbum_info = s_monAlbum_info
	PROTOCOL.s_monAlbum_turn = s_monAlbum_turn
	PROTOCOL.s_monAlbum_material = s_monAlbum_material
	
	PROTOCOL.s_monAlbum_refine = s_monAlbum_refine
	PROTOCOL.s_monAlbum_autoRefine = s_monAlbum_autoRefine
	
	PROTOCOL.s_monAlbum_itemList = s_monAlbum_itemList
	
end

