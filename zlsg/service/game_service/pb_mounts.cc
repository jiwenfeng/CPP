#include "config_mounts.h"
#include "config_mounts_skill.h"
#include "client.mounts.pb.h"
#include "game_interface.h"
#include "player_role.h"
#include "player_role_mounts.h"
#include "player_role_props.h"
#include "player_role_skill_mgr.h"
#include "player_role_statis_mgr.h"
#include "config_props.h"
void fill_mounts_msg(const mysql::tb_role_mounts& mounts, client::mounts_msg& mmsg)
{
    client::mounts_info* pmi = mmsg.add_mi();
    pmi->set_mount_id(mounts.mount_id());
    pmi->set_advance_time(mounts.advance_time());
    pmi->set_advance_bless(mounts.advance_bless());
    pmi->set_skill_upgrade_num(mounts.skill_upgrade_num());
    pmi->set_mount_level(mounts.mount_level());
    pmi->set_mount_exp(mounts.mount_exp());
    pmi->set_mount_power(mounts.mount_power());
}
string get_attr_name(int type);
// 创建坐骑
void* on_game_pb_receive_mounts_req(client::receive_mounts_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    player_role_mounts& m = psu->get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
    if (pdb == NULL || pdb->mount_id() != 0) {
        return NULL;
    }

    const config_mounts* pcm = CONFMGR->get_config_mounts_min();
    if (pcm == NULL || pcm->get_base() == NULL) {
        return NULL;
    }

    // 检测获得第一个坐骑的副本是否通关
    pdb->set_role_id(psu->get_role_id());
    pdb->set_mount_id(pcm->get_base()->get_mount_id());
    pdb->set_advance_num(0);
    pdb->set_advance_time(0);
    pdb->set_advance_bless(0);
    pdb->set_skill_upgrade_num(10);
    pdb->set_mount_level(1);
    pdb->set_mount_exp(0);
    uint32 mount_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_mounts_mgr)->calculate_mount_power(pdb->mount_id(),pdb->mount_level())));
    pdb->set_mount_power(mount_power);
    client::receive_mounts_rsp rsp;
    rsp.set_ret(client::receive_mounts_rsp_result_success);
    psu->send_client(rsp);

    const mysql::tb_role_mounts& mounts = psu->get_player_role().get_mounts().get_db();
    if (mounts.mount_id() != 0) {
        client::mounts_msg mmsg;
        fill_mounts_msg(mounts, mmsg);
        psu->send_client(ss->ctx, mmsg);

        psu->calculate_attr();
        psu->send_client_change_attr();
    }

    return NULL;
}

