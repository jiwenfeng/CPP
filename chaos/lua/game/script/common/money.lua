--货币


local goldTypeList = {						--货币类型表
	gold = {name = "钱币", order = 1, field = "needGold", get = "GetGold", set = "SetGold", picId = 104002}, 
	voucher = {name = "绑定元宝", order = 3, field = "needIngot", get = "GetTotalIngot", set = "SetTotalIngot", picId = 104005},
	nimbus = {name = "灵气", order = 2, field = "needNimbus", get = "GetNimbus", set = "SetNimbus", picId = 104003},
	ingot = {name = "元宝", order = 4, field = "needIngot", get = "GetTotalIngot", set = "SetTotalIngot", picId = 104004},
	exp = {name = "经验", order = 0, field = "needExp", get = "GetExp", set = "SetExp", picId = 104006},
}

function GetAllGoldType()
	return goldTypeList
end

--获取货币类型信息
function GetGoldInfoByType(type)
	return goldTypeList[type]
end

--获取货币类型权重（排序依据）
function GetGoldTypeOrder(type)
	if not goldTypeList[type] then
		return
	end
	return goldTypeList[type].order
end

function GetGoldTypeName(type)
	if not goldTypeList[type] then
		return
	end
	return goldTypeList[type].name
end

function GetGoldTypeByPicId(picId)
	for k, v in pairs(goldTypeList) do
		if v.picId == picId then
			return k
		end
	end
end