#include "base_time.h"

#include "config_birth.h"
#include "config_skill.h"
#include "config_effect.h"
#include "config_props.h"
#include "config_basic.h"
#include "config_upgrade.h"
#include "config_mounts.h"
#include "config_potential.h"
#include "config_beauty.h"
#include "config_upequip.h"
#include "config_legion.h"
#include "config_mgr.h"
#ifndef __LIB_MINI_SERVER__
#include "config_vip.h"
#endif
#include "config_magic.h"
#include "config_gem.h"
#include "config_gem_act.h"
#include "config_bows.h"
#include "client.create_role.pb.h"

#include "player_role_status.h"
#include "player_role_skill_mgr.h"
#include "player_role_raid_mgr.h"
#include "player_role_auto_raid.h"
#include "player_role_props.h"
#include "player_role_mail.h"
#include "player_role_task.h"
#include "player_role_label_mgr.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_vip.h"
#endif
#include "player_role_explore.h"
#include "player_role_statis_mgr.h"
#include "player_role_set.h"
#include "player_role_buffer.h"
#include "player_role_order_mgr.h"
#include "operate_log.h"
#include "player_role_ext_attr.h"
#include "player_role_mounts.h"
#include "player_role_potential.h"
#include "player_role_pet.h"
#include "player_role_cd_cooldown.h"
#include "player_role_treasure.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_mission.h"
#include "player_role_lottery.h"
#endif
#include "player_role.h"
#include "player_role_black_shop.h"
#include "player_role_levels.h"
#include "player_role_gem.h"
#include "player_role_bows.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_cycle.h"
#include "player_role_devil.h"
#endif

player_role::player_role(mysql::tb_player_role* r) 
    : _db_player_role(r)
    , _auto_raid(NULL)
{
    writer_type = WOTE_UPDATE;
    _team_id    = 0;

    _status       = new player_role_status      ;
    _skill_mgr    = new player_role_skill_mgr   ;
    _raid_mgr     = new player_role_raid_mgr    ;
    _items        = new player_role_props       ;
    _mails        = new player_role_mail        ;
    _tasks        = new player_role_task        ;
    _label_mgr    = new player_role_label_mgr   ;
#ifndef __LIB_MINI_SERVER__
    _vip          = new player_role_vip         ;
#endif
    _explore      = new player_role_explore     ;
    _set          = new player_role_set         ;
    _buffers      = new player_role_buffer      ;
    _order_mgr    = new player_role_order_mgr   ;
    _ext_attr_mgr = new player_role_ext_attr_mgr;
    _mounts       = new player_role_mounts      ;
    _statis_mgr   = new player_role_statis_mgr  ;
    _logs         = new operate_log             ;
    _potentials   = new player_role_potential   ;
    _pets         = new player_role_pet         ;
    _cds          = new player_role_cd_cooldown ;
    _treasures    = new player_role_treasure    ;
#ifndef __LIB_MINI_SERVER__
	_target		  = new player_role_mission		;
	_lottery	  = new player_role_lottery		;
#endif
	_black_shop   = new player_role_black_shop	;
	_levels		  = new player_role_levels		;
	_gems	      = new player_role_gem			;
	_bows         = new player_role_bows		;
#ifndef __LIB_MINI_SERVER__
	_cycle		  = new player_role_cycle		;
	_devil		  = new player_role_devil		;
#endif

    init_ext(_db_player_role);
}

player_role::~player_role()
{
    delete _status      ;
    delete _skill_mgr   ;
    delete _raid_mgr    ;
    delete _items       ;
    delete _mails       ;
    delete _tasks       ;
    delete _label_mgr   ;
#ifndef __LIB_MINI_SERVER__
    delete _vip         ;
#endif
    delete _explore     ;
    delete _statis_mgr  ;
    delete _set         ;
    delete _buffers     ;
    delete _order_mgr   ;
    delete _ext_attr_mgr;
    delete _mounts      ;
    delete _logs        ;
    delete _potentials  ;
    delete _pets        ;
    delete _cds         ;
    delete _treasures   ;
#ifndef __LIB_MINI_SERVER__
	delete _target		;
	delete _lottery		;
#endif
	delete _black_shop	;
	delete _levels		;
	delete _gems		;
	delete _bows		;
#ifndef __LIB_MINI_SERVER__
	delete _cycle		;
	delete _devil		; 
#endif

    if (_db_player_role != NULL) {
        delete _db_player_role;
        _db_player_role = NULL;
    }

    if (_auto_raid != NULL) {
        delete _auto_raid;
        _auto_raid = NULL;
    }
}

int player_role::attach_role_props(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    if (_db_player_role == NULL) {
        for (int i = 0; i < pb_count; ++i) {
            delete pb_ctx[i].pb_msg;
        }
        return -1;
    }

    return _items->attach_role_props(_db_player_role->backpack_size(), _db_player_role->warehouse_size(), pb_count, pb_ctx);
}

