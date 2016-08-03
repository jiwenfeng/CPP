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
    ATTR_MAX_LIFE = 1,    //(����)Ѫ������
    ATTR_INC_LIFE,        //(����)Ѫ������%��(��������)
    ATTR_CUR_LIFE,        //��ǰѪ��
    ATTR_MAX_POWER,       //(����)��������
    ATTR_INC_POWER,       //(����)��������%��
    ATTR_CUR_POWER,       //��ǰ����ֵ
    ATTR_MAX_WRATH,       //ŭ������
    ATTR_CUR_WRATH,       //��ǰŭ��
    ATTR_ATTACK,          //������
    ATTR_INC_ATTK,        //(����)��������%��
    ATTR_DEFENSE,         //����
    ATTR_INC_DEFENSE,     //(����)��������%��

    ATTR_HITPOINT,	      //����ֵ
    ATTR_DODGE,		      //����ֵ
    ATTR_CRITICAL,	      //����ֵ
    ATTR_TOUGNEN,		  //����ֵ
    ATTR_AGISITY,		  //����ֵ
    ATTR_RETURN_HARM_PER, //�����ܵ���n%�����˺�
    ATTR_RETURN_HARM_VAL, //�ܵ��˺�ʱ����N���˺�
    ATTR_IGNOR,		      //��͸ֵ
    ATTR_DEC_HARM,	      //�ܵ������˺�ʱ����N��

    ATTR_METAL_ATTK,	  //�����Թ���N��(���й���)
    ATTR_WOOD_ATTK,	      //ľ���Թ���N��
    ATTR_WATER_ATTK,	  //ˮ���Թ���N��
    ATTR_FIRE_ATTK,	      //�����Թ���N��
    ATTR_SOIL_ATTK,	      //�����Թ���N��

    ATTR_METAL_DEFENSE,   //�����Կ���N��
    ATTR_WOOD_DEFENSE,    //ľ���Կ���N��
    ATTR_WATER_DEFENSE,   //ˮ���Կ���N��
    ATTR_FIRE_DEFENSE,    //�����Կ���N��
    ATTR_SOIL_DEFENSE,    //�����Կ���N��

    ATTR_METAL_DEC,       //�������˺�����N��
    ATTR_WOOD_DEC,        //ľ�����˺�����N��
    ATTR_WATER_DEC,	      //ˮ�����˺�����N��
    ATTR_FIRE_DEC,	      //�������˺�����N��
    ATTR_SOIL_DEC,	      //�������˺�����N��

    ATTR_METAL_IGNORE,    //�����Դ�͸N��
    ATTR_WOOD_IGNORE,	  //ľ���Դ�͸N��
    ATTR_WATER_IGNORE,    //ˮ���Դ�͸N��
    ATTR_FIRE_IGNORE,     //�����Դ�͸N��
    ATTR_SOIL_IGNORE,     //�����Դ�͸N��

    ATTR_ATTK_SPEED,      //�����ٶ�N��
    ATTR_MOVE_SPEED,	  //�ƶ��ٶ�N��
    ATTR_BOOLD_RECOVER,   //��Ѫ�ָ��ٶ�N��
    ATTR_POWER_RECOVER,   //�����ָ��ٶ�N��
    ATTR_WRATH_RECOVER,   //ŭ���ظ��ٶ�N��

    ATTR_PROFESSION,      //ְҵ
    ATTR_LEVEL,           //����
    ATTR_UPGRADE_EXP,     //��������ľ���

    ATTR_FACTOR_EXPER,    //��ȡ�þ���ϵ��
    ATTR_FACTOR_TAEL,     //��ȡͭǮϵ��
    ATTR_FACTOR_POWER,    //��ȡŭ��ϵ��
    ATTR_FACTOR_FORCE,    //��ȡ����ϵ��

    ATTR_MIND_SENSE_LEVEL,//��ʶ�ȼ�			// ATTR_FACTOR_CURE,     //�ܵ�����Ч��ϵ�� (֮ǰδʹ��,����������)
    ATTR_MIND_SENSE_EXP,  //��ʶ����			// ATTR_FACTOR_BE_ATTK,  //�ܵ������˺�ϵ�� (֮ǰδʹ��,����������)
    ATTR_FACTOR_BE_METAL, //�ܵ����˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_BE_WOOD,  //�ܵ�ľ�˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_BE_WATER, //�ܵ�ˮ�˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_BE_FIRE,  //�ܵ����˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_BE_SOIL,  //�ܵ����˺�ϵ��		// ��δʹ��

    ATTR_FACTOR_TO_ATTK,  //��ɹ����˺�ϵ��	// ��δʹ��
    ATTR_FACTOR_TO_METAL, //��ɽ��˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_TO_WOOD,  //���ľ�˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_TO_WATER, //���ˮ�˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_TO_FIRE,  //��ɻ��˺�ϵ��		// ��δʹ��
    ATTR_FACTOR_TO_SOIL,  //������˺�ϵ��		// ��δʹ��

    ATTR_SEX,             //�Ա�
    ATTR_LABEL,           //�ƺ�ID
    ATTR_TITLE,           //��ְID
    ATTR_FAMILY,          //����ID
    ATTR_MATE_ID,         //��żID
    ATTR_MASTER_ID,       //ʦ��ID

    ATTR_TAEL,            //ͭǮ
    ATTR_SLIVER,          //��ʯ
    ATTR_INGOT,           //�ɱ�
    ATTR_INFAMY,          //����ֵ
    ATTR_EXPERIENCE,      //����ֵ
    ATTR_LUCKY,           //����ֵ
    ATTR_PVP_FAME,        //Ұ��pvp����

    ATTR_MAX_ENGINE,      //�����
    ATTR_CUR_ENGINE,      //��ǰ����ֵ
    ATTR_MAX_FORCE,       //�������
    ATTR_CUR_FORCE,       //��ǰ����ֵ

    ATTR_EXP_FAME,        //���ɱ�
    ATTR_BATTLE_FAME,     //��������
    ATTR_RAID_FAME,       //��������
    ATTR_MASTER_FAME,     //ʦͽ����
    ATTR_FAMILY_FAME,     //��������
    ATTR_GROUP,           //��Ӫ
    ATTR_SOUL_LEVEL,      //Ԫ��ȼ�
    ATTR_SPAR,            //��ʯ
    ATTR_INTAKING_FORCE,  //ÿ����ȡ����
    ATTR_INTAKING_TIME,	  //�����ȡ����ʱ��
    ATTR_INTAKED_FORCE,	  //ÿ�챻��ȡ����
    ATTR_INTAKED_TIME,	  //�����ȡ����ʱ��
    ATTR_EXP_CACHE,       //�����
    ATTR_VIP_LEVEL,       //VIP�ȼ�
    ATTR_TRANS_COUNT,     //��Ѵ��ʹ���
    ATTR_BACKP_SIZE,      //������С
    ATTR_DEPOT_SIZE,      //�ֿ��С
    ATTR_PET_SLOT,        //����۴�С
    ATTR_MAX_CHARGE,      //��ս������߲���
    ATTR_CHARGE_TIME,     //��ս����ʱ��
    ATTR_FLOWER,          //�ʻ�ֵ
    ATTR_GLAMOR,          //����ֵ
    ATTR_HOT1,            //��Ȫ����1
    ATTR_HOT2,            //��Ȫ����2
    ATTR_HOT3,            //��Ȫ����3
    ATTR_HOT_TIME,        //��Ȫ�ϴ�ˢ��ʱ��
    ATTR_DOOM_LVL,        //�����ȼ�
    ATTR_STRENGTH,        //ս����
    ATTR_RESERVED,		  //����
    ATTR_PLUNDER_REWARD,  //ÿ���ý��ڽ�������
    ATTR_PLUNDER_TIME,    //����ý��ڽ���ʱ��
    ATTR_GUARD_QUALITY,   //��ǰ���ڵ�Ʒ��
    ATTR_FREE_GUARD_REFRESH_COUNT,		//���ˢ�ڴ���
    ATTR_FREE_TASK_REFRESH_COUNT,		//���ˢ�������
    ATTR_VITALITY,						//��һ�Ծ��
    ATTR_VITALITY_TIME,					//��Ծ��ʱ��
    ATTR_EQUIP_TOTAL,					//��װ 120
    ATTR_EQUIP_COST,					//������װЧ��
    ATTR_REPUTATION,			        // ��ѫ
    ATTR_FASHION_HIDE,                 //ʱװ����

    ATTIB_MAX,
};

