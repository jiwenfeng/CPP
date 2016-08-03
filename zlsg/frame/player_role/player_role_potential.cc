#include "player_role_potential.h"

player_role_potential::player_role_potential(mysql::tb_role_potential* b) : db_role_potential(b)
{

}

player_role_potential::~player_role_potential()
{
    if (db_role_potential != NULL) {
        delete db_role_potential;
        db_role_potential = NULL;
    }
}

int player_role_potential::set_db_potential(mysql::tb_role_potential* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_potential == b) {
		return 0;
	}

	if (db_role_potential != NULL) {
		delete db_role_potential;
	}

	db_role_potential = b;
	parse_potential(db_role_potential->potentials());

    return 0;
}

int player_role_potential::save_potential(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_potential(out);
	if (count <= 0) {
		if (db_role_potential == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_potential == NULL) {
			db_role_potential = new mysql::tb_role_potential;
			db_role_potential->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_potential->set_potentials(out);
	}

	mysql::tb_role_potential* trb = new mysql::tb_role_potential(*db_role_potential);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

    return 0;
}

int player_role_potential::attach_role_potential(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}

	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_potential(static_cast<mysql::tb_role_potential*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}