int player_role::attach_role_auto_raid(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (i == 0) {
            _auto_raid = new player_role_auto_raid(get_role_id(), static_cast<mysql::tb_role_auto_raid*>(pb_ctx[i].pb_msg));
        } else {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

player_role_auto_raid* player_role::new_auto_raid()
{
    if (_auto_raid == NULL) {
        _auto_raid = new player_role_auto_raid(get_role_id(), NULL);
    }

    return _auto_raid;
}

int player_role::create_new_player_role(client::create_role_req* req, const config_birth* cb, uint32 role_id, uint32 test_reward)
{
    uint32 now = (uint32)time(NULL);
    int ret = create_db_player_role(req, cb, role_id, now);
    if (ret != 0) {
        return -1;
    }

    ret = create_new_role_item(cb, _db_player_role->backpack_size(), _db_player_role->warehouse_size());
    if (ret != 0) {
        return -2;
    }

    ret = create_new_role_status(cb, role_id, now);
    if (ret != 0) {
        return -3;
    }

    ret = create_new_role_skill(cb, role_id);
    if (ret != 0) {
        return -4;
    }

    init_ext(_db_player_role);
    init_once(_db_player_role);

    return 0;
}

int player_role::create_new_role_skill(const config_birth* cb, uint32 role_id)
{
    if (cb == NULL) {
        return -1;
    }
    
    _skill_mgr->add_skill(role_id, cb->get_default_skill_id());
    const std::vector<int>& skill_list = cb->get_skill_list();
    for (size_t i = 0; i < skill_list.size(); ++i) {
        _skill_mgr->add_skill(role_id, skill_list[i]);
    }
    
    return _skill_mgr->init_new_role_skill(role_id, cb->get_job());
}

int player_role::create_new_role_status(const config_birth* cb, uint32 role_id, uint32 now)
{
    if (cb == NULL) {
        return -1;
    }
    
    return _status->create(role_id, cb, now);
}

int player_role::create_new_role_item(const config_birth* cb, uint32 backp_size, uint32 depot_size)
{
    if (cb == NULL) {
        return -1;
    }
    
    _items->init(backp_size, depot_size);
    const std::map<int, int>& equip     = cb->get_equip();
    const std::map<int, int>& item_list = cb->get_item_list();
    for (std::map<int, int>::const_iterator i = equip.begin(); i != equip.end(); ++i) {
        const config_props* cp = CONFMGR->get_config_props(i->second);
        if (cp == NULL || cp->get_item_type() != PT_EQUIP) {
            continue;
        }

        client::props_info pi;
        _items->get_equip().makeup_props(cp->get_item_id(), 1, cp->get_name_color_index(), cp->get_bind_type(), *cp, pi);
        _items->get_equip().add_props(pi, cp->get_item_sub_type() + GRID_INDEX_EQUIP);
    }
    
    for (std::map<int, int>::const_iterator i = item_list.begin(); i != item_list.end(); ++i) {
		vector<client::props_info> vec_explore;
		vec_explore.clear();
        _items->get_pack().add_props(i->first, i->second,vec_explore);
		if (vec_explore.size() != 0){
			_items->add_prop_to_explore(vec_explore);
		}
    }

    return 0;
}

int player_role::create_db_player_role(client::create_role_req* req, const config_birth* cb, uint32 role_id, uint32 now)
{
    if (_db_player_role != NULL || req == NULL || req->svr_head().user_id() == 0) {
        return -1;
    }

	uint32 pack_size = 0;
	uint32 depot_size = 0;
#ifndef __LIB_MINI_SERVER__
	const config_vip *vip = CONFMGR->get_config_vip(0);
	if (vip == NULL){
		pack_size = MAX_PACK_SIZE;
		depot_size = MAX_DEPOT_SIZE;
	}else {
		pack_size = vip->_grid;
		depot_size = vip->_grid;
	}
#endif

	uint32 skill_point_base = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10086));
    writer_type = WOTE_INSERT;
    _db_player_role = new mysql::tb_player_role();
    _db_player_role->set_role_id                 (role_id                   );
    _db_player_role->set_system_id               (req->svr_head().svr_id()  );
    _db_player_role->set_player_id               (req->svr_head().user_id() );
    _db_player_role->set_role_name               (req->name()               );
    _db_player_role->set_status                  (player_role::ROLE_BAN_NULL);
    _db_player_role->set_sex                     (req->sex()                );
    _db_player_role->set_profession              (req->prof()               );
    _db_player_role->set_experience              (0                         );
    _db_player_role->set_level                   (1                         );
    _db_player_role->set_tael                    (0                         );
    _db_player_role->set_ingot                   (0                         );
    _db_player_role->set_gift_money              (0                         );
    _db_player_role->set_is_suspended_account    (0                         );
    _db_player_role->set_partner_id              (0                         );
    _db_player_role->set_family_id               (0                         );
    _db_player_role->set_family_position         (0                         );
    _db_player_role->set_can_be_raised           (0                         );
    _db_player_role->set_family_level            (0                         ); // 军团等级
    _db_player_role->set_can_join_family         (0                         ); // 改用为：个人的帮派贡献
    _db_player_role->set_can_be_transmit         (0                         );
    _db_player_role->set_pk_model                (0                         );
    _db_player_role->set_pk_value                (0                         );
    _db_player_role->set_military_strategy       (0                         );
    _db_player_role->set_map_id                  (1                         );
    _db_player_role->set_x_pos                   (0                         );
    _db_player_role->set_y_pos                   (0                         );
    _db_player_role->set_siege_reputation        (0                         );
    _db_player_role->set_cross_svr_war_reputation(0                         );
    _db_player_role->set_armor_id                (0                         );
    _db_player_role->set_weapon_id               (0                         );
    _db_player_role->set_backpack_size           (pack_size                 );
    _db_player_role->set_warehouse_size          (depot_size                );
    _db_player_role->set_physical_power          (0                         );
    _db_player_role->set_fighting_power          (0                         );
    _db_player_role->set_anger                   (0                         );
    _db_player_role->set_create_time             (now                       );
    _db_player_role->set_online_time             (0                         );
    _db_player_role->set_offline_time            (0                         );
    _db_player_role->set_update_time             (now                       );
    _db_player_role->set_history_online          (0                         );
	_db_player_role->set_melting                 (0                         );
	_db_player_role->set_potential_share_max     (0                         );
	_db_player_role->set_potential_accumulation  (0                         );
    _db_player_role->set_exp_coefficient         (10000                     );
    _db_player_role->set_skill_point             (skill_point_base          );
    _db_player_role->set_popularity              (0                         );
    _db_player_role->set_skill_point_time        (now                       );
    _db_player_role->set_vip_level               (0                         );
    _db_player_role->set_team_id                 (0                         );
    _db_player_role->set_camp_id                 (0                         );
    _db_player_role->set_strategy                (0                         );
    _db_player_role->set_store_weapon            (0                         );
	_db_player_role->set_bow_id					 (0                         );
	_db_player_role->set_bow_index			     (0                         );
	_db_player_role->set_cavalry_index			 (0							);
	_db_player_role->set_bow_speed			     (0							);
	_db_player_role->set_equip_power			 (0							);
    _query_attr.set_level(_db_player_role->level());
    _query_attr.set_experience(_db_player_role->experience());
    init_original_attr(_query_attr);

    _db_player_role->set_cur_hp(_db_player_role->max_hp());
    _db_player_role->set_cur_mp(_db_player_role->max_mp());
    
    return 0;
}

