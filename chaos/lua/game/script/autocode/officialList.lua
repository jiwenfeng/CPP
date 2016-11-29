local autoTable = {
	["101"] = {
		name = "御史",
		support = 1000,
		against = 3000,
		electNeed = {},
		attr = {
			goldRate = 0.05,
		},
		level = 3,
		impeachNeed = {["103"] = 1, ["104"] = 1},
	},
	["102"] = {
		name = "都尉",
		support = 1000,
		against = 3000,
		electNeed = {},
		attr = {
			expRate = 0.05,
		},
		level = 3,
		impeachNeed = {["103"] = 1, ["104"] = 1},
	},
	["103"] = {
		name = "太师",
		support = 1500,
		against = 4500,
		electNeed = {["101"] = 1},
		attr = {
			goldRate = 0.1,
		},
		level = 4,
		impeachNeed = {["101"] = 1, ["102"] = 1, ["105"] = 1},
	},
	["104"] = {
		name = "太尉",
		support = 1500,
		against = 4500,
		electNeed = {["102"] = 1},
		attr = {
			expRate = 0.1,
		},
		level = 4,
		impeachNeed = {["101"] = 1, ["102"] = 1, ["105"] = 1},
	},
	["105"] = {
		name = "帝王",
		support = 3000,
		against = 6000,
		electNeed = {["103"] = 1, ["104"] = 1},
		attr = {
			expRate = 0.15,
			goldRate = 0.15,
			ban = 10,
		},
		level = 5,
		impeachNeed = {["103"] = 1, ["104"] = 1},
	},
}
function GetTable()
	return autoTable
end
