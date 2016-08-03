#ifndef __ROLE_INTERFACE_H__
#define __ROLE_INTERFACE_H__

#include "pb_dispatcher.h"
#include "pb_coder.h"
#include "amtsvr.h"
#include "role_login_mgr.h"
#include "config_mgr.h"
#include "player_role.h"

namespace client {
    class auth_req;
}

class server_role_t
{
private:
    uint32 svr_id;
    uint32 max_role_id;

public:
    server_role_t() {
        svr_id = 0;
        max_role_id = 0;
    }

    virtual ~server_role_t() {
    }

    uint32 get() {
        uint32 id = max_role_id + 1;
        if (id == 0 || id > ROLE_ID_MASK) {
            return 0;
        }
        ++max_role_id;

        return PACK_SVR_ROLE_ID(svr_id, max_role_id);
    }

    uint32 get_svr_id() {
        return svr_id;
    }

    void set_max_roleid(uint32 id) {
        max_role_id = id;
    }

    void set_svrid(uint32 id) {
        svr_id = id;
    }
};

struct role_service_t
{
public:
    pb_dispatcher_s pd;
    struct amtsvr_context* ctx;

    role_login_mgr alm;

public:
    bool generate_role_id(uint32 svr_id, uint32& role_id);
    void push(uint32 svr_role_id);
    void push_svr(uint32 svr_id);

private:
    std::map<uint32, server_role_t> _svr_roles;
};

int role_dispatcher_init(struct role_service_t* a);

int call_sp_login_auth(role_service_t* a, client::auth_req* req);

int role_send_agent(role_service_t* a, uint32 agent_id, google::protobuf::Message& msg);

int role_send_center(role_service_t* a, google::protobuf::Message& msg);

#endif
