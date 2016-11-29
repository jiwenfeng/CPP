--怪物图鉴材料表
 local autoTable = {
	[1] = {
		row = {
			{type = "speed", num = 9},
			{type = "physcDef", num = 30},
			{type = "magicDef", num = 90},
		},
		col = {
			{type = "physcAtk", num = 90},
			{type = "magicAtk", num = 90},
			{type = "hpMax", num = 120},
		},
	},
	[2] = {
		row = {
			{type = "speed", num = 18},
			{type = "physcDef", num = 60},
			{type = "magicDef", num = 180},
		},
		col = {
			{type = "physcAtk", num = 180},
			{type = "magicAtk", num = 180},
			{type = "hpMax", num = 240},
		},
	},
	[3] = {
		row = {
			{type = "speed", num = 27},
			{type = "physcDef", num = 90},
			{type = "magicDef", num = 270},
		},
		col = {
			{type = "physcAtk", num = 270},
			{type = "magicAtk", num = 270},
			{type = "hpMax", num = 360},
		},
	},
}

function GetTable()
	return autoTable
end
