#ifndef __PLAYER_ROLE_H__
#define __PLAYER_ROLE_H__

#include "comm_define.h"

namespace client {
    class create_role_req;
}

class player_role_status      ;
class player_role_skill_mgr   ;
class player_role_raid_mgr    ;
class player_role_auto_raid   ;
class player_role_auto_raid   ;
class player_role_props       ;
class player_role_mail        ;
class player_role_task        ;
class player_role_label_mgr   ;
#ifndef __LIB_MINI_SERVER__
class player_role_vip         ;
#endif
class player_role_explore     ;
class player_role_statis_mgr  ;
class player_role_set         ;
class player_role_buffer      ;
class player_role_order_mgr   ;
class player_role_ext_attr_mgr;
class player_role_mounts      ;
class operate_log             ;
class player_role_potential   ;
class player_role_pet         ;
class player_role_cd_cooldown ;
class player_role_treasure    ;
#ifndef __LIB_MINI_SERVER__
class player_role_mission	  ;			// 日常 成就
class player_role_lottery	  ;			// 开宝箱
#endif
class player_role_black_shop  ;
class player_role_levels	  ;
class player_role_gem		  ;
class player_role_bows		  ;
#ifndef __LIB_MINI_SERVER__
class player_role_cycle		  ;
class player_role_devil		  ;
#endif

class player_role : public pb_writer
{
public:
    player_role(mysql::tb_player_role* r = NULL);
    virtual ~player_role();

public:
    inline mysql::tb_player_role* get_db_player_role() { return _db_player_role; }
    void write(amtsvr_context* ctx);
    void write_all(amtsvr_context* ctx);

    int create_new_player_role(client::create_role_req* req, const config_birth* cb, uint32 role_id, uint32 test_reward = 0);

    inline player_role_status&       get_role_status             () { return *_status         ; }
    inline player_role_skill_mgr&    get_role_skill_mgr          () { return *_skill_mgr      ; }
    inline player_role_task&         get_tasks                   () { return *_tasks          ; }
    inline player_role_props&        get_role_props              () { return *_items          ; }
    inline player_role_mail&         get_role_mails              () { return *_mails          ; }
    inline player_role_label_mgr&    get_role_label_mgr          () { return *_label_mgr      ; }
    inline player_role_explore&      get_role_explore            () { return *_explore        ; }
    inline player_role_raid_mgr&     get_raid_mgr                () { return *_raid_mgr       ; }
    inline player_role_auto_raid*    get_auto_raid               () { return new_auto_raid()  ; }
    inline player_role_statis_mgr&   get_role_statis_mgr         () { return *_statis_mgr     ; }
    inline player_role_set&          get_set                     () { return *_set            ; }
    inline player_role_buffer&       get_buffer                  () { return *_buffers        ; }
    inline player_role_order_mgr&    get_order_mgr               () { return *_order_mgr      ; }
    inline operate_log&              get_logs                    () { return *_logs           ; }
    inline player_role_ext_attr_mgr& get_player_role_ext_attr_mgr() { return *_ext_attr_mgr   ; }
    inline player_role_mounts&       get_mounts                  () { return *_mounts         ; }
    inline player_role_potential&    get_potentials				 () { return *_potentials     ; }
    inline player_role_pet&			 get_pets					 () { return *_pets           ; }
    inline player_role_cd_cooldown&	 get_role_cds				 () { return *_cds            ; }
    inline player_role_treasure&	 get_role_treasures			 () { return *_treasures      ; }

#ifndef __LIB_MINI_SERVER__
    inline player_role_mission &	 get_player_role_target	     () { return *_target		  ; }
	inline player_role_vip &		 get_player_role_vip		 ()	{ return *_vip			  ; }
	inline player_role_lottery &	 get_player_role_lottery	 () { return *_lottery		  ; }
#endif
	inline player_role_black_shop &	 get_player_role_black_shop	 () { return *_black_shop	  ; }
	inline player_role_levels &		 get_player_role_levels		 () { return *_levels		  ; }
	void reset_role_levels();
	inline player_role_gem &		 get_player_role_gems		 () { return *_gems			  ; }
	inline player_role_bows &		 get_player_role_bows		 () { return *_bows			  ; }
#ifndef __LIB_MINI_SERVER__
	inline player_role_cycle &		 get_role_cycle() { return *_cycle; }
	void init_role_cycle();
	void init_role_devil();
#endif
	void							 update_role_levels_info(uint32 role_id, uint32 lv, uint32 power);
	void							 update_role_levels_info();
	void							 init_role_levels();
#ifndef __LIB_MINI_SERVER__
	uint32							 get_lottery_prop_num(uint32 lottery_id);
	void							 modify_lottery_time(uint32 type, uint32 role_id);
	void							 modify_used_num(uint32 num = 1);
	uint32							 get_used_num();
	uint32							 get_lottery_time(uint32 type);
	void							 add_lottery_prop_num(uint32 prop_id, uint32 num = 1);

