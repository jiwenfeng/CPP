--兵营升级表
 local autoTable = {
	[0] = {
		condt = {
			needMainLv = 1,
			needGold = 160,
		},
		timelong = 13,
		speed = 0,
		expAdd = 0,
		describe = "后备兵力增加0/时，战魂训练经验提升。",
	},
	[1] = {
		condt = {
			needMainLv = 2,
			needGold = 240,
		},
		timelong = 19,
		speed = 7800,
		expAdd = 0.003,
		describe = "后备兵力增加7800/时，战魂训练经验提升。",
	},
	[2] = {
		condt = {
			needMainLv = 3,
			needGold = 420,
		},
		timelong = 27,
		speed = 8100,
		expAdd = 0.006,
		describe = "后备兵力增加8100/时，战魂训练经验提升。",
	},
	[3] = {
		condt = {
			needMainLv = 4,
			needGold = 690,
		},
		timelong = 35,
		speed = 8400,
		expAdd = 0.009,
		describe = "后备兵力增加8400/时，战魂训练经验提升。",
	},
	[4] = {
		condt = {
			needMainLv = 5,
			needGold = 1030,
		},
		timelong = 51,
		speed = 8700,
		expAdd = 0.012,
		describe = "后备兵力增加8700/时，战魂训练经验提升。",
	},
	[5] = {
		condt = {
			needMainLv = 6,
			needGold = 1430,
		},
		timelong = 62,
		speed = 9000,
		expAdd = 0.015,
		describe = "后备兵力增加9000/时，战魂训练经验提升。",
	},
	[6] = {
		condt = {
			needMainLv = 7,
			needGold = 1900,
		},
		timelong = 76,
		speed = 9300,
		expAdd = 0.018,
		describe = "后备兵力增加9300/时，战魂训练经验提升。",
	},
	[7] = {
		condt = {
			needMainLv = 8,
			needGold = 2430,
		},
		timelong = 89,
		speed = 9600,
		expAdd = 0.021,
		describe = "后备兵力增加9600/时，战魂训练经验提升。",
	},
	[8] = {
		condt = {
			needMainLv = 9,
			needGold = 3020,
		},
		timelong = 106,
		speed = 9900,
		expAdd = 0.024,
		describe = "后备兵力增加9900/时，战魂训练经验提升。",
	},
	[9] = {
		condt = {
			needMainLv = 10,
			needGold = 3660,
		},
		timelong = 131,
		speed = 10200,
		expAdd = 0.027,
		describe = "后备兵力增加10200/时，战魂训练经验提升。",
	},
	[10] = {
		condt = {
			needMainLv = 11,
			needGold = 4360,
		},
		timelong = 148,
		speed = 10500,
		expAdd = 0.03,
		describe = "后备兵力增加10500/时，战魂训练经验提升。",
	},
	[11] = {
		condt = {
			needMainLv = 12,
			needGold = 5110,
		},
		timelong = 165,
		speed = 10800,
		expAdd = 0.033,
		describe = "后备兵力增加10800/时，战魂训练经验提升。",
	},
	[12] = {
		condt = {
			needMainLv = 13,
			needGold = 5900,
		},
		timelong = 184,
		speed = 11100,
		expAdd = 0.036,
		describe = "后备兵力增加11100/时，战魂训练经验提升。",
	},
	[13] = {
		condt = {
			needMainLv = 14,
			needGold = 6750,
		},
		timelong = 204,
		speed = 11400,
		expAdd = 0.039,
		describe = "后备兵力增加11400/时，战魂训练经验提升。",
	},
	[14] = {
		condt = {
			needMainLv = 15,
			needGold = 7650,
		},
		timelong = 239,
		speed = 11700,
		expAdd = 0.042,
		describe = "后备兵力增加11700/时，战魂训练经验提升。",
	},
	[15] = {
		condt = {
			needMainLv = 16,
			needGold = 8590,
		},
		timelong = 261,
		speed = 12000,
		expAdd = 0.045,
		describe = "后备兵力增加12000/时，战魂训练经验提升。",
	},
	[16] = {
		condt = {
			needMainLv = 17,
			needGold = 9580,
		},
		timelong = 284,
		speed = 12300,
		expAdd = 0.048,
		describe = "后备兵力增加12300/时，战魂训练经验提升。",
	},
	[17] = {
		condt = {
			needMainLv = 18,
			needGold = 10620,
		},
		timelong = 308,
		speed = 12600,
		expAdd = 0.051,
		describe = "后备兵力增加12600/时，战魂训练经验提升。",
	},
	[18] = {
		condt = {
			needMainLv = 19,
			needGold = 11700,
		},
		timelong = 333,
		speed = 12900,
		expAdd = 0.054,
		describe = "后备兵力增加12900/时，战魂训练经验提升。",
	},
	[19] = {
		condt = {
			needMainLv = 20,
			needGold = 12820,
		},
		timelong = 434,
		speed = 13200,
		expAdd = 0.057,
		describe = "后备兵力增加13200/时，战魂训练经验提升。",
	},
	[20] = {
		condt = {
			needMainLv = 21,
			needGold = 13990,
		},
		timelong = 464,
		speed = 13500,
		expAdd = 0.06,
		describe = "后备兵力增加13500/时，战魂训练经验提升。",
	},
	[21] = {
		condt = {
			needMainLv = 22,
			needGold = 15200,
		},
		timelong = 498,
		speed = 13800,
		expAdd = 0.063,
		describe = "后备兵力增加13800/时，战魂训练经验提升。",
	},
	[22] = {
		condt = {
			needMainLv = 23,
			needGold = 16460,
		},
		timelong = 531,
		speed = 14100,
		expAdd = 0.066,
		describe = "后备兵力增加14100/时，战魂训练经验提升。",
	},
	[23] = {
		condt = {
			needMainLv = 24,
			needGold = 17750,
		},
		timelong = 571,
		speed = 14400,
		expAdd = 0.069,
		describe = "后备兵力增加14400/时，战魂训练经验提升。",
	},
	[24] = {
		condt = {
			needMainLv = 25,
			needGold = 19090,
		},
		timelong = 630,
		speed = 14700,
		expAdd = 0.072,
		describe = "后备兵力增加14700/时，战魂训练经验提升。",
	},
	[25] = {
		condt = {
			needMainLv = 26,
			needGold = 20460,
		},
		timelong = 676,
		speed = 15000,
		expAdd = 0.075,
		describe = "后备兵力增加15000/时，战魂训练经验提升。",
	},
	[26] = {
		condt = {
			needMainLv = 27,
			needGold = 21880,
		},
		timelong = 714,
		speed = 15300,
		expAdd = 0.078,
		describe = "后备兵力增加15300/时，战魂训练经验提升。",
	},
	[27] = {
		condt = {
			needMainLv = 28,
			needGold = 23340,
		},
		timelong = 767,
		speed = 15600,
		expAdd = 0.081,
		describe = "后备兵力增加15600/时，战魂训练经验提升。",
	},
	[28] = {
		condt = {
			needMainLv = 29,
			needGold = 24830,
		},
		timelong = 807,
		speed = 15900,
		expAdd = 0.084,
		describe = "后备兵力增加15900/时，战魂训练经验提升。",
	},
	[29] = {
		condt = {
			needMainLv = 30,
			needGold = 26370,
		},
		timelong = 897,
		speed = 16200,
		expAdd = 0.087,
		describe = "后备兵力增加16200/时，战魂训练经验提升。",
	},
	[30] = {
		condt = {
			needMainLv = 31,
			needGold = 27940,
		},
		timelong = 941,
		speed = 16500,
		expAdd = 0.09,
		describe = "后备兵力增加16500/时，战魂训练经验提升。",
	},
	[31] = {
		condt = {
			needMainLv = 32,
			needGold = 29550,
		},
		timelong = 1010,
		speed = 16800,
		expAdd = 0.093,
		describe = "后备兵力增加16800/时，战魂训练经验提升。",
	},
	[32] = {
		condt = {
			needMainLv = 33,
			needGold = 31200,
		},
		timelong = 1056,
		speed = 17100,
		expAdd = 0.096,
		describe = "后备兵力增加17100/时，战魂训练经验提升。",
	},
	[33] = {
		condt = {
			needMainLv = 34,
			needGold = 32890,
		},
		timelong = 1134,
		speed = 17400,
		expAdd = 0.099,
		describe = "后备兵力增加17400/时，战魂训练经验提升。",
	},
	[34] = {
		condt = {
			needMainLv = 35,
			needGold = 34610,
		},
		timelong = 1218,
		speed = 17700,
		expAdd = 0.102,
		describe = "后备兵力增加17700/时，战魂训练经验提升。",
	},
	[35] = {
		condt = {
			needMainLv = 36,
			needGold = 36370,
		},
		timelong = 1306,
		speed = 18000,
		expAdd = 0.105,
		describe = "后备兵力增加18000/时，战魂训练经验提升。",
	},
	[36] = {
		condt = {
			needMainLv = 37,
			needGold = 38170,
		},
		timelong = 1358,
		speed = 18300,
		expAdd = 0.108,
		describe = "后备兵力增加18300/时，战魂训练经验提升。",
	},
	[37] = {
		condt = {
			needMainLv = 38,
			needGold = 40010,
		},
		timelong = 1412,
		speed = 18600,
		expAdd = 0.111,
		describe = "后备兵力增加18600/时，战魂训练经验提升。",
	},
	[38] = {
		condt = {
			needMainLv = 39,
			needGold = 41880,
		},
		timelong = 1467,
		speed = 18900,
		expAdd = 0.114,
		describe = "后备兵力增加18900/时，战魂训练经验提升。",
	},
	[39] = {
		condt = {
			needMainLv = 40,
			needGold = 43780,
		},
		timelong = 1709,
		speed = 19200,
		expAdd = 0.117,
		describe = "后备兵力增加19200/时，战魂训练经验提升。",
	},
	[40] = {
		condt = {
			needMainLv = 41,
			needGold = 45730,
		},
		timelong = 1770,
		speed = 19500,
		expAdd = 0.12,
		describe = "后备兵力增加19500/时，战魂训练经验提升。",
	},
	[41] = {
		condt = {
			needMainLv = 42,
			needGold = 47700,
		},
		timelong = 1832,
		speed = 19800,
		expAdd = 0.123,
		describe = "后备兵力增加19800/时，战魂训练经验提升。",
	},
	[42] = {
		condt = {
			needMainLv = 43,
			needGold = 49710,
		},
		timelong = 1895,
		speed = 20100,
		expAdd = 0.126,
		describe = "后备兵力增加20100/时，战魂训练经验提升。",
	},
	[43] = {
		condt = {
			needMainLv = 44,
			needGold = 51760,
		},
		timelong = 1959,
		speed = 20400,
		expAdd = 0.129,
		describe = "后备兵力增加20400/时，战魂训练经验提升。",
	},
	[44] = {
		condt = {
			needMainLv = 45,
			needGold = 53840,
		},
		timelong = 2095,
		speed = 20700,
		expAdd = 0.132,
		describe = "后备兵力增加20700/时，战魂训练经验提升。",
	},
	[45] = {
		condt = {
			needMainLv = 46,
			needGold = 55960,
		},
		timelong = 2163,
		speed = 21000,
		expAdd = 0.135,
		describe = "后备兵力增加21000/时，战魂训练经验提升。",
	},
	[46] = {
		condt = {
			needMainLv = 47,
			needGold = 58110,
		},
		timelong = 2231,
		speed = 21300,
		expAdd = 0.138,
		describe = "后备兵力增加21300/时，战魂训练经验提升。",
	},
	[47] = {
		condt = {
			needMainLv = 48,
			needGold = 60290,
		},
		timelong = 2366,
		speed = 21600,
		expAdd = 0.141,
		describe = "后备兵力增加21600/时，战魂训练经验提升。",
	},
	[48] = {
		condt = {
			needMainLv = 49,
			needGold = 62510,
		},
		timelong = 2438,
		speed = 21900,
		expAdd = 0.144,
		describe = "后备兵力增加21900/时，战魂训练经验提升。",
	},
	[49] = {
		condt = {
			needMainLv = 50,
			needGold = 64760,
		},
		timelong = 2565,
		speed = 22200,
		expAdd = 0.147,
		describe = "后备兵力增加22200/时，战魂训练经验提升。",
	},
	[50] = {
		condt = {
			needMainLv = 51,
			needGold = 67040,
		},
		timelong = 2685,
		speed = 22500,
		expAdd = 0.15,
		describe = "后备兵力增加22500/时，战魂训练经验提升。",
	},
	[51] = {
		condt = {
			needMainLv = 52,
			needGold = 69360,
		},
		timelong = 2780,
		speed = 22800,
		expAdd = 0.153,
		describe = "后备兵力增加22800/时，战魂训练经验提升。",
	},
	[52] = {
		condt = {
			needMainLv = 53,
			needGold = 71710,
		},
		timelong = 2877,
		speed = 23100,
		expAdd = 0.156,
		describe = "后备兵力增加23100/时，战魂训练经验提升。",
	},
	[53] = {
		condt = {
			needMainLv = 54,
			needGold = 74090,
		},
		timelong = 2976,
		speed = 23400,
		expAdd = 0.159,
		describe = "后备兵力增加23400/时，战魂训练经验提升。",
	},
	[54] = {
		condt = {
			needMainLv = 55,
			needGold = 76510,
		},
		timelong = 3138,
		speed = 23700,
		expAdd = 0.162,
		describe = "后备兵力增加23700/时，战魂训练经验提升。",
	},
	[55] = {
		condt = {
			needMainLv = 56,
			needGold = 78960,
		},
		timelong = 3329,
		speed = 24000,
		expAdd = 0.165,
		describe = "后备兵力增加24000/时，战魂训练经验提升。",
	},
	[56] = {
		condt = {
			needMainLv = 57,
			needGold = 81440,
		},
		timelong = 3436,
		speed = 24300,
		expAdd = 0.168,
		describe = "后备兵力增加24300/时，战魂训练经验提升。",
	},
	[57] = {
		condt = {
			needMainLv = 58,
			needGold = 83950,
		},
		timelong = 3544,
		speed = 24600,
		expAdd = 0.171,
		describe = "后备兵力增加24600/时，战魂训练经验提升。",
	},
	[58] = {
		condt = {
			needMainLv = 59,
			needGold = 86490,
		},
		timelong = 3654,
		speed = 24900,
		expAdd = 0.174,
		describe = "后备兵力增加24900/时，战魂训练经验提升。",
	},
	[59] = {
		condt = {
			needMainLv = 60,
			needGold = 89070,
		},
		timelong = 4126,
		speed = 25200,
		expAdd = 0.177,
		describe = "后备兵力增加25200/时，战魂训练经验提升。",
	},
	[60] = {
		condt = {
			needMainLv = 61,
			needGold = 91680,
		},
		timelong = 4248,
		speed = 25500,
		expAdd = 0.18,
		describe = "后备兵力增加25500/时，战魂训练经验提升。",
	},
	[61] = {
		condt = {
			needMainLv = 62,
			needGold = 94320,
		},
		timelong = 4371,
		speed = 25800,
		expAdd = 0.183,
		describe = "后备兵力增加25800/时，战魂训练经验提升。",
	},
	[62] = {
		condt = {
			needMainLv = 63,
			needGold = 96990,
		},
		timelong = 4552,
		speed = 26100,
		expAdd = 0.186,
		describe = "后备兵力增加26100/时，战魂训练经验提升。",
	},
	[63] = {
		condt = {
			needMainLv = 64,
			needGold = 99690,
		},
		timelong = 4621,
		speed = 26400,
		expAdd = 0.189,
		describe = "后备兵力增加26400/时，战魂训练经验提升。",
	},
	[64] = {
		condt = {
			needMainLv = 65,
			needGold = 102420,
		},
		timelong = 4829,
		speed = 26700,
		expAdd = 0.192,
		describe = "后备兵力增加26700/时，战魂训练经验提升。",
	},
	[65] = {
		condt = {
			needMainLv = 66,
			needGold = 105190,
		},
		timelong = 4960,
		speed = 27000,
		expAdd = 0.195,
		describe = "后备兵力增加27000/时，战魂训练经验提升。",
	},
	[66] = {
		condt = {
			needMainLv = 67,
			needGold = 107980,
		},
		timelong = 5093,
		speed = 27300,
		expAdd = 0.198,
		describe = "后备兵力增加27300/时，战魂训练经验提升。",
	},
	[67] = {
		condt = {
			needMainLv = 68,
			needGold = 110810,
		},
		timelong = 5355,
		speed = 27600,
		expAdd = 0.201,
		describe = "后备兵力增加27600/时，战魂训练经验提升。",
	},
	[68] = {
		condt = {
			needMainLv = 69,
			needGold = 113660,
		},
		timelong = 5494,
		speed = 27900,
		expAdd = 0.204,
		describe = "后备兵力增加27900/时，战魂训练经验提升。",
	},
	[69] = {
		condt = {
			needMainLv = 70,
			needGold = 116550,
		},
		timelong = 5829,
		speed = 28200,
		expAdd = 0.207,
		describe = "后备兵力增加28200/时，战魂训练经验提升。",
	},
	[70] = {
		condt = {
			needMainLv = 71,
			needGold = 119460,
		},
		timelong = 5947,
		speed = 28500,
		expAdd = 0.21,
		describe = "后备兵力增加28500/时，战魂训练经验提升。",
	},
	[71] = {
		condt = {
			needMainLv = 72,
			needGold = 122410,
		},
		timelong = 6066,
		speed = 28800,
		expAdd = 0.213,
		describe = "后备兵力增加28800/时，战魂训练经验提升。",
	},
	[72] = {
		condt = {
			needMainLv = 73,
			needGold = 125390,
		},
		timelong = 6186,
		speed = 29100,
		expAdd = 0.216,
		describe = "后备兵力增加29100/时，战魂训练经验提升。",
	},
	[73] = {
		condt = {
			needMainLv = 74,
			needGold = 128390,
		},
		timelong = 6307,
		speed = 29400,
		expAdd = 0.219,
		describe = "后备兵力增加29400/时，战魂训练经验提升。",
	},
	[74] = {
		condt = {
			needMainLv = 75,
			needGold = 131430,
		},
		timelong = 6525,
		speed = 29700,
		expAdd = 0.222,
		describe = "后备兵力增加29700/时，战魂训练经验提升。",
	},
	[75] = {
		condt = {
			needMainLv = 76,
			needGold = 134500,
		},
		timelong = 6815,
		speed = 30000,
		expAdd = 0.225,
		describe = "后备兵力增加30000/时，战魂训练经验提升。",
	},
	[76] = {
		condt = {
			needMainLv = 77,
			needGold = 137590,
		},
		timelong = 6943,
		speed = 30300,
		expAdd = 0.228,
		describe = "后备兵力增加30300/时，战魂训练经验提升。",
	},
	[77] = {
		condt = {
			needMainLv = 78,
			needGold = 140720,
		},
		timelong = 7071,
		speed = 30600,
		expAdd = 0.231,
		describe = "后备兵力增加30600/时，战魂训练经验提升。",
	},
	[78] = {
		condt = {
			needMainLv = 79,
			needGold = 143870,
		},
		timelong = 7201,
		speed = 30900,
		expAdd = 0.234,
		describe = "后备兵力增加30900/时，战魂训练经验提升。",
	},
	[79] = {
		condt = {
			needMainLv = 80,
			needGold = 147060,
		},
		timelong = 7915,
		speed = 31200,
		expAdd = 0.237,
		describe = "后备兵力增加31200/时，战魂训练经验提升。",
	},
	[80] = {
		condt = {
			needMainLv = 81,
			needGold = 150270,
		},
		timelong = 8055,
		speed = 31500,
		expAdd = 0.24,
		describe = "后备兵力增加31500/时，战魂训练经验提升。",
	},
	[81] = {
		condt = {
			needMainLv = 82,
			needGold = 153510,
		},
		timelong = 8196,
		speed = 31800,
		expAdd = 0.243,
		describe = "后备兵力增加31800/时，战魂训练经验提升。",
	},
	[82] = {
		condt = {
			needMainLv = 83,
			needGold = 156780,
		},
		timelong = 8338,
		speed = 32100,
		expAdd = 0.246,
		describe = "后备兵力增加32100/时，战魂训练经验提升。",
	},
	[83] = {
		condt = {
			needMainLv = 84,
			needGold = 160080,
		},
		timelong = 8482,
		speed = 32400,
		expAdd = 0.249,
		describe = "后备兵力增加32400/时，战魂训练经验提升。",
	},
	[84] = {
		condt = {
			needMainLv = 85,
			needGold = 163410,
		},
		timelong = 8741,
		speed = 32700,
		expAdd = 0.252,
		describe = "后备兵力增加32700/时，战魂训练经验提升。",
	},
	[85] = {
		condt = {
			needMainLv = 86,
			needGold = 166770,
		},
		timelong = 8888,
		speed = 33000,
		expAdd = 0.255,
		describe = "后备兵力增加33000/时，战魂训练经验提升。",
	},
	[86] = {
		condt = {
			needMainLv = 87,
			needGold = 170150,
		},
		timelong = 9036,
		speed = 33300,
		expAdd = 0.258,
		describe = "后备兵力增加33300/时，战魂训练经验提升。",
	},
	[87] = {
		condt = {
			needMainLv = 88,
			needGold = 173570,
		},
		timelong = 9400,
		speed = 33600,
		expAdd = 0.261,
		describe = "后备兵力增加33600/时，战魂训练经验提升。",
	},
	[88] = {
		condt = {
			needMainLv = 89,
			needGold = 177010,
		},
		timelong = 9552,
		speed = 33900,
		expAdd = 0.264,
		describe = "后备兵力增加33900/时，战魂训练经验提升。",
	},
	[89] = {
		condt = {
			needMainLv = 90,
			needGold = 180480,
		},
		timelong = 9830,
		speed = 34200,
		expAdd = 0.267,
		describe = "后备兵力增加34200/时，战魂训练经验提升。",
	},
	[90] = {
		condt = {
			needMainLv = 91,
			needGold = 183980,
		},
		timelong = 9986,
		speed = 34500,
		expAdd = 0.27,
		describe = "后备兵力增加34500/时，战魂训练经验提升。",
	},
	[91] = {
		condt = {
			needMainLv = 92,
			needGold = 187500,
		},
		timelong = 10143,
		speed = 34800,
		expAdd = 0.273,
		describe = "后备兵力增加34800/时，战魂训练经验提升。",
	},
	[92] = {
		condt = {
			needMainLv = 93,
			needGold = 191060,
		},
		timelong = 10301,
		speed = 35100,
		expAdd = 0.276,
		describe = "后备兵力增加35100/时，战魂训练经验提升。",
	},
	[93] = {
		condt = {
			needMainLv = 94,
			needGold = 194640,
		},
		timelong = 10460,
		speed = 35400,
		expAdd = 0.279,
		describe = "后备兵力增加35400/时，战魂训练经验提升。",
	},
	[94] = {
		condt = {
			needMainLv = 95,
			needGold = 198250,
		},
		timelong = 10753,
		speed = 35700,
		expAdd = 0.282,
		describe = "后备兵力增加35700/时，战魂训练经验提升。",
	},
	[95] = {
		condt = {
			needMainLv = 96,
			needGold = 201890,
		},
		timelong = 11187,
		speed = 36000,
		expAdd = 0.285,
		describe = "后备兵力增加36000/时，战魂训练经验提升。",
	},
	[96] = {
		condt = {
			needMainLv = 97,
			needGold = 205560,
		},
		timelong = 11354,
		speed = 36300,
		expAdd = 0.288,
		describe = "后备兵力增加36300/时，战魂训练经验提升。",
	},
	[97] = {
		condt = {
			needMainLv = 98,
			needGold = 209250,
		},
		timelong = 11522,
		speed = 36600,
		expAdd = 0.291,
		describe = "后备兵力增加36600/时，战魂训练经验提升。",
	},
	[98] = {
		condt = {
			needMainLv = 99,
			needGold = 212970,
		},
		timelong = 11690,
		speed = 36900,
		expAdd = 0.294,
		describe = "后备兵力增加36900/时，战魂训练经验提升。",
	},
	[99] = {
		condt = {
			needMainLv = 100,
			needGold = 216720,
		},
		timelong = 12883,
		speed = 37200,
		expAdd = 0.297,
		describe = "后备兵力增加37200/时，战魂训练经验提升。",
	},
	[100] = {
		condt = {
		},
		speed = 37500,
		expAdd = 0.3,
		describe = "后备兵力增加37500/时，战魂训练经验提升。",
	},
}
function GetTable()
	return autoTable
end
