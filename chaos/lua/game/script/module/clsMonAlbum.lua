--炼妖系统 (Album 怪物图鉴)

local COL_NUM = 3		--列数
local ROW_NUM = 3		--行数
local STATE_DOING = 0	--未炼化
local STATE_DONE = 1	--已炼化
local NIMBUS_TO_INGOT = 1 / 1000

local rowAttrList = {"speed", "physcDef", "magicDef"}
local colAttrList = {"physcAtk", "magicAtk", "hpMax"}

----------------------------------------------------------------------

local AllMonAlbumList = {}

function GetAllMonAlbumList()
	return AllMonAlbumList
end

function GetMonAlbumInfoById(id)
	return AllMonAlbumList[id]
end


local AllMonAlbumMaterialList = {}

function GetMonAlbumMaterialList()
	return AllMonAlbumMaterialList
end

function GetMonAlbumMaterialById(id)
	return AllMonAlbumMaterialList[id]
end

function GetAutoRefineCost(user, monsterId)
	local monsterInfo = GetMonAlbumMaterialById(monsterId)
	local ingot = 0
	for _, v in ipairs(monsterInfo.material) do
		local num = user:GetItemNum(USERBASE.PACKAGE, v.goodsId)
		local itemInfo = BASEITEM.GetItemById(v.goodsId)
		if num < v.num then
			ingot = ingot + itemInfo.needIngot * (v.num - num)
		end
	end
	local nimbus = user:GetNimbus()
	if nimbus < monsterInfo.needNimbus then
		ingot = ingot + (monsterInfo.needNimbus - nimbus) * NIMBUS_TO_INGOT
	end
	return ingot
end
--------------------------------------------------------------------------
local AllMonAlbumAttr = {}

function GetMonAlbumAttr(index, type)
	return AllMonAlbumAttr[index][type]
end

--------------------------------------------------------------------------
--图鉴格子
local clsGrid = clsObject:Inherit()

function clsGrid:__init__()
	Super(clsGrid).__init__(self)
	self.__save = {}
	self.__temp = {}
	return true
end


function clsGrid:InitGrid(id)
	self:SetId(id)
	self:SetState(STATE_DOING)
end

function clsGrid:SetId(id)
	self.__save.id = id
end

function clsGrid:GetId()
	return self.__save.id
end

function clsGrid:SetState(state)
	self.__save.state = state
end

function clsGrid:GetState()
	return self.__save.state
end


function clsGrid:GetSaveData()
	return self.__save
end

function clsGrid:Restore(data)
	assert(type(data) == "table")
	self.__save = data
end

function NewGrid(monsterId)
	local grid = clsGrid:New()
	grid:InitGrid(monsterId)
	return grid
end
--------------------------------------------------------------------------
--单页的图鉴
clsAlbum = clsObject:Inherit()

function clsAlbum:__init__()
	Super(clsAlbum).__init__(self)
	self.__save = {}
	self.__temp = {}
	self:initsavedata()
	self:inittempdata()
	return true
end


function clsAlbum:initsavedata()
	self.__save.id = 0
end

function clsAlbum:inittempdata()	
	self.__temp.list = {}
end

function clsAlbum:InitAlbum(id, owner)
	self:SetId(id)
	self:SetOwner(owner)
	
	self:InitGridList()
end

-- id
function clsAlbum:SetId(id)
	self.__save.id = id
end

function clsAlbum:GetId()
	return self.__save.id
end

--属主
function clsAlbum:SetOwner(owner)
	self.__save.owner = owner
end

function clsAlbum:GetOwner()
	return self.__save.owner
end

--格子
function clsAlbum:InitGridList()
	for k, v in ipairs(GetMonAlbumInfoById(self:GetId())) do
		local grid = NewGrid(v)
		self:SetGrid(k, grid)
	end
end

function clsAlbum:SetGrid(pos, grid)
	self.__temp.list[tostring(pos)] = grid
end

function clsAlbum:GetGrid(pos)
	return self.__temp.list[tostring(pos)]
end

function clsAlbum:GetGridList()
	return self.__temp.list
end

function clsAlbum:GetMaxRow()
	return ROW_NUM
end

function clsAlbum:GetMaxCol()
	return COL_NUM
end

--向左移动
function clsAlbum:TurnLeft(rowNum)
	local base = (rowNum - 1) * COL_NUM
	local grid = self:GetGrid(base + 1)
	for i = 1, COL_NUM - 1 do
		self:SetGrid(base + i, self:GetGrid(base + i + 1))
	end
	self:SetGrid(base + COL_NUM, grid)
	self:SendAblumUpdate("colAttr")