void player_role::set_online_time(uint32 t)
{
    _db_player_role->set_online_time(t);
}

void player_role::set_offline_time(uint32 t)
{
    _db_player_role->set_offline_time(t);
}

void player_role::set_skill_point_time(uint32 t)
{
	_db_player_role->set_skill_point_time(t);
}

void player_role::add_history_online(uint32 t)
{
    _db_player_role->set_history_online(_db_player_role->history_online() + t);
}

void player_role::write(amtsvr_context* ctx)
{
    if (_db_player_role == NULL) {
        return ;
    }

    _db_player_role->set_update_time((uint32)time(NULL));
    CACHE_KEY k(_db_player_role->role_id(), 0);
    mysql::tb_player_role* tmp_db_player_role = new mysql::tb_player_role(*_db_player_role);
    pb_writer::write(ctx, k, tmp_db_player_role);
    writer_type = WOTE_UPDATE;
}

void player_role::write_all(amtsvr_context* ctx)
{
#ifdef __LIB_MINI_SERVER__
    return ; // miniserver 不保存
#else // __LIB_MINI_SERVER__

    if (_db_player_role != NULL) {
        _db_player_role->set_update_time((uint32)time(NULL));
        CACHE_KEY k(_db_player_role->role_id(), 0);
        mysql::tb_player_role* tmp_db_player_role = new mysql::tb_player_role(*_db_player_role);
        pb_writer::write(ctx, k, tmp_db_player_role);
        writer_type = WOTE_UPDATE;
    }

    _status->write_status(ctx);
    _skill_mgr->write(ctx);
    _raid_mgr->save(ctx);
    _mails->save_mail(ctx);
    _items->save_props(ctx, get_role_id());
    _tasks->save(ctx, get_role_id());
    //pulse_mgr.save(ctx);
    //label_mgr.save(ctx);
    //explore.save(ctx);
    _statis_mgr->write(ctx);
    //set.save(ctx);
    _buffers->save(get_role_id(), ctx);
    //order_mgr.save(ctx);
    //logs.save(ctx);
    _ext_attr_mgr->save(ctx);
    _mounts->save(ctx);
	_potentials->save_potential(get_role_id(),ctx);
	_pets->save_pet(get_role_id(),ctx);
	_cds->save_cd_cooldown(get_role_id(),ctx);
	_treasures->save_treasure(get_role_id(),ctx);
	_target->save(ctx, get_role_id());
	_vip->save(ctx, get_role_id());
	_lottery->save(ctx, get_role_id());
	_black_shop->save_black_shop(get_role_id(),ctx);
	_levels->save(ctx, get_role_id());
	_gems->save_gem(get_role_id(),ctx);
	_bows->save(ctx);
	_cycle->save(get_role_id(), ctx);
	_devil->save(get_role_id(), ctx);
#endif // __LIB_MINI_SERVER__
}

void player_role::init_role_attr(client::role_attr& attr)
{
    if (get_db_player_role() == NULL) {
        return ;
    }

    const google::protobuf::Reflection* ref_attr = attr.GetReflection();
    const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
    const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
    const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();
    if (ref_attr == NULL || des_attr == NULL || ref == NULL || des == NULL) {
        return ;
    }
    
    for (uint32 index = SEX; index < ROLE_ATTRIBUTE_MAX; ++index) {
        if ((uint32)des->field_count() <= index + ATTR_CODE_TO_DB) {
            break;
        }

        switch (des->field(index + ATTR_CODE_TO_DB)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(index + ATTR_CODE_TO_DB));
            ref_attr->SetUInt32(&attr, des_attr->field(index), val);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(index + ATTR_CODE_TO_DB));
            ref_attr->SetUInt64(&attr, des_attr->field(index), val);
            break;
        }
        default: break;
        }
    }
}

#ifndef __LIB_MINI_SERVER__
void player_role::fill_role_goods(inner::backpackGoods* data) const
{
    if (_db_player_role == NULL){
        return ;
    }

    data->mutable_user()->set_id(_db_player_role->role_id());
    data->mutable_user()->set_nickname(_db_player_role->role_name());

    _items->fill_role_props(data);
}

