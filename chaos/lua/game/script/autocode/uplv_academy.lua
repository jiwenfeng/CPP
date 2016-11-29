--学馆升级表
 local autoTable = {
	[0] = {
		condt = {
			needMainLv = 1,
			needGold = 140,
		},
		timelong = 11,
		uplimit = 0,
		describe = "科技等级可提升至0级。",
	},
	[1] = {
		condt = {
			needMainLv = 2,
			needGold = 210,
		},
		timelong = 16,
		uplimit = 1,
		describe = "科技等级可提升至1级。",
	},
	[2] = {
		condt = {
			needMainLv = 3,
			needGold = 370,
		},
		timelong = 23,
		uplimit = 2,
		describe = "科技等级可提升至2级。",
	},
	[3] = {
		condt = {
			needMainLv = 4,
			needGold = 600,
		},
		timelong = 30,
		uplimit = 3,
		describe = "科技等级可提升至3级。",
	},
	[4] = {
		condt = {
			needMainLv = 5,
			needGold = 890,
		},
		timelong = 44,
		uplimit = 4,
		describe = "科技等级可提升至4级。",
	},
	[5] = {
		condt = {
			needMainLv = 6,
			needGold = 1240,
		},
		timelong = 53,
		uplimit = 5,
		describe = "科技等级可提升至5级。",
	},
	[6] = {
		condt = {
			needMainLv = 7,
			needGold = 1640,
		},
		timelong = 65,
		uplimit = 6,
		describe = "科技等级可提升至6级。",
	},
	[7] = {
		condt = {
			needMainLv = 8,
			needGold = 2090,
		},
		timelong = 76,
		uplimit = 7,
		describe = "科技等级可提升至7级。",
	},
	[8] = {
		condt = {
			needMainLv = 9,
			needGold = 2590,
		},
		timelong = 90,
		uplimit = 8,
		describe = "科技等级可提升至8级。",
	},
	[9] = {
		condt = {
			needMainLv = 10,
			needGold = 3140,
		},
		timelong = 112,
		uplimit = 9,
		describe = "科技等级可提升至9级。",
	},
	[10] = {
		condt = {
			needMainLv = 11,
			needGold = 3730,
		},
		timelong = 126,
		uplimit = 10,
		describe = "科技等级可提升至10级。",
	},
	[11] = {
		condt = {
			needMainLv = 12,
			needGold = 4370,
		},
		timelong = 142,
		uplimit = 11,
		describe = "科技等级可提升至11级。",
	},
	[12] = {
		condt = {
			needMainLv = 13,
			needGold = 5040,
		},
		timelong = 158,
		uplimit = 12,
		describe = "科技等级可提升至12级。",
	},
	[13] = {
		condt = {
			needMainLv = 14,
			needGold = 5760,
		},
		timelong = 175,
		uplimit = 13,
		describe = "科技等级可提升至13级。",
	},
	[14] = {
		condt = {
			needMainLv = 15,
			needGold = 6520,
		},
		timelong = 205,
		uplimit = 14,
		describe = "科技等级可提升至14级。",
	},
	[15] = {
		condt = {
			needMainLv = 16,
			needGold = 7320,
		},
		timelong = 224,
		uplimit = 15,
		describe = "科技等级可提升至15级。",
	},
	[16] = {
		condt = {
			needMainLv = 17,
			needGold = 8160,
		},
		timelong = 244,
		uplimit = 16,
		describe = "科技等级可提升至16级。",
	},
	[17] = {
		condt = {
			needMainLv = 18,
			needGold = 9040,
		},
		timelong = 264,
		uplimit = 17,
		describe = "科技等级可提升至17级。",
	},
	[18] = {
		condt = {
			needMainLv = 19,
			needGold = 9950,
		},
		timelong = 286,
		uplimit = 18,
		describe = "科技等级可提升至18级。",
	},
	[19] = {
		condt = {
			needMainLv = 20,
			needGold = 10900,
		},
		timelong = 372,
		uplimit = 19,
		describe = "科技等级可提升至19级。",
	},
	[20] = {
		condt = {
			needMainLv = 21,
			needGold = 11890,
		},
		timelong = 398,
		uplimit = 20,
		describe = "科技等级可提升至20级。",
	},
	[21] = {
		condt = {
			needMainLv = 22,
			needGold = 12910,
		},
		timelong = 427,
		uplimit = 21,
		describe = "科技等级可提升至21级。",
	},
	[22] = {
		condt = {
			needMainLv = 23,
			needGold = 13960,
		},
		timelong = 455,
		uplimit = 22,
		describe = "科技等级可提升至22级。",
	},
	[23] = {
		condt = {
			needMainLv = 24,
			needGold = 15060,
		},
		timelong = 489,
		uplimit = 23,
		describe = "科技等级可提升至23级。",
	},
	[24] = {
		condt = {
			needMainLv = 25,
			needGold = 16180,
		},
		timelong = 540,
		uplimit = 24,
		describe = "科技等级可提升至24级。",
	},
	[25] = {
		condt = {
			needMainLv = 26,
			needGold = 17340,
		},
		timelong = 580,
		uplimit = 25,
		describe = "科技等级可提升至25级。",
	},
	[26] = {
		condt = {
			needMainLv = 27,
			needGold = 18540,
		},
		timelong = 612,
		uplimit = 26,
		describe = "科技等级可提升至26级。",
	},
	[27] = {
		condt = {
			needMainLv = 28,
			needGold = 19760,
		},
		timelong = 657,
		uplimit = 27,
		describe = "科技等级可提升至27级。",
	},
	[28] = {
		condt = {
			needMainLv = 29,
			needGold = 21020,
		},
		timelong = 692,
		uplimit = 28,
		describe = "科技等级可提升至28级。",
	},
	[29] = {
		condt = {
			needMainLv = 30,
			needGold = 22310,
		},
		timelong = 769,
		uplimit = 29,
		describe = "科技等级可提升至29级。",
	},
	[30] = {
		condt = {
			needMainLv = 31,
			needGold = 23630,
		},
		timelong = 806,
		uplimit = 30,
		describe = "科技等级可提升至30级。",
	},
	[31] = {
		condt = {
			needMainLv = 32,
			needGold = 24990,
		},
		timelong = 866,
		uplimit = 31,
		describe = "科技等级可提升至31级。",
	},
	[32] = {
		condt = {
			needMainLv = 33,
			needGold = 26380,
		},
		timelong = 905,
		uplimit = 32,
		describe = "科技等级可提升至32级。",
	},
	[33] = {
		condt = {
			needMainLv = 34,
			needGold = 27790,
		},
		timelong = 972,
		uplimit = 33,
		describe = "科技等级可提升至33级。",
	},
	[34] = {
		condt = {
			needMainLv = 35,
			needGold = 29240,
		},
		timelong = 1044,
		uplimit = 34,
		describe = "科技等级可提升至34级。",
	},
	[35] = {
		condt = {
			needMainLv = 36,
			needGold = 30720,
		},
		timelong = 1119,
		uplimit = 35,
		describe = "科技等级可提升至35级。",
	},
	[36] = {
		condt = {
			needMainLv = 37,
			needGold = 32230,
		},
		timelong = 1164,
		uplimit = 36,
		describe = "科技等级可提升至36级。",
	},
	[37] = {
		condt = {
			needMainLv = 38,
			needGold = 33770,
		},
		timelong = 1210,
		uplimit = 37,
		describe = "科技等级可提升至37级。",
	},
	[38] = {
		condt = {
			needMainLv = 39,
			needGold = 35340,
		},
		timelong = 1257,
		uplimit = 38,
		describe = "科技等级可提升至38级。",
	},
	[39] = {
		condt = {
			needMainLv = 40,
			needGold = 36940,
		},
		timelong = 1464,
		uplimit = 39,
		describe = "科技等级可提升至39级。",
	},
	[40] = {
		condt = {
			needMainLv = 41,
			needGold = 38570,
		},
		timelong = 1517,
		uplimit = 40,
		describe = "科技等级可提升至40级。",
	},
	[41] = {
		condt = {
			needMainLv = 42,
			needGold = 40220,
		},
		timelong = 1570,
		uplimit = 41,
		describe = "科技等级可提升至41级。",
	},
	[42] = {
		condt = {
			needMainLv = 43,
			needGold = 41910,
		},
		timelong = 1624,
		uplimit = 42,
		describe = "科技等级可提升至42级。",
	},
	[43] = {
		condt = {
			needMainLv = 44,
			needGold = 43620,
		},
		timelong = 1679,
		uplimit = 43,
		describe = "科技等级可提升至43级。",
	},
	[44] = {
		condt = {
			needMainLv = 45,
			needGold = 45370,
		},
		timelong = 1796,
		uplimit = 44,
		describe = "科技等级可提升至44级。",
	},
	[45] = {
		condt = {
			needMainLv = 46,
			needGold = 47140,
		},
		timelong = 1854,
		uplimit = 45,
		describe = "科技等级可提升至45级。",
	},
	[46] = {
		condt = {
			needMainLv = 47,
			needGold = 48940,
		},
		timelong = 1912,
		uplimit = 46,
		describe = "科技等级可提升至46级。",
	},
	[47] = {
		condt = {
			needMainLv = 48,
			needGold = 50770,
		},
		timelong = 2028,
		uplimit = 47,
		describe = "科技等级可提升至47级。",
	},
	[48] = {
		condt = {
			needMainLv = 49,
			needGold = 52620,
		},
		timelong = 2090,
		uplimit = 48,
		describe = "科技等级可提升至48级。",
	},
	[49] = {
		condt = {
			needMainLv = 50,
			needGold = 54510,
		},
		timelong = 2198,
		uplimit = 49,
		describe = "科技等级可提升至49级。",
	},
	[50] = {
		condt = {
			needMainLv = 51,
			needGold = 56420,
		},
		timelong = 2301,
		uplimit = 50,
		describe = "科技等级可提升至50级。",
	},
	[51] = {
		condt = {
			needMainLv = 52,
			needGold = 58360,
		},
		timelong = 2383,
		uplimit = 51,
		describe = "科技等级可提升至51级。",
	},
	[52] = {
		condt = {
			needMainLv = 53,
			needGold = 60320,
		},
		timelong = 2466,
		uplimit = 52,
		describe = "科技等级可提升至52级。",
	},
	[53] = {
		condt = {
			needMainLv = 54,
			needGold = 62310,
		},
		timelong = 2551,
		uplimit = 53,
		describe = "科技等级可提升至53级。",
	},
	[54] = {
		condt = {
			needMainLv = 55,
			needGold = 64330,
		},
		timelong = 2689,
		uplimit = 54,
		describe = "科技等级可提升至54级。",
	},
	[55] = {
		condt = {
			needMainLv = 56,
			needGold = 66380,
		},
		timelong = 2854,
		uplimit = 55,
		describe = "科技等级可提升至55级。",
	},
	[56] = {
		condt = {
			needMainLv = 57,
			needGold = 68450,
		},
		timelong = 2945,
		uplimit = 56,
		describe = "科技等级可提升至56级。",
	},
	[57] = {
		condt = {
			needMainLv = 58,
			needGold = 70550,
		},
		timelong = 3038,
		uplimit = 57,
		describe = "科技等级可提升至57级。",
	},
	[58] = {
		condt = {
			needMainLv = 59,
			needGold = 72680,
		},
		timelong = 3132,
		uplimit = 58,
		describe = "科技等级可提升至58级。",
	},
	[59] = {
		condt = {
			needMainLv = 60,
			needGold = 74830,
		},
		timelong = 3537,
		uplimit = 59,
		describe = "科技等级可提升至59级。",
	},
	[60] = {
		condt = {
			needMainLv = 61,
			needGold = 77010,
		},
		timelong = 3641,
		uplimit = 60,
		describe = "科技等级可提升至60级。",
	},
	[61] = {
		condt = {
			needMainLv = 62,
			needGold = 79210,
		},
		timelong = 3746,
		uplimit = 61,
		describe = "科技等级可提升至61级。",
	},
	[62] = {
		condt = {
			needMainLv = 63,
			needGold = 81440,
		},
		timelong = 3901,
		uplimit = 62,
		describe = "科技等级可提升至62级。",
	},
	[63] = {
		condt = {
			needMainLv = 64,
			needGold = 83690,
		},
		timelong = 3961,
		uplimit = 63,
		describe = "科技等级可提升至63级。",
	},
	[64] = {
		condt = {
			needMainLv = 65,
			needGold = 85980,
		},
		timelong = 4139,
		uplimit = 64,
		describe = "科技等级可提升至64级。",
	},
	[65] = {
		condt = {
			needMainLv = 66,
			needGold = 88280,
		},
		timelong = 4251,
		uplimit = 65,
		describe = "科技等级可提升至65级。",
	},
	[66] = {
		condt = {
			needMainLv = 67,
			needGold = 90610,
		},
		timelong = 4365,
		uplimit = 66,
		describe = "科技等级可提升至66级。",
	},
	[67] = {
		condt = {
			needMainLv = 68,
			needGold = 92970,
		},
		timelong = 4590,
		uplimit = 67,
		describe = "科技等级可提升至67级。",
	},
	[68] = {
		condt = {
			needMainLv = 69,
			needGold = 95350,
		},
		timelong = 4709,
		uplimit = 68,
		describe = "科技等级可提升至68级。",
	},
	[69] = {
		condt = {
			needMainLv = 70,
			needGold = 97760,
		},
		timelong = 4997,
		uplimit = 69,
		describe = "科技等级可提升至69级。",
	},
	[70] = {
		condt = {
			needMainLv = 71,
			needGold = 100190,
		},
		timelong = 5098,
		uplimit = 70,
		describe = "科技等级可提升至70级。",
	},
	[71] = {
		condt = {
			needMainLv = 72,
			needGold = 102650,
		},
		timelong = 5200,
		uplimit = 71,
		describe = "科技等级可提升至71级。",
	},
	[72] = {
		condt = {
			needMainLv = 73,
			needGold = 105130,
		},
		timelong = 5303,
		uplimit = 72,
		describe = "科技等级可提升至72级。",
	},
	[73] = {
		condt = {
			needMainLv = 74,
			needGold = 107630,
		},
		timelong = 5406,
		uplimit = 73,
		describe = "科技等级可提升至73级。",
	},
	[74] = {
		condt = {
			needMainLv = 75,
			needGold = 110160,
		},
		timelong = 5593,
		uplimit = 74,
		describe = "科技等级可提升至74级。",
	},
	[75] = {
		condt = {
			needMainLv = 76,
			needGold = 112720,
		},
		timelong = 5842,
		uplimit = 75,
		describe = "科技等级可提升至75级。",
	},
	[76] = {
		condt = {
			needMainLv = 77,
			needGold = 115300,
		},
		timelong = 5951,
		uplimit = 76,
		describe = "科技等级可提升至76级。",
	},
	[77] = {
		condt = {
			needMainLv = 78,
			needGold = 117900,
		},
		timelong = 6061,
		uplimit = 77,
		describe = "科技等级可提升至77级。",
	},
	[78] = {
		condt = {
			needMainLv = 79,
			needGold = 120530,
		},
		timelong = 6172,
		uplimit = 78,
		describe = "科技等级可提升至78级。",
	},
	[79] = {
		condt = {
			needMainLv = 80,
			needGold = 123180,
		},
		timelong = 6784,
		uplimit = 79,
		describe = "科技等级可提升至79级。",
	},
	[80] = {
		condt = {
			needMainLv = 81,
			needGold = 125850,
		},
		timelong = 6904,
		uplimit = 80,
		describe = "科技等级可提升至80级。",
	},
	[81] = {
		condt = {
			needMainLv = 82,
			needGold = 128550,
		},
		timelong = 7025,
		uplimit = 81,
		describe = "科技等级可提升至81级。",
	},
	[82] = {
		condt = {
			needMainLv = 83,
			needGold = 131280,
		},
		timelong = 7147,
		uplimit = 82,
		describe = "科技等级可提升至82级。",
	},
	[83] = {
		condt = {
			needMainLv = 84,
			needGold = 134020,
		},
		timelong = 7270,
		uplimit = 83,
		describe = "科技等级可提升至83级。",
	},
	[84] = {
		condt = {
			needMainLv = 85,
			needGold = 136790,
		},
		timelong = 7492,
		uplimit = 84,
		describe = "科技等级可提升至84级。",
	},
	[85] = {
		condt = {
			needMainLv = 86,
			needGold = 139590,
		},
		timelong = 7618,
		uplimit = 85,
		describe = "科技等级可提升至85级。",
	},
	[86] = {
		condt = {
			needMainLv = 87,
			needGold = 142400,
		},
		timelong = 7745,
		uplimit = 86,
		describe = "科技等级可提升至86级。",
	},
	[87] = {
		condt = {
			needMainLv = 88,
			needGold = 145240,
		},
		timelong = 8057,
		uplimit = 87,
		describe = "科技等级可提升至87级。",
	},
	[88] = {
		condt = {
			needMainLv = 89,
			needGold = 148110,
		},
		timelong = 8188,
		uplimit = 88,
		describe = "科技等级可提升至88级。",
	},
	[89] = {
		condt = {
			needMainLv = 90,
			needGold = 150990,
		},
		timelong = 8426,
		uplimit = 89,
		describe = "科技等级可提升至89级。",
	},
	[90] = {
		condt = {
			needMainLv = 91,
			needGold = 153900,
		},
		timelong = 8560,
		uplimit = 90,
		describe = "科技等级可提升至90级。",
	},
	[91] = {
		condt = {
			needMainLv = 92,
			needGold = 156840,
		},
		timelong = 8694,
		uplimit = 91,
		describe = "科技等级可提升至91级。",
	},
	[92] = {
		condt = {
			needMainLv = 93,
			needGold = 159790,
		},
		timelong = 8830,
		uplimit = 92,
		describe = "科技等级可提升至92级。",
	},
	[93] = {
		condt = {
			needMainLv = 94,
			needGold = 162770,
		},
		timelong = 8966,
		uplimit = 93,
		describe = "科技等级可提升至93级。",
	},
	[94] = {
		condt = {
			needMainLv = 95,
			needGold = 165770,
		},
		timelong = 9217,
		uplimit = 94,
		describe = "科技等级可提升至94级。",
	},
	[95] = {
		condt = {
			needMainLv = 96,
			needGold = 168800,
		},
		timelong = 9589,
		uplimit = 95,
		describe = "科技等级可提升至95级。",
	},
	[96] = {
		condt = {
			needMainLv = 97,
			needGold = 171840,
		},
		timelong = 9732,
		uplimit = 96,
		describe = "科技等级可提升至96级。",
	},
	[97] = {
		condt = {
			needMainLv = 98,
			needGold = 174910,
		},
		timelong = 9876,
		uplimit = 97,
		describe = "科技等级可提升至97级。",
	},
	[98] = {
		condt = {
			needMainLv = 99,
			needGold = 178010,
		},
		timelong = 10020,
		uplimit = 98,
		describe = "科技等级可提升至98级。",
	},
	[99] = {
		condt = {
			needMainLv = 100,
			needGold = 181120,
		},
		timelong = 11043,
		uplimit = 99,
		describe = "科技等级可提升至99级。",
	},
	[100] = {
		condt = {
		},
		uplimit = 100,
		describe = "科技等级可提升至100级。",
	},
}
function GetTable()
	return autoTable
end
