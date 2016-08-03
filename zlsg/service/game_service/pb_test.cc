#include "client.mounts.pb.h"
#include "client.role_login.pb.h"
#include "mysql.t_recharge.pb.h"
#include "config_mgr.h"
#include "config_props.h"
#include "config_npc.h"
#include "config_birth.h"
#include "player_role.h"
#include "player_role_ext_attr.h"
#include "player_role_props.h"
#include "player_role_skill.h"
#include "player_role_skill_mgr.h"
#include "player_role_statis.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "player_role_mounts.h"
#include "game_interface.h"
#include "config_beauty.h"
#include "config_raid.h"
#include "md5_encrypt.h"
#include "player_role_raid.h"
#include "player_role_raid_mgr.h"
#ifdef WIN32
#pragma warning(disable : 4503 4786)
#endif // WIN32

void* on_pb_role_statis_req(client::role_statis_req* req, void* ud, size_t* rsp_len);
void fill_mounts_msg(const mysql::tb_role_mounts& mounts, client::mounts_msg& mmsg);
void* on_pb_submit_task_req(client::submit_task_req* req, void* ud, size_t* rsp_len);
void accept_task(std::map<uint32, client::task_accepted>* pat, uint32 next_task_id, scene_user* psu);
void* on_pb_reported_raid_req(client::reported_raid_req* req, void* ud, size_t* rsp_len);

void inner_test(client::test* t, void* ud)
{
    client::role_statis_req req;
    req.mutable_svr_head()->CopyFrom(t->svr_head());
    req.add_type(4);
    on_pb_role_statis_req(&req, ud, NULL);
}

#ifdef __TEST_LUA__
#include "link_lua.h"
#endif // __TEST_LUA__

void do_lua(client::test* req, void* ud, size_t* rsp_len)
{
#ifdef __TEST_LUA__
        link_lua lua;
        lua.do_test((game_service_t*)ud, req->svr_head().role_id(), req->svr_head().client_id(), req->lua());
#endif // __TEST_LUA__
}

void reload_lua(game_service_t* pgs)
{
#ifdef __TEST_LUA__
    if (pgs->lua != NULL) {
        delete pgs->lua;
        pgs->lua = NULL;
    }

    pgs->lua = new link_lua;
    pgs->lua->init_game_pb();
#endif // __TEST_LUA__
}

void gen_key(client::reported_raid_info &rri)
{
	char str_buf[128]= {0};
	snprintf(str_buf, sizeof(str_buf), "%d%d%d%d%d%d%s%d%d%d", rri.id(), rri.star(), rri.used_tick(), rri.complete_time(), rri.type(), rri.time(), "chebazi", rri.verify().max_hp(), rri.verify().max_atk(), rri.verify().max_def());
	unsigned char digest[17] = {0};
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, (unsigned char*)str_buf, strlen(str_buf));
	MD5Final(&md5, digest);
	char ret[64] = {0};
	for (int i = 0, j = 0; i < 32; i += 2) {
		sprintf(ret + i, "%02x", digest[j++]);
	}
	rri.set_key(ret);
}

void on_raid_complete(game_service_t *ss, client::test req, scene_user *psu)
{
	const config_raid *cr = CONFMGR->get_config_raid(req.raid_id());
	if(NULL == cr || cr->get_map_type() != MAP_TYPE_RAID)
	{
		return ;
	}

	if(req.once())
	{
        player_role_raid* prr = psu->get_player_role().get_raid_mgr().mutable_raid(req.raid_id());
		if(prr != NULL && (req.raid_type() == client::enter_raid_req_type_normal || prr->get_elite_star_num() != 0))
		{
			return ;
		}
	}
	client::test t;
	t.CopyFrom(req);
	t.set_raid_id(cr->get_ex_map_id());
	on_raid_complete(ss, t, psu);

	if(check_enter_raid(req.raid_id(), req.raid_type(), psu, true))
	{
		return ;
	}

	psu->enter_mini_server(req.raid_id(), (uint32)time(NULL) - 1000);
	ss->sim->go_to_space(psu);
	client::reported_raid_req rrr;
	rrr.mutable_svr_head()->CopyFrom(req.svr_head());
	client::reported_raid_info *rri = rrr.add_rri();
	rri->set_id(req.raid_id());
	rri->set_star(1);
	rri->set_used_tick(6000);
	rri->set_complete_time((uint32)time(NULL));
	rri->set_type(req.raid_type());
	rri->set_time((uint32)time(NULL));
	client::raid_verify *rv = rri->mutable_verify();
	rv->set_max_hp(psu->get_attr().get_val(MAX_HP));
	rv->set_max_atk(psu->get_attr().get_val(ATTACK_MAX));
	rv->set_max_def(psu->get_attr().get_val(DEFENSE_MAX));
	rv->set_bow_index(0);
	gen_key(*rri);
	on_pb_reported_raid_req(&rrr, ss, NULL);
	ss->sim->go_to_scene(psu);
	psu->set_scene_ready(true);
	if (psu->get_scene() != NULL) {
		psu->get_scene()->screen_push_user(psu);
	}
}

