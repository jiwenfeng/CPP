--联盟活动列表
 local autoTable = {
	["500001"] = {
		name = "联盟秘境",
		needLv = 1,
		type = "uniSecret",
		describe = "盟主副盟主每天可在秘境中召唤玄冥\n进入后献祭可获得大量经验",
	},
	["500002"] = {
		name = "饕餮遗迹",
		needLv = 3,
		type = "uniRelic",
		describe = "每天进入遗迹喂养饕餮\n周日可进入遗迹击杀饕餮\n限时击杀饕餮可获得丰厚奖励",
	},
}
function GetTable()
	return autoTable
end
