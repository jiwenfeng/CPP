--商城道具列表
 local autoTable = {
	[1] = {
		{id = 101003, goldType = "ingot", price = 999},
		{id = 101021, goldType = "ingot", price = 36},
		{id = 101097, goldType = "ingot", price = 20},
		{id = 101098, goldType = "ingot", price = 10},
		{id = 101029, goldType = "ingot", price = 5},
		{id = 101010, goldType = "ingot", price = 30},
	},
	[2] = {
		{id = 101001, goldType = "ingot", price = 20},
		{id = 101002, goldType = "ingot", price = 15},
		{id = 101003, goldType = "ingot", price = 999},
		{id = 101004, goldType = "ingot", price = 9999},
		{id = 101030, goldType = "ingot", price = 198},
		{id = 101031, goldType = "ingot", price = 198},
		{id = 101032, goldType = "ingot", price = 198},
		{id = 101035, goldType = "ingot", price = 10},
		{id = 101036, goldType = "ingot", price = 20},
		{id = 101037, goldType = "ingot", price = 50},
		{id = 101041, goldType = "ingot", price = 20},
		{id = 102021, goldType = "ingot", price = 100},
		{id = 103001, goldType = "ingot", price = 888},
		{id = 103004, goldType = "ingot", price = 888},
		{id = 103005, goldType = "ingot", price = 888},
		{id = 103006, goldType = "ingot", price = 888},
		{id = 103011, goldType = "ingot", price = 888},
		{id = 101094, goldType = "ingot", price = 198},
		{id = 101097, goldType = "ingot", price = 20},
		{id = 101098, goldType = "ingot", price = 10},
	},
	[3] = {
		{id = 102001, goldType = "ingot", price = 5},
		{id = 102003, goldType = "ingot", price = 10},
		{id = 102004, goldType = "ingot", price = 10},
		{id = 102012, goldType = "ingot", price = 5},
		{id = 102013, goldType = "ingot", price = 30},
		{id = 102014, goldType = "ingot", price = 15},
		{id = 102015, goldType = "ingot", price = 10},
		{id = 111103, goldType = "ingot", price = 30},
		{id = 111203, goldType = "ingot", price = 30},
		{id = 111303, goldType = "ingot", price = 30},
		{id = 111403, goldType = "ingot", price = 30},
		{id = 111503, goldType = "ingot", price = 30},
		{id = 111603, goldType = "ingot", price = 30},
		{id = 111703, goldType = "ingot", price = 30},
		{id = 111803, goldType = "ingot", price = 30},
		{id = 111903, goldType = "ingot", price = 30},
		{id = 112003, goldType = "ingot", price = 30},
	},
	[4] = {
		{id = 101006, goldType = "ingot", price = 18},
		{id = 101007, goldType = "ingot", price = 27},
		{id = 101009, goldType = "ingot", price = 10},
		{id = 101010, goldType = "ingot", price = 30},
		{id = 101012, goldType = "ingot", price = 18},
		{id = 101013, goldType = "ingot", price = 27},
		{id = 101015, goldType = "ingot", price = 18},
		{id = 101016, goldType = "ingot", price = 27},
		{id = 101018, goldType = "ingot", price = 18},
		{id = 101019, goldType = "ingot", price = 27},
		{id = 101021, goldType = "ingot", price = 36},
		{id = 101022, goldType = "ingot", price = 15},
		{id = 101023, goldType = "ingot", price = 24},
		{id = 101029, goldType = "ingot", price = 5},
		{id = 101033, goldType = "ingot", price = 30},
		{id = 102019, goldType = "ingot", price = 10},
		{id = 102020, goldType = "ingot", price = 500},
		{id = 102022, goldType = "ingot", price = 80},
		{id = 101095, goldType = "ingot", price = 64},
		{id = 101096, goldType = "ingot", price = 88},
		{id = 101099, goldType = "ingot", price = 36},
	},
	[5] = {
		{id = 101006, goldType = "voucher", price = 10},
		{id = 101007, goldType = "voucher", price = 30},
		{id = 101008, goldType = "voucher", price = 5},
		{id = 101009, goldType = "voucher", price = 10},
		{id = 101010, goldType = "voucher", price = 30},
		{id = 101012, goldType = "voucher", price = 10},
		{id = 101013, goldType = "voucher", price = 30},
		{id = 101015, goldType = "voucher", price = 10},
		{id = 101016, goldType = "voucher", price = 30},
		{id = 101018, goldType = "voucher", price = 10},
		{id = 101019, goldType = "voucher", price = 30},
		{id = 111103, goldType = "voucher", price = 30},
		{id = 111203, goldType = "voucher", price = 30},
		{id = 111303, goldType = "voucher", price = 30},
		{id = 111403, goldType = "voucher", price = 30},
		{id = 111503, goldType = "voucher", price = 30},
		{id = 111603, goldType = "voucher", price = 30},
		{id = 111703, goldType = "voucher", price = 30},
		{id = 111803, goldType = "voucher", price = 30},
		{id = 111903, goldType = "voucher", price = 30},
		{id = 112003, goldType = "voucher", price = 30},
	},
	[6] = {
		{id = 101001, goldType = "ingot", price = 16, oldPrice = 20},
		{id = 101002, goldType = "ingot", price = 12, oldPrice = 15},
		{id = 101003, goldType = "ingot", price = 800, oldPrice = 999},
		{id = 101004, goldType = "ingot", price = 8000, oldPrice = 9999},
		{id = 101006, goldType = "ingot", price = 15, oldPrice = 18},
		{id = 101007, goldType = "ingot", price = 22, oldPrice = 27},
		{id = 101009, goldType = "ingot", price = 8, oldPrice = 10},
		{id = 101010, goldType = "ingot", price = 24, oldPrice = 30},
		{id = 101012, goldType = "ingot", price = 15, oldPrice = 18},
		{id = 101013, goldType = "ingot", price = 22, oldPrice = 27},
		{id = 101015, goldType = "ingot", price = 15, oldPrice = 18},
		{id = 101016, goldType = "ingot", price = 22, oldPrice = 27},
		{id = 101018, goldType = "ingot", price = 15, oldPrice = 18},
		{id = 101019, goldType = "ingot", price = 22, oldPrice = 27},
		{id = 101021, goldType = "ingot", price = 29, oldPrice = 36},
		{id = 101022, goldType = "ingot", price = 12, oldPrice = 15},
		{id = 101023, goldType = "ingot", price = 20, oldPrice = 24},
		{id = 101029, goldType = "ingot", price = 4, oldPrice = 5},
	},
}

function GetTable()
	return autoTable
end