#include "player_role_bows.h"
#include "config_bows.h"
#include "config_bow_exp.h"

player_role_bows::player_role_bows(mysql::tb_role_bows* p /* = NULL */)
        : _db(p)
{
    //init(p);
}

player_role_bows::~player_role_bows()
{
    if (_db != NULL) {
        delete _db;
        _db = NULL;
    }
}

int player_role_bows::attach(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }

        mysql::tb_role_bows* p = static_cast<mysql::tb_role_bows*>(pb_ctx[i].pb_msg);
        if (init(p) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

int player_role_bows::init(mysql::tb_role_bows* p)
{
    if (p == NULL || p == _db) {
        return -1;
    }

    _db = p;
    writer_type = WOTE_UPDATE;
    
    return 0;
}

int player_role_bows::save(amtsvr_context* ctx)
{
    if (_db == NULL || _db->role_id() == 0) {
        return -1;
    }

    mysql::tb_role_bows* p = new mysql::tb_role_bows(*_db);
    CACHE_KEY k(p->role_id(), 0);
    write(ctx, k, p);
    writer_type = WOTE_UPDATE;

    return 0;
}

int player_role_bows::create_bows(uint32 role_id)
{
	mysql::tb_role_bows* pm = mutable_db();
	pm->set_role_id(role_id);
	pm->set_bow_id(1);
	pm->set_advance_num(0);
	pm->set_advance_time(0);
	pm->set_advance_bless(0);
	pm->set_bow_level(1);
	pm->set_bow_exp(0);
	uint32 bow_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_bows_mgr)->calculate_bow_power(pm->bow_id(),pm->bow_level())));
	pm->set_bow_power(bow_power);


	return 0;
}

const mysql::tb_role_bows& player_role_bows::get_db()
{
    return *mutable_db();
}

mysql::tb_role_bows* player_role_bows::mutable_db()
{
    if (_db == NULL) {
        _db = new mysql::tb_role_bows;
        _db->set_bow_id(0);
        writer_type = WOTE_INSERT;
    }

    return _db;
}

void player_role_bows::upgrate_bow_level(uint32 player_level,uint32 add_exp)
{
	mutable_db()->set_bow_exp(get_db().bow_exp() + add_exp);
	while (true) {
		const config_bow_exp* pcu = CONFMGR->get_config_bow_exp(get_db().bow_level());
		const config_bow_exp* next = CONFMGR->get_config_bow_exp(get_db().bow_level() +1);
		if (pcu == NULL || next == NULL || pcu->get_upgrade_exp() == 0 || static_cast<uint32>(pcu->get_upgrade_exp()) > get_db().bow_exp()/* || player_level <= get_db().mount_level()*/
			|| static_cast<uint32>(next->get_bow_id()) > get_db().bow_id()) {

			return;
		}
		else{
			mutable_db()->set_bow_exp(get_db().bow_exp() - pcu->get_upgrade_exp());
			mutable_db()->set_bow_level(get_db().bow_level() + 1);
			uint32 bow_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_bows_mgr)->calculate_bow_power(mutable_db()->bow_id(),mutable_db()->bow_level())));
			mutable_db()->set_bow_power(bow_power);
		}
	}
}
