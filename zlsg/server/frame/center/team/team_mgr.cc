#include "team_mgr.h"
#include "client.team.pb.h"
team_mgr::team_mgr()
{
    _team_id = 1;
}

team_mgr::~team_mgr()
{

}

uint32 team_mgr::alloc_team_id()
{
    uint32 id = 0;
    int count= 0;
    do {
        id = _team_id++;
        if (id == 0){
            id = _team_id++;
        }
    } while(_teams.count(id) > 0 && count++ < 20);

    return id;
}

uint32 team_mgr::create_team(uint32 captain, uint32 raid_id)
{
    if (captain == 0){
        return 0;
    }
	uint32 id = alloc_team_id();
    if (id == 0)
	{
        return 0;
    }
	if(_role_team.end() != _role_team.find(captain))
	{
		return 0;
	}
    client::team_info ti;
    ti.set_team_id(id);
	ti.set_raid_id(raid_id);
    ti.set_captain(captain);
	ti.set_status(client::team_info_status_e_normal);
	client::member_info *mi = ti.add_member();
	mi->set_role_id(captain);
	mi->set_ready(true);
    _teams.insert(pair<uint32, client::team_info>(id, ti));
    _role_team[captain] = id;
	return id;
}

int team_mgr::send_leave_team_notify(uint32 role_id, uint32 type)
{
	client::team_notify tn;
	tn.set_role_id(role_id);
	tn.set_type(type);
	team_send_center(tn);
	return 0;
}

int team_mgr::team_send_center(google::protobuf::Message &coi)
{
	google::protobuf::Message *new_msg = create_msg(coi.GetTypeName());
	if(new_msg == NULL)
	{
		return -3;
	}
	new_msg->CopyFrom(coi);
	msg_handler *mh = MSG_HANDLER_POOL->pop();
	if(mh == NULL)
	{
		delete new_msg;
		return -4;
	}
	mh->msg = new_msg;
	return amtsvr_sendname(_ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

}

uint32 team_mgr::get_team_raid(uint32 team_id)
{
	client::team_info *ti = get_team_info(team_id);
	if(NULL == ti)
	{
		return 0;
	}
	return ti->raid_id();
}

void team_mgr::change_team_status(uint32 team_id, uint32 status)
{
	client::team_info *ti = get_team_info(team_id);
	if(NULL == ti)
	{
		return ;
	}
	ti->set_status(client::team_info_status_e(status));
}

uint32 team_mgr::get_team_status(uint32 team_id)
{
	client::team_info *ti = get_team_info(team_id);
	if(NULL == ti)
	{
		return 1;
	}
	return (uint32)ti->status();
}


int team_mgr::join_member(uint32 team_id, uint32 role_id)
{
    client::team_info* ti = get_team_info(team_id);
    if (ti == NULL)
	{
        return 10;
    }
    if (ti->member_size() >= MAX_TEAM_MEMBER_NUM)
	{
        return 11;
    }
	map<uint32, uint32>::iterator it = _role_team.find(role_id);
	if(it != _role_team.end())
	{
		return 13;
	}
	client::member_info *mi = ti->add_member();
	mi->set_role_id(role_id);
	mi->set_ready(false);
	_role_team[role_id] = team_id;
    return 0;
}

uint32 team_mgr::get_member_count(uint32 team_id)
{
	map<uint32, client::team_info>::iterator it = _teams.find(team_id);
	if(it == _teams.end())
	{
		return 0;
	}
	return it->second.member_size();
}

bool team_mgr::is_all_ready(uint32 team_id)
{
	map<uint32, client::team_info>::iterator it = _teams.find(team_id);
	if(it == _teams.end())
	{
		return false;
	}
	for(int i = 0; i < it->second.member_size(); ++i)
	{
		const client::member_info *ti = it->second.mutable_member(i);
		if(!ti->ready())
		{
			return false;
		}
	}
	return true;
}

int team_mgr::cancel_team(uint32 team_id, bool notify)
{
	map<uint32, client::team_info>::iterator it = _teams.find(team_id);
	if(it == _teams.end())
	{
		return -1;
	}
	for(int i = 0; i < it->second.member_size(); ++i)
	{
		map<uint32, uint32>::iterator j = _role_team.find(it->second.member(i).role_id());
		if(j == _role_team.end())
		{
			continue;
		}
		if(notify)
		{
			send_leave_team_notify(j->first, 0);
		}
		_role_team.erase(j);
	}
	it->second.mutable_member()->Clear();
	_teams.erase(it);
	return 0;
}

int team_mgr::leave_team(uint32 role_id, bool notify)
{
	map<uint32, uint32>::iterator i = _role_team.find(role_id);
	if(i == _role_team.end())
	{
		return -1;
	}
	map<uint32, client::team_info>::iterator j = _teams.find(i->second);
	if(j == _teams.end())
	{
		return -1;
	}
	if(j->second.captain() == role_id)
	{
		return cancel_team(i->second, notify);
	}
	client::team_info ti(j->second);
	j->second.mutable_member()->Clear();
	for(int k = 0; k < ti.member_size(); ++k)
	{
		if(ti.member(k).role_id() == role_id)
		{
			continue;
		}
		client::member_info *mi = j->second.add_member();
		mi->set_role_id(ti.member(k).role_id());
		mi->set_ready(ti.member(k).ready());
	}
	if(notify)
	{
		send_leave_team_notify(i->first, 1);
	}
	_role_team.erase(i);
    return 0;
}

client::team_info* team_mgr::get_team_info(uint32 team_id)
{
    map<uint32, client::team_info>::iterator it = _teams.find(team_id);
    if (it == _teams.end())
	{
        return NULL;
    }
    return &it->second;
}

client::team_info* team_mgr::get_my_team(uint32 role_id)
{
    map<uint32, uint32>::iterator it = _role_team.find(role_id);
    if (it == _role_team.end()){
        return NULL;
    }
    return get_team_info(it->second);
}

int team_mgr::get_team_members(uint32 team_id, vector<uint32>& roles)
{
	client::team_info *ti = get_team_info(team_id);
	if(ti == NULL)
	{
		return -1;
	}
	for(int i = 0; i < ti->member_size(); ++i)
	{
		roles.push_back(ti->member(i).role_id());
	}
    return 0;
}

uint32 team_mgr::get_team_captain(uint32 team_id)
{
    client::team_info* ti = get_team_info(team_id);
    if (ti == NULL){
        return 0;
    }
    return ti->captain();
}

int team_mgr::change_ready_status(uint32 role_id, bool status /* = true */)
{
	client::team_info *ti = get_my_team(role_id);
	if(ti == NULL)
	{
		return -1;
	}
	for(int i = 0; i < ti->member_size(); ++i)
	{
		if(ti->member(i).role_id() == role_id)
		{
			if(ti->member(i).ready() == status)
			{
				return -1;
			}
			ti->mutable_member(i)->set_ready(status);
			return 0;
		}
	}
	return -1;
}

uint32 team_mgr::get_team_id(uint32 role_id)
{
	map<uint32, uint32>::iterator it = _role_team.find(role_id);
	if(it == _role_team.end())
	{
		return 0;
	}
	return it->second;
}

bool team_mgr::is_same_team(uint32 r1, uint32 r2)
{
	map<uint32, uint32>::iterator i = _role_team.find(r1);
	if(i == _role_team.end())
	{
		return false;
	}
	map<uint32, uint32>::iterator j = _role_team.find(r2);
	if(j == _role_team.end())
	{
		return false;
	}
	return i->second == j->second;
}
