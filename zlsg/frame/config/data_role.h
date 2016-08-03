#ifndef __DATA_ROLE_H__
#define __DATA_ROLE_H__

#include "define.h"

enum PROFESSION_CODE
{
    WARRIOR   = 0,
    WIZARD_M  = 1,
    WIZARD_W  = 2,
    WIZARD    = 4,
    ALL       = 5,
};

enum ROLE_ATTR_E
{
    ATTR_MAX_LIFE = 1,    //(增加)血量上限
    ATTR_INC_LIFE,        //(增加)血量上限%量(本命属性)
    ATTR_CUR_LIFE,        //当前血量
    ATTR_MAX_POWER,       //(增加)仙气上限
    ATTR_INC_POWER,       //(增加)仙气上限%量
    ATTR_CUR_POWER,       //当前仙气值
    ATTR_MAX_WRATH,       //怒气上限
    ATTR_CUR_WRATH,       //当前怒气
    ATTR_ATTACK,          //攻击力
    ATTR_INC_ATTK,        //(增加)攻击上限%量
    ATTR_DEFENSE,         //防御
    ATTR_INC_DEFENSE,     //(增加)防御上限%量

    ATTR_HITPOINT,	      //命中值
    ATTR_DODGE,		      //闪避值
    ATTR_CRITICAL,	      //暴击值
    ATTR_TOUGNEN,		  //韧性值
    ATTR_AGISITY,		  //灵力值
    ATTR_RETURN_HARM_PER, //反弹受到的n%物理伤害
    ATTR_RETURN_HARM_VAL, //受到伤害时反弹N点伤害
    ATTR_IGNOR,		      //穿透值
    ATTR_DEC_HARM,	      //受到物理伤害时减少N点

    ATTR_METAL_ATTK,	  //金属性攻击N点(五行攻击)
    ATTR_WOOD_ATTK,	      //木属性攻击N点
    ATTR_WATER_ATTK,	  //水属性攻击N点
    ATTR_FIRE_ATTK,	      //火属性攻击N点
    ATTR_SOIL_ATTK,	      //土属性攻击N点

    ATTR_METAL_DEFENSE,   //金属性抗性N点
    ATTR_WOOD_DEFENSE,    //木属性抗性N点
    ATTR_WATER_DEFENSE,   //水属性抗性N点
    ATTR_FIRE_DEFENSE,    //火属性抗性N点
    ATTR_SOIL_DEFENSE,    //土属性抗性N点

    ATTR_METAL_DEC,       //金属性伤害减少N点
    ATTR_WOOD_DEC,        //木属性伤害减少N点
    ATTR_WATER_DEC,	      //水属性伤害减少N点
    ATTR_FIRE_DEC,	      //火属性伤害减少N点
    ATTR_SOIL_DEC,	      //土属性伤害减少N点

    ATTR_METAL_IGNORE,    //金属性穿透N点
    ATTR_WOOD_IGNORE,	  //木属性穿透N点
    ATTR_WATER_IGNORE,    //水属性穿透N点
    ATTR_FIRE_IGNORE,     //火属性穿透N点
    ATTR_SOIL_IGNORE,     //土属性穿透N点

    ATTR_ATTK_SPEED,      //攻击速度N点
    ATTR_MOVE_SPEED,	  //移动速度N点
    ATTR_BOOLD_RECOVER,   //气血恢复速度N点
    ATTR_POWER_RECOVER,   //仙气恢复速度N点
    ATTR_WRATH_RECOVER,   //怒气回复速度N点

    ATTR_PROFESSION,      //职业
    ATTR_LEVEL,           //级数
    ATTR_UPGRADE_EXP,     //升级所需的经验

    ATTR_FACTOR_EXPER,    //获取得经验系数
    ATTR_FACTOR_TAEL,     //获取铜钱系数
    ATTR_FACTOR_POWER,    //获取怒气系数
    ATTR_FACTOR_FORCE,    //获取真气系数

