#ifndef __FARM_MGR_H__
#define __FARM_MGR_H__

#include "farm.h"

class farm_mgr
{
public:
    farm_mgr();

    virtual ~farm_mgr();

public:
    
    int init();

    const farm* get(uint32 role_id) const;

    farm* mut(uint32 role_id, uint32* rank_id = NULL);

    int on_farm_info_req(client::farm_info_req& req, client::farm_info_rsp& rsp);

    int on_farm_record_req(client::farm_record_req& req, client::farm_record_rsp& rsp);

    int on_farm_create_req(client::farm_create_req& req, client::farm_create_rsp& rsp);

    int on_farm_enter_req(client::farm_enter_req& req, client::farm_enter_rsp& rsp);

    int on_farm_sow_req(client::farm_sow_req& req, client::farm_sow_rsp& rsp);

    int on_farm_steal_msg(client::farm_steal_msg& msg, client::farm_steal_rsp& rsp);

    int on_farm_gain_req(client::farm_gain_req& req, client::farm_gain_rsp& rsp);

    int on_farm_improve_req(client::farm_improve_req& req, client::farm_improve_rsp& rsp);

    int on_farm_gain_money_req(client::farm_gain_money_req& req, client::farm_gain_money_rsp& rsp);

    inline void attach_ctx( amtsvr_context* p) {ctx = p;}

    inline void attach_farm_service_id(uint32 id) {farm_service_id = id;}

private:
    // 将此处数据同步到game
    void update_game_farm(vector<farm*>& data, bool is_create);

    void update_game_farm(uint32 now, farm* f, bool is_create);

    void update_client_farm(uint32 client_id, uint32 now, farm* f);

    void send_client(uint32 client_id, const google::protobuf::Message& msg);

    void send_game(uint32 scenesvr_id, const google::protobuf::Message& msg);

    void just_farm_rank(farm* f, uint32 old_rank_id);

private:
    amtsvr_context* ctx;
    vector<farm*> farms;       // 农场信息
    map<uint32, uint32> ranks; // 角色id-->到排名
    uint32 farm_service_id;
};

#endif
