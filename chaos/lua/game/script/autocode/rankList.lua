--排行榜列表
local autoTable = {
	["userLv"] = {
		index = 1,
		name = "个人等级",
		upLimit = {real = 500, show = 50},
	},
	["userForce"] = {
		index = 2,
		name = "个人战力",
		upLimit = {real = 500, show = 50},
	},
	["unionLv"] = {
		index = 3,
		name = "联盟等级",
		upLimit = {real = 500, show = 50},
	},
	["unionForce"] = {
		index = 4,
		name = "联盟战力",
		upLimit = {real = 500, show = 50},
	},
	["charm"] = {
		index = 5,
		name = "个人魅力",
		upLimit = {real = 500, show = 50},
	},
	["hero"] = {
		index = 6,
		name = "武将战力",
		upLimit = {real = 500, show = 50},
	},
	["equip"] = {
		index = 7,
		name = "神兵榜",
		upLimit = {real = 100, show = 50},
	},
}

function GetTable()
	return autoTable
end