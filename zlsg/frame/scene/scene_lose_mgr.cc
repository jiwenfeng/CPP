#include "scene_inst.h"
#include "scene_lose_mgr.h"

scene_lose_mgr::scene_lose_mgr(scene_inst* si)
    : _si(si)
{
    _index = 0;
}

scene_lose_mgr::~scene_lose_mgr()
{
}

int scene_lose_mgr::on_update(uint32 tick)
{
    if (_loses.empty()) {
        return 0;
    }

    client::drop_props_msg msg;
    msg.set_m(client::drop_props_msg_METHOD_DEL);
    uint32 now = (uint32)time(NULL);
    std::map<uint32, scene_lose>::iterator i = _loses.begin();
    while (i != _loses.end()) {
        if (i->second.end_time() <= now) {
            client::drop_props_info* p = msg.add_dpi();
            p->CopyFrom(i->second);
            _loses.erase(i++);
            continue;
        }

        ++i;
    }

    if (msg.dpi_size() != 0) {
        _si->scene_broadcast(msg);
    }

    return 0;
}

scene_lose& scene_lose_mgr::generate_lose()
{
    return _loses[generate_index()];
}

void scene_lose_mgr::add_lose(const client::drop_props_info* dpi)
{
    if (dpi == NULL) {
        return ;
    }

    _loses.insert(pair<uint32, scene_lose>(dpi->index(), *dpi));
}

scene_lose* scene_lose_mgr::get_lose(uint32 index)
{
    std::map<uint32, scene_lose>::iterator i = _loses.find(index);
    if (i == _loses.end()) {
        return NULL;
    }

    return &(i->second);
}

void scene_lose_mgr::pick_lose(uint32 index)
{
    _loses.erase(index);
}

bool scene_lose_mgr::can_pick(uint32 index, uint32 role_id)
{
	std::map<uint32, scene_lose>::iterator it = _loses.find(index);
	if(it == _loses.end())
	{
		return false;
	}
	return it->second.can_pick(role_id);
}

bool scene_lose_mgr::fill_loses(client::drop_props_msg& msg)
{
    msg.set_m(client::drop_props_msg_METHOD_ADD);
    std::map<uint32, scene_lose>::iterator i = _loses.begin();
    while (i != _loses.end()) {
        client::drop_props_info* p = msg.add_dpi();
        p->CopyFrom(i->second);
        ++i;
    }

    return msg.dpi_size() != 0;
}

bool scene_lose::can_pick(uint32 role_id)
{
	uint32 now = (uint32)time(NULL);
	if(now > end_time())
	{
		return false;
	}
	if(belong_size() == 0)
	{
		return true;
	}
	bool ret = false;
	for(int i = 0; i < belong_size(); ++i)
	{
		if(belong(i) == role_id)
		{
			ret = true;
			break;
		}
	}
	if(!ret)
	{
		if(now < protect_time())
		{
			return false;
		}
	}
	return true;
}
