--主角模型表
 local autoTable = {
	['archer'] = {
		[1] = {
			modelId = 808005,
			headId = 818005,
		},
		[0] = {
			modelId = 808006,
			headId = 818006,
		},
	},
	['taoist'] = {
		[1] = {
			modelId = 808003,
			headId = 818003,
		},
		[0] = {
			modelId = 808004,
			headId = 818004,
		},
	},
	['warrior'] = {
		[1] = {
			modelId = 808001,
			headId = 818001,
		},
		[0] = {
			modelId = 808002,
			headId = 818002,
		},
	},
}
function GetTable()
	return autoTable
end
