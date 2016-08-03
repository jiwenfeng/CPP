#ifndef __PLAYER_ROLE_STATIS_H__
#define __PLAYER_ROLE_STATIS_H__

#include "comm_define.h"

#define ACTIVITY_DAY_TASK        3
#define ACTIVITY_DAY_FAMILY      3
#define ACTIVITY_FAMILY_DONATE   4
#define ACTIVITY_FAMILY_WELFARE  4
#define ACTIVITY_DAY_ONLINE      20
#define ACTIVITY_PUSH_FIG        2
#define ACTIVITY_WORLD_BOSS      25
#define ACTIVITY_PET_RAID        4
#define ACTIVITY_MONEY_RAID      4
#define ACTIVITY_EXP_RAID        10
#define ACTIVITY_STONE_RAID      4
#define ACTIVITY_TOWER_RAID      5
#define ACTIVITY_TEAM_TOWER_RAID 30
#define ACTIVITY_HIDE_PUSH_FIG   15
#define ACTIVITY_SECRETE_ELITE   18
#define ACTIVITY_BATTLE          25
#define ACTIVITY_FAMILY_BOSS     10
#define ACTIVITY_FAMILY_RAID     10
#define ACTIVITY_SPA             5
#define ACTIVITY_ARENA           1

enum PLAYER_ROLE_STATIS_TYPE_E
{
    PRSTE_DAY_ONCE_LIMIT				 = 1,   // 每天一次性限制 0 没有记录 1 有记录  右起第一位每天数据是否初始化
    PRSTE_USER_STATE					 = 2,   // 各种状态 1扫荡
    PRSTE_ENTER_RAID_COUNT				 = 3,   // 0xFFFF0000精英副本今日挑战成功次数 0x0000FFFF征战普通副本今日挑战成功次数
    PRSTE_AUTO_RAID_COUNT				 = 4,   // 今日扫荡征战副本次数
	PRSTE_BLACK_REFRESH_COUNT			 = 5,   // 黑市商店刷新次数记录
	PRSTE_BLACK_FRIST_REFRESH			 = 6,   // 黑市商店首次刷新记录 0 没有记录 1已经刷新过
	PRSTE_LEVELS_RESET_COUNT			 = 7,	// 过关斩将重置次数
    PRSTE_SIGN_IN						 = 8,   // 签到记录标记
    PRSTE_SIGN_IN_AWARD					 = 9,   // 签到奖励领取标记（不用）
    PRSTE_OPEN							 = 10,  // 功能开放标记
	PRSTE_ARENA_CHALLENGE_TIMES			 = 11,	// 竞技场挑战次数
	PRSTE_ARENA_RESET_CHALLENGE_TIMES	 = 12,  // 竞技场重置挑战次数
	PRSTE_ARENA_RECOVER_TIMES		     = 13,	// 竞技场恢复挑战次数
	PRSTE_MOUNT_FRIST_RIDE				 = 14,  // 坐骑第一次骑乘标记
	PRSTE_LAND_DAY						 = 15,  // 登陆的天数记录
	PRSTE_LAND_DAY_REWARD				 = 16,  // 登陆奖励领取的标记
	//////////////下面的可以改动使用/////////////////////////
    PRSTE_TOWER_LAYER           = 17,  // 塔层数 0xFFFF0000 通天塔 0x0000FFFF 通天塔真
    PRSTE_DAY_VIP_TRANS         = 18,  // VIP每天传送次数
    PRSTE_DAY_HIDE_RAID         = 19,  // 隐藏关卡32位 右起每位标识一个关卡
    PRSTE_MONTH_ONLINE          = 20,  // 每月在线天 每一位代表每一日 0 没上线 1上线
    PRSTE_MONTH_ONLINE_AWARD    = 21,  // 每月在线奖励 右起所在位代表积累天奖励 0 没领 1 领取
    PRSTE_WEEK_CUR_ONLINE       = 22,  // 周在线秒数
    PRSTE_WEEK_LAST_ONLINE      = 23,  // 上周在线秒数
    PRSTE_ONLINE_TIME           = 24,  // 闭关潜修时间
    PRSTE_WELFARE_GIFTS         = 25,  // 礼包领取情况 右起每位 礼包兑换(首充 新手卡 推广) 充值大礼(首充 1000 2500 ......)
    PRSTE_YELLOW_VIP_GIFTS      = 26,  // 黄钻礼包 右起第一位 新手礼包 第二位开始为等级成长礼包
    PRSTE_WEEK_ONCE_LIMIT       = 27,  // 每周一次性限制 右起第一位上周在线奖励 0 没领 1领取 第二位 攻城战官员奖励
    PRSTE_ONLINE_GIFTS_RECORD   = 28,  // 在线礼包领取记录 右起第一位1分钟 二 5分钟....
    PRSTE_ONLINE_GIFTS_TIME     = 29,  // 在线礼包时间秒数
    PRSTE_TEST_GIFTS_RECORD     = 30,  // 封测礼包领取记录 右起第一位登陆 二 40分钟....
    PRSTE_TEST_GIFTS_TIME       = 31,  // 封测礼包时间秒数
    PRSTE_DAY_ACTIVITY          = 32,  // 活跃度
    PRSTE_DAY_ONLINE_TIME       = 33,  // 每日在线时间
    PRSTE_DAY_ACTIVITY_GIFTS    = 34,  // 活跃度礼包领取记录 右起第一位开始 15, 50, 100 .....
    PRSTE_TARGET_ONCE_0         = 35,  // 右起 1宠物出战 2使用群攻技能 3使用怒气技能 4加入家族 5使用临时翅膀 6装备戒指 7装备翅膀 8进入家族副本仙官赐福 9进入组队爬塔
    PRSTE_TARGET_HALF_BYTE_0    = 36,  // 右起 1添加好友 2杀掉怪物 3杀掉玩家 4杀掉精英 5杀掉BOSS 6进入宠物副本 7进入铜钱副本 8学习技能
    PRSTE_TARGET_HALF_BYTE_1    = 37,  // 右起 1天机探险 2翅膀等级 3宠物品质 4参加攻城战 5参加领土战 6装备个数 7加持20的装备 8洗炼过的装备
    PRSTE_TARGET_HALF_BYTE_2    = 38,  // 右起 1紫色装备 2升华50级装备 3镶石头 4天图开启个数 5天都探险
    PRSTE_TARGET_BYTE           = 39,  // 右起 1筋斗云 2参加战场 3宠物进阶
    PRSTE_TARGET_COMPLETE_0     = 40,  // 达成标记 id对应右起每位
    PRSTE_TARGET_COMPLETE_1     = 41,
    PRSTE_TARGET_GIFTS_0        = 42,  // 领取标记 id对应右起每位
    PRSTE_TARGET_GIFTS_1        = 43,
    PRSTE_TARGET_ADD_GIFTS      = 44,  // 额外领取标记 id对应右起每位
    PRSTE_INVEST_RECORD         = 45,  // 右起 1标识是否参与投资 2第一天礼包 3第二天 4第三....
    PRSTE_INVEST_JOIN_TIME      = 46,  // 参加投资时间 unixtime
    PRSTE_OPEN_SERVER_GIFTS     = 47,  // 开服活动充值领取记录
    PRSTE_CONVOY                = 48,  // 每天3次免费刷镖车的机会(第1个Byte), 每天10次抢镖车的机会（第2个Byte）
    PRSTE_DAY_BYTE_LIMIT        = 49,  // 每天数次限制  0x000000FF 制作月饼个数 ..FF00 家族技能属性可点次数(第一次每个属性占一位) ..FF0000家族技能属性可点次数(第二次每个属性占一位)
    PRSTE_DOMAIN_DAY_REWARD     = 50,  // 0x000000FF 每天领取家家族领土奖励 0x00000F00 领主戒指标记 0x0000F000 城主戒指标记
    PRSTE_FILED_SCORE           = 51,  // 野外争夺积分
    PRSTE_OTHER_DAY_RAID_TIMES  = 52,  // 其它副本每日统计项　合欢谷副本次数统计 0xFF 领取仙宗贡奉奖励 0xFF00
    PRSTE_MATE_REWARD           = 53,  // 右起每位标记每领取的一个奖励

