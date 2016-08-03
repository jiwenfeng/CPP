#ifndef __SCENE_DEFINE_H__
#define __SCENE_DEFINE_H__

#include <vector>
#include <list>
#include <bitset>
#include <algorithm>
#include "define.h"
#include "data_role.h"
#include "postion.h"
#include "amtsvr.h"
#include "pb_mgr.h"
#include "base_time.h"
#include "config_mgr.h"
#include "random.h"
#include "msg_handler_pool.h"

using namespace std;

#define MAX_MOVE_NODE               128  // 最大行走结点个数
#define MAX_CAMP_NUM                8    // 最大阵营个数
#define DEFAUTL_SCENE_INST_USER_NUM 3000 // 默认场景用户数

enum ROLE_TYPE_E
{
    RTE_USER = 0,
    RTE_NPC  = 1, 
    RTE_PET  = 2,
    RTE_MAX,
};

enum EFFECT_CALCULATION_METHOD
{
    CALCULATION_NULL = 0,
    CALCULATION_ADD = 1,
    CALCULATION_SUB = 2,
    CALCULATION_MULTI  = 3,
    CALCULATION_DIVIDE = 4,
};

//阵营
enum CAMP_NORMAL_TYPE_E
{
    CNTE_TYPE_USR = 0,				//玩家
    CNTE_TYPE_NPC,					//普通NPC
    CNTE_NORMAL_MONSTER,			//普通怪物
    CNTE_MONSTER_GROUP_1,			//怪物阵营1
    CNTE_MONSTER_GROUP_2,			//怪物阵营2
    CNTE_MONSTER_GROUP_3,			//怪物阵营3
    CNTE_MONSTER_GROUP_4,			//怪物阵营4
    CNTE_MONSTER_GROUP_5,			//怪物阵营5
	CNTE_DEFEND_FLAG_USER,         //保护旗帜阵营
	CNTE_ATTACK_FLAG_USER,           //夺取旗帜阵营
    CNTE_GROUP_MAX,
};

enum REGION_ATTACK_TYPE_E
{
    EATE_NULL      = 0, // 无攻击模式限制
    EATE_FORCE_SET = 1, // 强制设置成
    EATE_FORBID    = 2, // 禁止设置的攻击模式类型
};

struct region_data
{	
    region_data() {
    };

    region_data& operator=(const region_data& rhs) {
        id     =      rhs.id;   
        return *this;
    }

    uint32 id;     //区域编号
};

enum ROLE_FACING {
    NORTH     = 0,
    NORTHEAST,
    EAST     ,
    SOUTHEAST,
    SOUTH    ,
    SOUTHWEST,
    WEST     ,
    NORTHWEST,
    MAX_ROLE_FACING,
};

enum MOVE_TYPE {
    MT_NORMAL           = 0,
    MT_FORCE            = 1,
    MT_FORCE_AND_NOTIFY = 2,
};

#endif
