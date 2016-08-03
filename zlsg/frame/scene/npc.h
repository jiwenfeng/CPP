#ifndef __NPC_H__
#define __NPC_H__

#include "role.h"
#include "postion.h"
#include "config_mgr.h"

enum NPC_TYPE
{
    NT_MONSTER  = 0, // 普通小怪
    NT_ELITE    = 1, // 精英
    NT_BOSS     = 2, // BOSS
    NT_NPC      = 3, // NPC
    NT_GUARD    = 4, // 守卫
    NT_PICK     = 5, // 采集物
    NT_FRIEND   = 6, // 与玩家同阵营
    NT_TEMP_PET = 7, // 场景中临时宠物玩家离开会自动清除
    NT_TASK_NPC = 8, // 任务NPC
};

enum NPC_ATTACK_MODE
{
    NAM_NOT        = 1, // 不攻击
    NAM_PASSTIVE   = 2, // 被动攻击
    NAM_INITIATIVE = 3, // 主动攻击
    NAM_NO         = 4, // 不能被攻击
};

enum NPC_TYPE_E
{
    NPC_TYPE_MONSTER = 0,	// 怪物
    NPC_TYPE_ELITE,         // 精英
    NPC_TYPE_BOSS,	        // BOSS
    NPC_TYPE_STORE,         // 仓库
    NPC_TYPE_GUARD,         // 守卫
    NPC_TYPE_PICK,          // 采摘
    NPC_TYPE_NONE,	//其它
    NPC_TYPE_TRNS,		//传送
    NPC_TYPE_FUN,		//功能
    NPC_TYPE_WILDBOSS,  //野外BOSS
    NPC_TYPE_ACTI,		//活动

    NPC_TYPE_DECO,		//装饰
    NPC_TYPE_SCENCE,    //野外场景挂机
};

enum NPC_ATTACK_TYPE_E
{
    NATE_DEFENCE    = 1, // 1不攻击
    NATE_PASSIVE    = 2, // 2被动攻击
    NATE_INITIATIVE = 3, // 3主动攻击
    NATE_NULL       = 4, // 4不可被攻击 
};

enum NPC_RELIVE_TYPE_E
{
    NRTE_NULL     = 0,
    NRTE_DEFAULT  = 1, // 系统默认复活规则
    NRTE_NEVER    = 2, // 死亡后不再复活
    NRTE_ONTIME   = 3, // 定时复活规则
    NRTE_INTERVAL = 4, // 死亡后N秒复活
};

#define START_NPC_ID_ALLOC 0xffffffff
#define PICK_NEED_TIME     1
#define PICK_MAX_DISTANCE  160

enum NPC_STS_E
{
    NPC_STS_STAND      = 1,
    NPC_STS_MOVE       = 2,
    NPC_STS_FIGHT      = 3,
    NPC_STS_DEAD       = 4,
    NPC_STS_FORCE_MOVE = 5,
    NPC_STS_RAND_MOVE  = 6,
    NPC_STS_BACK_MOVE  = 7,
    NPC_STS_PATH_MOVE  = 8,
    NPC_STS_IDLE       = 9,
    NPC_STS_IN_BLOCK   = 10,
    NPC_STS_HIDE       = 11,
};

class origin_npc;

class npc : virtual public role
{
public:
    npc(const npc& rhs);
    virtual ~npc();
};

#endif
