--宝箱列表
local autoTable = {
	[105005] = {
--		name = "玉规宝箱（小）",
		itemList = {
			{id = 104009, num = 100, rate = 1},
		},
	},
	[105006] = {
--		name = "玉规宝箱（大）",
		itemList = {
			{id = 104009, num = 1000, rate = 1},
		},
	},
	[105007] = {
--		name = "完璧礼包",
		gold = 200000,
		itemList = {
			{type = "gem", Lv = 2, num = 2, rate = 1},
		},
	},
	[105008] = {
--		name = "开拓者礼包",
		itemList = {
			{id = 101038, num = 15, rate = 1},
		},
	},
	[105009] = {
--		name = "征服者礼包",
		itemList = {
			{id = 101038, num = 30, rate = 1},
		},
	},
	[105010] = {
--		name = "独步天下礼包",
		gold = 500000,
		itemList = {
			{type = "gem", Lv = 4, num = 1, rate = 1},
		},
	},
	[105011] = {
--		name = "潜龙逆鳞礼包",
		gold = 300000,
		itemList = {
			{type = "gem", Lv = 3, num = 2, rate = 1},
		},
	},
	[105012] = {
--		name = "铁血战狂礼包",
		gold = 200000,
		itemList = {
			{type = "gem", Lv = 3, num = 1, rate = 1},
		},
	},
	[105013] = {
--		name = "百战勇者礼包",
		gold = 100000,
		itemList = {
			{type = "gem", Lv = 2, num = 2, rate = 1},
		},
	},
	[101092] = {
--		name = "绑定元宝",
		voucher = 10,
	},
	[101093] = {
--		name = "晶石",
		spar = 5000,
	},
	[105014] = {
--		name = "每日礼包",
		gold = 20000,
		itemList = {
			{id = 101038, num = 3, rate = 1},
		},
	},
	[105015] = {
--		name = "vip1礼包",
		gold = 50000,
		itemList = {
			{id = 101041, num = 3, rate = 1},
			{id = 102004, num = 1, rate = 1},
		},
	},
	[105016] = {
--		name = "vip2礼包",
		gold = 100000,
		itemList = {
			{id = 101041, num = 5, rate = 1},
			{id = 102004, num = 2, rate = 1},
		},
	},
	[105017] = {
--		name = "vip3礼包",
		gold = 150000,
		itemList = {
			{id = 101041, num = 7, rate = 1},
			{id = 102004, num = 3, rate = 1},
		},
	},
	[105018] = {
--		name = "vip4礼包",
		gold = 200000,
		itemList = {
			{id = 101041, num = 10, rate = 1},
			{id = 102004, num = 4, rate = 1},
		},
	},
	[105019] = {
--		name = "vip5礼包",
		gold = 250000,
		itemList = {
			{id = 101041, num = 15, rate = 1},
			{id = 102004, num = 5, rate = 1},
		},
	},
	[105020] = {
--		name = "10级成长礼包",
		itemList = {
			{id = 105021, num = 1, rate = 1},
			{id = 101021, num = 1, rate = 1},
		},
	},
	[105021] = {
--		name = "20级成长礼包",
		itemList = {
			{id = 105022, num = 1, rate = 1},
			{id = 101021, num = 1, rate = 1},
		},
	},
	[105022] = {
--		name = "30级成长礼包",
		itemList = {
			{id = 105023, num = 1, rate = 1},
			{id = 101021, num = 1, rate = 1},
		},
	},
	[105023] = {
--		name = "40级成长礼包",
		itemList = {
			{id = 105024, num = 1, rate = 1},
			{id = 101021, num = 1, rate = 1},
		},
	},
	[105024] = {
--		name = "50级成长礼包",
		itemList = {
			{id = 105025, num = 1, rate = 1},
			{id = 101095, num = 2, rate = 1},
		},
	},
	[105025] = {
--		name = "60级成长礼包",
		itemList = {
			{id = 105026, num = 1, rate = 1},
			{id = 101095, num = 2, rate = 1},
		},
	},
	[105026] = {
--		name = "70级成长礼包",
		itemList = {
			{id = 105027, num = 1, rate = 1},
			{id = 101096, num = 2, rate = 1},
		},
	},
	[105027] = {
--		name = "80级成长礼包",
		itemList = {
			{id = 105028, num = 1, rate = 1},
			{id = 101100, num = 5, rate = 1},
		},
	},
	[105028] = {
--		name = "90级成长礼包",
		itemList = {
			{id = 105029, num = 1, rate = 1},
			{id = 101101, num = 3, rate = 1},
		},
	},
	[105029] = {
--		name = "100级成长礼包",
		itemList = {
			{id = 104005, num = 30, rate = 1},
			{id = 102009, num = 3, rate = 1},
		},
	},
	[105030] = {
--		name = "新手礼包",
		itemList = {
			{id = 105020, num = 1, rate = 1},
		},
	},
	[105031] = {
--		name = "vip6礼包",
		gold = 300000,
		itemList = {
			{id = 101041, num = 20, rate = 1},
			{id = 102004, num = 6, rate = 1},
		},
	},
	[105032] = {
--		name = "vip7礼包",
		gold = 350000,
		itemList = {
			{id = 101041, num = 25, rate = 1},
			{id = 102004, num = 7, rate = 1},
		},
	},
	[105033] = {
--		name = "vip8礼包",
		gold = 400000,
		itemList = {
			{id = 101041, num = 30, rate = 1},
			{id = 102004, num = 8, rate = 1},
		},
	},
	[105034] = {
--		name = "vip9礼包",
		gold = 450000,
		itemList = {
			{id = 101041, num = 35, rate = 1},
			{id = 102004, num = 9, rate = 1},
		},
	},
	[105035] = {
--		name = "vip10礼包",
		gold = 500000,
		itemList = {
			{id = 101041, num = 40, rate = 1},
			{id = 102004, num = 10, rate = 1},
		},
	},
}

function GetTable()
	return autoTable
end