void test_recharge(client::test* req, game_service_t* service, scene_user* psu)
{
    if (!req->has_money()) {
        return ;
    }

	psu->recharge(req->money());
   // psu->add_money(INGOT, req->money());
    psu->send_client_change_attr();

    char buf[32] = { 0 };
    snprintf(buf, 32, "%d", (uint32)time(NULL));

    mysql::t_recharge log;
    log.set_id(0);
    log.set_sid("");
    log.set_did("");
    log.set_accountname("");
    log.set_playername(psu->get_role_name());
    log.set_playerlevel(psu->get_level());
    log.set_transactionid(buf);
    log.set_currency("RMB");
    log.set_inpourtype("三国宝");
    log.set_addcoin(0);
    log.set_money(req->money());
    log.set_gold(req->money());
    log.set_exchangetype(4);
    log.set_depay(0.0);
    log.set_gdepay(0.0);
    log.set_state(0);
    log.set_createdate((uint32)time(NULL));
    psu->send_log(log);
}

void* export_cfg();

void test_sub_attr(client::test* req, scene_user* psu);
void test_clear_attr(client::test* req, scene_user* psu);

void* on_pb_test(client::test* req, void* ud, size_t* rsp_len)
{
#ifdef __INNER_DEBUG__PACKAGE_
	PB_FUNC_GUARD_FIND(req->svr_head().role_id());

    test_recharge(req, ss, psu);

    if (req->has_lua()) {
        do_lua(req, ud, rsp_len);
    } else if (req->has_hp()) {
        //client::auto_login_req via;
        //via.set_role_id(psu->get_role_id());
        //via.set_key("");
        //via.mutable_svr_head()->CopyFrom(req->svr_head());
        //psu->send_center(via);
        psu->call_pet(req->hp(), psu->get_player_role().get_pets());
	} else if (req->has_mp()) {
        reload_lua((game_service_t*)ud);
	} else if (req->has_prop_id() && req->has_prop_num()) {
        player_role& spr = psu->get_player_role();
		if (req->prop_id() == 0 && req->prop_num() == 0) {
            spr.get_role_props().get_pack().remove_all();
            spr.get_role_props().get_depot().remove_all();
		}

		const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(req->prop_id());
		if (base_info == NULL){
			return NULL;
		}

		psu->add_props(req->prop_id(),req->prop_num());

	} else if (req->has_attr_id() && req->has_attr_num()) {
		if (req->attr_id() < SEX || req->attr_id() >= ROLE_ATTRIBUTE_MAX) {
			return NULL;
		}

		if (req->attr_id() == LEVEL) {
            uint32 old_level = psu->mutable_attr()->get_val(LEVEL);
		//	psu->modify_rank_center(client::RANK_PERSONAL_LEVEL, req->attr_num());
            for (uint32 i = old_level + 1; i <= (uint32)req->attr_num(); ++i) {
                psu->on_level_up(i - 1, i);
            }
		}

		if (req->attr_id() == EXPERIENCE) {
			psu->add_exp(req->attr_num());
		} else if (req->attr_id() != INGOT) {
            psu->mutable_attr()->set_val((ROLE_ATTRIBUTE)req->attr_id(), req->attr_num());
        } else {
			psu->add_money(INGOT, req->attr_num());
		}

    } else if (req->has_add_attr_id() && req->has_attr_num()) {
        client::role_attr attr;
        const google::protobuf::Reflection* ref_attr = attr.GetReflection();
        const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
        if (req->add_attr_id() >= (uint32)des_attr->field_count()) {
            return NULL;
        }

        switch (des_attr->field(req->add_attr_id())->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 add = ref_attr->GetUInt32(attr, des_attr->field(req->add_attr_id()));
            ref_attr->SetUInt32(&attr, des_attr->field(req->add_attr_id()), req->attr_num() + add);
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint64 add = ref_attr->GetUInt64(attr, des_attr->field(req->add_attr_id()));
            ref_attr->SetUInt64(&attr, des_attr->field(req->add_attr_id()), req->attr_num() + add);
            break;
                                                             }
        default: break;
        }

        psu->get_player_role().get_player_role_ext_attr_mgr().add_ext_attr(psu->get_role_id(), REASTE_TEST, attr);
    } else if (req->has_sub_attr_id() && req->has_attr_num()) {
        test_sub_attr(req, psu);
    } else if (req->has_clear_add_attr()) {
        test_clear_attr(req, psu);
    } else if (req->has_task_id()) {
        inner_test(req, ud);

        player_role_task& tasks = psu->get_task().get_player_role_task();
        std::map<uint32, client::task_accepted>* pat = tasks.mutable_accepted_tasks();
        if (req->task_id() == 0) {
            const config_birth_mgr* mgr = GET_CONFIG_MGR(config_birth_mgr);
            if (mgr == NULL) {
                return NULL;
            }

            const config_birth* pcb = mgr->get_config_birth(psu->get_attr(PROFESSION));
            if (pcb == NULL) {
                return NULL;
            }

            req->set_task_id(pcb->get_quest_id());
            pat->clear();
            accept_task(pat, req->task_id(), psu);
            return NULL;
        }

        if (pat->begin()->first > req->task_id()) {
            //const config_quest_mgr* mgr = GET_CONFIG_MGR(config_quest_mgr);
            //mgr->get_config_quest
            
            pat->clear();
            accept_task(pat, req->task_id(), psu);
            return NULL;
        }

        client::submit_task_req task;
        std::map<uint32, client::task_accepted>::iterator i = pat->begin();
        task.mutable_svr_head()->CopyFrom(req->svr_head());
        while (i != pat->end() && i->second.task_id() != req->task_id()) {
            if (task.task_id() == i->second.task_id()) {
                break;
            }

            task.set_task_id(i->second.task_id());
            on_pb_submit_task_req(&task, ud, rsp_len);
            i = pat->begin();
        }
	} else if(req->has_drop_id()){
		// 测试掉落数据
		psu->add_drop_props(req->drop_id());
	} else if (req->has_land_day()){
		psu->get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY)->set_val(req->land_day());

	}else if (req->has_mount_skill_count()){
		player_role_mounts& m = psu->get_player_role().get_mounts();
		mysql::tb_role_mounts* pdb = m.mutable_db();
		if (pdb != NULL && pdb->mount_id() != 0) {
			uint32 reset_count = req->mount_skill_count();
			if (reset_count > 10){
				reset_count = 10;
			}
			pdb->set_skill_upgrade_num(reset_count);
			const mysql::tb_role_mounts& mounts = psu->get_player_role().get_mounts().get_db();
			client::mounts_msg mmsg;
			fill_mounts_msg(mounts, mmsg);
			psu->send_client(mmsg);
		}
	} else if (req->has_cfg_file() && req->has_cfg_table() && req->has_cfg_field()) {
        const std::map<std::string, TTableGroup_s*>* p = CONFMGR->get_all_cfg();
        std::map<std::string, TTableGroup_s*>::const_iterator i = p->find(req->cfg_file());
        if (i == p->end()) {
            printf("%s:%d, cfg file name error %s \n", __FILE__, __LINE__, req->cfg_file().c_str());
            return NULL;
        }

        TTable* table = i->second->GetTable(req->cfg_table());
        if (table == NULL) {
            printf("%s:%d, cfg table name error %s \n", __FILE__, __LINE__, req->cfg_table().c_str());
            return NULL;
        }

        std::string str;
        for (int i = 1; i <= table->m_recordCount; ++i) {
            str += table->GetFieldString(i, req->cfg_field());
            str += "\n";
        }

        std::string exfile = "./export_cfg/";
        exfile += req->cfg_file();
        exfile += ".txt";
        ofstream outfile(exfile.c_str(), ofstream::binary|ofstream::app);
        outfile.write(str.c_str(), str.length());
        outfile.close();
    } else if (req->has_cfg_file()) {
        return export_cfg();
        std::map<std::string, std::map<std::string, std::vector<std::string> > > taget;
        const char* mainquest[] = {"MapName","MainMapName","QuestName","AcceptDialogue","DoneDialogue","TrackTips"};
        for (int i = 0; i < 6; ++i) {
            taget["mainquest"  ]["t_mainquest"].push_back(mainquest[i]);
        }

        const char* item[] = {"ItemName","Describes"};
        for (int i = 0; i < 2; ++i) {
            taget["item"       ]["t_item"       ].push_back(item[i]);
        }

        taget["general"    ]["t_general"    ].push_back("general_name");

        taget["potential"  ]["t_potential"  ].push_back("numberName");
        taget["potential"  ]["t_potential"  ].push_back("showProbability");

        const char*  scene[] = {"MapName","MainChapter","FatherMapName","MapDecraption"};
        for (int i = 0; i < 4; ++i) {
            taget["scene"      ]["t_scene"      ].push_back(scene[i]);
        }

        taget["monster"    ]["t_monster"    ].push_back("MonsterName");
        taget["monster"    ]["t_monster"    ].push_back("SpeakContent");

        taget["skill"      ]["t_skill"      ].push_back("SkillName");
        taget["skill"      ]["t_skill"      ].push_back("Describes");

        taget["birth"      ]["birth"        ].push_back("JobDescribes");

        taget["upgrade"    ]["upgrade"      ].push_back("Msg");

        taget["skill_role" ]["t_skill_role" ].push_back("SkillName");
        taget["skill_role" ]["t_skill_role" ].push_back("Describes");

        const char*  effect_role[] = {"Tips","TypefaceFloat","Describes","AddBuffMsg","RemoveBuffMsg"};
        for (int i = 0; i < 5; ++i) {
            taget["effect_role"]["t_effect_role"].push_back(effect_role[i]);
        }

        taget["legion"     ]["t_legion"     ].push_back("LegionNotice");

        taget["magic"      ]["t_magic"      ].push_back("MagicName");

        taget["capital"    ]["t_capital"    ].push_back("CapitalPost");

        taget["pack"       ]["t_pack"       ].push_back("item_pack_name");
        taget["pack"       ]["t_pack"       ].push_back("describes");

        const char*  effect[] = {"Tips","TypefaceFloat","AddBuffMsg","RemoveBuffMsg"};
        for (int i = 0; i < 4; ++i) {
            taget["effect"     ]["t_effect"     ].push_back(effect[i]);
        }

        taget["mount"      ]["t_mount"      ].push_back("MountName");

        std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator i = taget.begin();
        std::string str;
        const std::map<std::string, TTableGroup_s*>* p = CONFMGR->get_all_cfg();
        for ( ; i != taget.end(); ++i) {
            std::map<std::string, TTableGroup_s*>::const_iterator mt = p->find(i->first);
            if (mt == p->end()) {
                printf("%s:%d, cfg file name error \"%s\" \n", __FILE__, __LINE__, i->first.c_str());
                return NULL;
            }

            std::map<std::string, std::vector<std::string> >::iterator j  = i->second.begin();
            for ( ; j != i->second.end(); ++j) {
                TTable* table = mt->second->GetTable(j->first);
                if (table == NULL) {
                    printf("%s:%d, cfg table name error \"%s\" \n", __FILE__, __LINE__, j->first.c_str());
                    return NULL;
                }

                for (size_t k = 0; k < j->second.size(); ++k) {
                    for (int i = 1; i <= table->m_recordCount; ++i) {
                        str += table->GetFieldString(i, j->second[k]);
                        str += "\n";
                    }
                }
            }

            std::string exfile = "./export_cfg/word.txt";
            ofstream outfile(exfile.c_str(), ofstream::binary|ofstream::app);
            outfile.write(str.c_str(), str.length());
            outfile.close();
        }
    }else if (req->has_area_id()){
		client::broadcast_msg_req req;
		req.set_area_id(47);
		psu->send_center(ss->ctx,req);
    }else if (req->has_pet_id() && req->has_pet_exp()){
		player_role_pet& player_pet = psu->get_player_role().get_pets();
		client::pet_db_info pet_info;
		if (!player_pet.get_pet_info(req->pet_id(),pet_info)){
			return NULL;
		}

		if (pet_info.pet_level() >= 100){
			return NULL;
		}

		uint32 player_level = psu->get_attr(LEVEL);
		std::vector<uint32> act_skill_id;
		act_skill_id.clear();
		if (player_pet.upgrate_pet_level(player_level,req->pet_exp(),req->pet_id(),act_skill_id) != 0){
			return NULL;
		}

		for (size_t i = 0; i < act_skill_id.size(); ++i){
			psu->get_player_role().get_role_skill_mgr().add_skill(psu->get_role_id(),act_skill_id[i]);
			const config_skill* skill_info = CONFMGR->get_config_skill(act_skill_id[i]);
			if (skill_info != NULL){
				skill sk(*skill_info);
				psu->mutable_skill()->add_skill(sk);
			}
		}

		client::pet_info_rsp rsp_msg;
		client::role_data* role_data = 	rsp_msg.mutable_rd();
		role_data->set_role_id(psu->get_role_id());
		role_data->set_inst_id(psu->get_inst_id());
		role_data->set_role_typ((client::role_data_TYPE)psu->get_role_type());
		client::pet_db_info* info = rsp_msg.add_pet_info();
		info->CopyFrom(pet_info);
		psu->send_client(rsp_msg);

		client::role_skill_msg rsm;
		psu->fill_pets_skill_msg(rsm,pet_info.pet_id(),pet_info.pet_level());
		psu->send_client(rsm);
    } else if(req->has_vip_lv()) {
		if (psu->set_vip_lv(req->vip_lv(), req->month() > 0) > 0) {
            psu->update_online_smy_info(client::SMY_ROLE_VIP, req->vip_lv(), req->month());
        }
	} else if(req->has_raid_id()) {
		on_raid_complete(ss, *req, psu);
	}

	psu->send_client_change_attr();
	psu->send_client_change_props_msg();
    psu->calculate_attr();
    psu->save_user();