void player_role::update_role_goods(inner::backpackGoods* update)
{
    if (_db_player_role == NULL) {
        return ;
    }

    update->mutable_user()->set_id(_db_player_role->role_id());
    update->mutable_user()->set_nickname(_db_player_role->role_name());

    _items->update_role_props(update);
}
#endif // __LIB_MINI_SERVER__

int player_role::add_ingot(uint32 ingot)
{
    if (_db_player_role == NULL){
        return -1;
    }

    _db_player_role->set_ingot(_db_player_role->ingot() + ingot);
    get_role_statis_mgr().add_ingot(ingot);

    return 0;
}

void player_role::init_original_attr(client::role_attr& attr)
{
    if (_db_player_role == NULL) {
        return ;
    }

#ifdef __INNER_DEBUG__PACKAGE_
    uint32 old_level = _db_player_role->level();
#endif // __INNER_DEBUG__PACKAGE_

    const google::protobuf::Reflection* ref_attr = attr.GetReflection();
    const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
    const google::protobuf::Reflection* ref = _db_player_role->GetReflection();
    const google::protobuf::Descriptor* des = _db_player_role->GetDescriptor();
    if (ref_attr == NULL || des_attr == NULL || ref == NULL || des == NULL) {
        return ;
    }

    uint32 fix_index = 0;
    uint32 reset     = 0;
    for (uint32 index = MAX_HP; index <= MOVE_SPEED; ++index) {
        fix_index = index + ATTR_CODE_TO_DB;
        if ((uint32)des->field_count() <= fix_index) {
            break;
        }

        if (index == CUR_HP || index == CUR_MP) {
            continue;
        }

        if (index == EXP_COEFFICIENT) {
            reset = 10000;
        } else {
            reset = 0;
        }

        switch (des->field(fix_index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            ref->SetUInt32(_db_player_role, des->field(fix_index), reset);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            ref->SetUInt64(_db_player_role, des->field(fix_index), reset);
            break;
        }
        default: break;
        }
    }

    upgrade_attr(attr.level(), attr.experience());

    add_attr();

    add_attr_family(attr.family_id(), attr.famiy_level());

    for (uint32 index = MAX_HP; index <= MOVE_SPEED; ++index) {
        if ((uint32)des->field_count() <= index + ATTR_CODE_TO_DB) {
            continue;
        }

        if (index == CUR_HP || index == CUR_MP) {
            continue;
        }

        switch (des->field(index + ATTR_CODE_TO_DB)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref->GetUInt32(*_db_player_role, des->field(index + ATTR_CODE_TO_DB));
            ref_attr->SetUInt32(&attr, des_attr->field(index), val);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref->GetUInt64(*_db_player_role, des->field(index + ATTR_CODE_TO_DB));
            ref_attr->SetUInt64(&attr, des_attr->field(index), val);
            break;
        }
        default: break;
        }
    }

#ifdef __INNER_DEBUG__PACKAGE_
    if (old_level > _db_player_role->level()) {
        amtsvr_log("level error \n");
        //assert(false);
    }
#endif // __INNER_DEBUG__PACKAGE_
}

void player_role::calc_role_attr(client::role_attr& cal_attr)
{
    cal_attr.CopyFrom(_query_attr);
}

void player_role::get_query_role_info(client::role_query_info& query_info)
{
	//query_info.set_fight_power(_query_attr.fighting_power());
	//query_info.set_profession(_query_attr.profession());
	//query_info.set_level(_query_attr.level());
	//query_info.set_weapon_id(_query_attr.weapon_id());
	//query_info.set_armor_id(_query_attr.armor_id());
	//query_info.set_vip_level(_query_attr.vip_level());
	
	for (uint32 i = EQUIP_GRID_BEG; i <= EQUIP_SHOE; ++i){
		get_role_props().get_equip().fill_interval_props_info(query_info,i);
	}
	
}

void player_role::get_query_pet_info(client::pet_query_info& query_info)
{
	client::pet_db_info pet_info;
	// 挑选当前出战的
	if (!get_pets().get_query_fighting_pet(pet_info)){
		return;
	}

	query_info.set_pet_id(pet_info.pet_id());
	query_info.set_pet_level(pet_info.pet_level());
	query_info.set_magic_star(pet_info.magic_star());
	query_info.set_move_speed(_query_attr.move_speed());
	// 填充装备数据
	uint32 base_index = pet_info.pet_id() * 10;
	uint32 begin_index = base_index + 1;
	uint32 last_index = base_index + 6;
	for (uint32 i = begin_index; i <= last_index; ++i){
		get_role_props().get_equip().fill_pet_props_info(query_info,i);
	}
	// 填充技能数据
	const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_info.pet_id(), pet_info.pet_level());
	if (base_info == NULL){
		return;
	}

	uint32 pet_skill_type = base_info->get_skill_belong();
	
	
	std::vector<player_role_skill>& role_skill_vec = get_role_skill_mgr().get_skills();
	for (uint32 j = 0; j < role_skill_vec.size(); ++j){
		const config_skill* skill_info = CONFMGR->get_config_skill_mgr().get_config_skill(role_skill_vec[j].get_skill_id());
		if (skill_info == NULL){
			return;
		}


		if (skill_info->get_skills_belonging() != pet_skill_type) {
			continue;
		}

		client::skill_data* info = query_info.add_skill_info();
		role_skill_vec[j].fill_role_skill_data(*info);
	}
}

