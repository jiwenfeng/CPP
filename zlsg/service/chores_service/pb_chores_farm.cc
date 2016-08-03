#include "chores_interface.h"

void* on_chores_pb_farm_create_req(client::farm_create_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_create_rsp rsp;
    c->fa_mgr.on_farm_create_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_info_req(client::farm_info_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_info_rsp rsp;
    c->fa_mgr.on_farm_info_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_enter_req(client::farm_enter_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_enter_rsp rsp;
    c->fa_mgr.on_farm_enter_req(*req, rsp);
    if (rsp.ret() != client::FOE_SUCCESS){
        chores_send_client(c, req->svr_head(), rsp);
        return NULL;
    }

    chores_send_game(c, req->svr_head(), *req);

    return NULL;
}

void* on_chores_pb_farm_sow_req(client::farm_sow_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_sow_rsp rsp;
    c->fa_mgr.on_farm_sow_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_steal_msg(client::farm_steal_msg* msg, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_steal_rsp rsp;
    c->fa_mgr.on_farm_steal_msg(*msg, rsp);
    chores_send_client(c, msg->req().svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_gain_req(client::farm_gain_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_gain_rsp rsp;
    c->fa_mgr.on_farm_gain_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_improve_req(client::farm_improve_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_improve_rsp rsp;
    c->fa_mgr.on_farm_improve_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_gain_money_req(client::farm_gain_money_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_gain_money_rsp rsp;
    c->fa_mgr.on_farm_gain_money_req(*req, rsp);
    if (rsp.ret() != client::FOE_SUCCESS){
        chores_send_client(c, req->svr_head(), rsp);
        return NULL;
    }
    chores_send_game(c, req->svr_head(), rsp);

    return NULL;
}

void* on_chores_pb_farm_record_req(client::farm_record_req* req, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    client::farm_record_rsp rsp;
    c->fa_mgr.on_farm_record_req(*req, rsp);
    chores_send_client(c, req->svr_head(), rsp);

    return NULL;
}
