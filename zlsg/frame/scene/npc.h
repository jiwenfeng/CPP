#ifndef __NPC_H__
#define __NPC_H__

#include "role.h"
#include "postion.h"
#include "config_mgr.h"

enum NPC_TYPE
{
    NT_MONSTER  = 0, // ��ͨС��
    NT_ELITE    = 1, // ��Ӣ
    NT_BOSS     = 2, // BOSS
    NT_NPC      = 3, // NPC
    NT_GUARD    = 4, // ����
    NT_PICK     = 5, // �ɼ���
    NT_FRIEND   = 6, // �����ͬ��Ӫ
    NT_TEMP_PET = 7, // ��������ʱ��������뿪���Զ����
    NT_TASK_NPC = 8, // ����NPC
};

enum NPC_ATTACK_MODE
{
    NAM_NOT        = 1, // ������
    NAM_PASSTIVE   = 2, // ��������
    NAM_INITIATIVE = 3, // ��������
    NAM_NO         = 4, // ���ܱ�����
};

enum NPC_TYPE_E
{
    NPC_TYPE_MONSTER = 0,	// ����
    NPC_TYPE_ELITE,         // ��Ӣ
    NPC_TYPE_BOSS,	        // BOSS
    NPC_TYPE_STORE,         // �ֿ�
    NPC_TYPE_GUARD,         // ����
    NPC_TYPE_PICK,          // ��ժ
    NPC_TYPE_NONE,	//����
    NPC_TYPE_TRNS,		//����
    NPC_TYPE_FUN,		//����
    NPC_TYPE_WILDBOSS,  //Ұ��BOSS
    NPC_TYPE_ACTI,		//�

    NPC_TYPE_DECO,		//װ��
    NPC_TYPE_SCENCE,    //Ұ�ⳡ���һ�
};

enum NPC_ATTACK_TYPE_E
{
    NATE_DEFENCE    = 1, // 1������
    NATE_PASSIVE    = 2, // 2��������
    NATE_INITIATIVE = 3, // 3��������
    NATE_NULL       = 4, // 4���ɱ����� 
};

enum NPC_RELIVE_TYPE_E
{
    NRTE_NULL     = 0,
    NRTE_DEFAULT  = 1, // ϵͳĬ�ϸ������
    NRTE_NEVER    = 2, // �������ٸ���
    NRTE_ONTIME   = 3, // ��ʱ�������
    NRTE_INTERVAL = 4, // ������N�븴��
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
