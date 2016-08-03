#include "loader_role_mgr.h"
#include "loader_interface.h"

loader_role_mgr::loader_role_mgr() : _ud(NULL)
{
}

loader_role_mgr::loader_role_mgr(void* ud) : _ud(ud)
{
}

loader_role_mgr::~loader_role_mgr()
{
}

int loader_role_mgr::new_load_role(google::protobuf::Message* msg, uint32 role_id)
{
    loader_role* lr = new loader_role;
    lr->attact_req(msg);
    roleid_rolse[role_id] = lr;

    int session = lr->load(((loader_t*)_ud)->ctx, role_id);
    if (session <= 0) {
        delete lr;
        roleid_rolse.erase(role_id);
        return -1;
    }

    session_roles[session] = role_id;
    return 0;
}

int loader_role_mgr::proc_load_req(google::protobuf::Message* msg, uint32 role_id, int session, uint32_t source)
{
    map<uint32, loader_role*>::iterator it = roleid_rolse.find(role_id);
    if (it == roleid_rolse.end() || it->second == NULL) {
        return new_load_role(msg, role_id);
    }

    loader_role* lr = it->second;
    lr->attact_req(msg);
    if (!lr->is_loader_end()) {
        return 0;
    }

    load_rsp(*lr);
    roleid_rolse.erase(role_id);
    delete lr;
    
    return 0;
}

int loader_role_mgr::detach_load_role(loader_role& lr, player_role*& pr, center_role*& cr)
{
    pr = lr.pop_role();
    cr = lr.pop_crole();
    if (pr != NULL && cr != NULL) {
        return 0;
    }

    if (pr != NULL) {
        delete pr;
        pr = NULL;
    }
   
    if (cr != NULL) {
        delete cr;
        cr = NULL;
    }

    return -1;
}

int loader_role_mgr::load_rsp(loader_role& lr)
{
    loader_t* l = (loader_t*)_ud;
    player_role* pr = NULL;
    center_role* cr = NULL;
    if (detach_load_role(lr, pr, cr) != 0) {
        return 0;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete pr;
        delete cr;
        return 0;
    }

    // 分离加载到的用户信息
    server::load_role_ret* ret = new server::load_role_ret;
    uintptr_t ptr  = (uintptr_t)pr;
    uintptr_t cptr = (uintptr_t)cr;
    ret->set_ptr((uint64)ptr);
    ret->set_cptr((uint64)cptr);
    mh->msg = ret;
    amtsvr_sendname(l->ctx, CENTER_NAME, PTYPE_SYSTEM | PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

    // 分发原来的所有请求数据
    std::vector<google::protobuf::Message*>& req_list = lr.get_req();
    uint32 size = req_list.size();
    for (uint32 i = 0; i < size; ++i) {
        mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            continue;
        }

        mh->msg = req_list[i];
        req_list[i] = NULL;

        amtsvr_sendname(l->ctx, CENTER_NAME, PTYPE_SYSTEM | PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
    }

    return 0;
}

int loader_role_mgr::proc_sqlmgr_rsp(struct cmq_svr_msg_t* rsp, int session, uint32_t source)
{
    std::map<int, uint32>::iterator it = session_roles.find(session);
    if (it == session_roles.end()) {
        return -1;
    }

    uint32 role_id = it->second;
    session_roles.erase(it);

    std::map<uint32, loader_role*>::iterator lr_it = roleid_rolse.find(role_id);
    if (lr_it == roleid_rolse.end()) {
        return -2;
    }

    loader_role* lr = lr_it->second;
    if (lr == NULL) {
        roleid_rolse.erase(lr_it);
        return -3;
    }

    int new_session = lr->proc_sqlmgr_rsp(((loader_t*)_ud)->ctx, rsp);
    if (lr->is_loader_end()) {
        load_rsp(*lr);
        delete lr;
        roleid_rolse.erase(lr_it);
        return 0;
    }

    if (new_session <= 0) {
        // 遇到错误，销毁对象
        delete lr;
        roleid_rolse.erase(lr_it);
        return 0;
    }

    session_roles[new_session] = role_id;
    return 0;
}

