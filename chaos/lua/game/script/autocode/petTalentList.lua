--本命精灵天赋技能表
 local autoTable = {
	[311012] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			physcAtk = 1,
			magicAtk = 1,
		},
		descrip = "主将攻击力提升%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311013] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			physcDef = 1,
			magicDef = 1,
		},
		descrip = "主将防御力提升%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311014] = {
		init = 2,
		max = 20,
		attrList = {
			morale = 1,
		},
		descrip = "战斗开始时士气值提升%d",
		uplimit = {
			[1] = 4,
			[2] = 8,
			[3] = 12,
			[4] = 16,
			[5] = 20,
		},
	},
	[311015] = {
		init = 2,
		max = 20,
		attrList = {
			rage = 1,
		},
		descrip = "战斗开始时愤怒值提升%d",
		uplimit = {
			[1] = 4,
			[2] = 8,
			[3] = 12,
			[4] = 16,
			[5] = 20,
		},
	},
	[311016] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			physcAtk = 1,
			magicAtk = 1,
		},
		descrip = "上阵战魂攻击力提升%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311017] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			physcDef = 1,
			magicDef = 1,
		},
		descrip = "上阵战魂防御力提升%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311018] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			addExp = 1,
		},
		descrip = "提升战魂%g%%战斗所得的经验",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311019] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			reduceRes = 1,
		},
		descrip = "升级建筑所需资源减少%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311020] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			cdTime = 1,
		},
		descrip = "升级建筑冷却时间减少%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
	[311021] = {
		init = 0.02,
		max = 0.2,
		attrList = {
			AddRes = 1,
		},
		descrip = "市场、晶石矿产量提升%g%%",
		uplimit = {
			[1] = 0.04,
			[2] = 0.08,
			[3] = 0.12,
			[4] = 0.16,
			[5] = 0.2,
		},
	},
}
function GetTable()
	return autoTable
end
