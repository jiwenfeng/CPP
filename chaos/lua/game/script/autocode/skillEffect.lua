--技能效果列表
local autoTable = {
	["1"] = {
		name = "sp_up",
		IsBuffer = 1,
		state = "sp_up",
		IsPassive = 1,
		attr = "speed",
	},
	["2"] = {
		name = "sp_down",
		IsBuffer = 1,
		state = "sp_down",
	},
	["3"] = {
		name = "phy_atk_up",
		IsBuffer = 1,
		state = "phy_atk_up",
		IsPassive = 1,
		attr = "physcAtk",
	},
	["4"] = {
		name = "phy_atk_down",
		IsBuffer = 1,
		state = "phy_atk_down",
	},
	["5"] = {
		name = "phy_def_up",
		IsBuffer = 1,
		state = "phy_def_up",
		IsPassive = 1,
		attr = "physcDef",
	},
	["6"] = {
		name = "phy_def_down",
		IsBuffer = 1,
		state = "phy_def_down",
	},
	["7"] = {
		name = "phy_atk_per_up",
		IsBuffer = 1,
		state = "phy_atk_per_up",
	},
	["8"] = {
		name = "phy_atk_per_down",
		IsBuffer = 1,
		state = "phy_atk_per_down",
	},
	["9"] = {
		name = "phy_def_per_up",
		IsBuffer = 1,
		state = "phy_def_per_up",
	},
	["10"] = {
		name = "phy_def_per_down",
		IsBuffer = 1,
		state = "phy_def_per_down",
	},
	["11"] = {
		name = "mgc_atk_up",
		IsBuffer = 1,
		state = "mgc_atk_up",
		IsPassive = 1,
		attr = "magicAtk",
	},
	["12"] = {
		name = "mgc_atk_down",
		IsBuffer = 1,
		state = "mgc_atk_down",
	},
	["13"] = {
		name = "mgc_def_up",
		IsBuffer = 1,
		state = "mgc_def_up",
		IsPassive = 1,
		attr = "magicDef",
	},
	["14"] = {
		name = "mgc_def_down",
		IsBuffer = 1,
		state = "mgc_def_down",
	},
	["15"] = {
		name = "mgc_atk_per_up",
		IsBuffer = 1,
		state = "mgc_atk_per_up",
	},
	["16"] = {
		name = "mgc_atk_per_down",
		IsBuffer = 1,
		state = "mgc_atk_per_down",
	},
	["17"] = {
		name = "mgc_def_per_up",
		IsBuffer = 1,
		state = "mgc_def_per_up",
	},
	["18"] = {
		name = "mgc_def_per_down",
		IsBuffer = 1,
		state = "mgc_def_per_down",
	},
	["19"] = {
		IsActive = 1,
	},
	["20"] = {
		IsActive = 1,
	},
	["21"] = {
		IsPassive = 1,
		attr = "hpMax",
	},
	["22"] = {
		name = "fatal_up",
		IsBuffer = 1,
		state = "fatal_up",
	},
	["23"] = {
		name = "counter",
		IsBuffer = 1,
		state = "counter",
		IsPassive = 1,
		attr = "counter",
	},
	["24"] = {
		IsPassive = 1,
		attr = "block",
	},
	["25"] = {
		IsPassive = 1,
		attr = "batter",
	},
	["26"] = {
		name = "spun",
		IsBuffer = 1,
		state = "spun",
	},
	["27"] = {
		name = "frozen",
		IsBuffer = 1,
		state = "frozen",
	},
	["28"] = {
		name = "scared",
		IsBuffer = 1,
		state = "scared",
	},
	["29"] = {
		name = "drains",
		IsBuffer = 1,
		state = "drains",
	},
	["30"] = {
		name = "bleed",
		IsBuffer = 1,
		state = "bleed",
	},
	["31"] = {
		IsActive = 1,
	},
	["32"] = {
		name = "hurt_up",
		IsBuffer = 1,
		state = "hurt_up",
		IsActive = 1,
	},
	["33"] = {
		IsPassive = 1,
		attr = "hurtDown",
	},
	["34"] = {
		name = "hurt_per_up",
		IsBuffer = 1,
		state = "hurt_per_up",
		IsPassive = 1,
		attr = "harmDeep",
	},
	["35"] = {
		name = "hurt_per_down",
		IsBuffer = 1,
		state = "hurt_per_down",
	},
	["36"] = {
		name = "hurt_back",
		IsBuffer = 1,
		state = "hurt_back",
	},
	["37"] = {
		name = "hurt_rebound",
		IsBuffer = 1,
		state = "hurt_rebound",
	},
	["38"] = {
		name = "shield",
		IsBuffer = 1,
		state = "shield",
	},
	["39"] = {
		name = "atk_down",
		IsBuffer = 1,
		state = "atk_down",
	},
	["40"] = {
	},
	["41"] = {
		IsPassive = 1,
		attr = "ignoreMgcDef",
	},
	["42"] = {
		IsBuffer = 1,
		state = "hp_restore",
	},
	["43"] = {
	},
	["44"] = {
		name = "poison",
		IsBuffer = 1,
		state = "poison",
	},
	["45"] = {
		name = "sneer",
		IsBuffer = 1,
		state = "sneer",
	},
}

function GetTable()
	return autoTable
end
