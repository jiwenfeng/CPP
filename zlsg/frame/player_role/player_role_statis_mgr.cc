#include "player_role_statis_mgr.h"

player_role_statis_mgr::player_role_statis_mgr()
{
    _prs.resize(PRSTE_MAX);
    _prs[PRSTE_DAY_ONCE_LIMIT             ].set_type(PRSTE_DAY_ONCE_LIMIT             , PRSUTE_DAY_UPDATE    );
    _prs[PRSTE_USER_STATE                 ].set_type(PRSTE_USER_STATE                 , PRSUTE_NOT_UPDATE    );
    _prs[PRSTE_ENTER_RAID_COUNT           ].set_type(PRSTE_ENTER_RAID_COUNT           , PRSUTE_DAY_UPDATE    );
    _prs[PRSTE_AUTO_RAID_COUNT            ].set_type(PRSTE_AUTO_RAID_COUNT            , PRSUTE_DAY_UPDATE    );
    _prs[PRSTE_BLACK_REFRESH_COUNT        ].set_type(PRSTE_BLACK_REFRESH_COUNT        , PRSUTE_NOT_UPDATE    );
    _prs[PRSTE_BLACK_FRIST_REFRESH        ].set_type(PRSTE_BLACK_FRIST_REFRESH        , PRSUTE_NOT_UPDATE    );
    _prs[PRSTE_SIGN_IN                    ].set_type(PRSTE_SIGN_IN                    , PRSUTE_MONTH_UPDATE  );
    _prs[PRSTE_SIGN_IN_AWARD              ].set_type(PRSTE_SIGN_IN_AWARD              , PRSUTE_MONTH_UPDATE  );
    _prs[PRSTE_ARENA_RECOVER_TIMES        ].set_type(PRSTE_ARENA_RECOVER_TIMES        , PRSUTE_DAY_UPDATE    );
    _prs[PRSTE_ARENA_CHALLENGE_TIMES      ].set_type(PRSTE_ARENA_CHALLENGE_TIMES      , PRSUTE_DAY_UPDATE    );
    _prs[PRSTE_ARENA_RESET_CHALLENGE_TIMES].set_type(PRSTE_ARENA_RESET_CHALLENGE_TIMES, PRSUTE_DAY_UPDATE    );
	_prs[PRSTE_LEVELS_RESET_COUNT         ].set_type(PRSTE_LEVELS_RESET_COUNT         , PRSUTE_DAY_UPDATE    );
	_prs[PRSTE_MOUNT_FRIST_RIDE           ].set_type(PRSTE_MOUNT_FRIST_RIDE           , PRSUTE_NOT_UPDATE    );
	_prs[PRSTE_LAND_DAY                   ].set_type(PRSTE_LAND_DAY                   , PRSUTE_NOT_UPDATE    );
	_prs[PRSTE_LAND_DAY_REWARD            ].set_type(PRSTE_LAND_DAY_REWARD            , PRSUTE_NOT_UPDATE    );
	_prs[PRSTE_ARENA_CD                   ].set_type(PRSTE_ARENA_CD                   , PRSUTE_NOT_UPDATE    );
	_prs[PRSTE_OFFLINE_TIMELONG			  ].set_type(PRSTE_OFFLINE_TIMELONG, PRSUTE_DAY_UPDATE);
	_prs[PRSTE_TOTAL_OFFLINE_TL			  ].set_type(PRSTE_TOTAL_OFFLINE_TL, PRSUTE_DAY_UPDATE);
	_prs[PRSTE_CYCLE_INVITE_COUNT		  ].set_type(PRSTE_CYCLE_INVITE_COUNT, PRSUTE_DAY_UPDATE);
#ifndef __LIB_MINI_SERVER__
    target.init(&_prs);
#endif
}

player_role_statis_mgr::~player_role_statis_mgr()
{

}

int player_role_statis_mgr::init(uint32 role_id)
{
    for (size_t i = 0; i < _prs.size(); ++i) {
        _prs[i].set_role_id(role_id);
    }

    return 0;
}