	inline player_role_devil &	get_role_devil() { return *_devil; }
#endif
    void set_online_time (uint32 t);
    void set_offline_time(uint32 t);
    void set_skill_point_time(uint32 t);
    void add_history_online(uint32 t);
    inline uint32 get_online_time() { return _db_player_role->online_time(); }
    inline  uint32 get_offline_time() { return _db_player_role->offline_time(); }
    inline  uint32 get_history_time() { return _db_player_role->history_online(); }
    uint32 get_role_id() const {
        if (_db_player_role == NULL) {
            return 0;
        }
        return _db_player_role->role_id();
    }

    uint32 get_player_id() const {
        if (_db_player_role == NULL) {
            return 0;
        }
        return _db_player_role->player_id();
    }

    uint32 get_family_id() const {
        if (_db_player_role == NULL)
        {
            return 0;
        }
        return _db_player_role->family_id();
    }

    inline void   set_team_id(uint32 id) { _team_id = id; }
    inline uint32 get_team_id() const    { return _team_id; }
    inline client::role_attr& get_query_attr() { return _query_attr; }

    void set_ban(uint32 type, uint32 end_time);

    const std::string& account() const { return _account; }
    void account(const char* val) { _account = val; }
    void account(const std::string& val) { _account = val; }
    const std::string& sid() const { return _sid; }

public:
    void init_role_attr(client::role_attr& ra);

#ifndef __LIB_MINI_SERVER__
    void fill_role_goods(inner::backpackGoods* data) const;
    void update_role_goods(inner::backpackGoods* update);
#endif // __LIB_MINI_SERVER__

    int  add_ingot(uint32 ingot);

    void init_original_attr(client::role_attr& cal_attr);
    void calc_role_attr(client::role_attr& cal_attr);
    void upgrade_attr(uint32 level, uint64 experience);
	void get_query_role_info(client::role_query_info& query_info);
	void get_query_pet_info(client::pet_query_info& query_info);
	void get_query_bow_info(client::bows_query_info& query_info);
	void get_query_gem_info(client::gem_query_info& query_info);

public:
    int attach_role_props(int pb_count, cmq_svr_pb_ctx* pb_ctx);
    int attach_role_auto_raid(int pb_count, cmq_svr_pb_ctx* pb_ctx);

public:
    player_role_auto_raid* new_auto_raid();

public:
#ifndef __LIB_MINI_SERVER__
	int set_vip_lv(uint32 lv, bool month);
#endif

protected:
    int init_ext(mysql::tb_player_role* db_player_role);
    int init_once(mysql::tb_player_role* db_player_role);

protected:
    int create_db_player_role(client::create_role_req* req, const config_birth* cb, uint32 role_id, uint32 now);
    int create_new_role_item(const config_birth* cb, uint32 backp_size, uint32 depot_size);
    int create_new_role_skill(const config_birth* cb, uint32 role_id);
    int create_new_role_status(const config_birth* cb, uint32 role_id, uint32 test_reward);

private:
    void set_attr(const std::map<int, int>& a);
    void add_attr(const std::map<int, int>& a);
	void add_mulimap_attr(const std::multimap<int, int>& a);
    void add_attr(const client::role_attr& attr);
    void set_attr(const client::role_attr& attr);
    void add_attr();
    void add_attr_props();
    void add_attr_mounts();
    void add_attr_potential();
    void add_attr_pet();
    void add_attr_skill();
	void add_attr_gem();
	void add_attr_bows();
    void add_attr_ext();
    void add_attr_family(uint32 family_id,uint32 family_level);

public:
    static const uint32 ROLE_BAN_NULL  = 0;
    static const uint32 ROLE_BAN_LOGIN = 1;
    static const uint32 ROLE_BAN_CHAT  = 2;

protected:
    mysql::tb_player_role* _db_player_role; 
    uint32                 _team_id;
    client::role_attr      _query_attr;
    std::string            _account;
    std::string            _sid;

protected:
    player_role_status       * _status      ;
    player_role_skill_mgr    * _skill_mgr   ;
    player_role_raid_mgr     * _raid_mgr    ;
    player_role_auto_raid    * _auto_raid   ;
    player_role_props        * _items       ;
    player_role_mail         * _mails       ;
    player_role_task         * _tasks       ;
    player_role_label_mgr    * _label_mgr   ;
#ifndef __LIB_MINI_SERVER__
    player_role_vip          * _vip         ;
#endif
    player_role_explore      * _explore     ;
    player_role_statis_mgr   * _statis_mgr  ;
    player_role_set          * _set         ;
    player_role_buffer       * _buffers     ;
    player_role_order_mgr    * _order_mgr   ;
    player_role_ext_attr_mgr * _ext_attr_mgr;
    player_role_mounts       * _mounts      ;
    operate_log              * _logs        ;
    player_role_potential    * _potentials  ;
    player_role_pet			 * _pets        ;
    player_role_cd_cooldown  * _cds         ;
    player_role_treasure	 * _treasures   ;
#ifndef __LIB_MINI_SERVER__
    player_role_mission		 * _target		;
	player_role_lottery		 * _lottery		;
#endif
	player_role_black_shop   * _black_shop	;
	player_role_levels		 * _levels		;
	player_role_gem			 * _gems		;
	player_role_bows         * _bows	    ;
#ifndef __LIB_MINI_SERVER__
	player_role_cycle		 * _cycle		;
	player_role_devil		 * _devil		;
#endif
};

#endif