    ATTR_MIND_SENSE_LEVEL,//神识等级			// ATTR_FACTOR_CURE,     //受到治疗效果系数 (之前未使用,该索引征用)
    ATTR_MIND_SENSE_EXP,  //神识经验			// ATTR_FACTOR_BE_ATTK,  //受到攻击伤害系数 (之前未使用,该索引征用)
    ATTR_FACTOR_BE_METAL, //受到金伤害系数		// 暂未使用
    ATTR_FACTOR_BE_WOOD,  //受到木伤害系数		// 暂未使用
    ATTR_FACTOR_BE_WATER, //受到水伤害系数		// 暂未使用
    ATTR_FACTOR_BE_FIRE,  //受到火伤害系数		// 暂未使用
    ATTR_FACTOR_BE_SOIL,  //受到土伤害系数		// 暂未使用

    ATTR_FACTOR_TO_ATTK,  //造成攻击伤害系数	// 暂未使用
    ATTR_FACTOR_TO_METAL, //造成金伤害系数		// 暂未使用
    ATTR_FACTOR_TO_WOOD,  //造成木伤害系数		// 暂未使用
    ATTR_FACTOR_TO_WATER, //造成水伤害系数		// 暂未使用
    ATTR_FACTOR_TO_FIRE,  //造成火伤害系数		// 暂未使用
    ATTR_FACTOR_TO_SOIL,  //造成土伤害系数		// 暂未使用

    ATTR_SEX,             //性别
    ATTR_LABEL,           //称号ID
    ATTR_TITLE,           //官职ID
    ATTR_FAMILY,          //家族ID
    ATTR_MATE_ID,         //配偶ID
    ATTR_MASTER_ID,       //师傅ID

    ATTR_TAEL,            //铜钱
    ATTR_SLIVER,          //仙石
    ATTR_INGOT,           //仙币
    ATTR_INFAMY,          //恶名值
    ATTR_EXPERIENCE,      //经验值
    ATTR_LUCKY,           //幸运值
    ATTR_PVP_FAME,        //野外pvp积分

    ATTR_MAX_ENGINE,      //最大精力
    ATTR_CUR_ENGINE,      //当前精力值
    ATTR_MAX_FORCE,       //最大真气
    ATTR_CUR_FORCE,       //当前真气值

    ATTR_EXP_FAME,        //绑定仙币
    ATTR_BATTLE_FAME,     //竞技声望
    ATTR_RAID_FAME,       //副本声望
    ATTR_MASTER_FAME,     //师徒声望
    ATTR_FAMILY_FAME,     //家族声望
    ATTR_GROUP,           //阵营
    ATTR_SOUL_LEVEL,      //元神等级
    ATTR_SPAR,            //晶石
    ATTR_INTAKING_FORCE,  //每天吸取真气
    ATTR_INTAKING_TIME,	  //最后吸取真气时间
    ATTR_INTAKED_FORCE,	  //每天被吸取真气
    ATTR_INTAKED_TIME,	  //最后被吸取真气时间
    ATTR_EXP_CACHE,       //经验池
    ATTR_VIP_LEVEL,       //VIP等级
    ATTR_TRANS_COUNT,     //免费传送次数
    ATTR_BACKP_SIZE,      //背包大小
    ATTR_DEPOT_SIZE,      //仓库大小
    ATTR_PET_SLOT,        //宠物槽大小
    ATTR_MAX_CHARGE,      //挑战副本最高层数
    ATTR_CHARGE_TIME,     //挑战副本时间
    ATTR_FLOWER,          //鲜花值
    ATTR_GLAMOR,          //魅力值
    ATTR_HOT1,            //温泉互动1
    ATTR_HOT2,            //温泉互动2
    ATTR_HOT3,            //温泉互动3
    ATTR_HOT_TIME,        //温泉上次刷新时间
    ATTR_DOOM_LVL,        //劫数等级
    ATTR_STRENGTH,        //战斗力
    ATTR_RESERVED,		  //保留
    ATTR_PLUNDER_REWARD,  //每天获得劫镖奖励次数
    ATTR_PLUNDER_TIME,    //最后获得劫镖奖励时间
    ATTR_GUARD_QUALITY,   //当前运镖的品质
    ATTR_FREE_GUARD_REFRESH_COUNT,		//免费刷镖次数
    ATTR_FREE_TASK_REFRESH_COUNT,		//免费刷任务次数
    ATTR_VITALITY,						//玩家活跃度
    ATTR_VITALITY_TIME,					//活跃度时间
    ATTR_EQUIP_TOTAL,					//套装 120
    ATTR_EQUIP_COST,					//付费套装效果
    ATTR_REPUTATION,			        // 功勋
    ATTR_FASHION_HIDE,                 //时装隐藏