// 坐骑骑乘
void* on_game_pb_ride_mounts_req(client::ride_mounts_req* req, void* ud, size_t* rsp_len)
{
    //req->index() 0下马 ，不然就发坐骑ID过来
    bool update = false;
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* psu = ss->sim->get_user(req->svr_head().role_id());
    if (psu == NULL) {
        update = true;
        psu = ss->sim->get_space_user(req->svr_head().role_id());
    }

    if (psu == NULL) {
        return NULL;
    }


    // 取出坐骑数据
    player_role_mounts& m = psu->get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
    if (pdb == NULL || pdb->mount_id() == 0) {
        return NULL;
    }

    // 处理一下，如果骑乘的ID大于数据库所存的ID则返回
    if (req->index() > pdb->mount_id()) {
        return NULL;
    }
	uint32 old_mount_id = psu->get_attr(MOUNTS_ID);
    psu->get_player_role().get_query_attr().set_mounts_id(req->index());
    psu->calculate_attr();
    psu->mutable_attr()->set_val(MOUNTS_ID, req->index());
    psu->send_client_change_attr();
	uint32 new_mount_id = psu->get_attr(MOUNTS_ID);
    if (update) {
        psu->send_client_attr();
    }

	// 骑乘对宠物速度属性进行同步更新
	uint32 pet_id = psu->get_player_role().get_pets().get_fighting_pet_id();
	if (pet_id != 0){
		psu->pet_attr_change(pet_id,PET_ATTR_CHANGE_TYPE_EQUIP);
	}

	// 记录第一次骑乘
	uint32 is_frist_ride = 0;
	psu->get_player_role().get_role_statis_mgr().get(PRSTE_MOUNT_FRIST_RIDE)->get_val(is_frist_ride);
	if (is_frist_ride == 0){
		psu->get_player_role().get_role_statis_mgr().get(PRSTE_MOUNT_FRIST_RIDE)->set_val(1);
	}

    client::ride_mounts_rsp rsp;
    rsp.set_ret(0);
    rsp.set_index(req->index());
    psu->send_client(rsp);
	std::map<int,int> old_attr_map_;
	old_attr_map_.clear();
	// 旧属性列表：
	if (old_mount_id != 0){
		do 
		{
			const config_mounts* p = CONFMGR->get_config_mounts(old_mount_id);
			if (p == NULL) {
				break;
			}

			const std::map<int, std::map<int, config_mounts_attr*> >& mount_a = p->get_attr();
			const std::map<int, std::map<int, config_mounts_attr*> >::const_iterator m_job = mount_a.find(0);
			if (m_job == mount_a.end()) {
				break;
			}

			const std::map<int, config_mounts_attr*>::const_iterator m_job_level = m_job->second.find(pdb->mount_level());
			if (m_job_level == m_job->second.end() || m_job_level->second == NULL) {
				break;
			}

			old_attr_map_ = m_job_level->second->get_game_attribute();
			break;
		} while (false);
	}
	// 新属性列表：
	std::map<int,int> new_attr_map_;
	new_attr_map_.clear();
	if (new_mount_id != 0){
		do 
		{
			const config_mounts* p = CONFMGR->get_config_mounts(new_mount_id);
			if (p == NULL) {
				break;
			}

			const std::map<int, std::map<int, config_mounts_attr*> >& new_m = p->get_attr();
			const std::map<int, std::map<int, config_mounts_attr*> >::const_iterator new_m_job = new_m.find(0);
			if (new_m_job == new_m.end()) {
				break;
			}

			const std::map<int, config_mounts_attr*>::const_iterator new_job_level = new_m_job->second.find(pdb->mount_level());
			if (new_job_level == new_m_job->second.end() || new_job_level->second == NULL) {
				break;
			}

			new_attr_map_ = new_job_level->second->get_game_attribute();
			break;
		} while (false);
	}
	uint32 is_up = 0;
	if (req->index() == 0){
		// 下坐骑
		is_up = 0;
	}else{
		// 上坐骑
		is_up = 1;
	}

	std::string a = "";
	if (old_attr_map_.size() == 0){
		// 上坐骑，增加 属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX;
		std::map<int ,int>::iterator it = new_attr_map_.begin();
		
		char tmp[1024] = {0};
		for (;it != new_attr_map_.end();++it){
			memset(tmp, 0, sizeof(tmp));
			// %u|%u|%u 属性类型|前值|后值
			string name = get_attr_name(it->first);
			snprintf(tmp, sizeof(tmp), "|增加属性:%s,前值:%u/后值:%u", name.c_str(),0,it->second);
			a += tmp;
		}
	}

	if (new_attr_map_.size() == 0){
		// 下坐骑，减少 属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX;
		std::map<int ,int>::iterator it_old = old_attr_map_.begin();
		char tmb[1024] = {0};
		for (;it_old != old_attr_map_.end();++it_old){
			memset(tmb, 0, sizeof(tmb));
			// %u|%u|%u 属性类型|前值|后值
			string name = get_attr_name(it_old->first);
			snprintf(tmb, sizeof(tmb), "|减少属性:%s,前值:%u/后值:%u", name.c_str(),it_old->second,0);
			a += tmb;
		}
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%u%s", psu->get_role_id(), req->GetTypeName().c_str(), is_up,a.c_str());

    return NULL;
}
void cal_expend(uint32 id,uint32 count,std::map<uint32,uint32>& total_expend);
// 坐骑进阶
void* on_game_pb_upgrade_mounts_req(client::upgrade_mounts_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
    // 取出坐骑数据
    player_role_mounts& m = psu->get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
    if (pdb == NULL || pdb->mount_id() == 0) {
        return NULL;
    }
    // 根据ID获取当前坐骑基础数据与属性加成数据模版表
    const config_mounts* pcm = CONFMGR->get_config_mounts(pdb->mount_id());
    if (pcm == NULL || pcm->get_base() == NULL) {
        return NULL;
    }
    // 获得下一阶的坐骑基础数据与属性加成数据模版表
    const config_mounts* next_mounts = CONFMGR->get_config_mounts(pcm->get_base()->get_next_mount_id());
    if (next_mounts == NULL) {
        return NULL;
    }
    // 判断当前坐骑进阶是否已经是最高阶的坐骑
    if (pdb->mount_id() == 12){
        return NULL;
    }
    // 判断进阶材料数量是否足够
    props_parser& pack_obj =  psu->get_player_role().get_role_props().get_pack();

    bool is_enough_item = false;
    uint32 need_count = static_cast<uint32>(next_mounts->get_base()->get_advance_need_item_num());
	const std::vector<int>& need_item_vec = next_mounts->get_base()->get_advance_need_item();
	uint32 cur_total_count = 0;
	// 消耗前的物品的值与数量
	std::map<uint32,uint32> old_item_map_;
	old_item_map_.clear();
	uint32 cur_pro_count = 0;
	for (uint32 i = 0; i < need_item_vec.size(); ++i){
		cur_pro_count = pack_obj.get_props_num(need_item_vec[i]);
		cur_total_count += cur_pro_count;
		if (cur_pro_count != 0){
			cal_expend(need_item_vec[i],cur_pro_count,old_item_map_);
		}
	}
	// 消耗记录：
	std::map<uint32 ,uint32> expend_map_;
	expend_map_.clear();

	if (cur_total_count < need_count){
		// 检测替代品是否足够
		const std::vector<int>& replace_item_vec = next_mounts->get_base()->get_upgrade_replace_item();
		uint32 replace_count = 0;
		for (uint8 i = 0; i < replace_item_vec.size(); ++i){
			replace_count = pack_obj.get_props_num(static_cast<uint32>(replace_item_vec[i]));
		    if (replace_count>=  need_count){
		        // 扣除道具
		        if (pack_obj.remove_props(static_cast<uint32>(replace_item_vec[i]),need_count) == 0){
		            is_enough_item = true;
					break;
		        }
		    }else{
				pack_obj.remove_props(static_cast<uint32>(replace_item_vec[i]),replace_count);
				need_count -= replace_count;
			}
		}
	}else{
		// 扣除道具
		is_enough_item = true;

		psu->sub_super_props(need_item_vec,need_count,expend_map_);

	}

    if (!is_enough_item){
        return NULL;
    }

	// 消耗记录参数
	std::map<uint32 ,uint32>::iterator it = expend_map_.begin();
	std::string a = "";
	char tmp[1024] = {0};
	for (;it != expend_map_.end();++it){
		memset(tmp, 0, sizeof(tmp));
		const config_props* item_info = CONFMGR->get_config_props(it->first);
		if (item_info == NULL){
			continue;
		}
		std::map<uint32,uint32>::iterator itr = old_item_map_.find(it->first);
		if (itr != old_item_map_.end()){
			// 消耗物品ID（物品名字）|数量XX|前值XX|后值XX
			snprintf(tmp, sizeof(tmp), "|物品:%s,数量%u,前值:%u/后值:%u", item_info->get_item_name().c_str(),it->second,itr->second,itr->second - it->second);
			a += tmp;
		}
	}

    psu->send_client_change_props_msg();
	// 先增加次数
	pdb->set_advance_num(pdb->advance_num() + 1);
    bool is_advance = false;
    uint32 advance_bless_max = static_cast<uint32>(next_mounts->get_base()->get_advance_bless_max());
    uint32 advance_count_min = static_cast<uint32>(next_mounts->get_base()->get_advance_count_min());
    uint32 advance_count_max = static_cast<uint32>(next_mounts->get_base()->get_advance_count_max());
    // 进阶成功几率计算
    if (pdb->advance_bless() >= advance_bless_max){
        is_advance = true;
    }
    else{
        if (pdb->advance_num() < advance_count_min){
            is_advance = false;
        }
        else if (pdb->advance_num() >= advance_count_max){
            is_advance = true;
        }
        else if(advance_count_min <= pdb->advance_num() && pdb->advance_num() < advance_count_max){
            int rand_value = static_cast<int>(mtrandom::rand_mid32(0,10000));
            if (next_mounts->get_base()->get_advance_ratio() >= rand_value){
                is_advance = true;
            }
        }
    }
	// 祝福值旧值
	uint32 old_bless = pdb->advance_bless();
	// 坐骑阶数旧值
	uint32 old_mount_id = pdb->mount_id();
	// 操作所增加的祝福值
	uint32 add_bless = 0;
	uint32 is_success = 0;
    if (is_advance){
		is_success = 1;
        // 进阶成功
        // 数据保存
        pdb->set_mount_id(pcm->get_base()->get_next_mount_id());
        pdb->set_advance_num(0);
        pdb->set_advance_time(0);
        pdb->set_advance_bless(0);
        pdb->set_mount_level(pdb->mount_level());
        pdb->set_mount_exp(pdb->mount_exp());
        uint32 mount_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_mounts_mgr)->calculate_mount_power(pdb->mount_id(),pdb->mount_level())));
        pdb->set_mount_power(mount_power);
        // 查询数据更新
        psu->update_mount_info(pdb->mount_level(),pdb->mount_power());
        // 更新到排行榜
        psu->update_mounts_rank(pdb->mount_id(), pdb->mount_level());
        // 统计坐骑进阶数据
		CHECK_TARGET_PROGRESS(psu, MOUNT_ADVANCE, pdb->mount_id());

        // 下发给客户端
        client::upgrade_mounts_rsp rsp;
        rsp.set_ret(0);// 0成功 1失败
        rsp.set_index(pdb->mount_id());
        client::mounts_info* pmi = rsp.add_mi();
        pmi->set_mount_id(pdb->mount_id());
        pmi->set_advance_time(pdb->advance_time());
        pmi->set_advance_bless(pdb->advance_bless());
        pmi->set_skill_upgrade_num(pdb->skill_upgrade_num());
        pmi->set_mount_level(pdb->mount_level());
        pmi->set_mount_exp(pdb->mount_exp());
        pmi->set_mount_power(pdb->mount_power());
        psu->send_client(rsp);

        // 天赋技能的开放
        const std::vector<int>& talent_skill_vec = next_mounts->get_base()->get_talent_skill();

        for (size_t i = 0; i < talent_skill_vec.size();++i){
			const config_skill* talent_skill_info = CONFMGR->get_config_skill(talent_skill_vec[i]);
			if (talent_skill_info == NULL){
				continue;
			}

            psu->get_player_role().get_role_skill_mgr().add_skill(pdb->role_id(),talent_skill_vec[i]);
            skill sk(*talent_skill_info);
            psu->mutable_skill()->add_skill(sk);
            
        }

        // 技能的开放
        const std::vector<int>& skill_id_vec = next_mounts->get_base()->get_skill_id();

        for (uint8 i = 0; i < skill_id_vec.size(); ++i){
            psu->get_player_role().get_role_skill_mgr().add_skill(pdb->role_id(),skill_id_vec[i]);
            const config_skill* skill_info = CONFMGR->get_config_skill(skill_id_vec[i]);
            if (skill_info != NULL){
                skill sk(*skill_info);
                psu->mutable_skill()->add_skill(sk);
            }
        }

        // 下发技能数据
        client::role_skill_msg rsm;
        psu->fill_mounts_skill_msg(rsm);
        psu->send_client(rsm);

        if (psu->get_attr().get_val(MOUNTS_ID) != 0){
            psu->mutable_attr()->set_val(MOUNTS_ID, pcm->get_base()->get_next_mount_id());
            psu->get_player_role().get_query_attr().set_mounts_id(pcm->get_base()->get_next_mount_id());
            psu->calculate_attr();
            psu->mutable_attr()->set_val(MOUNTS_ID, pcm->get_base()->get_next_mount_id());
            psu->send_client_change_attr();
        }

        psu->calculate_attr();
        psu->send_client_change_attr();
        // 全服公告 0不公告 1公告
        if (next_mounts->get_base()->get_success_broadcast() == 1){
			
			client::broadcast_msg_req req;
			req.set_area_id(42);
			std::vector<client::content_data> data_vec;
			data_vec.clear();
			client::content_data data_1;
			data_1.set_s(psu->get_role_name());
			data_vec.push_back(data_1);

			client::content_data data_2;
			data_2.set_n(pdb->mount_id());
			data_vec.push_back(data_2);

			client::content_data data_3;
			data_3.set_s(next_mounts->get_base()->get_mount_name());
			data_vec.push_back(data_3);

			for (uint32 i = 0; i < data_vec.size(); ++i){
				client::content_data* data_info = req.add_data();
				data_info->CopyFrom(data_vec[i]);
			}

			psu->send_center(ss->ctx, req);
        }
    }
    else{
        // 进阶失败
        // 若没有祝福值，则为第一次增加祝福值，插入时间
        if (pdb->advance_bless() == 0){
            pdb->set_advance_time((uint32)time(NULL));
        }
        uint32 max_bless = static_cast<uint32>(next_mounts->get_base()->get_advance_fail_belss_max());
        uint32 min_bless = static_cast<uint32>(next_mounts->get_base()->get_advance_fail_bless_min());
        uint32 rand_bless = mtrandom::rand_mid32(min_bless,max_bless);
        uint32 total_bless = pdb->advance_bless() + rand_bless;
        if (total_bless >= static_cast<uint32>(next_mounts->get_base()->get_advance_bless_max())){
            total_bless = static_cast<uint32>(next_mounts->get_base()->get_advance_bless_max());
        }
        //数据库保存：
		pdb->set_advance_bless(total_bless);
		add_bless = rand_bless;
        // 下发给客户端
        client::upgrade_mounts_rsp rsp;
        rsp.set_ret(1);// 0成功 1失败
        rsp.set_index(pdb->mount_id());
        client::mounts_info* pmi = rsp.add_mi();
        pmi->set_mount_id(pdb->mount_id());
        pmi->set_advance_time(pdb->advance_time());
        pmi->set_advance_bless(pdb->advance_bless());
        pmi->set_skill_upgrade_num(pdb->skill_upgrade_num());
        pmi->set_mount_level(pdb->mount_level());
        pmi->set_mount_exp(pdb->mount_exp());
        pmi->set_mount_power(pdb->mount_power());
        psu->send_client(rsp);
    }
	// 消耗物品ID（物品名字）|数量XX|前值XX|后值XX|获得祝福值XX，祝福值前值XX/后值XX；坐骑等阶X ，等阶前值XX/后值XX；1成功/ 0失败；
	amtsvr_billlog(ss->ctx, "%u|%s%s|%u|%u|%u|%u|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(),
	a.c_str(),add_bless,old_bless,pdb->advance_bless(),pdb->mount_id(),old_mount_id,pdb->mount_id(),is_success);

    return NULL;
}

