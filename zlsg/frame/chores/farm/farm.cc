#include "farm.h"
#include "random.h"

static farm_level_cfg FARM_LEVEL_CFGS[FARM_MAX_LEVEL] ={
    {4, 20, 160},
    {6, 40, 1728},
    {8, 70, 6048},
    {10, 110, 15040},
    {12, 160, 31104},
    {14, 220, 57120},
    {16, 290, 96448},
    {18, 370, 140184}
};

farm::farm()
{
    head = 0;
    tail = 0;

    for (int i = 0; i < MAX_FARM_RECORD; ++i){
        fr[i].set_time(0);
        fr[i].set_type(client::FOTE_NULL);
    }
}

farm::~farm()
{
    
}

int farm::attach(mysql::tb_role_farm* p)
{    
    if (p == NULL){
        return -1;
    }

    int ret = parse(p);
    if (ret != 0){
        return -2;
    }

    return 0;
}

int farm::parse(mysql::tb_role_farm* data) // 内部接口,已经保证了data不为空
{
    const string& in = data->farm();
    bool ret = fi.ParseFromString(in);
    if (!ret){
        return -1;
    }

    return 0;
}

int farm::save(amtsvr_context* ctx)
{
    mysql::tb_role_farm* trf = new mysql::tb_role_farm();
    trf->set_role_id(fi.role_id());
    serial(trf->mutable_farm());

    CACHE_KEY k(trf->role_id(), 0);
    write(ctx, k, trf);
    writer_type = WOTE_UPDATE;

    return 0;
}

int farm::serial(string* out)  // 内部接口,已经保证了data不为空
{
    fi.SerializeToString(out);
    
    return 0;
}

int farm::create(uint32 role_id, uint32 level, uint32 now)
{
    writer_type = WOTE_INSERT;
    fi.set_role_id(role_id);
    fi.set_level(level);
    fi.set_score(0);
    fi.set_total_score(level == 2 ? 160 : 0);
    fi.set_money_tree_times(now);

    init_land(now);
    
    return 0;
}

int farm::init_land(uint32 now)
{
    fi.clear_fli();

    for (int i = 0; i < client::FPE_MAX_FARM_LAND_NUM; ++i){
        client::farm_land_info* fli = fi.add_fli();
        fli->set_land_id(i);
        fli->set_serial(0);
        land_gift_sow(now, fli);
    }

    return 0;
}

void farm::land_gift_sow(uint32 now, client::farm_land_info* fli)
{
    uint32 serial = 0;
    switch (fli->land_id())
    {
    case 0:
        {
            serial = 9;
        }
        break;
    case 1:
        {
            serial = 13;
        }
        break;
    case 2:
        {
            if (fi.level() == 2){
                serial = 1;
            }
        }
        break;
    case 3:
        {
            if (fi.level() == 2){
                serial = 3;
            }
        }
        break;
    default:
        break;
    }

    if (serial == 0){
        return;
    }

    sow_land_ex(now, fli, serial);
}

client::FARM_OPERATE_E farm::sow_land_ex(uint32 now, client::farm_land_info* fli, uint32 serial)
{
//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(serial);
//     if (tfs == NULL){
//         return client::FOE_NO_SEED;
//     }
// 
//     fli->set_serial(serial);
//     fli->set_sow_time(now);
//     uint32 total_num;
//     if (tfs->max_gain_num() > tfs->min_gain_num()){ // 种植时决定了其产量
//         uint32 diff = tfs->max_gain_num() - tfs->min_gain_num();
//         if (diff <= 3){
//             total_num = tfs->min_gain_num();
//         } else {
//             diff -= 2;
//             total_num = tfs->min_gain_num() + (mtrandom::rand_int32()%(diff)); 
//         }
//     } else {
//         total_num = tfs->max_gain_num();
//     }
//     fli->set_total_fruit_num(total_num);
//     fli->set_steal_fruit_num(0);
//     fli->set_status(client::FCSE_SEED);

    return client::FOE_SUCCESS;
}

int farm::update(uint32 now)
{
//     const config_farm_mgr& cfm = CONFMGR->get_config_farm_mgr();
//     int size = fi.fli_size();
//     for (int i = 0; i < size; ++i){
//         client::farm_land_info* fli = fi.mutable_fli(i);
//         if (fli->serial() == 0 || fli->status() == client::FCSE_MATURE){
//             continue;
//         }
//         land_status(fli, now, cfm.get_farm_set(fli->serial()));
//     }

    return 0;
}

