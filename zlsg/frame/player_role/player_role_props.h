#ifndef __ROLE_PROPS_H__
#define __ROLE_PROPS_H__

#include "props_parser.h"

enum PROPS_SITE_E
{
    GSE_PACK    = 0, // ����
    GSE_DEPOT   = 1, // װ������
    GSE_EQUIP   = 2, // ����
    GSE_EXPLORE = 3, // ��ʱ����
    GSE_TASK    = 4, // ���񱳰�
};

#define MIN_GRID 1001
#define MAX_GRID 1008
#define ONE_KEY_EQUIP 8
#define ONE_KEY_EQUIP_SUBLIME 4
#define DEFAULT_PACK_INIT_SIZE 64
//#define DEFAULT_PACK_INIT_SIZE 100 // ������ ��ɲ��Ժ���Ҫ������һ��
#define DEFAULT_DEPOT_INIT_SIZE 32

#define TEMP_WING_ID 2711
class scene_user;
class player_role_props : public pb_writer
{
public:
    player_role_props();
    virtual ~player_role_props();

public:
    int attach_role_props(uint32 backp_size, uint32 deopt_size, int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int init(uint32 pack_size, uint32 depot_size);
    int init(mysql::tb_role_props* db, uint32 pack_size, uint32 depot_size);

    int save_props(amtsvr_context* ctx, uint32 role_id);

public: 
    props_parser& get_pack()   { return pack   ; } 
    props_parser& get_depot()  { return depot  ; }
    props_parser& get_equip()  { return equip  ; }
    props_parser& get_explore(){ return explore; }
    props_parser& get_task()   { return task   ; }

    props_parser* get_use_src(uint32 grid_index, bool& use_task_prop);

#ifndef __LIB_MINI_SERVER__
    void update_role_props(inner::backpackGoods* data);
    void fill_role_props(inner::backpackGoods* data) const;
#endif // __LIB_MINI_SERVER__

    void fill_all_props_info(client::role_props_msg& rpm);
    void fill_equip_info(client::role_props_msg& rpm);
    void fill_change_props_info(client::role_props_msg& rpm);

    int move_props(uint32 src_grid, uint32 dst_grid, uint32 num = 0);

    int remove_props(uint32 grid, uint32 num = 1);
	// ��ʱ������Ʒ�ŵ�������
    int move_explore_to_bag();

	int add_prop_to_explore(vector<client::props_info> vec_prop);

    uint32 get_prop_id(uint32 grid_index);
    
    client::props_info* get_prop(uint32 grid_index);
    
    inline bool is_task_pack(uint32 grid_index){return (grid_index >= TASK_GRID_BEG && grid_index <= ( TASK_GRID_BEG + task.get_pack_size()));}

    void resize(uint32 pack_size, uint32 depot_size);

	bool check_explore_had_prop();

	bool is_can_add_prop();

	bool is_can_melting();
public: // װ�����
    // ������Ϣ
    uint32 get_equip_id(uint32 pos);
    uint32 get_wing_id();
    uint32 get_weapon_id();
    uint32 get_suit_id();
    uint32 get_wing_skill(uint32& id, uint32& lv);
    uint32 get_wing_lvl();
    
    // װ����ʱ
    bool sub_active_second(uint32 sec);
    bool check_equip_timeout();
    bool special_treatment(client::props_info& rg);
    
    // ���Ϊ�޸�
    void change_flag(uint32 grid_index);

    // TODO: ʵ���д���
    uint32 get_equip_code(uint32 grid_index);

    // ���ָ�����ӵĵ���
    client::props_info* mutable_props(uint32 grid_index);

    // ���
    bool check_equip(client::props_info* src);
    bool check_lnlaid_stone(client::props_info* src);

public:
    props_parser* get_pos_parser(uint32 grid_index);

private:
    void test();

protected:
    props_parser pack;			// ���ϱ���
    props_parser depot;			// װ������
    props_parser equip;		    // װ��
    props_parser explore;	    // ��ʱ���� 
    props_parser task;

    mysql::tb_role_props* db_props;

private:
	// ϴ����ʱ��������
	client::props_info* _item_info;
};

#endif