int player_role_statis_mgr::attach_role_statis(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[0].pb_msg == NULL) {
            continue;
        }

        if (push(static_cast<mysql::tb_role_statis*>(pb_ctx[i].pb_msg)) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

int player_role_statis_mgr::push(mysql::tb_role_statis* trs)
{
    if (trs == NULL) {
        return -1;
    }

    uint32 statis_type = trs->statis_type();
    if (statis_type >= PRSTE_MAX) {
        return -2;
    }

    if (_prs[statis_type].attach(trs) != 0) {
        return -3;
    }

    return 0;
}

player_role_statis* player_role_statis_mgr::get(PLAYER_ROLE_STATIS_TYPE_E type)
{
    uint32 size = _prs.size();
    if ((uint32)type >= size) {
        return NULL;
    }

    return &_prs[type];
}

void player_role_statis_mgr::write(amtsvr_context* ctx)
{
    uint32 size = _prs.size();
    for (uint32 i = 0; i < size; ++i){
        _prs[i].write(ctx);
    }
}

bool player_role_statis_mgr::is_day_init()
{
    uint32 val = 0;
    _prs[PRSTE_DAY_ONCE_LIMIT].get_val(val);
    return (val & 0x00000001) == 0;
}

void player_role_statis_mgr::set_day_init()
{
    uint32 val = 0;
    _prs[PRSTE_DAY_ONCE_LIMIT].get_val(val);
    _prs[PRSTE_DAY_ONCE_LIMIT].set_val(val |= 0x00000001);
	_prs[PRSTE_BLACK_REFRESH_COUNT].set_val(0);
}

void   player_role_statis_mgr::add_land_day()
{
	uint32 val = 0;
	_prs[PRSTE_LAND_DAY].get_val(val);
	val += 1;
	_prs[PRSTE_LAND_DAY].set_val(val);
}

int player_role_statis_mgr::init_vip_trans(uint32 num)
{
    if (_prs[PRSTE_DAY_VIP_TRANS].is_same_day((uint32)time(NULL))) {
        return -1;
    }

    return reset_vip_trans(num);
}

int player_role_statis_mgr::reset_vip_trans(uint32 num)
{
    _prs[PRSTE_DAY_VIP_TRANS].set_val(num);

    return 0;
}

uint32 player_role_statis_mgr::get_vip_trans()
{
    return _prs[PRSTE_DAY_VIP_TRANS].get();
}

int player_role_statis_mgr::sub_vip_trans(int num/* = 1*/)
{
    uint32 val = 0;
    _prs[PRSTE_DAY_VIP_TRANS].get_val(val);
    if (val == 0) {
        return -1;
    }

    _prs[PRSTE_DAY_VIP_TRANS].set_val(val - num);

    return 0;
}

bool player_role_statis_mgr::vip_premiums()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 5);
}

//int player_role_statis_mgr::vip_fresh_task()
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    int count = val & 0x0000000F;
//    if (count < 10) {
//        _prs[PRSTE_DAY_HALF_BYTE_LIMIT].add_val(1);
//    }
//
//    return count;
//}

uint32 player_role_statis_mgr::get_convoy_refresh_times()
{
    uint32 val = 0;
    _prs[PRSTE_CONVOY].get_val(val);

    return val & 0x000000FF;
}

void player_role_statis_mgr::add_convoy_refresh_times(uint32 num)
{
    uint32 val = 0;
    _prs[PRSTE_CONVOY].get_val(val);
    val += num;
    _prs[PRSTE_CONVOY].set_val(val);
}

uint32 player_role_statis_mgr::get_convoy_rob_times()
{
    uint32 val = 0;
    _prs[PRSTE_CONVOY].get_val(val);

    return ((val & 0x0000FF00)>>8);
}

void player_role_statis_mgr::add_convoy_rob_times(uint32 num)
{
    uint32 val = 0;
    _prs[PRSTE_CONVOY].get_val(val);
    val += (num<<8);
    _prs[PRSTE_CONVOY].set_val(val);
}

