--空场景列表
 local autoTable = {
	["697016"] = {
		name = "副本扫荡界面",
		type = "empty",
		mapId = "697001",
		kind = "copyMopup",
		size = {x = 2500, y = 650},
		born = {x = 75, y = 510},
		describe = "副本扫荡界面",
		npcList = {
		},
	},
	["697017"] = {
		name = "修仙塔扫荡界面",
		type = "empty",
		mapId = "697001",
		kind = "retreatMopup",
		size = {x = 2500, y = 650},
		born = {x = 75, y = 510},
		describe = "修仙塔扫荡界面",
		npcList = {
		},
	},
	["697018"] = {
		name = "部落",
		type = "empty",
		mapId = "697001",
		kind = "tribe",
		size = {x = 2500, y = 650},
		born = {x = 75, y = 510},
		describe = "部落",
		npcList = {
		},
	},
	["697019"] = {
		name = "组队副本",
		type = "empty",
		mapId = "697001",
		kind = "team",
		size = {x = 2500, y = 650},
		born = {x = 75, y = 510},
		describe = "组队副本",
		npcList = {
		},
	},
}
function GetTable()
	return autoTable
end