void fill_skill_data(player_role_skill* prs, client::skill_data* sd);
uint32 double_exp(uint32 match_id,uint32 other_id,std::map<uint32,uint32>& improve_skill_map_);
uint32 triple_exp(uint32 match_id,std::map<uint32,uint32>& improve_skill_map_);
uint32 add_exp(uint32 match_id,std::map<uint32,uint32>& improve_skill_map_);
bool calculate_pay(uint32 upgrade_type,mysql::tb_role_mounts* pdb,scene_user* psu,uint32& rotate_count,uint32& pay_value,ROLE_ATTRIBUTE& pay_type);
void* dispose_mounts_skill_up(scene_user* psu,mysql::tb_role_mounts* pdb,const config_mounts* pcm,uint32 last_skill_id,uint32 last_skill_exp,bool& is_change_id);
bool calculate_mounts_skill_exp(uint32 upgrade_type,uint32 rotate_count,std::vector<uint32>& skill_vec,std::map<uint32,uint32>& improve_skill_map_);
bool fill_improve_data(scene_user* psu,const config_mounts* pcm,std::map<uint32,uint32>& improve_skill_map_);
// 坐骑技能升级
void* on_game_pb_upgrade_mounts_skill_req(client::upgrade_mounts_skill_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
    client::upgrade_mounts_skill_rsp result_msg;
    result_msg.set_upgrade_type(req->upgrade_type());
    // 取出坐骑数据 
    player_role_mounts& m = psu->get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
    if (pdb == NULL || pdb->mount_id() == 0) {
        return NULL;
    }
    // 若超过当天的升级次数或坐骑阶数不够则返回
    if (pdb->mount_id() < 5){
        return NULL;
    }
    switch(req->upgrade_type())
    {
    case 0:
    case 1:
    case 2:
        if (pdb->skill_upgrade_num() < 1){
            return NULL;
        }
        break;
    default:
        return NULL;
        break;
    }

    // 根据ID获取当前坐骑基础数据与属性加成数据模版表
    const config_mounts* pcm = CONFMGR->get_config_mounts(pdb->mount_id());
    if (pcm == NULL || pcm->get_base() == NULL) {
        return NULL;
    }

    std::map<uint32,uint32> improve_skill_map_;
    improve_skill_map_.clear();
    // 填充可升级的技能数据
    if (!fill_improve_data(psu,pcm,improve_skill_map_)){
        return NULL;
    }

    if (improve_skill_map_.empty()){
        return NULL;
    }
    // 转的次数
    uint32 rotate_count = 0;
    // 付款的类型
    ROLE_ATTRIBUTE pay_type = TAEL;
    // 总共要付多少
    uint32 pay_value = 0;


    // 钱不够
    if (!calculate_pay(req->upgrade_type(),pdb,psu,rotate_count,pay_value,pay_type)){
        return NULL;
    }

    std::vector<uint32> skill_vec;
    skill_vec.clear();
    if (!calculate_mounts_skill_exp(req->upgrade_type(),rotate_count,skill_vec,improve_skill_map_)){
        return NULL;
    }

    // 对map进行相关技能的升级与经验的处理
    std::map<uint32,uint32>::iterator itr_2 = improve_skill_map_.begin();
    bool is_change_id = false;
    for (;itr_2 != improve_skill_map_.end(); ++itr_2){
        // 最终升级后的ID
        uint32 last_skill_id = itr_2->first;
        uint32 last_skill_exp = itr_2->second;
        dispose_mounts_skill_up(psu,pdb,pcm,last_skill_id,last_skill_exp,is_change_id);
    }

    if (!is_change_id){
        result_msg.set_ret(upgrade_mounts_skill_rsp_result_success_add_exp);
    }
    else{
		psu->calculate_attr();
		psu->send_client_change_attr();
        result_msg.set_ret(upgrade_mounts_skill_rsp_result_success_level_up);
    }

    //扣钱
	uint32 old_value = 0;
	if (pay_type == INGOT){
		old_value = psu->get_attr(INGOT);
	}

    psu->sub_money(pay_type,pay_value);
	if (pay_type == INGOT){
		psu->sub_money_log(MC_MOUNTS_SPEND, MC_UPGRADE_MOUNTS_SKILL, old_value, psu->get_attr(INGOT));
	}

    psu->send_client_change_attr();
    // 设置次数
    uint32 last_num = pdb->skill_upgrade_num() - rotate_count;
    pdb->set_skill_upgrade_num(last_num);
    result_msg.set_upgrade_num(last_num);
    // 统计日常
	CHECK_TARGET_PROGRESS(psu, MOUNT_SKILL_UPGRADE);
    // 下发技能数据
    client::role_skill_msg rsm;
    psu->fill_mounts_skill_msg(rsm);
    psu->send_client(rsm);

    for (uint32 i = 0; i < skill_vec.size(); ++i){
        result_msg.add_skill_id(skill_vec[i]);
    }

    psu->send_client(result_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->upgrade_type());

    return NULL;
}

