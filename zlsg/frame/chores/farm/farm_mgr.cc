#include "farm_mgr.h"
#include "msg_handler_pool.h"

farm_mgr::farm_mgr()
{
    ctx = NULL;
    farm_service_id = 0;
}

farm_mgr::~farm_mgr()
{
    uint32 size = farms.size();
    for (uint32 i = 0; i < size; ++i){
        if (farms[i] != NULL){
            delete farms[i];
        }
    }

    farms.clear();
    ranks.clear();
}

int farm_mgr::init()
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)CONFMGR->get_farm_head();
    while (cur != NULL){
        mysql::tb_role_farm* trf = static_cast<mysql::tb_role_farm*>(cur->mutable_val().pop_msg());
        cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)cur->get_next();
        if (trf == NULL){
            continue;
        }

        farm* f = new farm;
        if (f->attach(trf) != 0){
            delete trf;
            delete f;
            continue;
        }
        delete trf;
        
        farms.push_back(f);
    }

    uint32 size = farms.size();
    farm* tmp;
    for (uint32 i = 0; i < size; ++i){
        for (uint32 j = i+1; j < size; ++j){
            if (farms[j]->get_total_score() > farms[i]->get_total_score()){
                tmp = farms[i];
                farms[i] = farms[j];
                farms[j] = tmp;
            }
        }
    }

    for (uint32 i = 0; i < size; ++i){
        ranks[farms[i]->get_role_id()] = i;
    }

    return 0;
}

const farm* farm_mgr::get(uint32 role_id) const
{
    map<uint32, uint32>::const_iterator it = ranks.find(role_id);
    if (it == ranks.end()){
        return NULL;
    }

    if (it->second >= farms.size()){
        return NULL;
    }

    return farms[it->second];
}

farm* farm_mgr::mut(uint32 role_id, uint32* rank_id)
{
    map<uint32, uint32>::iterator it = ranks.find(role_id);
    if (it == ranks.end()){
        return NULL;
    }

    if (it->second >= farms.size()){
        ranks.erase(it);
        return NULL;
    }

    if (rank_id != NULL){
        *rank_id = it->second;
    }

    return farms[it->second];
}

int farm_mgr::on_farm_info_req(client::farm_info_req& req, client::farm_info_rsp& rsp)
{
    rsp.set_ret(client::FOE_SUCCESS);
    uint32 role_id = req.svr_head().role_id();
    map<uint32, uint32>::iterator it = ranks.find(role_id);
    if (it == ranks.end()){
        return -1;
    }

    uint32 size = farms.size();
    if (it->second >= size){
        ranks.erase(it);
        return 0;
    }

    uint32 rank = it->second;
    uint32 parm = rank / 14;
    uint32 beg = parm * 14;
    uint32 end = beg + 14;
    if (end > size){
        end = size;
    }

    for (uint32 i = beg; i < end; ++i){
        const client::farm_info& fi = farms[i]->get_farm_info();
        if (fi.role_id() == 0){
            continue;
        }
        client::farm_info* tmp = rsp.add_fis();
        tmp->CopyFrom(fi);
    }
    
    return 0;
}

int farm_mgr::on_farm_record_req(client::farm_record_req& req, client::farm_record_rsp& rsp)
{
    farm* f = mut(req.svr_head().role_id());
    if (f == NULL){
        return -1;
    }

    f->fill_farm_record_rsp(rsp);

    return 0;
}

int farm_mgr::on_farm_create_req(client::farm_create_req& req, client::farm_create_rsp& rsp)
{
    uint32 role_id = req.svr_head().role_id();
    farm* f =  mut(role_id);
    if (f != NULL){
        rsp.set_ret(client::FOE_SUCCESS);
        client::farm_info* fi = rsp.mutable_fi();
        fi->CopyFrom(f->get_farm_info());
        return 0;
    }

    f = new farm;
    uint32 now = (uint32)time(NULL);
    int ret = f->create(role_id, req.advanced()?2:1, now);
    if (ret != 0){
        delete f;
        rsp.set_ret(client::FOE_SYS_ERROR);
        return -1;
    }

    uint32 size = farms.size();
    farms.push_back(f);
    ranks.insert(pair<uint32, uint32>(role_id, size));

    if (f->get_farm_info().level() == 2){
        just_farm_rank(f, size);
    }

    rsp.set_ret(client::FOE_SUCCESS);
    client::farm_info* fi = rsp.mutable_fi();
    fi->CopyFrom(f->get_farm_info());

    f->save(ctx);

    return 0;
}

int farm_mgr::on_farm_enter_req(client::farm_enter_req& req, client::farm_enter_rsp& rsp)
{
    uint32 role_id = req.role_id();
    if (role_id == 0){
        role_id = req.svr_head().role_id();
    }

    farm* f = mut(role_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_SYS_ERROR);
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    update_game_farm(now, f, true); // 更新仙园信息到游戏场景服务

    rsp.set_ret(client::FOE_SUCCESS);

    return 0;
}

int farm_mgr::on_farm_sow_req(client::farm_sow_req& req, client::farm_sow_rsp& rsp)
{
    uint32 role_id = req.svr_head().role_id();
    farm* f = mut(role_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_NO_FARM);
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    client::FARM_OPERATE_E ret = f->sow(now, req.land_id(), req.serial());
    rsp.set_ret(ret);
    if (ret == client::FOE_SUCCESS){ // 播种成功,一定是有土地信息的
        update_game_farm(now, f, false);
        client::farm_land_info* fi = f->get_land_info(req.land_id());
        if (fi != NULL){
            rsp.mutable_fli()->CopyFrom(*fi);
        }

        f->save(ctx);
    }

    return 0;
}