uint32 player_role_statis_mgr::get_hide_raid(uint32 pos)
{
    uint32 val = 0;
    _prs[PRSTE_DAY_HIDE_RAID].get_val(val);

    return (val >> pos) & 0x00000001;
}

int player_role_statis_mgr::set_hide_raid(uint32 pos)
{
    uint32 val = 0;
    _prs[PRSTE_DAY_HIDE_RAID].get_val(val);
    val = val | (0x00000001 << pos);
    _prs[PRSTE_DAY_HIDE_RAID].set_val(val);

    return 0;
}

//void player_role_statis_mgr::add_join_world_boss(uint32 add_count/* = 1*/)
//{
//    index_add(PRSTE_JOIN_WORLD_BOSS, add_count);
//}

void player_role_statis_mgr::add_join_siege_battle(uint32 add_count/* = 1*/)
{
   // index_add(PRSTE_JOIN_SIEGE_BATTLE, add_count);
}

void player_role_statis_mgr::add_join_domain_battle(uint32 add_count/* = 1*/)
{
   // index_add(PRSTE_JOIN_DOMAIN_BATTLE, add_count);
}

bool player_role_statis_mgr::online_day(uint32 day)
{
    if (day > 31) {
        return false;
    }

    return  index_pos_bit(PRSTE_MONTH_ONLINE, 0x00000001 << day);
}

uint32 player_role_statis_mgr::online_day_count()
{
    uint32 count = 0;
    uint32 val = 0;
    uint32 index = PRSTE_MONTH_ONLINE;
    _prs[index].get_val(val);
    for (uint32 i = 0; i < 32; ++i) {
        if (((val >> i) & 0x00000001) == 1) {
            ++count;
        }
    }

    return count;
}

bool player_role_statis_mgr::online_day_award(uint32 day)
{
    if (day > 31) {
        return false;
    }

    return index_pos_bit(PRSTE_MONTH_ONLINE_AWARD, 0x00000001 << day);
}

void player_role_statis_mgr::add_week_online_time(uint32 add_time)
{
    uint32 last_time = _prs[PRSTE_WEEK_CUR_ONLINE].get_last_time();
    uint32 now_time  = (uint32)time(NULL);
    if (now_time > last_time) {
        add_time = (now_time - last_time) < add_time ? (now_time - last_time) : add_time;
    }

    uint32 old_time = _prs[PRSTE_WEEK_CUR_ONLINE].get();
    if (!_prs[PRSTE_WEEK_CUR_ONLINE].is_same_week(now_time)) {
        if (old_time != 0) {
            _prs[PRSTE_WEEK_LAST_ONLINE].set_val(old_time);
        }

        _prs[PRSTE_WEEK_CUR_ONLINE].set_val(0);
        old_time = 0;
    }

    _prs[PRSTE_WEEK_CUR_ONLINE].set_val(old_time + add_time);
}

bool player_role_statis_mgr::last_week_online_time()
{
    return  index_pos_bit(PRSTE_WEEK_ONCE_LIMIT, 0x00000001);
}

uint32 player_role_statis_mgr::week_time()
{
    uint32 index = PRSTE_WEEK_LAST_ONLINE;
    return _prs[index].get();
}

void player_role_statis_mgr::add_online_meditation_time(uint32 add_time)
{
    uint32 last_time = _prs[PRSTE_ONLINE_TIME].get_last_time();
    uint32 now_time  = (uint32)time(NULL);
    if (now_time > last_time) {
        add_time = (now_time - last_time) < add_time ? (now_time - last_time) : add_time;
    }

    uint32 all_time = _prs[PRSTE_ONLINE_TIME].get() + add_time;
    all_time = all_time < 43200 ? all_time : 43200;
    _prs[PRSTE_ONLINE_TIME].set_val(all_time);
}

void player_role_statis_mgr::add_offline_meditation_time()
{
    uint32 last_time = _prs[PRSTE_ONLINE_TIME].get_last_time();
    uint32 now_time  = (uint32)time(NULL);
    if (last_time < now_time && last_time != 0) {
        uint32 all_time = _prs[PRSTE_ONLINE_TIME].get() + (now_time - last_time);
        all_time = all_time < 43200 ? all_time : 43200;
        _prs[PRSTE_ONLINE_TIME].set_val(all_time);
    }
}

