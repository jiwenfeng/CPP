#include "config_beauty.h"
#include "config_magic.h"
#include "scene_inst.h"
#include "origin_npc.h"
#include "scene_npc.h"
#include "scene_user.h"
#include "scene_pet.h"
#include "props_parser.h"
#include "player_role.h"
#include "config_props.h"
#include "player_role_props.h"
#include "config_equip_upgrade.h"

scene_pet::scene_pet(const origin_npc& on, client::pet_db_info* prp, uint64 pet_inst_id, scene_role* master, bool is_full /*= true*/) 
    : pet(prp)
    , scene_npc(on, pet_inst_id)
{
    get_origin_npc().set_role_type(RTE_PET);
    set_role_type(RTE_PET);
    if (master != NULL) {
        set_master(master); 
        attach_scene(get_master()->get_scene());
    }

	_pet_status = client::PET_STATUS_TRUCE;
    init(is_full, on);
    set_kill_count(0);
    
}

scene_pet::~scene_pet()
{
}

int scene_pet::init(bool is_full, const origin_npc& on)
{
    scene_npc::init();

    if (get_pet() != NULL) {
        // 初始化宠物技能
        init_scene_pet_skill();
       if (recalculation_pet_attr(get_pet()->pet_id(), get_pet()->pet_level(), on, is_full)){
		   // 美人设置为出战状态
		   _pet_status = client::PET_STATUS_FIGHTING;
       }
    }

    return 0;
}

void scene_pet::init_scene_pet_skill(const vector<skill> &v)
{
	mutable_skill()->remove_all_skill();
	for(size_t i = 0; i < v.size(); ++i)
	{
		mutable_skill()->add_skill(v[i]);
	}
	mutable_skill()->init_min_atk_range();
	mutable_skill()->init_max_anger();
}

void scene_pet::init_scene_pet_skill()
{
    const role_skill& master_skill = get_master()->get_skill();
    std::vector<skill> pet_skill_vec;
    master_skill.get_pet_skill(get_pet()->pet_id(), get_pet()->pet_level(), pet_skill_vec);
    mutable_skill()->remove_all_skill();
    for (size_t i = 0; i < pet_skill_vec.size(); ++i) {
        mutable_skill()->add_skill(pet_skill_vec[i]);
    }

    mutable_skill()->init_min_atk_range();
    mutable_skill()->init_max_anger();
}

#if 0
void scene_pet::init_scene_pet_skill(const vector<uint32>& v)
{
	for(size_t i = 0; i < v.size(); ++i)
	{
		const config_skill *cs = CONFMGR->get_config_skill(v[i]);
		if(cs == NULL)
		{
			continue;
		}
		mutable_skill()->add_skill(skill(*cs));
	}
	mutable_skill()->init_min_atk_range();
	mutable_skill()->init_max_anger();
}
#endif

