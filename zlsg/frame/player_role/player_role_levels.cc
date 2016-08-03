#include "player_role_levels.h"
#include "base_time.h"
#include "config_giantraid.h"
#include "config_upgrade.h"
#include "random.h"

#define LEVELS_BUFFLEN 65535

struct cmp
{
	uint32 _id;
	cmp(uint32 id) : _id(id) {}
	bool operator()(const client::level_info *li)
	{
		return li->id() == _id;
	}
};

player_role_levels::player_role_levels(mysql::tb_role_levels *trl):_trl(trl)
{
#ifdef __LIB_MINI_SERVER__
	_cur_level = 0;
#endif
}

player_role_levels::~player_role_levels()
{
	if(_trl != NULL)
	{
		delete _trl;
		_trl = NULL;
	}
	for(size_t i = 0; i < _li.size(); ++i)
	{
		delete _li[i];
	}
	_li.clear();
}

void player_role_levels::update_role_info(uint32 role_id, uint32 lv, uint32 power)
{
	if(_trl == NULL)
	{
		return;
	}
	_trl->set_role_id(role_id);
	if(_trl->new_power() < power)
	{
		_trl->set_new_power(power);
	}
	if(_trl->new_level() < lv)
	{
		_trl->set_new_level(lv);
	}
}

void player_role_levels::update_role_info()
{
	if(_trl == NULL)
	{
		return;
	}
	if(!time_utils::is_same_day((uint32)time(NULL), _trl->time()))
	{
		_trl->set_old_level(_trl->new_level());
		_trl->set_old_power(_trl->new_power());
		_trl->set_time((uint32)time(NULL));
	}
}

void player_role_levels::clear_all_levels()
{
	for(size_t i = 0; i < _li.size(); ++i)
	{
		delete _li[i];
	}
	_li.clear();
}

int player_role_levels::save(amtsvr_context *ctx, uint32 role_id)
{
	if(NULL == _trl)
	{
		return -1;
	}
	string str1;
	if(-1 == serialize_levels(str1))
	{
		return -1;
	}
	_trl->set_levels(str1);
	mysql::tb_role_levels *trl = new mysql::tb_role_levels(*_trl);
	CACHE_KEY k(trl->role_id(), 0);
	write(ctx, k, trl);
	writer_type = WOTE_UPDATE;
	return 0;
}

void player_role_levels::attach_role_levels(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			if(_trl != NULL)
			{
				delete _trl;
			}
			_trl = static_cast<mysql::tb_role_levels *>(pb_ctx[i].pb_msg);
			if(-1 == unserialize_levels(_trl->levels()))
			{
				return;
			}
			writer_type = WOTE_UPDATE;
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

void player_role_levels::init_all_levels(uint32 job)
{
	if(_trl == NULL)
	{
		return;
	}
	update_role_info();
	const config_upgrade *cu = CONFMGR->get_config_upgrade(job, _trl->old_level());
	if(cu == NULL)
	{
		return;
	}
	clear_all_levels();
	vector<config_giantraid *> raids;
	CONFMGR->get_config_giantraid(_trl->old_level(), raids);
	if(raids.empty())
	{
		return ;
	}
	vector<uint32> group;
	CONFMGR->get_giant_unlock_group(_trl->old_level(), group);
	if(group.empty())
	{
		return ;
	}
	_trl->set_cur_level(raids[0]->get_raid_id());
	for(size_t i = 0; i < raids.size(); ++i)
	{
		client::level_info *li = new client::level_info;
		li->set_id(raids[i]->get_raid_id());
		vector<uint32> v(group);
		for(int j = 1; j <= 3; ++j)
		{
			client::level_hero_info *lhi = li->add_lhi();
			uint32 min_lv = raids[i]->get_rand_lv_min() + _trl->old_level();
			uint32 max_lv = raids[i]->get_rand_lv_max() + _trl->old_level();
			uint32 lv = mtrandom::rand_mid32(min_lv, max_lv);
			float p_rate = mtrandom::rand_mid32(raids[i]->get_rand_power_min(), raids[i]->get_rand_power_max()) / 10000.0f;
			float raw_power = _trl->old_power() * p_rate;
			uint32 power = (uint32)floor(raw_power * (cu->get_giant_factor() / 10000.0f));
			lhi->set_level(lv);
			lhi->set_power(power);
			uint32 gid;
			if(j == 1)
			{
				int idx = mtrandom::rand_mid32(0, group.size() - 1);
				gid = group.at(idx);
				group.erase(remove(group.begin(), group.end(), gid), group.end());
				v.erase(remove(v.begin(), v.end(), gid), v.end());
			}
			else
			{
				int idx = mtrandom::rand_mid32(0, v.size() - 1);
				gid = v.at(idx);
				v.erase(remove(v.begin(), v.end(), gid), v.end());
			}
			uint32 giant_id = CONFMGR->get_giant_id(gid, lv / 10 * 10);
			lhi->set_id(giant_id);
		}
		_li.push_back(li);
	}
}

int player_role_levels::unserialize_levels(const string &str)
{
	size_t offset = 0;
	size_t len = str.length();
	const char *ptr = str.c_str();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		if(len <= 0)
		{
			amtsvr_log("unserialize levels failed\n");
			return -1;
		}
		offset += sizeof(uint16);
		client::level_info *li = new client::level_info;
		if(!li->ParseFromArray((void *)(ptr + offset), len))
		{
			delete li;
			return -1;
		}
		_li.push_back(li);
		offset += len;
	}
	return 0;
}

