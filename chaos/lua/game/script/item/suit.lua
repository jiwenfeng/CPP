--套装

local AllSuitList = {}

function GetAllSuitList()
	return AllSuitList
end

function GetSuitInfoById(id)
	return AllSuitList[id]
end

--获取套装组建id列表
function GetSuitElementById(id, job)
	local info = GetSuitInfoById(id)
	if not info then
		return
	end
	if not job then
		return info.list
	end
	local buff = {}
	for k, v in ipairs(info.list) do
		if v.id then
			table.insert(sData.element, v.id)
		else
			table.insert(sData.element, v[job].id)
		end
	end
	return buff
end

--格式化套装描述
function GetFormatedSuitDesc(id)
	local info = GetSuitInfoById(id)
	local sData = {goodsId = id}
	sData.name = info.name
	sData.element = {}
	for k, v in ipairs(info.list) do
		if v.id then
			table.insert(sData.element, v.name)
		else
			local buff = {}
			for key, val in pairs(v) do
				buff[key] = val.name
			end
			table.insert(sData.element, buff)
		end
	end
	sData.attr = info.explain
	return sData
end

--获取套装附加属性
function GetSuitAttrById(id, num)
	local info = GetSuitInfoById(id)
	local sData = {}
	for k, v in ipairs(info.attr) do
		if num >= v.num then
			table.insert(sData, v.list)
		end
	end
	return sData
end







function __init__ ()

	SUITLIST = Import("autocode/itemSuit.lua")
	AllSuitList = SUITLIST.GetTable()
	
end
