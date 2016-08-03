#include "player_role_auto_raid.h"

player_role_auto_raid::player_role_auto_raid(uint32 role_id, mysql::tb_role_auto_raid* auto_raid /* = NULL */)
    : _auto_raid(auto_raid)
{
    init(role_id, auto_raid);
}

player_role_auto_raid::~player_role_auto_raid()
{
    if (_auto_raid != NULL) {
        delete _auto_raid;
        _auto_raid = NULL;
    }
}

int player_role_auto_raid::save(amtsvr_context* ctx)
{
    if (_auto_raid == NULL || _auto_raid->role_id() == 0) {
        return -1;
    }

    mysql::tb_role_auto_raid* trs = new mysql::tb_role_auto_raid(*_auto_raid);
    CACHE_KEY k(trs->role_id(), 0);
    string out;
    serialize(out);
    trs->set_info(out);
    write(ctx, k, trs);
    writer_type = WOTE_UPDATE;

    return 0;
}

int player_role_auto_raid::init(uint32 role_id, mysql::tb_role_auto_raid* s)
{
    if (s == NULL && _auto_raid == NULL) {
        _auto_raid = new mysql::tb_role_auto_raid;
        _auto_raid->set_role_id(role_id);
        writer_type = WOTE_INSERT;
    } else if (s != NULL) {
        _auto_raid  = s;
        writer_type = WOTE_UPDATE;
    } else {
        writer_type = WOTE_NULL;
        return -1;
    }

    parse(_auto_raid->info());

    return 0;
}

void player_role_auto_raid::clear()
{
    _info.Clear();
}

int player_role_auto_raid::parse(const string& in)
{
    if (in.empty()) {
        return -1;
    }

    clear();
    char* ptr     = (char*)in.c_str();
    _info.ParseFromArray(ptr, in.size());

    return 0;
}

int player_role_auto_raid::serialize(string& out)
{
    char buf[MAX_AUTO_RAID_BOLB_LEN];
    uint32 offset = 0;
    uint32 msg_len = _info.ByteSize();
    _info.SerializeToArray((void*)(buf + offset), msg_len);
    offset += msg_len;
    out.append(buf, offset);

    return 0;
}

void player_role_auto_raid::set_auto_raid(uint32 raid_id, uint32 need_time, uint32 count, uint32 type)
{
    _info.set_id(raid_id);
    _info.set_type(type);
    _info.set_start_time((uint32)time(NULL));
    _info.set_need_time(need_time);
    _info.set_frozen_silver(1);
    _info.set_frozen_physical_power(5);
    _info.set_count(count);
    _info.set_monster(0);
    _info.set_exp(0);
    _info.set_coin(0);
    _info.set_military_strategy(0);
}

