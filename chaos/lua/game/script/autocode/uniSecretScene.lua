--联盟密境
 local autoTable = {
	["697101"] = {
		name = "联盟秘境",
		type = "uniSecret",
		mapId = "619123",
		size = {x = 2500, y = 650},
		born = {x = 200, y = 515},
		describe = "这是一个简单的测试场景。",
		npcList = {
			{npcId = "210101", x = 1000, y = 475, isBoss = 1},
		},
		validY = 420,
	},
}
function GetTable()
	return autoTable
end