    ATTIB_MAX,
};

enum ROLE_ATTRIBUTE { // 程序中的顺序
    SEX                            = 0,
    PROFESSION                    , // 职业
    EXPERIENCE                    , // 经验
    LEVEL                         , // 等级
    TAEL                          , // 铜币
    INGOT                         , // 元宝
    GIFT_MONEY                    ,
    IS_SUSPENDED_ACCOUNT          ,
    PARTNER_ID                    ,
    FAMILY_ID                     ,
    FAMILY_POSITION               ,
    CAN_BE_RAISED                 ,
    FAMILY_LEVEL                  , // 军团等级
    CAN_JOIN_FAMILY               , // 改用：为个人帮派贡献
    CAN_BE_TRANSMIT               ,
    PK_MODEL                      ,
    PK_VALUE                      ,
    MILITARY_STRATEGY             , // 韬略
    MAP_ID                        ,
    X_POS                         ,
    Y_POS                         ,
    SIEGE_REPUTATION              , // 军功
    CROSS_SVR_WAR_REPUTATION      ,
    MAX_HP                        , // 最大生命
    CUR_HP                        , // 当前血量
    ABSORB_HP                     ,
    MAX_MP                        ,
    CUR_MP                        ,
    ABSORB_MP                     ,
    ATTACK_MIN                    , // 最小攻击
    ATTACK_MAX                    , // 最大攻击
    DEFENSE_MIN                   ,
    DEFENSE_MAX                   , 
    HIT_POINT                     ,
    DODGE                         ,
    STABILITY                     , // 稳定
    RECOVER                       ,
    ATTACK_SPEED                  ,
    FROZEN_ADDITION_RATIO		  , 
    POISONING_ADDITION_RATIO      ,
    PALSY_ADDITION_RATIO          ,
    DIZZINESS_ADDITION_RATIO      ,
    FROZEN_RESISTANCE_RATIO       ,
    POISONING_RESISTANCE_RATIO    ,
    PALSY_RESISTANCE_RATIO        ,
    DIZZINESS_RESISTANCE_RATIO    ,
    OUTPUT_HURT_PROMOTE_RATIO     ,
    SUFFER_HURT_REDUCE_RATIO      ,
    IGNORE_DEFENSE_RATIO          , // 忽略防御几率
    REBOUND_HURT_RATIO            , // 反弹伤害几率
	EXP_COEFFICIENT				  , // 经验系数
    MOVE_SPEED                    ,
    ARMOR_ID                      ,
    WEAPON_ID                     ,
    MOUNTS_ID                     ,
    BACKPACK_SIZE                 ,
    WAREHOUSE_SIZE                ,
    PHYSICAL_POWER                , // 体力
    FIGHTING_POWER                , // 战斗力
    ANGER                         ,
    CREATE_TIME                   , // 创建时间
    ONLINE_TIME                   , // 上线时间
    OFFLINE_TIME                  , // 离线时间
    UPDATE_TIME                   , // 数据库更新时间
    HISTORY_ONLINE                , // 累计在线时长
	MELTING                       , // 熔炼值
	POTENTIAL_SHARE_MAX			  , // 总计可分享心法冲击波经验上限（随着离线时间不同而动态变化<需要保存>）
	POTENTIAL_ACCUMULATION		  , // 玩家累计已获得的心法冲击波分享经验总量
	ROLE_SKILL_POINT			  ,	// 技能点数
	POPULARITY					  ,	// 竞技场声望
	ROLE_SKILL_POINT_TIME		  , // 技能点回复时间记录
	ROLE_VIP_LEVEL				  , // 玩家VIP等级
	ROLE_TEAM_ID				  , // 玩家队伍ID
	ROLE_CAMP_ID				  , // 玩家阵营ID
	STRATEGY					  , // 谋略
	STORE_WEAPON				  ,
	BOW_ID						  , // 弓箭ID
	BOW_INDEX					  , // 记录的弓箭的格子数
	CAVALRY_INDEX				  , // 记录的骑兵的技能格子数
	BOW_SPEED					  , 
	EQUIP_POWER				      , // 主人与美人的（装备历史最高战力值）
    ROLE_ATTRIBUTE_MAX            ,
};

