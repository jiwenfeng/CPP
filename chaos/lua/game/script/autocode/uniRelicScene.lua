--联盟遗迹
 local autoTable = {
	["697102"] = {
		name = "饕餮遗迹",
		type = "uniRelic",
		mapId = "619122",
		size = {x = 2500, y = 650},
		born = {x = 200, y = 510},
		describe = "这是一个简单的测试场景。",
		npcList = {
			{npcId = "260466", x = 1250, y = 480, isBoss = 1},
		},
		sealBarX = 850,
		sealBarY = 350,
		validY = 410,
	},
}
function GetTable()
	return autoTable
end
