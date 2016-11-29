--技能列表
local autoTable = {
	["301001"] = {
		name = "小试牛刀",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		method = 3,
		describe = "以普通招式攻击敌人",
		class = "atk",
		atkType=1,
		atkAction=350001,
		needMove = 1,
		quality = 1,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成61%+40物理伤害",
				num = 40,
				rate = 0.61,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成62%+80物理伤害",
				num = 80,
				rate = 0.62,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成63%+120物理伤害",
				num = 120,
				rate = 0.63,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成64%+160物理伤害",
				num = 160,
				rate = 0.64,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成65%+200物理伤害",
				num = 200,
				rate = 0.65,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成66%+240物理伤害",
				num = 240,
				rate = 0.66,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成67%+280物理伤害",
				num = 280,
				rate = 0.67,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成68%+320物理伤害",
				num = 320,
				rate = 0.68,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成69%+360物理伤害",
				num = 360,
				rate = 0.69,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成70%+400物理伤害",
				num = 400,
				rate = 0.7,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成71%+440物理伤害",
				num = 440,
				rate = 0.71,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成72%+480物理伤害",
				num = 480,
				rate = 0.72,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成73%+520物理伤害",
				num = 520,
				rate = 0.73,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成74%+560物理伤害",
				num = 560,
				rate = 0.74,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成75%+600物理伤害",
				num = 600,
				rate = 0.75,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成76%+640物理伤害",
				num = 640,
				rate = 0.76,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成77%+680物理伤害",
				num = 680,
				rate = 0.77,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成78%+720物理伤害",
				num = 720,
				rate = 0.78,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成79%+760物理伤害",
				num = 760,
				rate = 0.79,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成80%+800物理伤害",
				num = 800,
				rate = 0.8,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成81%+840物理伤害",
				num = 840,
				rate = 0.81,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成82%+880物理伤害",
				num = 880,
				rate = 0.82,
				buffer = {
				},
			},
		},
	},
	["301002"] = {
		name = "锋芒毕露",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		method = 3,
		describe = "提升自身攻击力，给敌人予重击",
		class = "atk",
		atkType=3,
		atkAction=350001,
		needMove = 1,
		quality = 2,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成76%+50物理伤害",
				num = 50,
				rate = 0.76,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成77%+100物理伤害",
				num = 100,
				rate = 0.77,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成78%+150物理伤害",
				num = 150,
				rate = 0.78,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成79%+200物理伤害",
				num = 200,
				rate = 0.79,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成80%+250物理伤害",
				num = 250,
				rate = 0.8,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成81%+300物理伤害",
				num = 300,
				rate = 0.81,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成82%+350物理伤害",
				num = 350,
				rate = 0.82,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成83%+400物理伤害",
				num = 400,
				rate = 0.83,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成84%+450物理伤害",
				num = 450,
				rate = 0.84,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成85%+500物理伤害",
				num = 500,
				rate = 0.85,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成86%+550物理伤害",
				num = 550,
				rate = 0.86,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成87%+600物理伤害",
				num = 600,
				rate = 0.87,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成88%+650物理伤害",
				num = 650,
				rate = 0.88,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成89%+700物理伤害",
				num = 700,
				rate = 0.89,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成90%+750物理伤害",
				num = 750,
				rate = 0.9,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成91%+800物理伤害",
				num = 800,
				rate = 0.91,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成92%+850物理伤害",
				num = 850,
				rate = 0.92,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成93%+900物理伤害",
				num = 900,
				rate = 0.93,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成94%+950物理伤害",
				num = 950,
				rate = 0.94,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成95%+1000物理伤害",
				num = 1000,
				rate = 0.95,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成96%+1050物理伤害",
				num = 1050,
				rate = 0.96,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成97%+1100物理伤害",
				num = 1100,
				rate = 0.97,
				buffer = {
				},
			},
		},
	},
	["301003"] = {
		name = "力劈华山",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		method = 3,
		describe = "以开山之力劈砍敌人",
		class = "atk",
		atkType=2,
		atkAction=350001,
		needMove = 1,
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成91%+60物理伤害",
				num = 60,
				rate = 0.91,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成92%+120物理伤害",
				num = 120,
				rate = 0.92,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成93%+180物理伤害",
				num = 180,
				rate = 0.93,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成94%+240物理伤害",
				num = 240,
				rate = 0.94,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成95%+300物理伤害",
				num = 300,
				rate = 0.95,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成96%+360物理伤害",
				num = 360,
				rate = 0.96,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成97%+420物理伤害",
				num = 420,
				rate = 0.97,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成98%+480物理伤害",
				num = 480,
				rate = 0.98,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成99%+540物理伤害",
				num = 540,
				rate = 0.99,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成100%+600物理伤害",
				num = 600,
				rate = 1,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成101%+660物理伤害",
				num = 660,
				rate = 1.01,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成102%+720物理伤害",
				num = 720,
				rate = 1.02,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "对目标造成103%+780物理伤害",
				num = 780,
				rate = 1.03,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "对目标造成104%+840物理伤害",
				num = 840,
				rate = 1.04,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "对目标造成105%+900物理伤害",
				num = 900,
				rate = 1.05,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "对目标造成106%+960物理伤害",
				num = 960,
				rate = 1.06,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "对目标造成107%+1020物理伤害",
				num = 1020,
				rate = 1.07,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "对目标造成108%+1080物理伤害",
				num = 1080,
				rate = 1.08,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "对目标造成109%+1140物理伤害",
				num = 1140,
				rate = 1.09,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "对目标造成110%+1200物理伤害",
				num = 1200,
				rate = 1.1,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "对目标造成111%+1260物理伤害",
				num = 1260,
				rate = 1.11,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "对目标造成112%+1320物理伤害",
				num = 1320,
				rate = 1.12,
				buffer = {
				},
			},
		},
	},
	["301004"] = {
		name = "长虹贯日",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		method = 3,
		describe = "急速刺向敌人，忽视敌人部分物理防御",
		class = "atk",
		atkType=2,
		atkAction=350002,
		needMove = 1,
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "忽视目标1.5%的物理防御，造成106%+70物理伤害",
				num = 70,
				rate = 1.06,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.015},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "忽视目标3.0%的物理防御，造成107%+140物理伤害",
				num = 140,
				rate = 1.07,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.03},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "忽视目标4.5%的物理防御，造成108%+210物理伤害",
				num = 210,
				rate = 1.08,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.045},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "忽视目标6.0%的物理防御，造成109%+280物理伤害",
				num = 280,
				rate = 1.09,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.06},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "忽视目标7.5%的物理防御，造成110%+350物理伤害",
				num = 350,
				rate = 1.1,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.075},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "忽视目标9.0%的物理防御，造成111%+420物理伤害",
				num = 420,
				rate = 1.11,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.09},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "忽视目标10.5%的物理防御，造成112%+490物理伤害",
				num = 490,
				rate = 1.12,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.105},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "忽视目标12.0%的物理防御，造成113%+560物理伤害",
				num = 560,
				rate = 1.13,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.12},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "忽视目标13.5%的物理防御，造成114%+630物理伤害",
				num = 630,
				rate = 1.14,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.135},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "忽视目标15.0%的物理防御，造成115%+700物理伤害",
				num = 700,
				rate = 1.15,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.15},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "忽视目标16.5%的物理防御，造成116%+770物理伤害",
				num = 770,
				rate = 1.16,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.165},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "忽视目标18.0%的物理防御，造成117%+840物理伤害",
				num = 840,
				rate = 1.17,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.18},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "忽视目标19.5%的物理防御，造成118%+910物理伤害",
				num = 910,
				rate = 1.18,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.195},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "忽视目标21.0%的物理防御，造成119%+980物理伤害",
				num = 980,
				rate = 1.19,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.21},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "忽视目标22.5%的物理防御，造成120%+1050物理伤害",
				num = 1050,
				rate = 1.2,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.225},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "忽视目标24.0%的物理防御，造成121%+1120物理伤害",
				num = 1120,
				rate = 1.21,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.24},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "忽视目标25.5%的物理防御，造成122%+1190物理伤害",
				num = 1190,
				rate = 1.22,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.255},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "忽视目标27.0%的物理防御，造成123%+1260物理伤害",
				num = 1260,
				rate = 1.23,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.27},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "忽视目标28.5%的物理防御，造成124%+1330物理伤害",
				num = 1330,
				rate = 1.24,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.285},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "忽视目标30.0%的物理防御，造成125%+1400物理伤害",
				num = 1400,
				rate = 1.25,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.3},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "忽视目标31.5%的物理防御，造成126%+1470物理伤害",
				num = 1470,
				rate = 1.26,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.315},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "忽视目标33.0%的物理防御，造成127%+1540物理伤害",
				num = 1540,
				rate = 1.27,
				buffer = {
					{id = "20", last = 0, occur = 1, rate = 0.33},
				},
			},
		},
	},
	["301005"] = {
		name = "斩天式",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 6,	--目标范围
		validJob = {warrior = 1,},
		method = 3,
		describe = "挥出弯月形的剑气攻击同一直线的敌人",
		class = "atk",
		atkType=2,
		atkAction=350003,
		needMove = 1,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对直线上所有目标造成150%+40物理伤害",
				num = 40,
				rate = 1.5,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对直线上所有目标造成66%+80物理伤害",
				num = 80,
				rate = 0.655,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对直线上所有目标造成66%+120物理伤害",
				num = 120,
				rate = 0.66,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对直线上所有目标造成67%+160物理伤害",
				num = 160,
				rate = 0.665,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对直线上所有目标造成67%+200物理伤害",
				num = 200,
				rate = 0.67,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对直线上所有目标造成68%+240物理伤害",
				num = 240,
				rate = 0.675,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对直线上所有目标造成68%+280物理伤害",
				num = 280,
				rate = 0.68,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对直线上所有目标造成69%+320物理伤害",
				num = 320,
				rate = 0.685,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对直线上所有目标造成69%+360物理伤害",
				num = 360,
				rate = 0.69,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对直线上所有目标造成70%+400物理伤害",
				num = 400,
				rate = 0.695,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对直线上所有目标造成70%+440物理伤害",
				num = 440,
				rate = 0.7,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对直线上所有目标造成71%+480物理伤害",
				num = 480,
				rate = 0.705,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对直线上所有目标造成71%+520物理伤害",
				num = 520,
				rate = 0.71,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对直线上所有目标造成72%+560物理伤害",
				num = 560,
				rate = 0.715,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对直线上所有目标造成72%+600物理伤害",
				num = 600,
				rate = 0.72,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对直线上所有目标造成73%+640物理伤害",
				num = 640,
				rate = 0.725,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对直线上所有目标造成73%+680物理伤害",
				num = 680,
				rate = 0.73,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对直线上所有目标造成74%+720物理伤害",
				num = 720,
				rate = 0.735,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对直线上所有目标造成74%+760物理伤害",
				num = 760,
				rate = 0.74,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对直线上所有目标造成75%+800物理伤害",
				num = 800,
				rate = 0.745,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对直线上所有目标造成75%+840物理伤害",
				num = 840,
				rate = 0.75,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对直线上所有目标造成76%+880物理伤害",
				num = 880,
				rate = 0.755,
				buffer = {
				},
			},
		},
	},
	["301006"] = {
		name = "雷霆冲撞",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior2 = 1,},
		method = 3,
		describe = "高速冲撞敌人造成大量伤害，并有几率造成眩晕效果",
		class = "atk_buff",
		atkType=2,
		atkAction=350001,
		needMove = 1,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成76%+80物理伤害，20%几率眩晕目标",
				num = 80,
				rate = 0.76,
				buffer = {
					{id = "26", last = 1, occur = 0.2},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成122%+160物理伤害，22%几率眩晕目标",
				num = 160,
				rate = 1.22,
				buffer = {
					{id = "26", last = 1, occur = 0.22},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成123%+240物理伤害，24%几率眩晕目标",
				num = 240,
				rate = 1.23,
				buffer = {
					{id = "26", last = 1, occur = 0.24},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成124%+320物理伤害，26%几率眩晕目标",
				num = 320,
				rate = 1.24,
				buffer = {
					{id = "26", last = 1, occur = 0.26},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成125%+400物理伤害，28%几率眩晕目标",
				num = 400,
				rate = 1.25,
				buffer = {
					{id = "26", last = 1, occur = 0.28},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成126%+480物理伤害，30%几率眩晕目标",
				num = 480,
				rate = 1.26,
				buffer = {
					{id = "26", last = 1, occur = 0.3},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成127%+560物理伤害，32%几率眩晕目标",
				num = 560,
				rate = 1.27,
				buffer = {
					{id = "26", last = 1, occur = 0.32},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成128%+640物理伤害，34%几率眩晕目标",
				num = 640,
				rate = 1.28,
				buffer = {
					{id = "26", last = 1, occur = 0.34},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成129%+720物理伤害，36%几率眩晕目标",
				num = 720,
				rate = 1.29,
				buffer = {
					{id = "26", last = 1, occur = 0.36},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成130%+800物理伤害，38%几率眩晕目标",
				num = 800,
				rate = 1.3,
				buffer = {
					{id = "26", last = 1, occur = 0.38},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成131%+880物理伤害，40%几率眩晕目标",
				num = 880,
				rate = 1.31,
				buffer = {
					{id = "26", last = 1, occur = 0.4},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成132%+960物理伤害，42%几率眩晕目标",
				num = 960,
				rate = 1.32,
				buffer = {
					{id = "26", last = 1, occur = 0.42},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成133%+1040物理伤害，44%几率眩晕目标",
				num = 1040,
				rate = 1.33,
				buffer = {
					{id = "26", last = 1, occur = 0.44},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成134%+1120物理伤害，46%几率眩晕目标",
				num = 1120,
				rate = 1.34,
				buffer = {
					{id = "26", last = 1, occur = 0.46},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成135%+1200物理伤害，48%几率眩晕目标",
				num = 1200,
				rate = 1.35,
				buffer = {
					{id = "26", last = 1, occur = 0.48},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成136%+1280物理伤害，50%几率眩晕目标",
				num = 1280,
				rate = 1.36,
				buffer = {
					{id = "26", last = 1, occur = 0.5},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成137%+1360物理伤害，52%几率眩晕目标",
				num = 1360,
				rate = 1.37,
				buffer = {
					{id = "26", last = 1, occur = 0.52},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成138%+1440物理伤害，54%几率眩晕目标",
				num = 1440,
				rate = 1.38,
				buffer = {
					{id = "26", last = 1, occur = 0.54},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成139%+1520物理伤害，56%几率眩晕目标",
				num = 1520,
				rate = 1.39,
				buffer = {
					{id = "26", last = 1, occur = 0.56},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成140%+1600物理伤害，58%几率眩晕目标",
				num = 1600,
				rate = 1.4,
				buffer = {
					{id = "26", last = 1, occur = 0.58},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成141%+1680物理伤害，60%几率眩晕目标",
				num = 1680,
				rate = 1.41,
				buffer = {
					{id = "26", last = 1, occur = 0.6},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成142%+1760物理伤害，62%几率眩晕目标",
				num = 1760,
				rate = 1.42,
				buffer = {
					{id = "26", last = 1, occur = 0.62},
				},
			},
		},
	},
	["301007"] = {
		name = "盾击",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior3 = 1,},
		method = 3,
		describe = "给予敌人重击，并降低敌人行动速度",
		class = "atk_buff",
		atkType=2,
		atkAction=350004,
		needMove = 1,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成121%+80物理伤害，降低其50行动速度",
				num = 80,
				rate = 1.21,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成122%+160物理伤害，降低其55行动速度",
				num = 160,
				rate = 1.22,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 55},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成123%+240物理伤害，降低其60行动速度",
				num = 240,
				rate = 1.23,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 60},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成124%+320物理伤害，降低其65行动速度",
				num = 320,
				rate = 1.24,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 65},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成125%+400物理伤害，降低其70行动速度",
				num = 400,
				rate = 1.25,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 70},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成126%+480物理伤害，降低其75行动速度",
				num = 480,
				rate = 1.26,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 75},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成127%+560物理伤害，降低其80行动速度",
				num = 560,
				rate = 1.27,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 80},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成128%+640物理伤害，降低其85行动速度",
				num = 640,
				rate = 1.28,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 85},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成129%+720物理伤害，降低其90行动速度",
				num = 720,
				rate = 1.29,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 90},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成130%+800物理伤害，降低其95行动速度",
				num = 800,
				rate = 1.3,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 95},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成131%+880物理伤害，降低其100行动速度",
				num = 880,
				rate = 1.31,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 100},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成132%+960物理伤害，降低其105行动速度",
				num = 960,
				rate = 1.32,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 105},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成133%+1040物理伤害，降低其110行动速度",
				num = 1040,
				rate = 1.33,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 110},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成134%+1120物理伤害，降低其115行动速度",
				num = 1120,
				rate = 1.34,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 115},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成135%+1200物理伤害，降低其120行动速度",
				num = 1200,
				rate = 1.35,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 120},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成136%+1280物理伤害，降低其125行动速度",
				num = 1280,
				rate = 1.36,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 125},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成137%+1360物理伤害，降低其130行动速度",
				num = 1360,
				rate = 1.37,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 130},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成138%+1440物理伤害，降低其135行动速度",
				num = 1440,
				rate = 1.38,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 135},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成139%+1520物理伤害，降低其140行动速度",
				num = 1520,
				rate = 1.39,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 140},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成140%+1600物理伤害，降低其145行动速度",
				num = 1600,
				rate = 1.4,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 145},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成141%+1680物理伤害，降低其150行动速度",
				num = 1680,
				rate = 1.41,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 150},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "对目标造成142%+1760物理伤害，降低其155行动速度",
				num = 1760,
				rate = 1.42,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 155},
				},
			},
		},
	},
	["301008"] = {
		name = "飓风之刃",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 5,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		method = 3,
		describe = "施放数道风刃攻击最前方的一排敌人",
		class = "atk",
		atkType=2,
		atkAction=350012,
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对直线上目标造成45.5%+44法术伤害",
				num = 44,
				rate = 0.455,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对直线上目标造成46.0%+88法术伤害",
				num = 88,
				rate = 0.46,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对直线上目标造成46.5%+132法术伤害",
				num = 132,
				rate = 0.465,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对直线上目标造成47.0%+176法术伤害",
				num = 176,
				rate = 0.47,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对直线上目标造成47.5%+220法术伤害",
				num = 220,
				rate = 0.475,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对直线上目标造成48.0%+264法术伤害",
				num = 264,
				rate = 0.48,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对直线上目标造成48.5%+308法术伤害",
				num = 308,
				rate = 0.485,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对直线上目标造成49.0%+352法术伤害",
				num = 352,
				rate = 0.49,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对直线上目标造成49.5%+396法术伤害",
				num = 396,
				rate = 0.495,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对直线上目标造成50.0%+440法术伤害",
				num = 440,
				rate = 0.5,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对直线上目标造成50.5%+484法术伤害",
				num = 484,
				rate = 0.505,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对直线上目标造成51.0%+528法术伤害",
				num = 528,
				rate = 0.51,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对直线上目标造成51.5%+572法术伤害",
				num = 572,
				rate = 0.515,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对直线上目标造成52.0%+616法术伤害",
				num = 616,
				rate = 0.52,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对直线上目标造成52.5%+660法术伤害",
				num = 660,
				rate = 0.525,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对直线上目标造成53.0%+704法术伤害",
				num = 704,
				rate = 0.53,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对直线上目标造成53.5%+748法术伤害",
				num = 748,
				rate = 0.535,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对直线上目标造成54.0%+792法术伤害",
				num = 792,
				rate = 0.54,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对直线上目标造成54.5%+836法术伤害",
				num = 836,
				rate = 0.545,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对直线上目标造成55.0%+880法术伤害",
				num = 880,
				rate = 0.55,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对直线上目标造成55.5%+924法术伤害",
				num = 924,
				rate = 0.555,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对直线上目标造成56.0%+968法术伤害",
				num = 968,
				rate = 0.56,
				buffer = {
				},
			},
		},
	},
	["301009"] = {
		name = "极寒地狱",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {taoist = 1,},
		method = 3,
		describe = "在敌人区域内生成一个寒冰地带，对敌方全体造成伤害",
		class = "atk",
		atkType=2,
		atkAction=350013,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对全体目标造成45.5%+40法术伤害",
				num = 40,
				rate = 0.455,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对全体目标造成46.0%+80法术伤害",
				num = 80,
				rate = 0.46,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对全体目标造成46.5%+120法术伤害",
				num = 120,
				rate = 0.465,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对全体目标造成47.0%+160法术伤害",
				num = 160,
				rate = 0.47,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对全体目标造成47.5%+200法术伤害",
				num = 200,
				rate = 0.475,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对全体目标造成48.0%+240法术伤害",
				num = 240,
				rate = 0.48,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对全体目标造成48.5%+280法术伤害",
				num = 280,
				rate = 0.485,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对全体目标造成49.0%+320法术伤害",
				num = 320,
				rate = 0.49,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对全体目标造成49.5%+360法术伤害",
				num = 360,
				rate = 0.495,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对全体目标造成50.0%+400法术伤害",
				num = 400,
				rate = 0.5,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对全体目标造成50.5%+440法术伤害",
				num = 440,
				rate = 0.505,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对全体目标造成51.0%+480法术伤害",
				num = 480,
				rate = 0.51,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对全体目标造成51.5%+520法术伤害",
				num = 520,
				rate = 0.515,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对全体目标造成52.0%+560法术伤害",
				num = 560,
				rate = 0.52,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对全体目标造成52.5%+600法术伤害",
				num = 600,
				rate = 0.525,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对全体目标造成53.0%+640法术伤害",
				num = 640,
				rate = 0.53,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对全体目标造成53.5%+680法术伤害",
				num = 680,
				rate = 0.535,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对全体目标造成54.0%+720法术伤害",
				num = 720,
				rate = 0.54,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对全体目标造成54.5%+760法术伤害",
				num = 760,
				rate = 0.545,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对全体目标造成55.0%+800法术伤害",
				num = 800,
				rate = 0.55,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对全体目标造成55.5%+840法术伤害",
				num = 840,
				rate = 0.555,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对全体目标造成56.0%+880法术伤害",
				num = 880,
				rate = 0.56,
				buffer = {
				},
			},
		},
	},
	["301010"] = {
		name = "惊神镇魂咒",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {taoist2 = 1,},
		method = 3,
		describe = "对敌人造成法术伤害，有几率恐吓敌方使其无法出招",
		class = "atk_buff",
		atkType=2,
		atkAction=350011,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成121%+120法术伤害，50%几率恐吓目标",
				num = 120,
				rate = 1.21,
				buffer = {
					{id = "28", last = 1, occur = 0.5},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成122%+240法术伤害，51%几率恐吓目标",
				num = 240,
				rate = 1.22,
				buffer = {
					{id = "28", last = 1, occur = 0.51},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成123%+360法术伤害，52%几率恐吓目标",
				num = 360,
				rate = 1.23,
				buffer = {
					{id = "28", last = 1, occur = 0.52},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成124%+480法术伤害，53%几率恐吓目标",
				num = 480,
				rate = 1.24,
				buffer = {
					{id = "28", last = 1, occur = 0.53},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成125%+600法术伤害，54%几率恐吓目标",
				num = 600,
				rate = 1.25,
				buffer = {
					{id = "28", last = 1, occur = 0.54},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成126%+720法术伤害，55%几率恐吓目标",
				num = 720,
				rate = 1.26,
				buffer = {
					{id = "28", last = 1, occur = 0.55},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成127%+840法术伤害，56%几率恐吓目标",
				num = 840,
				rate = 1.27,
				buffer = {
					{id = "28", last = 1, occur = 0.56},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成128%+960法术伤害，57%几率恐吓目标",
				num = 960,
				rate = 1.28,
				buffer = {
					{id = "28", last = 1, occur = 0.57},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成129%+1080法术伤害，58%几率恐吓目标",
				num = 1080,
				rate = 1.29,
				buffer = {
					{id = "28", last = 1, occur = 0.58},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成130%+1200法术伤害，59%几率恐吓目标",
				num = 1200,
				rate = 1.3,
				buffer = {
					{id = "28", last = 1, occur = 0.59},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成131%+1320法术伤害，60%几率恐吓目标",
				num = 1320,
				rate = 1.31,
				buffer = {
					{id = "28", last = 1, occur = 0.6},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成132%+1440法术伤害，61%几率恐吓目标",
				num = 1440,
				rate = 1.32,
				buffer = {
					{id = "28", last = 1, occur = 0.61},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成133%+1560法术伤害，62%几率恐吓目标",
				num = 1560,
				rate = 1.33,
				buffer = {
					{id = "28", last = 1, occur = 0.62},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成134%+1680法术伤害，63%几率恐吓目标",
				num = 1680,
				rate = 1.34,
				buffer = {
					{id = "28", last = 1, occur = 0.63},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成135%+1800法术伤害，64%几率恐吓目标",
				num = 1800,
				rate = 1.35,
				buffer = {
					{id = "28", last = 1, occur = 0.64},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成136%+1920法术伤害，65%几率恐吓目标",
				num = 1920,
				rate = 1.36,
				buffer = {
					{id = "28", last = 1, occur = 0.65},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成137%+2040法术伤害，66%几率恐吓目标",
				num = 2040,
				rate = 1.37,
				buffer = {
					{id = "28", last = 1, occur = 0.66},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成138%+2160法术伤害，67%几率恐吓目标",
				num = 2160,
				rate = 1.38,
				buffer = {
					{id = "28", last = 1, occur = 0.67},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成139%+2280法术伤害，68%几率恐吓目标",
				num = 2280,
				rate = 1.39,
				buffer = {
					{id = "28", last = 1, occur = 0.68},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成140%+2400法术伤害，69%几率恐吓目标",
				num = 2400,
				rate = 1.4,
				buffer = {
					{id = "28", last = 1, occur = 0.69},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成141%+2520法术伤害，70%几率恐吓目标",
				num = 2520,
				rate = 1.41,
				buffer = {
					{id = "28", last = 1, occur = 0.7},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成142%+2640法术伤害，71%几率恐吓目标",
				num = 2640,
				rate = 1.42,
				buffer = {
					{id = "28", last = 1, occur = 0.71},
				},
			},
		},
	},
	["301011"] = {
		name = "死亡凋零",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		method = 3,
		describe = "对敌方全体投出毒球造成法术伤害并引起中毒效果",
		class = "atk_buff",
		atkType=2,
		atkAction=350011,
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对全体目标造成38%+40法术伤害并进入中毒状态",
				num = 40,
				rate = 0.38,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.01},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对全体目标造成39%+80法术伤害并进入中毒状态",
				num = 80,
				rate = 0.385,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.015},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对全体目标造成39%+120法术伤害并进入中毒状态",
				num = 120,
				rate = 0.39,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.02},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对全体目标造成40%+160法术伤害并进入中毒状态",
				num = 160,
				rate = 0.395,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.025},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对全体目标造成40%+200法术伤害并进入中毒状态",
				num = 200,
				rate = 0.4,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.03},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对全体目标造成41%+240法术伤害并进入中毒状态",
				num = 240,
				rate = 0.405,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.035},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对全体目标造成41%+280法术伤害并进入中毒状态",
				num = 280,
				rate = 0.41,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.04},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对全体目标造成42%+320法术伤害并进入中毒状态",
				num = 320,
				rate = 0.415,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.045},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对全体目标造成42%+360法术伤害并进入中毒状态",
				num = 360,
				rate = 0.42,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.05},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对全体目标造成43%+400法术伤害并进入中毒状态",
				num = 400,
				rate = 0.425,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.055},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对全体目标造成43%+440法术伤害并进入中毒状态",
				num = 440,
				rate = 0.43,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.06},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对全体目标造成44%+480法术伤害并进入中毒状态",
				num = 480,
				rate = 0.435,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.065},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对全体目标造成44%+520法术伤害并进入中毒状态",
				num = 520,
				rate = 0.44,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.07},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对全体目标造成45%+560法术伤害并进入中毒状态",
				num = 560,
				rate = 0.445,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.075},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对全体目标造成45%+600法术伤害并进入中毒状态",
				num = 600,
				rate = 0.45,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.08},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对全体目标造成46%+640法术伤害并进入中毒状态",
				num = 640,
				rate = 0.455,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.085},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对全体目标造成46%+680法术伤害并进入中毒状态",
				num = 680,
				rate = 0.46,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.09},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对全体目标造成47%+720法术伤害并进入中毒状态",
				num = 720,
				rate = 0.465,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.095},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对全体目标造成47%+760法术伤害并进入中毒状态",
				num = 760,
				rate = 0.47,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.1},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对全体目标造成48%+800法术伤害并进入中毒状态",
				num = 800,
				rate = 0.475,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.105},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对全体目标造成48%+840法术伤害并进入中毒状态",
				num = 840,
				rate = 0.48,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.11},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对全体目标造成49%+880法术伤害并进入中毒状态",
				num = 880,
				rate = 0.485,
				buffer = {
					{id = "44", last = 3, occur = 1, rate = 0.115},
				},
			},
		},
	},
	["301012"] = {
		name = "烈火箭",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		method = 3,
		describe = "射出火箭，对敌方单体造成大量伤害",
		class = "atk",
		atkType=1,
		atkAction=350016,
		quality = 2,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成76%+50物理伤害",
				num = 50,
				rate = 0.76,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成77%+100物理伤害",
				num = 100,
				rate = 0.77,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成78%+150物理伤害",
				num = 150,
				rate = 0.78,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成79%+200物理伤害",
				num = 200,
				rate = 0.79,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成80%+250物理伤害",
				num = 250,
				rate = 0.8,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成81%+300物理伤害",
				num = 300,
				rate = 0.81,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成82%+350物理伤害",
				num = 350,
				rate = 0.82,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成83%+400物理伤害",
				num = 400,
				rate = 0.83,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成84%+450物理伤害",
				num = 450,
				rate = 0.84,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成85%+500物理伤害",
				num = 500,
				rate = 0.85,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成86%+550物理伤害",
				num = 550,
				rate = 0.86,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成87%+600物理伤害",
				num = 600,
				rate = 0.87,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成88%+650物理伤害",
				num = 650,
				rate = 0.88,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成89%+700物理伤害",
				num = 700,
				rate = 0.89,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成90%+750物理伤害",
				num = 750,
				rate = 0.9,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成91%+800物理伤害",
				num = 800,
				rate = 0.91,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成92%+850物理伤害",
				num = 850,
				rate = 0.92,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成93%+900物理伤害",
				num = 900,
				rate = 0.93,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成94%+950物理伤害",
				num = 950,
				rate = 0.94,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成95%+1000物理伤害",
				num = 1000,
				rate = 0.95,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成96%+1050物理伤害",
				num = 1050,
				rate = 0.96,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成97%+1100物理伤害",
				num = 1100,
				rate = 0.97,
				buffer = {
				},
			},
		},
	},
	["301013"] = {
		name = "寒冰箭",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		method = 3,
		describe = "射出一道冰箭，对敌方单体造成伤害，并有几率冰冻目标",
		class = "atk_buff",
		atkType=2,
		atkAction=350017,
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成91%+60物理伤害，20%几率冰冻目标",
				num = 60,
				rate = 0.91,
				buffer = {
					{id = "27", last = 1, occur = 0.2},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成92%+120物理伤害，22%几率冰冻目标",
				num = 120,
				rate = 0.92,
				buffer = {
					{id = "27", last = 1, occur = 0.22},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成93%+180物理伤害，24%几率冰冻目标",
				num = 180,
				rate = 0.93,
				buffer = {
					{id = "27", last = 1, occur = 0.24},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成94%+240物理伤害，26%几率冰冻目标",
				num = 240,
				rate = 0.94,
				buffer = {
					{id = "27", last = 1, occur = 0.26},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成95%+300物理伤害，28%几率冰冻目标",
				num = 300,
				rate = 0.95,
				buffer = {
					{id = "27", last = 1, occur = 0.28},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成96%+360物理伤害，30%几率冰冻目标",
				num = 360,
				rate = 0.96,
				buffer = {
					{id = "27", last = 1, occur = 0.3},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成97%+420物理伤害，32%几率冰冻目标",
				num = 420,
				rate = 0.97,
				buffer = {
					{id = "27", last = 1, occur = 0.32},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成98%+480物理伤害，34%几率冰冻目标",
				num = 480,
				rate = 0.98,
				buffer = {
					{id = "27", last = 1, occur = 0.34},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成99%+540物理伤害，36%几率冰冻目标",
				num = 540,
				rate = 0.99,
				buffer = {
					{id = "27", last = 1, occur = 0.36},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成100%+600物理伤害，38%几率冰冻目标",
				num = 600,
				rate = 1,
				buffer = {
					{id = "27", last = 1, occur = 0.38},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成101%+660物理伤害，40%几率冰冻目标",
				num = 660,
				rate = 1.01,
				buffer = {
					{id = "27", last = 1, occur = 0.4},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成102%+720物理伤害，42%几率冰冻目标",
				num = 720,
				rate = 1.02,
				buffer = {
					{id = "27", last = 1, occur = 0.42},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成103%+780物理伤害，44%几率冰冻目标",
				num = 780,
				rate = 1.03,
				buffer = {
					{id = "27", last = 1, occur = 0.44},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成104%+840物理伤害，46%几率冰冻目标",
				num = 840,
				rate = 1.04,
				buffer = {
					{id = "27", last = 1, occur = 0.46},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成105%+900物理伤害，48%几率冰冻目标",
				num = 900,
				rate = 1.05,
				buffer = {
					{id = "27", last = 1, occur = 0.48},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成106%+960物理伤害，50%几率冰冻目标",
				num = 960,
				rate = 1.06,
				buffer = {
					{id = "27", last = 1, occur = 0.5},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成107%+1020物理伤害，52%几率冰冻目标",
				num = 1020,
				rate = 1.07,
				buffer = {
					{id = "27", last = 1, occur = 0.52},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成108%+1080物理伤害，54%几率冰冻目标",
				num = 1080,
				rate = 1.08,
				buffer = {
					{id = "27", last = 1, occur = 0.54},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成109%+1140物理伤害，56%几率冰冻目标",
				num = 1140,
				rate = 1.09,
				buffer = {
					{id = "27", last = 1, occur = 0.56},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成110%+1200物理伤害，58%几率冰冻目标",
				num = 1200,
				rate = 1.1,
				buffer = {
					{id = "27", last = 1, occur = 0.58},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成111%+1260物理伤害，60%几率冰冻目标",
				num = 1260,
				rate = 1.11,
				buffer = {
					{id = "27", last = 1, occur = 0.6},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成112%+1320物理伤害，62%几率冰冻目标",
				num = 1320,
				rate = 1.12,
				buffer = {
					{id = "27", last = 1, occur = 0.62},
				},
			},
		},
	},
	["301014"] = {
		name = "流星箭雨",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {archer = 1,},
		method = 3,
		describe = "向敌方发射箭雨，对敌方全体造成大量伤害",
		class = "atk",
		atkType=2,
		atkAction=350018,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对全体目标造成45.5%+30法术伤害",
				num = 30,
				rate = 0.455,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对全体目标造成46.0%+60法术伤害",
				num = 60,
				rate = 0.46,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对全体目标造成46.5%+90法术伤害",
				num = 90,
				rate = 0.465,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对全体目标造成47.0%+120法术伤害",
				num = 120,
				rate = 0.47,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对全体目标造成47.5%+150法术伤害",
				num = 150,
				rate = 0.475,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对全体目标造成48.0%+180法术伤害",
				num = 180,
				rate = 0.48,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对全体目标造成48.5%+210法术伤害",
				num = 210,
				rate = 0.485,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对全体目标造成49.0%+240法术伤害",
				num = 240,
				rate = 0.49,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对全体目标造成49.5%+270法术伤害",
				num = 270,
				rate = 0.495,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对全体目标造成50.0%+300法术伤害",
				num = 300,
				rate = 0.5,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对全体目标造成50.5%+330法术伤害",
				num = 330,
				rate = 0.505,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对全体目标造成51.0%+360法术伤害",
				num = 360,
				rate = 0.51,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对全体目标造成51.5%+390法术伤害",
				num = 390,
				rate = 0.515,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对全体目标造成52.0%+420法术伤害",
				num = 420,
				rate = 0.52,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对全体目标造成52.5%+450法术伤害",
				num = 450,
				rate = 0.525,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对全体目标造成53.0%+480法术伤害",
				num = 480,
				rate = 0.53,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对全体目标造成53.5%+510法术伤害",
				num = 510,
				rate = 0.535,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对全体目标造成54.0%+540法术伤害",
				num = 540,
				rate = 0.54,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对全体目标造成54.5%+570法术伤害",
				num = 570,
				rate = 0.545,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对全体目标造成55.0%+600法术伤害",
				num = 600,
				rate = 0.55,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对全体目标造成55.5%+630法术伤害",
				num = 630,
				rate = 0.555,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对全体目标造成56.0%+660法术伤害",
				num = 660,
				rate = 0.56,
				buffer = {
				},
			},
		},
	},
	["301015"] = {
		name = "凝霜箭",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer2 = 1,},
		method = 3,
		describe = "射出一道冰箭，对目标造成伤害并减低敌人的行动速度",
		class = "atk_buff",
		atkType=2,
		atkAction=350017,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造121%+80物理伤害，降低其50行动速度",
				num = 80,
				rate = 1.21,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造122%+160物理伤害，降低其55行动速度",
				num = 160,
				rate = 1.22,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 55},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造123%+240物理伤害，降低其60行动速度",
				num = 240,
				rate = 1.23,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 60},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造124%+320物理伤害，降低其65行动速度",
				num = 320,
				rate = 1.24,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 65},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造125%+400物理伤害，降低其70行动速度",
				num = 400,
				rate = 1.25,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 70},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造126%+480物理伤害，降低其75行动速度",
				num = 480,
				rate = 1.26,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 75},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造127%+560物理伤害，降低其80行动速度",
				num = 560,
				rate = 1.27,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 80},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造128%+640物理伤害，降低其85行动速度",
				num = 640,
				rate = 1.28,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 85},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造129%+720物理伤害，降低其90行动速度",
				num = 720,
				rate = 1.29,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 90},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造130%+800物理伤害，降低其95行动速度",
				num = 800,
				rate = 1.3,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 95},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造131%+880物理伤害，降低其100行动速度",
				num = 880,
				rate = 1.31,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 100},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造132%+960物理伤害，降低其105行动速度",
				num = 960,
				rate = 1.32,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 105},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造133%+1040物理伤害，降低其110行动速度",
				num = 1040,
				rate = 1.33,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 110},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造134%+1120物理伤害，降低其115行动速度",
				num = 1120,
				rate = 1.34,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 115},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造135%+1200物理伤害，降低其120行动速度",
				num = 1200,
				rate = 1.35,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 120},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造136%+1280物理伤害，降低其125行动速度",
				num = 1280,
				rate = 1.36,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 125},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造137%+1360物理伤害，降低其130行动速度",
				num = 1360,
				rate = 1.37,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 130},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造138%+1440物理伤害，降低其135行动速度",
				num = 1440,
				rate = 1.38,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 135},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造139%+1520物理伤害，降低其140行动速度",
				num = 1520,
				rate = 1.39,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 140},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造140%+1600物理伤害，降低其145行动速度",
				num = 1600,
				rate = 1.4,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 145},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造141%+1680物理伤害，降低其150行动速度",
				num = 1680,
				rate = 1.41,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 150},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造142%+1760物理伤害，降低其155行动速度",
				num = 1760,
				rate = 1.42,
				buffer = {
					{id = "2", last = 3, occur = 1, num = 155},
				},
			},
		},
	},
	["301016"] = {
		name = "七杀破体箭",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer3 = 1,},
		method = 3,
		describe = "射出强力破甲箭矢，造成大量伤害并降低敌方防御力",
		class = "atk_buff",
		atkType=2,
		atkAction=350019,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成121%+80物理伤害，降低其10%物理防御",
				num = 80,
				rate = 1.21,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.1},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成122%+160物理伤害，降低其11%物理防御",
				num = 160,
				rate = 1.22,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.11},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成123%+240物理伤害，降低其12%物理防御",
				num = 240,
				rate = 1.23,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.12},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成124%+320物理伤害，降低其13%物理防御",
				num = 320,
				rate = 1.24,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.13},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成125%+400物理伤害，降低其14%物理防御",
				num = 400,
				rate = 1.25,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.14},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成126%+480物理伤害，降低其15%物理防御",
				num = 480,
				rate = 1.26,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.15},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成127%+560物理伤害，降低其16%物理防御",
				num = 560,
				rate = 1.27,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.16},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成128%+640物理伤害，降低其17%物理防御",
				num = 640,
				rate = 1.28,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.17},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成129%+720物理伤害，降低其18%物理防御",
				num = 720,
				rate = 1.29,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.18},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成130%+800物理伤害，降低其19%物理防御",
				num = 800,
				rate = 1.3,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.19},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成131%+880物理伤害，降低其20%物理防御",
				num = 880,
				rate = 1.31,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.2},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成132%+960物理伤害，降低其21%物理防御",
				num = 960,
				rate = 1.32,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.21},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成133%+1040物理伤害，降低其22%物理防御",
				num = 1040,
				rate = 1.33,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.22},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成134%+1120物理伤害，降低其23%物理防御",
				num = 1120,
				rate = 1.34,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.23},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成135%+1200物理伤害，降低其24%物理防御",
				num = 1200,
				rate = 1.35,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.24},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成136%+1280物理伤害，降低其25%物理防御",
				num = 1280,
				rate = 1.36,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.25},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成137%+1360物理伤害，降低其26%物理防御",
				num = 1360,
				rate = 1.37,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.26},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成138%+1440物理伤害，降低其27%物理防御",
				num = 1440,
				rate = 1.38,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.27},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成139%+1520物理伤害，降低其28%物理防御",
				num = 1520,
				rate = 1.39,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.28},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成140%+1600物理伤害，降低其29%物理防御",
				num = 1600,
				rate = 1.4,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.29},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成141%+1680物理伤害，降低其30%物理防御",
				num = 1680,
				rate = 1.41,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.3},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成142%+1760物理伤害，降低其31%物理防御",
				num = 1760,
				rate = 1.42,
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.31},
				},
			},
		},
	},
	["301017"] = {
		name = "小试牛刀",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		method = 3,
		describe = "以普通招式攻击敌人",
		class = "atk",
		atkType=1,
		atkAction=350001,
		quality = 1,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成61%+60法术伤害",
				num = 60,
				rate = 0.61,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成62%+120法术伤害",
				num = 120,
				rate = 0.62,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成63%+180法术伤害",
				num = 180,
				rate = 0.63,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成64%+240法术伤害",
				num = 240,
				rate = 0.64,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成65%+300法术伤害",
				num = 300,
				rate = 0.65,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成66%+360法术伤害",
				num = 360,
				rate = 0.66,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成67%+420法术伤害",
				num = 420,
				rate = 0.67,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成68%+480法术伤害",
				num = 480,
				rate = 0.68,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成69%+540法术伤害",
				num = 540,
				rate = 0.69,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成70%+600法术伤害",
				num = 600,
				rate = 0.7,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成71%+660法术伤害",
				num = 660,
				rate = 0.71,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成72%+720法术伤害",
				num = 720,
				rate = 0.72,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成73%+780法术伤害",
				num = 780,
				rate = 0.73,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成74%+840法术伤害",
				num = 840,
				rate = 0.74,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成75%+900法术伤害",
				num = 900,
				rate = 0.75,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成76%+960法术伤害",
				num = 960,
				rate = 0.76,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成77%+1020法术伤害",
				num = 1020,
				rate = 0.77,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成78%+1080法术伤害",
				num = 1080,
				rate = 0.78,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成79%+1140法术伤害",
				num = 1140,
				rate = 0.79,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成80%+1200法术伤害",
				num = 1200,
				rate = 0.8,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成81%+1260法术伤害",
				num = 1260,
				rate = 0.81,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "对目标造成82%+1320法术伤害",
				num = 1320,
				rate = 0.82,
				buffer = {
				},
			},
		},
	},
	["301018"] = {
		name = "锋芒毕露",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		method = 3,
		describe = "提升自身攻击力，给敌人予重击",
		class = "atk",
		atkType=3,
		atkAction=350001,
		quality = 2,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成76%+75法术伤害",
				num = 75,
				rate = 0.76,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成77%+150法术伤害",
				num = 150,
				rate = 0.77,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成78%+225法术伤害",
				num = 225,
				rate = 0.78,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成79%+300法术伤害",
				num = 300,
				rate = 0.79,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成80%+375法术伤害",
				num = 375,
				rate = 0.8,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成81%+450法术伤害",
				num = 450,
				rate = 0.81,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成82%+525法术伤害",
				num = 525,
				rate = 0.82,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成83%+600法术伤害",
				num = 600,
				rate = 0.83,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成84%+675法术伤害",
				num = 675,
				rate = 0.84,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成85%+750法术伤害",
				num = 750,
				rate = 0.85,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成86%+825法术伤害",
				num = 825,
				rate = 0.86,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成87%+900法术伤害",
				num = 900,
				rate = 0.87,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成88%+975法术伤害",
				num = 975,
				rate = 0.88,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成89%+1050法术伤害",
				num = 1050,
				rate = 0.89,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成90%+1125法术伤害",
				num = 1125,
				rate = 0.9,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成91%+1200法术伤害",
				num = 1200,
				rate = 0.91,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成92%+1275法术伤害",
				num = 1275,
				rate = 0.92,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成93%+1350法术伤害",
				num = 1350,
				rate = 0.93,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成94%+1425法术伤害",
				num = 1425,
				rate = 0.94,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成95%+1500法术伤害",
				num = 1500,
				rate = 0.95,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成96%+1575法术伤害",
				num = 1575,
				rate = 0.96,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成97%+1650法术伤害",
				num = 1650,
				rate = 0.97,
				buffer = {
				},
			},
		},
	},
	["301019"] = {
		name = "小试牛刀",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		method = 3,
		describe = "以普通招式攻击敌人",
		class = "atk",
		atkType=1,
		atkAction=350001,
		quality = 1,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成61%+40物理伤害",
				num = 40,
				rate = 0.61,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成62%+80物理伤害",
				num = 80,
				rate = 0.62,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成63%+120物理伤害",
				num = 120,
				rate = 0.63,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成64%+160物理伤害",
				num = 160,
				rate = 0.64,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成65%+200物理伤害",
				num = 200,
				rate = 0.65,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成66%+240物理伤害",
				num = 240,
				rate = 0.66,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成67%+280物理伤害",
				num = 280,
				rate = 0.67,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成68%+320物理伤害",
				num = 320,
				rate = 0.68,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成69%+360物理伤害",
				num = 360,
				rate = 0.69,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成70%+400物理伤害",
				num = 400,
				rate = 0.7,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成71%+440物理伤害",
				num = 440,
				rate = 0.71,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成72%+480物理伤害",
				num = 480,
				rate = 0.72,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成73%+520物理伤害",
				num = 520,
				rate = 0.73,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成74%+560物理伤害",
				num = 560,
				rate = 0.74,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成75%+600物理伤害",
				num = 600,
				rate = 0.75,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成76%+640物理伤害",
				num = 640,
				rate = 0.76,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成77%+680物理伤害",
				num = 680,
				rate = 0.77,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成78%+720物理伤害",
				num = 720,
				rate = 0.78,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成79%+760物理伤害",
				num = 760,
				rate = 0.79,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成80%+800物理伤害",
				num = 800,
				rate = 0.8,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成81%+840物理伤害",
				num = 840,
				rate = 0.81,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成82%+880物理伤害",
				num = 880,
				rate = 0.82,
				buffer = {
				},
			},
		},
	},
	["301020"] = {
		name = "锋芒毕露",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		method = 3,
		describe = "提升自身攻击力，给敌人予重击",
		class = "atk",
		atkType=3,
		atkAction=350001,
		quality = 2,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "对目标造成76%+50物理伤害",
				num = 50,
				rate = 0.76,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 1,
				explain = "对目标造成77%+100物理伤害",
				num = 100,
				rate = 0.77,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 1,
				explain = "对目标造成78%+150物理伤害",
				num = 150,
				rate = 0.78,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 1,
				explain = "对目标造成79%+200物理伤害",
				num = 200,
				rate = 0.79,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 1,
				explain = "对目标造成80%+250物理伤害",
				num = 250,
				rate = 0.8,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 1,
				explain = "对目标造成81%+300物理伤害",
				num = 300,
				rate = 0.81,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 1,
				explain = "对目标造成82%+350物理伤害",
				num = 350,
				rate = 0.82,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 1,
				explain = "对目标造成83%+400物理伤害",
				num = 400,
				rate = 0.83,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 1,
				explain = "对目标造成84%+450物理伤害",
				num = 450,
				rate = 0.84,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 1,
				explain = "对目标造成85%+500物理伤害",
				num = 500,
				rate = 0.85,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 1,
				explain = "对目标造成86%+550物理伤害",
				num = 550,
				rate = 0.86,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 1,
				explain = "对目标造成87%+600物理伤害",
				num = 600,
				rate = 0.87,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 1,
				explain = "对目标造成88%+650物理伤害",
				num = 650,
				rate = 0.88,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 1,
				explain = "对目标造成89%+700物理伤害",
				num = 700,
				rate = 0.89,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 1,
				explain = "对目标造成90%+750物理伤害",
				num = 750,
				rate = 0.9,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 1,
				explain = "对目标造成91%+800物理伤害",
				num = 800,
				rate = 0.91,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 1,
				explain = "对目标造成92%+850物理伤害",
				num = 850,
				rate = 0.92,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 1,
				explain = "对目标造成93%+900物理伤害",
				num = 900,
				rate = 0.93,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 1,
				explain = "对目标造成94%+950物理伤害",
				num = 950,
				rate = 0.94,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 1,
				explain = "对目标造成95%+1000物理伤害",
				num = 1000,
				rate = 0.95,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 1,
				explain = "对目标造成96%+1050物理伤害",
				num = 1050,
				rate = 0.96,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 1,
				explain = "对目标造成97%+1100物理伤害",
				num = 1100,
				rate = 0.97,
				buffer = {
				},
			},
		},
	},
	["302001"] = {
		name = "回春术",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 7,	--目标范围
		validJob = {taoist3 = 1,},
		describe = "对己方单个受伤角色进行治疗",
		class = "restore",
		atkType=3,
		atkAction=350014,
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 2,
				explain = "随机恢复我方单人200兵力",
				num = 200,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 2,
				explain = "随机恢复我方单人500兵力",
				num = 500,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 2,
				explain = "随机恢复我方单人800兵力",
				num = 800,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 2,
				explain = "随机恢复我方单人1100兵力",
				num = 1100,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 2,
				explain = "随机恢复我方单人1400兵力",
				num = 1400,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 2,
				explain = "随机恢复我方单人1700兵力",
				num = 1700,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 2,
				explain = "随机恢复我方单人2000兵力",
				num = 2000,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 2,
				explain = "随机恢复我方单人2300兵力",
				num = 2300,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 2,
				explain = "随机恢复我方单人2600兵力",
				num = 2600,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 2,
				explain = "随机恢复我方单人2900兵力",
				num = 2900,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 2,
				explain = "随机恢复我方单人3200兵力",
				num = 3200,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 2,
				explain = "随机恢复我方单人3500兵力",
				num = 3500,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 2,
				explain = "随机恢复我方单人3800兵力",
				num = 3800,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 2,
				explain = "随机恢复我方单人4100兵力",
				num = 4100,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 2,
				explain = "随机恢复我方单人4400兵力",
				num = 4400,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 2,
				explain = "随机恢复我方单人4700兵力",
				num = 4700,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 2,
				explain = "随机恢复我方单人5000兵力",
				num = 5000,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 2,
				explain = "随机恢复我方单人5300兵力",
				num = 5300,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 2,
				explain = "随机恢复我方单人5600兵力",
				num = 5600,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 2,
				explain = "随机恢复我方单人5900兵力",
				num = 5900,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 2,
				explain = "随机恢复我方单人6200兵力",
				num = 6200,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 2,
				explain = "随机恢复我方单人6500兵力",
				num = 6500,
				buffer = {
				},
			},
		},
	},
	["302002"] = {
		name = "春风化雨",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {taoist3 = 1,},
		describe = "对己方受伤全体角色进行治疗",
		class = "restore",
		atkType=3,
		atkAction=350015,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 4,
				explain = "恢复我方全体100兵力",
				num = 100,
				buffer = {
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 4,
				explain = "恢复我方全体250兵力",
				num = 250,
				buffer = {
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 4,
				explain = "恢复我方全体400兵力",
				num = 400,
				buffer = {
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 4,
				explain = "恢复我方全体550兵力",
				num = 550,
				buffer = {
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 4,
				explain = "恢复我方全体700兵力",
				num = 700,
				buffer = {
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 4,
				explain = "恢复我方全体850兵力",
				num = 850,
				buffer = {
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 4,
				explain = "恢复我方全体1000兵力",
				num = 1000,
				buffer = {
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 4,
				explain = "恢复我方全体1150兵力",
				num = 1150,
				buffer = {
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 4,
				explain = "恢复我方全体1300兵力",
				num = 1300,
				buffer = {
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 4,
				explain = "恢复我方全体1450兵力",
				num = 1450,
				buffer = {
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 4,
				explain = "恢复我方全体1600兵力",
				num = 1600,
				buffer = {
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 4,
				explain = "恢复我方全体1750兵力",
				num = 1750,
				buffer = {
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 4,
				explain = "恢复我方全体1900兵力",
				num = 1900,
				buffer = {
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 4,
				explain = "恢复我方全体2050兵力",
				num = 2050,
				buffer = {
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 4,
				explain = "恢复我方全体2200兵力",
				num = 2200,
				buffer = {
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 4,
				explain = "恢复我方全体2350兵力",
				num = 2350,
				buffer = {
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 4,
				explain = "恢复我方全体2500兵力",
				num = 2500,
				buffer = {
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 4,
				explain = "恢复我方全体2650兵力",
				num = 2650,
				buffer = {
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 4,
				explain = "恢复我方全体2800兵力",
				num = 2800,
				buffer = {
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 4,
				explain = "恢复我方全体2950兵力",
				num = 2950,
				buffer = {
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 4,
				explain = "恢复我方全体3100兵力",
				num = 3100,
				buffer = {
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 4,
				explain = "恢复我方全体3250兵力",
				num = 3250,
				buffer = {
				},
			},
		},
	},
	["302003"] = {
		name = "龙神之力",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "召唤神龙附体，大量提升自身物理攻击力",
		class = "buff",
		atkType=3,
		atkAction=350005,
		quality = 3,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升自身50物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升自身150物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 150},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升自身250物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 250},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升自身350物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 350},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升自身450物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 450},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升自身550物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 550},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升自身650物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 650},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升自身750物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 750},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升自身850物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 850},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升自身950物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 950},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升自身1050物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1050},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升自身1150物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1150},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升自身1250物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1250},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升自身1350物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1350},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升自身1450物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1450},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升自身1550物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1550},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升自身1650物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1650},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升自身1750物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1750},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升自身1850物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1850},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升自身1950物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 1950},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升自身2050物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 2050},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升自身2150物理攻击",
				buffer = {
					{id = "3", last = 3, occur = 1, num = 2150},
				},
			},
		},
	},
	["302004"] = {
		name = "冥灵护体",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "运用灵力护身，大量提升自身法术防御力",
		class = "buff",
		atkType=3,
		atkAction=350006,
		quality = 2,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升自身40法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 40},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升自身100法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 100},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升自身160法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 160},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升自身220法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 220},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升自身280法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 280},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升自身340法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 340},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升自身400法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 400},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升自身460法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 460},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升自身520法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 520},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升自身580法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 580},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升自身640法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 640},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升自身700法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 700},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升自身760法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 760},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升自身820法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 820},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升自身880法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 880},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升自身940法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 940},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升自身1000法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1000},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升自身1060法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1060},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升自身1120法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1120},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升自身1180法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1180},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升自身1240法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1240},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升自身1300法术防御",
				buffer = {
					{id = "13", last = 3, occur = 1, num = 1300},
				},
			},
		},
	},
	["302005"] = {
		name = "血刃",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "使用后，每次对敌人造成伤害时都会造成额外伤害",
		class = "buff",
		atkType=3,
		atkAction=350007,
		quality = 1,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后攻击时附带50额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后攻击时附带150额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 150},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后攻击时附带250额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 250},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后攻击时附带350额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 350},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后攻击时附带450额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 450},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后攻击时附带550额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 550},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后攻击时附带650额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 650},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后攻击时附带750额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 750},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后攻击时附带850额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 850},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后攻击时附带950额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 950},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后攻击时附带1050额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1050},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后攻击时附带1150额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1150},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后攻击时附带1250额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1250},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后攻击时附带1350额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1350},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后攻击时附带1450额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1450},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后攻击时附带1550额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1550},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后攻击时附带1650额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1650},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后攻击时附带1750额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1750},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后攻击时附带1850额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1850},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后攻击时附带1950额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 1950},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后攻击时附带2050额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 2050},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后攻击时附带2150额外伤害",
				buffer = {
					{id = "32", last = 3, occur = 1, num = 2150},
				},
			},
		},
	},
	["302006"] = {
		name = "金刚护体",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "吸引敌人攻击，并降低自身所受伤害",
		class = "buff",
		atkType=3,
		atkAction=350008,
		quality = 4,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害15%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.15},
					{id = "45", last = 3, occur = 1},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害16%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.16},
					{id = "45", last = 3, occur = 1},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害17%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.17},
					{id = "45", last = 3, occur = 1},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害18%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.18},
					{id = "45", last = 3, occur = 1},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害19%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.19},
					{id = "45", last = 3, occur = 1},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害20%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.2},
					{id = "45", last = 3, occur = 1},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害21%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.21},
					{id = "45", last = 3, occur = 1},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害22%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.22},
					{id = "45", last = 3, occur = 1},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害23%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.23},
					{id = "45", last = 3, occur = 1},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害24%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.24},
					{id = "45", last = 3, occur = 1},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害25%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.25},
					{id = "45", last = 3, occur = 1},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害26%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.26},
					{id = "45", last = 3, occur = 1},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害27%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.27},
					{id = "45", last = 3, occur = 1},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害28%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.28},
					{id = "45", last = 3, occur = 1},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害29%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.29},
					{id = "45", last = 3, occur = 1},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害30%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.3},
					{id = "45", last = 3, occur = 1},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害31%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.31},
					{id = "45", last = 3, occur = 1},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害32%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.32},
					{id = "45", last = 3, occur = 1},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害33%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.33},
					{id = "45", last = 3, occur = 1},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害34%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.34},
					{id = "45", last = 3, occur = 1},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害35%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.35},
					{id = "45", last = 3, occur = 1},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后吸引敌人攻击，并降低自身所受伤害36%",
				buffer = {
					{id = "35", last = 3, occur = 1, rate = 0.36},
					{id = "45", last = 3, occur = 1},
				},
			},
		},
	},
	["302007"] = {
		name = "势如破竹",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "提升自身气势，一定回合内提升自己的攻击伤害",
		class = "buff",
		atkType=3,
		atkAction=350009,
		quality = 5,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升22%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.22},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升23.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.235},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升25%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.25},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升26.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.265},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升28%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.28},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升29.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.295},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升31%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.31},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升32.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.325},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升34%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.34},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升35.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.355},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升37%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.37},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升38.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.385},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升40%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.4},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升41.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.415},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升43%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.43},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升44.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.445},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升46%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.46},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升47.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.475},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升49%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.49},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升50.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.505},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升52%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.52},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升53.5%的伤害",
				buffer = {
					{id = "34", last = 3, occur = 1, rate = 0.535},
				},
			},
		},
	},
	["302008"] = {
		name = "玄心奥妙诀",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "使用玄心妙术，大量提升自身法术攻击力",
		class = "buff",
		atkType=3,
		atkAction=350014,
		quality = 1,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升自身50法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升自身150法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 150},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升自身250法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 250},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升自身350法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 350},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升自身450法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 450},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升自身550法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 550},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升自身650法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 650},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升自身750法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 750},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升自身850法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 850},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升自身950法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 950},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升自身1050法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1050},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升自身1150法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1150},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升自身1250法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1250},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升自身1350法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1350},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升自身1450法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1450},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升自身1550法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1550},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升自身1650法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1650},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升自身1750法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1750},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升自身1850法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1850},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升自身1950法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 1950},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升自身2050法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 2050},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升自身2150法术攻击",
				buffer = {
					{id = "11", last = 3, occur = 1, num = 2150},
				},
			},
		},
	},
	["302009"] = {
		name = "玄灵法盾",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "布置法术盾保护自身，每次被敌人攻击时都会抵消伤害，直到法盾耐久消失",
		class = "buff",
		atkType=3,
		atkAction=350008,
		quality = 2,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "为自身施放吸收300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 300},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "为自身施放吸收800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 800},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "为自身施放吸收1300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 1300},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "为自身施放吸收1800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 1800},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "为自身施放吸收2300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 2300},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "为自身施放吸收2800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 2800},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "为自身施放吸收3300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 3300},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "为自身施放吸收3800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 3800},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "为自身施放吸收4300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 4300},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "为自身施放吸收4800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 4800},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "为自身施放吸收5300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 5300},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "为自身施放吸收5800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 5800},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "为自身施放吸收6300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 6300},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "为自身施放吸收6800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 6800},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "为自身施放吸收7300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 7300},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "为自身施放吸收7800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 7800},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "为自身施放吸收8300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 8300},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "为自身施放吸收8800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 8800},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "为自身施放吸收9300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 9300},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "为自身施放吸收9800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 9800},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "为自身施放吸收10300伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 10300},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "为自身施放吸收10800伤害的法术盾",
				buffer = {
					{id = "38", last = 3, occur = 1, num = 10800},
				},
			},
		},
	},
	["302010"] = {
		name = "御灵护体",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "召唤御灵护身，每次被攻击时几率对攻击者进行法术反击",
		class = "buff",
		atkType=3,
		atkAction=350010,
		quality = 4,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后被攻击时，20%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.2},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后被攻击时，22%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.22},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后被攻击时，24%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.24},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后被攻击时，26%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.26},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后被攻击时，28%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.28},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后被攻击时，30%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.3},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后被攻击时，32%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.32},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后被攻击时，34%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.34},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后被攻击时，36%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.36},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后被攻击时，38%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.38},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后被攻击时，40%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.4},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后被攻击时，42%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.42},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后被攻击时，44%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.44},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后被攻击时，46%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.46},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后被攻击时，48%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.48},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后被攻击时，50%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.5},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后被攻击时，52%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.52},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后被攻击时，54%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.54},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后被攻击时，56%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.56},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后被攻击时，58%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.58},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后被攻击时，60%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.6},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后被攻击时，62%几率进行反击",
				buffer = {
					{id = "23", last = 3, occur = 1, rate = 0.62},
				},
			},
		},
	},
	["302011"] = {
		name = "巫灵境",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "在身前布置一面灵境，将伤害完全转移给攻击者；如果没有敌人击中你，该状态持续到冷却",
		class = "buff",
		atkType=3,
		atkAction=350004,
		quality = 5,
		lastTimes = 1,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹20%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.2},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹21%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.21},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹22%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.22},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹23%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.23},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹24%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.24},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹25%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.25},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹26%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.26},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹27%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.27},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹28%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.28},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹29%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.29},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹30%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.3},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹31%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.31},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹32%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.32},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹33%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.33},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹34%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.34},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹35%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.35},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹36%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.36},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹37%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.37},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹38%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.38},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹39%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.39},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹40%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.4},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后自身无敌，且被敌人击中时对敌人反弹41%的伤害",
				buffer = {
					{id = "37", last = 1, occur = 1, rate = 0.41},
				},
			},
		},
	},
	["302012"] = {
		name = "献祭提灵",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "召唤魔神附体，降低物理防御提升自身法术攻击力",
		class = "buff",
		atkType=3,
		atkAction=350007,
		quality = 3,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后自身减少14%物理防御，提升22%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.14},
					{id = "15", last = 3, occur = 1, rate = 0.22},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后自身减少15%物理防御，提升23%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.15},
					{id = "15", last = 3, occur = 1, rate = 0.23},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后自身减少16%物理防御，提升24%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.16},
					{id = "15", last = 3, occur = 1, rate = 0.24},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后自身减少17%物理防御，提升25%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.17},
					{id = "15", last = 3, occur = 1, rate = 0.25},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后自身减少18%物理防御，提升26%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.18},
					{id = "15", last = 3, occur = 1, rate = 0.26},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后自身减少19%物理防御，提升27%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.19},
					{id = "15", last = 3, occur = 1, rate = 0.27},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后自身减少20%物理防御，提升28%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.2},
					{id = "15", last = 3, occur = 1, rate = 0.28},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后自身减少21%物理防御，提升29%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.21},
					{id = "15", last = 3, occur = 1, rate = 0.29},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后自身减少22%物理防御，提升30%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.22},
					{id = "15", last = 3, occur = 1, rate = 0.3},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后自身减少23%物理防御，提升31%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.23},
					{id = "15", last = 3, occur = 1, rate = 0.31},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后自身减少24%物理防御，提升32%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.24},
					{id = "15", last = 3, occur = 1, rate = 0.32},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后自身减少25%物理防御，提升33%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.25},
					{id = "15", last = 3, occur = 1, rate = 0.33},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "释放后自身减少26%物理防御，提升34%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.26},
					{id = "15", last = 3, occur = 1, rate = 0.34},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "释放后自身减少27%物理防御，提升35%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.27},
					{id = "15", last = 3, occur = 1, rate = 0.35},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "释放后自身减少28%物理防御，提升36%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.28},
					{id = "15", last = 3, occur = 1, rate = 0.36},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "释放后自身减少29%物理防御，提升37%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.29},
					{id = "15", last = 3, occur = 1, rate = 0.37},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "释放后自身减少30%物理防御，提升38%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.3},
					{id = "15", last = 3, occur = 1, rate = 0.38},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "释放后自身减少31%物理防御，提升39%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.31},
					{id = "15", last = 3, occur = 1, rate = 0.39},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "释放后自身减少32%物理防御，提升40%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.32},
					{id = "15", last = 3, occur = 1, rate = 0.4},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "释放后自身减少33%物理防御，提升41%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.33},
					{id = "15", last = 3, occur = 1, rate = 0.41},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "释放后自身减少34%物理防御，提升42%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.34},
					{id = "15", last = 3, occur = 1, rate = 0.42},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "释放后自身减少35%物理防御，提升43%法术攻击",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.35},
					{id = "15", last = 3, occur = 1, rate = 0.43},
				},
			},
		},
	},
	["302013"] = {
		name = "破灭法目",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "施法于目，看清敌人的弱点，增加自身暴击率",
		class = "buff",
		atkType=3,
		atkAction=350020,
		quality = 4,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升自身800的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 800},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升自身900的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 900},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升自身1000的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1000},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升自身1100的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1100},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升自身1200的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1200},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升自身1300的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1300},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升自身1400的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1400},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升自身1500的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1500},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升自身1600的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1600},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升自身1700的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1700},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升自身1800的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1800},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升自身1900的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 1900},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升自身2000的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2000},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升自身2100的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2100},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升自身2200的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2200},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升自身2300的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2300},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升自身2400的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2400},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升自身2500的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2500},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升自身2600的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2600},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升自身2700的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2700},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升自身2800的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2800},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升自身2900的暴击值",
				buffer = {
					{id = "22", last = 3, occur = 1, num = 2900},
				},
			},
		},
	},
	["302014"] = {
		name = "灵虚御风术",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "驾驭灵风，提升己方阵营全体成员速度",
		class = "buff",
		atkType=3,
		atkAction=350022,
		quality = 3,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后提升我方全体15行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 15},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后提升我方全体20行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 20},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后提升我方全体25行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 25},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后提升我方全体30行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 30},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后提升我方全体35行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 35},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后提升我方全体40行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 40},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后提升我方全体45行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 45},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后提升我方全体50行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 50},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后提升我方全体55行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 55},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后提升我方全体60行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 60},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后提升我方全体65行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 65},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后提升我方全体70行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 70},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后提升我方全体75行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 75},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后提升我方全体80行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 80},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后提升我方全体85行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 85},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后提升我方全体90行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 90},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后提升我方全体95行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 95},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后提升我方全体100行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 100},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后提升我方全体105行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 105},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后提升我方全体110行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 110},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后提升我方全体115行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 115},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后提升我方全体120行动速度",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 120},
				},
			},
		},
	},
	["302015"] = {
		name = "雷影幻身",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "召唤雷神虚影护身，抵消敌人部分攻击",
		class = "buff",
		atkType=3,
		atkAction=350021,
		quality = 1,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方60的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 60},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方150的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 150},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方240的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 240},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方330的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 330},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方420的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 420},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方510的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 510},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方600的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 600},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方690的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 690},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方780的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 780},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方870的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 870},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方960的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 960},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1050的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1050},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1140的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1140},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1230的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1230},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1320的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1320},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1410的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1410},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1500的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1500},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1590的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1590},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1680的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1680},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1770的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1770},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1860的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1860},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后每次被敌方攻击时可以抵消敌方1950的伤害",
				buffer = {
					{id = "39", last = 3, occur = 1, num = 1950},
				},
			},
		},
	},
	["302016"] = {
		name = "嗜杀",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "施放嗜血秘术，能够从自身对敌方所造成的伤害中吸取一部分兵力，转化为自身的兵力值",
		class = "buff",
		atkType=3,
		atkAction=350021,
		quality = 5,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的15%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.15},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的16%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.16},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的17%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.17},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的18%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.18},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的19%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.19},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的20%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.2},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的21%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.21},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的22%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.22},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的23%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.23},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的24%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.24},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的25%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.25},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的26%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.26},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的27%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.27},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的28%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.28},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的29%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.29},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的30%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.3},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的31%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.31},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的32%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.32},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的33%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.33},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的34%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.34},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的35%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.35},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后对目标造成伤害时，可以将伤害的36%转化为自身兵力",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.36},
				},
			},
		},
	},
	["302017"] = {
		name = "暴走",
		nonMiss = 1,	--必中
		type = 2,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "使自身进入狂暴状态，一定回合内，物理攻击力提升，法术防御减少",
		class = "buff",
		atkType=3,
		atkAction=350007,
		quality = 2,
		lastTimes = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "释放后物理攻击力提升10%，法术防御减少5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.1},
					{id = "18", last = 3, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 3,
				explain = "释放后物理攻击力提升11%，法术防御减少5.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.11},
					{id = "18", last = 3, occur = 1, rate = 0.055},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 3,
				explain = "释放后物理攻击力提升12%，法术防御减少6%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.12},
					{id = "18", last = 3, occur = 1, rate = 0.06},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 3,
				explain = "释放后物理攻击力提升13%，法术防御减少6.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.13},
					{id = "18", last = 3, occur = 1, rate = 0.065},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 3,
				explain = "释放后物理攻击力提升14%，法术防御减少7%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.14},
					{id = "18", last = 3, occur = 1, rate = 0.07},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 3,
				explain = "释放后物理攻击力提升15%，法术防御减少7.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.15},
					{id = "18", last = 3, occur = 1, rate = 0.075},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 3,
				explain = "释放后物理攻击力提升16%，法术防御减少8%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.16},
					{id = "18", last = 3, occur = 1, rate = 0.08},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 3,
				explain = "释放后物理攻击力提升17%，法术防御减少8.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.17},
					{id = "18", last = 3, occur = 1, rate = 0.085},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 3,
				explain = "释放后物理攻击力提升18%，法术防御减少9%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.18},
					{id = "18", last = 3, occur = 1, rate = 0.09},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 3,
				explain = "释放后物理攻击力提升19%，法术防御减少9.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.19},
					{id = "18", last = 3, occur = 1, rate = 0.095},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 3,
				explain = "释放后物理攻击力提升20%，法术防御减少10%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.2},
					{id = "18", last = 3, occur = 1, rate = 0.1},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 3,
				explain = "释放后物理攻击力提升21%，法术防御减少10.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.21},
					{id = "18", last = 3, occur = 1, rate = 0.105},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 3,
				explain = "释放后物理攻击力提升22%，法术防御减少11%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.22},
					{id = "18", last = 3, occur = 1, rate = 0.11},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 3,
				explain = "释放后物理攻击力提升23%，法术防御减少11.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.23},
					{id = "18", last = 3, occur = 1, rate = 0.115},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 3,
				explain = "释放后物理攻击力提升24%，法术防御减少12%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.24},
					{id = "18", last = 3, occur = 1, rate = 0.12},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 3,
				explain = "释放后物理攻击力提升25%，法术防御减少12.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.25},
					{id = "18", last = 3, occur = 1, rate = 0.125},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 3,
				explain = "释放后物理攻击力提升26%，法术防御减少13%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.26},
					{id = "18", last = 3, occur = 1, rate = 0.13},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 3,
				explain = "释放后物理攻击力提升27%，法术防御减少13.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.27},
					{id = "18", last = 3, occur = 1, rate = 0.135},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 3,
				explain = "释放后物理攻击力提升28%，法术防御减少14%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.28},
					{id = "18", last = 3, occur = 1, rate = 0.14},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 3,
				explain = "释放后物理攻击力提升29%，法术防御减少14.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.29},
					{id = "18", last = 3, occur = 1, rate = 0.145},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 3,
				explain = "释放后物理攻击力提升30%，法术防御减少15%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.3},
					{id = "18", last = 3, occur = 1, rate = 0.15},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 3,
				explain = "释放后物理攻击力提升31%，法术防御减少15.5%",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.31},
					{id = "18", last = 3, occur = 1, rate = 0.155},
				},
			},
		},
	},
	["303001"] = {
		name = "强身术",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		describe = "强身健体，增加自身物理防御力",
		quality = 1,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身20物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 20},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身80物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 80},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身140物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 140},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身200物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 200},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身260物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 260},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身320物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 320},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身380物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 380},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身440物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 440},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身500物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 500},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身560物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 560},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身620物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 620},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身680物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 680},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身740物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 740},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身800物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 800},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身860物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 860},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身920物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 920},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身980物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 980},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身1040物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 1040},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身1100物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 1100},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身1160物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 1160},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身1220物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 1220},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身1280物理防御",
				buffer = {
					{id = "5", last = 0, occur = 1, num = 1280},
				},
			},
		},
	},
	["303002"] = {
		name = "养生术",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		describe = "养生之道，增加自身兵力上限",
		quality = 2,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身50兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身150兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 150},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身250兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 250},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身350兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 350},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身450兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 450},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身550兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 550},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身650兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 650},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身750兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 750},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身850兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 850},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身950兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 950},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身1050兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1050},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身1150兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1150},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身1250兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1250},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身1350兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1350},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身1450兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1450},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身1550兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1550},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身1650兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1650},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身1750兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1750},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身1850兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1850},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身1950兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 1950},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身2050兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 2050},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身2150兵力上限",
				buffer = {
					{id = "21", last = 0, occur = 1, num = 2150},
				},
			},
		},
	},
	["303003"] = {
		name = "反击",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "学习本技能后，被物理攻击时有几率对攻击者进行物理反击",
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "被攻击时有15%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.15},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "被攻击时有16%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.16},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "被攻击时有17%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.17},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "被攻击时有18%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.18},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "被攻击时有19%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.19},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "被攻击时有20%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.2},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "被攻击时有21%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.21},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "被攻击时有22%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.22},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "被攻击时有23%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.23},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "被攻击时有24%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.24},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "被攻击时有25%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.25},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "被攻击时有26%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.26},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "被攻击时有27%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.27},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "被攻击时有28%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.28},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "被攻击时有29%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.29},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "被攻击时有30%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.3},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "被攻击时有31%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.31},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "被攻击时有32%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.32},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "被攻击时有33%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.33},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "被攻击时有34%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.34},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "被攻击时有35%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.35},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "被攻击时有36%的几率以普通攻击反击对方",
				buffer = {
					{id = "23", last = 0, occur = 1, rate = 0.36},
				},
			},
		},
	},
	["303004"] = {
		name = "凝魄",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "增加法术防御力",
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身30法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 30},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身95法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 95},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身160法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 160},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身225法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 225},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身290法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 290},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身355法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 355},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身420法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 420},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身485法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 485},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身550法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 550},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身615法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 615},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身680法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 680},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身745法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 745},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身810法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 810},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身875法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 875},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身940法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 940},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身1005法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1005},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身1070法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1070},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身1135法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1135},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身1200法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1200},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身1265法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1265},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身1330法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1330},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身1395法术防御",
				buffer = {
					{id = "13", last = 0, occur = 1, num = 1395},
				},
			},
		},
	},
	["303005"] = {
		name = "淬体术",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,warrior2 = 1,warrior3 = 1,},
		describe = "淬练自己的身体，增加物理攻击力",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身80物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 80},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身180物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 180},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身280物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 280},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身380物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 380},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身480物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 480},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身580物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 580},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身680物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 680},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身780物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 780},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身880物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 880},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身980物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 980},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身1080物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1080},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身1180物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1180},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身1280物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1280},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身1380物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1380},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身1480物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1480},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身1580物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1580},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身1680物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1680},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身1780物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1780},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身1880物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1880},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身1980物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 1980},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身2080物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 2080},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身2180物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 2180},
				},
			},
		},
	},
	["303006"] = {
		name = "不败金身",
		type = 4,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {warrior = 1,},
		describe = "在遭受物理攻击时有几率完全免疫伤害。",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "遭到物理攻击时5%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.05},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "遭到物理攻击时6%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.06},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "遭到物理攻击时7%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.07},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "遭到物理攻击时8%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.08},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "遭到物理攻击时9%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.09},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "遭到物理攻击时10%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.1},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "遭到物理攻击时11%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.11},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "遭到物理攻击时12%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.12},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "遭到物理攻击时13%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.13},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "遭到物理攻击时14%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.14},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "遭到物理攻击时15%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.15},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "遭到物理攻击时16%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.16},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "遭到物理攻击时17%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.17},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "遭到物理攻击时18%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.18},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "遭到物理攻击时19%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.19},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "遭到物理攻击时20%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.2},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "遭到物理攻击时21%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.21},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "遭到物理攻击时22%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.22},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "遭到物理攻击时23%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.23},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "遭到物理攻击时24%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.24},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "遭到物理攻击时25%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.25},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "遭到物理攻击时26%的几率进行格挡，格挡后不会受到伤害",
				buffer = {
					{id = "24", last = 0, occur = 0.26},
				},
			},
		},
	},
	["303007"] = {
		name = "凝心破魔咒",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "对敌进行法术攻击时，忽视敌方部分法术防御力",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标2%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.02},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标3%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.03},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标4%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.04},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标5%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.05},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标6%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.06},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标7%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.07},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标8%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.08},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标9%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.09},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标10%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.1},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标11%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.11},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标12%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.12},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标13%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.13},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标14%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.14},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标15%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.15},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标16%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.16},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标17%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.17},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标18%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.18},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标19%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.19},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标20%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.2},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标21%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.21},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标22%的法术防御",
				buffer = {
					{id = "41", last = 0, occur = 1, rate = 0.22},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "使用法术攻击对方时忽视目标23%的法术防御",
				buffer = {
				},
			},
		},
	},
	["303008"] = {
		name = "固魂大法",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "学习后提升自身法术攻击力",
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身50法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 50},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身120法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 120},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身190法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 190},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身260法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 260},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身330法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 330},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身400法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 400},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身470法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 470},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身540法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 540},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身610法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 610},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身680法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 680},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身750法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 750},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身820法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 820},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身890法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 890},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身960法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 960},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身1030法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1030},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身1100法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1100},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身1170法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1170},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身1240法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1240},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身1310法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1310},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身1380法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1380},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身1450法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1450},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身1520法术攻击",
				buffer = {
					{id = "11", last = 0, occur = 1, num = 1520},
				},
			},
		},
	},
	["303009"] = {
		name = "幽谧影体",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,taoist2 = 1,taoist3 = 1,},
		describe = "习得该体质后能够减少所受到的伤害",
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "每次受到攻击时减少30的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 30},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "每次受到攻击时减少60的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 60},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "每次受到攻击时减少110的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 110},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "每次受到攻击时减少150的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 150},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "每次受到攻击时减少190的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 190},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "每次受到攻击时减少230的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 230},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "每次受到攻击时减少270的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 270},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "每次受到攻击时减少310的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 310},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "每次受到攻击时减少350的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 350},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "每次受到攻击时减少390的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 390},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "每次受到攻击时减少430的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 430},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "每次受到攻击时减少470的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 470},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "每次受到攻击时减少510的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 510},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "每次受到攻击时减少550的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 550},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "每次受到攻击时减少590的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 590},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "每次受到攻击时减少630的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 630},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "每次受到攻击时减少670的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 670},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "每次受到攻击时减少710的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 710},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "每次受到攻击时减少750的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 750},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "每次受到攻击时减少790的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 790},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "每次受到攻击时减少830的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 830},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "每次受到攻击时减少870的伤害",
				buffer = {
					{id = "33", last = 0, occur = 1, num = 870},
				},
			},
		},
	},
	["303010"] = {
		name = "摄魂夺魄",
		type = 4,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {taoist = 1,},
		describe = "每次对敌方造成伤害时，造成百分比的额外伤害。",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升1%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.01},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升2%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.02},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升3%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.03},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升4%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.04},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升5%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.05},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升6%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.06},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升7%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.07},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升8%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.08},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升9%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.09},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升10%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.1},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升11%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.11},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升12%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.12},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升13%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.13},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升14%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.14},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升15%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.15},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升16%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.16},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升17%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.17},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升18%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.18},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升19%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.19},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升20%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.2},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升21%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.21},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "每次攻击目标时，伤害提升22%",
				buffer = {
					{id = "34", last = 0, occur = 1, rate = 0.22},
				},
			},
		},
	},
	["303011"] = {
		name = "身轻如燕",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "提升自身速度",
		quality = 4,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身10行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 10},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身16行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 16},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身22行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 22},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身28行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 28},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身34行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 34},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身40行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 40},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身46行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 46},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身52行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 52},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身58行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 58},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身64行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 64},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身70行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 70},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身76行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 76},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身82行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 82},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身88行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 88},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身94行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 94},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身100行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 100},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身106行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 106},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身112行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 112},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身118行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 118},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身124行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 124},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身130行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 130},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身136行动速度",
				buffer = {
					{id = "1", last = 0, occur = 1, num = 136},
				},
			},
		},
	},
	["303012"] = {
		name = "不灭魔体",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "学习后提升自身物理攻击力与法术防御力",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身35物理攻击和40法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 35},
					{id = "13", last = 0, occur = 1, num = 40},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身80物理攻击和90法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 80},
					{id = "13", last = 0, occur = 1, num = 90},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身125物理攻击和140法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 125},
					{id = "13", last = 0, occur = 1, num = 140},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身170物理攻击和190法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 170},
					{id = "13", last = 0, occur = 1, num = 190},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身215物理攻击和240法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 215},
					{id = "13", last = 0, occur = 1, num = 240},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身260物理攻击和290法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 260},
					{id = "13", last = 0, occur = 1, num = 290},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身305物理攻击和340法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 305},
					{id = "13", last = 0, occur = 1, num = 340},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身350物理攻击和390法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 350},
					{id = "13", last = 0, occur = 1, num = 390},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身395物理攻击和440法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 395},
					{id = "13", last = 0, occur = 1, num = 440},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身440物理攻击和490法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 440},
					{id = "13", last = 0, occur = 1, num = 490},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身485物理攻击和540法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 485},
					{id = "13", last = 0, occur = 1, num = 540},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身530物理攻击和590法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 530},
					{id = "13", last = 0, occur = 1, num = 590},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身575物理攻击和640法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 575},
					{id = "13", last = 0, occur = 1, num = 640},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身620物理攻击和690法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 620},
					{id = "13", last = 0, occur = 1, num = 690},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身665物理攻击和740法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 665},
					{id = "13", last = 0, occur = 1, num = 740},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身710物理攻击和790法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 710},
					{id = "13", last = 0, occur = 1, num = 790},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身755物理攻击和840法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 755},
					{id = "13", last = 0, occur = 1, num = 840},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身800物理攻击和890法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 800},
					{id = "13", last = 0, occur = 1, num = 890},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身845物理攻击和940法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 845},
					{id = "13", last = 0, occur = 1, num = 940},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身890物理攻击和990法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 890},
					{id = "13", last = 0, occur = 1, num = 990},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身935物理攻击和1040法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 935},
					{id = "13", last = 0, occur = 1, num = 1040},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身980物理攻击和1090法术防御",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 980},
					{id = "13", last = 0, occur = 1, num = 1090},
				},
			},
		},
	},
	["303013"] = {
		name = "强化淬体术",
		type = 3,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,archer3 = 1,archer2 = 1,},
		describe = "遂练自己的身体，增加大量物理攻击力",
		quality = 3,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "提升自身30物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 30},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "提升自身60物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 60},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "提升自身90物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 90},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "提升自身120物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 120},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "提升自身150物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 150},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "提升自身180物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 180},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "提升自身210物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 210},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "提升自身240物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 240},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "提升自身270物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 270},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "提升自身300物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 300},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "提升自身330物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 330},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "提升自身360物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 360},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "提升自身390物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 390},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "提升自身420物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 420},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "提升自身450物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 450},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "提升自身480物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 480},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "提升自身510物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 510},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "提升自身540物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 540},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "提升自身570物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 570},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "提升自身600物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 600},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "提升自身630物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 630},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "提升自身660物理攻击",
				buffer = {
					{id = "3", last = 0, occur = 1, num = 660},
				},
			},
		},
	},
	["303014"] = {
		name = "九幽连环箭",
		type = 4,	--攻击/辅助/被动/特性
		target = 1,	--自己/我方/敌方
		trigger = 2,	--主动/被动
		range = 1,	--目标范围
		validJob = {archer = 1,},
		describe = "普通攻击时一定几率产生连击。",
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 0,
				explain = "普通攻击时有2%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.02},
				},
			},
			[2] = {
				needLv = 5,
				cdTime = 0,
				explain = "普通攻击时有3%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.03},
				},
			},
			[3] = {
				needLv = 10,
				cdTime = 0,
				explain = "普通攻击时有4%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.04},
				},
			},
			[4] = {
				needLv = 15,
				cdTime = 0,
				explain = "普通攻击时有5%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.05},
				},
			},
			[5] = {
				needLv = 20,
				cdTime = 0,
				explain = "普通攻击时有6%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.06},
				},
			},
			[6] = {
				needLv = 25,
				cdTime = 0,
				explain = "普通攻击时有7%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.07},
				},
			},
			[7] = {
				needLv = 30,
				cdTime = 0,
				explain = "普通攻击时有8%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.08},
				},
			},
			[8] = {
				needLv = 35,
				cdTime = 0,
				explain = "普通攻击时有9%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.09},
				},
			},
			[9] = {
				needLv = 40,
				cdTime = 0,
				explain = "普通攻击时有10%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.1},
				},
			},
			[10] = {
				needLv = 45,
				cdTime = 0,
				explain = "普通攻击时有11%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.11},
				},
			},
			[11] = {
				needLv = 50,
				cdTime = 0,
				explain = "普通攻击时有12%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.12},
				},
			},
			[12] = {
				needLv = 55,
				cdTime = 0,
				explain = "普通攻击时有13%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.13},
				},
			},
			[13] = {
				needLv = 60,
				cdTime = 0,
				explain = "普通攻击时有14%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.14},
				},
			},
			[14] = {
				needLv = 65,
				cdTime = 0,
				explain = "普通攻击时有15%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.15},
				},
			},
			[15] = {
				needLv = 70,
				cdTime = 0,
				explain = "普通攻击时有16%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.16},
				},
			},
			[16] = {
				needLv = 75,
				cdTime = 0,
				explain = "普通攻击时有17%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.17},
				},
			},
			[17] = {
				needLv = 80,
				cdTime = 0,
				explain = "普通攻击时有18%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.18},
				},
			},
			[18] = {
				needLv = 85,
				cdTime = 0,
				explain = "普通攻击时有19%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.19},
				},
			},
			[19] = {
				needLv = 90,
				cdTime = 0,
				explain = "普通攻击时有20%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.2},
				},
			},
			[20] = {
				needLv = 95,
				cdTime = 0,
				explain = "普通攻击时有21%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.21},
				},
			},
			[21] = {
				needLv = 100,
				cdTime = 0,
				explain = "普通攻击时有22%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.22},
				},
			},
			[22] = {
				needLv = 105,
				cdTime = 0,
				explain = "普通攻击时有23%的几率进行连击",
				buffer = {
					{id = "25", last = 0, occur = 0.23},
				},
			},
		},
	},
	["311001"] = {
		name = "灵蛇缚吼",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "幻化本体缠绕敌人，并造成声波伤害",
		class = "stunt",
		atkAction=350037,
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311002"] = {
		name = "战争践踏",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "幻化出本体践踏敌人",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311003"] = {
		name = "雷霆风暴",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "用本体制造出大风暴，卷杀敌人",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311004"] = {
		name = "麒麟幻爪",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "以自身之力幻化锋利的巨爪伤害敌人",
		class = "stunt",
		atkAction=350038,
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311005"] = {
		name = "红莲天火",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "以自身之力引动红莲天火烧伤敌人",
		class = "stunt",
		atkAction=350039,
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311006"] = {
		name = "聚能炎弹",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "汇聚自身强大的仙力，压缩成能力球攻击敌人",
		class = "stunt",
		atkAction=350040,
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311007"] = {
		name = "琥珀之剑",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "以身为引召唤本命之剑，攻击敌人",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311008"] = {
		name = "夜幕雷迎",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "以强大的力量，引发天雷攻击敌人",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311009"] = {
		name = "冰焰龙息",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "喷出龙息伤害敌人",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311010"] = {
		name = "百鸟朝凤",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "百鸟朝凤，万禽皆臣服",
		class = "stunt",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["311011"] = {
		name = "琥珀之羽",
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 3,	--主动/被动
		range = 3,	--目标范围
		validJob = {pet = 1,},
		method = 5,
		describe = "羽毛化作密集的利剑穿透敌人",
		class = "stunt",
		atkAction=350041,
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "对敌方全体造成1.05倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.05,
				buffer = {
				},
			},
			[2] = {
				cdTime = 0,
				explain = "对敌方全体造成1.1倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.1,
				buffer = {
				},
			},
			[3] = {
				cdTime = 0,
				explain = "对敌方全体造成1.15倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.15,
				buffer = {
				},
			},
			[4] = {
				cdTime = 0,
				explain = "对敌方全体造成1.2倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.2,
				buffer = {
				},
			},
			[5] = {
				cdTime = 0,
				explain = "对敌方全体造成1.25倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.25,
				buffer = {
				},
			},
			[6] = {
				cdTime = 0,
				explain = "对敌方全体造成1.3倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.3,
				buffer = {
				},
			},
			[7] = {
				cdTime = 0,
				explain = "对敌方全体造成1.35倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.35,
				buffer = {
				},
			},
			[8] = {
				cdTime = 0,
				explain = "对敌方全体造成1.4倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.4,
				buffer = {
				},
			},
			[9] = {
				cdTime = 0,
				explain = "对敌方全体造成1.45倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.45,
				buffer = {
				},
			},
			[10] = {
				cdTime = 0,
				explain = "对敌方全体造成1.5倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.5,
				buffer = {
				},
			},
			[11] = {
				cdTime = 0,
				explain = "对敌方全体造成1.55倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.55,
				buffer = {
				},
			},
			[12] = {
				cdTime = 0,
				explain = "对敌方全体造成1.6倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.6,
				buffer = {
				},
			},
			[13] = {
				cdTime = 0,
				explain = "对敌方全体造成1.65倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.65,
				buffer = {
				},
			},
			[14] = {
				cdTime = 0,
				explain = "对敌方全体造成1.7倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.7,
				buffer = {
				},
			},
			[15] = {
				cdTime = 0,
				explain = "对敌方全体造成1.75倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.75,
				buffer = {
				},
			},
			[16] = {
				cdTime = 0,
				explain = "对敌方全体造成1.8倍主将攻击力的伤害，可通过仙器提升等级",
				num = 1.8,
				buffer = {
				},
			},
		},
	},
	["340001"] = {
		name = "日暮之境",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {},
		describe = "防守时前来进攻的敌人全体进入减速状态，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少100，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 100},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少150，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 150},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少200，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 200},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少250，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 250},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少300，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 300},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时敌全体行动速度减少350，持续1回合",
				buffer = {
					{id = "2", last = 1, occur = 1, num = 350},
				},
			},
		},
	},
	["340002"] = {
		name = "冰雪之息",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {},
		describe = "防守时前来进攻的敌人全体进入冰冻状态，持续的回合数与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续1回合",
				buffer = {
					{id = "27", last = 1, occur = 1},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续2回合",
				buffer = {
					{id = "27", last = 2, occur = 1},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续3回合",
				buffer = {
					{id = "27", last = 3, occur = 1},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续4回合",
				buffer = {
					{id = "27", last = 4, occur = 1},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续5回合",
				buffer = {
					{id = "27", last = 5, occur = 1},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时敌全体进入冰冻状态，持续6回合",
				buffer = {
					{id = "27", last = 6, occur = 1},
				},
			},
		},
	},
	["340003"] = {
		name = "复苏之息",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "防守时己方全体回复兵力，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续1回合",
				buffer = {
					{id = "42", last = 1, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续2回合",
				buffer = {
					{id = "42", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续3回合",
				buffer = {
					{id = "42", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续4回合",
				buffer = {
					{id = "42", last = 4, occur = 1, rate = 0.05},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续5回合",
				buffer = {
					{id = "42", last = 5, occur = 1, rate = 0.05},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时己方全体回复兵力5%，持续6回合",
				buffer = {
					{id = "42", last = 6, occur = 1, rate = 0.05},
				},
			},
		},
	},
	["340004"] = {
		name = "颤栗之地",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {},
		describe = "防守时前来进攻的敌人全体进入恐吓状态，持续的回合数与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续1回合",
				buffer = {
					{id = "28", last = 1, occur = 1},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续2回合",
				buffer = {
					{id = "28", last = 2, occur = 1},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续3回合",
				buffer = {
					{id = "28", last = 3, occur = 1},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续4回合",
				buffer = {
					{id = "28", last = 4, occur = 1},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续5回合",
				buffer = {
					{id = "28", last = 5, occur = 1},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时敌全体进入恐吓状态，持续6回合",
				buffer = {
					{id = "28", last = 6, occur = 1},
				},
			},
		},
	},
	["340005"] = {
		name = "星移之阵",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {},
		describe = "防守时前来进攻的敌人全体进入眩晕状态，持续的回合数与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续1回合",
				buffer = {
					{id = "26", last = 1, occur = 1},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续2回合",
				buffer = {
					{id = "26", last = 2, occur = 1},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续3回合",
				buffer = {
					{id = "26", last = 3, occur = 1},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续4回合",
				buffer = {
					{id = "26", last = 4, occur = 1},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续5回合",
				buffer = {
					{id = "26", last = 5, occur = 1},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时敌全体进入眩晕状态，持续6回合",
				buffer = {
					{id = "26", last = 6, occur = 1},
				},
			},
		},
	},
	["340006"] = {
		name = "腐朽之境",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {},
		describe = "防守时前来进攻的敌人全体进入中毒状态，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续1回合",
				buffer = {
					{id = "42", last = 1, occur = 1, rate = -0.05},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续2回合",
				buffer = {
					{id = "42", last = 2, occur = 1, rate = -0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续3回合",
				buffer = {
					{id = "42", last = 3, occur = 1, rate = -0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续4回合",
				buffer = {
					{id = "42", last = 4, occur = 1, rate = -0.05},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续5回合",
				buffer = {
					{id = "42", last = 5, occur = 1, rate = -0.05},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "防守时敌方全体兵力减少5%，持续6回合",
				buffer = {
					{id = "42", last = 6, occur = 1, rate = -0.05},
				},
			},
		},
	},
	["340007"] = {
		name = "嗜血杀意",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进攻时己方全体进入嗜血状态，持续的回合数与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续1回合",
				buffer = {
					{id = "29", last = 1, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续2回合",
				buffer = {
					{id = "29", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续3回合",
				buffer = {
					{id = "29", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续4回合",
				buffer = {
					{id = "29", last = 4, occur = 1, rate = 0.05},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续5回合",
				buffer = {
					{id = "29", last = 5, occur = 1, rate = 0.05},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "进攻时己方队员回复攻击时造成伤害5%的兵力，持续6回合",
				buffer = {
					{id = "29", last = 6, occur = 1, rate = 0.05},
				},
			},
		},
	},
	["340008"] = {
		name = "法力宿印",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体进入魔神附体状态，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续1回合",
				buffer = {
					{id = "10", last = 1, occur = 1, rate = 0.04},
					{id = "15", last = 1, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续2回合",
				buffer = {
					{id = "10", last = 2, occur = 1, rate = 0.04},
					{id = "15", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续3回合",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.04},
					{id = "15", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续4回合",
				buffer = {
					{id = "10", last = 4, occur = 1, rate = 0.04},
					{id = "15", last = 4, occur = 1, rate = 0.05},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续5回合",
				buffer = {
					{id = "10", last = 5, occur = 1, rate = 0.04},
					{id = "15", last = 5, occur = 1, rate = 0.05},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体物防减少4%，法攻增加5%，持续6回合",
				buffer = {
					{id = "10", last = 6, occur = 1, rate = 0.04},
					{id = "15", last = 6, occur = 1, rate = 0.05},
				},
			},
		},
	},
	["340009"] = {
		name = "武力宿印",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体进入暴走状态，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续1回合",
				buffer = {
					{id = "10", last = 1, occur = 1, rate = 0.04},
					{id = "15", last = 1, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续2回合",
				buffer = {
					{id = "10", last = 2, occur = 1, rate = 0.04},
					{id = "15", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续3回合",
				buffer = {
					{id = "10", last = 3, occur = 1, rate = 0.04},
					{id = "15", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续4回合",
				buffer = {
					{id = "10", last = 4, occur = 1, rate = 0.04},
					{id = "15", last = 4, occur = 1, rate = 0.05},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续5回合",
				buffer = {
					{id = "10", last = 5, occur = 1, rate = 0.04},
					{id = "15", last = 5, occur = 1, rate = 0.05},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体法防减少4%，物攻增加5%，持续6回合",
				buffer = {
					{id = "10", last = 6, occur = 1, rate = 0.04},
					{id = "15", last = 6, occur = 1, rate = 0.05},
				},
			},
		},
	},
	["340010"] = {
		name = "凌虚御风",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体进入加速状态，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续1回合",
				buffer = {
					{id = "1", last = 1, occur = 1, num = 100},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续2回合",
				buffer = {
					{id = "1", last = 2, occur = 1, num = 100},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续3回合",
				buffer = {
					{id = "1", last = 3, occur = 1, num = 100},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续4回合",
				buffer = {
					{id = "1", last = 4, occur = 1, num = 100},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续5回合",
				buffer = {
					{id = "1", last = 5, occur = 1, num = 100},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体行动速度提升100，持续6回合",
				buffer = {
					{id = "1", last = 6, occur = 1, num = 100},
				},
			},
		},
	},
	["340011"] = {
		name = "不坏之身",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体物防上升，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升4%，持续1回合",
				buffer = {
					{id = "9", last = 1, occur = 1, rate = 0.04},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升5%，持续1回合",
				buffer = {
					{id = "9", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升6%，持续1回合",
				buffer = {
					{id = "9", last = 3, occur = 1, rate = 0.06},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升7%，持续1回合",
				buffer = {
					{id = "9", last = 4, occur = 1, rate = 0.07},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升8%，持续1回合",
				buffer = {
					{id = "9", last = 5, occur = 1, rate = 0.08},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体物防提升9%，持续1回合",
				buffer = {
					{id = "9", last = 6, occur = 1, rate = 0.09},
				},
			},
		},
	},
	["340012"] = {
		name = "武力之源",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体物攻上升，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升3%，持续1回合",
				buffer = {
					{id = "7", last = 1, occur = 1, rate = 0.03},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升4%，持续1回合",
				buffer = {
					{id = "7", last = 2, occur = 1, rate = 0.04},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升5%，持续1回合",
				buffer = {
					{id = "7", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升6%，持续1回合",
				buffer = {
					{id = "7", last = 4, occur = 1, rate = 0.06},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升7%，持续1回合",
				buffer = {
					{id = "7", last = 5, occur = 1, rate = 0.07},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体物攻提升8%，持续1回合",
				buffer = {
					{id = "7", last = 6, occur = 1, rate = 0.08},
				},
			},
		},
	},
	["340013"] = {
		name = "不灭之魂",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体法防上升，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升4%，持续1回合",
				buffer = {
					{id = "17", last = 1, occur = 1, rate = 0.04},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升5%，持续1回合",
				buffer = {
					{id = "17", last = 2, occur = 1, rate = 0.05},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升6%，持续1回合",
				buffer = {
					{id = "17", last = 3, occur = 1, rate = 0.06},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升7%，持续1回合",
				buffer = {
					{id = "17", last = 4, occur = 1, rate = 0.07},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升8%，持续1回合",
				buffer = {
					{id = "17", last = 5, occur = 1, rate = 0.08},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体法防提升9%，持续1回合",
				buffer = {
					{id = "17", last = 6, occur = 1, rate = 0.09},
				},
			},
		},
	},
	["340014"] = {
		name = "法力之源",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体法攻上升，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升3%，持续1回合",
				buffer = {
					{id = "15", last = 1, occur = 1, rate = 0.03},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升4%，持续1回合",
				buffer = {
					{id = "15", last = 2, occur = 1, rate = 0.04},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升5%，持续1回合",
				buffer = {
					{id = "15", last = 3, occur = 1, rate = 0.05},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升6%，持续1回合",
				buffer = {
					{id = "15", last = 4, occur = 1, rate = 0.06},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升7%，持续1回合",
				buffer = {
					{id = "15", last = 5, occur = 1, rate = 0.07},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体法攻提升8%，持续1回合",
				buffer = {
					{id = "15", last = 6, occur = 1, rate = 0.08},
				},
			},
		},
	},
	["340015"] = {
		name = "不屈意志",
		nonMiss = 1,	--必中
		type = 5,	--攻击/辅助/被动/特性
		target = 2,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 2,	--目标范围
		validJob = {},
		describe = "进入战斗后己方全体防御上升，持续的回合数和效果与联协城池数有关",
		class = "buff",
		quality = 5,
		list = {
			[1] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续1回合",
				buffer = {
					{id = "9", last = 1, occur = 1, rate = 0.02},
					{id = "17", last = 1, occur = 1, rate = 0.02},
				},
			},
			[2] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续2回合",
				buffer = {
					{id = "9", last = 2, occur = 1, rate = 0.02},
					{id = "17", last = 2, occur = 1, rate = 0.02},
				},
			},
			[3] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续3回合",
				buffer = {
					{id = "9", last = 3, occur = 1, rate = 0.02},
					{id = "17", last = 3, occur = 1, rate = 0.02},
				},
			},
			[4] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续4回合",
				buffer = {
					{id = "9", last = 4, occur = 1, rate = 0.02},
					{id = "17", last = 4, occur = 1, rate = 0.02},
				},
			},
			[5] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续5回合",
				buffer = {
					{id = "9", last = 5, occur = 1, rate = 0.02},
					{id = "17", last = 5, occur = 1, rate = 0.02},
				},
			},
			[6] = {
				cdTime = 0,
				explain = "战斗时己方全体防御提升2%，持续6回合",
				buffer = {
					{id = "9", last = 6, occur = 1, rate = 0.02},
					{id = "17", last = 6, occur = 1, rate = 0.02},
				},
			},
		},
	},
	["304002"] = {
		name = "天兰斩",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350024,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304003"] = {
		name = "爆裂流焰",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350025,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304004"] = {
		name = "毒针攻击",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "akt_buff",
		atkType=2,
		atkAction=350026,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.05},
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.06},
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.07},
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.08},
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.09},
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.1},
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.11},
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.12},
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.13},
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.14},
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.15},
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
					{id = "30", last = 3, occur = 1, rate = 0.16},
				},
			},
		},
	},
	["304005"] = {
		name = "元灵天雷",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350027,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304006"] = {
		name = "噬心毒液",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350028,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304007"] = {
		name = "流光飞逝",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 6,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350029,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304008"] = {
		name = "飓风狂雷",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350030,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304009"] = {
		name = "影闪",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 6,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350031,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304010"] = {
		name = "千钧",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 4,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350032,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304012"] = {
		name = "风云暗涌",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350034,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304013"] = {
		name = "八卦封灵",
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有攻击技能",
		class = "atk",
		atkType=2,
		atkAction=350035,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 652,
				buffer = {
				},
			},
			[2] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1341,
				buffer = {
				},
			},
			[3] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2030,
				buffer = {
				},
			},
			[4] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 2719,
				buffer = {
				},
			},
			[5] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 3408,
				buffer = {
				},
			},
			[6] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4097,
				buffer = {
				},
			},
			[7] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 4786,
				buffer = {
				},
			},
			[8] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 5475,
				buffer = {
				},
			},
			[9] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6164,
				buffer = {
				},
			},
			[10] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 6853,
				buffer = {
				},
			},
			[11] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 7542,
				buffer = {
				},
			},
			[12] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 8231,
				buffer = {
				},
			},
		},
	},
	["304014"] = {
		name = "毒针攻击",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有技能，不解释",
		class = "atk",
		atkType=2,
		atkAction=350026,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1e+07,
				buffer = {
				},
			},
		},
	},
	["304015"] = {
		name = "噬心毒液",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有技能，不解释",
		class = "atk",
		atkType=2,
		atkAction=350028,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1e+07,
				buffer = {
				},
			},
		},
	},
	["304016"] = {
		name = "元灵天雷",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有技能，不解释",
		class = "atk",
		atkType=2,
		atkAction=350027,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1e+07,
				buffer = {
				},
			},
		},
	},
	["304017"] = {
		name = "噬魂",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "怪物特有技能，不解释",
		class = "atk",
		atkType=2,
		atkAction=350036,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 1,
				explain = "怪物特有攻击技能",
				num = 1e+07,
				buffer = {
				},
			},
		},
	},
	["304018"] = {
		name = "至尊咆哮",
		nonMiss = 1,	--必中
		type = 1,	--攻击/辅助/被动/特性
		target = 3,	--自己/我方/敌方
		trigger = 1,	--主动/被动
		range = 3,	--目标范围
		validJob = {warrior = 1,archer2 = 1,taoist = 1,warrior2 = 1,archer = 1,warrior3 = 1,taoist2 = 1,archer3 = 1,taoist3 = 1,},
		method = 1,
		describe = "世界及联盟boss群晕技能",
		class = "atk_buff",
		atkType=2,
		atkAction=350026,
		quality = 5,
		list = {
			[1] = {
				needLv = 1,
				cdTime = 3,
				explain = "对战斗中玩家队伍所有人造成5000绝对值伤害并55%使角色眩晕",
				num = 5000,
				buffer = {
					{id = "26", last = 2, occur = 0.55},
				},
			},
		},
	},
}

function GetTable()
	return autoTable
end