bool calculate_pay(uint32 upgrade_type,mysql::tb_role_mounts* pdb,scene_user* psu,uint32& rotate_count,uint32& pay_value,ROLE_ATTRIBUTE& pay_type)
{
    //player_role_props& prg = psu->get_player_role().get_role_props();
    // 获取主背包
    //props_parser& pack_obj = prg.get_pack();
    uint32 key = pdb->mount_id();
    if (pdb->mount_id() >= 8){
        key = 8;
    }
    //获取坐骑技能转盘费用数据
    const config_mounts_skill* skill_pay_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_money_pay(key);//key 是坐骑阶数
    if (skill_pay_info == NULL){
        return false;
    }
    // 第几次？
    int cost = 0;
    // 减少的次数
    uint32 reduce_count = 0;
    // 当前拥有的钱
    uint32 cur_money = 0;
    switch(upgrade_type)
    {
    case 0:{
        uint32 now = (uint32)time(NULL);
        uint32 day = amtsvr_svr_start_day(now);
		
        double base_value = day * (CONFMGR->get_basic_value(10015) / 10000.0f);
        if (base_value > 0.5){
            base_value = 0.5;
        }
        rotate_count = 1;
        cost = 10 - pdb->skill_upgrade_num() + rotate_count;
        cur_money = psu->mutable_attr()->get_val(pay_type);
        const std::map<int, int>& money_game_map_ = skill_pay_info->get_consume_money_game();
        std::map<int,int>::const_iterator it_money = money_game_map_.begin();
        for (;it_money != money_game_map_.end(); ++it_money){
            if (it_money->first == cost){
                pay_value = static_cast<uint32>(ceil(it_money->second * (1 - base_value)));
                break;
            }
        }
        if (pay_value <= cur_money){
            return true;
        }
           }
           break;
    case 1:{
        rotate_count = 1;
        pay_type = INGOT;
        cost = 10 - pdb->skill_upgrade_num() + rotate_count;
        const std::map<int, int>& silver_map_ = skill_pay_info->get_consume_money_cash();
        std::map<int,int>::const_iterator it_cash = silver_map_.begin();
        for (;it_cash != silver_map_.end(); ++it_cash){
            if (it_cash->first == cost){
                pay_value = it_cash->second;
                break;
            }
        }
        if (pay_value <= psu->mutable_attr()->get_val(pay_type)){
            return true;
        }
        //else{
        //    pay_type = SIEGE_REPUTATION;
        //    pay_value = 0;
        //    cur_money = psu->mutable_attr()->get_val(pay_type);
        //    const std::map<int, int>& money_repute_map_ = skill_pay_info->get_consume_repute();
        //    std::map<int,int>::const_iterator it_repute = money_repute_map_.begin();
        //    for (;it_repute != money_repute_map_.end(); ++it_repute){
        //        if (it_repute->first == cost){
        //            pay_value += it_repute->second;
        //            break;
        //        }
        //    }
        //    if (pay_value <= cur_money){
        //        return true;
        //    }
        //}
           }
           break;
    case 2:{//一键
        rotate_count = pdb->skill_upgrade_num();
        reduce_count = rotate_count;
        pay_type = INGOT;
        const std::map<int, int>& money_cash_map_ = skill_pay_info->get_consume_money_cash();
        std::map<int,int>::const_iterator it_cash = money_cash_map_.begin();
        for (uint32 i = 0; i < rotate_count; ++i)
        {
            cost = 10 - reduce_count + 1;
            for (;it_cash != money_cash_map_.end(); ++it_cash){
                if (it_cash->first == cost){
                    pay_value += it_cash->second;
                    reduce_count -= 1;
                    break;
                }
            }
        }

        if (pay_value <= psu->mutable_attr()->get_val(pay_type)){
            return true;
        }
        //else{
        //    pay_type = SIEGE_REPUTATION;
        //    pay_value = 0;
        //    cost = 0;
        //    reduce_count = rotate_count;
        //    cur_money = psu->mutable_attr()->get_val(pay_type);
        //    const std::map<int, int>& money_repute_map_ = skill_pay_info->get_consume_repute();
        //    std::map<int,int>::const_iterator it_repute = money_repute_map_.begin();
        //    for (uint32 j = 0; j < rotate_count; ++j){
        //        cost = 10 - reduce_count + 1;
        //        for (;it_repute != money_repute_map_.end(); ++it_repute){
        //            if (it_repute->first == cost){
        //                pay_value += it_repute->second;
        //                reduce_count -= 1;
        //            }
        //        }
        //    }
        //    if (pay_value <= cur_money){
        //        return true;
        //    }
        //}
           }
           break;
    default:
        return false;
        break;
    }
    return false;
}

