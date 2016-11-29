--民宅升级表
 local autoTable = {
	[0] = {
		condt = {
			needMainLv = 1,
			needGold = 90,
		},
		timelong = 7,
		speed = 0,
		upLimit = 0,
		describe = "人口增长0/小时,人口上限0",
	},
	[1] = {
		condt = {
			needMainLv = 2,
			needGold = 140,
		},
		timelong = 11,
		speed = 10400,
		upLimit = 374400,
		describe = "人口增长10400/小时,人口上限374400",
	},
	[2] = {
		condt = {
			needMainLv = 3,
			needGold = 250,
		},
		timelong = 15,
		speed = 10800,
		upLimit = 388800,
		describe = "人口增长10800/小时,人口上限388800",
	},
	[3] = {
		condt = {
			needMainLv = 4,
			needGold = 410,
		},
		timelong = 20,
		speed = 11200,
		upLimit = 403200,
		describe = "人口增长11200/小时,人口上限403200",
	},
	[4] = {
		condt = {
			needMainLv = 5,
			needGold = 610,
		},
		timelong = 29,
		speed = 11600,
		upLimit = 417600,
		describe = "人口增长11600/小时,人口上限417600",
	},
	[5] = {
		condt = {
			needMainLv = 6,
			needGold = 850,
		},
		timelong = 35,
		speed = 12000,
		upLimit = 432000,
		describe = "人口增长12000/小时,人口上限432000",
	},
	[6] = {
		condt = {
			needMainLv = 7,
			needGold = 1120,
		},
		timelong = 43,
		speed = 12400,
		upLimit = 446400,
		describe = "人口增长12400/小时,人口上限446400",
	},
	[7] = {
		condt = {
			needMainLv = 8,
			needGold = 1430,
		},
		timelong = 51,
		speed = 12800,
		upLimit = 460800,
		describe = "人口增长12800/小时,人口上限460800",
	},
	[8] = {
		condt = {
			needMainLv = 9,
			needGold = 1770,
		},
		timelong = 60,
		speed = 13200,
		upLimit = 475200,
		describe = "人口增长13200/小时,人口上限475200",
	},
	[9] = {
		condt = {
			needMainLv = 10,
			needGold = 2140,
		},
		timelong = 75,
		speed = 13600,
		upLimit = 489600,
		describe = "人口增长13600/小时,人口上限489600",
	},
	[10] = {
		condt = {
			needMainLv = 11,
			needGold = 2540,
		},
		timelong = 84,
		speed = 14000,
		upLimit = 504000,
		describe = "人口增长14000/小时,人口上限504000",
	},
	[11] = {
		condt = {
			needMainLv = 12,
			needGold = 2970,
		},
		timelong = 94,
		speed = 14400,
		upLimit = 518400,
		describe = "人口增长14400/小时,人口上限518400",
	},
	[12] = {
		condt = {
			needMainLv = 13,
			needGold = 3420,
		},
		timelong = 105,
		speed = 14800,
		upLimit = 532800,
		describe = "人口增长14800/小时,人口上限532800",
	},
	[13] = {
		condt = {
			needMainLv = 14,
			needGold = 3910,
		},
		timelong = 116,
		speed = 15200,
		upLimit = 547200,
		describe = "人口增长15200/小时,人口上限547200",
	},
	[14] = {
		condt = {
			needMainLv = 15,
			needGold = 4410,
		},
		timelong = 136,
		speed = 15600,
		upLimit = 561600,
		describe = "人口增长15600/小时,人口上限561600",
	},
	[15] = {
		condt = {
			needMainLv = 16,
			needGold = 4950,
		},
		timelong = 149,
		speed = 16000,
		upLimit = 576000,
		describe = "人口增长16000/小时,人口上限576000",
	},
	[16] = {
		condt = {
			needMainLv = 17,
			needGold = 5510,
		},
		timelong = 162,
		speed = 16400,
		upLimit = 590400,
		describe = "人口增长16400/小时,人口上限590400",
	},
	[17] = {
		condt = {
			needMainLv = 18,
			needGold = 6090,
		},
		timelong = 176,
		speed = 16800,
		upLimit = 604800,
		describe = "人口增长16800/小时,人口上限604800",
	},
	[18] = {
		condt = {
			needMainLv = 19,
			needGold = 6700,
		},
		timelong = 190,
		speed = 17200,
		upLimit = 619200,
		describe = "人口增长17200/小时,人口上限619200",
	},
	[19] = {
		condt = {
			needMainLv = 20,
			needGold = 7340,
		},
		timelong = 248,
		speed = 17600,
		upLimit = 633600,
		describe = "人口增长17600/小时,人口上限633600",
	},
	[20] = {
		condt = {
			needMainLv = 21,
			needGold = 7990,
		},
		timelong = 265,
		speed = 18000,
		upLimit = 648000,
		describe = "人口增长18000/小时,人口上限648000",
	},
	[21] = {
		condt = {
			needMainLv = 22,
			needGold = 8670,
		},
		timelong = 285,
		speed = 18400,
		upLimit = 662400,
		describe = "人口增长18400/小时,人口上限662400",
	},
	[22] = {
		condt = {
			needMainLv = 23,
			needGold = 9370,
		},
		timelong = 303,
		speed = 18800,
		upLimit = 676800,
		describe = "人口增长18800/小时,人口上限676800",
	},
	[23] = {
		condt = {
			needMainLv = 24,
			needGold = 10100,
		},
		timelong = 326,
		speed = 19200,
		upLimit = 691200,
		describe = "人口增长19200/小时,人口上限691200",
	},
	[24] = {
		condt = {
			needMainLv = 25,
			needGold = 10840,
		},
		timelong = 360,
		speed = 19600,
		upLimit = 705600,
		describe = "人口增长19600/小时,人口上限705600",
	},
	[25] = {
		condt = {
			needMainLv = 26,
			needGold = 11610,
		},
		timelong = 386,
		speed = 20000,
		upLimit = 720000,
		describe = "人口增长20000/小时,人口上限720000",
	},
	[26] = {
		condt = {
			needMainLv = 27,
			needGold = 12400,
		},
		timelong = 408,
		speed = 20400,
		upLimit = 734400,
		describe = "人口增长20400/小时,人口上限734400",
	},
	[27] = {
		condt = {
			needMainLv = 28,
			needGold = 13210,
		},
		timelong = 438,
		speed = 20800,
		upLimit = 748800,
		describe = "人口增长20800/小时,人口上限748800",
	},
	[28] = {
		condt = {
			needMainLv = 29,
			needGold = 14040,
		},
		timelong = 461,
		speed = 21200,
		upLimit = 763200,
		describe = "人口增长21200/小时,人口上限763200",
	},
	[29] = {
		condt = {
			needMainLv = 30,
			needGold = 14900,
		},
		timelong = 513,
		speed = 21600,
		upLimit = 777600,
		describe = "人口增长21600/小时,人口上限777600",
	},
	[30] = {
		condt = {
			needMainLv = 31,
			needGold = 15770,
		},
		timelong = 538,
		speed = 22000,
		upLimit = 792000,
		describe = "人口增长22000/小时,人口上限792000",
	},
	[31] = {
		condt = {
			needMainLv = 32,
			needGold = 16660,
		},
		timelong = 577,
		speed = 22400,
		upLimit = 806400,
		describe = "人口增长22400/小时,人口上限806400",
	},
	[32] = {
		condt = {
			needMainLv = 33,
			needGold = 17580,
		},
		timelong = 603,
		speed = 22800,
		upLimit = 820800,
		describe = "人口增长22800/小时,人口上限820800",
	},
	[33] = {
		condt = {
			needMainLv = 34,
			needGold = 18510,
		},
		timelong = 648,
		speed = 23200,
		upLimit = 835200,
		describe = "人口增长23200/小时,人口上限835200",
	},
	[34] = {
		condt = {
			needMainLv = 35,
			needGold = 19460,
		},
		timelong = 696,
		speed = 23600,
		upLimit = 849600,
		describe = "人口增长23600/小时,人口上限849600",
	},
	[35] = {
		condt = {
			needMainLv = 36,
			needGold = 20430,
		},
		timelong = 746,
		speed = 24000,
		upLimit = 864000,
		describe = "人口增长24000/小时,人口上限864000",
	},
	[36] = {
		condt = {
			needMainLv = 37,
			needGold = 21430,
		},
		timelong = 776,
		speed = 24400,
		upLimit = 878400,
		describe = "人口增长24400/小时,人口上限878400",
	},
	[37] = {
		condt = {
			needMainLv = 38,
			needGold = 22440,
		},
		timelong = 807,
		speed = 24800,
		upLimit = 892800,
		describe = "人口增长24800/小时,人口上限892800",
	},
	[38] = {
		condt = {
			needMainLv = 39,
			needGold = 23470,
		},
		timelong = 838,
		speed = 25200,
		upLimit = 907200,
		describe = "人口增长25200/小时,人口上限907200",
	},
	[39] = {
		condt = {
			needMainLv = 40,
			needGold = 24520,
		},
		timelong = 976,
		speed = 25600,
		upLimit = 921600,
		describe = "人口增长25600/小时,人口上限921600",
	},
	[40] = {
		condt = {
			needMainLv = 41,
			needGold = 25580,
		},
		timelong = 1011,
		speed = 26000,
		upLimit = 936000,
		describe = "人口增长26000/小时,人口上限936000",
	},
	[41] = {
		condt = {
			needMainLv = 42,
			needGold = 26670,
		},
		timelong = 1047,
		speed = 26400,
		upLimit = 950400,
		describe = "人口增长26400/小时,人口上限950400",
	},
	[42] = {
		condt = {
			needMainLv = 43,
			needGold = 27780,
		},
		timelong = 1083,
		speed = 26800,
		upLimit = 964800,
		describe = "人口增长26800/小时,人口上限964800",
	},
	[43] = {
		condt = {
			needMainLv = 44,
			needGold = 28900,
		},
		timelong = 1119,
		speed = 27200,
		upLimit = 979200,
		describe = "人口增长27200/小时,人口上限979200",
	},
	[44] = {
		condt = {
			needMainLv = 45,
			needGold = 30040,
		},
		timelong = 1197,
		speed = 27600,
		upLimit = 993600,
		describe = "人口增长27600/小时,人口上限993600",
	},
	[45] = {
		condt = {
			needMainLv = 46,
			needGold = 31200,
		},
		timelong = 1236,
		speed = 28000,
		upLimit = 1008000,
		describe = "人口增长28000/小时,人口上限1008000",
	},
	[46] = {
		condt = {
			needMainLv = 47,
			needGold = 32380,
		},
		timelong = 1275,
		speed = 28400,
		upLimit = 1022400,
		describe = "人口增长28400/小时,人口上限1022400",
	},
	[47] = {
		condt = {
			needMainLv = 48,
			needGold = 33570,
		},
		timelong = 1352,
		speed = 28800,
		upLimit = 1036800,
		describe = "人口增长28800/小时,人口上限1036800",
	},
	[48] = {
		condt = {
			needMainLv = 49,
			needGold = 34790,
		},
		timelong = 1393,
		speed = 29200,
		upLimit = 1051200,
		describe = "人口增长29200/小时,人口上限1051200",
	},
	[49] = {
		condt = {
			needMainLv = 50,
			needGold = 36020,
		},
		timelong = 1466,
		speed = 29600,
		upLimit = 1065600,
		describe = "人口增长29600/小时,人口上限1065600",
	},
	[50] = {
		condt = {
			needMainLv = 51,
			needGold = 37260,
		},
		timelong = 1534,
		speed = 30000,
		upLimit = 1080000,
		describe = "人口增长30000/小时,人口上限1080000",
	},
	[51] = {
		condt = {
			needMainLv = 52,
			needGold = 38530,
		},
		timelong = 1589,
		speed = 30400,
		upLimit = 1094400,
		describe = "人口增长30400/小时,人口上限1094400",
	},
	[52] = {
		condt = {
			needMainLv = 53,
			needGold = 39810,
		},
		timelong = 1644,
		speed = 30800,
		upLimit = 1108800,
		describe = "人口增长30800/小时,人口上限1108800",
	},
	[53] = {
		condt = {
			needMainLv = 54,
			needGold = 41110,
		},
		timelong = 1701,
		speed = 31200,
		upLimit = 1123200,
		describe = "人口增长31200/小时,人口上限1123200",
	},
	[54] = {
		condt = {
			needMainLv = 55,
			needGold = 42430,
		},
		timelong = 1793,
		speed = 31600,
		upLimit = 1137600,
		describe = "人口增长31600/小时,人口上限1137600",
	},
	[55] = {
		condt = {
			needMainLv = 56,
			needGold = 43760,
		},
		timelong = 1902,
		speed = 32000,
		upLimit = 1152000,
		describe = "人口增长32000/小时,人口上限1152000",
	},
	[56] = {
		condt = {
			needMainLv = 57,
			needGold = 45110,
		},
		timelong = 1963,
		speed = 32400,
		upLimit = 1166400,
		describe = "人口增长32400/小时,人口上限1166400",
	},
	[57] = {
		condt = {
			needMainLv = 58,
			needGold = 46480,
		},
		timelong = 2025,
		speed = 32800,
		upLimit = 1180800,
		describe = "人口增长32800/小时,人口上限1180800",
	},
	[58] = {
		condt = {
			needMainLv = 59,
			needGold = 47860,
		},
		timelong = 2088,
		speed = 33200,
		upLimit = 1195200,
		describe = "人口增长33200/小时,人口上限1195200",
	},
	[59] = {
		condt = {
			needMainLv = 60,
			needGold = 49260,
		},
		timelong = 2358,
		speed = 33600,
		upLimit = 1209600,
		describe = "人口增长33600/小时,人口上限1209600",
	},
	[60] = {
		condt = {
			needMainLv = 61,
			needGold = 50680,
		},
		timelong = 2427,
		speed = 34000,
		upLimit = 1224000,
		describe = "人口增长34000/小时,人口上限1224000",
	},
	[61] = {
		condt = {
			needMainLv = 62,
			needGold = 52110,
		},
		timelong = 2498,
		speed = 34400,
		upLimit = 1238400,
		describe = "人口增长34400/小时,人口上限1238400",
	},
	[62] = {
		condt = {
			needMainLv = 63,
			needGold = 53560,
		},
		timelong = 2601,
		speed = 34800,
		upLimit = 1252800,
		describe = "人口增长34800/小时,人口上限1252800",
	},
	[63] = {
		condt = {
			needMainLv = 64,
			needGold = 55030,
		},
		timelong = 2641,
		speed = 35200,
		upLimit = 1267200,
		describe = "人口增长35200/小时,人口上限1267200",
	},
	[64] = {
		condt = {
			needMainLv = 65,
			needGold = 56510,
		},
		timelong = 2759,
		speed = 35600,
		upLimit = 1281600,
		describe = "人口增长35600/小时,人口上限1281600",
	},
	[65] = {
		condt = {
			needMainLv = 66,
			needGold = 58010,
		},
		timelong = 2834,
		speed = 36000,
		upLimit = 1296000,
		describe = "人口增长36000/小时,人口上限1296000",
	},
	[66] = {
		condt = {
			needMainLv = 67,
			needGold = 59520,
		},
		timelong = 2910,
		speed = 36400,
		upLimit = 1310400,
		describe = "人口增长36400/小时,人口上限1310400",
	},
	[67] = {
		condt = {
			needMainLv = 68,
			needGold = 61050,
		},
		timelong = 3060,
		speed = 36800,
		upLimit = 1324800,
		describe = "人口增长36800/小时,人口上限1324800",
	},
	[68] = {
		condt = {
			needMainLv = 69,
			needGold = 62590,
		},
		timelong = 3139,
		speed = 37200,
		upLimit = 1339200,
		describe = "人口增长37200/小时,人口上限1339200",
	},
	[69] = {
		condt = {
			needMainLv = 70,
			needGold = 64160,
		},
		timelong = 3331,
		speed = 37600,
		upLimit = 1353600,
		describe = "人口增长37600/小时,人口上限1353600",
	},
	[70] = {
		condt = {
			needMainLv = 71,
			needGold = 65730,
		},
		timelong = 3398,
		speed = 38000,
		upLimit = 1368000,
		describe = "人口增长38000/小时,人口上限1368000",
	},
	[71] = {
		condt = {
			needMainLv = 72,
			needGold = 67330,
		},
		timelong = 3466,
		speed = 38400,
		upLimit = 1382400,
		describe = "人口增长38400/小时,人口上限1382400",
	},
	[72] = {
		condt = {
			needMainLv = 73,
			needGold = 68930,
		},
		timelong = 3535,
		speed = 38800,
		upLimit = 1396800,
		describe = "人口增长38800/小时,人口上限1396800",
	},
	[73] = {
		condt = {
			needMainLv = 74,
			needGold = 70560,
		},
		timelong = 3604,
		speed = 39200,
		upLimit = 1411200,
		describe = "人口增长39200/小时,人口上限1411200",
	},
	[74] = {
		condt = {
			needMainLv = 75,
			needGold = 72200,
		},
		timelong = 3728,
		speed = 39600,
		upLimit = 1425600,
		describe = "人口增长39600/小时,人口上限1425600",
	},
	[75] = {
		condt = {
			needMainLv = 76,
			needGold = 73850,
		},
		timelong = 3895,
		speed = 40000,
		upLimit = 1440000,
		describe = "人口增长40000/小时,人口上限1440000",
	},
	[76] = {
		condt = {
			needMainLv = 77,
			needGold = 75520,
		},
		timelong = 3967,
		speed = 40400,
		upLimit = 1454400,
		describe = "人口增长40400/小时,人口上限1454400",
	},
	[77] = {
		condt = {
			needMainLv = 78,
			needGold = 77210,
		},
		timelong = 4041,
		speed = 40800,
		upLimit = 1468800,
		describe = "人口增长40800/小时,人口上限1468800",
	},
	[78] = {
		condt = {
			needMainLv = 79,
			needGold = 78910,
		},
		timelong = 4115,
		speed = 41200,
		upLimit = 1483200,
		describe = "人口增长41200/小时,人口上限1483200",
	},
	[79] = {
		condt = {
			needMainLv = 80,
			needGold = 80620,
		},
		timelong = 4523,
		speed = 41600,
		upLimit = 1497600,
		describe = "人口增长41600/小时,人口上限1497600",
	},
	[80] = {
		condt = {
			needMainLv = 81,
			needGold = 82350,
		},
		timelong = 4603,
		speed = 42000,
		upLimit = 1512000,
		describe = "人口增长42000/小时,人口上限1512000",
	},
	[81] = {
		condt = {
			needMainLv = 82,
			needGold = 84100,
		},
		timelong = 4684,
		speed = 42400,
		upLimit = 1526400,
		describe = "人口增长42400/小时,人口上限1526400",
	},
	[82] = {
		condt = {
			needMainLv = 83,
			needGold = 85860,
		},
		timelong = 4765,
		speed = 42800,
		upLimit = 1540800,
		describe = "人口增长42800/小时,人口上限1540800",
	},
	[83] = {
		condt = {
			needMainLv = 84,
			needGold = 87630,
		},
		timelong = 4847,
		speed = 43200,
		upLimit = 1555200,
		describe = "人口增长43200/小时,人口上限1555200",
	},
	[84] = {
		condt = {
			needMainLv = 85,
			needGold = 89420,
		},
		timelong = 4995,
		speed = 43600,
		upLimit = 1569600,
		describe = "人口增长43600/小时,人口上限1569600",
	},
	[85] = {
		condt = {
			needMainLv = 86,
			needGold = 91230,
		},
		timelong = 5079,
		speed = 44000,
		upLimit = 1584000,
		describe = "人口增长44000/小时,人口上限1584000",
	},
	[86] = {
		condt = {
			needMainLv = 87,
			needGold = 93040,
		},
		timelong = 5163,
		speed = 44400,
		upLimit = 1598400,
		describe = "人口增长44400/小时,人口上限1598400",
	},
	[87] = {
		condt = {
			needMainLv = 88,
			needGold = 94880,
		},
		timelong = 5371,
		speed = 44800,
		upLimit = 1612800,
		describe = "人口增长44800/小时,人口上限1612800",
	},
	[88] = {
		condt = {
			needMainLv = 89,
			needGold = 96730,
		},
		timelong = 5458,
		speed = 45200,
		upLimit = 1627200,
		describe = "人口增长45200/小时,人口上限1627200",
	},
	[89] = {
		condt = {
			needMainLv = 90,
			needGold = 98590,
		},
		timelong = 5617,
		speed = 45600,
		upLimit = 1641600,
		describe = "人口增长45600/小时,人口上限1641600",
	},
	[90] = {
		condt = {
			needMainLv = 91,
			needGold = 100470,
		},
		timelong = 5706,
		speed = 46000,
		upLimit = 1656000,
		describe = "人口增长46000/小时,人口上限1656000",
	},
	[91] = {
		condt = {
			needMainLv = 92,
			needGold = 102360,
		},
		timelong = 5796,
		speed = 46400,
		upLimit = 1670400,
		describe = "人口增长46400/小时,人口上限1670400",
	},
	[92] = {
		condt = {
			needMainLv = 93,
			needGold = 104270,
		},
		timelong = 5886,
		speed = 46800,
		upLimit = 1684800,
		describe = "人口增长46800/小时,人口上限1684800",
	},
	[93] = {
		condt = {
			needMainLv = 94,
			needGold = 106190,
		},
		timelong = 5977,
		speed = 47200,
		upLimit = 1699200,
		describe = "人口增长47200/小时,人口上限1699200",
	},
	[94] = {
		condt = {
			needMainLv = 95,
			needGold = 108120,
		},
		timelong = 6144,
		speed = 47600,
		upLimit = 1713600,
		describe = "人口增长47600/小时,人口上限1713600",
	},
	[95] = {
		condt = {
			needMainLv = 96,
			needGold = 110070,
		},
		timelong = 6393,
		speed = 48000,
		upLimit = 1728000,
		describe = "人口增长48000/小时,人口上限1728000",
	},
	[96] = {
		condt = {
			needMainLv = 97,
			needGold = 112040,
		},
		timelong = 6488,
		speed = 48400,
		upLimit = 1742400,
		describe = "人口增长48400/小时,人口上限1742400",
	},
	[97] = {
		condt = {
			needMainLv = 98,
			needGold = 114010,
		},
		timelong = 6584,
		speed = 48800,
		upLimit = 1756800,
		describe = "人口增长48800/小时,人口上限1756800",
	},
	[98] = {
		condt = {
			needMainLv = 99,
			needGold = 116010,
		},
		timelong = 6680,
		speed = 49200,
		upLimit = 1771200,
		describe = "人口增长49200/小时,人口上限1771200",
	},
	[99] = {
		condt = {
			needMainLv = 100,
			needGold = 118010,
		},
		timelong = 7362,
		speed = 49600,
		upLimit = 1785600,
		describe = "人口增长49600/小时,人口上限1785600",
	},
	[100] = {
		condt = {
		},
		speed = 50000,
		upLimit = 1800000,
		describe = "人口增长50000/小时,人口上限1800000",
	},
}
function GetTable()
	return autoTable
end
