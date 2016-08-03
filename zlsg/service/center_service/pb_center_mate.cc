#include "center_interface.h"

void sys_msg_mate(center_service_t* w, uint32 type,
                  const string& n1, uint32 id1,
                  const string& n2, uint32 id2)
{
    uint32 msg_id = 0;
    switch (type) {
    case 1: msg_id = 324; break;
    case 2: msg_id = 325; break;
    case 3: msg_id = 326; break;
    default: return ;
    }
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%s|%u", n1.c_str(), id1, n2.c_str(), id2);
    client::chat_msg cm;
    cm.set_src_rold_id(msg_id);
    cm.set_content(cmd);
    cm.set_channel(client::chat_msg_CHANNEL_SYSTEM);
    w->oi_mgr.brodcast(w->ctx, cm);
}

void* on_center_pb_mate_info_req(client::mate_info_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = req->svr_head().role_id();
    client::mate_info_rsp rsp;
    rsp.set_mate_role_id(0);
    w->mm_mgr.fill_mate_info_rsp(role_id, rsp);
   
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

// A组B
void* on_center_pb_mate_invite_req(client::mate_invite_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    client::mate_invite_rsp rsp;
    uint32 role_id = req->svr_head().role_id();
    uint32 mate_id = req->mate_role_id();
    const online_role* oi = w->oi_mgr.get_online_info(mate_id);
    if (oi == NULL){
        rsp.set_ret(client::MORE_MATE_OFFLINE);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    uint32 degree = w->fm_mgr.get_degree(role_id, mate_id);
    if (degree < 200){ // 亲密度200才允许结成小伙伴
        rsp.set_ret(client::MORE_INTIMACY_LIMIT);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }
    
    mate* src = NULL;
    mate* dst = NULL;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.can_mate_with_role(role_id, src, mate_id, dst);
    if (ret != client::MORE_SUCCESS){
        rsp.set_ret(ret);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }


    req->set_mate_role_id(role_id);
    req->clear_svr_head();
    center_send_client(w, *oi, *req);

    return NULL;
}

// B答复A
void* on_center_pb_mate_invite_rsp(client::mate_invite_rsp* rsp, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = rsp->svr_head().role_id();
    client::mate_invite_rsp result;
    uint32 mate_id = rsp->mate_role_id();
    const online_role* oi = w->oi_mgr.get_online_info(mate_id);
    if (oi == NULL){
        result.set_ret(client::MORE_MATE_OFFLINE);
        center_send_client(w, rsp->svr_head(), result);
        return NULL;
    }

    if (rsp->ret() == client::MORE_MATE_DENEY) {
        result.set_ret(client::MORE_MATE_DENEY);
        center_send_client_by_role_id(w, mate_id, result);
        return NULL;
    }

    mate* src = NULL;
    mate* dst = NULL;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.can_mate_with_role(role_id, src, mate_id, dst);
    if (ret != client::MORE_SUCCESS){
        result.set_ret(ret);
        center_send_client(w, rsp->svr_head(), result);
        return NULL;
    }

    client::mate_create_msg msg;
    msg.set_role_id(mate_id);
    msg.set_mate_role_id(role_id);
    msg.set_ret(client::MORE_SUCCESS);
    center_send_scene(w, *oi, msg);

    return NULL;
}

// 扣除完A的道具和B结成小伙伴
void* on_center_pb_mate_create_msg(client::mate_create_msg* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = msg->role_id();
    uint32 mate_id = msg->mate_role_id();
    client::mate_invite_rsp rsp;
    if (msg->ret() != client::MORE_SUCCESS){
        rsp.set_ret(msg->ret());
        center_send_client_by_role_id(w, role_id, rsp);
        return NULL;
    }
    
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.create_mate_relation(role_id, mate_id);
    rsp.set_ret(ret);
    rsp.set_mate_role_id(mate_id);
    center_send_client_by_role_id(w, role_id, rsp);
    rsp.set_mate_role_id(role_id);
    center_send_client_by_role_id(w, mate_id, rsp);

    client::mate_info_rsp mirsp;
    mirsp.set_mate_role_id(0);
    w->mm_mgr.fill_mate_info_rsp(role_id, mirsp);
    center_send_client_by_role_id(w, role_id, mirsp);

    mirsp.set_mate_role_id(0);
    w->mm_mgr.fill_mate_info_rsp(mate_id, mirsp);
    center_send_client_by_role_id(w, mate_id, mirsp);

    const online_role* oi1 = w->oi_mgr.get_online_info(role_id);
    const online_role* oi2 = w->oi_mgr.get_online_info(mate_id);
    if (oi1 != NULL && oi2 != NULL) {
        sys_msg_mate(w, 1, oi1->role_name, oi1->role_id, oi2->role_name, oi2->role_id);
    }

    return NULL;
}

void* on_center_pb_depth_mate_req(client::depth_mate_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = req->svr_head().role_id();
    mate* src = NULL;
    mate* dst = NULL;
    uint32 src_depth_times = 0;
    uint32 dst_depth_times = 0;
    uint32 mate_id = 0;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.can_enter_depth_mate(role_id, src, src_depth_times, mate_id, dst, dst_depth_times);
    if (ret != client::MORE_SUCCESS){
        depth_mate_rsp rsp;
        rsp.set_ret(ret);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    // 双修小伙伴中任何一方普通仙修过都必须要使用元宝仙修
    if ((!req->has_advanced() || !req->advanced()) 
        && (src_depth_times < MAX_DAY_DEPTH_MATE_TIMES || dst_depth_times < MAX_DAY_DEPTH_MATE_TIMES)){
        depth_mate_rsp rsp;
        rsp.set_ret(client::MORE_NEED_ADVANCE);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    const online_role* oi = w->oi_mgr.get_online_info(mate_id);
    if (oi == NULL){
        depth_mate_rsp rsp;
        rsp.set_ret(client::MORE_MATE_OFFLINE);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    req->clear_svr_head();
    center_send_client(w, *oi, *req);

    return NULL;
}

void* on_center_pb_depth_mate_rsp(client::depth_mate_rsp* rsp, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = rsp->svr_head().role_id();
    mate* src = NULL;
    mate* dst = NULL;
    uint32 src_depth_times = 0;
    uint32 dst_depth_times = 0;
    uint32 mate_id = 0;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.can_enter_depth_mate(role_id, src, src_depth_times, mate_id, dst, dst_depth_times);
    if (ret != client::MORE_SUCCESS){
        depth_mate_rsp result;
        result.set_ret(ret);
        center_send_client_by_role_id(w, role_id, result);
        center_send_client_by_role_id(w, mate_id, result);
        return NULL;
    }

    const online_role* oi = w->oi_mgr.get_online_info(mate_id);
    if (oi == NULL){
        depth_mate_rsp result;
        result.set_ret(client::MORE_MATE_OFFLINE);
        center_send_client_by_role_id(w, role_id, result);
        return NULL;
    }

    if (rsp->ret() != client::MORE_SUCCESS){
        depth_mate_rsp result;
        result.set_ret(rsp->ret());
        center_send_client(w, *oi, result);
        return NULL;
    }

    // 双修小伙伴中任何一方普通仙修过都必须要使用元宝仙修
    if ((!rsp->has_advanced() || !rsp->advanced()) 
        && (src_depth_times < MAX_DAY_DEPTH_MATE_TIMES || dst_depth_times < MAX_DAY_DEPTH_MATE_TIMES)){
        depth_mate_rsp result;
        result.set_ret(client::MORE_NEED_ADVANCE);
        center_send_client_by_role_id(w, role_id, result);
        center_send_client_by_role_id(w, mate_id, result);
        return NULL;
    }

    client::depth_mate_msg msg;
    msg.set_role_id(mate_id);
    msg.set_mate_role_id(role_id);
    msg.set_need_ingot(dst_depth_times < MAX_DAY_DEPTH_MATE_TIMES || rsp->advanced());
    msg.set_ret(client::MORE_SUCCESS);
    center_send_scene(w, *oi, msg);

    return NULL;
}

void* on_center_pb_depth_mate_msg(client::depth_mate_msg* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    if (msg->ret() != client::MORE_SUCCESS){
        client::depth_mate_rsp rsp;
        rsp.set_ret(msg->ret());
        center_send_client_by_role_id(w, msg->role_id(), rsp);
        center_send_client_by_role_id(w, msg->mate_role_id(), rsp);
        return NULL;
    }

    uint32 mate_id = 0;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.enter_depth_mate(msg->role_id(), mate_id, msg->need_ingot());
    if (ret != client::MORE_SUCCESS){
        client::depth_mate_rsp rsp;
        rsp.set_ret(ret);
        center_send_client_by_role_id(w, msg->role_id(), rsp);
        center_send_client_by_role_id(w, msg->mate_role_id(), rsp);
        return NULL;
    }

    client::depth_mate_rsp rsp;
    rsp.set_ret(client::MORE_SUCCESS);
    center_send_client_by_role_id(w, msg->role_id(), rsp);
    center_send_client_by_role_id(w, msg->mate_role_id(), rsp);

    const online_role* oi1 = w->oi_mgr.get_online_info(msg->role_id());
    const online_role* oi2 = w->oi_mgr.get_online_info(msg->mate_role_id());
    if (oi1 != NULL && oi2 != NULL) {
        sys_msg_mate(w, 2, oi1->role_name, oi1->role_id, oi2->role_name, oi2->role_id);
    }

    return NULL;
}

void* on_center_pb_depth_mate_end_req(client::depth_mate_end_req* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    uint32 role_id = msg->svr_head().role_id();
    bool advanced = false;
    uint32 mate_id = 0;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.end_depth_mate(role_id, advanced, mate_id);
    client::depth_mate_end_msg result;
    result.set_ret(ret);
    result.set_role_id(role_id);
    result.set_mate_role_id(mate_id);
    result.set_advanced(advanced);
    if (ret != client::MORE_SUCCESS){
        center_send_client_by_role_id(w, role_id, result);
        return NULL;
    }
    center_send_scene(w, role_id, result);
    /*client::MATE_OPERATE_RET_E src_ret;
    client::MATE_OPERATE_RET_E dst_ret;
    uint32 mate_id = 0;
    bool src_advanced = false;
    bool dst_advanced = false;
    w->mm_mgr.end_depth_mate(role_id, src_ret, src_advanced, mate_id, dst_ret, dst_advanced);
    
    client::depth_mate_end_msg result;
    result.set_ret(src_ret);
    result.set_role_id(role_id);
    result.set_mate_role_id(mate_id);
    result.set_advanced(src_advanced);
    center_send_scene(w, role_id, result);

    result.set_ret(dst_ret);
    result.set_role_id(mate_id);
    result.set_mate_role_id(role_id);
    result.set_advanced(dst_advanced);
    center_send_scene(w, mate_id, result);*/

    return NULL;
}

void* on_center_pb_mate_remove_req(client::mate_remove_req* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    client::mate_remove_msg rsp;
    uint32 role_id = msg->svr_head().role_id();
    const mate* src = w->mm_mgr.get_my_mate(role_id);
    if (src == NULL || src->get_mate_role_id() == 0){
        rsp.set_ret(client::MORE_NO_MATE);
        center_send_client_by_role_id(w, role_id, rsp);
        return NULL;
    }

    center_send_scene(w, role_id, *msg);

    return NULL;
}

void* on_center_pb_mate_remove_msg(client::mate_remove_msg* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    if (msg->ret() != client::MORE_SUCCESS){
        return NULL;
    }

    uint32 role_id = msg->role_id();
    uint32 mate_id = 0;
    client::MATE_OPERATE_RET_E ret = w->mm_mgr.remove_mate_relation(role_id, mate_id);
    if (ret != client::MORE_SUCCESS){
        msg->set_ret(ret);
        center_send_client_by_role_id(w, role_id, *msg);
        return NULL;
    }

    if (mate_id > 0){
        center_send_client_by_role_id(w, mate_id, *msg);
    }
    
    center_send_client_by_role_id(w, role_id, *msg);

    const online_role* oi1 = w->oi_mgr.get_online_info(role_id);
    const online_role* oi2 = w->oi_mgr.get_online_info(mate_id);
    if (oi1 != NULL && oi2 != NULL) {
        sys_msg_mate(w, 3, oi1->role_name, oi1->role_id, oi2->role_name, oi2->role_id);
    }

    return NULL;
}

void* on_center_pb_mate_award_req(client::mate_award_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }
    
    uint32 role_id = req->svr_head().role_id();
    client::mate_award_via via;
    via.set_role_id(role_id);
    const mate* src = w->mm_mgr.get_my_mate(role_id);
    if (req->type() < 12 && src->get_total_times() >= (2 * (req->type() + 1))) {
        via.set_check(true);
        via.set_award_id(3030 + req->type());
    } else {
        via.set_check(false);
    }

    center_send_scene(w, req->svr_head(), via);
    return NULL;
}
