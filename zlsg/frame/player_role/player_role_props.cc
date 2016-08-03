#include <math.h>
#include "random.h"
#include "config_props.h"
#include "player_role_props.h"

player_role_props::player_role_props() 
        : pack(PACK_GRID_BEG)
        , depot(DEPOT_GRID_BEG)
        , equip(EQUIP_GRID_BEG)
        , explore(EXPLORE_GRID_BEG)
        , task(TASK_GRID_BEG)
{
    db_props = NULL;
}

player_role_props::~player_role_props()
{
    if (db_props != NULL) {
        delete db_props;
        db_props = NULL;
    }
}

int player_role_props::init(uint32 pack_size, uint32 depot_size)
{
    pack.resize(pack_size);
    depot.resize(depot_size);
    equip.resize(MAX_EQUIP_SIZE);
    explore.resize(MAX_EXPLORE_SIZE);
    task.resize(MAX_TASK_SIZE);

    //test();
    
    return 0;
}

int player_role_props::attach_role_props(uint32 pack_size, uint32 deopt_size, int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    if (pb_count <= 0) {
        init(pack_size, deopt_size);
        return 0;
    }

    for (int i = 0; i < pb_count; ++i) {
        if (i == 0) {
            init(static_cast<mysql::tb_role_props*>(pb_ctx[i].pb_msg), pack_size, deopt_size);
        } else {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

int player_role_props::init(mysql::tb_role_props* db, uint32 pack_size, uint32 depot_size)
{
    if (db == NULL) {
        return -1;
    }
    
    if (db_props != NULL) {
        delete db_props;
    }
    
    db_props = db;
    int ret = 0;

    pack.resize(pack_size);
    if (pack.parse_props(db_props->pack()) != 0) {
        return -2;
    }

    depot.resize(depot_size);
    if (depot.parse_props(db_props->depot()) != 0) {
        return -2;
    }

    equip.resize(MAX_EQUIP_SIZE);
    ret = equip.parse_props(db_props->equip());
    if (ret != 0) {
        return -2;
    }

    explore.resize(MAX_EXPLORE_SIZE);
    if (explore.parse_props(db_props->explore()) != 0) {
        return -2;
    }

    task.resize(MAX_TASK_SIZE);
    if (task.parse_props(db_props->task()) != 0) {
        return -2;
    }

    return 0;
}

#ifndef __LIB_MINI_SERVER__

void player_role_props::update_role_props(inner::backpackGoods* data)
{
    int32 size = data->goods_size();
    for (int32 i = 0; i < size; ++i) {
        const inner::inner_goods& ig = data->goods(i);
        if (!ig.has_goods_num() || !ig.has_index() || !ig.has_goods_id()) {
            continue;
        }
        
        client::props_info* rg = mutable_props(ig.index());
        if (rg == NULL) {
            continue;
        }
        
        change_flag(ig.index());
        rg->set_props_id(ig.goods_id());
        rg->set_count(ig.goods_num());
    }
}

void player_role_props::fill_role_props(inner::backpackGoods* data) const
{
    pack   .fill_role_props(data);
    depot  .fill_role_props(data);
    equip  .fill_role_props(data);
    explore.fill_role_props(data);
    task   .fill_role_props(data);
}

#endif // __LIB_MINI_SERVER__

void player_role_props::fill_all_props_info(client::role_props_msg& rpm)
{
    pack   .fill_all_props_info(rpm);
    depot  .fill_all_props_info(rpm);
    equip  .fill_all_props_info(rpm);
    explore.fill_all_props_info(rpm);
    task   .fill_all_props_info(rpm);
}

void player_role_props::fill_equip_info(client::role_props_msg& rpm)
{
    equip.fill_all_props_info(rpm);
}

void player_role_props::fill_change_props_info(client::role_props_msg& rpm)
{
    pack.fill_change_props_info(rpm);
    depot.fill_change_props_info(rpm);
    equip.fill_change_props_info(rpm);
    explore.fill_change_props_info(rpm);
    task.fill_change_props_info(rpm);
}

int player_role_props::move_props(uint32 src_grid, uint32 dst_grid, uint32 num)
{
    props_parser* src = get_pos_parser(src_grid);
    props_parser* dst = get_pos_parser(dst_grid);
    if (src == NULL || dst == NULL) {
        return -1;
    }

    if (src == dst) {
        return src->move_props(src_grid, dst_grid, num);
    }

    client::props_info rg;
    if (src->pop_props(src_grid, rg, num) != 0) {
        return -3;
    }

    if (dst->add_props(rg, dst_grid) < 0) {
        return -4;
    } else if (rg.count() > 0) {
        src->add_props(rg);
    }

    return 0;
}

int player_role_props::remove_props(uint32 grid, uint32 num/* = 1*/)
{
    props_parser* src = get_pos_parser(grid);
    if (src == NULL) {
        return -1;
    }

    uint32 props_id = 0;
    if (src->remove_props(grid, num, props_id) != 0){
        return -3;
    }

    return 0;
}

int player_role_props::move_explore_to_bag()
{
    client::props_info rg;
	uint32 cur_num = 0;
    for (int i = EXPLORE_GRID_BEG; i <= GRID_INDEX_EXPLORE + MAX_EXPLORE_SIZE; ++i) {
		//cur_num = explore.get_index_props_num(i);
        if (explore.pop_props(i, rg, cur_num) != 0) {
            continue;
        }
        
        if (rg.count() == 0) {
            continue;
        }

		const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(rg.props_id());
		if (base_info == NULL){
			continue;
		}

        if (base_info->get_item_type() == PT_PET_EQUIP || base_info->get_item_type() == PT_EQUIP){
			if (depot.add_props(rg) == -1) {// 背包又满了~放回去临时背包，然后返回
				explore.add_props(rg);
				return 1;
			}
        }else{
			if (pack.add_props(rg) == -1) {
				explore.add_props(rg);
				return 1;
			}
		}

    }

    return 0;
}

int player_role_props::add_prop_to_explore(vector<client::props_info> vec_prop)
{
	printf("add_prop_to_explore !!!!!!!! =============\n");

	uint32 explore_count = 0;
	vector<client::props_info> vec_explore;
	vec_explore.clear();

	get_explore().add_props(vec_prop,explore_count,vec_explore);

	return 0;
}

uint32 player_role_props::get_prop_id(uint32 grid_index)
{
    client::props_info* rg = get_prop(grid_index);
    if (rg == NULL) {
        return 0;
    }

    return rg->props_id();
}

client::props_info* player_role_props::get_prop(uint32 grid_index)
{
    props_parser* gp = get_pos_parser(grid_index);
    if (gp == NULL) {
        return NULL;
    }
    
    client::props_info* rg = gp->mutable_props(grid_index);
    
    return rg;
}

void player_role_props::resize(uint32 pack_size, uint32 depot_size)
{
    pack.resize(pack_size);
    depot.resize(depot_size);
}

bool player_role_props::check_explore_had_prop()
{
	if (explore.is_has_prop()){
		return true;
	}

	return false;
}

bool player_role_props::is_can_add_prop()
{
	if (depot.is_full() || pack.is_full() || check_explore_had_prop()){
		return false;
	}
	
	return true;
}

bool player_role_props::is_can_melting()
{
	if (pack.is_full()){
		return false;
	}

	return true;
}

uint32 player_role_props::get_equip_id(uint32 pos)
{
    client::props_info* rg = equip.mutable_props(pos);
    if (rg != NULL && rg->count() != 0) {
        return rg->props_id();
    }

    return 0;
}

uint32 player_role_props::get_weapon_id()
{
    return get_equip_id(PST_WEAPON + GRID_INDEX_EQUIP);
}

uint32 player_role_props::get_suit_id()
{
    return get_equip_id(PST_ARMOR + GRID_INDEX_EQUIP);
}

bool player_role_props::sub_active_second(uint32 sec)
{
    bool change = false;
    uint32 max = EQUIP_GRID_BEG + MAX_EQUIP_SIZE;
    for (uint32 i = EQUIP_GRID_BEG; i < max; ++i) {
        client::props_info* rg = equip.mutable_props(i);
        if (rg == NULL || rg->active_time() == 0) {
            continue ;
        }

        if (rg->active_time() <= sec) {
            rg->set_active_time(1);
        } else {
            rg->set_active_time(rg->active_time() - sec);
        }

        change = true;
    }

    return change;
}

bool player_role_props::check_equip_timeout()
{
    uint32 now = (uint32)time(NULL);
    uint32 max = EQUIP_GRID_BEG + MAX_EQUIP_SIZE;
    bool change = false;
    for (uint32 i = EQUIP_GRID_BEG; i < max; ++i) {
        client::props_info* rg = equip.mutable_props(i);
        if (rg == NULL || rg->count() == 0) {
            continue ;
        }

        if ((rg->end_date() != 0 && rg->end_date() <= now) || rg->active_time() == 1) {
            client::props_info pop;
            if (equip.pop_props(i, pop) != 0) {
                continue ;
            }
            
            if (special_treatment(pop)) {
                continue ;
            }
            
            pack.add_props(pop);
            change = true;
        }
    }

    return change;
}

bool player_role_props::special_treatment(client::props_info& rg)
{
    return false;
}

void player_role_props::change_flag(uint32 grid_index)
{
    props_parser* gp = get_pos_parser(grid_index);
    if (gp == NULL) {
        return ;
    }

    gp->grid_change_flag(grid_index);
}

uint32 player_role_props::get_equip_code(uint32 grid_index)
{
    props_parser* gp = get_pos_parser(grid_index);
    if (gp == NULL) {
        return 0;
    }

    return grid_index - gp->get_beg_index() + 1;
}

client::props_info* player_role_props::mutable_props(uint32 grid_index)
{
    props_parser* gp = get_pos_parser(grid_index);
    if (gp == NULL) {
        return NULL;
    }
    
    return gp->mutable_props(grid_index);
}

bool player_role_props::check_equip(client::props_info* src)
{
    if (src == NULL) {
        return false;
    }
    
    const config_props* cg = CONFMGR->get_config_props_mgr().get_config_props(src->props_id());
    return cg->is_equip();
}

bool player_role_props::check_lnlaid_stone(client::props_info* src)
{
    if (src == NULL) {
        return false;
    }
    
    const config_props* cg = CONFMGR->get_config_props_mgr().get_config_props(src->props_id());
    return cg->is_lnlaid_stone();
}

props_parser* player_role_props::get_pos_parser(uint32 grid_index)
{
    if (grid_index > TASK_GRID_BEG + task.get_pack_size()){
        return NULL;
    }

    if (grid_index >= TASK_GRID_BEG && grid_index <= GRID_INDEX_TASK + task.get_pack_size()){
        return &task;
    }

    if (grid_index >= EXPLORE_GRID_BEG && grid_index <= GRID_INDEX_EXPLORE + explore.get_pack_size()){
        return &explore;
    }

    if (grid_index >= DEPOT_GRID_BEG && grid_index <= GRID_INDEX_DEPOT + depot.get_pack_size()){
        return &depot;// 装备背包
    }

	if (grid_index >= EQUIP_GRID_BEG && grid_index <= GRID_INDEX_EQUIP + equip.get_pack_size()){
		return &equip;// 装备（身上）
	}

    if (grid_index >= PACK_GRID_BEG && grid_index <= PACK_GRID_BEG + pack.get_pack_size()){
        return &pack; // 主背包
    }

    return NULL;
}

int player_role_props::save_props(amtsvr_context* ctx, uint32 role_id)
{
    string pack_str;
    int count = pack.serialize_props(pack_str);
    
    string depot_str;
    count += depot.serialize_props(depot_str);

    string equip_str;
    count += equip.serialize_props(equip_str);

    string explore_str;
    count += explore.serialize_props(explore_str);

    string task_str;
    count += task.serialize_props(task_str);

    /*if (count <= 0) {
        if (db_props == NULL) {
            return 0;
        }
        writer_type = WOTE_REMOVE;
        } else {*/
        if (db_props == NULL) {
            writer_type = WOTE_INSERT;
            db_props = new mysql::tb_role_props; 
            db_props->set_role_id(role_id);
            db_props->set_created_date((uint32)time(NULL));
        } else {
            writer_type = WOTE_UPDATE;
        }

        db_props->set_pack(pack_str);
        db_props->set_depot(depot_str);
        db_props->set_equip(equip_str);
        db_props->set_explore(explore_str);
        db_props->set_task(task_str);
        //}

    mysql::tb_role_props* trg = new mysql::tb_role_props(*db_props);
    CACHE_KEY k(trg->role_id(), 0);
    write(ctx, k, trg);

    return 0;
}

props_parser* player_role_props::get_use_src(uint32 grid_index, bool& use_task_prop)
{
    if (grid_index >= TASK_GRID_BEG && grid_index < TASK_GRID_BEG + task.get_pack_size()){
        use_task_prop = true;
        return &task;
    }

    use_task_prop = false;
    if (grid_index >= PACK_GRID_BEG && grid_index < PACK_GRID_BEG + pack.get_pack_size()){
        return &pack;
    }

    return NULL;
}

void player_role_props::test()
{
}