void player_role::get_query_bow_info(client::bows_query_info& query_info)
{
	const mysql::tb_role_bows& player_bow = get_player_role_bows().get_db();

	query_info.set_bow_id(player_bow.bow_id());
	query_info.set_bow_level(player_bow.bow_level());
	query_info.set_bow_power(player_bow.bow_power());
}

void player_role::get_query_gem_info(client::gem_query_info& query_info)
{
	uint32 total_level = 0;
	uint32 power = 0;

	const std::map<uint32, client::gem_info>& gem_map_ = get_player_role_gems().get_all_gem_info();

	std::map<uint32, client::gem_info>::const_iterator it = gem_map_.begin();
	for (;it != gem_map_.end(); ++it){

	}

	query_info.set_total_level(total_level);
	query_info.set_power(power);
}

void player_role::upgrade_attr(uint32 level, uint64 experience)
{
    const config_upgrade* cu = CONFMGR->get_config_upgrade(get_db_player_role()->profession(), level);
    if (cu == NULL) {
        return ;
    }

    get_db_player_role()->set_level(level);
    get_db_player_role()->set_experience(experience);

    set_attr(cu->get_game_attribute());
}

int player_role::init_ext(mysql::tb_player_role* tpr)
{
    if (tpr == NULL) {
        //amtsvr_log("error init\n");
        return -1;
    }

    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "%u", get_db_player_role()->system_id());
    _sid = buf;

#ifndef __LIB_MINI_SERVER__
    _statis_mgr->init(tpr->role_id());
#endif

    init_role_attr(_query_attr);

    return 0;
}

int player_role::init_once(mysql::tb_player_role* tpr)
{
    if (tpr == NULL) {
        return -1;
    }

    return 0;
}

void player_role::set_attr(const std::map<int, int>& a)
{
    const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
    const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();
    if (ref == NULL || des == NULL) {
        return ;
    }

    for (std::map<int, int>::const_iterator i = a.begin(); i != a.end(); ++i) {
        if (i->first >= GAME_ATTRIBUTE_MAX ) {
            amtsvr_log("attr type error %d\n", i->first);
            continue;
        }

        int32 index = ATTR_CFG_TO_CODE[i->first];
        if (des->field_count() <= index || index < MAX_HP || index > MOVE_SPEED) {
            continue;
        }

        index += ATTR_CODE_TO_DB;
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            ref->SetUInt32(get_db_player_role(), des->field(index), i->second);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            ref->SetUInt64(get_db_player_role(), des->field(index), i->second);
            break;
        }
        default: break;
        }
    }
}

void player_role::add_attr(const std::map<int, int>& a)
{
    const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
    const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();
    if (ref == NULL || des == NULL) {
        return ;
    }

    for (std::map<int, int>::const_iterator i = a.begin(); i != a.end(); ++i) {
        if (i->first >= GAME_ATTRIBUTE_MAX ) {
            amtsvr_log("attr type error %d\n", i->first);
            continue;
        }

        int32 index = ATTR_CFG_TO_CODE[i->first];
        if (des->field_count() <= index || index < MAX_HP || index > MOVE_SPEED) {
            continue;
        }

        index += ATTR_CODE_TO_DB;
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(index));
            ref->SetUInt32(get_db_player_role(), des->field(index), i->second + val);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(index));
            ref->SetUInt64(get_db_player_role(), des->field(index), i->second + val);
            break;
        }
        default: break;
        }
    }
}

void player_role::add_mulimap_attr(const std::multimap<int, int>& a)
{
	const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
	const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();
	if (ref == NULL || des == NULL) {
		return ;
	}

	for (std::multimap<int, int>::const_iterator i = a.begin(); i != a.end(); ++i) {
		int32 index = ATTR_CFG_TO_CODE[i->first];
		if (des->field_count() <= index || index < MAX_HP || index > MOVE_SPEED) {
			continue;
		}

		index += ATTR_CODE_TO_DB;
		switch (des->field(index)->type()) {
		case google::protobuf::FieldDescriptor::TYPE_UINT32: {
			uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(index));
			ref->SetUInt32(get_db_player_role(), des->field(index), i->second + val);
			break;
															 }
		case google::protobuf::FieldDescriptor::TYPE_UINT64: {
			uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(index));
			ref->SetUInt64(get_db_player_role(), des->field(index), i->second + val);
			break;
															 }
		default: break;
		}
	}
}

void player_role::add_attr(const client::role_attr& attr)
{
    const google::protobuf::Reflection* ref      = get_db_player_role()->GetReflection();
    const google::protobuf::Descriptor* des      = get_db_player_role()->GetDescriptor();
    const google::protobuf::Reflection* ref_attr = attr.GetReflection();
    const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
    if (ref == NULL || des == NULL || ref_attr == NULL || des_attr == NULL) {
        return ;
    }

    uint32 fix_index = 0;
    for (int32 index = 0; index <= des_attr->field_count(); ++index) {
        fix_index = index + ATTR_CODE_TO_DB;
        if ((uint32)des->field_count() <= fix_index) {
            break;
        }

        switch (des->field(fix_index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(fix_index));
            uint32 add = ref_attr->GetUInt32(attr, des_attr->field(index));
            ref->SetUInt32(get_db_player_role(), des->field(fix_index), val + add);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(fix_index));
            uint64 add = ref_attr->GetUInt64(attr, des_attr->field(index));
            ref->SetUInt64(get_db_player_role(), des->field(fix_index), val + add);
            break;
                                                             }
        default: break;
        }
    }
}