uint32 double_exp(uint32 match_id,uint32 other_id,std::map<uint32,uint32>& improve_skill_map_)
{
    const config_mounts_skill* match_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(match_id);
    if (match_info == NULL){
        return 0;
    }
    std::map<uint32,uint32>::iterator it_exp = improve_skill_map_.find(match_id);
    if (it_exp != improve_skill_map_.end()){
        it_exp->second += match_info->get_addtion_exp_2();
    }

    const config_mounts_skill* other_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(other_id);
    if (other_info == NULL){
        return 0;
    }
    std::map<uint32,uint32>::iterator it_exp_1 = improve_skill_map_.find(other_id);
    if (it_exp_1 != improve_skill_map_.end()){
        it_exp_1->second += other_info->get_addtion_exp_1();
    }
    return 0;
}

uint32 triple_exp(uint32 match_id,std::map<uint32,uint32>& improve_skill_map_)
{
    const config_mounts_skill* match_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(match_id);
    if (match_info == NULL){
        return 0;
    }
    std::map<uint32,uint32>::iterator it_exp = improve_skill_map_.find(match_id);
    if (it_exp != improve_skill_map_.end()){
        it_exp->second += match_info->get_addtion_exp_3();
    }
    return 0;
}

uint32 add_exp(uint32 match_id,std::map<uint32,uint32>& improve_skill_map_)
{
    const config_mounts_skill* match_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(match_id);
    if (match_info == NULL){
        return 0;
    }
    std::map<uint32,uint32>::iterator it_exp = improve_skill_map_.find(match_id);
    if (it_exp != improve_skill_map_.end()){
        it_exp->second += match_info->get_addtion_exp_1();
    }
    return 0;
}