uint32 player_role_statis_mgr::meditation_time()
{
    uint32 val = 0;
    uint32 index = PRSTE_ONLINE_TIME;
    _prs[index].get_val(val);
    _prs[index].set_val(0);

    return val;
}

bool   player_role_statis_mgr::welfare_gifts(uint32 index)
{
    return index_pos_bit(PRSTE_WELFARE_GIFTS, 0x00000001 << index);
}

bool player_role_statis_mgr::yellow_vip_every_day()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 3);
}

bool player_role_statis_mgr::yellow_vip_year_every_day()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 4);
}

bool player_role_statis_mgr::yellow_vip_gifts(uint32 index)
{
    return index_pos_bit(PRSTE_YELLOW_VIP_GIFTS, 0x00000001 << index);
}

void player_role_statis_mgr::add_online_time(uint32 add_time /* = 60 */)
{
    uint32 last_time = _prs[PRSTE_ONLINE_GIFTS_TIME].get_last_time();
    uint32 now_time  = (uint32)time(NULL);
    if (now_time > last_time) {
        add_time = now_time - last_time;
    }

    uint32 all_time = _prs[PRSTE_ONLINE_GIFTS_TIME].get() + add_time;
    _prs[PRSTE_ONLINE_GIFTS_TIME].set_val(all_time);

    if (_prs[PRSTE_ONLINE_GIFTS_RECORD].get() & 0x00000010) {
        if (!_prs[PRSTE_ONLINE_GIFTS_RECORD].is_same_day(now_time)) {
            _prs[PRSTE_ONLINE_GIFTS_RECORD].set_val(0);
        }
    }
}

bool player_role_statis_mgr::online_gifts_record(uint32 index)
{
    bool same_day = true;
    uint32 now = (uint32)time(NULL);
    same_day = _prs[PRSTE_ONLINE_GIFTS_RECORD].is_same_day(now);

    uint32 val = 0;
    bool   ret = false;
    _prs[PRSTE_ONLINE_GIFTS_RECORD].get_val(val);
    if ((val & (0x00000001 << index)) == 0) {
        val |= (0x00000001 << index);
        ret = true;
        _prs[PRSTE_ONLINE_GIFTS_RECORD].set_val(val);
    } else if (index == 4) {
        ++index;
        if ((val & (0x00000001 << index)) == 0) {
            val |= (0x00000001 << index);
            ret = true;
            _prs[PRSTE_ONLINE_GIFTS_RECORD].set_val(val);
        }
    }

    if (ret) {
        _prs[PRSTE_ONLINE_GIFTS_TIME].set_val(0);
    }

    if (!same_day && ret && index != 0) {
        _prs[PRSTE_ONLINE_GIFTS_RECORD].set_val(0);
    }

    return ret;
}

void player_role_statis_mgr::add_test_time(uint32 add_time)
{
    return index_add(PRSTE_TEST_GIFTS_TIME, add_time);
}

bool player_role_statis_mgr::test_gifts_record(uint32 index)
{
    bool b = index_pos_bit(PRSTE_TEST_GIFTS_RECORD, 0x00000001 << index);
    if (b) {
         _prs[PRSTE_TEST_GIFTS_TIME].set_val(0);
    }

    return b;
}

void player_role_statis_mgr::add_activity(uint32 count)
{
    uint32 val = 0;
    _prs[PRSTE_DAY_ACTIVITY].get_val(val);
    _prs[PRSTE_DAY_ACTIVITY].set_val(val + count);
}

uint32 player_role_statis_mgr::get_activity()
{
    uint32 val = 0;
    _prs[PRSTE_DAY_ACTIVITY].get_val(val);
    return val;
}

bool player_role_statis_mgr::activity_gifts(uint32 index)
{
    return index_pos_bit(PRSTE_DAY_ACTIVITY_GIFTS, 0x00000001 << index);
}

