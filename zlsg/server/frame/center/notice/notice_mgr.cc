#include "notice_mgr.h"



notice_mgr::notice_mgr()
{
	_msg_id = 0;
}

notice_mgr::~notice_mgr()
{
	std::map<uint32, mysql::tb_center_notice*>::iterator i = _notices_map.begin();
	for (; i != _notices_map.end(); ++i) {
		delete i->second;
	}

	_notices_map.clear();
	_update.clear();
	_insert.clear();
	_delete.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int notice_mgr::init(struct cmq_t* l)
{
	CUR* cur = (CUR*)CONFMGR->get_center_notice_head();
	while (cur != NULL) {
		google::protobuf::Message* p = cur->mutable_val().pop_msg();
		mysql::tb_center_notice* cn = static_cast<mysql::tb_center_notice*>(p);
		cur = (CUR*)cur->get_next();
		if (cn == NULL) {
			continue;
		}

		if (set_msg_id(*cn) != 0) {
			delete cn;
			continue;
		}

		_notices_map.insert(std::pair<uint32,tb_center_notice*>(cn->msg_id(),cn));
	}

	return 0;
}

int notice_mgr::save(amtsvr_context* ctx)
{
	writer_type = WOTE_UPDATE;
	for(size_t i = 0; i < _update.size(); ++i){
		mysql::tb_center_notice *tcd = new mysql::tb_center_notice(*_update[i]);	
		CACHE_KEY k(tcd->msg_id(), 0);
		write(ctx, k, tcd);
	}

	writer_type = WOTE_INSERT;
	for(size_t i = 0; i < _insert.size(); ++i){
		mysql::tb_center_notice *cn = new mysql::tb_center_notice(*_insert[i]);	
		CACHE_KEY k(cn->msg_id(), 0);
		write(ctx, k, cn);
	}

	writer_type = WOTE_REMOVE;
	for(size_t i = 0; i < _delete.size(); ++i){
		mysql::tb_center_notice *de = new mysql::tb_center_notice(*_delete[i]);	
		CACHE_KEY k(de->msg_id(), 0);
		write(ctx, k, de);
	}

	_update.clear();
	_insert.clear();
	_delete.clear();

	return 0;
}

int notice_mgr::set_msg_id(const mysql::tb_center_notice& db_notice)
{
	uint32 msg_id = db_notice.msg_id();

	uint32 max_sid = get_msg_id();

	if (msg_id > max_sid){
		set(msg_id);
	}

	return 0;
}

bool notice_mgr::add_notice_record(string content,uint32 start_time,uint32 close_time,uint32 interval_time)
{
	mysql::tb_center_notice* cn = new mysql::tb_center_notice;
	cn->set_msg_id(get());
	cn->set_content(content);
	cn->set_start_time(start_time);
	cn->set_close_time(close_time);
	cn->set_interval_time(interval_time);
	cn->set_is_over(0);
	cn->set_brocast_time(0);
	_notices_map.insert(std::pair<uint32,mysql::tb_center_notice*>(cn->msg_id(),cn));
	_insert.push_back(cn);

	return true;
}

void notice_mgr::set_over(uint32 msg_id)
{
	std::map<uint32,mysql::tb_center_notice*>::iterator it = _notices_map.find(msg_id);

	if (it != _notices_map.end()){
		it->second->set_is_over(1);
		if (it->second->is_over() == 1){
			it->second->set_is_over(0);
		}else{
			it->second->set_is_over(1);
		}

		_update.push_back(it->second);
	}
}

void notice_mgr::set_broast_time(uint32 msg_id)
{
	std::map<uint32,mysql::tb_center_notice*>::iterator it = _notices_map.find(msg_id);

	if (it != _notices_map.end()){
		it->second->set_brocast_time((uint32)time(NULL));
		_update.push_back(it->second);
	}

}

void notice_mgr::remove_notice(uint32 msg_id)
{
	std::map<uint32,mysql::tb_center_notice*>::iterator it = _notices_map.find(msg_id);
	if ( it != _notices_map.end()){
		_delete.push_back(it->second);
		_notices_map.erase(it);
	}
}