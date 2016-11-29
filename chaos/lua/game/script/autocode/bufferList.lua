--buffer列表
 local autoTable = {
	["1"]={
		name = "统帅秘药",
		describe = "+50统帅值",
		label = 1,
		type = "attr",
		timelong = 3600,
		attr = {
			captain = 50,
		},
	},
	["2"]={
		name = "统帅秘药",
		describe = "+80统帅值",
		label = 1,
		type = "attr",
		timelong = 3600,
		attr = {
			captain = 80,
		},
	},
	["3"]={
		name = "统帅秘药",
		describe = "+120统帅值",
		label = 1,
		type = "attr",
		timelong = 3600,
		attr = {
			captain = 120,
		},
	},
	["4"]={
		name = "多倍经验",
		describe = "上阵将领杀怪获得1.2倍经验",
		label = 2,
		type = "exp",
		num = 1.2,
		timelong = 3600,
	},
	["5"]={
		name = "多倍经验",
		describe = "上阵将领杀怪获得1.5倍经验",
		label = 2,
		type = "exp",
		num = 1.5,
		timelong = 3600,
	},
	["6"]={
		name = "多倍经验",
		describe = "上阵将领杀怪获得2倍经验",
		label = 2,
		type = "exp",
		num = 2,
		timelong = 3600,
	},
	["7"]={
		name = "力量秘药",
		describe = "+50力量",
		label = 3,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 50,
		},
	},
	["8"]={
		name = "力量秘药",
		describe = "+80力量",
		label = 3,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 80,
		},
	},
	["9"]={
		name = "力量秘药",
		describe = "+120力量",
		label = 3,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 120,
		},
	},
	["10"]={
		name = "智力秘药",
		describe = "+50智力",
		label = 4,
		type = "attr",
		timelong = 3600,
		attr = {
			agility = 50,
		},
	},
	["11"]={
		name = "智力秘药",
		describe = "+80智力",
		label = 4,
		type = "attr",
		timelong = 3600,
		attr = {
			agility = 80,
		},
	},
	["12"]={
		name = "智力秘药",
		describe = "+120智力",
		label = 4,
		type = "attr",
		timelong = 3600,
		attr = {
			agility = 120,
		},
	},
	["13"]={
		name = "敏捷秘药",
		describe = "+50敏捷",
		label = 5,
		type = "attr",
		timelong = 3600,
		attr = {
			intellect = 50,
		},
	},
	["14"]={
		name = "敏捷秘药",
		describe = "+80敏捷",
		label = 5,
		type = "attr",
		timelong = 3600,
		attr = {
			intellect = 80,
		},
	},
	["15"]={
		name = "敏捷秘药",
		describe = "+120敏捷",
		label = 5,
		type = "attr",
		timelong = 3600,
		attr = {
			intellect = 120,
		},
	},
	["16"]={
		name = "英雄秘药",
		describe = "+30统帅、+30力量、+30智力、+30敏捷",
		label = 6,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 30,
			agility = 30,
			intellect = 30,
			captain = 30,
		},
	},
	["17"]={
		name = "英雄秘药",
		describe = "+60统帅、+60力量、+60智力、+60敏捷",
		label = 6,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 60,
			agility = 60,
			intellect = 60,
			captain = 60,
		},
	},
	["18"]={
		name = "快马加鞭",
		describe = "自动提取部落的后备兵补充兵力",
		label = 7,
		type = "solider",
		num = 8888,
	},
	["19"]={
		name = "佣兵令",
		describe = "自动召唤佣兵补充兵力+%d",
		label = 8,
		type = "solider",
		num = 88888,
		canAdd = 1,
	},
	["20"]={
		name = "势力战胜利BUFF",
		describe = "钱币产出+5%\n晶石产出+5%",
		label = 9,
		type = "resource",
		num = 0.05,
		timelong = 86400,
	},
	["21"]={
		name = "酒足饭饱",
		describe = "体力+%d",
		label = 10,
		type = "vigour",
		num = 50,
	},
	["22"]={
		name = "英雄秘药",
		describe = "+90统帅、+90力量、+90智力、+90敏捷",
		label = 6,
		type = "attr",
		timelong = 3600,
		attr = {
			strength = 90,
			agility = 90,
			intellect = 90,
			captain = 90,
		},
	},
}
function GetTable()
	return autoTable
end
