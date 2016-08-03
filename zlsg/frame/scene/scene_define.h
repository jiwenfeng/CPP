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

#define MAX_MOVE_NODE               128  // ������߽�����
#define MAX_CAMP_NUM                8    // �����Ӫ����
#define DEFAUTL_SCENE_INST_USER_NUM 3000 // Ĭ�ϳ����û���

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

//��Ӫ
enum CAMP_NORMAL_TYPE_E
{
    CNTE_TYPE_USR = 0,				//���
    CNTE_TYPE_NPC,					//��ͨNPC
    CNTE_NORMAL_MONSTER,			//��ͨ����
    CNTE_MONSTER_GROUP_1,			//������Ӫ1
    CNTE_MONSTER_GROUP_2,			//������Ӫ2
    CNTE_MONSTER_GROUP_3,			//������Ӫ3
    CNTE_MONSTER_GROUP_4,			//������Ӫ4
    CNTE_MONSTER_GROUP_5,			//������Ӫ5
	CNTE_DEFEND_FLAG_USER,         //����������Ӫ
	CNTE_ATTACK_FLAG_USER,           //��ȡ������Ӫ
    CNTE_GROUP_MAX,
};

enum REGION_ATTACK_TYPE_E
{
    EATE_NULL      = 0, // �޹���ģʽ����
    EATE_FORCE_SET = 1, // ǿ�����ó�
    EATE_FORBID    = 2, // ��ֹ���õĹ���ģʽ����
};

struct region_data
{	
    region_data() {
    };

    region_data& operator=(const region_data& rhs) {
        id     =      rhs.id;   
        return *this;
    }

    uint32 id;     //������
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
