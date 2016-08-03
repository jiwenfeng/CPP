#ifndef __LOAD_ROLE_MGR_H__
#define __LOAD_ROLE_MGR_H__

#include "pb_mgr.h"
#include "loader_role.h"

class loader_role_mgr 
{
public:
    loader_role_mgr();
    loader_role_mgr(void* ud);

    virtual ~loader_role_mgr();

    int proc_load_req(google::protobuf::Message* msg, uint32 role_id, int session, uint32_t source);

    int proc_sqlmgr_rsp(struct cmq_svr_msg_t* rsp, int session, uint32_t source);

    void set_ud(void* ud) { _ud = ud; }

protected:
    int new_load_role(google::protobuf::Message* msg, uint32 role_id);
    int load_rsp(loader_role& lr);
    int detach_load_role(loader_role& lr, player_role*& pr, center_role*& cr);

protected:
    std::map<uint32, loader_role*> roleid_rolse; // role_id->
    std::map<int, uint32>  session_roles;        // session_id->role_id

    void* _ud;
};


#endif
