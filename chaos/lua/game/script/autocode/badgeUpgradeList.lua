--徽章等级表
 local autoTable = {
	[0] = {
		needPoint = 50,
		attr = {
			strength = 0,
			intellect = 0,
			agility = 0,
			captain = 0,
		},
		name = '乡野草民',
	},
	[1] = {
		needPoint = 100,
		attr = {
			strength = 5,
			intellect = 5,
			agility = 5,
			captain = 5,
		},
		name = '初入蛮荒',
	},
	[2] = {
		needPoint = 190,
		attr = {
			strength = 10,
			intellect = 10,
			agility = 10,
			captain = 10,
		},
		name = '蛮荒小虾',
	},
	[3] = {
		needPoint = 310,
		attr = {
			strength = 15,
			intellect = 15,
			agility = 15,
			captain = 15,
		},
		name = '混水泥鳅',
	},
	[4] = {
		needPoint = 460,
		attr = {
			strength = 20,
			intellect = 20,
			agility = 20,
			captain = 20,
		},
		name = '如鱼得水',
	},
	[5] = {
		needPoint = 640,
		attr = {
			strength = 25,
			intellect = 25,
			agility = 25,
			captain = 25,
		},
		name = '纵横一方',
	},
	[6] = {
		needPoint = 850,
		attr = {
			strength = 30,
			intellect = 30,
			agility = 30,
			captain = 30,
		},
		name = '横行无阻',
	},
	[7] = {
		needPoint = 1090,
		attr = {
			strength = 35,
			intellect = 35,
			agility = 35,
			captain = 35,
		},
		name = '一流高手',
	},
	[8] = {
		needPoint = 1360,
		attr = {
			strength = 40,
			intellect = 40,
			agility = 40,
			captain = 40,
		},
		name = '超级高手',
	},
	[9] = {
		needPoint = 1660,
		attr = {
			strength = 45,
			intellect = 45,
			agility = 45,
			captain = 45,
		},
		name = '蛮荒传说',
	},
	[10] = {
		attr = {
			strength = 50,
			intellect = 50,
			agility = 50,
			captain = 50,
		},
		name = '蛮荒神话',
	},
}
function GetTable()
	return autoTable
end