void player_role::set_attr(const client::role_attr& attr)
{
    const google::protobuf::Reflection* ref      = get_db_player_role()->GetReflection();
    const google::protobuf::Descriptor* des      = get_db_player_role()->GetDescriptor();
    const google::protobuf::Reflection* ref_attr = attr.GetReflection();
    const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
    if (ref == NULL || des == NULL || ref_attr == NULL || des_attr == NULL) {
        return ;
    }

    uint32 fix_index = 0;
    for (int32 index = 0; index <= des_attr->field_count(); ++index) {
        fix_index = index + ATTR_CODE_TO_DB;
        if ((uint32)des->field_count() <= fix_index) {
            break;
        }

        if (index == CUR_HP || index == CUR_MP || index == LEVEL || index == EXPERIENCE) {
            continue;
        }

        switch (des->field(fix_index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            //uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(fix_index));
            uint32 add = ref_attr->GetUInt32(attr, des_attr->field(index));
            ref->SetUInt32(get_db_player_role(), des->field(fix_index), add);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            //uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(fix_index));
            uint64 add = ref_attr->GetUInt64(attr, des_attr->field(index));
            ref->SetUInt64(get_db_player_role(), des->field(fix_index), add);
            break;
                                                             }
        default: break;
        }
    }
}
void player_role::add_attr()
{
    add_attr_props();
    add_attr_mounts();
	add_attr_potential();
	add_attr_pet();
	add_attr_skill();
    add_attr_ext();
	add_attr_gem();
	add_attr_bows();
}

void player_role::add_attr_props()
{
	// 获取玩家身上的穿的装备(只算玩家身上的)
	props_parser& equip_pack = _items->get_equip();
	const grid_role_props* equip_info = NULL;
	const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
	const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();
	if (ref == NULL || des == NULL) {
		return ;
	}

	for (size_t i = EQUIP_GRID_BEG; i <= EQUIP_SHOE; ++i) {
		equip_info = equip_pack.get_props(i);
		if (equip_info == NULL || !equip_info->has_props()) {
			continue;
		}

		// 随机属性加成
		for (int j = 0; j < equip_info->attr_size(); ++j) {
			const client::attribute& attr = equip_info->attr(j);
            if (attr.type() >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", attr.type());
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[attr.type()] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(index));
				ref->SetUInt32(get_db_player_role(), des->field(index), val + attr.value());
			}
			break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(index));
				ref->SetUInt64(get_db_player_role(), des->field(index), val + attr.value());
			}
			break;
			default: break;
			}
		}

		const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(equip_info->props_id());
		if (base_info == NULL){
			continue;
		}

		// 主属性加成
		const std::map<int, int>& main_attr_map_ = base_info->get_game_attribute();
		std::map<int, int>::const_iterator itr = main_attr_map_.begin();
		for (;itr != main_attr_map_.end(); ++itr){
            if (itr->first >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", itr->first);
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[itr->first] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				ref->SetUInt32(_db_player_role, des->field(index), val + itr->second);
			}
			break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				ref->SetUInt64(_db_player_role, des->field(index), val + itr->second);
			}
			break;														
			default: break;
			}
		}

		//强化的属性加成
		for (int a = 0; a < equip_info->upgrade_attr_size(); ++a) {
			const client::attribute& attr = equip_info->upgrade_attr(a);
            if (attr.type() >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", attr.type());
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[attr.type()] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				ref->SetUInt32(_db_player_role, des->field(index), val + attr.value());
			}
			break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				ref->SetUInt64(_db_player_role, des->field(index), val + attr.value());
			}
			break;
			default: break;
			}
		}
	}
}

void player_role::add_attr_mounts()
{
    const config_mounts* p = CONFMGR->get_config_mounts(_query_attr.mounts_id());
    if (p == NULL) {
        return ;
    }

    const std::map<int, std::map<int, config_mounts_attr*> >& m = p->get_attr();
    const std::map<int, std::map<int, config_mounts_attr*> >::const_iterator m_job = m.find(0);
    if (m_job == m.end()) {
        return ;
    }

    const std::map<int, config_mounts_attr*>::const_iterator m_job_level = m_job->second.find(_mounts->get_db().mount_level());
    if (m_job_level == m_job->second.end() || m_job_level->second == NULL) {
        return ;
    }
    
    add_attr(m_job_level->second->get_game_attribute());
}

void player_role::add_attr_gem()
{
	player_role_gem& player_gem = get_player_role_gems();
	props_parser& equip_pack = _items->get_equip();
	const grid_role_props* equip_info = NULL;
	const std::map<uint32, client::gem_info>& gem_map_ = player_gem.get_all_gem_info();
	if (gem_map_.size() == 0){
		return;
	}

	uint32 total_level = player_gem.get_gem_total_level();
	std::map<uint32, client::gem_info>::const_iterator it = gem_map_.begin();
	// 宝石本身的属性
	for (; it != gem_map_.end(); ++it){
		// 当前部位没装备不加成属性
		equip_info = equip_pack.get_props(it->first / 10);
		if (equip_info == NULL || !equip_info->has_props()){
			continue;
		}

		const config_gem* gem_base = CONFMGR->get_config_gem(it->second.gem_type(),it->second.gem_level());
		if (gem_base == NULL){
			continue;
		}

		add_attr(gem_base->get_game_attr());
		
	}

	// 宝石等级激活的属性（穿脱装备不影响）
	std::multimap<int,int> gem_act_map_;
	gem_act_map_.clear();
	CONFMGR->get_config_gem_act_mgr().get_config_gem_act_attr(1,total_level,gem_act_map_);

	add_mulimap_attr(gem_act_map_);
}

