--防沉迷提醒表
 local autoTable = {
	[1] = {
		timelong = 3600,
		rate = 1,
		notice = "您已累计在线1小时，请合理安排游戏时间。",
	},
	[2] = {
		timelong = 7200,
		rate = 1,
		notice = "您已累计在线2小时，请合理安排游戏时间。",
	},
	[3] = {
		timelong = 10800,
		rate = 0.5,
		notice = "您已累计在线3小时，您已进入疲劳游戏时间，游戏收益变为正常的50%，请您下线休息。",
	},
	[4] = {
		timelong = 12600,
		rate = 0.5,
		notice = "您已累计在线3.5小时，您已进入疲劳游戏时间，游戏收益变成正常的50%，请您下线休息。",
	},
	[5] = {
		timelong = 94400,
		rate = 0.5,
		notice = "您已累计在线4小时，您已进入疲劳游戏时间，游戏收益变成正常的50%，请您下线休息。",
	},
	[6] = {
		timelong = 16200,
		rate = 0.5,
		notice = "您已累计在线4.5小时，您已进入疲劳游戏时间，游戏收益变成正常的50%，请您下线休息。",
	},
	[7] = {
		timelong = 18000,
		rate = 0,
		notice = "您已累计在线5小时，您已进入不健康游戏时间，游戏收益变为0，请下线休息。",
	},
}
function GetTable()
	return autoTable
end
