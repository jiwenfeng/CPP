--兵种列表
local autoTable = {
	["archer"] = {
		[1] = {minLv = 0, maxLv = 29, name = "弓兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "弩兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "神射手", add = 0.1},
	},
	["archer2"] = {
		[1] = {minLv = 0, maxLv = 29, name = "弓兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "弩兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "神射手", add = 0.1},
	},
	["archer3"] = {
		[1] = {minLv = 0, maxLv = 29, name = "弓兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "弩兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "神射手", add = 0.1},
	},
	["warrior"] = {
		[1] = {minLv = 0, maxLv = 29, name = "步兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "重步兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "禁卫军", add = 0.1},
	},
	["warrior2"] = {
		[1] = {minLv = 0, maxLv = 29, name = "骑兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "重骑兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "虎豹骑", add = 0.1},
	},
	["warrior3"] = {
		[1] = {minLv = 0, maxLv = 29, name = "步兵", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "重步兵", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "禁卫军", add = 0.1},
	},
	["taoist"] = {
		[1] = {minLv = 0, maxLv = 29, name = "学徒", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "术士", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "贤者", add = 0.1},
	},
	["taoist2"] = {
		[1] = {minLv = 0, maxLv = 29, name = "学徒", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "术士", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "贤者", add = 0.1},
	},
	["taoist3"] = {
		[1] = {minLv = 0, maxLv = 29, name = "学徒", add = 0},
		[2] = {minLv = 30, maxLv = 59, name = "术士", add = 0.05},
		[3] = {minLv = 60, maxLv = 60, name = "贤者", add = 0.1},
	},
}

function GetTable()
	return autoTable
end
