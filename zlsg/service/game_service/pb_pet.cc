#include "config_beauty.h"
#include "config_props.h"
#include "config_magic.h"
#include "game_interface.h"
#include "player_role.h"
#include "player_role_props.h"
#include "player_role_skill_mgr.h"
#include "scene_user.h"
#include "scene_pet.h"
#include "scene_inst.h"
#include "config_raid.h"
string get_attr_name(int type);
// 招募
void* on_pb_pet_recruit_req(client::pet_recruit_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    player_role_pet& player_pet = su->get_player_role().get_pets();
    client::pet_db_info pet_info;
    if (player_pet.get_pet_info(req->pet_id(),pet_info)){
        return NULL;
    }

    const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(req->pet_id(),1);
    if (base_info == NULL){
        return NULL;
    }

    uint32 master_level = su->mutable_attr()->get_val(LEVEL);
    if (master_level < base_info->get_recruit_level()){
        return NULL;
    }

    const std::map<int32,int32>& recruit_need_item_map_ = base_info->get_recruit_need_item();
    if (recruit_need_item_map_.size() == 0){
        return NULL;
    }

    props_parser& pack_obj = su->get_player_role().get_role_props().get_pack();

    std::map<int32,int32>::const_iterator itr = recruit_need_item_map_.begin();
    for (; itr != recruit_need_item_map_.end(); ++itr){
        if (pack_obj.get_props_num(itr->first)< static_cast<uint32>(itr->second)){
            return NULL;
        }
    }
    uint32 master_hp = su->mutable_attr()->get_val(MAX_HP);
    // 创建宠物
    if (!player_pet.create_pet(req->pet_id(),pet_info,master_hp)){
        return NULL;
    }

    uint32 player_level = su->mutable_attr()->get_val(LEVEL);
    const std::vector<int>& skill_id_vec = base_info->get_general_skill();
    // 招募新美人开放技能
    if (skill_id_vec.size() != 0){
        for (size_t i = 0; i < skill_id_vec.size(); ++i){
            const config_skill* skill_base_info = CONFMGR->get_config_skill_mgr().get_config_skill(skill_id_vec[i]);
            if (skill_base_info == NULL){
                continue;
            }

            const std::map<std::string, int>& learn_need_level_map_ = skill_base_info->get_learn_need_levels();
            std::map<std::string, int>::const_iterator itr = learn_need_level_map_.begin();
            //uint32 limit_level = 0;
            bool is_match = true;
            for (;itr != learn_need_level_map_.end(); ++itr){
                if (itr->first == "*"){
                    if (static_cast<uint32>(itr->second) > player_level){
                        is_match = false;
                    }
                }
                else if (itr->first == "!"){
                    if (static_cast<uint32>(itr->second) > pet_info.pet_level()){
                        is_match = false;
                    }
                }
                else if (itr->first == "#"){
                    if (static_cast<uint32>(itr->second) > pet_info.magic_star()){
                        is_match = false;
                    }
                }
            }

            if (is_match){
                su->get_player_role().get_role_skill_mgr().add_skill(req->svr_head().role_id(),skill_id_vec[i]);
                const config_skill* skill_info = CONFMGR->get_config_skill(skill_id_vec[i]);
                if (skill_info != NULL){
                    skill sk(*skill_info);
                    su->mutable_skill()->add_skill(sk);
                }
            }

        }
    }

    // 扣除碎片
    std::map<int32,int32>::const_iterator it_1 = recruit_need_item_map_.begin();
    for (; it_1 != recruit_need_item_map_.end(); ++it_1){
        if (pack_obj.get_props_num(it_1->first) >= static_cast<uint32>(it_1->second)){
            pack_obj.remove_props(static_cast<uint32>(it_1->first),static_cast<uint32>(it_1->second));
        }
    }

    su->send_client_change_props_msg();
    client::pet_recruit_rsp recruit_rsp;
    recruit_rsp.set_recruit_type(RECRUIT_SUCCESS);
    client::pet_db_info* info = recruit_rsp.mutable_pet_info();
    info->CopyFrom(pet_info);
    su->send_client(recruit_rsp);

    // 宠物技能下发
    client::role_skill_msg rsm;
    su->fill_pets_skill_msg(rsm,pet_info.pet_id(),pet_info.pet_level());
    su->send_client(rsm);
	CHECK_TARGET_PROGRESS(su, PET_NUMBER);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(), req->pet_id());

    return NULL;
}
// 缠绵
void* on_pb_pet_lingering_req(client::pet_lingering_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    player_role_pet& player_pet = su->get_player_role().get_pets();
    client::pet_db_info pet_info;
    if (!player_pet.get_pet_info(req->pet_id(),pet_info)){
        return NULL;
    }
    // 获取缠绵数据
    const config_beauty* base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty_by_lingering_count(pet_info.pet_id(),pet_info.lingering_count());
    if (base_info == NULL){
        return NULL;
    }
	
    if (pet_info.pet_level() < pet_info.lingering_count() +1){
        return NULL;
    }

    player_role_props& prg = su->get_player_role().get_role_props();
    props_parser& pack_obj = prg.get_pack();
    const std::map<int32,int32>& need_item_map_ = base_info->get_fit_need_item();
    //检测物品是否足够
    std::map<int32,int32>::const_iterator it = need_item_map_.begin();
	uint32 need_item_id = 0;
	uint32 need_item_count = 0;
	uint32 old_item_count = 0;
    for (;it != need_item_map_.end(); ++it){
		need_item_id = static_cast<uint32>(it->first);
		need_item_count = static_cast<uint32>(it->second);
		old_item_count = pack_obj.get_props_num(need_item_id);
        if (old_item_count < need_item_count){
            return NULL;
        }
    }

	uint32 cur_item_count = old_item_count - need_item_count;
	const config_props* base_item = CONFMGR->get_config_props(need_item_id);
	if (base_item == NULL){
		return NULL;
	}

	uint32 old_linger_count = pet_info.lingering_count();
    // 修改缠绵状态
    pet_info.set_lingering_count(pet_info.lingering_count() + 1);
	uint32 new_linger_count = pet_info.lingering_count();
    if (player_pet.change_pet_info(pet_info) != 0 ){
        return NULL;
    }
    //删除物品
    std::map<int32,int32>::const_iterator it_1 = need_item_map_.begin();
    for (;it_1 != need_item_map_.end(); ++it_1){
        pack_obj.remove_props(it_1->first,it_1->second);
    }

    pet_lingering_rsp lingering_rsp;
    lingering_rsp.set_result_type(pet_lingering_rsp_result_success);
    client::pet_db_info* info = lingering_rsp.add_pet_info();
    info->CopyFrom(pet_info);
    su->calculate_attr();  
    su->send_client_change_attr();
    su->send_client_change_props_msg();
    su->send_client(lingering_rsp);
	CHECK_TARGET_PROGRESS(su, PET_LINGERING, pet_info.lingering_count());

	std::map<int,int> old_attr_map_;
	std::map<int,int> new_attr_map_;
	old_attr_map_.clear();
	new_attr_map_.clear();
	// 缠绵前属性值
	if (old_linger_count != 0){
		do 
		{
			const config_beauty* old_info = CONFMGR->get_config_beauty_mgr().get_config_beauty_by_lingering_count(pet_info.pet_id(),old_linger_count-1);
			if (old_info == NULL){
				break;
			}

			old_attr_map_ = old_info->get_fit_add_property();
			break;
		} while (false);
	}
	// 缠绵后属性值
	do 
	{
		const config_beauty* new_info = CONFMGR->get_config_beauty_mgr().get_config_beauty_by_lingering_count(pet_info.pet_id(),new_linger_count-1);
		if (new_info == NULL){
			break;
		}

		new_attr_map_ = new_info->get_fit_add_property();
		break;
	} while (false);

	std::string a = "";
	if (old_attr_map_.size() == 0){
		std::map<int ,int>::iterator it = new_attr_map_.begin();
		char tmp[1024] = {0};
		for (;it != new_attr_map_.end();++it){
			memset(tmp, 0, sizeof(tmp));
			// %u|%u|%u 属性类型|前值|后值
			string name = get_attr_name(it->first);
			snprintf(tmp, sizeof(tmp), "|属性:%s,前值:%u/后值:%u", name.c_str(),0,it->second);
			a += tmp;
		}
	}else{
		std::map<int ,int>::iterator it_old = old_attr_map_.begin();
		char tmp[1024] = {0};
		for (; it_old != old_attr_map_.end();++it_old){
			std::map<int,int>::iterator it_new = new_attr_map_.find(it_old->first);
			if (it_new != new_attr_map_.end()){
				memset(tmp, 0, sizeof(tmp));
				string name = get_attr_name(it_old->first);
				snprintf(tmp, sizeof(tmp), "|属性:%s,前值:%u/后值:%u", name.c_str(),it_old->second,it_new->second);
				a += tmp;
			}
		}
	}

	const config_beauty* beauty_info = CONFMGR->get_config_beauty(pet_info.pet_id(),pet_info.pet_level());
	if (beauty_info != NULL){
		string general_name = beauty_info->get_general_name();
		string item_name = base_item->get_item_name();
		uint32 l_ling_count = pet_info.lingering_count();
		// 美人XX|消耗物品ID(物品名字)|前值XX/后值XX；增加 属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；已缠绵次数XX；
		amtsvr_billlog(ss->ctx, "%u|%s|%s|%s|%u|%u%s|%u", su->get_role_id(), req->GetTypeName().c_str(), general_name.c_str(),item_name.c_str(),old_item_count,cur_item_count,a.c_str(),l_ling_count);
	}

    return NULL;
}
// 神兵升级
void* on_pb_pet_magic_upgrade_req(client::magic_upgrade_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }
    client::magic_upgrade_rsp rsp_msg;
    player_role_pet& player_pet = su->get_player_role().get_pets();
    client::pet_db_info pet_info;
    if (!player_pet.get_pet_info(req->pet_id(),pet_info)){
        return NULL;
    }
    // 神兵到达100星不能再吃碎片了
    if (pet_info.magic_star() >= 100){
        return NULL;
    }

	uint32 l_old_star = pet_info.magic_star();
	
    // 获取美人表数据
    const config_beauty* pet_base_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_info.pet_id(),pet_info.pet_level());
    if (pet_base_info == NULL){
        return NULL;
    }

	uint32 l_magic_id  = pet_base_info->get_magic_id();

    player_role_props& prg = su->get_player_role().get_role_props();
    props_parser& pack_obj = prg.get_pack();

    const config_magic* magic_info = CONFMGR->get_config_magic_mgr().get_config_magic(pet_base_info->get_magic_id(),pet_info.magic_star());
    if (magic_info == NULL){
        return NULL;
    }
    const std::map<int,int>& item_need_map_ = magic_info->get_star_need();
    std::map<int,int>::const_iterator itr = item_need_map_.begin();
    // 当前身上所拥有的道具
    uint32 cur_item_count = 0;
    // 检测身上的道具数量
    for (;itr != item_need_map_.end(); ++itr){
        cur_item_count = pack_obj.get_props_num(itr->first);

    }

    if (cur_item_count == 0){
        return NULL;
    }


    uint32 total_exp = pet_info.debris_exp() + cur_item_count;
    // 升级所需的物品数量
    uint32 need_item_count = 0;
    // 升级碎片ID
    uint32 need_item_id = 0;
	// 道具消耗前值：
	uint32 l_old_count = cur_item_count;
	// 记录消耗具数量
	uint32 l_item_count = 0;
	// 神兵开放的技能
	//uint32 skill_id = 0;
    while(true) {
        if (pet_info.magic_star() >= 100){
            break;
        }

        // 真实消耗的道具数量
        uint32 total_pay_item_count = 0;

        // 获取神兵表数据
        const config_magic* magic_info = CONFMGR->get_config_magic_mgr().get_config_magic(pet_base_info->get_magic_id(),pet_info.magic_star());
        if (magic_info == NULL){
            return NULL;
        }

        const std::map<int,int>& item_need_map_ = magic_info->get_star_need();
        std::map<int,int>::const_iterator itr = item_need_map_.begin();
        for (;itr != item_need_map_.end(); ++itr){
            need_item_count = static_cast<uint32>(itr->second);
            need_item_id = static_cast<uint32>(itr->first);
            cur_item_count = pack_obj.get_props_num(itr->first);
        }

        //// 记录消耗的道具数
        if (cur_item_count > need_item_count){
            total_pay_item_count += need_item_count;
        }
        else{
            total_pay_item_count += cur_item_count;
        }

        if (total_exp < need_item_count ){// 加经验
            pet_info.set_debris_exp(total_exp);
            player_pet.change_pet_info(pet_info);
            pack_obj.remove_props(need_item_id,total_pay_item_count);
			l_item_count += total_pay_item_count;
            break;
        }
        else{// 升级
            pet_info.set_magic_star(pet_info.magic_star() + 1);
            total_exp -= need_item_count;
            if (pet_info.magic_star() >= 100){
                pet_info.set_debris_exp(0);
                // 多出的减回来
                total_pay_item_count -= total_exp; 
            }
            else{
                pet_info.set_debris_exp(total_exp);
            }

            player_pet.change_pet_info(pet_info);

            // 扣除碎片
            pack_obj.remove_props(need_item_id,total_pay_item_count);
			l_item_count += total_pay_item_count;
        }
    }


    uint32 player_level = su->mutable_attr()->get_val(LEVEL);
    const std::vector<int>& skill_id_vec = pet_base_info->get_general_skill();
    // 招募新美人开放技能
    if (skill_id_vec.size() != 0){
        for (size_t i = 0; i < skill_id_vec.size(); ++i){
            const config_skill* skill_base_info = CONFMGR->get_config_skill_mgr().get_config_skill(skill_id_vec[i]);
            if (skill_base_info == NULL){
                continue;
            }

            const std::map<std::string, int>& learn_need_level_map_ = skill_base_info->get_learn_need_levels();
            std::map<std::string, int>::const_iterator itr = learn_need_level_map_.begin();
            //uint32 limit_level = 0;
            bool is_match = true;
            for (;itr != learn_need_level_map_.end(); ++itr){
                if (itr->first == "*"){
                    if (static_cast<uint32>(itr->second) > player_level){
                        is_match = false;
                    }
                }
                else if (itr->first == "!"){
                    if (static_cast<uint32>(itr->second) > pet_info.pet_level()){
                        is_match = false;
                    }
                }
                else if (itr->first == "#"){
                    if (static_cast<uint32>(itr->second) > pet_info.magic_star()){
                        is_match = false;
                    }
                }
            }

            if (is_match){
                su->get_player_role().get_role_skill_mgr().add_skill(req->svr_head().role_id(),skill_id_vec[i]);
                const config_skill* skill_info = CONFMGR->get_config_skill(skill_id_vec[i]);
                if (skill_info != NULL){
                    skill sk(*skill_info);
                    su->mutable_skill()->add_skill(sk);
                }
            }

        }
    }

    su->calculate_attr();
    su->send_client_change_props_msg();
    rsp_msg.set_result_type(0);
    client::pet_db_info* info = rsp_msg.add_pet_info();
    info->CopyFrom(pet_info);
    su->send_client(rsp_msg);

    client::role_skill_msg rsm;
    su->fill_pets_skill_msg(rsm,pet_info.pet_id(),pet_info.pet_level());
    su->send_client(rsm);
	////////////////////////////////////////log///////////////////////////////////////////
	uint32 l_new_start = pet_info.magic_star();
	std::map<int,int> old_attr_map_;
	std::map<int,int> new_attr_map_;
	old_attr_map_.clear();
	new_attr_map_.clear();
	// 属性前值：
	if (l_old_star != 0){
		do 
		{
			const config_magic* old_magic_base = CONFMGR->get_config_magic(l_magic_id,l_old_star);
			if (old_magic_base == NULL){
				break;
			}

			old_attr_map_ = old_magic_base->get_up_charater();
			break;
		} while (false);
	}
	// 属性后值：
	do 
	{

		const config_magic* new_magic_base = CONFMGR->get_config_magic(l_magic_id,l_new_start);
		if (new_magic_base == NULL){
			break;
		}

		new_attr_map_ = new_magic_base->get_up_charater();
		break;
	} while (false);

	std::string a = "";
	if (old_attr_map_.size() == 0){
		std::map<int ,int>::iterator it = new_attr_map_.begin();
		char tmp[1024] = {0};
		for (;it != new_attr_map_.end();++it){
			memset(tmp, 0, sizeof(tmp));
			// %u|%u|%u 属性类型|前值|后值
			string name = get_attr_name(it->first);
			snprintf(tmp, sizeof(tmp), "|属性:%s,前值:%u/后值:%u",name.c_str(),0,it->second);
			a += tmp;
		}
	}else{
		std::map<int ,int>::iterator it_old = old_attr_map_.begin();
		char tmp[1024] = {0};
		for (; it_old != old_attr_map_.end();++it_old){
			std::map<int,int>::iterator it_new = new_attr_map_.find(it_old->first);
			if (it_new != new_attr_map_.end()){
				memset(tmp, 0, sizeof(tmp));
				string name = get_attr_name(it_old->first);
				snprintf(tmp, sizeof(tmp), "|属性:%s,前值:%u/后值:%u", name.c_str(),it_old->second,it_new->second);
				a += tmp;
			}
		}
	}
	// 实际旧阶数;
	uint32 l_old_stage = (l_old_star / 10) + 1;
	// 实际旧星数
	uint32 l_old_true_start = l_old_star % 10;
	// 实际新阶数
	uint32 l_new_stage = (l_new_start / 10) + 1;
	// 实际新星数
	uint32 l_new_true_start = l_new_start % 10;
	// 消耗物品后值
	uint32 l_new_item_count = l_old_count - l_item_count;
	// 美人XX|消耗物品ID（物品名字）前值XX/后值XX；前值XX阶XX星/后值XX阶XX星；
	//属性值XX，前值XX/后值XX；XX（比如生命）属性值XX，前值XX/后值XX；XX（比如生命）属性值XX，前值XX/后值XX；
	const config_props* item_info = CONFMGR->get_config_props(need_item_id);
	if (item_info != NULL){
		amtsvr_billlog(ss->ctx, "%u|%s|%s|%s|%u|%u|%u|%u|%u|%u%s", su->get_role_id(), req->GetTypeName().c_str(), pet_base_info->get_general_name().c_str(),
			item_info->get_item_name().c_str(),l_old_count,l_new_item_count,l_old_stage,l_old_true_start,l_new_stage,l_new_true_start,a.c_str());
	}

    return NULL;
}
// 升级
void* on_pb_pet_levelup_req(client::pet_levelup_req* req, void* ud, size_t* rsp_len)
{

    return NULL;
}
// 宠物列表请求
void* on_pb_pet_list_req(client::pet_list_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    player_role_pet& player_pet = su->get_player_role().get_pets();

    client::pet_list_rsp list_rsp;
    client::role_data* role_data = 	list_rsp.mutable_rd();
    role_data->set_role_id(req->svr_head().role_id());
    role_data->set_inst_id(su->get_inst_id());
    role_data->set_role_typ((client::role_data_TYPE)su->get_role_type());
    const std::map<uint32, client::pet_db_info>& pet_info_map_ = player_pet.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator itr = pet_info_map_.begin();
    for (; itr != pet_info_map_.end();++itr){
        client::pet_db_info* info = list_rsp.add_pet_info();
        info->CopyFrom(itr->second);
    }

    su->send_client(list_rsp);

    // 美人技能下发数据
    client::role_skill_msg rsm;
    su->fill_all_pets_skill_msg(rsm);
    su->send_client(rsm);

    return NULL;
}

