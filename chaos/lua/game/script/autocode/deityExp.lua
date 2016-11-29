--玄冥经验表
local autoTable = {
	[1] = {
		needTimes = 2,
		expAdd = 0, 
	},
	[2] = {
		needTimes = 4,
		expAdd = 0.1, 
	},
	[3] = {
		needTimes = 8,
		expAdd = 0.2, 
	},
	[4] = {
		needTimes = 12,
		expAdd = 0.3, 
	},
	[5] = {
		needTimes = 20,
		expAdd = 0.4, 
	},
	[6] = {
		needTimes = 40,
		expAdd = 0.5, 
	},
}

function GetTable()
	return autoTable
end
