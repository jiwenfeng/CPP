#include "config_skill.h"
#include "config_effect.h"
#include "player_role_skill_mgr.h"

player_role_skill_mgr::player_role_skill_mgr()
{
	_role_id    = 0;
	smy_id      = 0;
}

player_role_skill_mgr::~player_role_skill_mgr()
{
    uint32 size = role_skill_list.size();
    for (uint32 i = 0; i < size; ++i) {
        role_skill_list[i].release();
    }

    role_skill_list.clear();
}

int player_role_skill_mgr::attach_role_skill(uint32 role_id,int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
	init_skill_sid(role_id);
    role_skill_list.resize(pb_count);
	//uint32 true_id = 0;
	//uint32 old_id  = 0;
    for (int i = 0; i < pb_count; ++i) {
        player_role_skill& prs = role_skill_list[i];
		//bool is_change = false;
      /*  old_id = */prs.attach(static_cast<mysql::tb_role_skill*>(pb_ctx[i].pb_msg));
		set_skill_smy_id(*static_cast<mysql::tb_role_skill*>(pb_ctx[i].pb_msg));
		//if (is_change){
		//	prs.set_smy_id(true_id);
		//}
		//prs.set_old_id(old_id);
    }

    return 0;
}

int player_role_skill_mgr::init_skill_sid(uint32 role_id)
{
	_role_id = role_id;
	smy_id   = 0;

	return 0;
}

bool player_role_skill_mgr::generate_skill_smy_id(uint32 role_id,uint32& fid)
{
	if (_role_id == 0){
		_role_id = role_id;
	}
	fid = get();

	if (fid == 0){
		return false;
	}

	return true;
}

int player_role_skill_mgr::set_skill_smy_id(const mysql::tb_role_skill& db_skil)
{
	uint32 sk_smy_id = db_skil.smy_id();

	if (db_skil.role_id() != _role_id){
		return -1;
	}

	uint32 max_sid = get_skill_smy_id();

	if (sk_smy_id > max_sid){
		set(db_skil.role_id(),sk_smy_id);
	}

	return 0;
}

int player_role_skill_mgr::init_new_role_skill(uint32 role_id, uint32 job_id)
{
    std::vector<const config_skill*> vcs;
    config_skill_mgr* pcsm = MUTABLE_CONFIG_MGR(config_skill_mgr);
    if (pcsm == NULL) {
        return -1;
    }

    pcsm->get_config_skill_by_job(job_id, 1, vcs);
    for (size_t i = 0; i < vcs.size(); ++i) {
        if (vcs[i] == NULL) {
            continue;
        }

        std::vector<player_role_skill>::iterator it = std::find(role_skill_list.begin(), role_skill_list.end(), vcs[i]->get_skill_id());
        if (it != role_skill_list.end()) {
            continue;
        }

        player_role_skill prs;
		create_skill_info(prs,role_id,vcs[i]);
    }

    return 0;
}

player_role_skill* player_role_skill_mgr::get_skill(uint32 skill_id,uint32 ser_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
	if (skill_base == NULL){
		return NULL;
	}

	if (!is_bow_skill(skill_base->get_skills_belonging()) && !is_gem_skill(skill_base->get_skills_belonging())){
		for (uint32 i = 0; i < role_skill_list.size(); ++i) {
			if (role_skill_list[i].get_db_role_skill()->skill_id() == skill_id) {
				return &role_skill_list[i];
			}
		}
	}else{
		for (uint32 i = 0; i < role_skill_list.size(); ++i) {
			if (role_skill_list[i].get_db_role_skill()->skill_id() == skill_id && role_skill_list[i].get_db_role_skill()->skill_grid_index() == ser_index) {
				return &role_skill_list[i];
			}
		}
	}

    return NULL;
}