	PRSTE_ARENA_CD				= 54,	// 冷却时间

	PRSTE_OFFLINE_TIMELONG		= 55,	// 单次离线时长
	PRSTE_TOTAL_OFFLINE_TL		= 56,	// 离线总时长
	PRSTE_CYCLE_INVITE_COUNT	= 57,	// 掠阵次数
    PRSTE_MAX
};

enum PLAYER_ROLE_STATIS_UPDATE_TYPE_E
{
    PRSUTE_NOT_UPDATE   = 0, // 一直累加不更新
    PRSUTE_DAY_UPDATE   = 1, // 按天更新统计项
    PRSUTE_WEEK_UPDATE  = 2, // 按周更新统计项
    PRSUTE_MONTH_UPDATE = 3, // 按月更新统计项
    PRSUTE_MAX
};
// 签到天数对应的标识值sign_vale[当前日期]
const uint32 sign_vale[32] = {0x00000000,0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,
							  0x00000080,0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,
							  0x00008000,0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,
							  0x00800000,0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000};
class player_role_statis : public pb_writer
{
public:
    player_role_statis();
    virtual ~player_role_statis();

public:
    int attach(mysql::tb_role_statis* trs);

    int set_role_id(uint32 role_id);

    int add(uint32 role_id, PLAYER_ROLE_STATIS_TYPE_E type, PLAYER_ROLE_STATIS_UPDATE_TYPE_E update_type, uint32 val = 1);