bool player_role_statis_mgr::family_donate()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 6);
}

bool player_role_statis_mgr::family_welfare()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 7);
}

bool player_role_statis_mgr::family_raid()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 10);
}

bool player_role_statis_mgr::family_boss()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 11);
}

bool player_role_statis_mgr::secrete_elite()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 12);
}

bool player_role_statis_mgr::world_boss()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 9);
}

bool player_role_statis_mgr::add_day_online(uint32 add_time)
{
    uint32 val = 0;
    _prs[PRSTE_DAY_ONLINE_TIME].get_val(val);
    val += add_time;
    _prs[PRSTE_DAY_ONLINE_TIME].set_val(val);
    return val > 10800 && index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 8);
}

bool player_role_statis_mgr::hide_push_fig()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 13);
}

bool player_role_statis_mgr::join_spa()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 15);
}

bool player_role_statis_mgr::join_arena()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 16);
}

//uint32 player_role_statis_mgr::exchange_tael()
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    return ((val & 0x000000F0) >> 4);
//}

//void player_role_statis_mgr::add_exchange_tael()
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].set_val(val + 0x00000010);
//}

bool player_role_statis_mgr::honor_return()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 14);
}

#ifndef __LIB_MINI_SERVER__
bool player_role_statis_mgr::target_gifts(uint32 index)
{
    if (index == 0) {
        return false;
    }

    --index;
    if (index < 32) {
        return index_pos_bit(PRSTE_TARGET_GIFTS_0, 0x00000001 << index );
    } else {
        index -= 32;
        return index_pos_bit(PRSTE_TARGET_GIFTS_1, 0x00000001 << index);
    }
}

bool player_role_statis_mgr::target_add_gifts(uint32 index)
{
    return index_pos_bit(PRSTE_TARGET_ADD_GIFTS, 0x00000001 << index);
}
#endif

void player_role_statis_mgr::set_invest_join_time(uint32 t)
{
    _prs[PRSTE_INVEST_JOIN_TIME].set_val(t);
}

uint32 player_role_statis_mgr::get_invest_join_time()
{
    return _prs[PRSTE_INVEST_JOIN_TIME].get();
}

bool player_role_statis_mgr::is_join_invest()
{
    return is_set_bit(PRSTE_INVEST_RECORD, 0x00000001);
}

bool player_role_statis_mgr::join_invest()
{
    return index_pos_bit(PRSTE_INVEST_RECORD, 0x00000001);
}

bool player_role_statis_mgr::invest_ret(uint32 index)
{
    if (index < 1) {
        return false;
    }

    return index_pos_bit(PRSTE_INVEST_RECORD, 0x00000001 << index);
}

void player_role_statis_mgr::add_ingot(uint32 ingot)
{
    //_prs[PRSTE_CHARGE_INGOT].add_val(ingot);
}

uint32 player_role_statis_mgr::get_recharge()
{
    //return _prs[PRSTE_CHARGE_INGOT].get();
    return 0;
}

bool player_role_statis_mgr::enter_land_raid()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 17);
}

bool player_role_statis_mgr::get_land_tax_reward()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 18);
}

uint32 player_role_statis_mgr::get_mooncake()
{
    uint32 val = 0;
    _prs[PRSTE_DAY_BYTE_LIMIT].get_val(val);
    return (val & 0x000000FF);
}

bool player_role_statis_mgr::add_mooncake(uint32 num)
{
    if (num > 188) {
        return false;
    }

    uint32 val = 0;
    if (_prs[PRSTE_DAY_BYTE_LIMIT].get_val(val) != 0){
        return false;
    }

    if ((val & 0x000000FF) + num > 188) {
        return false;
    }

    _prs[PRSTE_DAY_BYTE_LIMIT].set_val(val + num);
    return true;
}

uint32 player_role_statis_mgr::get_domain_day_reward()
{
    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return 1;
    }

    return (val & 0x000000FF);
}

