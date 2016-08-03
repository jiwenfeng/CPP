#include "chores_interface.h"

void* on_chores_pb_scene_service_msg(server::scene_service_msg * msg, void* ud, size_t* rsp_len);

void* on_chores_pb_farm_create_req(client::farm_create_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_info_req(client::farm_info_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_enter_req(client::farm_enter_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_sow_req(client::farm_sow_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_steal_msg(client::farm_steal_msg* msg, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_gain_req(client::farm_gain_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_improve_req(client::farm_improve_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_gain_money_req(client::farm_gain_money_req* req, void* ud, size_t* rsp_len);
void* on_chores_pb_farm_record_req(client::farm_record_req* req, void* ud, size_t* rsp_len);

int chores_dispatcher_init(struct chores_service_t* c)
{
    c->pd.reg<server::scene_service_msg>("server.scene_service_msg", on_chores_pb_scene_service_msg);

    c->pd.reg<client::farm_create_req>("client.farm_create_req", on_chores_pb_farm_create_req);
    c->pd.reg<client::farm_info_req>("client.farm_info_req", on_chores_pb_farm_info_req);
    c->pd.reg<client::farm_enter_req>("client.farm_enter_req", on_chores_pb_farm_enter_req);
    c->pd.reg<client::farm_sow_req>("client.farm_sow_req", on_chores_pb_farm_sow_req);
    c->pd.reg<client::farm_steal_msg>("client.farm_steal_msg", on_chores_pb_farm_steal_msg);
    c->pd.reg<client::farm_gain_req>("client.farm_gain_req", on_chores_pb_farm_gain_req);
    c->pd.reg<client::farm_improve_req>("client.farm_improve_req", on_chores_pb_farm_improve_req);
    c->pd.reg<client::farm_gain_money_req>("client.farm_gain_money_req", on_chores_pb_farm_gain_money_req);
    c->pd.reg<client::farm_record_req>("client.farm_record_req", on_chores_pb_farm_record_req);

    return 0;
}

int chores_send_center(chores_service_t* c, const google::protobuf::Message& msg)
{
    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL){
        return -1;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        delete new_msg;
        return -2;
    }
    mh->msg = new_msg;

    return amtsvr_sendname(c->ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int chores_send_game(chores_service_t* c, uint32 scene_id, const google::protobuf::Message& msg)
{
    if (scene_id == 0){
        return -1;
    }

    map<uint32, uint32>::const_iterator it = c->gss.find(scene_id);
    if (it == c->gss.end()){
        return -2;
    }

    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL){
        return -3;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        delete new_msg;
        return -4;
    }
    mh->msg = new_msg;

    return amtsvr_send(c->ctx, 0, it->second, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int chores_send_game(chores_service_t* c, const client::server_head& svr_head, const google::protobuf::Message& msg)
{
    uint32 scenesvr_id = svr_head.scenesvr_id();
    if (scenesvr_id == 0){
        return -1;
    }

    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL){
        return -2;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        delete new_msg;
        return -3;
    }
    mh->msg = new_msg;

    return amtsvr_send(c->ctx, 0, scenesvr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

void* on_chores_pb_scene_service_msg(server::scene_service_msg * msg, void* ud, size_t* rsp_len)
{
    chores_service_t* c = (chores_service_t*)ud;
    int size = msg->ssi_size();
    for (int i = 0; i < size; ++i){
        c->gss.insert(pair<uint32,uint32>(msg->ssi(i).scene_id(), msg->ssi(i).service_id()));
        if (msg->ssi(i).scene_id() == 479){ // Å©³¡³¡¾°ID
            c->fa_mgr.attach_farm_service_id(msg->ssi(i).service_id());
        }
    }

    return NULL;
}

int chores_send_client(chores_service_t* c, const client::server_head& svr_head, const google::protobuf::Message& msg)
{
    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL){
        return -1;
    }
    return amtsvr_send(c->ctx, 0, svr_head.client_id(), PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
}