void* dispose_mounts_skill_up(scene_user* psu,mysql::tb_role_mounts* pdb,const config_mounts* pcm,uint32 last_skill_id,uint32 last_skill_exp,bool& is_change_id)
{
    uint32 cur_skill_exp = 0;
    cur_skill_exp = last_skill_exp;
    for (uint32 i = 0; i < 5; ++i){
        const config_skill* ts = CONFMGR->get_config_skill(last_skill_id);
        const config_mounts_skill* cms = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(last_skill_id);
        if (cms == NULL){
            return NULL;
        }
        if (ts == NULL || ts->get_upgrade_skill_id() == 0 || cms->get_skill_level() >= (uint32)pcm->get_base()->get_max_skill_level()){
            player_role_skill* over_prs = psu->get_player_role().get_role_skill_mgr().get_skill(last_skill_id);
            if (over_prs == NULL) {
                return NULL;
            }
            over_prs->get_db_role_skill()->set_skillful(cur_skill_exp);
            psu->mutable_skill()->set_mounts_skill_exp(last_skill_id,cur_skill_exp);
            break;
        }
        // 获取坐骑技能转盘模版数据
        const config_mounts_skill* mounts_skill_info_2 = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(last_skill_id);
        if (mounts_skill_info_2 == NULL){
            return NULL;
        }
        // 升级
        if (cur_skill_exp >= mounts_skill_info_2->get_upgrade_need_exp()){
            const config_skill* skill_info = CONFMGR->get_config_skill(last_skill_id);
            if (skill_info == NULL){
                return NULL;
            }
            const config_skill* next_skill_info = CONFMGR->get_config_skill(skill_info->get_upgrade_skill_id());
            if (next_skill_info != NULL){
                psu->get_player_role().get_role_skill_mgr().delete_skill(last_skill_id);
                psu->mutable_skill()->delete_skill(last_skill_id);
                last_skill_id = next_skill_info->get_skill_id();
                cur_skill_exp -= mounts_skill_info_2->get_upgrade_need_exp();
                psu->get_player_role().get_role_skill_mgr().add_skill(pdb->role_id(),last_skill_id);
                const config_skill* last_skill_info = CONFMGR->get_config_skill(last_skill_id);
                if (last_skill_info != NULL){
                    skill sk(*last_skill_info);
                    psu->mutable_skill()->add_skill(sk);
                    is_change_id = true;
                }
                // 坐骑技能升级相应的效果ID会改变
                psu->mutable_skill()->upgrade_mount_skill_effect(last_skill_id);
                psu->get_player_role().get_role_skill_mgr().upgrade_mount_skill_effect(last_skill_id);
            }
        }
        else{
            // 加经验
            player_role_skill* cur_prs = psu->get_player_role().get_role_skill_mgr().get_skill(last_skill_id);
            if (cur_prs == NULL) {
                return NULL;
            }
            cur_prs->get_db_role_skill()->set_skillful(cur_skill_exp);
            psu->mutable_skill()->set_mounts_skill_exp(last_skill_id,cur_skill_exp);
            break;
        }
    }

    return NULL;
}