bool scene_pet::on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    trans_to_master_pos();

    return scene_npc::on_update(tick, user_num, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

bool scene_pet::on_status_stand(uint32 tick)
{
    scene_npc::on_status_stand(tick);

    if (get_scene()->to_cell_pos(get_scene_pos()).get_distance(get_scene()->to_cell_pos(get_born_pos())) > 2) {
        return go_back();
    }

    return true;
}

bool scene_pet::on_status_dead (uint32 tick)
{
    if (get_pet() == NULL) {
        return scene_npc::on_status_dead(tick);
    }

    if (get_role_id() == get_pet()->pet_id()) {
        remove_body(tick);
        return true;
    }

    return true;
}

bool scene_pet::on_update_fiv_second(uint32 tick, uint32 user_num)
{
    if (get_pet() == NULL) {
        return scene_role::on_update_fiv_second(tick, user_num);
    }

    if (get_pet()->status() != PET_STATUS_FIGHTING || get_pet()->cur_hp() == 0 || get_pet()->cur_hp() == get_attr(MAX_HP)) {
        return false;
    }

    // 美人回血功能
    const uint32 blood_time = 1500;
    if (tick - get_last_recover_tick() > blood_time || get_last_recover_tick() == 0) {
        set_last_recover_tick(tick);
        uint32 recover_value = get_attr(RECOVER);
        uint32 hp = get_attr(CUR_HP) + recover_value;
        hp = hp < get_attr(MAX_HP) ? hp : get_attr(MAX_HP);
        get_pet()->set_cur_hp(hp);
        mutable_attr()->set_val(CUR_HP, hp);
        send_client_change_attr();
    }

    return true;
}

int scene_pet::on_kill(scene_role* psr)
{
    add_kill_count();

    if (get_master()->get_role_type() == RTE_USER) {
        client::pet_kill_count_msg msg;
        msg.set_id(get_role_id());
        msg.set_count(get_kill_count());
        send_client(msg);
    }

    if (get_master() != NULL) { // 宠物杀的人，要寄放在其主人的身上
        get_master()->on_kill(psr);
    }

    return 0;
}

int scene_pet::on_killed(scene_role* psr)
{
    if (get_pet() != NULL) {
        get_pet()->set_cur_hp(0);
#ifndef __LIB_MINI_SERVER__
        // 美人死亡通知
        client::pet_id_rsp rsp_msg;
        client::pet_db_info* pi = rsp_msg.mutable_pet_info();
        pi->CopyFrom(*get_pet());
        get_master()->send_client(rsp_msg);
#endif // __LIB_MINI_SERVER__
    }

    return scene_role::on_killed(psr);
}

void scene_pet::fill_role_data(client::role_data& rd) const
{
    scene_role::fill_role_data(rd);
    if (get_master() != NULL) {
        rd.set_pet_id(get_master()->get_inst_id());
    }
}

void scene_pet::fill_role_name(client::role_name& rn)
{
    fill_role_data(*rn.mutable_rd());
    if (get_master() != NULL) {
        rn.set_name(get_master()->get_role_name());
    } else {
        rn.set_name(get_role_name());
    }
}

void scene_pet::on_reset(bool reborn /* = true */)
{
    map_postion mp = get_scene_pos();

    scene_npc::on_reset(reborn);

    set_scene_pos(mp);
    if (get_master() == NULL) {
        set_born_pos(get_scene_pos());
    } else {
        set_born_pos(get_master()->get_scene_pos());
    }

    set_kill_count(0);
}

bool scene_pet::on_pet_attr_change(uint32 pet_id,uint32 opt_type)
{
	if (pet_id != get_pet()->pet_id()){
		return false;
	}

    recalculation_pet_attr(get_pet()->pet_id(), get_pet()->pet_level(), get_origin_npc(), true);

	if (opt_type == PET_ATTR_CHANGE_TYPE_LEVEL){
		init_scene_pet_skill();
	}

    return true;
}

bool scene_pet::on_skill_effect_up(uint32 pet_id,uint32 skill_id,uint32 upgrade_level)
{
    // 更新相关技能的数据
    if (get_role_id() != pet_id) {
        amtsvr_log("src:%u dst:%u\n", get_role_id(), pet_id);
    }

    _rsk.upgrade_skill_effect(skill_id,upgrade_level);

    return true;
}

bool scene_pet::recalculation_pet_attr(uint32 pet_id, uint32 pet_level, const origin_npc& on, bool is_full_hp /* = false */)
{
    origin_self.operator=(on);
    origin_npc* origin_obj = &origin_self;

    const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(get_pet()->pet_id(), pet_level);
    if (base_info != NULL) {
        set_origin_attr(origin_obj, base_info->get_game_attribute());
    }else {
		return false;
	}
	const config_magic* magic_info = CONFMGR->get_config_magic_mgr().get_config_magic(base_info->get_magic_id(), get_pet()->magic_star());
    // 装备属性附加,神兵附加
    add_equip_attr(origin_obj, get_role_id(),magic_info);
	// 宠物等级
	origin_obj->mutable_attr()->set_val(LEVEL,pet_level);
   
    //float magic_per = 0.0f;
    //if (magic_info != NULL) {
    //    magic_per = magic_info->get_magic_add_attack() / 10000.0f;
    //}
	
    // 最大攻击与最小攻击
    //uint32 attack_min = (uint32)origin_obj->mutable_attr()->get_val(ATTACK_MIN);
    //uint32 pet_attack_min = attack_min + static_cast<uint32>(ceil(attack_min * magic_per));
    //uint32 atttack_max = (uint32)origin_obj->mutable_attr()->get_val(ATTACK_MAX);
    //uint32 pet_atttack_max = atttack_max + static_cast<uint32>(ceil(atttack_max * magic_per));

    // 美人移动速度与主人速度相同
    uint32 master_move_speed = (uint32)get_master()->get_attr().get_val(MOVE_SPEED);

    // 血量的修正
    uint32 pet_hp_max = origin_obj->get_attr(MAX_HP);

    uint32 cur_hp = 0;
    if (is_full_hp) {
        cur_hp = pet_hp_max;
    }else{
        cur_hp = get_pet()->cur_hp();
    }

	if (cur_hp == 0){
		cur_hp = pet_hp_max;
	}

    origin_obj->mutable_attr()->set_val(CUR_HP, cur_hp);
    origin_obj->mutable_attr()->set_val(MOVE_SPEED, master_move_speed);
    //origin_obj->mutable_attr()->set_val(ATTACK_MIN, pet_attack_min);
    //origin_obj->mutable_attr()->set_val(ATTACK_MAX, pet_atttack_max);

    set_attr(CUR_HP, cur_hp);
    reset_original_attr();
    send_client_change_attr();
    return true;
}

void scene_pet::set_origin_attr(const std::map<int, int> &m)
{
	set_origin_attr(&origin_self, m);
	uint32 cur_hp = origin_self.get_attr(MAX_HP);
	origin_self.mutable_attr()->set_val(CUR_HP, cur_hp);
	set_attr(CUR_HP, cur_hp);
	reset_original_attr();
	send_client_change_attr();
}

void scene_pet::set_origin_attr(origin_npc* origin_obj, const std::map<int, int>& a)
{
    const google::protobuf::Reflection* ref = origin_obj->mutable_attr()->GetReflection();
    const google::protobuf::Descriptor* des = origin_obj->mutable_attr()->GetDescriptor();
    if (ref == NULL || des == NULL) {
        return ;
    }

    for (std::map<int, int>::const_iterator i = a.begin(); i != a.end(); ++i) {
        int32 index = ATTR_CFG_TO_CODE[i->first];
        if (des->field_count() <= index || index < MAX_HP || index >= MOVE_SPEED) {
            continue;
        }

        //index += ATTR_CODE_TO_DB;
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            ref->SetUInt32(origin_obj->mutable_attr(), des->field(index), i->second);
                                                             }
                                                             break;
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            ref->SetUInt64(origin_obj->mutable_attr(), des->field(index), i->second);
                                                             }
                                                             break;
        default: break;
        }
    }
}

