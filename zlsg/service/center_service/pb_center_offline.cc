#include "client.offline.pb.h"
#include "offline_role_mgr.h"
#include "role_key.h"
#include "client.arena.pb.h"
#include "center_interface.h"

/*void notify_practice_offline(center_service_t* w, const client::server_head& svr_head)
{
    client::stop_comm_practice_req req;
    req.set_unixtime(0);
    req.mutable_svr_head()->CopyFrom(svr_head);
    center_send_scene(w, svr_head, req);
    }*/

void notify_friend_offline(center_service_t* w, uint32 role_id)
{
    client::friend_on_off_notify foon;
    foon.set_role_id(role_id);
    foon.set_online(false);
    player_role_friends* src_friends = w->fm_mgr.mut(role_id);
    if (src_friends == NULL) {
        return ;
    }
    map<uint32, client::friend_info>* p = src_friends->get_friend();
    map<uint32, client::friend_info>::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        center_send_client_by_role_id(w, i->second.role_id(), foon);
    }
}

void notify_team_offline(center_service_t* w, uint32 role_id)
{
	uint32 team_id = w->tm_mgr.get_team_id(role_id);
	if(0 == team_id)
	{
		return ;
	}
	w->tm_mgr.leave_team(role_id);
	update_team_info(w, team_id);
}

static void proc_user_offline(center_service_t* w, uint32 role_id)
{
    if (role_id == 0) {
        return;
    }

    // 通知双修
    //notify_practice_offline(w, p->svr_head());
    // 通知好友下线
    notify_friend_offline(w, role_id);
    // 通知队友
    notify_team_offline(w, role_id);
    // 去掉在线用户信息
    w->oi_mgr.remove(role_id);
    w->_role_key->add_key_time(role_id);

    center_on_user_offline(w, role_id);
}

void* on_pb_center_offline(client::offline* p, void* ud, size_t* rsp_len)
{
    proc_user_offline((center_service_t*)ud, p->svr_head().role_id());

    return NULL;
}

void* on_pb_center_user_lose(client::user_lose* ul, void* ud, size_t* rsp_len)
{
    proc_user_offline((center_service_t*)ud, ul->role_id());

    return NULL;
}

void* on_pb_record_offline(server::record_offline_user* ro, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    uintptr_t ptr = (uintptr_t)ro->ptr();
    player_role* pr = (player_role*)(ptr);
    if (pr == NULL) {
        return NULL;
    }

    uint32 role_id = pr->get_role_id();
    center_role* cr = NULL;
    online_role* oi = w->oi_mgr.mutable_online_info(role_id);
    if (oi != NULL) {
        cr = oi->pop_center_role();
    }

    w->ou_mgr->push(role_id, pr, cr);
    w->oi_mgr.remove(role_id);
	w->aus_mgr.update_user_offline_time(role_id, (uint32)time(NULL));

    center_on_user_offline(w, role_id);

    return NULL;
}

void *on_pb_center_offline_load_image(client::offline_load_image *req, void *ud, size_t *len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	vector<uint32> v;
	w->aus_mgr.get_offline_user(req, v);
	vector<uint32> v1;
	for(size_t i = 0; i < v.size(); ++i)
	{
		bool flag = false;
		if(w->oi_mgr.get_online_info(v[i]) != NULL)
		{
			continue;
		}
		uint32 fid = w->cf_mgr->get_family_id(v[i]);
		if(req->flag() == 1)
		{
			if(fid == 0)
			{
				continue;
			}
			for(int j = 0; j < req->family_size(); ++j)
			{
				if(fid == req->family(j))
				{
					v1.push_back(v[i]);
					break;
				}
			}
		}
		else
		{
			for(int j = 0; j < req->family_size(); ++j)
			{
				if(fid == req->family(j))
				{
					flag = true;
					break;
				}
			}
			if(!flag)
			{
				v1.push_back(v[i]);
			}
		}
	}
	if(v1.empty())
	{
		return NULL;
	}
	uint32 role_id = v1[mtrandom::rand_mid32(0, v1.size() - 1)];
	client::load_target_data_req l;
	l.set_target(role_id);
	l.set_scene_id(req->scene_id());
	l.set_role_id(0);
	load_user_data(w, l);
	return NULL;
}
