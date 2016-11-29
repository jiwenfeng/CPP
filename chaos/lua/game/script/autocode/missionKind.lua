--任务列表
local autoTable = {
	["431004"] = {
		class = "collect",
		type = "missKind",
		name = "救命药草",
		aim = "老酋长陌司伏秦的孙子犯了重病，需要紫花兰治疗，但是一直找不到该药。",
		needLv = 20,
		needCopyId = 697016,
		content = "老酋长陌司伏秦的孙子犯了重病，需要紫花兰治疗，但是一直找不到该药。",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s收集%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210001",
		endQ = "啊！这不是我一直在寻找的药么，来的真及时，真的太感谢了！",
		endA = "老人家不必客气，这是我应该做的",
	},
	["431005"] = {
		class = "collect",
		type = "missKind",
		name = "受伤的小狗",
		aim = "酋长家的小狗受伤了，急需止血草进行治疗。",
		needLv = 20,
		needCopyId = 697016,
		content = "酋长家的小狗受伤了，急需止血草进行治疗。",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s收集%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210001",
		endQ = "恩？我正忙着准备出去帮它找药治疗呢，想不到你已经找到了，真是有爱的年轻人啊！",
		endA = "助人乃快乐之本！",
	},
	["431007"] = {
		class = "collect",
		type = "missKind",
		name = "打造材料",
		aim = "铁匠大叔帮人打造一件武器，但是紧缺铁矿一直找不到！",
		needLv = 20,
		needCopyId = 697016,
		content = "铁匠大叔帮人打造一件武器，但是紧缺铁矿一直找不到！",
		target = {
			{type = "collect", num = 5},
		},
		shortDesc = "去%s采集%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210007",
		endQ = "哎哟，真的谢谢你帮我找来了，这武器是我帮我朋友做的，要是没做出来还真不好办！",
		endA = "以前大叔帮我那么多，这是我应该做的！",
	},
	["431008"] = {
		class = "dialog",
		type = "missKind",
		name = "送信",
		aim = "睿芮很想念她妹妹，想给她送封信，但是自己一直很忙走不开！",
		needLv = 20,
		content = "睿芮很想念她妹妹，想给她送封信，但是自己一直很忙走不开！",
		target = {
			{type = "dialog", id = 210011, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>古舟城</u></font>送信给%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210011",
		endQ = "真是太好了，姐姐终于来信了，我都好久没见过她了，好想念她哦，谢谢你咯！",
		endA = "恩恩，不用客气！",
	},
	["431009"] = {
		class = "dialog",
		type = "missKind",
		name = "慰问",
		aim = "最近天气不好，邢简大叔的腿病又犯了，快去慰问一下他！",
		needLv = 20,
		content = "最近天气不好，邢简大叔的腿病又犯了，老人家一个人也挺孤独的，去看看他！",
		target = {
			{type = "dialog", id = 210003, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>真炎部落</u></font>慰问%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210003",
		endQ = "哟，你们这些年轻人还挺有心的吗，不枉我当年那么照顾你们！",
		endA = "呵呵，承蒙大叔恩惠了！",
	},
	["431012"] = {
		class = "dialog",
		type = "missKind",
		name = "送信",
		aim = "最近魔族似有大动作，邗昊需要找人帮忙送信给古舟城的冉可！",
		needLv = 20,
		content = "最近魔族似有大动作，邗昊需要找人帮忙送信给古舟城的冉可！",
		target = {
			{type = "dialog", id = 210016, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>熊弋城</u></font>向%s汇报情况",
		prize = {
		},
		given = {
		},
		endNpcId = "210016",
		endQ = "像你这么热心的年轻人真是太少了，谢谢你来告知，看来我得好好准备才行！",
		endA = "姐姐你这么夸我我会不好意思的！",
	},
	["431013"] = {
		class = "dialog",
		type = "missKind",
		name = "关系调和",
		aim = "魃落跟九霄的观念有点分歧而搞得最近关系不好，去找魃落帮九霄说好话！",
		needLv = 20,
		content = "魃落跟九霄的观念有点分歧而搞得最近关系不好，去找魃落帮九霄说好话！",
		target = {
			{type = "dialog", id = 210020, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>太虚仙境</u></font>找%s谈谈",
		prize = {
		},
		given = {
		},
		endNpcId = "210020",
		endQ = "其实我心里并不生气的，只是这个家伙整天一脸严肃的，搞的我很不爽，不过还是要谢谢你了！",
		endA = "前辈乃真性情啊！",
	},
	["431014"] = {
		class = "dialog",
		type = "missKind",
		name = "关系调和",
		aim = "魃落跟九霄的观念有点分歧而搞得最近关系不好，去找九霄帮魃落说好话！",
		needLv = 20,
		content = "魃落跟九霄的观念有点分歧而搞得最近关系不好，去找九霄帮魃落说好话！",
		target = {
			{type = "dialog", id = 210021, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>太虚仙境</u></font>找%s谈谈",
		prize = {
		},
		given = {
		},
		endNpcId = "210021",
		endQ = "哼，既然他叫你来求情了，那我就勉强的原谅他吧！麻烦你了！",
		endA = "前辈真是宽宏大量啊！",
	},
	["431015"] = {
		class = "buyItem",
		type = "missKind",
		name = "送花",
		aim = "狄白暗恋冉可，可是一直不敢表白，不如带上鲜花，假装是狄白托你去表白的，以成就一桩好事！",
		needLv = 20,
		content = "狄白暗恋冉可，可是一直不敢表白，他想拖人送朵花给冉可！",
		target = {
			{type = "buyItem", id = 101002, num = 1},
		},
		shortDesc = "将%s送给冉可",
		prize = {
		},
		given = {
		},
		endNpcId = "210016",
		endQ = "其实我对他也有意，可是总不能让我一个女孩子主动吧，想不到他竟然开窍了！",
		endA = "呵呵，那就祝你们幸福啦！",
	},
	["431016"] = {
		class = "dialog",
		type = "missKind",
		name = "赞助",
		aim = "小七一个人孤苦伶仃的，这么一个小孩生活相当的艰苦，去安慰下她，有钱就赞助点！",
		needLv = 20,
		content = "熊弋城的小七一个人孤苦伶仃的，无依无靠，这么一个小孩生活相当的艰苦！",
		target = {
			{type = "dialog", id = 210011, num = 1},
		},
		shortDesc = "去<font color = \"#ffff00\"><u>熊弋城</u></font>安慰下%s",
		prize = {
		},
		given = {
		},
		endNpcId = "210011",
		endQ = "打发点咯，哥哥姐姐们！哎呀您真是好人啊！",
		endA = "小妹妹，拿去买糖吧！",
	},
	["431018"] = {
		class = "buyItem",
		type = "missKind",
		name = "力量秘药",
		aim = "近日跟魔族的战争越演越烈，军需部严重缺少力量秘药！",
		needLv = 20,
		content = "近日跟魔族的战争越演越烈，军需部严重缺少秘药！",
		target = {
			{type = "buyItem", id = 101012, num = 1},
		},
		shortDesc = "将%s捐给郝大叔",
		prize = {
		},
		given = {
		},
		endNpcId = "210031",
		endQ = "哎哟，真是不错的年轻人啊，军需部现在确实紧缺这些东西，你来的真的是太及时了！",
		endA = "恩恩，不用客气！",
	},
	["431019"] = {
		class = "buyItem",
		type = "missKind",
		name = "智力秘药",
		aim = "近日跟魔族的战争越演越烈，军需部严重缺少智力秘药！",
		needLv = 20,
		content = "近日跟魔族的战争越演越烈，军需部严重缺少秘药！",
		target = {
			{type = "buyItem", id = 101015, num = 1},
		},
		shortDesc = "将%s捐给郝大叔",
		prize = {
		},
		given = {
		},
		endNpcId = "210031",
		endQ = "哎哟，真是不错的年轻人啊，军需部现在确实紧缺这些东西，你来的真的是太及时了！",
		endA = "恩恩，不用客气！",
	},
	["431020"] = {
		class = "buyItem",
		type = "missKind",
		name = "英雄秘药",
		aim = "近日跟魔族的战争越演越烈，军需部严重缺少英雄秘药！",
		needLv = 20,
		content = "近日跟魔族的战争越演越烈，军需部严重缺少秘药！",
		target = {
			{type = "buyItem", id = 101023, num = 1},
		},
		shortDesc = "将%s捐给郝大叔",
		prize = {
		},
		given = {
		},
		endNpcId = "210031",
		endQ = "哎哟，真是不错的年轻人啊，军需部现在确实紧缺这些东西，你来的真的是太及时了！",
		endA = "恩恩，不用客气！",
	},
}

function GetTable()
	return autoTable
end
