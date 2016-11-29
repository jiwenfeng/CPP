--势力祭坛物品表
 local autoTable = {
	[1] = {
		{type = "gem", Lv = 1, num = 1, rate = 0.6},
		{type = "gem", Lv = 2, num = 1, rate = 0.25},
		{type = "gem", Lv = 3, num = 1, rate = 0.1339},
	},
	[2] = {
		{goodsId = 102012, num = 1, rate = 0.34},
		{goodsId = 102003, num = 1, rate = 0.33},
		{goodsId = 102004, num = 1, rate = 0.33},
	},
	[3] = {
		{type = "buffer", Lv = 1, num = 1, rate = 0.9},
		{type = "buffer", Lv = 2, num = 1, rate = 0.1},
	},
	[4] = {
		{goodsId = 104002, num = 100000, rate = 0.9},
		{goodsId = 104002, num = 250000, rate = 0.09},
		{goodsId = 104002, num = 500000, rate = 0.01},
	},
	[5] = {
		{goodsId = 104003, num = 2000, rate = 0.9},
		{goodsId = 104003, num = 5000, rate = 0.09},
		{goodsId = 104003, num = 10000, rate = 0.01},
	},
	[6] = {
		{goodsId = 104005, num = 10, rate = 0.9},
		{goodsId = 104005, num = 30, rate = 0.09},
		{goodsId = 104005, num = 100, rate = 0.01},
	},
	[7] = {
		{goodsId = 104006, num = 5000, rate = 0.9},
		{goodsId = 104006, num = 10000, rate = 0.1},
	},
	[8] = {
		{goodsId = 101029, num = 1, rate = 0.1},
		{goodsId = 101002, num = 1, rate = 0.6},
		{goodsId = 101001, num = 1, rate = 0.3},
	},
}
function GetTable()
	return autoTable
end
