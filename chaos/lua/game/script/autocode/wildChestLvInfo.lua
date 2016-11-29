--野地矿产等级信息
 local autoTable = {
	[30] = {
		monsterList = {"260166", "260179", "260200", "260211", "260228"},
		prize = {
			{type = "buffer", Lv = 1, num = 1, rate = 0.29},
			{id = 101038, num = 1, rate = 0.3},
			{id = 102003, num = 1, rate = 0.2},
			{id = 102004, num = 1, rate = 0.2},
			{id = 102007, num = 1, rate = 0.009},
			{id = 102008, num = 1, rate = 0.001},
		},
	},
	[40] = {
		monsterList = {"260244", "260253", "260267", "260298", "260316"},
		prize = {
			{type = "buffer", Lv = 1, num = 1, rate = 0.29},
			{id = 101038, num = 1, rate = 0.3},
			{id = 102003, num = 1, rate = 0.2},
			{id = 102004, num = 1, rate = 0.2},
			{id = 102007, num = 1, rate = 0.009},
			{id = 102008, num = 1, rate = 0.001},
		},
	},
	[50] = {
		monsterList = {"260337", "260361", "260389", "260414", "260449"},
		prize = {
			{type = "buffer", Lv = 1, num = 1, rate = 0.19},
			{id = 101038, num = 1, rate = 0.3},
			{id = 102003, num = 1, rate = 0.2},
			{id = 102004, num = 1, rate = 0.3},
			{id = 102007, num = 1, rate = 0.009},
			{id = 102008, num = 1, rate = 0.001},
		},
	},
}

function GetTable()
	return autoTable
end