void player_role_skill_mgr::write(amtsvr_context* ctx)
{
//	for(size_t i = 0; i < del.size(); ++i)
//	{
//		del[i].delete_record(ctx);
//	}
//	for(size_t i = 0; i < role_skill_list.size(); ++i)
//	{
//		if(role_skill_list[i].need_delete())
//		{
//			role_skill_list[i].delete_old_record(ctx);
//		}
//		role_skill_list[i].write_skill(ctx);
//	}
//#if 0
//	std::vector<player_role_skill> rewrite_vec;
//	rewrite_vec.clear();
//    for (size_t i = 0; i < role_skill_list.size(); ++i) {
//        if (!role_skill_list[i].write_skill(ctx)){
//			rewrite_vec.push_back(role_skill_list[i]);
//        }
//    }
//
//    if (!del.empty()) {
//        for (size_t i = 0; i < del.size(); ++i) {
//            del[i].delete_record(ctx);
//        }
//
//        del.clear();
//    }
//
//	if (rewrite_vec.size() != 0){
//		for (size_t j = 0; j < rewrite_vec.size(); ++j){
//			rewrite_vec[j].rewrite_skill(ctx);
//		}
//
//		rewrite_vec.clear();
//	}
//#endif

	for (size_t i = 0; i < role_skill_list.size(); ++i) {
		role_skill_list[i].write_skill(ctx);
	}

	if (!del.empty()) {
		for (size_t i = 0; i < del.size(); ++i) {
			del[i].delete_record(ctx);
		}

		del.clear();
	}
}

void player_role_skill_mgr::fill_role_skill(client::role_skill_msg& rsm) const
{
    uint32 size = role_skill_list.size();
    for (uint32 i = 0; i < size; ++i) {
        role_skill_list[i].fill_role_skill_data(*rsm.add_sk());
    }
}

void player_role_skill_mgr::fill_role_skill(client::skills* sm) const
{
    uint32 size = role_skill_list.size();
    for (uint32 i = 0; i < size; ++i) {
        role_skill_list[i].fill_role_skill_data(*sm->add_sk());
    }
}

bool player_role_skill_mgr::add_skill(uint32 role_id, uint32 skill_id,uint32 ser_index)
{
    const config_skill* pcs = CONFMGR->get_config_skill_mgr().get_config_skill(skill_id);
	if (!add_skill(role_id, pcs,ser_index)) {
		return false;
	}
  
	return true;
}

bool player_role_skill_mgr::add_skill(uint32 role_id, const config_skill* pcs,uint32 ser_index)
{
    if (pcs == NULL) {
        return false;
    }

	if (!is_bow_skill(pcs->get_skills_belonging()) && ! is_gem_skill(pcs->get_skills_belonging())){
		std::vector<player_role_skill>::iterator it = std::find(role_skill_list.begin(), role_skill_list.end(), pcs->get_skill_id());
		if (it != role_skill_list.end()) {
			return false;
		}
		player_role_skill prs;
		create_skill_info(prs,role_id,pcs);
	}else{
		bool is_have = false;
		for (size_t i = 0; i < role_skill_list.size(); ++i){
			if (role_skill_list[i].get_skill_ser_index()!= 0 && role_skill_list[i].get_skill_ser_index() == ser_index
				&& role_skill_list[i].get_skill_id() == pcs->get_skill_id()){
				is_have = true;
			}
		}

		if (!is_have){
			player_role_skill prs;
			create_skill_info(prs,role_id,pcs,ser_index);
		}

	}


	return true;
}
uint32 player_role_skill_mgr::get_skill_effect_level(uint32 skill_id,uint32 ser_index)
{
	const config_skill* pcs = CONFMGR->get_config_skill(skill_id);
	if (pcs != NULL){
		if (!is_bow_skill(pcs->get_skills_belonging()) && ! is_gem_skill(pcs->get_skills_belonging())){
			std::vector<player_role_skill>::iterator it = std::find(role_skill_list.begin(), role_skill_list.end(), pcs->get_skill_id());
			if (it != role_skill_list.end()) {
				return it->get_db_role_skill()->skill_level();
			}else{
				return 0;
			}
		}else{
			for (size_t i = 0; i < role_skill_list.size(); ++i){
				if (role_skill_list[i].get_skill_ser_index()!= 0 && role_skill_list[i].get_skill_ser_index() == ser_index
					&& role_skill_list[i].get_skill_id() == pcs->get_skill_id()){
						return role_skill_list[i].get_db_role_skill()->skill_level();
				}
			}

		}
	}

	return 0;
}

uint32 player_role_skill_mgr::create_skill_info(player_role_skill& prs,uint32 role_id,const config_skill* skill_base,uint32 ser_index)
{
	uint32 smy_id = 0;
	generate_skill_smy_id(role_id,smy_id);
	prs.create(role_id,smy_id, skill_base,ser_index);
	//prs.set_old_id(smy_id);
	role_skill_list.push_back(prs);

	return 0;
}