int farm_mgr::on_farm_steal_msg(client::farm_steal_msg& msg, client::farm_steal_rsp& rsp)
{
    uint32 farm_role_id = msg.farm_role_id();
    uint32 thief_role_id = msg.req().svr_head().role_id();
    if (farm_role_id == thief_role_id){
        rsp.set_ret(client::FOE_CANT_STEAL_SELF);
        return -1;
    }
    
    farm* f = mut(farm_role_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_NOT_EXIST_FARM);
        return -2;
    }

    uint32 now = (uint32)time(NULL);
    uint32 steal_goods_id = 0;
    uint32 steal_num = 0;
    client::FARM_OPERATE_E ret = f->steal(now, thief_role_id, msg.req().land_id(), steal_goods_id, steal_num);
    rsp.set_ret(ret);
    if (ret != client::FOE_SUCCESS){
        return -3;
    }

    client::farm_stolen_msg fsm_msg;
    fsm_msg.set_farm_role_id(farm_role_id);
    fsm_msg.set_thief_role_id(thief_role_id);
    fsm_msg.set_steal_goods_id(steal_goods_id);
    fsm_msg.set_steal_num(steal_num);
    send_game(msg.req().svr_head().scenesvr_id(), fsm_msg);

    f->save(ctx);

    return 0;
}

int farm_mgr::on_farm_gain_req(client::farm_gain_req& req, client::farm_gain_rsp& rsp)
{
    uint32 rank_id = 0;
    farm* f = mut(req.svr_head().role_id(), &rank_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_NO_FARM);
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    client::farm_gain_msg msg;
    uint32 add_score = 0;
    client::FARM_OPERATE_E ret = f->gain(req, now, msg, add_score);
    rsp.set_ret(ret);
    rsp.set_add_score(add_score);

    f->add_farm_score(add_score);
    just_farm_rank(f, rank_id);

    send_game(req.svr_head().scenesvr_id(), msg);
    
    update_game_farm(now, f, false);
    update_client_farm(req.svr_head().client_id(), now, f); 

    f->save(ctx);

    return 0;
}

void farm_mgr::just_farm_rank(farm* f, uint32 old_rank_id)
{
    if (old_rank_id == 0 || old_rank_id >= farms.size() || farms[old_rank_id] != f){
        return;
    }

    uint32 new_rank_id = old_rank_id;
    uint32 end = old_rank_id;
    uint32 front = end - 1;
    
    while (farms[front]->get_total_score() < f->get_total_score()){
        farm* tmp = farms[front];
        ranks[tmp->get_role_id()] = end; //  交换数据
        farms[end] = tmp; 

        farms[front] = f;
        new_rank_id = front;

        --end;                           

        if (front == 0){ // 已经到第一名了
            break;
        }

        --front;
    }

    if (new_rank_id != old_rank_id){
        ranks[f->get_role_id()] = new_rank_id;
    }
}

int farm_mgr::on_farm_improve_req(client::farm_improve_req& req, client::farm_improve_rsp& rsp)
{
    uint32 role_id = req.svr_head().role_id();
    farm* f = mut(role_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_NO_FARM);
        return -1;
    }

    uint32 now = (uint32)time(NULL);

    rsp.set_ret(f->improve_land(now, req.land_id(), req.type()));
    if (rsp.ret() != client::FOE_SUCCESS){
        return -2;
    }

    update_game_farm(now, f, false);
    update_client_farm(req.svr_head().client_id(), now, f); 

    return 0;
}

int farm_mgr::on_farm_gain_money_req(client::farm_gain_money_req& req, client::farm_gain_money_rsp& rsp)
{
    uint32 role_id = req.svr_head().role_id();
    farm* f = mut(role_id);
    if (f == NULL){
        rsp.set_ret(client::FOE_NO_FARM);
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    uint32 money = 0;
    rsp.set_ret(f->gain_money_tree(now, money));
    if (rsp.ret() != client::FOE_SUCCESS){
        return -2;
    }
    rsp.set_money(money);
    rsp.set_role_id(role_id);

    return 0;
}

void farm_mgr::update_game_farm(vector<farm*>& data, bool is_create)
{
    uint32 now = (uint32)time(NULL);
    uint32 size = data.size();
    for (uint32 i = 0; i < size; ++i){
        update_game_farm(now, data[i], is_create);
    }
}

void farm_mgr::update_game_farm(uint32 now, farm* f, bool is_create)
{
    f->update(now);
    if (farm_service_id == 0){
        return;   
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        return;
    }

    client::update_farm_info* ufi = new client::update_farm_info;
    ufi->set_is_create(is_create); 
    ufi->mutable_fi()->CopyFrom(f->get_farm_info());
    mh->msg = ufi;

    amtsvr_send(ctx, 0, farm_service_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

}

void farm_mgr::update_client_farm(uint32 client_id, uint32 now, farm* f)
{
    if (client_id == 0){
        return;
    }

    client::update_farm_info ufi;
    ufi.set_is_create(false);
    ufi.mutable_fi()->CopyFrom(f->get_farm_info());
    send_client(client_id, ufi);
}

void farm_mgr::send_client(uint32 client_id, const google::protobuf::Message& msg)
{
    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL){
        return;
    }
    
    amtsvr_send(ctx, 0, client_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
}

void farm_mgr::send_game(uint32 scenesvr_id, const google::protobuf::Message& msg)
{
    if (scenesvr_id == 0){
        return;
    }

    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL){
        return;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        delete new_msg;
        return;
    }
    mh->msg = new_msg;

    amtsvr_send(ctx, 0, scenesvr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}
