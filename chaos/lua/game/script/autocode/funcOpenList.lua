--功能开启表
 local autoTable = {
	[16] = {
		name = "战魂",
		type = "guideLine",
		order = 1,
		condt = {
			needLv = 1,
		},
	},
	[22] = {
		name = "背包",
		type = "guideLine",
		order = 2,
		condt = {
			needLv = 1,
		},
	},
	[7] = {
		name = "仙器",
		type = "guideLine",
		order = 3,
		condt = {
			needLv = 7,
		},
	},
	[14] = {
		name = "阵型",
		type = "guideLine",
		order = 4,
		condt = {
			needLv = 8,
		},
	},
	[6] = {
		name = "锻造",
		type = "guideLine",
		order = 5,
		condt = {
			needLv = 4,
		},
	},
	[23] = {
		name = "任务",
		type = "guideLine",
		order = 7,
		condt = {
			needLv = 1,
		},
	},
	[21] = {
		name = "联盟",
		type = "guideLine",
		order = 8,
		condt = {
			needLv = 24,
		},
	},
	[36] = {
		name = "本命精灵",
		type = "guideLine",
		order = 9,
		condt = {
			needLv = 1,
		},
	},
	[103] = {
		name = "首充礼包",
		type = "act",
		order = 101,
		condt = {
			needLv = 1,
		},
	},
	[102] = {
		name = "累积充值礼包",
		type = "act",
		order = 102,
		condt = {
			needLv = 1,
		},
	},
	[101] = {
		name = "在线奖励",
		type = "act",
		order = 104,
		condt = {
			needLv = 1,
		},
	},
	[104] = {
		name = "每日活动",
		type = "act",
		order = 103,
		condt = {
			needLv = 1,
		},
	},
	[17] = {
		name = "成就",
		type = "act",
		order = 25,
		condt = {
			needLv = 10,
		},
	},
	[19] = {
		name = "竞技场",
		type = "act",
		order = 22,
		condt = {
			needLv = 15,
		},
	},
	[41] = {
		name = "世界boss",
		type = "act",
		order = 27,
		condt = {
			needLv = 25,
		},
	},
	[15] = {
		name = "排行榜",
		type = "act",
		order = 23,
		condt = {
			needLv = 25,
		},
	},
	[18] = {
		name = "日常任务",
		type = "act",
		order = 24,
		condt = {
			needLv = 27,
		},
	},
	[105] = {
		name = "竞技场奖励",
		type = "act",
		order = 28,
		condt = {
			needLv = 15,
		},
	},
	[106] = {
		name = "建设值奖励",
		type = "act",
		order = 29,
		condt = {
			needLv = 28,
		},
	},
	[35] = {
		name = "势力战",
		type = "act",
		order = 26,
		condt = {
			needLv = 30,
		},
	},
	[40] = {
		name = "摇钱树",
		type = "act",
		order = 30,
		condt = {
			needLv = 22,
		},
	},
	[31] = {
		name = "组队副本",
		type = "act",
		order = 31,
		condt = {
			needLv = 32,
		},
	},
	[44] = {
		name = "城池争夺战",
		type = "act",
		order = 32,
		condt = {
			needLv = 21,
		},
	},
	[37] = {
		name = "打坐",
		condt = {
			needLv = 11,
		},
	},
	[12] = {
		name = "战魂招募",
		condt = {
			needLv = 8,
		},
	},
	[30] = {
		name = "英魂副本",
		condt = {
			needLv = 12,
		},
	},
	[38] = {
		name = "好友",
		condt = {
			needLv = 18,
		},
	},
	[39] = {
		name = "势力选择",
		condt = {
			needLv = 21,
		},
	},
	[45] = {
		name = "炼妖阵",
		type = "guideLine",
		order = 6,
		condt = {
			needLv = 30,
		},
	},
	[46] = {
		name = "日行一善",
		type = "act",
		order = 33,
		condt = {
			needLv = 25,
		},
	},
	[200] = {
		name = "部落",
		condt = {
			needLv = 21,
		},
	},
	[201] = {
		name = "晶石矿",
		type = "struct",
		condt = {
			needLv = 23,
		},
	},
	[202] = {
		name = "学馆",
		type = "struct",
		condt = {
			needLv = 23,
		},
	},
	[203] = {
		name = "使馆",
		type = "struct",
		condt = {
			needLv = 24,
		},
	},
	[204] = {
		name = "修仙塔",
		type = "struct",
		condt = {
			needLv = 28,
		},
	},
	[205] = {
		name = "天工炉",
		type = "struct",
		condt = {
			needLv = 29,
		},
	},
	[206] = {
		name = "淬炼",
		type = "struct",
		condt = {
			needLv = 40,
		},
	},
	[20] = {
		name = "藏宝峡谷",
		type = "act",
		order = 34,
		condt = {
			needLv = 20,
		},
	},
}

function GetTable()
	return autoTable
end
