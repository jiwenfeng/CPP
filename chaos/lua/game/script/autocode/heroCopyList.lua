--英魂副本列表
 local autoTable = {
	["604001"]={
		name = "赤壁",
		type = "hero",
		mapId = "604001",
		needLv = 12,
		needMissionId = "423001",
		describe = "异时空的裂缝通往的地方，人传为赤壁，具体情况无人知晓。",
		monsterList = {
			[1] = {
				teamId = "260460",
				posX = 750,
				posY = 480,
				gold = 57,
				exp = 254,
				itemList = {
				},
			},
			[2] = {
				teamId = "260461",
				posX = 1500,
				posY = 480,
				gold = 57,
				exp = 254,
				itemList = {
				},
			},
			[3] = {
				teamId = "260464",
				posX = 2250,
				posY = 480,
				gold = 57,
				exp = 254,
				itemList = {
				},
			},
		},
		gold = 400,
		exp = 6858,
		itemList = {
			{type = "equip", Lv = 20, quality = 1, num = 1, rate = 0.212},
			{type = "equip", Lv = 20, quality = 3, num = 1, rate = 0.163},
			{id = 104002, num = 0, rate = 0.15},
			{id = 102012, num = 1, rate = 0.175},
			{id = 102003, num = 1, rate = 0.15},
			{id = 102004, num = 1, rate = 0.15},
		},
	},
	["604002"]={
		name = "华山",
		type = "hero",
		mapId = "604002",
		needLv = 12,
		needMissionId = "423002",
		describe = "异时空的裂缝通往的地方，那里的人都说那是华山。",
		monsterList = {
			[1] = {
				teamId = "260462",
				posX = 750,
				posY = 480,
				gold = 429,
				exp = 1000,
				itemList = {
				},
			},
			[2] = {
				teamId = "260463",
				posX = 1500,
				posY = 480,
				gold = 429,
				exp = 1000,
				itemList = {
				},
			},
			[3] = {
				teamId = "260465",
				posX = 2250,
				posY = 480,
				gold = 429,
				exp = 1000,
				itemList = {
				},
			},
		},
		gold = 2999,
		exp = 25101,
		itemList = {
			{type = "equip", Lv = 30, quality = 1, num = 1, rate = 0.212},
			{type = "equip", Lv = 30, quality = 3, num = 1, rate = 0.163},
			{id = 104002, num = 0, rate = 0.15},
			{id = 102012, num = 1, rate = 0.175},
			{id = 102003, num = 1, rate = 0.15},
			{id = 102004, num = 1, rate = 0.15},
		},
	},
}

function GetTable()
	return autoTable
end
