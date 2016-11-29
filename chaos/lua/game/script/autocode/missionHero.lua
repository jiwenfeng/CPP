--任务列表
local autoTable = {
	["423001"] = {
		class = "fight",
		type = "hero",
		name = "火烧赤壁",
		aim = "草船借箭打败曹操",
		needLv = 12,
		needCopyId = 604001,
		content = "去赤壁击败曹操",
		target = {
			{type = "fight", id = 230072, num = 1},
		},
		shortDesc = "去%s击败%s",
		prize = {
			gold = 8000,
			exp = 8550,
			nimbus = 800,
		},
		given = {
		},
		startNpcId = "210035",
		startQ = "你进去查看下，到底有何奇异之处。",
		startA = "好，我这就去！",
		doingQ = "你还没去吗？",
		doingA = "我马上去。",
		endNpcId = "210035",
		endQ = "有何收获？",
		endA = "真是一个奇异的地方啊~",
	},
	["423002"] = {
		fore = "423001",
		class = "fight",
		type = "hero",
		name = "华山论剑",
		aim = "去华山比试武功，与各路高手交手",
		needLv = 12,
		needCopyId = 604002,
		content = "去华山击败王重阳",
		target = {
			{type = "fight", id = 230073, num = 1},
		},
		shortDesc = "去%s击败%s",
		prize = {
			gold = 20000,
			exp = 9999,
			nimbus = 2000,
		},
		given = {
		},
		startNpcId = "210035",
		startQ = "你进去查看下，到底有何奇异之处。",
		startA = "好，我这就去！",
		doingQ = "你还没去吗？",
		doingA = "我马上去。",
		endNpcId = "210035",
		endQ = "有何收获？",
		endA = "真是一个奇异的地方啊~",
	},
}

function GetTable()
	return autoTable
end
