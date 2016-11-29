--好感度升级表
 local autoTable = {
	[1] = {
		expNeed = 5,
		attrAdd = 0.01,
		expAdd = 0.01,
		retreatAdd = 0.01,
	},
	[2] = {
		expNeed = 10,
		attrAdd = 0.02,
		expAdd = 0.02,
		retreatAdd = 0.02,
	},
	[3] = {
		expNeed = 20,
		attrAdd = 0.03,
		expAdd = 0.03,
		retreatAdd = 0.03,
	},
	[4] = {
		expNeed = 40,
		attrAdd = 0.04,
		expAdd = 0.04,
		retreatAdd = 0.04,
	},
	[5] = {
		expNeed = 80,
		attrAdd = 0.05,
		expAdd = 0.05,
		retreatAdd = 0.05,
	},
	[6] = {
		expNeed = 160,
		attrAdd = 0.06,
		expAdd = 0.06,
		retreatAdd = 0.06,
	},
	[7] = {
		expNeed = 320,
		attrAdd = 0.07,
		expAdd = 0.07,
		retreatAdd = 0.07,
	},
	[8] = {
		expNeed = 640,
		attrAdd = 0.08,
		expAdd = 0.08,
		retreatAdd = 0.08,
	},
	[9] = {
		expNeed = 1280,
		attrAdd = 0.09,
		expAdd = 0.09,
		retreatAdd = 0.09,
	},
	[10] = {
		expNeed = 2560,
		attrAdd = 0.1,
		expAdd = 0.1,
		retreatAdd = 0.1,
	},
}

function GetTable()
	return autoTable
end
