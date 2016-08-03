#ifndef __PROPS_PARSER_H__
#define __PROPS_PARSER_H__

#include "comm_define.h"

#define MAX_PROPS_BOLB_LEN  40960 // ��Ʒ������󳤶� 
#define MAX_GRID_NUM        320   // ��������
// ���ϱ���
#define GRID_INDEX_PACK     1000  // ����������������
#define PACK_GRID_BEG       1001  // ������ʼ���
#define MAX_PACK_SIZE       70   // ����������
// װ������
#define GRID_INDEX_DEPOT    500	  // װ������������������
#define DEPOT_GRID_BEG      501   // װ��������ʼ���
#define MAX_DEPOT_SIZE      70   // װ������������
// װ���ĸ���
#define GRID_INDEX_EQUIP    0     // װ����������
#define EQUIP_GRID_BEG      1     // װ����ʼ��ţ�����������װ��һ����1 + pet_id * 10(��ǰ���˲�λ����)
#define EQUIP_ARMOR         2     // װ���·�
#define EQUIP_BELT          3     // ����
#define EQUIP_HELMET        4     // ͷ��
#define EQUIP_NECKLACE      5     // ����
#define EQUIP_SHOE          6     // Ь��
#define MAX_EQUIP_SIZE      140   // ���װ����������
// ��ʱ����
#define GRID_INDEX_EXPLORE  1500  // ̽�ղֿ������������
#define EXPLORE_GRID_BEG    1501  // ̽�ղֿ⿪ʼ������
#define MAX_EXPLORE_SIZE    300   // ̽�ղֿ�����������

#define GRID_INDEX_TASK     2000  // ���񱳰�������������
#define TASK_GRID_BEG       2001  // ���񱳰�
#define MAX_TASK_SIZE       64    // ���񱳰����ֵ
class props_smyid
{
public:
	props_smyid() {
		beg_index    = 0;
		props_smy_id = 0;
	}

	virtual ~props_smyid() {

	}

	inline void set(uint32 sid, uint32 psid) {
		beg_index    = sid;
		props_smy_id = psid;
	}

	inline uint32 get() {
		uint32 id = props_smy_id + 1;
		if (id == 0 || id > PROPS_SMY_ID_MASK) {
			return 0;
		}

		++props_smy_id;

		return PACK_SVR_PROP_ID(beg_index, props_smy_id);
	}

	inline uint32 get_props_smy_id() const { return props_smy_id; }
private:
	uint32 beg_index;
	uint32 props_smy_id;
};

class grid_role_props : public client::props_info
{
public:
    grid_role_props(){}
    virtual ~grid_role_props(){}

public:
    inline grid_role_props& operator=(const grid_role_props& rhs)
    {
        uint32 index = grid_index();
        client::props_info::CopyFrom(rhs);
        set_grid_index(index);

        return *this;
    }

    inline grid_role_props& operator=(const client::props_info& rhs)
    {
        uint32 index = grid_index();
        client::props_info::CopyFrom(rhs);
        set_grid_index(index);

        return *this;
    }

public:
    inline bool has_props() const { return props_id() > 0 && count() > 0; }

    inline int pop_props(client::props_info& rg, uint32 num)
    {
        if (!has_props() || count() < num) {
            return -1;
        }

        uint32 rest_num = count() - num;
        if (num == 0 || rest_num == 0) {
            rg.CopyFrom(*this);
            set_count(0);
        } else {
            rg.CopyFrom(*this);
            rg.set_count(num);
            set_count(rest_num);
        }

        return 0;
    }

    inline int remove_props(uint32 num, uint32& props_id, uint32* binding)
    {
        if (!has_props() || count() < num) {
            return -1;
        }

        set_count(count() - num);
        if (binding != NULL) {
            *binding = client::props_info::bind();
        }

        props_id = client::props_info::props_id();

		if (count() == num){
			props_id = 0;
		}
		
        return 0;
    }

    inline int get_props_num(uint32 props_id) const
    {
        if (!has_props() || client::props_info::props_id() != props_id) {
            return 0;
        }

		// ������
		if (client::props_info::has_is_due()){
			if (client::props_info::is_due()){
				return 0;
			}
		}
		
        return count();
    }

    inline void reset()
    {
        uint32 index = grid_index();
        Clear();
        client::props_info::set_props_id(0);
        set_grid_index(index);
    }

};

class props_parser
{
public: // ��ʼ����DB��ط���
    props_parser(uint32 index);
    virtual ~props_parser();

public:
    int resize(uint32 max_size);

    int parse_props(const string& in); // ��DB�е����ݣ���������Ʒ