void player_role::add_attr_bows()
{
	player_role_bows& player_bows = get_player_role_bows();

	const mysql::tb_role_bows& bow_db = player_bows.get_db();
	if (bow_db.bow_id() == 0){
		return;
	}

	const config_bows_attr* bow_attr =  CONFMGR->get_config_bows_mgr().get_config_bows_attr(bow_db.bow_id(),bow_db.bow_level());
	if (bow_attr == NULL){
		return;
	}

	add_attr(bow_attr->get_game_attribute());
}

void player_role::add_attr_potential()
{
	const google::protobuf::Reflection* ref = _db_player_role->GetReflection();
	const google::protobuf::Descriptor* des = _db_player_role->GetDescriptor();

	const std::map<uint32, client::acupoint_info>& potential_map_ = get_potentials().get_all_potential_info();
	std::map<uint32,client::acupoint_info>::const_iterator itr = potential_map_.begin();
	for (; itr != potential_map_.end(); ++itr){
		if (itr->second.status() == 0){
			continue;
		}
		const config_potential* potential_info = CONFMGR->get_config_potential_mgr().get_config_potential(itr->first);
		if (potential_info == NULL){
			continue;
		}
		const std::map<int,int>&  game_attr_map_ = potential_info->get_game_attribute();
		std::map<int,int>::const_iterator it = game_attr_map_.begin();
		for (; it != game_attr_map_.end(); ++it){
            if (it->first >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", it->first);
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[it->first] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				ref->SetUInt32(_db_player_role, des->field(index), val + it->second);
			}
				break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				ref->SetUInt64(_db_player_role, des->field(index), val + it->second);
			}
				break;
			default: 
				break;
			}
		}
	}
}

void player_role::add_attr_pet()
{
    const google::protobuf::Reflection* ref = _db_player_role->GetReflection();
    const google::protobuf::Descriptor* des = _db_player_role->GetDescriptor();

    const std::map<uint32, client::pet_db_info>& pets_map_ = get_pets().get_all_pet_db_info();
    std::map<uint32,client::pet_db_info>::const_iterator itr = pets_map_.begin();
//     uint32 last_level = 0;
//     const config_beauty* base_info = NULL;
    for (; itr != pets_map_.end(); ++itr){
		if (itr->second.lingering_count() == 0){
			continue;
		}

        const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty_by_lingering_count(itr->second.pet_id(),itr->second.lingering_count()-1);
        if (base_info == NULL){
            continue;
        }
        const std::map<int32,int32>& fi_add_property_map_ = base_info->get_fit_add_property();
        std::map<int32,int32>::const_iterator itr_1 = fi_add_property_map_.begin();
		for (; itr_1 != fi_add_property_map_.end(); ++itr_1){
            if (itr_1->first >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", itr_1->first);
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[itr_1->first] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				ref->SetUInt32(_db_player_role, des->field(index), val + itr_1->second);
			}
		    break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				ref->SetUInt64(_db_player_role, des->field(index), val + itr_1->second);
			}
			break;
			default: 
				break;
			}
		}

		const config_magic* magic_info = CONFMGR->get_config_magic_mgr().get_config_magic(base_info->get_magic_id(), itr->second.magic_star());
		if (magic_info == NULL){
			continue;
		}

		const std::map<int,int>& up_charater_map_ = magic_info->get_up_charater();
		std::map<int,int>::const_iterator it_charater = up_charater_map_.begin();
		for (;it_charater != up_charater_map_.end(); ++it_charater){
            if (it_charater->first >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", it_charater->first);
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[it_charater->first] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				ref->SetUInt32(_db_player_role, des->field(index), val + it_charater->second);
																 }
																 break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				ref->SetUInt64(_db_player_role, des->field(index), val + it_charater->second);
																 }
																 break;
			default: 
				break;
			}
		}
    }
}

void player_role::add_attr_skill()
{
	const google::protobuf::Reflection* ref = _db_player_role->GetReflection();
	const google::protobuf::Descriptor* des = _db_player_role->GetDescriptor();

	std::vector<player_role_skill> vs;
    std::vector<player_role_skill>& role_skill_vec = get_role_skill_mgr().get_skills();
    for (size_t i = 0; i < role_skill_vec.size(); ++i) {
        const config_skill* cs = CONFMGR->get_config_skill(role_skill_vec[i].get_skill_id());
        if (cs == NULL) {
            continue;
        }

        if (cs->get_skill_type() == ST_ATTRIBUTE) {
            vs.push_back(role_skill_vec[i]);
        }
    }

	for (size_t i = 0; i < vs.size(); i++) {
		const config_effect* effect_info = CONFMGR->get_config_effect(vs[i].get_effect_id());
		if (effect_info == NULL){
			continue;
		}

		const std::map<int, cpf_val_2>& effect_map_ = effect_info->get_affect_list();
		std::map<int, cpf_val_2>::const_iterator itr = effect_map_.begin();
		while (itr != effect_map_.end()) {
            if (itr->second.type >= GAME_ATTRIBUTE_MAX ) {
                amtsvr_log("attr type error %d\n", itr->second.type);
                continue;
            }

			int32 index = ATTR_CFG_TO_CODE[itr->second.type] + ATTR_CODE_TO_DB;
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*_db_player_role, des->field(index));
				if (itr->second.denominator != 0) {
                    int32 add = (int32)(val * ((float)itr->second.molecular / itr->second.denominator));
                    add = min(add, itr->second.threshold);
                    val += add;
					ref->SetUInt32(_db_player_role, des->field(index), val);
				}
				else{
					ref->SetUInt32(_db_player_role, des->field(index), val + itr->second.molecular);
				}
			}
				break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*_db_player_role, des->field(index));
				if (itr->second.denominator != 0) {
					ref->SetUInt64(_db_player_role, des->field(index), val + ((uint32)((float)itr->second.molecular / itr->second.denominator * 10000)));
				}
				else{
					ref->SetUInt64(_db_player_role, des->field(index), val + itr->second.molecular);
				}
			}
				break;
			default: 
				break;
			}
			++itr;
		}
	}
}

