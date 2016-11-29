--宝箱列表
local autoTable = {
	[105001] = {
		name = "下品饕餮宝箱",
		min = 1,
		max = 1,
		itemList = {
			{id = 102013, rate = 0.31},
		},
	},
	[105002] = {
		name = "上品饕餮宝箱",
		min = 1,
		max = 2,
		itemList = {
			{id = 102013, rate = 0.31},
		},
	},
	[105003] = {
		name = "精品饕餮宝箱",
		min = 1,
		max = 3,
		itemList = {
			{id = 102013, rate = 0.31},
		},
	},
	[105004] = {
		name = "宝石礼包",
		min = 1,
		max = 1,
		itemList = {
			{type = "gem", Lv = 1, rate = 0.8},
		},
	},
}

function GetTable()
	return autoTable
end