bool player_role_skill_mgr::add_skill(uint32 role_id, const client::skill_data& skill)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill.id());
	if (skill_base == NULL){
		return false;
	}

	if (!is_bow_skill(skill_base->get_skills_belonging()) && ! is_gem_skill(skill_base->get_skills_belonging())){
		std::vector<player_role_skill>::iterator it = std::find(role_skill_list.begin(), role_skill_list.end(), skill.id());
		if (it != role_skill_list.end()) {
			it->set_skill(skill);
			return true;
		}

		player_role_skill prs;
		create_skill_info(prs,role_id,skill_base);
        it = std::find(role_skill_list.begin(), role_skill_list.end(), skill.id());
        if (it != role_skill_list.end()) {
            it->set_skill(skill);
        }
	}else{
		bool is_have = false;
		for (size_t i = 0; i < role_skill_list.size(); ++i){
			if (role_skill_list[i].get_skill_ser_index()!= 0 && role_skill_list[i].get_skill_ser_index() == skill.serial()
				&& role_skill_list[i].get_skill_id() == skill_base->get_skill_id()){
					is_have = true;
			}
		}

		if (!is_have){
			player_role_skill prs;
			create_skill_info(prs,role_id,skill_base,skill.serial());
		}
	}


    return true;
}

bool player_role_skill_mgr::delete_skill(uint32 skill_id,uint32 ser_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
	if (skill_base == NULL){
		return false;
	}
	if (!is_bow_skill(skill_base->get_skills_belonging()) && ! is_gem_skill(skill_base->get_skills_belonging())){
		std::vector<player_role_skill>::iterator i = std::find(role_skill_list.begin(), role_skill_list.end(), skill_id);
		if (i != role_skill_list.end()) {
			del.push_back(*i);
			role_skill_list.erase(i);
			return true;
		}
	}else{
		for (vector<player_role_skill>::iterator itr = role_skill_list.begin(); itr != role_skill_list.end();){
			if (itr->get_skill_ser_index() != 0 && itr->get_skill_id() == skill_id && itr->get_skill_ser_index() == ser_index){
				del.push_back(*itr);
				role_skill_list.erase(itr++);
				return true;
			}else{
				itr++;
			}
		}
	}


	return false;
}

bool player_role_skill_mgr::check_old_skill()
{
	for (size_t i = 0; i < role_skill_list.size(); ++i){
		const config_skill* skill_base = CONFMGR->get_config_skill(role_skill_list[i].get_skill_id());
		if (skill_base == NULL){
			continue;
		}

		if (! is_bow_skill(skill_base->get_skills_belonging()) && ! is_gem_skill(skill_base->get_skills_belonging())){
			role_skill_list[i].set_skill_ser_index(0);
		}

	}

	return true;
}

bool player_role_skill_mgr::upgrade_skill_effect(uint32 skill_id,uint32 add_level)
{
	std::vector<player_role_skill>::iterator itr = std::find(role_skill_list.begin(), role_skill_list.end(), skill_id);
	if (itr == role_skill_list.end()) {
		return false;
	}

	const config_skill* skill_base_info = CONFMGR->get_config_skill_mgr().get_config_skill(skill_id);
	if (skill_base_info == NULL) {
		return false;
	}

	uint32 new_skill_level = itr->get_db_role_skill()->skill_level() + add_level;

	const config_effect* effect_info = CONFMGR->get_config_effect_mgr().get_config_effect_by_key(skill_base_info->get_key(),new_skill_level);
	if (effect_info == NULL) {
		return false;
	}
	
	// 设置效果ID和效果等级
	itr->set_skill_effect_data(effect_info->get_effect_id(),new_skill_level);

	return true;
}