enum ROLE_ATTRIBUTE { // �����е�˳��
    SEX                            = 0,
    PROFESSION                    , // ְҵ
    EXPERIENCE                    , // ����
    LEVEL                         , // �ȼ�
    TAEL                          , // ͭ��
    INGOT                         , // Ԫ��
    GIFT_MONEY                    ,
    IS_SUSPENDED_ACCOUNT          ,
    PARTNER_ID                    ,
    FAMILY_ID                     ,
    FAMILY_POSITION               ,
    CAN_BE_RAISED                 ,
    FAMILY_LEVEL                  , // ���ŵȼ�
    CAN_JOIN_FAMILY               , // ���ã�Ϊ���˰��ɹ���
    CAN_BE_TRANSMIT               ,
    PK_MODEL                      ,
    PK_VALUE                      ,
    MILITARY_STRATEGY             , // ���
    MAP_ID                        ,
    X_POS                         ,
    Y_POS                         ,
    SIEGE_REPUTATION              , // ����
    CROSS_SVR_WAR_REPUTATION      ,
    MAX_HP                        , // �������
    CUR_HP                        , // ��ǰѪ��
    ABSORB_HP                     ,
    MAX_MP                        ,
    CUR_MP                        ,
    ABSORB_MP                     ,
    ATTACK_MIN                    , // ��С����
    ATTACK_MAX                    , // ��󹥻�
    DEFENSE_MIN                   ,
    DEFENSE_MAX                   , 
    HIT_POINT                     ,
    DODGE                         ,
    STABILITY                     , // �ȶ�
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
    IGNORE_DEFENSE_RATIO          , // ���Է�������
    REBOUND_HURT_RATIO            , // �����˺�����
	EXP_COEFFICIENT				  , // ����ϵ��
    MOVE_SPEED                    ,
    ARMOR_ID                      ,
    WEAPON_ID                     ,
    MOUNTS_ID                     ,
    BACKPACK_SIZE                 ,
    WAREHOUSE_SIZE                ,
    PHYSICAL_POWER                , // ����
    FIGHTING_POWER                , // ս����
    ANGER                         ,
    CREATE_TIME                   , // ����ʱ��
    ONLINE_TIME                   , // ����ʱ��
    OFFLINE_TIME                  , // ����ʱ��
    UPDATE_TIME                   , // ���ݿ����ʱ��
    HISTORY_ONLINE                , // �ۼ�����ʱ��
	MELTING                       , // ����ֵ
	POTENTIAL_SHARE_MAX			  , // �ܼƿɷ����ķ�������������ޣ���������ʱ�䲻ͬ����̬�仯<��Ҫ����>��
	POTENTIAL_ACCUMULATION		  , // ����ۼ��ѻ�õ��ķ����������������
	ROLE_SKILL_POINT			  ,	// ���ܵ���
	POPULARITY					  ,	// ����������
	ROLE_SKILL_POINT_TIME		  , // ���ܵ�ظ�ʱ���¼
	ROLE_VIP_LEVEL				  , // ���VIP�ȼ�
	ROLE_TEAM_ID				  , // ��Ҷ���ID
	ROLE_CAMP_ID				  , // �����ӪID
	STRATEGY					  , // ı��
	STORE_WEAPON				  ,
	BOW_ID						  , // ����ID
	BOW_INDEX					  , // ��¼�Ĺ����ĸ�����
	CAVALRY_INDEX				  , // ��¼������ļ��ܸ�����
	BOW_SPEED					  , 
	EQUIP_POWER				      , // ���������˵ģ�װ����ʷ���ս��ֵ��
    ROLE_ATTRIBUTE_MAX            ,
};

enum GAME_ATTRIBUTE_E { // �����е�˳��
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
