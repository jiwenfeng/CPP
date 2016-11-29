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
			[1] = {num = 2, desc = "兵力+384"},
			[2] = {num = 4, desc = "物理防御+60\n法术防御+60"},
			[3] = {num = 6, desc = "附加躲避率5%\n附加物理抗性5%\n附加法术抗性5%"},
		},
		attr={
			{num = 6, list ={ jink=0.05, physcResist=0.05, magicResist=0.05,}},
			{num = 4, list ={ physcDef=60, magicDef=60,}},
			{num = 2, list ={ hpMax=384,}},
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
			[1] = {num = 2, desc = "兵力+564"},
			[2] = {num = 4, desc = "物理防御+90\n法术防御+90"},
			[3] = {num = 6, desc = "附加躲避率5%\n附加物理抗性5%\n附加法术抗性5%"},
		},
		attr={
			{num = 6, list ={ jink=0.05, physcResist=0.05, magicResist=0.05,}},
			{num = 4, list ={ physcDef=90, magicDef=90,}},
			{num = 2, list ={ hpMax=564,}},
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
			[1] = {num = 2, desc = "兵力+744"},
			[2] = {num = 4, desc = "物理防御+120\n法术防御+120"},
			[3] = {num = 6, desc = "附加躲避率10%\n附加物理抗性10%\n附加法术抗性10%"},
		},
		attr={
			{num = 6, list ={ jink=0.1, physcResist=0.1, magicResist=0.1,}},
			{num = 4, list ={ physcDef=120, magicDef=120,}},
			{num = 2, list ={ hpMax=744,}},
		},
	},
	[155004]={
		class = "suit",
		name="月影套装",
		list = {
			[1] = {
				["warrior"] = {id = 151113, name = "月影剑"},
				["archer"] = {id = 151213, name = "月影弓"},
				["taoist"] = {id = 151313, name = "月影杖"},
			},
			[2] = {id = 151413, name = "月影袍"},
			[3] = {id = 151513, name = "月影裤"},
			[4] = {id = 151613, name = "月影靴"},
			[5] = {id = 151713, name = "月影项链"},
			[6] = {id = 151813, name = "月影戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "兵力+924"},
			[2] = {num = 4, desc = "物理防御+150\n法术防御+150"},
			[3] = {num = 6, desc = "附加躲避率10%\n附加物理抗性10%\n附加法术抗性10%"},
		},
		attr={
			{num = 6, list ={ jink=0.1, physcResist=0.1, magicResist=0.1,}},
			{num = 4, list ={ physcDef=150, magicDef=150,}},
			{num = 2, list ={ hpMax=924,}},
		},
	},
	[155005]={
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
			[1] = {num = 2, desc = "兵力+1080"},
			[2] = {num = 4, desc = "物理防御+180\n法术防御+180"},
			[3] = {num = 6, desc = "附加躲避率12%\n附加物理抗性12%\n附加法术抗性12%"},
		},
		attr={
			{num = 6, list ={ jink=0.12, physcResist=0.12, magicResist=0.12,}},
			{num = 4, list ={ physcDef=180, magicDef=180,}},
			{num = 2, list ={ hpMax=1080,}},
		},
	},
	[155006]={
		class = "suit",
		name="星尘套装",
		list = {
			[1] = {
				["warrior"] = {id = 151117, name = "星尘剑"},
				["archer"] = {id = 151217, name = "星尘弓"},
				["taoist"] = {id = 151317, name = "星尘杖"},
			},
			[2] = {id = 151417, name = "星尘铠"},
			[3] = {id = 151517, name = "星尘护腿"},
			[4] = {id = 151617, name = "星尘靴"},
			[5] = {id = 151717, name = "星尘项链"},
			[6] = {id = 151817, name = "星尘戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "兵力+1104"},
			[2] = {num = 4, desc = "物理防御+180\n法术防御+180"},
			[3] = {num = 6, desc = "附加躲避率10%\n附加物理抗性10%\n附加法术抗性10%"},
		},
		attr={
			{num = 6, list ={ jink=0.1, physcResist=0.1, magicResist=0.1,}},
			{num = 4, list ={ physcDef=180, magicDef=180,}},
			{num = 2, list ={ hpMax=1104,}},
		},
	},
	[155007]={
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
			[1] = {num = 2, desc = "兵力+1320"},
			[2] = {num = 4, desc = "物理防御+216\n法术防御+216"},
			[3] = {num = 6, desc = "附加躲避率12%\n附加物理抗性12%\n附加法术抗性12%"},
		},
		attr={
			{num = 6, list ={ jink=0.12, physcResist=0.12, magicResist=0.12,}},
			{num = 4, list ={ physcDef=216, magicDef=216,}},
			{num = 2, list ={ hpMax=1320,}},
		},
	},
	[155008]={
		class = "suit",
		name="玄黄破坚套装",
		list = {
			[1] = {
				["warrior"] = {id = 151121, name = "玄黄破坚剑"},
				["archer"] = {id = 151221, name = "玄黄破坚弓"},
				["taoist"] = {id = 151321, name = "玄黄破坚杖"},
			},
			[2] = {id = 151421, name = "玄黄破坚铠"},
			[3] = {id = 151521, name = "玄黄破坚下装"},
			[4] = {id = 151621, name = "玄黄破坚战靴"},
			[5] = {id = 151721, name = "玄黄破坚项链"},
			[6] = {id = 151821, name = "玄黄破坚戒指"},
		},
		explain = {
			[1] = {num = 2, desc = "兵力+1284"},
			[2] = {num = 4, desc = "物理防御+185\n法术防御+185"},
			[3] = {num = 6, desc = "附加躲避率10%\n附加物理抗性10%\n附加法术抗性10%"},
		},
		attr={
			{num = 6, list ={ jink=0.1, physcResist=0.1, magicResist=0.1,}},
			{num = 4, list ={ physcDef=185, magicDef=185,}},
			{num = 2, list ={ hpMax=1284,}},
		},
	},
	[155009]={
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
			[1] = {num = 2, desc = "兵力+1540"},
			[2] = {num = 4, desc = "物理防御+222\n法术防御+222"},
			[3] = {num = 6, desc = "附加躲避率12%\n附加物理抗性12%\n附加法术抗性12%"},
		},
		attr={
			{num = 6, list ={ jink=0.12, physcResist=0.12, magicResist=0.12,}},
			{num = 4, list ={ physcDef=222, magicDef=222,}},
			{num = 2, list ={ hpMax=1540,}},
		},
	},
}
function GetTable()
	return autoTable
end