void scene_pet::add_equip_attr(origin_npc* origin_obj, uint32 pet_id,const config_magic* magic_info)
{
    if (get_master() == NULL || get_master()->get_role_type() != RTE_USER) {
        return ;
    }

    scene_user* master_su = static_cast<scene_user*>(get_master());
    if (master_su == NULL) {
        return ;
    }

    const grid_role_props* equip_info = NULL;
    const google::protobuf::Reflection* ref = origin_obj->mutable_attr()->GetReflection();
    const google::protobuf::Descriptor* des = origin_obj->mutable_attr()->GetDescriptor();
    props_parser& equip_pack = master_su->get_player_role().get_role_props().get_equip();

    uint32 begin_index = EQUIP_GRID_BEG + pet_id * 10;
    uint32 end_index = EQUIP_SHOE + pet_id * 10;
    for (uint32 i = begin_index; i <= end_index; ++i) {
        equip_info = equip_pack.get_props(i);
        if (equip_info == NULL || !equip_info->has_props()) {
            continue;
        }

        // 随机属性加成
        for (int j = 0; j < equip_info->attr_size(); ++j) {
            const client::attribute& attr = equip_info->attr(j);
            int32 index = ATTR_CFG_TO_CODE[attr.type()];
            switch (des->field(index)->type()) {
            case google::protobuf::FieldDescriptor::TYPE_UINT32: {
                uint32 val = ref->GetUInt32(*origin_obj->mutable_attr(), des->field(index));
                ref->SetUInt32(origin_obj->mutable_attr(), des->field(index), val + attr.value());
                                                                 }
                                                                 break;												 
            case google::protobuf::FieldDescriptor::TYPE_UINT64: {
                uint64 val = ref->GetUInt64(*origin_obj->mutable_attr(), des->field(index));
                ref->SetUInt64(origin_obj->mutable_attr(), des->field(index), val + attr.value());
                                                                 }
                                                                 break;
            default: break;
            }
        }

        const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(equip_info->props_id());
        if (base_info == NULL) {
            continue;
        }

        // 主属性加成
        const std::map<int, int>& main_attr_map_ = base_info->get_game_attribute();
        std::map<int, int>::const_iterator itr = main_attr_map_.begin();
        for ( ; itr != main_attr_map_.end(); ++itr) {
            int32 index = ATTR_CFG_TO_CODE[itr->first];
            switch (des->field(index)->type()) {
            case google::protobuf::FieldDescriptor::TYPE_UINT32: {
                uint32 val = ref->GetUInt32(*origin_obj->mutable_attr(), des->field(index));
                ref->SetUInt32(origin_obj->mutable_attr(), des->field(index), val + itr->second);
                                                                 }
																 break;
            case google::protobuf::FieldDescriptor::TYPE_UINT64: {
                uint64 val = ref->GetUInt64(*origin_obj->mutable_attr(), des->field(index));
                ref->SetUInt64(origin_obj->mutable_attr(), des->field(index), val + itr->second);
                                                                 }
																 break;
            default: break;
            }
        }
		
        //强化的属性加成
		for (int k = 0; k < equip_info->upgrade_attr_size(); ++k) {
			const client::attribute& up_attr = equip_info->upgrade_attr(k);
			int32 index = ATTR_CFG_TO_CODE[up_attr.type()];
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*origin_obj->mutable_attr(), des->field(index));
				ref->SetUInt32(origin_obj->mutable_attr(), des->field(index), val + up_attr.value());
																 }
																 break;												 
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*origin_obj->mutable_attr(), des->field(index));
				ref->SetUInt64(origin_obj->mutable_attr(), des->field(index), val + up_attr.value());
																 }
																 break;
			default: break;
			}
		}
    }

	// 神兵附近属性
	if (magic_info != NULL){
		const std::map<int,int>& up_general_map_ = magic_info->get_up_general();
		std::map<int,int>::const_iterator it_general = up_general_map_.begin();
		for (;it_general != up_general_map_.end(); ++it_general){
			int32 index = ATTR_CFG_TO_CODE[it_general->first];
			switch (des->field(index)->type()) {
			case google::protobuf::FieldDescriptor::TYPE_UINT32: {
				uint32 val = ref->GetUInt32(*origin_obj->mutable_attr(), des->field(index));
				ref->SetUInt32(origin_obj->mutable_attr(), des->field(index), val + static_cast<uint32>(it_general->second));
																 }
																 break;												 
			case google::protobuf::FieldDescriptor::TYPE_UINT64: {
				uint64 val = ref->GetUInt64(*origin_obj->mutable_attr(), des->field(index));
				ref->SetUInt64(origin_obj->mutable_attr(), des->field(index), val + static_cast<uint32>(it_general->second));
																 }
																 break;
			default: break;
			}
		}
	}
}

