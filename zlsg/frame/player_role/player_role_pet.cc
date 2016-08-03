#include "config_beauty.h"
#include "player_role_pet.h"
#include "config_skill.h"

player_role_pet::player_role_pet(mysql::tb_role_pet* b) : db_role_pet(b)
{

}

player_role_pet::~player_role_pet()
{
	if (db_role_pet != NULL) {
		delete db_role_pet;
		db_role_pet = NULL;
	}
}

int player_role_pet::set_db_pet(mysql::tb_role_pet* b)
{
	if (b == NULL) {
		return -1;
	}

	if (db_role_pet == b) {
		return 0;
	}

	if (db_role_pet != NULL) {
		delete db_role_pet;
	}

	db_role_pet = b;
	parse_pet(db_role_pet->pets());

	return 0;
}

int player_role_pet::save_pet(uint32 role_id, amtsvr_context* ctx)
{
	string out;
	int count = serialize_pet(out);
	if (count <= 0) {
		if (db_role_pet == NULL) {
			return 0;
		}
		writer_type = WOTE_REMOVE;
	} else {
		if (db_role_pet == NULL) {
			db_role_pet = new mysql::tb_role_pet;
			db_role_pet->set_role_id(role_id);
			writer_type = WOTE_INSERT;
		} else {
			writer_type = WOTE_UPDATE;
		}
		db_role_pet->set_pets(out);
	}

	mysql::tb_role_pet* trb = new mysql::tb_role_pet(*db_role_pet);
	CACHE_KEY k(trb->role_id(), 0);
	write(ctx, k, trb);

	return 0;

}

int player_role_pet::attach_role_pet(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	if (pb_count <= 0){
		return 0;
	}

	for (int i = 0; i < pb_count; ++i){
		if (i == 0){
			set_db_pet(static_cast<mysql::tb_role_pet*>(pb_ctx[i].pb_msg));
		} else {
			delete pb_ctx[i].pb_msg;
		}
	}

	return 0;
}

int player_role_pet::upgrate_pet_level(uint32 player_level, uint32 add_exp, uint32 pet_id, std::vector<uint32>& act_skill_id)
{
	client::pet_db_info pet_info;
	if (!get_pet_info(pet_id,pet_info)) {
		return -1;
	}

	pet_info.set_pet_exp(pet_info.pet_exp() + add_exp);

	while (true) {
		const config_beauty* pcu = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_info.pet_id(),pet_info.pet_level());
		const config_beauty* next = CONFMGR->get_config_beauty_mgr().get_config_beauty(pet_info.pet_id(),pet_info.pet_level()+1);
		if (pcu == NULL || next == NULL || pcu->get_up_need_exp() == 0 || pcu->get_up_need_exp() > pet_info.pet_exp() || player_level <= pet_info.pet_level()) {
			change_pet_info(pet_info);
			return 0;
		}
		else{
			pet_info.set_pet_exp(pet_info.pet_exp() -  pcu->get_up_need_exp());
			pet_info.set_pet_level(pet_info.pet_level() + 1);
			change_pet_info(pet_info);
			// 检测技能触发开放
			const std::vector<int>& skill_id_vec = next->get_general_skill();
			for (uint32 i = 0; i< skill_id_vec.size(); ++i){
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
					act_skill_id.push_back(skill_id_vec[i]);
				}
			}
		}
	}

	return 0;
}