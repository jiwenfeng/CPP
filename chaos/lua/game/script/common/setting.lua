--设置

--设置项
AUTO_USE_DEVOUR_PRO = 1			--仙器吞噬时自动使用吞噬符
RETREAT_EXP_DOUBLE = 3			--修仙塔自动使用经验翻倍道具
REFINE_NO_REMIND = 8			--淬炼不再提示
FAMOUS_EMPL_NO_REMIND = 10		--名将招募不再提示

WORLD_CHAT_NO_REMIND = 2		--世界聊天不再提醒扣除元宝
GODHERO_FRESH_NO_REMIND = 4		--神将刷新不再提醒
UNION_INVITE_NO_REMIND = 5		--联盟邀请不要提示
HEARSAY_CHAT_NO_REMIND = 6		--喇叭消息时不再提示扣除元宝
SHOW_SCENE_USER_NUM = 7			--显示场景玩家
ADV_EMPL_NO_REMIND = 9			--高级招募不再提示
WB_EMBRAVE_NO_REMIND_INGOT = 11		--世界boss元宝鼓舞不再提示
WB_EMBRAVE_NO_REMIND_NIMBUS = 12	--世界boss灵气鼓舞不再提示
BUY_HERO_APT_PILL_NO_REMIND = 13	--购买飞仙丹不再提示
RESET_SKILL_NO_REMIND = 14			--重置技能不再提示
RESET_POINT_NO_REMIND = 15			--重置技能点不再提示
USE_SKILL_PILLS_NO_REMIND = 16		--使用帝脉丹不再提示
USE_SKILL_BOOK_NO_REMIND = 17		--使用技能书不再提示
ARENA_ACCELERATE_NO_REMIND = 18		--竞技场加速冷去不再提示
REFINE_USE_INGOT_NO_REMIND = 19		--元宝炼制不再提示
REMERGE_USE_INGOT_NO_REMIND = 20	--重练不再提示
AUTOREFINE_USE_GOLD_NO_REMIND = 21	--自动炼器不再提示
DEVOUR_NO_REMIND = 22				--吞噬
AUTO_DEVOUR_NO_REMIND = 23			--一键吞噬
OVERWRITE_OLD_ATTR_NO_REMIND = 24	--新属性低于旧属性不再提示
UNSAVE_NEW_ATTR_NO_REMIND = 25		--新属性未保存不再提示

--1 当天有效
--0 永久有效
local AllSettingList = {
	[1] = 1, [2] = 1, [3] = 0, [4] = 1, [5] = 1, [6] = 1, [7] = 0, [8] = 1, 
	[9] = 1, [10] = 1, [11] = 1, [12] = 1, [13] = 1, [14] = 1, [15] = 1, [16] = 1,
	[17] = 1, [18] =1, [19] = 1, [20] = 1, [21] = 1, [22] = 1, [23] = 1, [24] = 1, [25] = 1
}

function IsValidSetting(type)
	return AllSettingList[type]
end

function GetSettingList()
	return AllSettingList
end

function IsDailySetting(type)
	return AllSettingList[type] == 1
end


--------------------------------------------------------
--功能项

FUNC_MISSION = 0	--任务
FUNC_PROP = 1		--道具商人
FUNC_EQUIP = 2		--装备商人
--FUNC_OFFICIAL = 3	--官职竞选
FUNC_PHYDEVOTE = 4	--势力捐献
FUNC_PHYEXCHG = 5	--势力贡献兑换


FUNC_ENHANCE = 6	--强化
FUNC_FAIRYDEV = 7	--炼制仙器
--FUNC_EMPLOYE = 8	--酒馆雇佣
FUNC_PILLAGE = 9	--掠夺
FUNC_MALL = 10		--商城

FUNC_BOURSE = 11	--交易行界面
FUNC_TAVERN = 12	--酒馆界面
FUNC_ALTAR = 13		--祭坛界面
FUNC_FORMATION = 14	--阵型
FUNC_RANK = 15		--排名
FUNC_HEROINFO = 16	--武将查看界面
FUNC_ACHIEVE = 17	--成就
FUNC_DAILYMISSION = 18	--日常任务
FUNC_ARENA = 19		--竞技场
FUNC_TREASURE = 20	--寻宝
FUNC_UNION = 21		--联盟
FUNC_BAG = 22		--背包界面
FUNC_MISSION_UI = 23	--任务按钮


FUNC_DALIY_LOGIN = 24	--每日登陆
FUNC_UNION_DEVOTE = 25	--联盟捐献
FUNC_ATK_CITY_SAME_PHYLE = 26	--攻打己方势力城池
FUNC_ATK_CITY_OHER_PHYLE = 27	--攻打敌城(不同势力)
FUNC_ATK_WILD_MINE = 28	--攻打野地矿藏
FUNC_NORMAL_COPY = 29	--单人副本
FUNC_HERO_COPY = 30		--武将副本
FUNC_TEAM_COPY = 31		--组队副本
FUNC_RETREAT = 32		--修仙塔界面
FUNC_WORLD_CHAT = 33	--世界频道发言
FUNC_SEND_ROSE = 34		--每日送花
FUNC_PHYLE_FIGHT = 35	--势力战
FUNC_PET_UI = 36		--本命精灵按钮
FUNC_MEDITATE = 37 		--打坐
FUNC_FRIEND = 38		--好友系统
FUNC_PHYLE_CHOOSE = 39	--势力选择
FUNC_TREE_ROCK = 40		--摇钱树

FUNC_WORLD_BOSS_1 = 41	--世界boss1
FUNC_WORLD_BOSS_2 = 42	--世界boss2
FUNC_WORLD_BOSS_3 = 43	--世界boss3

FUNC_CITY_FIGHT = 44	--城池争夺战
FUNC_MON_ALBUM = 45		--炼妖阵
FUNC_MISS_KIND = 46		--日行一善
FUNC_ATK_WILD_MONSTER = 47	--攻打野地怪物


FUNC_TRIBE = 200			--部落
FUNC_ST_SPAR = 201			--晶石矿
FUNC_ST_ACADEMY = 202		--学馆
FUNC_ST_EMBASSY = 203		--使馆
FUNC_ST_RETREAT = 204		--修仙塔
FUNC_ST_FURNACE = 205		--天工炉
FUNC_ST_MAINHALL = 206		--议事厅
FUNC_ST_MARKET = 207		--市场
FUNC_ST_BARRACK = 208		--兵营


FUNC_FAVOR_GAME = 100		--收藏游戏

ACT_ONLINE_PRIZE = 101		--在线时长奖励
ACT_RECHARGE_PRIZE = 102	--累计充值奖励
ACT_FIRST_RECHARGE = 103	--首充礼包
ACT_DAILY_ACT = 104			--每日活动
ACT_ARENA_PRIZE = 105		--竞技场奖励
ACT_PHYLE_PRIZE = 106		--势力建设值奖励



