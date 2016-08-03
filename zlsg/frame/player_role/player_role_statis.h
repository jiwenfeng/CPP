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
    PRSTE_DAY_ONCE_LIMIT				 = 1,   // ÿ��һ�������� 0 û�м�¼ 1 �м�¼  �����һλÿ�������Ƿ��ʼ��
    PRSTE_USER_STATE					 = 2,   // ����״̬ 1ɨ��
    PRSTE_ENTER_RAID_COUNT				 = 3,   // 0xFFFF0000��Ӣ����������ս�ɹ����� 0x0000FFFF��ս��ͨ����������ս�ɹ�����
    PRSTE_AUTO_RAID_COUNT				 = 4,   // ����ɨ����ս��������
	PRSTE_BLACK_REFRESH_COUNT			 = 5,   // �����̵�ˢ�´�����¼
	PRSTE_BLACK_FRIST_REFRESH			 = 6,   // �����̵��״�ˢ�¼�¼ 0 û�м�¼ 1�Ѿ�ˢ�¹�
	PRSTE_LEVELS_RESET_COUNT			 = 7,	// ����ն�����ô���
    PRSTE_SIGN_IN						 = 8,   // ǩ����¼���
    PRSTE_SIGN_IN_AWARD					 = 9,   // ǩ��������ȡ��ǣ����ã�
    PRSTE_OPEN							 = 10,  // ���ܿ��ű��
	PRSTE_ARENA_CHALLENGE_TIMES			 = 11,	// ��������ս����
	PRSTE_ARENA_RESET_CHALLENGE_TIMES	 = 12,  // ������������ս����
	PRSTE_ARENA_RECOVER_TIMES		     = 13,	// �������ָ���ս����
	PRSTE_MOUNT_FRIST_RIDE				 = 14,  // �����һ����˱��
	PRSTE_LAND_DAY						 = 15,  // ��½��������¼
	PRSTE_LAND_DAY_REWARD				 = 16,  // ��½������ȡ�ı��
	//////////////����Ŀ��ԸĶ�ʹ��/////////////////////////
    PRSTE_TOWER_LAYER           = 17,  // ������ 0xFFFF0000 ͨ���� 0x0000FFFF ͨ������
    PRSTE_DAY_VIP_TRANS         = 18,  // VIPÿ�촫�ʹ���
    PRSTE_DAY_HIDE_RAID         = 19,  // ���عؿ�32λ ����ÿλ��ʶһ���ؿ�
    PRSTE_MONTH_ONLINE          = 20,  // ÿ�������� ÿһλ����ÿһ�� 0 û���� 1����
    PRSTE_MONTH_ONLINE_AWARD    = 21,  // ÿ�����߽��� ��������λ��������콱�� 0 û�� 1 ��ȡ
    PRSTE_WEEK_CUR_ONLINE       = 22,  // ����������
    PRSTE_WEEK_LAST_ONLINE      = 23,  // ������������
    PRSTE_ONLINE_TIME           = 24,  // �չ�Ǳ��ʱ��
    PRSTE_WELFARE_GIFTS         = 25,  // �����ȡ��� ����ÿλ ����һ�(�׳� ���ֿ� �ƹ�) ��ֵ����(�׳� 1000 2500 ......)
    PRSTE_YELLOW_VIP_GIFTS      = 26,  // ������� �����һλ ������� �ڶ�λ��ʼΪ�ȼ��ɳ����
    PRSTE_WEEK_ONCE_LIMIT       = 27,  // ÿ��һ�������� �����һλ�������߽��� 0 û�� 1��ȡ �ڶ�λ ����ս��Ա����
    PRSTE_ONLINE_GIFTS_RECORD   = 28,  // ���������ȡ��¼ �����һλ1���� �� 5����....
    PRSTE_ONLINE_GIFTS_TIME     = 29,  // �������ʱ������
    PRSTE_TEST_GIFTS_RECORD     = 30,  // ��������ȡ��¼ �����һλ��½ �� 40����....
    PRSTE_TEST_GIFTS_TIME       = 31,  // ������ʱ������
    PRSTE_DAY_ACTIVITY          = 32,  // ��Ծ��
    PRSTE_DAY_ONLINE_TIME       = 33,  // ÿ������ʱ��
    PRSTE_DAY_ACTIVITY_GIFTS    = 34,  // ��Ծ�������ȡ��¼ �����һλ��ʼ 15, 50, 100 .....
    PRSTE_TARGET_ONCE_0         = 35,  // ���� 1�����ս 2ʹ��Ⱥ������ 3ʹ��ŭ������ 4������� 5ʹ����ʱ��� 6װ����ָ 7װ����� 8������帱���ɹٴ͸� 9�����������
    PRSTE_TARGET_HALF_BYTE_0    = 36,  // ���� 1��Ӻ��� 2ɱ������ 3ɱ����� 4ɱ����Ӣ 5ɱ��BOSS 6������︱�� 7����ͭǮ���� 8ѧϰ����
    PRSTE_TARGET_HALF_BYTE_1    = 37,  // ���� 1���̽�� 2���ȼ� 3����Ʒ�� 4�μӹ���ս 5�μ�����ս 6װ������ 7�ӳ�20��װ�� 8ϴ������װ��
    PRSTE_TARGET_HALF_BYTE_2    = 38,  // ���� 1��ɫװ�� 2����50��װ�� 3��ʯͷ 4��ͼ�������� 5�춼̽��
    PRSTE_TARGET_BYTE           = 39,  // ���� 1��� 2�μ�ս�� 3�������
    PRSTE_TARGET_COMPLETE_0     = 40,  // ��ɱ�� id��Ӧ����ÿλ
    PRSTE_TARGET_COMPLETE_1     = 41,
    PRSTE_TARGET_GIFTS_0        = 42,  // ��ȡ��� id��Ӧ����ÿλ
    PRSTE_TARGET_GIFTS_1        = 43,
    PRSTE_TARGET_ADD_GIFTS      = 44,  // ������ȡ��� id��Ӧ����ÿλ
    PRSTE_INVEST_RECORD         = 45,  // ���� 1��ʶ�Ƿ����Ͷ�� 2��һ����� 3�ڶ��� 4����....
    PRSTE_INVEST_JOIN_TIME      = 46,  // �μ�Ͷ��ʱ�� unixtime
    PRSTE_OPEN_SERVER_GIFTS     = 47,  // �������ֵ��ȡ��¼
    PRSTE_CONVOY                = 48,  // ÿ��3�����ˢ�ڳ��Ļ���(��1��Byte), ÿ��10�����ڳ��Ļ��ᣨ��2��Byte��
    PRSTE_DAY_BYTE_LIMIT        = 49,  // ÿ����������  0x000000FF �����±����� ..FF00 ���弼�����Կɵ����(��һ��ÿ������ռһλ) ..FF0000���弼�����Կɵ����(�ڶ���ÿ������ռһλ)
    PRSTE_DOMAIN_DAY_REWARD     = 50,  // 0x000000FF ÿ����ȡ�Ҽ����������� 0x00000F00 ������ָ��� 0x0000F000 ������ָ���
    PRSTE_FILED_SCORE           = 51,  // Ұ���������
    PRSTE_OTHER_DAY_RAID_TIMES  = 52,  // ��������ÿ��ͳ����ϻ��ȸ�������ͳ�� 0xFF ��ȡ���ڹ���� 0xFF00
    PRSTE_MATE_REWARD           = 53,  // ����ÿλ���ÿ��ȡ��һ������

	PRSTE_ARENA_CD				= 54,	// ��ȴʱ��

	PRSTE_OFFLINE_TIMELONG		= 55,	// ��������ʱ��
	PRSTE_TOTAL_OFFLINE_TL		= 56,	// ������ʱ��
	PRSTE_CYCLE_INVITE_COUNT	= 57,	// �������
    PRSTE_MAX
};

enum PLAYER_ROLE_STATIS_UPDATE_TYPE_E
{
    PRSUTE_NOT_UPDATE   = 0, // һֱ�ۼӲ�����
    PRSUTE_DAY_UPDATE   = 1, // �������ͳ����
    PRSUTE_WEEK_UPDATE  = 2, // ���ܸ���ͳ����
    PRSUTE_MONTH_UPDATE = 3, // ���¸���ͳ����
    PRSUTE_MAX
};
// ǩ��������Ӧ�ı�ʶֵsign_vale[��ǰ����]
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
