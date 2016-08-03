#include "player_role_cd_cooldown.h"

player_role_cd_cooldown::player_role_cd_cooldown(mysql::tb_role_cd_cooldown* b) : db_role_cd_cooldown(b)
{

}

player_role_cd_cooldown::~player_role_cd_cooldown()
{
    if (db_role_cd_cooldown != NULL) {
        delete db_role_cd_cooldown;
        db_role_cd_cooldown = NULL;
    }
}

int player_role_cd_cooldown::set_db_cd_cooldown(mysql::tb_role_cd_cooldown* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_cd_cooldown == b) {
		return 0;
	}

	if (db_role_cd_cooldown != NULL) {
		delete db_role_cd_cooldown;
	}

	db_role_cd_cooldown = b;
	parse_cd(db_role_cd_cooldown->cds());

    return 0;
}

int player_role_cd_cooldown::save_cd_cooldown(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_cd(out);
	if (count <= 0) {
		if (db_role_cd_cooldown == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_cd_cooldown == NULL) {
			db_role_cd_cooldown = new mysql::tb_role_cd_cooldown;
			db_role_cd_cooldown->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_cd_cooldown->set_cds(out);
	}

	mysql::tb_role_cd_cooldown* trb = new mysql::tb_role_cd_cooldown(*db_role_cd_cooldown);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

    return 0;
}

int player_role_cd_cooldown::attach_role_cd_cooldown(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}

	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_cd_cooldown(static_cast<mysql::tb_role_cd_cooldown*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}

bool player_role_cd_cooldown::add_cooldown_by_id(uint32 type_id)
{
	if (!add_cooldown_time(type_id)){
		return false;
	}

	return true;
}

uint32 get_cooldown_time(uint32 type_id)
{



	return 0;
}