end

--向右移动
function clsAlbum:TurnRight(rowNum)
	local base = (rowNum - 1) * COL_NUM
	local grid = self:GetGrid(base + COL_NUM)
	for i = COL_NUM, 2, -1 do
		self:SetGrid(base + i, self:GetGrid(base + i - 1))
	end
	self:SetGrid(base + 1, grid)
	self:SendAblumUpdate("colAttr")
end

--获取行属性
function clsAlbum:GetRowAttr(isArray)
	local attrList = GetMonAlbumAttr(self:GetId(), "row")
	local sData = {}
	for i = 1, #attrList do
		local sum = 0
		local base = (i - 1) * COL_NUM
		for j = 1, COL_NUM do
			local grid = self:GetGrid(base + j)
			if grid:GetState() == STATE_DONE then
				sum = sum + 1
			end
		end
		if isArray then
			table.insert(sData, {type = attrList[i].type, num = attrList[i].num * (sum / COL_NUM)})
		else
			sData[attrList[i].type] = attrList[i].num * (sum / COL_NUM)
		end
	end
	return sData
end

--获取列属性
function clsAlbum:GetColAttr(isArray)
	local attrList = GetMonAlbumAttr(self:GetId(), "col")
	local sData = {}
	for i = 1, #attrList do
		local sum = 0
		for j = 1, ROW_NUM do
			local grid = self:GetGrid((j - 1) * COL_NUM + i)
			if grid:GetState() == STATE_DONE then
				sum = sum + 1
			end
		end
		if isArray then
			table.insert(sData, {type = attrList[i].type, num = attrList[i].num * (sum / ROW_NUM)})
		else
			sData[attrList[i].type] = attrList[i].num * (sum / ROW_NUM)
		end
	end
	return sData
end

function clsAlbum:GetSaveData()
	self.__save.list = {}
	for k, v in pairs(self:GetGridList()) do
		self.__save.list[k] = v:GetSaveData()
	end
	return self.__save
end

function clsAlbum:Restore(data)
	self.__save = data
	self:InitGridList()
	for k, v in pairs(self.__save.list) do
		local grid = self:GetGrid(k)
		grid:Restore(v)
	end
end

function clsAlbum:Refine(pos)
	local grid = self:GetGrid(pos)
	grid:SetState(STATE_DONE)
	self:SendAblumUpdate("pos", pos)
	self:SendAblumUpdate("rowAttr")
	self:SendAblumUpdate("colAttr")
end

--是否已完成本页所有炼化
function clsAlbum:IsAllDone()
	for k, v in pairs(self:GetGridList()) do
		local grid = self:GetGrid()
		if not grid or grid:GetState() ~= STATE_DONE then
			return false
		end
	end
	return true
end

local updateList = {
	["pos"] = function(album, pos)
		local grid = album:GetGrid(pos)
		local sData = {}
		sData.index = album:GetId()
		sData.pos = pos
		sData.state = grid:GetState()
		return sData
	end,
	["rowAttr"] = function(album)
		local sData = {}
		sData.index = album:GetId()
		sData.rowAttr = album:GetRowAttr(true)
		return sData
	end,
	["colAttr"] = function(album)
		local sData = {}
		sData.index = album:GetId()
		sData.colAttr = album:GetColAttr(true)
		return sData
	end,
}

function clsAlbum:SendAblumUpdate(uType, pos)
	local uFunc = updateList[uType]
	if not uFunc then
		return
	end
	local sData = uFunc(self, pos)
	local owner = self:GetOwner()
	local con = LOGIN.GetConnection(owner)
	if con and con.roleId == owner then
		con:SendProtocol("c_monAlbum_update", sData)
	end
end

function clsAlbum:GetInfo(user)
	local sData = {}
	sData.index = self:GetId()
	sData.maxIndex = #user:GetMonAlbumList()
	sData.gridList = {}
	for i = 1, COL_NUM * ROW_NUM do
		local pos = tostring(i)
		local grid = self:GetGrid(i)
		local buff = {}
		buff.pos = i
		buff.state = grid:GetState()
		buff.monsterId = grid:GetId()
		local monsterInfo = GetMonAlbumMaterialById(buff.monsterId)
		if monsterInfo then
--			buff.value = monsterInfo.value
			buff.name = monsterInfo.name
		end
		table.insert(sData.gridList, buff)
	end
	