void player_role::add_attr_ext()
{
    const client::role_attr& attr = _ext_attr_mgr->get_all_attr();
    add_attr(attr);

    player_role_ext_attr* ext = _ext_attr_mgr->get(REASTE_MINI_SERVER_SET);
    if (ext != NULL) {
        set_attr(ext->get_attr());
    }
}

void player_role::add_attr_family(uint32 family_id,uint32 family_level)
{
	if (family_id == 0){
		return;
	}

	const google::protobuf::Reflection* ref = get_db_player_role()->GetReflection();
	const google::protobuf::Descriptor* des = get_db_player_role()->GetDescriptor();

	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(family_level);
	if (legion_info == NULL){
		return;
	}

	const std::map<int,int>& atrr_map_ = legion_info->get_add_property();
	std::map<int,int>::const_iterator itr = atrr_map_.begin();
	for (; itr != atrr_map_.end(); ++itr){
        if (itr->first >= GAME_ATTRIBUTE_MAX ) {
            amtsvr_log("attr type error %d\n", itr->first);
            continue;
        }

		int32 index = ATTR_CFG_TO_CODE[itr->first] + ATTR_CODE_TO_DB;
		switch (des->field(index)->type()) {
		case google::protobuf::FieldDescriptor::TYPE_UINT32: {
			uint32 val = ref->GetUInt32(*get_db_player_role(), des->field(index));
			ref->SetUInt32(get_db_player_role(), des->field(index), val + itr->second);
		}
		break;
		case google::protobuf::FieldDescriptor::TYPE_UINT64: {
			uint64 val = ref->GetUInt64(*get_db_player_role(), des->field(index));
			ref->SetUInt64(get_db_player_role(), des->field(index), val + itr->second);
		}
		break;
		default: 
			break;
		}
	}
}

#ifndef __LIB_MINI_SERVER__
uint32 player_role::get_lottery_time(uint32 type)
{
#ifndef __LIB_MINI_SERVER__
	return _lottery->get_lottery_time(type);
#else
    return 0;
#endif
}

void player_role::modify_lottery_time(uint32 type, uint32 role_id)
{
#ifndef __LIB_MINI_SERVER__
	_lottery->modify_lottery_time(type, role_id);
#endif
}

void player_role::modify_used_num(uint32 n /* = 1*/)
{
#ifndef __LIB_MINI_SERVER__
    _lottery->modify_used_num(get_role_id(), n);
#endif
}

uint32 player_role::get_used_num()
{
#ifndef __LIB_MINI_SERVER__
	return _lottery->get_used_num();
#else
    return 0;
#endif
}

uint32 player_role::get_lottery_prop_num(uint32 lottery_id)
{
#ifndef __LIB_MINI_SERVER__
	return _lottery->get_prop_num(lottery_id);
#else
    return 0;
#endif
}

void player_role::add_lottery_prop_num(uint32 lottery_id, uint32 num /* = 1*/)
{
#ifndef __LIB_MINI_SERVER__
	_lottery->add_prop_num(lottery_id, num);
#endif 
}
#endif

void player_role::update_role_levels_info(uint32 role_id, uint32 lv, uint32 power)
{
	_levels->update_role_info(role_id, lv, power);
}

void player_role::update_role_levels_info()
{
	_levels->update_role_info();
}

void player_role::init_role_levels()
{
	_levels->init_role_levels(_db_player_role->role_id(), _db_player_role->profession(), _db_player_role->level(), _db_player_role->fighting_power());
}

void player_role::set_ban(uint32 type, uint32 end_time)
{
    if (get_db_player_role() != NULL) {
        get_db_player_role()->set_status(type);
        get_db_player_role()->set_is_suspended_account(end_time);
    }
}

#ifndef __LIB_MINI_SERVER__
void player_role::init_role_cycle()
{
	_cycle->init_role_cycle(_db_player_role->role_id());
}

void player_role::init_role_devil()
{
	_devil->init_role_devil(_db_player_role->role_id());
}

int player_role::set_vip_lv(uint32 lv, bool month)
{
	const config_vip *cv = CONFMGR->get_config_vip(lv);
	if(NULL == cv)
	{
		return 0;
	}
	player_role_vip &prv = get_player_role_vip();
	if(prv.get_month_timeout() > (uint32)time(NULL))
	{
		month = false;
	}
	prv.set_vip_lv(get_role_id(), lv, month);
    get_role_props().resize(cv->_grid, cv->_grid);
	_db_player_role->set_backpack_size(cv->_grid);
	_db_player_role->set_warehouse_size(cv->_grid);
	_db_player_role->set_vip_level(lv);
//	update_online_smy_info(client::SMY_ROLE_VIP, lv);
	return lv;
}

#endif

void player_role::reset_role_levels()
{
	_levels->init_all_levels(_db_player_role->profession());
}

