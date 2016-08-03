#include "arena_record.h"

#define BUFFLEN 65535

#define RECORD_NEW 1
#define RECORD_OLD 2

arena_record::arena_record(mysql::tb_arena_record *tar):_tar(tar)
{
	_r.resize(MAX_RECORD_NUM);
//	memset(_r, '\0', MAX_RECORD_NUM);
	_tail = 0;
	_need_save = false;
}

arena_record::~arena_record()
{
	if(_tar)
	{
		delete _tar;
		_tar = NULL;
	}
	for(int i = 0; i < MAX_RECORD_NUM; ++i)
	{
		if(!_r[i])
		{
			break;
		}
		delete _r[i];
	}
	_r.clear();
//	memset(_r, '\0', MAX_RECORD_NUM);
}

void arena_record::mod_record_status(int status)
{
	if((int)_tar->status() == status)
	{
		return;
	}
	_tar->set_status(status);
}

int arena_record::get_record_status()
{
	return _tar->status();
}

void arena_record::mod_writer_type(WRITER_OPERATE_TYPE_E status)
{
	writer_type = status;
}

void arena_record::push(client::arena_record_info *tar)
{
	int i = _tail % MAX_RECORD_NUM;
	if(_r[i])
	{
		delete _r[i];
		_r[i] = NULL;
	}
	_r[i] = tar;
	++_tail;
}

int arena_record::serialize(string &out)
{
	char buf[BUFFLEN] = {0};
	uint32 offset = 0;
	int count = 0;
	for(int i = 0; i < MAX_RECORD_NUM; ++i)
	{
		if(!_r[i])
		{
			break;
		}
		uint32 len = _r[i]->ByteSize();
		if(offset + sizeof(uint16) + len >= BUFFLEN)
		{
			break;
		}
		*(uint16 *)(buf + offset) = (uint16)len;
		offset += sizeof(uint16);
		_r[i]->SerializeToArray((void *)(buf + offset), len);
		offset += len;
		++count;
	}
	out.append(buf, offset);
	return count;
}

void arena_record::unserialize(const string &str)
{
	const char *ptr = str.c_str();
	int len = str.length();
	int offset = 0;
	while(offset < len)
	{
		uint16 msg_len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::arena_record_info *ari = new client::arena_record_info;
		if(!ari->ParseFromArray(ptr + offset, msg_len))
		{
			delete ari;
			break;
		}
		offset += msg_len;
		push(ari);
	}
}

void arena_record::save(uint32 role_id, amtsvr_context *ctx)
{
	if(!_need_save)
	{
		return;
	}
	string out;
	int count = serialize(out);
	if(count == 0)
	{
		writer_type = WOTE_REMOVE;
	}
	_tar->set_log(out);
	mysql::tb_arena_record *tar = new mysql::tb_arena_record(*_tar);
	CACHE_KEY k(tar->role_id(), 0);
	write(ctx, k, tar);
	writer_type = WOTE_UPDATE;
	_need_save = false;
}

arena_record_mgr::arena_record_mgr():_ctx(NULL)
{
}

arena_record_mgr::~arena_record_mgr()
{
	std::map<uint32, arena_record *>::iterator it = _record.begin();
	for(; it != _record.end(); ++it)
	{
		delete it->second;
	}
	_record.clear();
}

int arena_record_mgr::init()
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_arena_record_head();
	while(cur != NULL)
	{
		mysql::tb_arena_record *tar = static_cast<mysql::tb_arena_record *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tar)
		{
			continue;
		}
		arena_record *r = new arena_record(tar);
		r->unserialize(tar->log());
		_record.insert(make_pair(tar->role_id(), r));
	}
	return 0;
}

void arena_record_mgr::save(amtsvr_context *ctx)
{
	std::map<uint32, arena_record *>::iterator it = _record.begin();
	for(; it != _record.end(); ++it)
	{
		it->second->save(it->first, ctx);
	}
}

void arena_record_mgr::get_record(uint32 role_id, vector<client::arena_record_info> &v)
{
	std::map<uint32, arena_record *>::iterator it = _record.find(role_id);
	if(it == _record.end())
	{
		return ;
	}
	for(int i = 0; i < MAX_RECORD_NUM; ++i)
	{
		if(!(it->second->_r[i]))
		{
			break ;
		}
		v.push_back(*(it->second->_r[i]));
	}
	it->second->mod_record_status(RECORD_OLD);
	it->second->_need_save = true;
}

void arena_record_mgr::attach_ctx(amtsvr_context *ctx)
{
	_ctx = ctx;
}

void arena_record_mgr::add_record(uint32 role_id, const string &name, uint32 rank, int action, int ret)
{
	std::map<uint32, arena_record *>::iterator it = _record.find(role_id);
	arena_record *ar = NULL;
	if(it == _record.end())
	{
		mysql::tb_arena_record *tar = new mysql::tb_arena_record;
		tar->set_role_id(role_id);
		tar->set_status(RECORD_NEW);
		ar = new arena_record(tar);
		ar->mod_writer_type(WOTE_INSERT);
		_record.insert(make_pair(role_id, ar));
	}
	else
	{
		ar = it->second;
	}
	client::arena_record_info *ari = new client::arena_record_info;
	ari->set_name(name);
	ari->set_time((uint32)time(NULL));
	ari->set_action(action);
	ari->set_ret(ret);
	ari->set_rank(rank);
	ar->push(ari);
	ar->mod_record_status(RECORD_NEW);
	ar->_need_save = true;
	ar->save(role_id, _ctx);
}

int arena_record_mgr::get_record_status(uint32 role_id)
{
	std::map<uint32, arena_record *>::iterator it = _record.find(role_id);
	if(it == _record.end())
	{
		return 2;
	}
	return it->second->get_record_status();
}