    int serialize_props(string& out);  // ����Ʒ���л���������

public: //��������
    uint32 get_free_space();
    uint32 get_use_space();

    int add_props(uint32 props_id, uint32 number,vector<client::props_info>& vec_prop,uint32 quality = 0, uint32 binding = 0, uint32 profession = 0);

    int add_props(vector<client::props_info>& vpi, uint32& count,vector<client::props_info>& vec_prop);

    int add_props(const client::props_info& pi);
	// ���ص��ǵ�ǰ��������
	int add_props_equip(const client::props_info& pi);

    int add_cast_props(client::props_info& pi);

    int add_props(client::props_info& rg, uint32 grid_index);

    int remove_props(uint32 props_id, uint32 num);

    int remove_props(uint32 props_id, uint32 num, bool& bind);

    int remove_props(uint32 grid_index, uint32 num, uint32& props_id, uint32* binding = NULL);

    int pop_props(uint32 grid_index, client::props_info& rg, uint32 num = 0);

    int move_props(uint32 src_grid, uint32 dst_grid, uint32 num = 0);
    // װ������
    int upgrade_equip();
    // װ��ǿ��
    bool strengthen_equip(uint32 old_level,uint32 new_level,uint32 grid_index);
    // ������
    int strengthen_cast_equip(client::props_info& props_info,const config_upequip* upequip_info);

    const grid_role_props* get_props(uint32 grid_index) const;

    grid_role_props* mutable_props(uint32 grid_index);

    const uint32 get_props_num(uint32 props_id) const;

	const uint32 get_index_props_num(uint32 grindex_id) const;

    void sort();

    bool is_full();
	// �����ʱ�����Ƿ�������Ʒ �У���
	bool is_has_prop();

    bool is_bind(uint32 grid_index);

    // �Ƿ�����װ������
    inline bool is_depot_pack(){return beg_index >= DEPOT_GRID_BEG && beg_index <= (DEPOT_GRID_BEG + props_list.size());}

    // �Ƿ�������
    inline bool is_equip_pack() {return beg_index >= GRID_INDEX_EQUIP && beg_index <= (GRID_INDEX_EQUIP + props_list.size());}

    inline bool is_task_pack(){return (beg_index >= TASK_GRID_BEG && beg_index <= ( TASK_GRID_BEG + props_list.size()));}

    void grid_change_flag(uint32 grid_index);

    inline uint32 get_beg_index() {return beg_index;};

	inline uint32 get_pack_size(){return props_list.size();};

	bool check_props_time();

	bool compare_props_time(uint32& need_count,const std::vector<int>& sub_item_id,std::map<uint32 ,uint32>& expend_map_);

	float calculate_equip_power(grid_role_props& grid_prop);

	float calcula_depot_power(uint32 gr_index);
public:
    int makeup_props(uint32 props_id, uint32 number, uint32 quality, uint32 binding, const config_props& cg, client::props_info& rg);

    int makeup_props(const client::props_info& pi, const config_props& cg, client::props_info& rg);

public: // Э�鷽��
#ifndef __LIB_MINI_SERVER__
    void fill_role_props(inner::backpackGoods* data) const;
#else
    void set_props(std::vector<client::props_info>& pis);
#endif // __LIB_MINI_SERVER__

    void fill_all_props_info(client::role_props_msg& rpm);
    void fill_change_props_info(client::role_props_msg& rpm); 
    void fill_single_props_info(client::role_props_msg& rpm, uint32 grid_index);
	void fill_interval_props_info(client::role_query_info& query_info,uint32 grindex);
	void fill_pet_props_info(client::pet_query_info& query_info,uint32 grindex);
protected:
    bool same_props(const client::props_info& src, const client::props_info& dst);

    inline bool can_overlaped(client::props_info& src, const client::props_info& dst, const config_props* cp);

protected:

    int add(client::props_info& dst, const config_props* cp);
    int add_ex(client::props_info& dst, const config_props* cp);

    int overlaped(uint32 index, client::props_info& src, client::props_info& dst, const config_props* cp, uint32 num = 0);

    void clear();

    void fill_props_info(const client::props_info& rg, client::props_info& pi);

    void fill_null_props_info(client::props_info& pi, uint32 grid_index);

    client::props_info_POS get_pos();

	int set_props_smy_id(client::props_info& item_info);

	bool generate_props_smy_id(uint32& psid);
public: // test
    void remove_all();

protected:
    uint32 beg_index;
    bitset<MAX_GRID_NUM>         change_flag;
    std::vector<grid_role_props> props_list;
    uint32                       sort_time;
	map<uint32,props_smyid>      prop_smy_id; // key:beg_index
};

#endif // __PROPS_PARSER_H__

