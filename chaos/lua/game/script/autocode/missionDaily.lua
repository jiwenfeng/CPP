--任务列表
local autoTable = {
	["420001"] = {
		class = "func",
		type = "daily",
		name = "强化装备",
		aim = "熟练强化功能",
		needLv = 20,
		content = "使用任何装备强化5次。",
		target = {
			{type = "func", id = 6, num = 1},
		},
		shortDesc = "强化装备1次",
		prize = {
		},
		given = {
		},
	},
	["420002"] = {
		class = "func",
		type = "daily",
		name = "炼制仙器",
		aim = "炼制仙器提升能力",
		needLv = 20,
		content = "使用炼器功能炼制5次仙器。",
		target = {
			{type = "func", id = 7, num = 1},
		},
		shortDesc = "炼制仙器1次",
		prize = {
		},
		given = {
		},
	},
	["420003"] = {
		class = "func",
		type = "daily",
		name = "招募伙伴",
		aim = "人多力量大，行走江湖还是要人多靠谱点。",
		needLv = 20,
		content = "到酒馆使用招募功能一次。",
		target = {
			{type = "func", id = 12, num = 1},
		},
		shortDesc = "招募武将1次",
		prize = {
		},
		given = {
		},
	},
	["420004"] = {
		class = "fight",
		type = "daily",
		name = "除害",
		aim = "真炎部落附近的庄稼经常被香猪啃食，前往清除。",
		needLv = 20,
		needCopyId = 600005,
		content = "去穹桥谷击杀香猪10只。",
		target = {
			{type = "fight", id = 220005, num = 10},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420005"] = {
		class = "fight",
		type = "daily",
		name = "扰乱的山贼",
		aim = "西部山麓有群山贼烧杀淫掳无所不干，前往清除。",
		needLv = 20,
		needCopyId = 600013,
		content = "去西部山麓边缘击杀山贼12个。",
		target = {
			{type = "fight", id = 220003, num = 12},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420006"] = {
		class = "fight",
		type = "daily",
		name = "除魔",
		aim = "万窟洞底部出现魔族，前往击杀。",
		needLv = 20,
		needCopyId = 600009,
		content = "去万窟洞底部击杀休明3次。",
		target = {
			{type = "fight", id = 230003, num = 3},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420007"] = {
		class = "fight",
		type = "daily",
		name = "讨厌的蛙声",
		aim = "落霞山的蛙声特别扰人，前往清除青蛙。",
		needLv = 20,
		needCopyId = 600001,
		content = "去落霞山山脚击杀魔化蛙12只。",
		target = {
			{type = "fight", id = 220001, num = 12},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420008"] = {
		class = "fight",
		type = "daily",
		name = "山贼头领",
		aim = "城外有个山贼头领整天聚集山贼们抢东西，前往击杀。",
		needLv = 20,
		needCopyId = 600015,
		content = "去部山麓深处击杀马彪3次。",
		target = {
			{type = "fight", id = 230006, num = 3},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420009"] = {
		class = "fight",
		type = "daily",
		name = "醉汉",
		aim = "酒馆内有一群喝醉的恶汉捣乱，前往海扁他们。",
		needLv = 20,
		needCopyId = 600019,
		content = "去酒馆痛扁恶汉12个。",
		target = {
			{type = "fight", id = 220009, num = 12},
		},
		shortDesc = "去%s痛扁%s",
		prize = {
		},
		given = {
		},
	},
	["420011"] = {
		class = "fight",
		type = "daily",
		name = "除魔",
		aim = "传闻穹桥谷里部有大量妖怪出现，前往消灭。",
		needLv = 20,
		needCopyId = 600006,
		content = "去穹桥谷里部击杀魔化蜥人。",
		target = {
			{type = "fight", id = 230002, num = 3},
		},
		shortDesc = "去%s击杀%s",
		prize = {
		},
		given = {
		},
	},
	["420012"] = {
		class = "func",
		type = "daily",
		name = "攻打野矿",
		aim = "攻打野矿一次",
		needLv = 20,
		content = "攻打野矿",
		target = {
			{type = "func", id = 28, num = 1},
		},
		shortDesc = "攻打野矿一次",
		prize = {
		},
		given = {
		},
	},
	["420013"] = {
		class = "func",
		type = "daily",
		name = "剿灭游荡的怪物",
		aim = "到势力地图剿灭一只游荡的怪物",
		needLv = 20,
		content = "剿灭游荡的怪物",
		target = {
			{type = "func", id = 47, num = 1},
		},
		shortDesc = "剿灭游荡的怪物",
		prize = {
		},
		given = {
		},
	},
	["420014"] = {
		class = "func",
		type = "daily",
		name = "竞技场挑战",
		aim = "参加一次竞技场挑战",
		needLv = 20,
		content = "竞技场挑战",
		target = {
			{type = "func", id = 19, num = 1},
		},
		shortDesc = "竞技场挑战一次",
		prize = {
		},
		given = {
		},
	},
	["420015"] = {
		class = "collect",
		type = "daily",
		name = "救命药草",
		aim = "老酋长陌司伏秦的孙子犯了重病，听说需要紫花兰做药引方能治好。",
		needLv = 20,
		needCopyId = 697016,
		content = "老酋长陌司伏秦的孙子犯了重病，听说需要紫花兰做药引方能治好。",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s收集%s",
		prize = {
		},
		given = {
		},
	},
	["420016"] = {
		class = "collect",
		type = "daily",
		name = "命苦的旺财",
		aim = "听说村口的旺财早上追人跳墙的时候不小心摔到腿了，现在还流血不止，急需止血草止血。",
		needLv = 20,
		needCopyId = 697016,
		content = "听说村口的旺财早上追人跳墙的时候不小心摔到腿了，现在还流血不止，急需止血草止血。",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s收集%s",
		prize = {
		},
		given = {
		},
	},
	["420017"] = {
		class = "collect",
		type = "daily",
		name = "食材",
		aim = "酒馆现在急需食材，去搞点肉兽的肉回来！",
		needLv = 20,
		needCopyId = 697016,
		content = "酒馆现在急需食材，去搞点肉兽的肉回来！",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s获取%s",
		prize = {
		},
		given = {
		},
	},
	["420018"] = {
		class = "collect",
		type = "daily",
		name = "打造材料",
		aim = "铁匠铺现在紧缺一批打造武器的铁矿，去采些回来！",
		needLv = 20,
		needCopyId = 697016,
		content = "铁匠铺现在紧缺一批打造武器的铁矿，去采些回来！",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s采集%s",
		prize = {
		},
		given = {
		},
	},
	["420019"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "万窟洞里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600008,
		content = "万窟洞里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220081, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420020"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "凡心谷外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600010,
		content = "凡心谷外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220007, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420021"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "凡心谷有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600011,
		content = "凡心谷有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220008, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420022"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "凡心谷内部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600012,
		content = "凡心谷内部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230005, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420023"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "紫灵谷里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600016,
		content = "紫灵谷里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220006, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420024"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "紫灵谷深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600018,
		content = "紫灵谷深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230004, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420025"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "酒馆二层有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600021,
		content = "酒馆二层有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230007, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420026"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "寒土沼泽边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600022,
		content = "寒土沼泽边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220011, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420027"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "寒土沼泽深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600024,
		content = "寒土沼泽深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230008, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420028"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "衍沙戈壁有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600025,
		content = "衍沙戈壁有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220013, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420029"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "衍沙戈壁深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600027,
		content = "衍沙戈壁深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230009, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420030"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "倒影洞里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600028,
		content = "倒影洞里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220015, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420031"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "倒影洞底层有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600030,
		content = "倒影洞底层有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230010, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420032"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "风沙境边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600031,
		content = "风沙境边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220017, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420033"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "风沙境有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600033,
		content = "风沙境有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230011, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420034"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "回殇洞里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600034,
		content = "回殇洞里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220019, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420035"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "回殇洞底有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600036,
		content = "回殇洞底有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230012, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420036"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "裂缝外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600037,
		content = "裂缝外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220020, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420037"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "裂缝口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600039,
		content = "裂缝口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230013, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420038"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "秋心湖畔有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600040,
		content = "秋心湖畔有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220021, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420039"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "秋心湖有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600042,
		content = "秋心湖有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230014, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420040"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "连心洞口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600043,
		content = "连心洞口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220022, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420041"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "连心洞里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600045,
		content = "连心洞里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230015, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420042"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "千幻湖畔有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600046,
		content = "千幻湖畔有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220023, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420043"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "千幻湖畔深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600048,
		content = "千幻湖畔深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230016, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420044"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "北野森林外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600049,
		content = "北野森林外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220025, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420045"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "北野森林里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600051,
		content = "北野森林里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230017, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420046"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "常山山脚有强盗作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600052,
		content = "常山山脚有强盗作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220026, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420047"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "常山山顶有强盗作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600054,
		content = "常山山顶有强盗作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230018, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420048"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "无为境有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600055,
		content = "无为境有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220027, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420049"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "无为境内部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600057,
		content = "无为境内部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230019, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420050"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "悟道谷口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600058,
		content = "悟道谷口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220028, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420051"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "悟道谷里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600060,
		content = "悟道谷里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230020, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420052"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "七阳关关口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600061,
		content = "七阳关关口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220029, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420053"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "七阳关有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600063,
		content = "七阳关有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230021, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420054"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "城东小村外妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600064,
		content = "城东小村外妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220030, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420055"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "城东小村有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600066,
		content = "城东小村有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230022, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420056"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "附阴山山脚有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600067,
		content = "附阴山山脚有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220031, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420057"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "附阴山有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600069,
		content = "附阴山有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230023, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420058"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "婆沙森林边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600070,
		content = "婆沙森林边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220032, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420059"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "婆沙森林深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600072,
		content = "婆沙森林深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230024, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420060"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "小周境外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600073,
		content = "小周境外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220034, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420061"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "小周境内部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600075,
		content = "小周境内部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230025, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420062"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "幻象空间有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600076,
		content = "幻象空间有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220036, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420063"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "幻象空间二层有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600078,
		content = "幻象空间二层有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230026, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420064"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "困龙谷里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600079,
		content = "困龙谷里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220038, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420065"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "困龙谷内部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600081,
		content = "困龙谷内部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230027, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420066"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "千阳谷外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600082,
		content = "千阳谷外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220040, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420067"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "千阳谷里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600084,
		content = "千阳谷里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230028, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420068"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "往生湖畔有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600085,
		content = "往生湖畔有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220042, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420069"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "往生湖里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600087,
		content = "往生湖里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230029, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420070"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "落虎涧外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600088,
		content = "落虎涧外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220043, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420071"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "落虎涧深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600090,
		content = "落虎涧深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230030, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420072"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "钟灵山山脚有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600091,
		content = "钟灵山山脚有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220044, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420073"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "钟灵山里面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600093,
		content = "钟灵山里面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230031, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420074"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "荒羽部落外有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600094,
		content = "荒羽部落外有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220045, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420075"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "荒羽部落有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600096,
		content = "荒羽部落有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230032, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420076"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "涌泉窟洞口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600097,
		content = "涌泉窟洞口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220046, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420077"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "涌泉窟内有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600099,
		content = "涌泉窟内有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230033, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420078"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "涌泉窟二层有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600100,
		content = "涌泉窟二层有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220047, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420079"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "涌泉窟深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600102,
		content = "涌泉窟深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230034, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420080"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "熊弋城边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600103,
		content = "熊弋城边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220050, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420081"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "熊弋城外有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600105,
		content = "熊弋城外有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230035, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420082"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "莫沙城边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600106,
		content = "莫沙城边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220052, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420083"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "莫沙城外有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600108,
		content = "莫沙城外有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230036, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420084"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "图州沙漠有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600109,
		content = "图州沙漠有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220054, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420085"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "图州沙漠深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600111,
		content = "图州沙漠深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230037, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420086"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "废墟之城边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600112,
		content = "废墟之城边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220056, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420087"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "废墟之城面有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600114,
		content = "废墟之城面有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230038, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420088"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻碧峰山山脚有人捣乱，前往教训！",
		needLv = 20,
		needCopyId = 600130,
		content = "传闻碧峰山山脚有人捣乱，前往教训！",
		target = {
			{type = "fight", id = 220061, num = 10},
		},
		shortDesc = "去%s教训%s",
		prize = {
		},
		given = {
		},
	},
	["420089"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻碧峰山腰有人捣乱，前往教训！",
		needLv = 20,
		needCopyId = 600132,
		content = "传闻碧峰山腰有人捣乱，前往教训！",
		target = {
			{type = "fight", id = 230044, num = 10},
		},
		shortDesc = "去%s教训%s",
		prize = {
		},
		given = {
		},
	},
	["420090"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻烈阳洞的人很嚣张，前往教训！",
		needLv = 20,
		needCopyId = 600136,
		content = "传闻烈阳洞的人很嚣张，前往教训！",
		target = {
			{type = "fight", id = 220063, num = 10},
		},
		shortDesc = "去%s教训%s",
		prize = {
		},
		given = {
		},
	},
	["420091"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "战场遗迹边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600139,
		content = "战场遗迹边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220064, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420092"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "战场遗迹里部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600141,
		content = "战场遗迹里部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230047, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420093"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻地穴有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600142,
		content = "传闻地穴有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220065, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420094"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻地穴底部有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600144,
		content = "传闻地穴底部有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230048, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420095"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻铁石林边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600145,
		content = "传闻铁石林边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220066, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420096"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻铁石林有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600147,
		content = "传闻铁石林有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230049, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420097"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻黑水沼泽外有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600148,
		content = "传闻黑水沼泽外有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220067, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420098"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻黑水沼泽有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600150,
		content = "传闻黑水沼泽有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230050, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420099"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻灵犀湖有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600153,
		content = "传闻灵犀湖有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230051, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420100"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻四方寨外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600154,
		content = "传闻四方寨外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220068, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420101"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻四方寨有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600156,
		content = "传闻四方寨有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230052, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420102"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻紫兰洞口有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600157,
		content = "传闻紫兰洞口有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220069, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420103"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻紫兰洞有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600159,
		content = "传闻紫兰洞有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230053, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420104"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻苦行村口的野蛮人蛮不讲理，你去开导下他们！",
		needLv = 20,
		needCopyId = 600160,
		content = "传闻苦行村口的野蛮人蛮不讲理，你去开导下他们！",
		target = {
			{type = "fight", id = 220070, num = 10},
		},
		shortDesc = "去%s教训%s",
		prize = {
		},
		given = {
		},
	},
	["420105"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻苦行村口的野蛮人蛮不讲理，你去开导下他们！",
		needLv = 20,
		needCopyId = 600162,
		content = "传闻苦行村口的野蛮人蛮不讲理，你去开导下他们！",
		target = {
			{type = "fight", id = 230054, num = 10},
		},
		shortDesc = "去%s教训%s",
		prize = {
		},
		given = {
		},
	},
	["420106"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻三生湖边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600163,
		content = "传闻三生湖边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220071, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420107"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻三生湖有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600165,
		content = "传闻三生湖有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230055, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420108"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻麋鹿山脚有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600166,
		content = "传闻麋鹿山脚有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220072, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420109"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻麋鹿山有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600168,
		content = "传闻麋鹿山有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230056, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420110"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻玄阴洞有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600171,
		content = "传闻玄阴洞有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220073, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420111"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻玄阴洞二层有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600174,
		content = "传闻玄阴洞二层有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220074, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420112"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻玄阴洞深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600177,
		content = "传闻玄阴洞深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220075, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420113"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻雾霭森林边缘有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600178,
		content = "传闻雾霭森林边缘有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220076, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420114"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻雾霭森林有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600180,
		content = "传闻雾霭森林有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230060, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420115"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻落幕坡外围有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600183,
		content = "传闻落幕坡外围有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220077, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420116"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻落幕坡有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600186,
		content = "传闻落幕坡有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220078, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420117"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻落幕坡深处有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600189,
		content = "传闻落幕坡深处有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 220079, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420118"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻太阴洞有妖魔作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600192,
		content = "传闻太阴洞有妖魔作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230064, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
	["420119"] = {
		class = "fight",
		type = "daily",
		name = "斩妖除魔",
		aim = "传闻太阴洞底层有妖人作乱，前往剿灭！",
		needLv = 20,
		needCopyId = 600195,
		content = "传闻太阴洞底层有妖人作乱，前往剿灭！",
		target = {
			{type = "fight", id = 230065, num = 10},
		},
		shortDesc = "去%s剿灭%s",
		prize = {
		},
		given = {
		},
	},
}

function GetTable()
	return autoTable
end