bool player_role_statis_mgr::add_domain_day_reward(uint32 num)
{
    if (num > 1) {
        return false;
    }

    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return false;
    }

    if ((val & 0x000000FF) + num > 1) {
        return false;
    }

    _prs[PRSTE_DOMAIN_DAY_REWARD].set_val(val + num);
    return true;
}

uint32 player_role_statis_mgr::get_domain_day_ring()
{
    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return 1;
    }

    return (val & 0x00000F00) >> 8;
}

bool player_role_statis_mgr::add_domain_day_ring(uint32 num)
{
    if (num > 1) {
        return false;
    }

    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return false;
    }

    if (((val & 0x00000F00) >> 8) + num > 1) {
        return false;
    }

    _prs[PRSTE_DOMAIN_DAY_REWARD].set_val(val + (num << 8));
    return true;
}

uint32 player_role_statis_mgr::get_siege_day_ring()
{
    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return 1;
    }

    return (val & 0x0000F000) >> 12;
}

bool player_role_statis_mgr::add_siege_day_ring(uint32 num)
{
    if (num > 1) {
        return false;
    }

    uint32 val = 0;
    if (_prs[PRSTE_DOMAIN_DAY_REWARD].get_val(val) != 0){
        return false;
    }

    if (((val & 0x0000F000) >> 12) + num > 1) {
        return false;
    }

    _prs[PRSTE_DOMAIN_DAY_REWARD].set_val(val + (num << 12));
    return true;
}

//uint32 player_role_statis_mgr::get_domain_worship_num(uint32 id)
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    if (((val & 0x0000F000) >> (12 + id) & 0x00000001) == 0) {
//        return 2;
//    } else if (((val & 0x000F0000) >> (16 + id) & 0x00000001) == 0) {
//        return 1;
//    }
//
//    return 0;
//}

//void player_role_statis_mgr::add_domain_worship_num(uint32 id)
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    if (((val & 0x0000F000) >> (12 + id) & 0x00000001) == 0) {
//        val |= 0x00001000 << id;
//    } else if (((val & 0x000F0000) >> (16 + id) & 0x00000001) == 0) {
//        val |= 0x00010000 << id;
//    }
//
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].set_val(val);
//}
//
//uint32 player_role_statis_mgr::filed_score_exchange()
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    return ((val & 0x00000F00) >> 8);
//}
//
//void player_role_statis_mgr::add_filed_score_exchange()
//{
//    uint32 val = 0;
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].get_val(val);
//    _prs[PRSTE_DAY_HALF_BYTE_LIMIT].set_val(val + 0x00000100);
//}

bool player_role_statis_mgr::add_filed_score(uint32 num)
{
    uint32 val = 0;
    if (_prs[PRSTE_FILED_SCORE].get_val(val) != 0){
        return false;
    }

    val += num;
    val = val <= 1000 ? val : 1000;
    _prs[PRSTE_FILED_SCORE].set_val(val);
    return true;
}

bool player_role_statis_mgr::sub_filed_score(uint32 num)
{
    uint32 val = 0;
    if (_prs[PRSTE_FILED_SCORE].get_val(val) != 0){
        return false;
    }

    if (val < num) {
        val = 0;
    } else {
        val -= num;
    }

    _prs[PRSTE_FILED_SCORE].set_val(val);
    return true;
}

uint32 player_role_statis_mgr::get_filed_score()
{
    uint32 val = 0;
    if (_prs[PRSTE_FILED_SCORE].get_val(val) != 0){
        return 0;
    }

    return val;
}

uint32 player_role_statis_mgr::get_family_skill_attr(uint32 id)
{
    if (id > 8 || id == 0) {
        return 0;
    }

    --id;
    uint32 val = 0;
    _prs[PRSTE_DAY_BYTE_LIMIT].get_val(val);
    if (((val & 0x0000FF00) >> (8 + id) & 0x00000001) == 0) {
        return 0;
    } else if (((val & 0x00FF0000) >> (16 + id) & 0x00000001) == 0) {
        return 1;
    }

    return 2;
}