// 特殊处理美人扣血
uint32 scene_pet::sub_hp(uint32& hp)
{
    if (get_pet() == NULL) {
        return scene_npc::sub_hp(hp);
    }

    // 如果不是出战状态则返回
    if (get_pet()->status() != client::PET_STATUS_FIGHTING && _pet_status != client::PET_STATUS_FIGHTING) {
        return 0;
    }

    const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(get_pet()->pet_id(), get_pet()->pet_level());
    if (base_info == NULL) {
        return 0;
    }

    mutable_buffer()->calculate_shield(mutable_attr(), hp);
    uint32 subtract_hp = hp;
    uint32 cur = get_attr().get_val(CUR_HP) > subtract_hp ? get_attr().get_val(CUR_HP) - subtract_hp : 0;
    mutable_attr()->set_val(CUR_HP, cur);
    get_pet()->set_cur_hp(cur);

    if (cur == 0) {
        mutable_status()->set_dead();
    }

    return cur;
}

scene_role* scene_pet::get_hatest_role()
{
    if (get_pet() != NULL && get_master() != NULL && get_scene() != NULL) {
        uint64 hatest_role = get_master()->get_hate_role();
        scene_role* sr = get_scene()->get_scene_role(hatest_role);
        if (sr != NULL && !sr->get_status().is_dead()) {
            return sr;
        } else {
            get_master()->del_hate_role(hatest_role);
        }
    }

    return scene_npc::get_hatest_role();
}

uint32 scene_pet::get_camp() const
{
    if (get_master() == NULL) {
        return 0;
    }

    return get_master()->get_camp();
}

uint32 scene_pet::get_team_id() const
{
    if (get_master() == NULL) {
        return 0;
    }

    return get_master()->get_team_id();
}

uint32 scene_pet::get_family_id() const
{
    if (get_master() == NULL) {
        return 0;
    }

    return get_master()->get_family_id();
}

void scene_pet::add_counter_role(scene_role *sr, uint32 hate)
{
	scene_role *master = get_master();
	if(master == NULL)
	{
		return ;
	}
	master->add_counter_role(sr, hate);
}

void scene_pet::del_counter_role(scene_role *sr)
{
	scene_role *master = get_master();
	if(master == NULL)
	{
		return ;
	}
	master->del_counter_role(sr);
}

void scene_pet::clear_counter_list()
{
}

bool scene_pet::is_counter_role(scene_role *sr)
{
	scene_role *master = get_master();
	if(master == NULL)
	{
		return false;
	}
	return master->is_counter_role(sr);
}