// void farm::land_status(client::farm_land_info* fli, uint32 now, const mysql::tb_farm_set* tfs)
// {
//     if (fli->serial() == 0 || fli->status() == client::FCSE_MATURE){ // 没有种的和已经成熟的不需要更新了
//         return;
//     }
// 
//     if (tfs == NULL){
//         fli->set_serial(0);
//          return;
//     }
// 
//     int interval = now - fli->sow_time();
//     if (interval < 0){
//         interval = 0;
//     }
// 
//     if ((uint32)interval < tfs->growup_times()){ // 判断作物状态
//         fli->set_status(client::FCSE_SEED);
//         fli->set_countdown(tfs->growup_times() - interval);
//     } else if ((uint32)interval < (tfs->growup_times() + tfs->mature_times())){
//         fli->set_status(client::FCSE_GROWUP);
//         fli->set_countdown(tfs->growup_times() + tfs->mature_times() - interval);
//     } else {
//         fli->set_status(client::FCSE_MATURE);
//         fli->set_countdown(0);
//     }
// }

bool farm::can_sow()
{
    uint32 count = 0;
    farm_level_cfg& flc = get_level_cfg();
 
    int size = fi.fli_size();
    for (int i = 0; i < size; ++i){
        if (fi.fli(i).serial() > 0){
            if (++count >= flc.max_lands){
                return false;
            }
        }
    }

    return true;
}

client::FARM_OPERATE_E farm::sow(uint32 now, uint32 land_id, uint32 serial)
{
    client::farm_land_info* fli = get_land_info(land_id); 
    if (fli == NULL || fli->serial() > 0){
        return client::FOE_SOW_NUM_LIMIT;
    }

    if (!can_sow()){
        return client::FOE_SOW_NUM_LIMIT;
    }

    return sow_land_ex(now, fli, serial);
}

client::farm_land_info* farm::get_land_info(uint32 land_id)
{
    if (land_id >= (uint32)fi.fli_size()){
        return NULL;
    }

    return fi.mutable_fli(land_id);
}

client::FARM_OPERATE_E farm::steal(uint32 now, uint32 thief_role_id, uint32 land_id, uint32& goods_id, uint32& num)
{
//     client::farm_land_info* fli = get_land_info(land_id); 
//     if (fli == NULL){
//         return client::FOE_NOT_EXIST_FARM;
//     }
// 
//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(fli->serial());
//     land_status(fli, now, tfs); // 更新一下作物的状态
// 
//     if (tfs == NULL || fli->serial() == 0 || fli->status() != client::FCSE_MATURE){ // 没有成熟不能偷取
//         return client::FOE_NO_MATURE;
//     }
// 
//     int fruit_num = fli->total_fruit_num() - fli->steal_fruit_num();
//     if (fruit_num <= 1 || tfs->max_steal_num() == 0 || tfs->max_steal_num() <= fli->steal_fruit_num()){ // 这块地被偷个数判断
//         return client::FOE_STEAL_NUM_LIMIT;
//     }
// 
//     int steal_role_size = fli->steal_role_id_size();  // 判断这个人是否偷取过这个作物
//     for (int i = 0; i < steal_role_size; ++i){
//         if (fli->steal_role_id(i) == thief_role_id){
//             return client::FOE_STEALED;
//         }
//     }
// 
//     // 到这里应该是可以偷取了
//     fli->set_steal_fruit_num(fli->steal_fruit_num()+1);
//     fli->add_steal_role_id(thief_role_id);
//     goods_id = tfs->gain_goods_id();
//     num = 1;
// 
//     record_steal(now, thief_role_id, fli->serial(), num);

    return client::FOE_SUCCESS;
}

void farm::record_steal(uint32 now, uint32 thief_role_id, uint32 serial, uint32 num)
{
    if (thief_role_id == 0 || serial == 0 || num == 0){
        return;
    }

    just_recored_pos();

    fr[tail].set_time(now);
    fr[tail].set_type(client::FOTE_STEAL);
    fr[tail].set_role_id(thief_role_id);
    fr[tail].set_num(num);
    fr[tail].set_times(now);
    fr[tail].set_serial(serial);

    ++tail;

}

client::FARM_OPERATE_E farm::gain_land(uint32 now, uint32 land_id, client::farm_gain_msg& msg, uint32& add_score)
{
    client::farm_land_info* fli = get_land_info(land_id); 
    if (fli == NULL){
        return client::FOE_NOT_EXIST_FARM;
    }

//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(fli->serial());
//     land_status(fli, now, tfs); // 更新一下作物的状态
// 
//     if (tfs == NULL || fli->serial() == 0 || fli->status() != client::FCSE_MATURE){ // 没有成熟不能偷取
//         return client::FOE_NO_MATURE;
//     }
// 
//     int fruit_num = fli->total_fruit_num() - fli->steal_fruit_num();
//     if (fruit_num <= 0){
//         fruit_num = 0;
//     }
// 
//     add_score += tfs->score();
//     if (fruit_num > 0 && tfs->gain_goods_id() > 0){
//         client::farm_gain_unit* unit =  msg.add_fgus();
//         unit->set_goods_id(tfs->gain_goods_id());
//         unit->set_goods_num((uint32)fruit_num);
//     }
// 
//     fli->set_serial(0);
//     fli->set_sow_time(0);
//     fli->set_status(client::FCSE_NULL);
//     fli->set_countdown(0);
//     fli->set_total_fruit_num(0);
//     fli->set_steal_fruit_num(0);
//     fli->clear_steal_role_id();

    return client::FOE_SUCCESS;
}


