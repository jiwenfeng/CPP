#ifndef __CONFIG_PROPS_H__
#define __CONFIG_PROPS_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

enum PROPS_BIND_TYPE {
    PBT_NOT_BIND = 0,
    PBT_BIND     = 1,
    PBT_USE_BIND = 2,
};

enum PROPS_TYPE {
    PT_MONEY		= 0,// ��Ǯ	
    PT_NORMAL		= 1,// ��ͨ��Ʒ
    PT_EQUIP		= 2,// ��ɫװ��
    PT_PET_EQUIP    = 3,// ����װ��
    PT_REEL			= 4,// ���;���
    PT_USE			= 5,// ʹ�õ���
    PT_MEDICINE		= 6,// ҩˮ
	PT_COMPOUND		= 7,// �ϳ�
};

enum PROPS_SUB_TYPE {
    PST_WEAPON   = 1,   // ����
    PST_ARMOR    = 2,   // �·�
	PST_BELT     = 3,   // ����
	PST_HELMET   = 4,   // ͷ��
	PST_NECKLACE = 5,   // ����
	PST_SHOE     = 6,   // Ь��
};
enum PICK_UP_TYPE {
	PUT_GOLD      = 1, // �Զ�ʰȡ���
	PUT_MEDICINE  = 2, // �Զ�ʰȡҩƷ
	PUT_TASK      = 3, // �Զ�ʰȡ������Ʒ
	PUT_EQUIP     = 4, // �Զ�ʰȡװ��
	PUT_MATERIAL  = 5, // �Զ�ʰȡ����
	PUT_SHOP_ITEM = 6, // �Զ�ʰȡ�̳ǵ���
	PUT_OTHER     = 7, // ������Ʒ
};

