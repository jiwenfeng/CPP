--装备表
 local autoTable = {
	[155001]={
		class = "suit",
		name="猎魔套装",
		list = {
			[1] = {
				["warrior"] = {id = 151104, name = "猎魔剑"},
				["archer"] = {id = 151204, name = "猎魔弓"},
				["taoist"] = {id = 151304, name = "猎魔杖"},
			},
			[2] = {id = 151404, name = "猎魔袍"},
			[3] = {id = 151504, name = "猎魔护腿"},
			[4] = {id = 151604, name = "猎魔靴"},
			[5] = {id = 151704, name = "猎魔项链"},
			[6] = {id = 151804, name = "猎魔戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+44"},
			[2] = {num = 4, desc = "兵力上限+120"},
			[3] = {num = 6, desc = "行动速度+12"},
		},
		attr={
			{num = 6, list ={ speed=12,}},
			{num = 4, list ={ hpMax=120,}},
			{num = 2, list ={ physcDef=44, magicDef=44,}},
		},
	},
	[155002]={
		class = "suit",
		name="无极套装",
		list = {
			[1] = {
				["warrior"] = {id = 151107, name = "无极剑"},
				["archer"] = {id = 151207, name = "无极弓"},
				["taoist"] = {id = 151307, name = "无极杖"},
			},
			[2] = {id = 151407, name = "无极袍"},
			[3] = {id = 151507, name = "无极护腿"},
			[4] = {id = 151607, name = "无极靴"},
			[5] = {id = 151707, name = "无极项链"},
			[6] = {id = 151807, name = "无极戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+66"},
			[2] = {num = 4, desc = "兵力上限+190"},
			[3] = {num = 6, desc = "行动速度+19"},
		},
		attr={
			{num = 6, list ={ speed=19,}},
			{num = 4, list ={ hpMax=190,}},
			{num = 2, list ={ physcDef=66, magicDef=66,}},
		},
	},
	[155003]={
		class = "suit",
		name="赤炎套装",
		list = {
			[1] = {
				["warrior"] = {id = 151110, name = "赤炎剑"},
				["archer"] = {id = 151210, name = "赤炎弓"},
				["taoist"] = {id = 151310, name = "赤炎杖"},
			},
			[2] = {id = 151410, name = "赤炎袍"},
			[3] = {id = 151510, name = "赤炎护腿"},
			[4] = {id = 151610, name = "赤炎靴"},
			[5] = {id = 151710, name = "赤炎项链"},
			[6] = {id = 151810, name = "赤炎戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+88"},
			[2] = {num = 4, desc = "兵力上限+250"},
			[3] = {num = 6, desc = "行动速度+25"},
		},
		attr={
			{num = 6, list ={ speed=25,}},
			{num = 4, list ={ hpMax=250,}},
			{num = 2, list ={ physcDef=88, magicDef=88,}},
		},
	},
	[155004]={
		class = "suit",
		name="月影套装",
		list = {
			[1] = {
				["warrior"] = {id = 151114, name = "月影剑"},
				["archer"] = {id = 151214, name = "月影弓"},
				["taoist"] = {id = 151314, name = "月影杖"},
			},
			[2] = {id = 151414, name = "月影袍"},
			[3] = {id = 151514, name = "月影裤"},
			[4] = {id = 151614, name = "月影靴"},
			[5] = {id = 151714, name = "月影项链"},
			[6] = {id = 151814, name = "月影戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+111"},
			[2] = {num = 4, desc = "兵力上限+320"},
			[3] = {num = 6, desc = "行动速度+32"},
		},
		attr={
			{num = 6, list ={ speed=32,}},
			{num = 4, list ={ hpMax=320,}},
			{num = 2, list ={ physcDef=111, magicDef=111,}},
		},
	},
	[155005]={
		class = "suit",
		name="星尘套装",
		list = {
			[1] = {
				["warrior"] = {id = 151118, name = "星尘剑"},
				["archer"] = {id = 151218, name = "星尘弓"},
				["taoist"] = {id = 151318, name = "星尘杖"},
			},
			[2] = {id = 151418, name = "星尘铠"},
			[3] = {id = 151518, name = "星尘护腿"},
			[4] = {id = 151618, name = "星尘靴"},
			[5] = {id = 151718, name = "星尘项链"},
			[6] = {id = 151818, name = "星尘戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+133"},
			[2] = {num = 4, desc = "兵力上限+380"},
			[3] = {num = 6, desc = "行动速度+38"},
		},
		attr={
			{num = 6, list ={ speed=38,}},
			{num = 4, list ={ hpMax=380,}},
			{num = 2, list ={ physcDef=133, magicDef=133,}},
		},
	},
	[155006]={
		class = "suit",
		name="玄黄破坚套装",
		list = {
			[1] = {
				["warrior"] = {id = 151122, name = "玄黄破坚剑"},
				["archer"] = {id = 151222, name = "玄黄破坚弓"},
				["taoist"] = {id = 151322, name = "玄黄破坚杖"},
			},
			[2] = {id = 151422, name = "玄黄破坚铠"},
			[3] = {id = 151522, name = "玄黄破坚下装"},
			[4] = {id = 151622, name = "玄黄破坚战靴"},
			[5] = {id = 151722, name = "玄黄破坚项链"},
			[6] = {id = 151822, name = "玄黄破坚戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+156"},
			[2] = {num = 4, desc = "兵力上限+440"},
			[3] = {num = 6, desc = "行动速度+44"},
		},
		attr={
			{num = 6, list ={ speed=44,}},
			{num = 4, list ={ hpMax=440,}},
			{num = 2, list ={ physcDef=156, magicDef=156,}},
		},
	},
	[155007]={
		class = "suit",
		name="龙吟套装",
		list = {
			[1] = {
				["warrior"] = {id = 151840, name = "龙吟剑"},
				["archer"] = {id = 151841, name = "龙吟弓"},
				["taoist"] = {id = 151842, name = "龙吟杖"},
			},
			[2] = {id = 151843, name = "龙吟战甲"},
			[3] = {id = 151844, name = "龙吟腿甲"},
			[4] = {id = 151845, name = "龙吟战靴"},
			[5] = {id = 151846, name = "龙吟项链"},
			[6] = {id = 151847, name = "龙吟戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "物理、法术防御+80"},
			[2] = {num = 4, desc = "兵力上限+230"},
			[3] = {num = 6, desc = "行动速度+23"},
		},
		attr={
			{num = 6, list ={ speed=23,}},
			{num = 4, list ={ hpMax=230,}},
			{num = 2, list ={ physcDef=80, magicDef=80,}},
		},
	},
}
function GetTable()
	return autoTable
end
