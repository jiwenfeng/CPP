--藏宝点表
 local autoTable = {
	["101041"] = {
		name = "藏宝峡谷",
		spadeList = {
			[101035] = 1,
			[101036] = 1,
			[101037] = 1,
		},
		mapId = 101041,
		picId = 101041,
		rateList = {
			[101035] = {
				[1] = {
					min = 0,
					max = 11,
					prizeList = {
						{type = 1, rate = 0.7999},
						{type = 2, rate = 0.2},
						{type = 8, rate = 0.0001},
					},
				},
				[2] = {
					min = 11,
					max = 31,
					prizeList = {
						{type = 1, rate = 0.6668},
						{type = 2, rate = 0.3},
						{type = 3, rate = 0.02},
						{type = 4, rate = 0.01},
						{type = 5, rate = 0.003},
						{type = 8, rate = 0.0002},
					},
				},
				[3] = {
					min = 31,
					max = math.huge,
					prizeList = {
						{type = 1, rate = 0.6087},
						{type = 2, rate = 0.35},
						{type = 3, rate = 0.02},
						{type = 4, rate = 0.015},
						{type = 5, rate = 0.005},
						{type = 6, rate = 0.001},
						{type = 8, rate = 0.0003},
					},
				},
			},
			[101036] = {
				[1] = {
					min = 0,
					max = 11,
					prizeList = {
						{type = 1, rate = 0.4},
						{type = 2, rate = 0.5},
						{type = 3, rate = 0.06},
						{type = 4, rate = 0.04},
					},
				},
				[2] = {
					min = 11,
					max = 31,
					prizeList = {
						{type = 1, rate = 0.3},
						{type = 2, rate = 0.5},
						{type = 3, rate = 0.105},
						{type = 4, rate = 0.08},
						{type = 5, rate = 0.01},
						{type = 6, rate = 0.0048},
						{type = 8, rate = 0.0002},
					},
				},
				[3] = {
					min = 31,
					max = 51,
					prizeList = {
						{type = 1, rate = 0.25},
						{type = 2, rate = 0.4},
						{type = 3, rate = 0.2185},
						{type = 4, rate = 0.1},
						{type = 5, rate = 0.02},
						{type = 6, rate = 0.01},
						{type = 7, rate = 0.001},
						{type = 8, rate = 0.0005},
					},
				},
				[4] = {
					min = 51,
					max = math.huge,
					prizeList = {
						{type = 1, rate = 0.2},
						{type = 2, rate = 0.33},
						{type = 3, rate = 0.28},
						{type = 4, rate = 0.12},
						{type = 5, rate = 0.05},
						{type = 6, rate = 0.014},
						{type = 7, rate = 0.005},
						{type = 8, rate = 0.0007},
						{type = 9, rate = 0.0002},
						{type = 10, rate = 0.0001},
					},
				},
			},
			[101037] = {
				[1] = {
					min = 0,
					max = 11,
					prizeList = {
						{type = 1, rate = 0.09},
						{type = 2, rate = 0.11},
						{type = 3, rate = 0.24},
						{type = 4, rate = 0.32},
						{type = 5, rate = 0.127},
						{type = 6, rate = 0.1},
						{type = 7, rate = 0.008},
						{type = 8, rate = 0.005},
					},
				},
				[2] = {
					min = 11,
					max = 31,
					prizeList = {
						{type = 1, rate = 0.07},
						{type = 2, rate = 0.08},
						{type = 3, rate = 0.1},
						{type = 4, rate = 0.35},
						{type = 5, rate = 0.25},
						{type = 6, rate = 0.132},
						{type = 7, rate = 0.01},
						{type = 8, rate = 0.0077},
						{type = 9, rate = 0.0002},
						{type = 10, rate = 0.0001},
					},
				},
				[3] = {
					min = 31,
					max = 51,
					prizeList = {
						{type = 1, rate = 0.07},
						{type = 2, rate = 0.08},
						{type = 3, rate = 0.1},
						{type = 4, rate = 0.27},
						{type = 5, rate = 0.24},
						{type = 6, rate = 0.2},
						{type = 7, rate = 0.0295},
						{type = 8, rate = 0.01},
						{type = 9, rate = 0.0003},
						{type = 10, rate = 0.0002},
					},
				},
				[4] = {
					min = 51,
					max = math.huge,
					prizeList = {
						{type = 1, rate = 0.07},
						{type = 2, rate = 0.08},
						{type = 3, rate = 0.1},
						{type = 4, rate = 0.26},
						{type = 5, rate = 0.23},
						{type = 6, rate = 0.2},
						{type = 7, rate = 0.04384},
						{type = 8, rate = 0.015},
						{type = 9, rate = 0.0006},
						{type = 10, rate = 0.0005},
						{type = 11, rate = 3e-05},
						{type = 12, rate = 2e-05},
						{type = 13, rate = 1e-05},
					},
				},
			},
		},
	},
}

function GetTable()
	return autoTable
end
