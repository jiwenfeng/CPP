--炼器鼎列表
 local autoTable = {
	[1] = {
		name = "炼器鼎",
		needGold = 3000,
		forward = 0.5,
		rateList = {
			{quality = 1, rate = 0.5},
			{quality = 2, rate = 0.4},
			{quality = 3, rate = 0.1},
		},
		next = 2,
	},
	[2] = {
		name = "筑基鼎",
		needGold = 4000,
		forward = 0.4,
		rateList = {
			{quality = 1, rate = 0.5},
			{quality = 2, rate = 0.45},
			{quality = 3, rate = 0.05},
		},
		next = 3,
	},
	[3] = {
		name = "淬灵鼎",
		needGold = 5000,
		forward = 0.3,
		rateList = {
			{quality = 1, rate = 0.2},
			{quality = 2, rate = 0.4},
			{quality = 3, rate = 0.32},
			{quality = 4, rate = 0.08},
		},
		next = 4,
	},
	[4] = {
		name = "衍神鼎",
		needGold = 10000,
		forward = 0.2,
		rateList = {
			{quality = 2, rate = 0.4},
			{quality = 3, rate = 0.5},
			{quality = 4, rate = 0.1},
		},
		next = 5,
	},
	[5] = {
		name = "混沌鼎",
		needGold = 20000,
		needIngot = 200,
		forward = 1,
		rateList = {
			{quality = 2, rate = 0.35},
			{quality = 3, rate = 0.43},
			{quality = 4, rate = 0.2},
			{quality = 5, rate = 0.02},
		},
		next = 1,
		needVipLv = 1,
	},
}
function GetTable()
	return autoTable
end
