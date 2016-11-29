--晶体矿升级表
 local autoTable = {
	[0] = {
		condt = {
			needMainLv = 1,
			needGold = 150,
		},
		timelong = 12,
		speed = 0,
		upLimit = 500000,
		describe = "晶石产量0/小时，容量上限500000。",
	},
	[1] = {
		condt = {
			needMainLv = 2,
			needGold = 220,
		},
		timelong = 18,
		speed = 1550,
		upLimit = 600000,
		describe = "晶石产量1550/小时，容量上限600000。",
	},
	[2] = {
		condt = {
			needMainLv = 3,
			needGold = 380,
		},
		timelong = 25,
		speed = 1680,
		upLimit = 700000,
		describe = "晶石产量1680/小时，容量上限700000。",
	},
	[3] = {
		condt = {
			needMainLv = 4,
			needGold = 620,
		},
		timelong = 33,
		speed = 1860,
		upLimit = 800000,
		describe = "晶石产量1860/小时，容量上限800000。",
	},
	[4] = {
		condt = {
			needMainLv = 5,
			needGold = 920,
		},
		timelong = 47,
		speed = 2060,
		upLimit = 900000,
		describe = "晶石产量2060/小时，容量上限900000。",
	},
	[5] = {
		condt = {
			needMainLv = 6,
			needGold = 1280,
		},
		timelong = 58,
		speed = 2290,
		upLimit = 1000000,
		describe = "晶石产量2290/小时，容量上限1000000。",
	},
	[6] = {
		condt = {
			needMainLv = 7,
			needGold = 1700,
		},
		timelong = 70,
		speed = 2520,
		upLimit = 1100000,
		describe = "晶石产量2520/小时，容量上限1100000。",
	},
	[7] = {
		condt = {
			needMainLv = 8,
			needGold = 2180,
		},
		timelong = 83,
		speed = 2770,
		upLimit = 1200000,
		describe = "晶石产量2770/小时，容量上限1200000。",
	},
	[8] = {
		condt = {
			needMainLv = 9,
			needGold = 2710,
		},
		timelong = 98,
		speed = 3030,
		upLimit = 1300000,
		describe = "晶石产量3030/小时，容量上限1300000。",
	},
	[9] = {
		condt = {
			needMainLv = 10,
			needGold = 3280,
		},
		timelong = 121,
		speed = 3300,
		upLimit = 1400000,
		describe = "晶石产量3300/小时，容量上限1400000。",
	},
	[10] = {
		condt = {
			needMainLv = 11,
			needGold = 3910,
		},
		timelong = 137,
		speed = 3580,
		upLimit = 1500000,
		describe = "晶石产量3580/小时，容量上限1500000。",
	},
	[11] = {
		condt = {
			needMainLv = 12,
			needGold = 4580,
		},
		timelong = 154,
		speed = 3860,
		upLimit = 1600000,
		describe = "晶石产量3860/小时，容量上限1600000。",
	},
	[12] = {
		condt = {
			needMainLv = 13,
			needGold = 5300,
		},
		timelong = 171,
		speed = 4150,
		upLimit = 1700000,
		describe = "晶石产量4150/小时，容量上限1700000。",
	},
	[13] = {
		condt = {
			needMainLv = 14,
			needGold = 6070,
		},
		timelong = 189,
		speed = 4450,
		upLimit = 1800000,
		describe = "晶石产量4450/小时，容量上限1800000。",
	},
	[14] = {
		condt = {
			needMainLv = 15,
			needGold = 6880,
		},
		timelong = 222,
		speed = 4760,
		upLimit = 1900000,
		describe = "晶石产量4760/小时，容量上限1900000。",
	},
	[15] = {
		condt = {
			needMainLv = 16,
			needGold = 7730,
		},
		timelong = 242,
		speed = 5070,
		upLimit = 2000000,
		describe = "晶石产量5070/小时，容量上限2000000。",
	},
	[16] = {
		condt = {
			needMainLv = 17,
			needGold = 8630,
		},
		timelong = 264,
		speed = 5390,
		upLimit = 2100000,
		describe = "晶石产量5390/小时，容量上限2100000。",
	},
	[17] = {
		condt = {
			needMainLv = 18,
			needGold = 9560,
		},
		timelong = 286,
		speed = 5710,
		upLimit = 2200000,
		describe = "晶石产量5710/小时，容量上限2200000。",
	},
	[18] = {
		condt = {
			needMainLv = 19,
			needGold = 10540,
		},
		timelong = 309,
		speed = 6040,
		upLimit = 2300000,
		describe = "晶石产量6040/小时，容量上限2300000。",
	},
	[19] = {
		condt = {
			needMainLv = 20,
			needGold = 11560,
		},
		timelong = 403,
		speed = 6370,
		upLimit = 2400000,
		describe = "晶石产量6370/小时，容量上限2400000。",
	},
	[20] = {
		condt = {
			needMainLv = 21,
			needGold = 12620,
		},
		timelong = 431,
		speed = 6710,
		upLimit = 2500000,
		describe = "晶石产量6710/小时，容量上限2500000。",
	},
	[21] = {
		condt = {
			needMainLv = 22,
			needGold = 13720,
		},
		timelong = 463,
		speed = 7050,
		upLimit = 2600000,
		describe = "晶石产量7050/小时，容量上限2600000。",
	},
	[22] = {
		condt = {
			needMainLv = 23,
			needGold = 14860,
		},
		timelong = 493,
		speed = 7390,
		upLimit = 2700000,
		describe = "晶石产量7390/小时，容量上限2700000。",
	},
	[23] = {
		condt = {
			needMainLv = 24,
			needGold = 16030,
		},
		timelong = 530,
		speed = 7740,
		upLimit = 2800000,
		describe = "晶石产量7740/小时，容量上限2800000。",
	},
	[24] = {
		condt = {
			needMainLv = 25,
			needGold = 17240,
		},
		timelong = 585,
		speed = 8100,
		upLimit = 2900000,
		describe = "晶石产量8100/小时，容量上限2900000。",
	},
	[25] = {
		condt = {
			needMainLv = 26,
			needGold = 18500,
		},
		timelong = 628,
		speed = 8460,
		upLimit = 3000000,
		describe = "晶石产量8460/小时，容量上限3000000。",
	},
	[26] = {
		condt = {
			needMainLv = 27,
			needGold = 19780,
		},
		timelong = 663,
		speed = 8820,
		upLimit = 3100000,
		describe = "晶石产量8820/小时，容量上限3100000。",
	},
	[27] = {
		condt = {
			needMainLv = 28,
			needGold = 21110,
		},
		timelong = 712,
		speed = 9180,
		upLimit = 3200000,
		describe = "晶石产量9180/小时，容量上限3200000。",
	},
	[28] = {
		condt = {
			needMainLv = 29,
			needGold = 22470,
		},
		timelong = 749,
		speed = 9550,
		upLimit = 3300000,
		describe = "晶石产量9550/小时，容量上限3300000。",
	},
	[29] = {
		condt = {
			needMainLv = 30,
			needGold = 23870,
		},
		timelong = 833,
		speed = 9920,
		upLimit = 3400000,
		describe = "晶石产量9920/小时，容量上限3400000。",
	},
	[30] = {
		condt = {
			needMainLv = 31,
			needGold = 25300,
		},
		timelong = 873,
		speed = 10300,
		upLimit = 3500000,
		describe = "晶石产量10300/小时，容量上限3500000。",
	},
	[31] = {
		condt = {
			needMainLv = 32,
			needGold = 26770,
		},
		timelong = 938,
		speed = 10680,
		upLimit = 3600000,
		describe = "晶石产量10680/小时，容量上限3600000。",
	},
	[32] = {
		condt = {
			needMainLv = 33,
			needGold = 28270,
		},
		timelong = 980,
		speed = 11060,
		upLimit = 3700000,
		describe = "晶石产量11060/小时，容量上限3700000。",
	},
	[33] = {
		condt = {
			needMainLv = 34,
			needGold = 29810,
		},
		timelong = 1053,
		speed = 11440,
		upLimit = 3800000,
		describe = "晶石产量11440/小时，容量上限3800000。",
	},
	[34] = {
		condt = {
			needMainLv = 35,
			needGold = 31380,
		},
		timelong = 1131,
		speed = 11830,
		upLimit = 3900000,
		describe = "晶石产量11830/小时，容量上限3900000。",
	},
	[35] = {
		condt = {
			needMainLv = 36,
			needGold = 32980,
		},
		timelong = 1213,
		speed = 12220,
		upLimit = 4000000,
		describe = "晶石产量12220/小时，容量上限4000000。",
	},
	[36] = {
		condt = {
			needMainLv = 37,
			needGold = 34620,
		},
		timelong = 1261,
		speed = 12620,
		upLimit = 4100000,
		describe = "晶石产量12620/小时，容量上限4100000。",
	},
	[37] = {
		condt = {
			needMainLv = 38,
			needGold = 36300,
		},
		timelong = 1311,
		speed = 13010,
		upLimit = 4200000,
		describe = "晶石产量13010/小时，容量上限4200000。",
	},
	[38] = {
		condt = {
			needMainLv = 39,
			needGold = 38010,
		},
		timelong = 1362,
		speed = 13410,
		upLimit = 4300000,
		describe = "晶石产量13410/小时，容量上限4300000。",
	},
	[39] = {
		condt = {
			needMainLv = 40,
			needGold = 39740,
		},
		timelong = 1587,
		speed = 13820,
		upLimit = 4400000,
		describe = "晶石产量13820/小时，容量上限4400000。",
	},
	[40] = {
		condt = {
			needMainLv = 41,
			needGold = 41520,
		},
		timelong = 1643,
		speed = 14220,
		upLimit = 4500000,
		describe = "晶石产量14220/小时，容量上限4500000。",
	},
	[41] = {
		condt = {
			needMainLv = 42,
			needGold = 43320,
		},
		timelong = 1701,
		speed = 14630,
		upLimit = 4600000,
		describe = "晶石产量14630/小时，容量上限4600000。",
	},
	[42] = {
		condt = {
			needMainLv = 43,
			needGold = 45160,
		},
		timelong = 1759,
		speed = 15040,
		upLimit = 4700000,
		describe = "晶石产量15040/小时，容量上限4700000。",
	},
	[43] = {
		condt = {
			needMainLv = 44,
			needGold = 47030,
		},
		timelong = 1819,
		speed = 15450,
		upLimit = 4800000,
		describe = "晶石产量15450/小时，容量上限4800000。",
	},
	[44] = {
		condt = {
			needMainLv = 45,
			needGold = 48930,
		},
		timelong = 1946,
		speed = 15860,
		upLimit = 4900000,
		describe = "晶石产量15860/小时，容量上限4900000。",
	},
	[45] = {
		condt = {
			needMainLv = 46,
			needGold = 50870,
		},
		timelong = 2008,
		speed = 16280,
		upLimit = 5000000,
		describe = "晶石产量16280/小时，容量上限5000000。",
	},
	[46] = {
		condt = {
			needMainLv = 47,
			needGold = 52830,
		},
		timelong = 2072,
		speed = 16700,
		upLimit = 5100000,
		describe = "晶石产量16700/小时，容量上限5100000。",
	},
	[47] = {
		condt = {
			needMainLv = 48,
			needGold = 54830,
		},
		timelong = 2197,
		speed = 17120,
		upLimit = 5200000,
		describe = "晶石产量17120/小时，容量上限5200000。",
	},
	[48] = {
		condt = {
			needMainLv = 49,
			needGold = 56860,
		},
		timelong = 2264,
		speed = 17550,
		upLimit = 5300000,
		describe = "晶石产量17550/小时，容量上限5300000。",
	},
	[49] = {
		condt = {
			needMainLv = 50,
			needGold = 58920,
		},
		timelong = 2382,
		speed = 17970,
		upLimit = 5400000,
		describe = "晶石产量17970/小时，容量上限5400000。",
	},
	[50] = {
		condt = {
			needMainLv = 51,
			needGold = 61010,
		},
		timelong = 2493,
		speed = 18400,
		upLimit = 5500000,
		describe = "晶石产量18400/小时，容量上限5500000。",
	},
	[51] = {
		condt = {
			needMainLv = 52,
			needGold = 63130,
		},
		timelong = 2582,
		speed = 18830,
		upLimit = 5600000,
		describe = "晶石产量18830/小时，容量上限5600000。",
	},
	[52] = {
		condt = {
			needMainLv = 53,
			needGold = 65280,
		},
		timelong = 2672,
		speed = 19270,
		upLimit = 5700000,
		describe = "晶石产量19270/小时，容量上限5700000。",
	},
	[53] = {
		condt = {
			needMainLv = 54,
			needGold = 67460,
		},
		timelong = 2764,
		speed = 19700,
		upLimit = 5800000,
		describe = "晶石产量19700/小时，容量上限5800000。",
	},
	[54] = {
		condt = {
			needMainLv = 55,
			needGold = 69670,
		},
		timelong = 2913,
		speed = 20140,
		upLimit = 5900000,
		describe = "晶石产量20140/小时，容量上限5900000。",
	},
	[55] = {
		condt = {
			needMainLv = 56,
			needGold = 71920,
		},
		timelong = 3091,
		speed = 20580,
		upLimit = 6000000,
		describe = "晶石产量20580/小时，容量上限6000000。",
	},
	[56] = {
		condt = {
			needMainLv = 57,
			needGold = 74190,
		},
		timelong = 3190,
		speed = 21020,
		upLimit = 6100000,
		describe = "晶石产量21020/小时，容量上限6100000。",
	},
	[57] = {
		condt = {
			needMainLv = 58,
			needGold = 76490,
		},
		timelong = 3291,
		speed = 21460,
		upLimit = 6200000,
		describe = "晶石产量21460/小时，容量上限6200000。",
	},
	[58] = {
		condt = {
			needMainLv = 59,
			needGold = 78820,
		},
		timelong = 3393,
		speed = 21910,
		upLimit = 6300000,
		describe = "晶石产量21910/小时，容量上限6300000。",
	},
	[59] = {
		condt = {
			needMainLv = 60,
			needGold = 81180,
		},
		timelong = 3832,
		speed = 22360,
		upLimit = 6400000,
		describe = "晶石产量22360/小时，容量上限6400000。",
	},
	[60] = {
		condt = {
			needMainLv = 61,
			needGold = 83580,
		},
		timelong = 3944,
		speed = 22810,
		upLimit = 6500000,
		describe = "晶石产量22810/小时，容量上限6500000。",
	},
	[61] = {
		condt = {
			needMainLv = 62,
			needGold = 86000,
		},
		timelong = 4058,
		speed = 23260,
		upLimit = 6600000,
		describe = "晶石产量23260/小时，容量上限6600000。",
	},
	[62] = {
		condt = {
			needMainLv = 63,
			needGold = 88440,
		},
		timelong = 4226,
		speed = 23710,
		upLimit = 6700000,
		describe = "晶石产量23710/小时，容量上限6700000。",
	},
	[63] = {
		condt = {
			needMainLv = 64,
			needGold = 90920,
		},
		timelong = 4291,
		speed = 24170,
		upLimit = 6800000,
		describe = "晶石产量24170/小时，容量上限6800000。",
	},
	[64] = {
		condt = {
			needMainLv = 65,
			needGold = 93430,
		},
		timelong = 4484,
		speed = 24620,
		upLimit = 6900000,
		describe = "晶石产量24620/小时，容量上限6900000。",
	},
	[65] = {
		condt = {
			needMainLv = 66,
			needGold = 95970,
		},
		timelong = 4606,
		speed = 25080,
		upLimit = 7000000,
		describe = "晶石产量25080/小时，容量上限7000000。",
	},
	[66] = {
		condt = {
			needMainLv = 67,
			needGold = 98530,
		},
		timelong = 4729,
		speed = 25540,
		upLimit = 7100000,
		describe = "晶石产量25540/小时，容量上限7100000。",
	},
	[67] = {
		condt = {
			needMainLv = 68,
			needGold = 101120,
		},
		timelong = 4973,
		speed = 26010,
		upLimit = 7200000,
		describe = "晶石产量26010/小时，容量上限7200000。",
	},
	[68] = {
		condt = {
			needMainLv = 69,
			needGold = 103740,
		},
		timelong = 5101,
		speed = 26470,
		upLimit = 7300000,
		describe = "晶石产量26470/小时，容量上限7300000。",
	},
	[69] = {
		condt = {
			needMainLv = 70,
			needGold = 106390,
		},
		timelong = 5413,
		speed = 26940,
		upLimit = 7400000,
		describe = "晶石产量26940/小时，容量上限7400000。",
	},
	[70] = {
		condt = {
			needMainLv = 71,
			needGold = 109070,
		},
		timelong = 5522,
		speed = 27400,
		upLimit = 7500000,
		describe = "晶石产量27400/小时，容量上限7500000。",
	},
	[71] = {
		condt = {
			needMainLv = 72,
			needGold = 111780,
		},
		timelong = 5633,
		speed = 27870,
		upLimit = 7600000,
		describe = "晶石产量27870/小时，容量上限7600000。",
	},
	[72] = {
		condt = {
			needMainLv = 73,
			needGold = 114510,
		},
		timelong = 5745,
		speed = 28340,
		upLimit = 7700000,
		describe = "晶石产量28340/小时，容量上限7700000。",
	},
	[73] = {
		condt = {
			needMainLv = 74,
			needGold = 117270,
		},
		timelong = 5857,
		speed = 28820,
		upLimit = 7800000,
		describe = "晶石产量28820/小时，容量上限7800000。",
	},
	[74] = {
		condt = {
			needMainLv = 75,
			needGold = 120060,
		},
		timelong = 6059,
		speed = 29290,
		upLimit = 7900000,
		describe = "晶石产量29290/小时，容量上限7900000。",
	},
	[75] = {
		condt = {
			needMainLv = 76,
			needGold = 122880,
		},
		timelong = 6329,
		speed = 29770,
		upLimit = 8000000,
		describe = "晶石产量29770/小时，容量上限8000000。",
	},
	[76] = {
		condt = {
			needMainLv = 77,
			needGold = 125730,
		},
		timelong = 6447,
		speed = 30240,
		upLimit = 8100000,
		describe = "晶石产量30240/小时，容量上限8100000。",
	},
	[77] = {
		condt = {
			needMainLv = 78,
			needGold = 128600,
		},
		timelong = 6566,
		speed = 30720,
		upLimit = 8200000,
		describe = "晶石产量30720/小时，容量上限8200000。",
	},
	[78] = {
		condt = {
			needMainLv = 79,
			needGold = 131500,
		},
		timelong = 6687,
		speed = 31200,
		upLimit = 8300000,
		describe = "晶石产量31200/小时，容量上限8300000。",
	},
	[79] = {
		condt = {
			needMainLv = 80,
			needGold = 134420,
		},
		timelong = 7350,
		speed = 31680,
		upLimit = 8400000,
		describe = "晶石产量31680/小时，容量上限8400000。",
	},
	[80] = {
		condt = {
			needMainLv = 81,
			needGold = 137380,
		},
		timelong = 7480,
		speed = 32170,
		upLimit = 8500000,
		describe = "晶石产量32170/小时，容量上限8500000。",
	},
	[81] = {
		condt = {
			needMainLv = 82,
			needGold = 140360,
		},
		timelong = 7611,
		speed = 32650,
		upLimit = 8600000,
		describe = "晶石产量32650/小时，容量上限8600000。",
	},
	[82] = {
		condt = {
			needMainLv = 83,
			needGold = 143370,
		},
		timelong = 7743,
		speed = 33140,
		upLimit = 8700000,
		describe = "晶石产量33140/小时，容量上限8700000。",
	},
	[83] = {
		condt = {
			needMainLv = 84,
			needGold = 146400,
		},
		timelong = 7876,
		speed = 33630,
		upLimit = 8800000,
		describe = "晶石产量33630/小时，容量上限8800000。",
	},
	[84] = {
		condt = {
			needMainLv = 85,
			needGold = 149460,
		},
		timelong = 8117,
		speed = 34120,
		upLimit = 8900000,
		describe = "晶石产量34120/小时，容量上限8900000。",
	},
	[85] = {
		condt = {
			needMainLv = 86,
			needGold = 152550,
		},
		timelong = 8253,
		speed = 34610,
		upLimit = 9000000,
		describe = "晶石产量34610/小时，容量上限9000000。",
	},
	[86] = {
		condt = {
			needMainLv = 87,
			needGold = 155670,
		},
		timelong = 8390,
		speed = 35100,
		upLimit = 9100000,
		describe = "晶石产量35100/小时，容量上限9100000。",
	},
	[87] = {
		condt = {
			needMainLv = 88,
			needGold = 158810,
		},
		timelong = 8728,
		speed = 35600,
		upLimit = 9200000,
		describe = "晶石产量35600/小时，容量上限9200000。",
	},
	[88] = {
		condt = {
			needMainLv = 89,
			needGold = 161980,
		},
		timelong = 8870,
		speed = 36090,
		upLimit = 9300000,
		describe = "晶石产量36090/小时，容量上限9300000。",
	},
	[89] = {
		condt = {
			needMainLv = 90,
			needGold = 165170,
		},
		timelong = 9128,
		speed = 36590,
		upLimit = 9400000,
		describe = "晶石产量36590/小时，容量上限9400000。",
	},
	[90] = {
		condt = {
			needMainLv = 91,
			needGold = 168390,
		},
		timelong = 9273,
		speed = 37090,
		upLimit = 9500000,
		describe = "晶石产量37090/小时，容量上限9500000。",
	},
	[91] = {
		condt = {
			needMainLv = 92,
			needGold = 171640,
		},
		timelong = 9418,
		speed = 37590,
		upLimit = 9600000,
		describe = "晶石产量37590/小时，容量上限9600000。",
	},
	[92] = {
		condt = {
			needMainLv = 93,
			needGold = 174910,
		},
		timelong = 9566,
		speed = 38090,
		upLimit = 9700000,
		describe = "晶石产量38090/小时，容量上限9700000。",
	},
	[93] = {
		condt = {
			needMainLv = 94,
			needGold = 178210,
		},
		timelong = 9713,
		speed = 38590,
		upLimit = 9800000,
		describe = "晶石产量38590/小时，容量上限9800000。",
	},
	[94] = {
		condt = {
			needMainLv = 95,
			needGold = 181540,
		},
		timelong = 9985,
		speed = 39090,
		upLimit = 9900000,
		describe = "晶石产量39090/小时，容量上限9900000。",
	},
	[95] = {
		condt = {
			needMainLv = 96,
			needGold = 184890,
		},
		timelong = 10388,
		speed = 39600,
		upLimit = 10000000,
		describe = "晶石产量39600/小时，容量上限10000000。",
	},
	[96] = {
		condt = {
			needMainLv = 97,
			needGold = 188270,
		},
		timelong = 10543,
		speed = 40110,
		upLimit = 10100000,
		describe = "晶石产量40110/小时，容量上限10100000。",
	},
	[97] = {
		condt = {
			needMainLv = 98,
			needGold = 191670,
		},
		timelong = 10699,
		speed = 40610,
		upLimit = 10200000,
		describe = "晶石产量40610/小时，容量上限10200000。",
	},
	[98] = {
		condt = {
			needMainLv = 99,
			needGold = 195100,
		},
		timelong = 10855,
		speed = 41120,
		upLimit = 10300000,
		describe = "晶石产量41120/小时，容量上限10300000。",
	},
	[99] = {
		condt = {
			needMainLv = 100,
			needGold = 198550,
		},
		timelong = 11963,
		speed = 41630,
		upLimit = 10400000,
		describe = "晶石产量41630/小时，容量上限10400000。",
	},
	[100] = {
		condt = {
		},
		speed = 42150,
		upLimit = 10500000,
		describe = "晶石产量42150/小时，容量上限10500000。",
	},
}
function GetTable()
	return autoTable
end
