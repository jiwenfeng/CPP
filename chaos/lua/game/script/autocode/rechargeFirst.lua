--充值次数奖励表
local autoTable = {
	[1] = {
		name = '首充礼包',
		picId = 10000,
		price = 1000,
		itemList = {
			{goodsId = 104002, num = 500000},
		},
	},
}

function GetTable()
	return autoTable
end