bool player_role_skill_mgr::set_skill_effect(uint32 skill_id,uint32 effect_id,uint32 level,uint32 ser_index)
{
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);

	if (skill_base == NULL){
		return false;
	}

	if (!is_bow_skill(skill_base->get_skills_belonging()) && ! is_gem_skill(skill_base->get_skills_belonging())){
		std::vector<player_role_skill>::iterator itr = std::find(role_skill_list.begin(), role_skill_list.end(), skill_id);
		if (itr == role_skill_list.end()) {
			return false;
		}

		itr->set_skill_effect_data(effect_id,level);
	}else{
		for (size_t i = 0; i < role_skill_list.size(); ++i){
			if (role_skill_list[i].get_skill_ser_index()!= 0 && role_skill_list[i].get_skill_ser_index() == ser_index
				&& role_skill_list[i].get_skill_id() == skill_id){
					role_skill_list[i].set_skill_effect_data(effect_id,level);
			}
		}
	}


	return true;
}

bool player_role_skill_mgr::upgrade_mount_skill_effect(uint32 skill_id)
{
	std::vector<player_role_skill>::iterator itr = std::find(role_skill_list.begin(), role_skill_list.end(), skill_id);
	if (itr == role_skill_list.end()) {
		return false;
	}

	const config_skill* skill_base_info = CONFMGR->get_config_skill_mgr().get_config_skill(skill_id);
	if (skill_base_info == NULL) {
		return false;
	}

	const std::vector<int>&   skill_effect_vec = skill_base_info->get_method_list();
	if (skill_effect_vec.size() == 0){
		return false;
	}

	// 设置效果ID和效果等级
	itr->set_skill_effect_data(skill_effect_vec[0],1);

	return true;
}

void player_role_skill_mgr::set_default_skill(uint32 skill_id)
{
    for (size_t i = 0; i < role_skill_list.size(); ++i) {
        if (role_skill_list[i].get_default_skill() == player_role_skill::DEFAULT_SKILL) {
            role_skill_list[i].set_default_skill(player_role_skill::NOT_DEFAULT_SKILL);
        }

        if (role_skill_list[i].get_skill_id() == skill_id) {
            role_skill_list[i].set_default_skill(player_role_skill::DEFAULT_SKILL);
        }
    }
}

int player_role_skill_mgr::check_skill(uint32 role_id, uint32 profession, uint32 level)
{
    return 0;
    config_skill_mgr* pcsm = MUTABLE_CONFIG_MGR(config_skill_mgr);
    if (pcsm == NULL) {
        return -1;
    }

    std::vector<const config_skill*> vcs;
    for (int32 i = (int32)level; i > 0; --i) {
        pcsm->get_config_skill_by_job(profession, i, vcs);
    }

    int ret = 0;
    for (size_t i = 0; i < vcs.size(); ++i) {
        if (vcs[i] == NULL) {
            continue;
        }

        amtsvr_log("can skill %u\n", vcs[i]->get_skill_id());
        bool had = false;
        for (size_t j = 0; j < role_skill_list.size(); ++j) {
			if (!is_bow_skill(vcs[i]->get_skills_belonging()) && ! is_gem_skill(vcs[i]->get_skills_belonging())){
				if (role_skill_list[j].get_skill_id() == vcs[i]->get_skill_id() ||
					role_skill_list[j].get_db_role_skill()->skill_grid_index() == vcs[i]->get_skill_index()) {
						had = true;
						break;
				}
			}else{
				// 弓箭，宝石技能另外处理


			}
        }

        if (had) {
            continue;
        }

        amtsvr_log("had skill %u\n", vcs[i]->get_skill_id());
//         std::vector<player_role_skill>::iterator it = std::find(role_skill_list.begin(), role_skill_list.end(), vcs[i]->get_skill_id());
//         if (it != role_skill_list.end()) {
//             continue;
//         }

       player_role_skill prs;
        //printf("%s:%d, skill %d\n", __FILE__, __LINE__, vcs[i]->get_skill_id());
		//prs.create(role_id, vcs[i]);
		//role_skill_list.push_back(prs);
		create_skill_info(prs,role_id,vcs[i]);
        ++ret;
    }

    return ret;
}

uint32 player_role_skill_mgr::get_bows_skill_count()
{
	uint32 bow_skill_count = 0;

	for (size_t i = 0; i < role_skill_list.size(); ++i){
		const config_skill* skill_base = CONFMGR->get_config_skill(role_skill_list[i].get_skill_id());
		if (skill_base == NULL){
			continue;
		}
		
		if (is_bow_skill(skill_base->get_skills_belonging())){
			++bow_skill_count;
		}

	}

	return bow_skill_count;
}