void player_role_statis_mgr::add_family_skill_attr(uint32 id)
{
    if (id > 8) {
        return ;
    }

    --id;
    uint32 val = 0;
    _prs[PRSTE_DAY_BYTE_LIMIT].get_val(val);
    if (((val & 0x0000FF00) >> (8 + id) & 0x00000001) == 0) {
         val |= (0x00000100 << id);
    } else if (((val & 0x00FF0000) >> (16 + id) & 0x00000001) == 0) {
         val |= (0x00010000 << id);
    }

    _prs[PRSTE_DAY_BYTE_LIMIT].set_val(val);
}

bool player_role_statis_mgr::get_joint_reward()
{
    return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x80000000 >> 19);
}

bool player_role_statis_mgr::get_mate_reward(uint32 index)
{
    return index_pos_bit(PRSTE_MATE_REWARD, 0x00000001 << index);
}

bool player_role_statis_mgr::get_saparatism_award(uint32 id)
{
	//割据9个场景ID对应的领取状态
	if (id == 1)
	{
		return index_pos_bit(PRSTE_DAY_ONCE_LIMIT, 0x00000001 >> 19);
	}
	return true;
}

void player_role_statis_mgr::fill_all(client::role_statis_rsp& rsp)
{
    add_week_online_time(0);
    for (uint32 i = 0; i < _prs.size(); ++i) {
        _prs[i].fill(rsp);
    }
}

void player_role_statis_mgr::fill(client::role_statis_req* req, client::role_statis_rsp& rsp)
{
    add_week_online_time(0);
    for (int32 i = 0; i < req->type_size(); ++i) {
        if ((uint32)req->type(i) >= _prs.size()) {
            return ;
        }

		if (&_prs[req->type(i)] == NULL){
			continue;
		}

        _prs[req->type(i)].fill(rsp);
    }
}

void player_role_statis_mgr::fill_update(client::role_statis_rsp& rsp)
{
    add_week_online_time(0);
    size_t size = _prs.size();
    for (size_t i = 0; i < size; ++i) {
        if (_prs[i].get_update_type() == PRSUTE_NOT_UPDATE) {
            continue;
        }

        _prs[i].fill(rsp);
    }
}

bool player_role_statis_mgr::index_pos_bit(uint32 index, uint32 pos)
{
    if (index >= _prs.size()) {
        return false;
    }

    uint32 val = 0;
    bool   ret = false;
    int success = _prs[index].get_val(val);
    if ((val & pos) == 0 && success == 0) {
        val |= pos;
        ret = (_prs[index].set_val(val) == 0);
    }

    return ret;
}

void player_role_statis_mgr::index_add(uint32 index, uint32 num)
{
    if (index >= _prs.size()) {
        return ;
    }

    _prs[index].add_val(num);
}

bool player_role_statis_mgr::is_set_bit(uint32 index, uint32 pos)
{
    if (index >= _prs.size()) {
        return false;
    }

    uint32 val = 0;
    int success = _prs[index].get_val(val);
    return ((val & pos) != 0 && success == 0);
}

uint32 player_role_statis_mgr::get_mate_scene_enter_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);

    return (val & 0xFF);
}

void player_role_statis_mgr::add_mate_scene_enter_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);
    val += 1;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].set_val(val);
}

uint32 player_role_statis_mgr::get_sect_reward_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);

    return ((val & 0xFF00)>>8);
}

void player_role_statis_mgr::add_sect_reward_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);
    val += (1<<8);
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].set_val(val);
}

uint32 player_role_statis_mgr::get_joint_raid_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);

    return ((val & 0xFF0000)>>16);
}

void player_role_statis_mgr::add_joint_raid_times()
{
    uint32 val = 0;
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].get_val(val);
    val += (1<<16);
    _prs[PRSTE_OTHER_DAY_RAID_TIMES].set_val(val);
}

void player_role_statis_mgr::recover_challenge_times(uint32 n, uint32 timelong)
{
	_prs[PRSTE_ARENA_RECOVER_TIMES].only_set_val(1);
	_prs[PRSTE_ARENA_RECOVER_TIMES].set_last_time(_prs[PRSTE_ARENA_RECOVER_TIMES].get_last_time() + timelong);		// 修正恢复时间
	sub_challenge_times(n);
}

