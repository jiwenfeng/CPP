--本命精灵表
 local autoTable = {
	[200001] = {
		name = "腾蛇",
		skillId = "311001",
		skillName = "灵蛇缚吼",
		describe = "神话中由女娲娘娘以自己形象制造的宠物，是一种会架雾的蛇，是一种仙兽。",
		modelList = {
			[1] = {headId = 824101, modelId = 824001},
			[2] = {headId = 824102, modelId = 824002},
		},
	},
	[200004] = {
		name = "麒麟",
		skillId = "311004",
		skillName = "麒麟幻爪",
		describe = "传说中一种神兽，浑身充满火焰，极度凶残，天生大力，可御空飞行",
		modelList = {
			[1] = {headId = 824103, modelId = 824003},
			[2] = {headId = 824104, modelId = 824004},
		},
	},
	[200005] = {
		name = "朱雀",
		skillId = "311005",
		skillName = "红莲天火",
		describe = "古代四圣兽之一，祥瑞的象征",
		modelList = {
			[1] = {headId = 824105, modelId = 824005},
			[2] = {headId = 824106, modelId = 824006},
		},
	},
	[200006] = {
		name = "九尾狐",
		skillId = "311006",
		skillName = "聚能炎弹",
		describe = "九尾的仙狐，出世时天降异象，被认为是祥瑞的象征",
		modelList = {
			[1] = {headId = 824107, modelId = 824007},
			[2] = {headId = 824108, modelId = 824008},
		},
	},
	[200011] = {
		name = "本命精灵",
		describe = "未知神兽的蛋，吸收天地之灵气、聚日月之精华等待破壳而生",
		modelList = {
			[0] = {headId = 824111, modelId = 824011},
		},
	},
	[200012] = {
		name = "青鸾",
		skillId = "311011",
		skillName = "琥珀之羽",
		describe = "传说中羽翼青如晓天，散发着柔和光芒的神鸟",
		modelList = {
			[1] = {headId = 824109, modelId = 824009},
			[2] = {headId = 824110, modelId = 824010},
		},
	},
}
function GetTable()
	return autoTable
end