int player_role_levels::serialize_levels(string &str)
{
	char buf[LEVELS_BUFFLEN] = {0};
	int offset = 0;
	for(size_t i = 0; i < _li.size(); ++i)
	{
		uint16 len = _li[i]->ByteSize();
		if(len + offset + sizeof(uint16) >= LEVELS_BUFFLEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!_li[i]->SerializeToArray(buf + offset, len))
		{
			amtsvr_log("serialize to array failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return 0;
}

uint32 player_role_levels::get_cur_level()
{
#ifndef __LIB_MINI_SERVER__
	if(_trl == NULL)
	{
		return 0;
	}
	return _trl->cur_level();
#else
	return _cur_level;
#endif
}

void player_role_levels::init_role_levels(uint32 role_id, uint32 job, uint32 lv, uint32 power)
{
	if(_trl != NULL)
	{
		return;
	}
	_trl = new mysql::tb_role_levels;
	_trl->set_role_id(role_id);
	uint32 min_lv = (uint32)CONFMGR->get_basic_value(10117);
	uint32 max_lv = (uint32)CONFMGR->get_basic_value(10116);
	lv = max(lv, min_lv);
	lv = min(lv, max_lv);
	_trl->set_old_level(lv);
	_trl->set_new_level(lv);
	uint32 basic_power = (uint32)CONFMGR->get_basic_value(10014);
	if(power < basic_power)
	{
		power = basic_power;
	}
	_trl->set_old_power(power);
	_trl->set_new_power(power);
	_trl->set_time(0);
	init_all_levels(job);
	writer_type = WOTE_INSERT;
}

const client::level_info *player_role_levels::get_level_info(uint32 id)const
{
	for(size_t i = 0; i < _li.size(); ++i)
	{
		if(_li[i]->id() == id)
		{
			return _li[i];
		}
	}

	return NULL;
}

void player_role_levels::change_cur_levels()
{
	if(_trl == NULL)
	{
		return ;
	}
	uint32 cur = _trl->cur_level() + 1;
	_trl->set_cur_level(cur);
}

int player_role_levels::set_cur_level(uint32 level)
{
#ifndef __LIB_MINI_SERVER__
    if(_trl == NULL) {
        return -1;
    }
    
    _trl->set_cur_level(level);
#else
	_cur_level = level;
#endif
    return 0;
}

void player_role_levels::clear_level_info()
{
    for (size_t index = 0; index < _li.size(); ++index) {
        delete _li[index];
    }
    _li.clear();
}

void player_role_levels::set_level_info(std::vector<level_info *>& li)
{
    clear_level_info();
    _li = li;
}

void player_role_levels::fill_role_levels_list(client::levels_list_rsp &rsp)
{
	if(_li.empty())
	{
		return ;
	}
	int cur = static_cast<int>(get_cur_level());
	rsp.set_cur((uint32)cur);
	rsp.set_tail(_li.size());
	rsp.set_max_level(_li[_li.size() - 1]->id());
	if((uint32)cur > _li[_li.size() - 1]->id())
	{
		cur = _li[_li.size() - 1]->id();
	}
	const config_giantraid *cur_cg = CONFMGR->get_config_giantraid((uint32)cur);
	if(NULL == cur_cg)
	{
		return ;
	}
	for(int i = -1; i <= 1; ++i)
	{
		int id = cur + i;
		if(id <= 0)
		{
			continue;
		}
		
		const config_giantraid *cg = CONFMGR->get_config_giantraid((uint32)id);
		if(NULL == cg)
		{
			continue;
		}
		if(cur_cg->get_player_min_level() != cg->get_player_min_level() || cur_cg->get_player_max_level() != cg->get_player_max_level())
		{
			continue;
		}
		vector<client::level_info *>::iterator j = find_if(_li.begin(), _li.end(), cmp(id));
		if(j != _li.end())
		{
			client::level_info *li = rsp.add_li();
			li->CopyFrom(**j);
		}
	}
}

bool player_role_levels::levels_need_reset()
{
	if(_trl == NULL)
	{
		return true;
	}
	uint32 cur = get_cur_level();
	const config_giantraid *cg = CONFMGR->get_config_giantraid(cur);
	if(cg == NULL)
	{
		return true;
	}
	return cg->get_player_min_level() > _trl->old_level() || _trl->old_level() > cg->get_player_max_level();
}