uint32 player_role_statis_mgr::get_last_recover_time()
{
	return _prs[PRSTE_ARENA_RECOVER_TIMES].get_last_time();
}

void player_role_statis_mgr::add_challenge_times()
{
	uint32 val = _prs[PRSTE_ARENA_CHALLENGE_TIMES].get();
	if(val == 0)
	{
		_prs[PRSTE_ARENA_RECOVER_TIMES].set_last_time((uint32)time(NULL));
	}
	_prs[PRSTE_ARENA_CHALLENGE_TIMES].add_val(1);
	set_challenge_cd();
}

void player_role_statis_mgr::sub_challenge_times(uint32 times)
{
    uint32 val = 0;
    _prs[PRSTE_ARENA_CHALLENGE_TIMES].get_val(val);
	if(val > times)
	{
		_prs[PRSTE_ARENA_CHALLENGE_TIMES].only_set_val(val - times);
	}
	else
	{
		_prs[PRSTE_ARENA_CHALLENGE_TIMES].only_set_val(0);
	}
}

uint32 player_role_statis_mgr::get_challenge_times()
{
    uint32 val = 0;
    _prs[PRSTE_ARENA_CHALLENGE_TIMES].get_val(val);
	return val;
}

uint32 player_role_statis_mgr::get_challenge_time()
{
	return _prs[PRSTE_ARENA_CHALLENGE_TIMES].get_last_time();
}

//重置挑战次数
void player_role_statis_mgr::reset_challenge_times()
{
	_prs[PRSTE_ARENA_RESET_CHALLENGE_TIMES].add_val(1);
	_prs[PRSTE_ARENA_CHALLENGE_TIMES].only_set_val(0);
}

uint32 player_role_statis_mgr::get_reset_challenge_times()
{
	uint32 val = 0;
	_prs[PRSTE_ARENA_CHALLENGE_TIMES].get_val(val);
	return val;
}

void player_role_statis_mgr::set_challenge_cd()
{
	_prs[PRSTE_ARENA_CD].set_val(1);
}

// 清除冷却时间
void player_role_statis_mgr::clear_challenge_cd()
{
	_prs[PRSTE_ARENA_CD].set_last_time(0);
}

uint32 player_role_statis_mgr::get_challenge_cd()
{
	return _prs[PRSTE_ARENA_CD].get_last_time();
}

uint32 player_role_statis_mgr::get_levels_reset_times()
{
	uint32 val = 0;
	_prs[PRSTE_LEVELS_RESET_COUNT].get_val(val);
	return val;
}

void player_role_statis_mgr::add_levels_reset_times()
{
	_prs[PRSTE_LEVELS_RESET_COUNT].add_val(1);
}

void player_role_statis_mgr::add_offline_timelong(uint32 t)
{
	_prs[PRSTE_OFFLINE_TIMELONG].add_val(t);
}

uint32 player_role_statis_mgr::get_offline_timelong()
{
	uint32 val = 0;
	_prs[PRSTE_OFFLINE_TIMELONG].get_val(val);
	return val;
}

void player_role_statis_mgr::reset_offline_timelong()
{
	_prs[PRSTE_OFFLINE_TIMELONG].set_val(0);
}

void player_role_statis_mgr::add_total_offline_tl(uint32 t)
{
	_prs[PRSTE_TOTAL_OFFLINE_TL].add_val(t);
}

uint32 player_role_statis_mgr::get_total_offline_tl()
{
	uint32 val = 0;
	_prs[PRSTE_TOTAL_OFFLINE_TL].get_val(val);
	return val;
}

uint32 player_role_statis_mgr::get_cycle_invite_count()
{
	uint32 val = 0;
	_prs[PRSTE_CYCLE_INVITE_COUNT].get_val(val);
	return val;
}

void player_role_statis_mgr::add_cycle_invite_count(uint32 t)
{
	_prs[PRSTE_CYCLE_INVITE_COUNT].add_val(t);
}
