#include "online_role_mgr.h"
#include "pb_coder.h"

online_role_mgr::online_role_mgr()
{
}

online_role_mgr::~online_role_mgr()
{
}

const online_role* online_role_mgr::get_online_info(uint32 role_id)
{
    map<uint32, online_role>::iterator it = id_infos.find(role_id);
    if (it == id_infos.end()){
        return NULL;
    }

    return &(it->second);
}

online_role* online_role_mgr::mutable_online_info(uint32 role_id)
{
    map<uint32, online_role>::iterator it = id_infos.find(role_id);
    if (it == id_infos.end()){
        return NULL;
    }

    return &(it->second); 
}

void online_role_mgr::update(const online_role& oi)
{
    uint32 old_scene_id = 0; 
    std::map<uint32, online_role>::iterator it = id_infos.find(oi.role_id);
    if (it != id_infos.end()){
        old_scene_id = it->second.scene_id;
        it->second = oi;
        update_scene_roles(old_scene_id, oi.scene_id, oi.role_id);
        return;
    }

    id_infos.insert(std::pair<uint32, online_role>(oi.role_id, oi));
    update_scene_roles(old_scene_id, oi.scene_id, oi.role_id);
}

void online_role_mgr::update_scene(uint32 role_id, uint32 scene_id, uint32 scene_service_id)
{
    map<uint32, online_role>::iterator it = id_infos.find(role_id);
    if (it == id_infos.end()){
        return;
    }

    online_role& oi = it->second;
    uint32 old_scene_id = oi.scene_id;
    oi.scene_id = scene_id;
    oi.scene_svr_id = scene_service_id;
    update_scene_roles(old_scene_id, oi.scene_id, role_id);
}

void online_role_mgr::update_scene_roles(uint32 old_scene_id, uint32 new_scene_id, uint32 role_id)
{
    if (old_scene_id == new_scene_id || new_scene_id == 0){
        return;
    }

    map<uint32, map<uint32, uint32> >::iterator scene_it = scene_roles.find(old_scene_id);
    if (scene_it != scene_roles.end()){
        map<uint32, uint32>& ref_map = scene_it->second;
        ref_map.erase(role_id);
    }

    scene_it = scene_roles.find(new_scene_id);
    if (scene_it == scene_roles.end()){
        map<uint32, uint32> new_map;
        new_map.insert(pair<uint32, uint32>(role_id,0));
        scene_roles[new_scene_id] = new_map;
    } else {
        map<uint32, uint32>& ref_map = scene_it->second;
        ref_map.insert(pair<uint32, uint32>(role_id,0));
    }
}

void online_role_mgr::remove(uint32 role_id)
{
    const online_role* oi = get_online_info(role_id);
    if (oi == NULL){
        return;
    }

    map<uint32, map<uint32, uint32> >::iterator it = scene_roles.find(oi->scene_id);
    if (it != scene_roles.end()){
        it->second.erase(role_id);
    }

    id_infos.erase(role_id);
}

void online_role_mgr::brodcast(amtsvr_context* ctx, google::protobuf::Message& msg)
{
    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    uint32 id_count = 0;
    uint32 size = id_infos.size();
    if (size > MAX_BROADCAST_AGENT_NUM){
        size = MAX_BROADCAST_AGENT_NUM;
    }

    map<uint32, online_role>::iterator it = id_infos.begin();
    for (; it != id_infos.end() && id_count < size; ++it, ++id_count){
        ids[id_count] = it->second.client_id;
    }

    if (id_count == 0){
        return ;
    }

    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL){
        return;
    }

    amtsvr_broadcast(ctx, ids, id_count, (void*)buf, msg_len);
}

void online_role_mgr::get_scene_roles(uint32 scene_id, vector<uint32>& roles)
{
    map<uint32, map<uint32, uint32> >::const_iterator it = scene_roles.find(scene_id);
    if (it == scene_roles.end()){
        return;
    }

    const map<uint32, uint32>& ref_map = it->second;
    map<uint32, uint32>::const_iterator it_roles;
    for (it_roles = ref_map.begin(); it_roles != ref_map.end(); ++it_roles){
        roles.push_back(it_roles->first);
    }
}
