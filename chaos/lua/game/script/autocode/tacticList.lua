--计谋列表
 local autoTable = {
	["390001"] = {
		name = "以逸待劳",
		type = 0,
		target = 1,
		describe = "坚守城池，1小时内前来进攻的敌人因疲劳而失去城池联协属性加成",
		explain = "受到攻击时对方的联协属性失效",
		needIngot = 300,
		timelong = 3600,
		uplimit = 86400,
		attr = {
		},
	},
	["390002"] = {
		name = "暗度陈仓",
		type = 1,
		target = 2,
		describe = "可越过敌对势力外围，直接攻击邻近外围的城池",
		needIngot = 200,
		attr = {
		},
	},
	["390003"] = {
		name = "调虎离山",
		type = 1,
		target = 2,
		describe = "诱敌离开城池使之无法获得城池联协属性后，直接攻击",
		needIngot = 200,
		attr = {
		},
	},
	["390004"] = {
		name = "秣马厉兵",
		type = 0,
		target = 1,
		describe = "城内成员做好进攻准备，1小时内攻击增加10%",
		explain = "攻击力上升10%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			physcAtkRate = 0.1,
			magicAtkRate = 0.1,
		},
	},
	["390005"] = {
		name = "无懈可击",
		type = 0,
		target = 1,
		describe = "城内成员做好防守准备，1小时内防御增加10%",
		explain = "防御力上升10%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			physcDefRate = 0.1,
			magicDefRate = 0.1,
		},
	},
	["390006"] = {
		name = "声东击西",
		type = 0,
		target = 2,
		describe = "散布虚假战报，1小时内敌人防御降低10%",
		explain = "防御力下降10%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			physcAtkRate = -0.1,
			magicAtkRate = -0.1,
		},
	},
	["390007"] = {
		name = "折戟销戈",
		type = 0,
		target = 2,
		describe = "破坏敌人兵器，1小时内敌人攻击降低10%",
		explain = "攻击力下降10%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			physcDefRate = -0.1,
			magicDefRate = -0.1,
		},
	},
	["390008"] = {
		name = "天道酬勤",
		type = 0,
		target = 1,
		describe = "城内成员辛勤地工作，1小时内产出增加30%",
		explain = "晶石、钱币产出上升30%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			resRate = -0.3,
		},
	},
	["390009"] = {
		name = "釜底抽薪",
		type = 0,
		target = 2,
		describe = "破坏城内设施，1小时内敌人产出降低30%",
		explain = "晶石、钱币产出下降30%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			resRate = 0.3,
		},
	},
	["390010"] = {
		name = "顺手牵羊",
		type = 0,
		target = 1,
		describe = "尽力夺取军资，1小时内掠夺获得钱币和晶石增加50%",
		explain = "掠夺收获上升50%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			pillageIncrease = 0.5,
		},
	},
	["390011"] = {
		name = "坚壁清野",
		type = 0,
		target = 1,
		describe = "严密防守军资，1小时内遭到掠夺时损失减少50%",
		explain = "掠夺收获降低50%",
		needIngot = 100,
		timelong = 3600,
		uplimit = 86400,
		attr = {
			pillageReduce = 0.5,
		},
	},
}

function GetTable()
	return autoTable
end
