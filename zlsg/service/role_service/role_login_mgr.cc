#include <stdio.h>
#include "role_login_mgr.h"

role_login_mgr::role_login_mgr()
{

}

role_login_mgr::~role_login_mgr()
{

}

void role_login_mgr::put(mysql::sp_login_auth* sla)
{
    if (sla == NULL) {
        return;
    }

    mysql::sp_login_auth* esla = mut(sla->p_openid(), sla->p_server_id());
    if (esla != NULL) {
        if (esla->v_role_id() > 0) {
            esla->set_p_yel_vip  (sla->p_yel_vip  ());
            esla->set_p_server_id(sla->p_server_id());
            esla->set_p_agent_id (sla->p_agent_id ());
            esla->set_p_client_id(sla->p_client_id());
        } else {
            esla->CopyFrom(*sla);
        }

        return;
    }

    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), "%s%u", sla->p_openid().c_str(), sla->p_server_id());
    string k = tmp;

    map_sla[k] = *sla;
}

mysql::sp_login_auth* role_login_mgr::mut(const string& openid, uint32 svr_id)
{
    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), "%s%u", openid.c_str(), svr_id);
    string k = tmp;

    std::map<string, mysql::sp_login_auth>::iterator it = map_sla.find(k);
    if (it == map_sla.end()) {
        return NULL;
    }

    return &it->second;
}

void role_login_mgr::erase(const string& openid, uint32 svr_id)
{
    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), "%s%u", openid.c_str(), svr_id);
    string k = tmp;
    map_sla.erase(k);
}