bool fill_improve_data(scene_user* psu,const config_mounts* pcm,std::map<uint32,uint32>& improve_skill_map_)
{
    // 取出当前坐骑所激活的技能
    vector<player_role_skill>& skill_list = psu->get_player_role().get_role_skill_mgr().get_skills();
    if (skill_list.size() == 0){
        return false;
    }

    for (size_t i = 0; i < skill_list.size(); ++i){
        const config_skill* base_info = CONFMGR->get_config_skill(skill_list[i].get_skill_id());
        if (base_info == NULL){
            return false;
        }
        if (base_info->get_skills_belonging() != 2){
            continue;
        }
        player_role_skill* prs = psu->get_player_role().get_role_skill_mgr().get_skill(skill_list[i].get_skill_id());
        if (prs == NULL /*|| !prs->studied()*/) {
            return false;
        }
        if (prs->max_lvl()) {
            continue;
        }

        const config_mounts_skill* mount_skill_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(skill_list[i].get_skill_id());
        if (mount_skill_info == NULL){
            continue;
        }

        if (mount_skill_info->get_skill_level() >= (uint32)pcm->get_base()->get_max_skill_level()){
            continue;
        }

        improve_skill_map_.insert(std::pair<uint32,uint32>(skill_list[i].get_skill_id(),prs->get_db_role_skill()->skillful()));
    }

    return true;
}