#endif

    return NULL;
}

#include "game_map_mgr.h"

void* export_cfg()
{
    std::string str;
    const std::map<uint32, game_map*>& maps = GAME_MAP_MGR->get_map_list();
    std::map<uint32, game_map*>::const_iterator begin = maps.begin();
    std::map<uint32, game_map*>::const_iterator end   = maps.end();
    while (begin != end) {
        const std::map<uint32, scene*>& scenes = begin->second->get_scene_map();
        std::map<uint32, scene*>::const_iterator sbegin = scenes.begin();
        std::map<uint32, scene*>::const_iterator send   = scenes.end();
        while (sbegin != send) {
            int32 i = 0;
            char buf[32] = {0};
            sprintf(buf, "%d", sbegin->first);
            str += buf;
            int32 size = sbegin->second->get_create_npc_rules().rules_size();
            while (i < size) {
                int32 mi = 0;
                int32 msize = sbegin->second->get_create_npc_rules().rules(i).monster_size();
                while (mi < msize) {
                    if (sbegin->second->get_create_npc_rules().rules(i).monster(mi).has_monster_id()) {
                        sprintf(buf, " %d", sbegin->second->get_create_npc_rules().rules(i).monster(mi).monster_id());
                        str += buf;
                    }

                    ++mi;
                }

                ++i;
            }

            str += "\n";

            ++sbegin;
        }

        ++begin;
    }
    
    std::string exfile = "./export_cfg/monster_id.txt";
    ofstream outfile(exfile.c_str(), ofstream::binary|ofstream::trunc);
    outfile.write(str.c_str(), str.length());
    outfile.close();

    return NULL;
}

