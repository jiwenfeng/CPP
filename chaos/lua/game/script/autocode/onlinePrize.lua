--在线奖励列表
local autoTable = {
	[1] = {
		next = 2,
		timelong = 60,
		itemList = {
			{id = 104002, num = 50000},
			{id = 104003, num = 1000},
		},
	},
	[2] = {
		next = 3,
		timelong = 180,
		itemList = {
			{id = 101041, num = 2},
			{id = 101035, num = 1},
		},
	},
	[3] = {
		next = 4,
		timelong = 300,
		itemList = {
			{id = 101041, num = 2},
			{id = 101036, num = 1},
		},
	},
	[4] = {
		next = 5,
		timelong = 480,
		itemList = {
			{id = 104005, num = 5},
			{id = 102012, num = 5},
		},
	},
	[5] = {
		next = 6,
		timelong = 720,
		itemList = {
			{id = 104002, num = 100000},
			{id = 104003, num = 5000},
		},
	},
	[6] = {
		next = 7,
		timelong = 960,
		itemList = {
			{id = 104005, num = 5},
			{id = 102001, num = 1},
		},
	},
	[7] = {
		next = 8,
		timelong = 1200,
		itemList = {
			{id = 104005, num = 10},
			{id = 102003, num = 1},
			{id = 102012, num = 10},
		},
	},
	[8] = {
		next = 9,
		timelong = 1500,
		itemList = {
			{id = 101041, num = 2},
			{id = 102004, num = 1},
		},
	},
	[9] = {
		timelong = 1800,
		itemList = {
			{id = 102011, num = 1},
			{id = 101037, num = 1},
		},
	},
}

function GetTable()
	return autoTable
end