bool check_fighting(std::map<uint32, client::pet_db_info> pet_info_map_);
void pet_operate(scene_user* su,uint32 operate_type,uint32 pet_id,const std::map<uint32, client::pet_db_info>& pet_info_map_,client::pet_rest_or_fight_rsp& rsp_msg);
// 宠物出战与休战
void* on_pb_pet_rest_or_fight_req(client::pet_rest_or_fight_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    player_role_pet& player_pet = su->get_player_role().get_pets();
    client::pet_db_info pet_info;
    if (!player_pet.get_pet_info(req->pet_id(),pet_info)){
        return NULL;
    }

    client::pet_rest_or_fight_rsp rsp_msg;
    rsp_msg.set_pet_id(req->pet_id());
    rsp_msg.set_result_type(PET_FIGHT_FAILD);
    const std::map<uint32, client::pet_db_info>& pet_info_map_ = player_pet.get_all_pet_db_info();

    // 休战与出战处理
    pet_operate(su,req->operate_type(),req->pet_id(),pet_info_map_,rsp_msg);

    su->send_client(rsp_msg);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", su->get_role_id(), req->GetTypeName().c_str(), req->pet_id(),req->operate_type());

    return NULL;
}

bool check_fighting(const std::map<uint32, client::pet_db_info> pet_info_map_)
{
    std::map<uint32, client::pet_db_info>::const_iterator it = pet_info_map_.begin();
    for (;it != pet_info_map_.end(); ++it){
        if (it->second.status() == PET_STATUS_DEAD){
            return false;
        }
    }

    return true;
}