bool calculate_mounts_skill_exp(uint32 upgrade_type,uint32 rotate_count,std::vector<uint32>& skill_vec,std::map<uint32,uint32>& improve_skill_map_)
{
    uint32 denominator_1 = 0;// 外圈分母
    uint32 denominator_2 = 0;// 中圈分母
    uint32 denominator_3 = 0;// 内圈分母

    std::map<uint32,uint32>::iterator itr = improve_skill_map_.begin();
    for (;itr != improve_skill_map_.end();++itr){
        // 获取坐骑技能转盘模版数据
        const config_mounts_skill* mounts_skill_info = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(itr->first);
        if (mounts_skill_info == NULL){
            return false;
        }

        denominator_1 += mounts_skill_info->get_select_probability_1();
        denominator_2 += mounts_skill_info->get_select_probability_2();
        denominator_3 += mounts_skill_info->get_select_probability_3();
    }

    switch(upgrade_type){
    case 0:{
        // 随机外圈几率
        uint32 rand_1 = mtrandom::rand_mid32(1,denominator_1);
        // 随机中圈几率
        uint32 rand_2 = mtrandom::rand_mid32(1,denominator_2);
        // 随机内圈几率
        uint32 rand_3 = mtrandom::rand_mid32(1,denominator_3);
        // 递增数
        uint32 attenuation_1 = 0;
        uint32 attenuation_2 = 0;
        uint32 attenuation_3 = 0;
        bool turn_over_1 = false;
        bool turn_over_2 = false;
        bool turn_over_3 = false;
        uint32 match_id_1 = 0;
        uint32 match_id_2 = 1;
        uint32 match_id_3 = 2;
        std::map<uint32,uint32>::iterator it_1 = improve_skill_map_.begin();
        for (;it_1 != improve_skill_map_.end(); ++it_1){
            // 获取坐骑技能转盘模版数据
            const config_mounts_skill* mounts_skill_info_1 = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(it_1->first);
            if (mounts_skill_info_1 == NULL){
                return false;
            }
            if (!turn_over_1){// 外圈
                attenuation_1 += mounts_skill_info_1->get_select_probability_1();
                if (rand_1 <= attenuation_1){
                    match_id_1 = it_1->first;
                    turn_over_1 = true;
                }
            }
            if (!turn_over_2){// 中圈
                attenuation_2 += mounts_skill_info_1->get_select_probability_2();
                if (rand_2 <= attenuation_2){
                    match_id_2 = it_1->first;
                    turn_over_2 = true;
                }
            }
            if (!turn_over_3){// 内圈
                attenuation_3 += mounts_skill_info_1->get_select_probability_3();
                if (rand_3 <= attenuation_3){
                    match_id_3 = it_1->first;
                    turn_over_3 = true;
                }
            }
        }
        if (match_id_3 == match_id_2 && match_id_2 == match_id_1){
            triple_exp(match_id_2,improve_skill_map_);
        }
        else if (match_id_3 == match_id_2){
            double_exp(match_id_3,match_id_1,improve_skill_map_);
        }
        else if (match_id_3 == match_id_1){
            double_exp(match_id_3,match_id_2,improve_skill_map_);		
        }
        else if (match_id_2 == match_id_1){
            double_exp(match_id_2,match_id_3,improve_skill_map_);
        }
        else{
            add_exp(match_id_1,improve_skill_map_);
            add_exp(match_id_2,improve_skill_map_);
            add_exp(match_id_3,improve_skill_map_);
        }
        skill_vec.push_back(match_id_1);
        skill_vec.push_back(match_id_2);
        skill_vec.push_back(match_id_3);
           }
           break;
    case 1:
    case 2:
        for (uint32 i = 0; i < rotate_count; ++i){
            uint32 rand_1 = mtrandom::rand_mid32(1,denominator_1);
            uint32 attenuation_1 = 0;
            bool turn_over_1 = false;
            std::map<uint32,uint32>::iterator it_luck = improve_skill_map_.begin();
            for (;it_luck != improve_skill_map_.end(); ++it_luck){
                // 获取坐骑技能转盘模版数据
                const config_mounts_skill* mounts_skill_luck = CONFMGR->get_config_mounts_skill_mgr().get_config_mounts_skill(it_luck->first);
                if (mounts_skill_luck == NULL) {
                    return false;
                }

                if (!turn_over_1){// 外圈
                    attenuation_1 += mounts_skill_luck->get_select_probability_1();
                    if (rand_1 <= attenuation_1){
                        it_luck->second += (mounts_skill_luck->get_addtion_exp_3());
                        skill_vec.push_back(it_luck->first);
                        turn_over_1 = true;
                    }
                }
            }
        }
        break;
    default:
        return false;
        break;
    }
    return true;

}

void* on_game_pb_mounts_bless_time_req(client::mounts_bless_time_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::mounts_bless_time_rsp rsp_msg;
	player_role_mounts& m = psu->get_player_role().get_mounts();
	mysql::tb_role_mounts* pdb = m.mutable_db();
	if (pdb != NULL && pdb->mount_id() != 0) {
		if (pdb->advance_bless() != 0){
			const config_mounts* mount_info = CONFMGR->get_config_mounts(pdb->mount_id());
			if (mount_info != NULL){
				uint32 clear_time = mount_info->get_base()->get_advance_bless_time();
				if (clear_time != 0){
					uint32 valid_time = pdb->advance_time() + clear_time;
					uint32 system_time = (uint32)time(NULL);
					if (valid_time >= system_time){
						uint32 remain_time = valid_time - system_time;
						rsp_msg.set_remain_time(remain_time);
						psu->send_client(rsp_msg);
					}
				  }
				}
			}
		}

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->unixtime());

	return NULL;
}
