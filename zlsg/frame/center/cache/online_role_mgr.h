#ifndef __ONLINE_INFO_MGR_H__
#define __ONLINE_INFO_MGR_H__

#include <map>
#include <vector>
#include <string>
#include "define.h"
#include "amtsvr.h"
#include "pb_mgr.h"
#include "online_role.h"

using namespace std;

class online_role_mgr
{
public:
    online_role_mgr();

    virtual ~online_role_mgr();

    const online_role* get_online_info(uint32 role_id);

    online_role* mutable_online_info(uint32 role_id);

    void update(const online_role& oi);

    void update_scene(uint32 role_id, uint32 scene_id, uint32 scene_service_id);

    void remove(uint32 role_id);

    void brodcast(amtsvr_context* ctx, google::protobuf::Message& msg);

    void get_scene_roles(uint32 scene_id, vector<uint32>& roles);

    const map<uint32, online_role>& get_all_online_info() const { return id_infos; }

private:
    void update_scene_roles(uint32 old_scene_id, uint32 new_scene_id, uint32 role_id);

private:
	// 玩家在线列表
    std::map<uint32, online_role> id_infos;        // role_id-->
    std::map<uint32, std::map<uint32, uint32> > scene_roles; // scene_id-->role_ids
};

#endif
