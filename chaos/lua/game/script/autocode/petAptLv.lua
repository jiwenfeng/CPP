--本命精灵品级关系表
 local autoTable = {
	[1]= {
		needLv = 0,
		talentNum = 1,
		guard = 0.05,
		extra = 0.02,
		mission = 0.12,
		discount = 0.02,
	},
	[2]= {
		needLv = 20,
		talentNum = 2,
		guard = 0.1,
		extra = 0.02,
		mission = 0.14,
		discount = 0.02,
	},
	[3]= {
		needLv = 30,
		talentNum = 3,
		guard = 0.15,
		extra = 0.02,
		mission = 0.16,
		discount = 0.02,
	},
	[4]= {
		needLv = 40,
		talentNum = 4,
		guard = 0.2,
		extra = 0.02,
		mission = 0.18,
		discount = 0.02,
	},
	[5]= {
		needLv = 50,
		talentNum = 5,
		guard = 0.25,
		extra = 0.02,
		mission = 0.2,
		discount = 0.02,
	},
}
function GetTable()
	return autoTable
end
