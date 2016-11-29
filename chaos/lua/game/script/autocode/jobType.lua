--职业列表
local autoTable = {
	["archer"] = {
		name = "羽灵",
		type = "physical",
		basic = "archer",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 2},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["warrior2"]= 1,
			["archer3"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809021,},
			[0] = {809011,809016,},
		},
	},
	["archer2"] = {
		name = "乱羽",
		type = "physical",
		basic = "archer",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 2},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["warrior"]= 1,
			["archer"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809021,},
			[0] = {809011,809016,},
		},
	},
	["archer3"] = {
		name = "破羽",
		type = "physical",
		basic = "archer",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 2},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["warrior3"]= 1,
			["archer2"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809021,},
			[0] = {809011,809016,},
		},
	},
	["warrior"] = {
		name = "极武",
		type = "physical",
		basic = "warrior",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 1},
			{type = "captain", num = 2},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["archer2"]= 1,
			["warrior2"]= 1,
		},
		range = 0,
		modelList = {
			[1] = {809001,809003,809006,809008,},
			[0] = {809012,809014,809017,809019,},
		},
	},
	["warrior2"] = {
		name = "骑士",
		type = "physical",
		basic = "warrior",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 1},
			{type = "captain", num = 2},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["archer3"]= 1,
			["warrior3"]= 1,
		},
		range = 0,
		modelList = {
			[1] = {809004,809009,},
			[0] = {809013,809018,},
		},
	},
	["warrior3"] = {
		name = "守卫者",
		type = "physical",
		basic = "warrior",
		recommend = {
			{type = "strength", num = 2},
			{type = "agility", num = 1},
			{type = "captain", num = 2},
		},
		restraint = {
			["taoist"]= 1,
			["taoist2"]= 1,
			["taoist3"]= 1,
			["archer"]= 1,
			["warrior"]= 1,
		},
		range = 0,
		modelList = {
			[1] = {809001,809003,809006,809008,},
			[0] = {809012,809014,809017,809019,},
		},
	},
	["taoist"] = {
		name = "御魂",
		type = "magic",
		basic = "taoist",
		recommend = {
			{type = "intellect", num = 3},
			{type = "agility", num = 1},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist3"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809002,809005,809007,809010,},
			[0] = {809015,809020,},
		},
	},
	["taoist2"] = {
		name = "混巫",
		type = "magic",
		basic = "taoist",
		recommend = {
			{type = "intellect", num = 3},
			{type = "agility", num = 1},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809002,809005,809007,809010,},
			[0] = {809015,809020,},
		},
	},
	["taoist3"] = {
		name = "医师",
		type = "magic",
		basic = "taoist",
		recommend = {
			{type = "intellect", num = 3},
			{type = "agility", num = 1},
			{type = "captain", num = 1},
		},
		restraint = {
			["taoist2"]= 1,
		},
		range = 1,
		modelList = {
			[1] = {809002,809005,809007,809010,},
			[0] = {809015,809020,},
		},
	},
}

function GetTable()
	return autoTable
end