void farm::just_recored_pos()
{
    if (tail >= MAX_FARM_RECORD){
        tail = 0;
    } 

    if (head >= tail){
        head = tail + 1;
    }
}

void farm::fill_farm_record_rsp(client::farm_record_rsp& rsp)
{
    if (head < tail){
        for (uint32 i = head; i < tail && i < MAX_FARM_RECORD; ++i){
            rsp.add_frs()->CopyFrom(fr[i]);
        }
    } else {
        for (uint32 i = head; i < MAX_FARM_RECORD; ++i){
            rsp.add_frs()->CopyFrom(fr[i]);
        }

        for (uint32 i = 0; i < tail && i < MAX_FARM_RECORD; ++i){
            rsp.add_frs()->CopyFrom(fr[i]);
        }
    }
}

client::FARM_OPERATE_E farm::improve_land(uint32 now, uint32 land_id, client::FARM_IMPROVE_TYPE_E type)
{
    client::farm_land_info* fli = get_land_info(land_id); 
    if (fli == NULL){
        return client::FOE_NOT_EXIST_FARM;
    }

//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(fli->serial());
//     if (tfs == NULL || fli->serial() == 0){ // 没有成熟不能偷取
//         return client::FOE_NOT_EXIST_FARM;
//     }
// 
//     uint32 seconds = 0;
//     switch (type)
//     {
//     case client::FITE_ONE_HOUR:
//         {
//             seconds = 3600;
//         }
//         break;
//     case client::FITE_TWO_HOUR:
//         {
//             seconds = 7200;
//         }
//         break;
//     case client::FITE_FIVE_HOUR:
//         {
//             seconds = 18000;
//         }
//         break;
//     default:
//         break;
//     }
// 
//     if (seconds == 0){
//         return client::FOE_NOT_EXIST_FARM;
//     }
// 
//     uint32 add_num = 0;
//     if (fli->total_fruit_num() < tfs->max_gain_num()){
//         uint32 tmp_val = mtrandom::rand_int32()%21600;
//         if (seconds >= tmp_val){
//             add_num = 1;
//             fli->set_total_fruit_num(fli->total_fruit_num()+add_num);
//         }
//     }
// 
//     if (fli->sow_time() <= seconds){
//         fli->set_sow_time(0);
//     } else {
//         fli->set_sow_time(fli->sow_time() - seconds);
//     }
// 
//     record_improve(now, get_role_id(), fli->serial(), add_num, seconds);

    return client::FOE_SUCCESS;
}

void farm::record_improve(uint32 now, uint32 role_id, uint32 serial, uint32 add_num, uint32 times)
{
    if (role_id == 0 || serial == 0){
        return;
    }

    just_recored_pos();

    fr[tail].set_time(now);
    fr[tail].set_type(client::FOTE_IMPROVE);
    fr[tail].set_role_id(role_id);
    fr[tail].set_num(add_num);
    fr[tail].set_times(times);
    fr[tail].set_serial(serial);

    ++tail;
}

client::FARM_OPERATE_E farm::gain_money_tree(uint32 now, uint32& money)
{
    if (fi.money_tree_times() + MONEY_TREE_GAIN_INTERVAL > now){
        return client::FOE_MONEY_TIMELIMIT;
    }

    fi.set_money_tree_times(now);
    farm_level_cfg& flc = get_level_cfg();
    money = flc.money;

    return client::FOE_SUCCESS;
}

client::FARM_OPERATE_E farm::gain(client::farm_gain_req& req, uint32 now, client::farm_gain_msg& msg, uint32& add_score)
{
    add_score = 0;
    msg.set_farm_role_id(req.svr_head().role_id());
    int size = req.land_id_size();
    for (int i = 0; i < size; ++i){
        gain_land(now, req.land_id(i), msg, add_score);
    }

    return client::FOE_SUCCESS;
}

void farm::add_farm_score(uint32 add_score)
{
    fi.set_total_score(fi.total_score() + add_score);

    fi.set_score(fi.score() + add_score);
    if (fi.level() >= FARM_MAX_LEVEL){
        return;
    } 

    uint32 score = fi.score();
    while (score > 0) {
        farm_level_cfg& flc = farm::get_level_cfg();
        if (score < flc.upscore || fi.level() >= FARM_MAX_LEVEL || flc.upscore == 0) {
            break;
        }
        
        score -= flc.upscore;
        fi.set_level(fi.level() + 1);
        fi.set_score(score);
    }
}

farm_level_cfg& farm::get_level_cfg()
{
    if (fi.level() == 0){
        fi.set_level(1);
    } else if (fi.level() > FARM_MAX_LEVEL){
        fi.set_level(FARM_MAX_LEVEL);
    }

    return FARM_LEVEL_CFGS[fi.level()-1];
}

