#include "player_role_treasure.h"


player_role_treasure::player_role_treasure(mysql::tb_role_treasure* b) : db_role_treasure(b)
{

}

player_role_treasure::~player_role_treasure()
{
	if (db_role_treasure != NULL) {
		delete db_role_treasure;
		db_role_treasure = NULL;
	}
}

int player_role_treasure::set_db_treasure(mysql::tb_role_treasure* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_treasure == b) {
		return 0;
	}

	if (db_role_treasure != NULL) {
		delete db_role_treasure;
	}

	db_role_treasure = b;
	parse_treasure(db_role_treasure->treasures());

	return 0;
}

int player_role_treasure::save_treasure(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_treasure(out);
	if (count <= 0) {
		if (db_role_treasure == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_treasure == NULL) {
			db_role_treasure = new mysql::tb_role_treasure;
			db_role_treasure->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_treasure->set_treasures(out);
	}

	mysql::tb_role_treasure* trb = new mysql::tb_role_treasure(*db_role_treasure);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

	return 0;

}

int player_role_treasure::attach_role_treasure(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}

	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_treasure(static_cast<mysql::tb_role_treasure*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}