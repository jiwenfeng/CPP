--世界boss场景
 local autoTable = {
	["697103"] = {
		name = "炼狱鳞兽",
		type = "worldBoss",
		mapId = "619122",
		size = {x = 2500, y = 650},
		born = {x = 200, y = 510},
		describe = "这是一个简单的测试场景。",
		npcList = {
			{npcId = "260467", x = 1250, y = 480, isBoss = 1},
		},
		sealBarX = 850,
		sealBarY = 350,
		validY = 420,
	},
	["697104"] = {
		name = "婆沙蜘蛛",
		type = "worldBoss",
		mapId = "619124",
		size = {x = 2500, y = 650},
		born = {x = 200, y = 510},
		describe = "这是一个简单的测试场景。",
		npcList = {
			{npcId = "260468", x = 1250, y = 480, isBoss = 1},
		},
		sealBarX = 850,
		sealBarY = 350,
		validY = 410,
	},
	["697105"] = {
		name = "黑森炎黄",
		type = "worldBoss",
		mapId = "619125",
		size = {x = 2500, y = 650},
		born = {x = 200, y = 510},
		describe = "这是一个简单的测试场景。",
		npcList = {
			{npcId = "260469", x = 1250, y = 480, isBoss = 1},
		},
		sealBarX = 850,
		sealBarY = 350,
		validY = 405,
	},
}
function GetTable()
	return autoTable
end