--[[	for k, v in ipairs(GetMonAlbumInfoById(self:GetId())) do
		local buff = {}
		buff.pos = k
		local grid = self:GetGrid(k)
		buff.state = grid:GetState()
		buff.monsterId = v
		local monsterInfo = GetMonAlbumMaterialById(v)
		if monsterInfo then
--			buff.value = monsterInfo.num
			buff.name = monsterInfo.name
		end
		table.insert(sData.gridList, buff)
	end]]
	sData.colAttr = self:GetColAttr(true)
	sData.rowAttr = self:GetRowAttr(true)
	return sData
end

function clsAlbum:GetMaterialInfo(user, pos)
	local sData = {}
	sData.index = self:GetId()
	sData.pos = pos
	local grid = self:GetGrid(pos)
	sData.state = grid:GetState()
	local monsterId = grid:GetId()
	sData.monsterId = monsterId
	local monsterInfo = GetMonAlbumMaterialById(monsterId)
	if monsterInfo then
		sData.needNimbus = monsterInfo.needNimbus
		sData.material = monsterInfo.material
	end
	sData.autoCost = GetAutoRefineCost(user, monsterId)
	return sData
end

function clsAlbum:ProRefine(user, pos)
	local grid = self:GetGrid(pos)
	if not grid then
		return false, "没有指定的格子位置。"
	end
	if grid:GetState() == STATE_DONE then
		return false, "该妖物已经被成功炼化。"
	end
	local monsterInfo = GetMonAlbumMaterialById(grid:GetId())
	if not monsterInfo then
		return false, "没有指定的妖物信息。"
	end
	if user:GetNimbus() < monsterInfo.needNimbus then
		return false, "灵气不足。"
	end
	for _, v in ipairs(monsterInfo.material) do
		if user:GetItemNum(USERBASE.PACKAGE, v.goodsId) < v.num then
			local itemInfo = BASEITEM.GetItemById(v.goodsId)
			return false, string.format("炼化所需的【%s】数量不足。", itemInfo.name)
		end
	end
	for _, v in ipairs(monsterInfo.material) do
		user:DelItem(USERBASE.PACKAGE, v.goodsId, v.num)
	end
	self:Refine(pos)
	if self:IsAllDone() then
		user:OpenNewMonAlbum()
	end
	return true
end

--一键炼化
function clsAlbum:ProAutoRefine(user, pos)
	local grid = self:GetGrid(pos)
	if not grid then
		return false, "没有指定的格子位置。"
	end
	if grid:GetState() == STATE_DONE then
		return false, "该妖物已经被成功炼化。"
	end
	local monsterInfo = GetMonAlbumMaterialById(grid:GetId())
	if not monsterInfo then
		return false, "没有指定的妖物信息。"
	end
	if not VIP.IsVipFuncValid(user:GetVipLv(), VIP.VIP_MONALBUM_AUTO) then
		return false, "您的vip等级不足。"
	end	
	local needIngot, needNimbus = 0, 0
	local rmList = {}
	for _, v in ipairs(monsterInfo.material) do
		local num = user:GetItemNum(USERBASE.PACKAGE, v.goodsId)
		local itemInfo = BASEITEM.GetItemById(v.goodsId)
		if num < v.num then
			needIngot = needIngot + itemInfo.needIngot * (v.num - num)
		end
		table.insert(rmList, {goodsId = v.goodsId, num = math.min(v.num, num)})
	end
	local nimbus = user:GetNimbus()
	if nimbus < monsterInfo.needNimbus then
		needIngot = needIngot + (monsterInfo.needNimbus - nimbus) * NIMBUS_TO_INGOT
	end
	needNimbus = math.min(nimbus, monsterInfo.needNimbus)
	if user:GetTotalIngot() < needIngot then
		return false, "您的元宝不足，请充值。"
	end
	for _, v in ipairs(rmList) do
		user:DelItem(USERBASE.PACKAGE, v.goodsId, v.num)
	end
	user:SetNimbus(-needNimbus)
	user:SetTotalIngot(-needIngot)
	self:Refine(pos)
	if self:IsAllDone() then
		user:OpenNewMonAlbum()
	end
	return true
end


function NewMonAlbum(index, owner)
	local album = clsAlbum:New()
	album:InitAlbum(index, owner)
	return album
end



function __init__()
	
	local MONALBUMINFO = Import("autocode/monAlbumGrid.lua")
	AllMonAlbumList = MONALBUMINFO.GetTable()
	
	local MONALBUMATTR = Import("autocode/monAlbumAttr.lua")
	AllMonAlbumAttr = MONALBUMATTR.GetTable()
	
	local MONALBUMMATERIAL = Import("autocode/monAlbumMaterial.lua")
	AllMonAlbumMaterialList = MONALBUMMATERIAL.GetTable()
	
end