class config_props
{
public:
    config_props();
    ~config_props();

public:
    inline int                             get_item_id         () const { return _item_id         ; } 
    inline string                          get_item_name       () const { return _item_name       ; } 
    inline int                             get_item_type       () const { return _item_type       ; } 
    inline int                             get_item_sub_type   () const { return _item_sub_type   ; } 
    inline int                             get_bind_type       () const { return _bind_type       ; } 
    inline int                             get_is_get_use      () const { return _is_get_use      ; } 
    inline int                             get_pick_range      () const { return _pick_range      ; } 
    inline int                             get_total           () const { return _total           ; } 
    inline int                             get_need_job        () const { return _need_job        ; } 
    inline int                             get_need_level      () const { return _need_level      ; } 
    inline int                             get_cool_group_id   () const { return _cool_group_id   ; } 
    inline int                             get_cool_group_tick () const { return _cool_group_tick ; } 
    inline int                             get_buy_price       () const { return _buy_price       ; } 
    inline int                             get_is_shop_recover () const { return _is_shop_recover ; } 
    inline int                             get_sales_price     () const { return _sales_price     ; } 
    inline int                             get_is_shop_confirm () const { return _is_shop_confirm ; } 
    inline int                             get_can_repair      () const { return _can_repair      ; } 
    inline int                             get_repair_price    () const { return _repair_price    ; } 
    inline int                             get_can_discard     () const { return _can_discard     ; } 
    inline int                             get_discard_confirm () const { return _discard_confirm ; } 
    inline int                             get_can_inlay       () const { return _can_inlay       ; } 
    inline int                             get_can_synthesis   () const { return _can_synthesis   ; } 
    inline int                             get_durable_max     () const { return _durable_max     ; } 
    inline int                             get_increase_max    () const { return _increase_max    ; } 
    inline const std::map<int, int>&       get_game_attribute  () const { return _game_attribute  ; } 
    inline const std::map<int, cpf_val_3>& get_random_attribute() const { return _random_attribute; } 
    inline int                             get_affect_hp       () const { return _affect_hp       ; } 
    inline int                             get_affect_mp       () const { return _affect_mp       ; } 
    inline int                             get_can_shortcut    () const { return _can_shortcut    ; } 
    inline int                             get_name_color_index() const { return _name_color_index; } 
    inline string                          get_describes       () const { return _describes       ; } 
    inline int                             get_pickup_type     () const { return _pickup_type     ; } 
    inline int                             get_lotuser_count   () const { return _lotuser_count   ; }
	inline const std::vector<int>&         get_trigger_list	() const { return _trigger_list    ; }
	inline int                             get_exceed_limit    () const { return _exceed_limit    ; }
	inline const std::map<int, succinct_info>&   get_succinct_need_material  () const { return _succinct_need_material  ; } 
	inline int                             get_smelting_coefficient    () const { return _smelting_coefficient    ; }
	inline int                             get_basic_upgrade			() const { return _basic_upgrade		   ; }
	inline int                             get_upgrade_cost			() const { return _upgrade_cost            ; }
	inline int                             get_start_upgrade_cost		() const { return _start_upgrade_cost      ; }
public:
    inline bool is_equip() const { return get_item_type() == PT_EQUIP; }
	inline bool is_pet_equip() const { return get_item_type() == PT_PET_EQUIP;}
    inline bool is_lnlaid_stone() const { return false; }
    inline uint32 get_quality() const { return get_name_color_index(); }
public:
    int                       _item_id         ; // ��ƷID
    string                    _item_name       ; // ��������
    int                       _item_type       ; // ��������
    int                       _item_sub_type   ; // ����������
    int                       _bind_type       ; // �����ͣ�0���󶨣�1ʰȡ�󶨣�2,ʹ�ú�󶨣�
    int                       _is_get_use      ; // ���ʱ�Ƿ�����ʹ��(0,1)
    int                       _pick_range      ; // ʰȡ/������Χ(���7)��
    int                       _total           ; // ������
    int                       _need_job        ; // ����ְҵ
    int                       _need_level      ; // ����ȼ�
    int                       _cool_group_id   ; // ������ȴ�飨1~64��
    int                       _cool_group_tick ; // ������ȴʱ�䣨��λ�����룩
    int                       _buy_price       ; // �̵��еĹ���۸�
    int                       _is_shop_recover ; // �̵��Ƿ����(0,1)
    int                       _sales_price     ; // �̵���ռ۸�
    int                       _is_shop_confirm ; // ����ʱ�Ƿ񵯳�ȷ�ϣ�0��1�ǣ�
    int                       _can_repair      ; // �Ƿ�������0��1�ǣ�
    int                       _repair_price    ; // ����۸�
    int                       _can_discard     ; // �Ƿ�ɶ�����0��1,�ǣ�
    int                       _discard_confirm ; // �����Ƿ񵯳�ȷ��
    int                       _can_inlay       ; // �Ƿ�����Ƕ��Ʒ��0��1�ǣ�
    int                       _can_synthesis   ; // �ܷ�ϳ�(0=��1=��)
    int                       _durable_max     ; // ����;�
    int                       _increase_max    ; // ����ǿ���ȼ�
    std::map<int, int>        _game_attribute  ; // ��Ϸ�����б�����|ֵ/Ratio<Max|Job(0ͨ��)��
    std::map<int, cpf_val_3>  _random_attribute; // �����������(����|����,��С/���;)
    int                       _affect_hp       ; // Ӱ������(����д�̶���ֵ������)
    int                       _affect_mp       ; // Ӱ��ħ��(����д�̶���ֵ������)
    int                       _can_shortcut    ; // �Ƿ����������
    int                       _name_color_index; // ��Ʒ������ɫ������0��ɫ��1��ɫ��2��ɫ��3��ɫ��4��ɫ��
    string                    _describes       ; // ��Ʒ������Ϣ
    int                       _pickup_type     ; // ����ʰȡ���ͣ�ʰȡ��Ʒ����1���Զ�ʰȡ��ң�2���Զ�ʰȡҩƷ��3���Զ�ʰȡ������Ʒ��4���Զ�ʰȡװ����5���Զ�ʰȡ���ϣ�6���Զ�ʰȡ�̳ǵ��ߣ�7=������Ʒ��
    int                       _lotuser_count   ; // ����ʹ�������
	std::vector<int>		  _trigger_list	   ; // ��ƷЧ��ID�б�
	int						  _exceed_limit	   ; // ���������Ƿ�����ʹ�ã���1����2��
	std::map<int, succinct_info>	  _succinct_need_material; // ϴ�����Ĳ������У���ʽ������ID|����;����ID_����;��
	int					      _smelting_coefficient; // װ������ϵ��(��ֵ����ɰٷֱȣ�
	int                       _basic_upgrade	;  // װ���ɳ�ֵ
	int                       _upgrade_cost     ;  // ǿ�����Ļ���
	int						  _start_upgrade_cost; // �������Ļ���
};

class config_props_mgr : public config_base_mgr
{
public:
    config_props_mgr(TTableGroup_s* p = NULL);
    ~config_props_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_props* get_config_props(uint32 props_id) const;
	const string get_prop_name(uint32 prop_id) const ;

private:
    TTableGroup_s*                     _props;
    TTable*                          _table;
    std::map<uint32, config_props*>  _cp;
};

#endif // __CONFIG_PROPS_H__
