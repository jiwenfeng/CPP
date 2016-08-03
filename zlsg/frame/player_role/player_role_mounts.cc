#include "config_mounts.h"
#include "config_mount_exp.h"
#include "client.mounts.pb.h"
#include "player_role_mounts.h"

player_role_mounts::player_role_mounts(mysql::tb_role_mounts* p /* = NULL */)
        : _db(p)
{
    //init(p);
}

player_role_mounts::~player_role_mounts()
{
    if (_db != NULL) {
        delete _db;
        _db = NULL;
    }
}

int player_role_mounts::attach(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }

        mysql::tb_role_mounts* p = static_cast<mysql::tb_role_mounts*>(pb_ctx[i].pb_msg);
        if (init(p) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

int player_role_mounts::init(mysql::tb_role_mounts* p)
{
    if (p == NULL || p == _db) {
        return -1;
    }

    _db = p;
    writer_type = WOTE_UPDATE;
    
    return 0;
}

int player_role_mounts::save(amtsvr_context* ctx)
{
    if (_db == NULL || _db->role_id() == 0) {
        return -1;
    }

    mysql::tb_role_mounts* p = new mysql::tb_role_mounts(*_db);
    CACHE_KEY k(p->role_id(), 0);
    write(ctx, k, p);
    writer_type = WOTE_UPDATE;

    return 0;
}

const mysql::tb_role_mounts& player_role_mounts::get_db()
{
    return *mutable_db();
}

mysql::tb_role_mounts* player_role_mounts::mutable_db()
{
    if (_db == NULL) {
        _db = new mysql::tb_role_mounts;
        _db->set_mount_id(0);
        writer_type = WOTE_INSERT;
    }

    return _db;
}

void fill_mounts_msg(const mysql::tb_role_mounts& mounts, client::mounts_msg& mmsg);
void player_role_mounts::upgrate_mount_level(uint32 player_level,uint32 add_exp)
{
	mutable_db()->set_mount_exp(get_db().mount_exp() + add_exp);
	while (true) {
		const config_mount_exp* pcu = CONFMGR->get_mount_exp_mgr().get_config_mount_exp(get_db().mount_level());
		const config_mount_exp* next = CONFMGR->get_mount_exp_mgr().get_config_mount_exp(get_db().mount_level() +1);
		if (pcu == NULL || next == NULL || pcu->get_upgrate_exp() == 0 || static_cast<uint32>(pcu->get_upgrate_exp()) > get_db().mount_exp()/* || player_level <= get_db().mount_level()*/
			|| static_cast<uint32>(next->get_mount_idx_limit()) > get_db().mount_id()) {

			return;
		}
		else{
			mutable_db()->set_mount_exp(get_db().mount_exp() - pcu->get_upgrate_exp());
			mutable_db()->set_mount_level(get_db().mount_level() + 1);
			uint32 mount_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_mounts_mgr)->calculate_mount_power(mutable_db()->mount_id(),mutable_db()->mount_level())));
			mutable_db()->set_mount_power(mount_power);
		}
	}
}
