#include "player_role_gem.h"

player_role_gem::player_role_gem(mysql::tb_role_gem* b) : db_role_gem(b)
{

}

player_role_gem::~player_role_gem()
{
    if (db_role_gem != NULL) {
        delete db_role_gem;
        db_role_gem = NULL;
    }
}

int player_role_gem::set_db_gem(mysql::tb_role_gem* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_gem == b) {
		return 0;
	}

	if (db_role_gem != NULL) {
		delete db_role_gem;
	}

	db_role_gem = b;
	parse_gem(db_role_gem->gems());

    return 0;
}

int player_role_gem::save_gem(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_gem(out);
	if (count <= 0) {
		if (db_role_gem == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_gem == NULL) {
			db_role_gem = new mysql::tb_role_gem;
			db_role_gem->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_gem->set_gems(out);
	}

	mysql::tb_role_gem* trb = new mysql::tb_role_gem(*db_role_gem);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

    return 0;
}

int player_role_gem::attach_role_gem(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}
	
	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_gem(static_cast<mysql::tb_role_gem*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}

void player_role_gem::fill_gem_pos_info(uint32 gem_pos,gem_level_up_rsp& rsp_msg)
{
	std::map<uint32, client::gem_info> gem_info_map_;
	gem_info_map_.clear();
	get_pos_gem_list(gem_pos,gem_info_map_);

	std::map<uint32, client::gem_info>::iterator it = gem_info_map_.begin();

	for (; it != gem_info_map_.end(); ++it){
		client::gem_info* info = rsp_msg.add_info();
		info->CopyFrom(it->second);
	}

}

bool player_role_gem::check_is_max(uint32 gem_pos)
{
	std::map<uint32, client::gem_info> pos_gem_list_;
	pos_gem_list_.clear();

	get_pos_gem_list(gem_pos,pos_gem_list_);

	uint32 gem_size = pos_gem_list_.size();

	if (gem_size == 0){
		return true;
	}

	uint32 max_count = 0;

	std::map<uint32, client::gem_info>::iterator it = pos_gem_list_.begin();
	for (;it != pos_gem_list_.end();++it){
		if (gem_level_is_max(it->second.gem_type(),it->second.gem_level())){
			++max_count;
		}
	}

	if (max_count == gem_size){
		return true;
	}

	return false;
}