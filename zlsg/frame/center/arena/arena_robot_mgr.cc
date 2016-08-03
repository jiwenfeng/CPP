#include "config_robot.h"
#include "arena_robot_mgr.h"
#include "client.pets.pb.h"

#define R_BUFFLEN 65535

robot::robot(mysql::tb_arena_robot *tar)
{
    _tar = tar;
	writer_type = WOTE_INSERT;
}

robot::~robot()
{
	_attr.clear();
	_b_attr.clear();
	_b_attr.clear();
    if(_tar == NULL)
    {
        return;
    }
    delete _tar;
}

const string &robot::get_role_name() const
{
	if(_tar == NULL)
	{
		static string n = "";
		return n;
	}
	return _tar->name();
}

const uint32 robot::get_fight_power() const
{
	if(_tar == NULL)
	{
		return 0;
	}
	return _tar->fighting_power();
}

const uint32 robot::get_pet_id() const
{
	if(_tar == NULL)
	{
		return 0;
	}
	return _tar->pet_id();
}

const uint32 robot::get_standby_id() const
{
	if(_tar == NULL)
	{
		return 0;
	}
	return _tar->standby();
}

const uint32 robot::get_role_id() const
{
	if(_tar == NULL)
	{
		return 0;
	}
	return _tar->id();
}

const uint32 robot::get_config_id() const
{
	if(_tar == NULL)
	{
		return 0;
	}
	return _tar->config_id();
}

int robot::save(amtsvr_context *ctx)
{
    if (_tar == NULL)
    {
        return -1;
    }
	string attr1, attr2, attr3;
	if((serialize_attr(attr1, _attr) == -1) || (serialize_attr(attr2, _b_attr) == -1) || (serialize_attr(attr3, _b_attr) == -1))
	{
		return -1;
	}
	_tar->set_attr(attr1);
	_tar->set_g_attr(attr2);
	_tar->set_b_attr(attr3);
    mysql::tb_arena_robot* tar = new mysql::tb_arena_robot(*_tar);
    CACHE_KEY k(tar->id(), 0);
    write(ctx, k, tar);
    writer_type = WOTE_UPDATE;
    return 0;
}

int robot::unserialize_pet_attr(const string &str)
{
	return unserialize_attr(str, _b_attr);
}

int robot::unserialize_standby_attr(const string &str)
{
	return unserialize_attr(str, _b_attr);
}

int robot::unserialize_robot_attr(const string &str)
{
	return unserialize_attr(str, _attr);
}

int robot::serialize_attr(string &str, const map<int, int> &m)
{
	char buf[R_BUFFLEN] = {0};
	int offset = 0;
	map<int, int>::const_iterator it;
	for(it = m.begin(); it != m.end(); ++it)
	{
		client::pet_fight_attr pfa;
		pfa.set_id(it->first);
		pfa.set_num(it->second);
		uint16 len = pfa.ByteSize();
		if(offset + len + sizeof(uint16) >= R_BUFFLEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!pfa.SerializeToArray((void *)(buf + offset), len))
		{
			amtsvr_log("serialize failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return offset;
}

int robot::unserialize_attr(const string &str, map<int, int> &m)
{
	int len = str.length();
	int offset = 0;
	const char *ptr = str.c_str();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::pet_fight_attr pfa;
		if(!pfa.ParseFromArray(ptr + offset, len))
		{
			return -1;
		}
		m.insert(make_pair(pfa.id(), pfa.num()));
		offset += len;
	}
	return 0;
}

robot_mgr::robot_mgr()
{

}

robot *robot_mgr::add_robot(mysql::tb_arena_robot *tar)
{
	map<uint32, robot *>::iterator it = _robot.find(tar->id());
    if (it != _robot.end()) {
		amtsvr_log("repeated id:%u\n", tar->id());
		return NULL;
    }
    robot *r = new robot(tar);
    _robot.insert(make_pair(tar->id(), r));
	_name.insert(make_pair(tar->name(), tar->id()));
    return r;
}

int robot_mgr::save(amtsvr_context *ctx)
{
	map<uint32, robot *>::iterator it;
	for(it = _robot.begin(); it != _robot.end(); ++it)
	{
		it->second->save(ctx);
	}
	return 0;
}

int robot_mgr::init()
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_arena_robot();
    while(cur != NULL)
    {
        mysql::tb_arena_robot *tar = static_cast<mysql::tb_arena_robot *>(cur->mutable_val().pop_msg());
        cur = cur->get_next();
        if(NULL == tar)
        {
            continue;
        }
		robot *r = add_robot(tar);
		if(r != NULL)
		{
			r->change_writer_type();
			r->unserialize_robot_attr(tar->attr());
			r->unserialize_pet_attr(tar->g_attr());
			r->unserialize_standby_attr(tar->g_attr());
		}
    }
    return 0;
}

robot_mgr::~robot_mgr()
{
    map<uint32, robot *>::iterator it;
    for(it = _robot.begin(); it != _robot.end(); ++it)
    {
        delete it->second;
    }
    _robot.clear();
	_name.clear();
}

const robot *robot_mgr::get_robot(uint32 r) const
{
	map<uint32, robot *>::const_iterator it = _robot.find(r);
	if(it == _robot.end())
	{
		return NULL;
	}
	return it->second;
}