/*t_mainquest|MapName,MainMapName,QuestName,AcceptDialogue,DoneDialogue,TrackTips;
t_item|ItemName,Describes;
t_general|general_name;
t_potential|numberName,showProbability;
t_scene|MapName,MainChapter,FatherMapName,MapDecraption;
t_monster|MonsterName,SpeakContent;
t_skill|SkillName,Describes;
birth|JobDescribes;
upgrade|Msg;
t_skill_role|SkillName,Describes;
t_effect_role|Tips,TypefaceFloat,Describes,AddBuffMsg,RemoveBuffMsg;
t_legion|LegionNotice;
t_magic|MagicName;
t_capital|CapitalPost;
t_pack|item_pack_name,describes;
t_effect|Tips,TypefaceFloat,AddBuffMsg,RemoveBuffMsg;
t_mount|MountName;*/


void test_sub_attr(client::test* req, scene_user* psu)
{
    client::role_attr attr;
    const google::protobuf::Reflection* ref_attr = attr.GetReflection();
    const google::protobuf::Descriptor* des_attr = attr.GetDescriptor();
    if (req->sub_attr_id() >= (uint32)des_attr->field_count()) {
        return ;
    }

    switch (des_attr->field(req->sub_attr_id())->type()) {
    case google::protobuf::FieldDescriptor::TYPE_UINT32: {
        uint32 add = ref_attr->GetUInt32(attr, des_attr->field(req->sub_attr_id()));
        ref_attr->SetUInt32(&attr, des_attr->field(req->sub_attr_id()), req->attr_num() + add);
        break;
                                                         }
    case google::protobuf::FieldDescriptor::TYPE_UINT64: {
        uint64 add = ref_attr->GetUInt64(attr, des_attr->field(req->sub_attr_id()));
        ref_attr->SetUInt64(&attr, des_attr->field(req->sub_attr_id()), req->attr_num() + add);
        break;
                                                         }
    default: break;
    }

    psu->get_player_role().get_player_role_ext_attr_mgr().sub_ext_attr(psu->get_role_id(), REASTE_TEST, attr);

}

void test_clear_attr(client::test* req, scene_user* psu)
{
    psu->get_player_role().get_player_role_ext_attr_mgr().clear_ext_attr(psu->get_role_id(), REASTE_TEST);
}