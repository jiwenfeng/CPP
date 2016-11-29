--阵型列表
local autoTable = {
	["321001"] = {
		name = "无阵型",
		initPos = "5",
		needLv = 1,
		needGold = 0,
		explain = "无任何附加效果",
		restraint = {
		},
		linkSkill = {},
		list = {
			[1] = {
				attr = {
				},
				posList = {"1","2","3","4","5","6","7","8","9"}, 
				describe = "随意排列的站位，无任何附加效果",
			},
		},
	},
	["321002"] = {
		name = "血翼阵",
		initPos = "6",
		needLv = 1,
		needGold = 0,
		explain = "提高上阵战魂兵力上限",
		restraint = {
			["321005"] = 0.1,
		},
		linkSkill = {},
		list = {
			[1] = {
				attr = {
					hpMax = 35,
				},
				posList = {"2","6"}, 
				describe = "阵型中的战魂兵力上限提高35",
				needGold = 5000,
			},
			[2] = {
				attr = {
					hpMax = 70,
				},
				posList = {"2","6"}, 
				describe = "阵型中的战魂兵力上限提高70",
				needGold = 10000,
			},
			[3] = {
				attr = {
					hpMax = 105,
				},
				posList = {"2","6"}, 
				describe = "阵型中的战魂兵力上限提高105",
				needGold = 17000,
			},
			[4] = {
				attr = {
					hpMax = 140,
				},
				posList = {"2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高140",
				needGold = 25500,
			},
			[5] = {
				attr = {
					hpMax = 175,
				},
				posList = {"2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高175",
				needGold = 35500,
			},
			[6] = {
				attr = {
					hpMax = 210,
				},
				posList = {"2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高210",
				needGold = 46500,
			},
			[7] = {
				attr = {
					hpMax = 245,
				},
				posList = {"1","2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高245",
				needGold = 58500,
			},
			[8] = {
				attr = {
					hpMax = 280,
				},
				posList = {"1","2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高280",
				needGold = 71500,
			},
			[9] = {
				attr = {
					hpMax = 315,
				},
				posList = {"1","2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高315",
				needGold = 85500,
			},
			[10] = {
				attr = {
					hpMax = 350,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高350",
				needGold = 100500,
			},
			[11] = {
				attr = {
					hpMax = 385,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高385",
				needGold = 116500,
			},
			[12] = {
				attr = {
					hpMax = 420,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高420",
				needGold = 133000,
			},
			[13] = {
				attr = {
					hpMax = 455,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高455",
				needGold = 150500,
			},
			[14] = {
				attr = {
					hpMax = 490,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高490",
				needGold = 168500,
			},
			[15] = {
				attr = {
					hpMax = 525,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高525",
				needGold = 187000,
			},
			[16] = {
				attr = {
					hpMax = 560,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高560",
				needGold = 206500,
			},
			[17] = {
				attr = {
					hpMax = 595,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高595",
				needGold = 226500,
			},
			[18] = {
				attr = {
					hpMax = 630,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高630",
				needGold = 247000,
			},
			[19] = {
				attr = {
					hpMax = 665,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高665",
				needGold = 268000,
			},
			[20] = {
				attr = {
					hpMax = 700,
				},
				posList = {"1","2","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高700",
			},
		},
	},
	["321003"] = {
		name = "磐石阵",
		initPos = "5",
		needLv = 20,
		needGold = 2000,
		explain = "提高上阵战魂防御",
		restraint = {
			["321002"] = 0.1,
		},
		linkSkill = {"301007"},
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					physcDef = 12,
					magicDef = 12,
				},
				posList = {"3","5"}, 
				describe = "阵型中的战魂防御提高12，0.5%几率进行联动攻击",
				needGold = 10000,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					physcDef = 24,
					magicDef = 24,
				},
				posList = {"3","5"}, 
				describe = "阵型中的战魂防御提高24，1.0%几率进行联动攻击",
				needGold = 20000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					physcDef = 36,
					magicDef = 36,
				},
				posList = {"3","5"}, 
				describe = "阵型中的战魂防御提高36，1.5%几率进行联动攻击",
				needGold = 34000,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					physcDef = 48,
					magicDef = 48,
				},
				posList = {"3","5","9"}, 
				describe = "阵型中的战魂防御提高48，2.0%几率进行联动攻击",
				needGold = 51000,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					physcDef = 60,
					magicDef = 60,
				},
				posList = {"3","5","9"}, 
				describe = "阵型中的战魂防御提高60，2.5%几率进行联动攻击",
				needGold = 71000,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					physcDef = 72,
					magicDef = 72,
				},
				posList = {"3","5","9"}, 
				describe = "阵型中的战魂防御提高72，3.0%几率进行联动攻击",
				needGold = 93000,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					physcDef = 84,
					magicDef = 84,
				},
				posList = {"1","3","5","9"}, 
				describe = "阵型中的战魂防御提高84，3.5%几率进行联动攻击",
				needGold = 117000,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					physcDef = 96,
					magicDef = 96,
				},
				posList = {"1","3","5","9"}, 
				describe = "阵型中的战魂防御提高96，4.0%几率进行联动攻击",
				needGold = 143000,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					physcDef = 108,
					magicDef = 108,
				},
				posList = {"1","3","5","9"}, 
				describe = "阵型中的战魂防御提高108，4.5%几率进行联动攻击",
				needGold = 171000,
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					physcDef = 120,
					magicDef = 120,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高120，5.0%几率进行联动攻击",
				needGold = 201000,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					physcDef = 132,
					magicDef = 132,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高132，5.5%几率进行联动攻击",
				needGold = 233000,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					physcDef = 144,
					magicDef = 144,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高144，6.0%几率进行联动攻击",
				needGold = 266000,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					physcDef = 156,
					magicDef = 156,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高156，6.5%几率进行联动攻击",
				needGold = 301000,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					physcDef = 168,
					magicDef = 168,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高168，7.0%几率进行联动攻击",
				needGold = 337000,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					physcDef = 180,
					magicDef = 180,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高180，7.5%几率进行联动攻击",
				needGold = 374000,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					physcDef = 192,
					magicDef = 192,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高192，8.0%几率进行联动攻击",
				needGold = 413000,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					physcDef = 204,
					magicDef = 204,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高204，8.5%几率进行联动攻击",
				needGold = 453000,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					physcDef = 216,
					magicDef = 216,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高216，9.0%几率进行联动攻击",
				needGold = 494000,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					physcDef = 228,
					magicDef = 228,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高228，9.5%几率进行联动攻击",
				needGold = 536000,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					physcDef = 240,
					magicDef = 240,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高240，10.0%几率进行联动攻击",
			},
		},
	},
	["321004"] = {
		name = "迅风阵",
		initPos = "6",
		needLv = 40,
		needGold = 5000,
		explain = "提高上阵战魂行动速度",
		restraint = {
			["321003"] = 0.1,
		},
		linkSkill = {"301013"},
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					speed = 4,
				},
				posList = {"3","6"}, 
				describe = "阵型中的战魂行动速度提高4，0.5%几率进行联动攻击",
				needGold = 15000,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					speed = 8,
				},
				posList = {"3","6"}, 
				describe = "阵型中的战魂行动速度提高8，1.0%几率进行联动攻击",
				needGold = 30000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					speed = 12,
				},
				posList = {"3","6"}, 
				describe = "阵型中的战魂行动速度提高12，1.5%几率进行联动攻击",
				needGold = 51000,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					speed = 16,
				},
				posList = {"3","6","9"}, 
				describe = "阵型中的战魂行动速度提高16，2.0%几率进行联动攻击",
				needGold = 76500,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					speed = 20,
				},
				posList = {"3","6","9"}, 
				describe = "阵型中的战魂行动速度提高20，2.5%几率进行联动攻击",
				needGold = 106500,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					speed = 24,
				},
				posList = {"3","6","9"}, 
				describe = "阵型中的战魂行动速度提高24，3.0%几率进行联动攻击",
				needGold = 139500,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					speed = 28,
				},
				posList = {"3","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高28，3.5%几率进行联动攻击",
				needGold = 175500,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					speed = 32,
				},
				posList = {"3","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高32，4.0%几率进行联动攻击",
				needGold = 214500,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					speed = 36,
				},
				posList = {"3","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高36，4.5%几率进行联动攻击",
				needGold = 256500,
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					speed = 40,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高40，5.0%几率进行联动攻击",
				needGold = 301500,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					speed = 44,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高44，5.5%几率进行联动攻击",
				needGold = 349500,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					speed = 48,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高48，6.0%几率进行联动攻击",
				needGold = 399000,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					speed = 52,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高52，6.5%几率进行联动攻击",
				needGold = 451500,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					speed = 56,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高56，7.0%几率进行联动攻击",
				needGold = 505500,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					speed = 60,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高60，7.5%几率进行联动攻击",
				needGold = 561000,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					speed = 64,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高64，8.0%几率进行联动攻击",
				needGold = 619500,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					speed = 68,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高68，8.5%几率进行联动攻击",
				needGold = 679500,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					speed = 72,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高72，9.0%几率进行联动攻击",
				needGold = 741000,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					speed = 76,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高76，9.5%几率进行联动攻击",
				needGold = 804000,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					speed = 80,
				},
				posList = {"3","4","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高80，10.0%几率进行联动攻击",
			},
		},
	},
	["321005"] = {
		name = "破势阵",
		initPos = "5",
		needLv = 60,
		needNimbus = 5000,
		explain = "提高上阵战魂攻击",
		restraint = {
			["321004"] = 0.1,
		},
		linkSkill = {"301008"},
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					physcAtk = 28,
					magicAtk = 28,
				},
				posList = {"5","6"}, 
				describe = "阵型中的战魂防御提高28，0.5%几率进行联动攻击",
				needNimbus = 1500,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					physcAtk = 56,
					magicAtk = 56,
				},
				posList = {"5","6"}, 
				describe = "阵型中的战魂攻击提高56，1.0%几率进行联动攻击",
				needNimbus = 3000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					physcAtk = 84,
					magicAtk = 84,
				},
				posList = {"5","6"}, 
				describe = "阵型中的战魂攻击提高84，1.5%几率进行联动攻击",
				needNimbus = 5100,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					physcAtk = 112,
					magicAtk = 112,
				},
				posList = {"2","5","6"}, 
				describe = "阵型中的战魂攻击提高112，2.0%几率进行联动攻击",
				needNimbus = 7650,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					physcAtk = 140,
					magicAtk = 140,
				},
				posList = {"2","5","6"}, 
				describe = "阵型中的战魂攻击提高140，2.5%几率进行联动攻击",
				needNimbus = 10650,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					physcAtk = 168,
					magicAtk = 168,
				},
				posList = {"2","5","6"}, 
				describe = "阵型中的战魂攻击提高168，3.0%几率进行联动攻击",
				needNimbus = 13950,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					physcAtk = 196,
					magicAtk = 196,
				},
				posList = {"2","5","6","8"}, 
				describe = "阵型中的战魂攻击提高196，3.5%几率进行联动攻击",
				needNimbus = 17550,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					physcAtk = 224,
					magicAtk = 224,
				},
				posList = {"2","5","6","8"}, 
				describe = "阵型中的战魂攻击提高224，4.0%几率进行联动攻击",
				needNimbus = 21450,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					physcAtk = 252,
					magicAtk = 252,
				},
				posList = {"2","5","6","8"}, 
				describe = "阵型中的战魂攻击提高252，4.5%几率进行联动攻击",
				needNimbus = 25650,
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					physcAtk = 280,
					magicAtk = 280,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高280，5.0%几率进行联动攻击",
				needNimbus = 30150,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					physcAtk = 308,
					magicAtk = 308,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高308，5.5%几率进行联动攻击",
				needNimbus = 34950,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					physcAtk = 336,
					magicAtk = 336,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高336，6.0%几率进行联动攻击",
				needNimbus = 39900,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					physcAtk = 364,
					magicAtk = 364,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高364，6.5%几率进行联动攻击",
				needNimbus = 45150,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					physcAtk = 392,
					magicAtk = 392,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高392，7.0%几率进行联动攻击",
				needNimbus = 50550,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					physcAtk = 420,
					magicAtk = 420,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高420，7.5%几率进行联动攻击",
				needNimbus = 56100,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					physcAtk = 448,
					magicAtk = 448,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高448，8.0%几率进行联动攻击",
				needNimbus = 61950,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					physcAtk = 476,
					magicAtk = 476,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高476，8.5%几率进行联动攻击",
				needNimbus = 67950,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					physcAtk = 504,
					magicAtk = 504,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高504，9.0%几率进行联动攻击",
				needNimbus = 74100,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					physcAtk = 532,
					magicAtk = 532,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高532，9.5%几率进行联动攻击",
				needNimbus = 80400,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					physcAtk = 560,
					magicAtk = 560,
				},
				posList = {"2","4","5","6","8"}, 
				describe = "阵型中的战魂攻击提高560，10.0%几率进行联动攻击",
			},
		},
	},
	["321006"] = {
		name = "天命阵",
		initPos = "6",
		needLv = 50,
		needNimbus = 10000,
		explain = "大幅提高上阵战魂兵力上限",
		restraint = {
			["321005"] = 0.1,
			["321009"] = 0.1,
		},
		linkSkill = {"301005"},
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					hpMax = 64,
				},
				posList = {"2","6"}, 
				describe = "阵型中的战魂兵力上限提高64，0.5%几率进行联动攻击",
				needNimbus = 3000,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					hpMax = 128,
				},
				posList = {"2","6"}, 
				describe = "阵型中的战魂兵力上限提高128，1.0%几率进行联动攻击",
				needNimbus = 6000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					hpMax = 192,
				},
				posList = {"2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高192，1.5%几率进行联动攻击",
				needNimbus = 10200,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					hpMax = 256,
				},
				posList = {"2","6","8"}, 
				describe = "阵型中的战魂兵力上限提高256，2.0%几率进行联动攻击",
				needNimbus = 15300,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					hpMax = 320,
				},
				posList = {"2","5","6","8"}, 
				describe = "阵型中的战魂兵力上限提高320，2.5%几率进行联动攻击",
				needNimbus = 21300,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					hpMax = 384,
				},
				posList = {"2","5","6","8"}, 
				describe = "阵型中的战魂兵力上限提高384，3.0%几率进行联动攻击",
				needNimbus = 27900,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					hpMax = 448,
				},
				posList = {"1","2","5","6","8"}, 
				describe = "阵型中的战魂兵力上限提高448，3.5%几率进行联动攻击",
				needNimbus = 35100,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					hpMax = 512,
				},
				posList = {"1","2","5","6","8"}, 
				describe = "阵型中的战魂兵力上限提高512，4.0%几率进行联动攻击",
				needNimbus = 42900,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					hpMax = 576,
				},
				posList = {"1","2","5","6","8"}, 
				describe = "阵型中的战魂兵力上限提高576，4.5%几率进行联动攻击",
				needItem = {goodsId = 101030, num = 1},
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					hpMax = 640,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高640，5.0%几率进行联动攻击",
				needNimbus = 60300,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					hpMax = 704,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高704，5.5%几率进行联动攻击",
				needNimbus = 69900,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					hpMax = 768,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高768，6.0%几率进行联动攻击",
				needNimbus = 79800,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					hpMax = 832,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高832，6.5%几率进行联动攻击",
				needNimbus = 90300,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					hpMax = 896,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高896，7.0%几率进行联动攻击",
				needNimbus = 101100,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					hpMax = 960,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高960，7.5%几率进行联动攻击",
				needNimbus = 112200,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					hpMax = 1024,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高1024，8.0%几率进行联动攻击",
				needNimbus = 123900,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					hpMax = 1088,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高1088，8.5%几率进行联动攻击",
				needNimbus = 135900,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					hpMax = 1152,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高1152，9.0%几率进行联动攻击",
				needNimbus = 148200,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					hpMax = 1216,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高1216，9.5%几率进行联动攻击",
				needNimbus = 160800,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					hpMax = 1280,
				},
				posList = {"1","2","5","6","7","8"}, 
				describe = "阵型中的战魂兵力上限提高1280，10.0%几率进行联动攻击",
			},
		},
	},
	["321007"] = {
		name = "降魔阵",
		initPos = "5",
		needLv = 60,
		needNimbus = 15000,
		explain = "大幅提高上阵成员的防御",
		needCity = 1,
		restraint = {
			["321002"] = 0.1,
			["321006"] = 0.1,
		},
		linkSkill = {"301006", "301004"},
		isNeedCity = 1,
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					physcDef = 22,
					magicDef = 22,
				},
				posList = {"3","5"}, 
				describe = "阵型中的战魂防御提高22，0.5%几率进行联动攻击",
				needNimbus = 4500,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					physcDef = 44,
					magicDef = 44,
				},
				posList = {"3","5"}, 
				describe = "阵型中的战魂防御提高44，1.0%几率进行联动攻击",
				needNimbus = 9000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					physcDef = 66,
					magicDef = 66,
				},
				posList = {"3","5","9"}, 
				describe = "阵型中的战魂防御提高66，1.5%几率进行联动攻击",
				needNimbus = 15300,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					physcDef = 88,
					magicDef = 88,
				},
				posList = {"3","5","9"}, 
				describe = "阵型中的战魂防御提高88，2.0%几率进行联动攻击",
				needNimbus = 22950,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					physcDef = 110,
					magicDef = 110,
				},
				posList = {"3","5","7","9"}, 
				describe = "阵型中的战魂防御提高110，2.5%几率进行联动攻击",
				needNimbus = 31950,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					physcDef = 132,
					magicDef = 132,
				},
				posList = {"3","5","7","9"}, 
				describe = "阵型中的战魂防御提高132，3.0%几率进行联动攻击",
				needNimbus = 41850,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					physcDef = 154,
					magicDef = 154,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高154，3.5%几率进行联动攻击",
				needNimbus = 52650,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					physcDef = 176,
					magicDef = 176,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高176，4.0%几率进行联动攻击",
				needNimbus = 64350,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					physcDef = 198,
					magicDef = 198,
				},
				posList = {"1","3","5","7","9"}, 
				describe = "阵型中的战魂防御提高198，4.5%几率进行联动攻击",
				needItem = {goodsId = 101032, num = 1},
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					physcDef = 220,
					magicDef = 220,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高220，5.0%几率进行联动攻击",
				needNimbus = 90450,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					physcDef = 242,
					magicDef = 242,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高242，5.5%几率进行联动攻击",
				needNimbus = 104850,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					physcDef = 264,
					magicDef = 264,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高264，6.0%几率进行联动攻击",
				needNimbus = 119700,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					physcDef = 286,
					magicDef = 286,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高286，6.5%几率进行联动攻击",
				needNimbus = 135450,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					physcDef = 308,
					magicDef = 308,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高308，7.0%几率进行联动攻击",
				needNimbus = 151650,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					physcDef = 330,
					magicDef = 330,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高330，7.5%几率进行联动攻击",
				needNimbus = 168300,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					physcDef = 352,
					magicDef = 352,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高352，8.0%几率进行联动攻击",
				needNimbus = 185850,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					physcDef = 374,
					magicDef = 374,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高374，8.5%几率进行联动攻击",
				needNimbus = 203850,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					physcDef = 396,
					magicDef = 396,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高396，9.0%几率进行联动攻击",
				needNimbus = 222300,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					physcDef = 418,
					magicDef = 418,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高418，9.5%几率进行联动攻击",
				needNimbus = 241200,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					physcDef = 440,
					magicDef = 440,
				},
				posList = {"1","3","5","6","7","9"}, 
				describe = "阵型中的战魂防御提高440，10.0%几率进行联动攻击",
			},
		},
	},
	["321008"] = {
		name = "罡风阵",
		initPos = "6",
		needLv = 70,
		needNimbus = 20000,
		explain = "大幅提高上阵成员的行动速度",
		needCity = 1,
		restraint = {
			["321003"] = 0.1,
			["321007"] = 0.1,
		},
		linkSkill = {"301013", "301014"},
		isNeedCity = 1,
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					speed = 7,
				},
				posList = {"3","6"}, 
				describe = "阵型中的战魂行动速度提高7，0.5%几率进行联动攻击",
				needNimbus = 6000,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					speed = 14,
				},
				posList = {"3","6"}, 
				describe = "阵型中的战魂行动速度提高14，1.0%几率进行联动攻击",
				needNimbus = 12000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					speed = 21,
				},
				posList = {"3","6","9"}, 
				describe = "阵型中的战魂行动速度提高21，1.5%几率进行联动攻击",
				needNimbus = 20400,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					speed = 28,
				},
				posList = {"3","6","9"}, 
				describe = "阵型中的战魂行动速度提高28，2.0%几率进行联动攻击",
				needNimbus = 30600,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					speed = 35,
				},
				posList = {"3","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高35，2.5%几率进行联动攻击",
				needNimbus = 42600,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					speed = 42,
				},
				posList = {"3","5","6","9"}, 
				describe = "阵型中的战魂行动速度提高42，3.0%几率进行联动攻击",
				needNimbus = 55800,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					speed = 49,
				},
				posList = {"3","5","6","8","9"}, 
				describe = "阵型中的战魂行动速度提高49，3.5%几率进行联动攻击",
				needNimbus = 70200,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					speed = 56,
				},
				posList = {"3","5","6","8","9"}, 
				describe = "阵型中的战魂行动速度提高56，4.0%几率进行联动攻击",
				needNimbus = 85800,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					speed = 63,
				},
				posList = {"3","5","6","8","9"}, 
				describe = "阵型中的战魂行动速度提高63，4.5%几率进行联动攻击",
				needItem = {goodsId = 101031, num = 1},
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					speed = 70,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高70，5.0%几率进行联动攻击",
				needNimbus = 120600,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					speed = 77,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高77，5.5%几率进行联动攻击",
				needNimbus = 139800,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					speed = 84,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高84，6.0%几率进行联动攻击",
				needNimbus = 159600,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					speed = 91,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高91，6.5%几率进行联动攻击",
				needNimbus = 180600,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					speed = 98,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高98，7.0%几率进行联动攻击",
				needNimbus = 202200,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					speed = 105,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高105，7.5%几率进行联动攻击",
				needNimbus = 224400,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					speed = 112,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高112，8.0%几率进行联动攻击",
				needNimbus = 247800,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					speed = 119,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高119，8.5%几率进行联动攻击",
				needNimbus = 271800,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					speed = 126,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高126，9.0%几率进行联动攻击",
				needNimbus = 296400,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					speed = 133,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高133，9.5%几率进行联动攻击",
				needNimbus = 321600,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					speed = 140,
				},
				posList = {"3","5","6","7","8","9"}, 
				describe = "阵型中的战魂行动速度提高140，10.0%几率进行联动攻击",
			},
		},
	},
	["321009"] = {
		name = "诛仙阵",
		initPos = "5",
		needLv = 80,
		needNimbus = 25000,
		explain = "大幅提高上阵成员的攻击",
		needCity = 1,
		restraint = {
			["321004"] = 0.1,
			["321008"] = 0.1,
		},
		linkSkill = {"301008", "301011"},
		isNeedCity = 1,
		list = {
			[1] = {
				linkRate = 0.005,
				attr = {
					physcAtk = 50,
					magicAtk = 50,
				},
				posList = {"5","6"}, 
				describe = "阵型中的战魂攻击提高50，0.5%几率进行联动攻击",
				needNimbus = 7500,
			},
			[2] = {
				linkRate = 0.01,
				attr = {
					physcAtk = 100,
					magicAtk = 100,
				},
				posList = {"5","6"}, 
				describe = "阵型中的战魂攻击提高100，1.0%几率进行联动攻击",
				needNimbus = 15000,
			},
			[3] = {
				linkRate = 0.015,
				attr = {
					physcAtk = 150,
					magicAtk = 150,
				},
				posList = {"2","5","6"}, 
				describe = "阵型中的战魂攻击提高150，1.5%几率进行联动攻击",
				needNimbus = 25500,
			},
			[4] = {
				linkRate = 0.02,
				attr = {
					physcAtk = 200,
					magicAtk = 200,
				},
				posList = {"2","5","6"}, 
				describe = "阵型中的战魂攻击提高200，2.0%几率进行联动攻击",
				needNimbus = 38250,
			},
			[5] = {
				linkRate = 0.025,
				attr = {
					physcAtk = 250,
					magicAtk = 250,
				},
				posList = {"2","3","5","6"}, 
				describe = "阵型中的战魂攻击提高250，2.5%几率进行联动攻击",
				needNimbus = 53250,
			},
			[6] = {
				linkRate = 0.03,
				attr = {
					physcAtk = 300,
					magicAtk = 300,
				},
				posList = {"2","3","5","6"}, 
				describe = "阵型中的战魂攻击提高300，3.0%几率进行联动攻击",
				needNimbus = 69750,
			},
			[7] = {
				linkRate = 0.035,
				attr = {
					physcAtk = 350,
					magicAtk = 350,
				},
				posList = {"2","3","5","6","8"}, 
				describe = "阵型中的战魂攻击提高350，3.5%几率进行联动攻击",
				needNimbus = 87750,
			},
			[8] = {
				linkRate = 0.04,
				attr = {
					physcAtk = 400,
					magicAtk = 400,
				},
				posList = {"2","3","5","6","8"}, 
				describe = "阵型中的战魂攻击提高400，4.0%几率进行联动攻击",
				needNimbus = 107250,
			},
			[9] = {
				linkRate = 0.045,
				attr = {
					physcAtk = 450,
					magicAtk = 450,
				},
				posList = {"2","3","5","6","8"}, 
				describe = "阵型中的战魂攻击提高450，4.5%几率进行联动攻击",
				needItem = {goodsId = 101094, num = 1},
			},
			[10] = {
				linkRate = 0.05,
				attr = {
					physcAtk = 500,
					magicAtk = 500,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高500，5.0%几率进行联动攻击",
				needNimbus = 150750,
			},
			[11] = {
				linkRate = 0.055,
				attr = {
					physcAtk = 550,
					magicAtk = 550,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高550，5.5%几率进行联动攻击",
				needNimbus = 174750,
			},
			[12] = {
				linkRate = 0.06,
				attr = {
					physcAtk = 600,
					magicAtk = 600,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高600，6.0%几率进行联动攻击",
				needNimbus = 199500,
			},
			[13] = {
				linkRate = 0.065,
				attr = {
					physcAtk = 650,
					magicAtk = 650,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高650，6.5%几率进行联动攻击",
				needNimbus = 225750,
			},
			[14] = {
				linkRate = 0.07,
				attr = {
					physcAtk = 700,
					magicAtk = 700,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高700，7.0%几率进行联动攻击",
				needNimbus = 252750,
			},
			[15] = {
				linkRate = 0.075,
				attr = {
					physcAtk = 750,
					magicAtk = 750,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高750，7.5%几率进行联动攻击",
				needNimbus = 280500,
			},
			[16] = {
				linkRate = 0.08,
				attr = {
					physcAtk = 800,
					magicAtk = 800,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高800，8.0%几率进行联动攻击",
				needNimbus = 309750,
			},
			[17] = {
				linkRate = 0.085,
				attr = {
					physcAtk = 850,
					magicAtk = 850,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高850，8.5%几率进行联动攻击",
				needNimbus = 339750,
			},
			[18] = {
				linkRate = 0.09,
				attr = {
					physcAtk = 900,
					magicAtk = 900,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高900，9.0%几率进行联动攻击",
				needNimbus = 370500,
			},
			[19] = {
				linkRate = 0.095,
				attr = {
					physcAtk = 950,
					magicAtk = 950,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高950，9.5%几率进行联动攻击",
				needNimbus = 402000,
			},
			[20] = {
				linkRate = 0.1,
				attr = {
					physcAtk = 1000,
					magicAtk = 1000,
				},
				posList = {"2","3","5","6","8","9"}, 
				describe = "阵型中的战魂攻击提高1000，10.0%几率进行联动攻击",
			},
		},
	},
}

function GetTable()
	return autoTable
end