enum GAME_ATTRIBUTE_E { // 配置中的顺序
    GAME_ATTRIBUTE_MIN                         = 0,
    GAME_ATTRIBUTE_MAX_HP                      ,
    GAME_ATTRIBUTE_ATTACK_MIN                  ,
    GAME_ATTRIBUTE_ATTACK_MAX                  ,
    GAME_ATTRIBUTE_DEFENSE_MIN                 ,
    GAME_ATTRIBUTE_DEFENSE_MAX                 ,
    GAME_ATTRIBUTE_HIT_POINT                   ,
    GAME_ATTRIBUTE_DODGE                       ,
    GAME_ATTRIBUTE_ATTACK_SPEED                ,
    GAME_ATTRIBUTE_RECOVER                     ,
    GAME_ATTRIBUTE_MOVE_SPEED                  ,
    GAME_ATTRIBUTE_MAX_MP                      ,
    GAME_ATTRIBUTE_STABILITY                   ,
    GAME_ATTRIBUTE_CUR_HP                      ,
    GAME_ATTRIBUTE_CUR_MP                      ,
    GAME_ATTRIBUTE_FROZEN_ADDITION_RATIO	   ,
    GAME_ATTRIBUTE_POISONING_ADDITION_RATIO    ,
    GAME_ATTRIBUTE_PALSY_ADDITION_RATIO        ,
    GAME_ATTRIBUTE_DIZZINESS_ADDITION_RATIO    ,
    GAME_ATTRIBUTE_FROZEN_RESISTANCE_RATIO     ,
    GAME_ATTRIBUTE_POISONING_RESISTANCE_RATIO  ,
    GAME_ATTRIBUTE_PALSY_RESISTANCE_RATIO      ,
    GAME_ATTRIBUTE_DIZZINESS_RESISTANCE_RATIO  ,
    GAME_ATTRIBUTE_OUTPUT_HURT_PROMOTE_RATIO   ,
    GAME_ATTRIBUTE_SUFFER_HURT_REDUCE_RATIO    ,
    GAME_ATTRIBUTE_IGNORE_DEFENSE_RATIO        ,
    GAME_ATTRIBUTE_REBOUND_HURT_RATIO          ,
    GAME_ATTRIBUTE_ABSORB_HP                   ,
    GAME_ATTRIBUTE_ABSORB_MP                   ,
    GAME_ATTRIBUTE_MAX
};

static const uint32 ATTR_CODE_TO_DB = 5;
static const ROLE_ATTRIBUTE ATTR_CFG_TO_CODE[GAME_ATTRIBUTE_MAX] = {
    MAX_HP                    ,
    MAX_HP                    ,
    ATTACK_MIN                ,
    ATTACK_MAX                ,
    DEFENSE_MIN               ,
    DEFENSE_MAX               ,
    HIT_POINT                 ,
    DODGE                     ,
    ATTACK_SPEED              ,
    RECOVER                   ,
    MOVE_SPEED                ,
    MAX_MP                    ,
    STABILITY                 ,
    CUR_HP                    ,
    CUR_MP                    ,
    FROZEN_ADDITION_RATIO	  ,
    POISONING_ADDITION_RATIO  ,
    PALSY_ADDITION_RATIO      ,
    DIZZINESS_ADDITION_RATIO  ,
    FROZEN_RESISTANCE_RATIO   ,
    POISONING_RESISTANCE_RATIO,
    PALSY_RESISTANCE_RATIO    ,
    DIZZINESS_RESISTANCE_RATIO,
    OUTPUT_HURT_PROMOTE_RATIO ,
    SUFFER_HURT_REDUCE_RATIO  ,
    IGNORE_DEFENSE_RATIO      ,
    REBOUND_HURT_RATIO        ,
    ABSORB_HP                 ,
    ABSORB_MP                 ,
};

#endif