void pet_operate(scene_user* su,uint32 operate_type,uint32 pet_id,const std::map<uint32, client::pet_db_info>& pet_info_map_,client::pet_rest_or_fight_rsp& rsp_msg)
{
    player_role_pet& player_pet = su->get_player_role().get_pets();
    uint32 pet_status = 0;
    switch(operate_type)
    {
    case PET_OPERATE_FIGHT:// 出战
        {
            rsp_msg.set_result_type(PET_FIGHT_FAILD);
            uint32 old_fight_id = 0;
            std::map<uint32, client::pet_db_info>::const_iterator it_old = pet_info_map_.begin();
            for (;it_old != pet_info_map_.end(); ++it_old) {
                if (player_pet.get_pet_status(it_old->first,pet_status) != 0){
                    continue;
                }

                if (pet_status == PET_STATUS_FIGHTING){
                    if (it_old->first == pet_id){
                        return;
                    }

                    old_fight_id = it_old->first;
                }
            }
            bool is_set_prepare = false;
            std::map<uint32, client::pet_db_info>::const_iterator itr = pet_info_map_.begin();
            for (; itr != pet_info_map_.end();++itr){
                if (itr->first == pet_id){
                    if (player_pet.get_pet_status(pet_id,pet_status) != 0){
                        continue;
                    }

                    if (pet_status == PET_STATUS_FIGHTING){
                        break;
                    }
                    // 有出战的并且没有备战的
                    if (player_pet.is_has_pet_fighting() && !player_pet.is_has_pet_prepare()){
                        // 设置为备战
                        player_pet.change_pet_status(pet_id,PET_STATUS_PREPARE);
                        rsp_msg.set_result_type(PET_PREPARE_SUCCESS);
                        is_set_prepare = true;
                        continue;
                    }else{
                        if (su->call_pet(pet_id,player_pet)){
                            player_pet.change_pet_status(pet_id,PET_STATUS_FIGHTING);
                            rsp_msg.set_result_type(PET_FIGHT_SUCCESS);
                            continue;
                        }
                    }
                }

            }

            std::map<uint32, client::pet_db_info>::const_iterator itr_3 = pet_info_map_.begin();
            for (;itr_3 != pet_info_map_.end(); ++itr_3){
                if (itr_3->first == pet_id){
                    continue;
                }
                if (player_pet.get_pet_status(itr_3->first,pet_status) != 0){
                    continue;
                }

                if (itr_3->first == old_fight_id && !is_set_prepare){
                    if (su->rest_pet(itr_3->first,false)){
                        player_pet.change_pet_status(itr_3->first,PET_STATUS_TRUCE);
                    }
                }else{
                    continue;
                }
            }
        }
        break;
    case PET_OPERATE_REST:// 休战
        {
            rsp_msg.set_result_type(PET_REST_FAILD);
            uint32 old_prepare_id = 0;
            std::map<uint32, client::pet_db_info>::const_iterator it_old = pet_info_map_.begin();
            for (;it_old != pet_info_map_.end(); ++it_old) {
                if (player_pet.get_pet_status(it_old->first,pet_status) != 0){
                    continue;
                }

                if (pet_status == PET_STATUS_PREPARE){
                    old_prepare_id = it_old->first;
                }
            }

            std::map<uint32, client::pet_db_info>::const_iterator itr_2 = pet_info_map_.begin();
            for (; itr_2 != pet_info_map_.end();++itr_2){
                if (itr_2->first == pet_id){
                    if (player_pet.get_pet_status(pet_id,pet_status) != 0){
                        continue;
                    }

                    if (pet_status == PET_STATUS_TRUCE){
                        break;
                    }

                    if (su->rest_pet(pet_id,false)){
                        player_pet.change_pet_status(pet_id,PET_STATUS_TRUCE);
                        rsp_msg.set_result_type(PET_REST_SUCCESS);
                    }
                }
                else{
                    if (player_pet.get_pet_status(itr_2->first,pet_status) != 0){
                        continue;
                    }

                    if (pet_status != PET_STATUS_PREPARE){
                        continue;
                    }

                    if (old_prepare_id == itr_2->first){
                        if (su->call_pet(itr_2->first,player_pet)){
                            player_pet.change_pet_status(itr_2->first,PET_STATUS_FIGHTING);
                            continue;
                        }
                    }

                }
            }
        }
        break;
    default:
        break;
    }
}
// 宠物模式改变
void* on_game_pb_pet_change_mode_req(client::pet_change_mode_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    std::map<uint64, scene_role*>& pets = psu->get_pets();
    scene_npc* psn = NULL;
    for (std::map<uint64, scene_role*>::iterator i = pets.begin(); i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }

        psn = static_cast<scene_npc*>(i->second);
        psn->change_mode(req->m());
    }

    client::pet_change_mode_rsp rsp;
    rsp.set_m(req->m());
    game_send_client(ss, req->svr_head().client_id(), rsp);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), (uint32)req->m());

    return NULL;
}
// 宠物信息
void* on_game_pb_pet_info_req(client::pet_info_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    player_role_pet& player_pet = su->get_player_role().get_pets();
    client::pet_db_info pet_info;
    if (!player_pet.get_pet_info(req->pet_id(),pet_info)){
        return NULL;
    }

    client::pet_info_rsp rsp_msg;
    client::role_data* role_data = 	rsp_msg.mutable_rd();
    role_data->set_role_id(req->svr_head().role_id());
    role_data->set_inst_id(su->get_inst_id());
    role_data->set_role_typ((client::role_data_TYPE)su->get_role_type());
    client::pet_db_info* info = rsp_msg.add_pet_info();
    info->CopyFrom(pet_info);
    su->send_client(rsp_msg);

    client::role_skill_msg rsm;
    su->fill_pets_skill_msg(rsm,pet_info.pet_id(),pet_info.pet_level());
    su->send_client(rsm);

    return NULL;
}
// 副本中美人战斗切换
void* on_game_pb_pet_fight_mode_change_req(client::pet_fight_mode_change_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    client::pet_fight_mode_change_rsp rsp_msg;
	if (req->has_flag_type()){
		rsp_msg.set_flag_type(req->flag_type());
	}

    rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);

    player_role_pet& player_pet = psu->get_player_role().get_pets();
    uint32 pet_status = 0;
    if (player_pet.get_pet_status(req->pet_id(), pet_status) != 0) {
        psu->send_client(rsp_msg);
        return NULL;
    }

	scene_inst* psi = psu->get_scene();
	if (psi == NULL){
		psu->send_client(rsp_msg);
		return NULL;
	}
	
	bool is_first = false;

	if (req->has_is_first() && req->is_first() == 1){
		is_first = true;
	}

	if (psi->get_scene_type() == MAP_TYPE_LEVELS){
		psu->levels_call_pet(req,player_pet,rsp_msg,is_first);

		psu->send_client(rsp_msg);
		return NULL;
	}

	if (psi->get_scene_type() != MAP_TYPE_PET && psi->get_scene_type() != MAP_TYPE_LEVELS){
		if (pet_status != PET_STATUS_FIGHTING && pet_status != PET_STATUS_PREPARE) {
			psu->send_client(rsp_msg);
			return NULL;
		}
	}

	if (psu->ectype_call_pet(req->pet_id(),  player_pet, rsp_msg,is_first)) {
		rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_SUCCEED);
	}

    psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->pet_id());

    return NULL;
}

void* on_game_pb_pet_levels_set_req(client::pet_levels_set_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::pet_levels_set_rsp rsp_msg;

	psu->pet_levels_set_req(req->pet_id(),req->levels_status(),rsp_msg);

	psu->send_client(rsp_msg);

	return NULL;
}