    uint32 get();

    uint32 get_last_time();
	void set_last_time(uint32 t);

    int get_val(uint32& val);

    int set_val(uint32 val);
	int only_set_val(uint32 val);

    void write(amtsvr_context* ctx);

public:
    void set_type(PLAYER_ROLE_STATIS_TYPE_E type, PLAYER_ROLE_STATIS_UPDATE_TYPE_E e);

    PLAYER_ROLE_STATIS_UPDATE_TYPE_E get_update_type();

    void add_val(uint32 val);

    void sub_val(uint32 val);

    bool is_same_day(uint32 now);
    bool is_same_week(uint32 now);
    bool is_same_month(uint32 now);

public:
    uint32 get_team_exp_raid(uint32 src);
    uint32 get_exp_raid(uint32 src);
    uint32 add_team_exp_raid(uint32 src, uint32 num = 1);
    uint32 add_exp_raid(uint32 src, uint32 num = 1);
    uint32 get_pet_raid(uint32 src);
    uint32 add_pet_raid(uint32 src, uint32 num = 1);
    uint32 get_stone_raid(uint32 src);
    uint32 add_stone_raid(uint32 src, uint32 num = 1);
    uint32 add_family_raid(uint32 src, uint32 num = 1);

    uint32 get_team_layer_raid(uint32 src);
    uint32 add_team_layer_raid(uint32 src, uint32 num = 1);
    uint32 get_layer_raid(uint32 src);
    uint32 add_layer_raid(uint32 src, uint32 num = 1);
    uint32 get_money_raid(uint32 src);
    uint32 add_money_raid(uint32 src, uint32 num = 1);

    uint32 set_family_welfare_day_once_limit(uint32 src);
    uint32 set_join_battle_day_once_limit(uint32 src);
    bool is_family_welfare_day_once_limit(uint32 src);
    bool is_join_battle_day_once_limit(uint32 src);
    bool is_vip_premiums_day_once_limit(uint32 src);
public:
    void fill(client::role_statis_rsp& rsp);

protected:
    bool is_need_update(PLAYER_ROLE_STATIS_UPDATE_TYPE_E update_type, uint32 now);

protected:
    mysql::tb_role_statis* db_role_statis;
};

#endif
