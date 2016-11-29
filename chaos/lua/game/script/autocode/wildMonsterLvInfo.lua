--野地矿产等级信息
 local autoTable = {
	[30] = {
		monsterList = {"260166", "260179", "260200", "260211", "260228"},
		prize = {
			{gold = 3000, rate = 0.34},
			{nimbus = 300, rate = 0.33},
			{spar = 2000, rate = 0.33},
		},
	},
	[40] = {
		monsterList = {"260244", "260253", "260267", "260298", "260316"},
		prize = {
			{gold = 4000, rate = 0.34},
			{nimbus = 400, rate = 0.33},
			{spar = 3000, rate = 0.33},
		},
	},
	[50] = {
		monsterList = {"260337", "260361", "260389", "260414", "260449"},
		prize = {
			{gold = 5000, rate = 0.34},
			{nimbus = 500, rate = 0.33},
			{spar = 4000, rate = 0.33},
		},
	},
}

function GetTable()
	return autoTable
end
