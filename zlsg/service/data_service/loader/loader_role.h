#ifndef __LOADER_ROLE_H__
#define __LOADER_ROLE_H__

#include "player_role.h"
#include "center_role.h"
#include "amtsvr.h"
#include "msg_handler_pool.h"

enum LOADER_PLAYER_STEP_E
{
    LPSE_INIT           = 0 ,
    LPSE_ROLE           = 1 ,
    LPSE_STATUS         = 2 , 
    LPSE_SKILL          = 3 ,
    LPSE_PROPS          = 4 ,
    LPSE_MAIL           = 5 ,
    LPSE_TASK           = 6 ,
    LPSE_PET            = 7 ,
    LPSE_LABEL          = 8,
    LPSE_SET            = 9 ,
    LPSE_EXPLORE        = 10,
    LPSE_EXPLORE_DETAIL = 11,
    LPSE_ROLE_SET       = 12,
    LPSE_ROLE_BUFFER    = 13,
    LPSE_ROLE_RAID      = 14,
    LPSE_ROLE_ORDER     = 15,
    LPSE_ROLE_STATIS    = 16,
    LPSE_ROLE_EXT_ATTR  = 17,
    LPSE_ROLE_MOUNTS    = 18,
	LPSE_ROLE_POTENTIAL = 19,
    LPSE_ROLE_AUTO_RAID = 20,
	LPSE_ROLE_CD_COOLDOWN = 21,
	LPSE_ROLE_TREASURE  = 22,
	LPSE_ROLE_TARGET	= 23,
	LPSE_ROLE_VIP		= 24,
	LPSE_ROLE_LOTTERY	= 25,
	LPSE_ROLE_BLACK_SHOP = 26,
	LPSE_ROLE_LEVELS	= 27,
	LPSE_ROLE_GEMS		= 28,
	LPSE_ROLE_BOWS		= 29,
	LPSE_ROLE_CYCLE		= 30,
	LPSE_ROLE_DEVIL		= 31,
    LPSE_END,        // 从DB加载数据完成
};

class loader_role
{
public:
    loader_role();

    virtual ~loader_role();

    inline bool is_loader_end() {return step >= LPSE_END;}

    inline void attact_req(google::protobuf::Message* msg)
    {
        req_list.push_back(msg);
    }

    inline vector<google::protobuf::Message*>& get_req()
    {
        return req_list;
    }

    inline player_role* pop_role()
    {
        player_role* tmp = _role;
        _role = NULL;
        return tmp;
    }

    inline center_role* pop_crole()
    {
        center_role* tmp = _crole;
        _crole = NULL;
        return tmp;
    }

    int load(struct amtsvr_context * ctx, uint32 role_id);

    int proc_sqlmgr_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);

    inline void add_step(){++step;};
    inline void end_step(){step = LPSE_END;};

protected:
    int on_player_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_role_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_status_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_skill_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_props_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_mail_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_task_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);
    int on_pet_rsp(struct amtsvr_context * ctx, struct cmq_svr_msg_t* rsp);


    int on_label_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_set_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_explore_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_explore_detail_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_set_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_buffer_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_potential_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_raid_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_order_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_statis_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_ext_attr_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_mounts_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
    int on_role_auto_raid_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_cd_cooldown_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_treasure_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_target_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_vip_rsp(struct amtsvr_context* ctx, struct cmq_svr_msg_t* rsp);
	int on_role_lottery_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	int on_role_black_shop_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	int on_role_levels_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	int on_role_gem_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	int on_role_bows_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	int on_role_cycle_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);

	int on_role_devil_rsp(struct amtsvr_context *ctx, struct cmq_svr_msg_t *rsp);
	
protected:
    player_role*                       _role;
    center_role*                       _crole;
    int                                step;
    vector<google::protobuf::Message*> req_list;
}; 

#endif
