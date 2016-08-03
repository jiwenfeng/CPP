#include "config_legion.h"
#include "game_interface.h"
#include "player_role.h"
#include "player_role_props.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "family_scene_inst.h"
#include "siege_scene_inst.h"
#include "player_role_ext_attr_family.h"

void* on_game_pb_family_rsp(client::family_rsp* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    if (ss == NULL) {
        return NULL;
    }

    std::vector<scene_inst*> vsi;
    uint32 scene_id = family_scene_inst::FAMILY_SCENE_ID;
    ss->sim->get_scene_by_scene_id(scene_id, vsi);
    size_t size = vsi.size();
    for (size_t i = 0; i < size; ++i) {
    }
    
    return NULL;
}
// 创建军团
void* on_game_pb_create_family_req(client::create_family_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
	//先检测是否已经有军团
	uint32 family_id = psu->mutable_attr()->get_val(FAMILY_ID);
	if (family_id != 0){
		return NULL;
	}

	const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(1);
	if (legion_info == NULL){
		return NULL;
	}
    // 检测相关道具
	switch(req->type())
	{
	case create_family_req_use_copper:// 铜钱创建
		{
			uint32 money = psu->mutable_attr()->get_val(TAEL);
			uint32 need_pay = legion_info->get_create_need_money();
			if (money < need_pay){
				return NULL;
			}
		}
		break;
	case create_family_req_use_silver:// 元宝创建
		{
			uint32 cur_ingot = psu->mutable_attr()->get_val(INGOT);
			uint32 need_siliver = legion_info->get_create_need_silver();
			//props_parser& pack_obj = psu->get_player_role().get_role_props().get_pack();
			if (cur_ingot < need_siliver){
				return NULL;
			}

		}
		break;
	case create_family_req_use_token:// 军团令创建
		{
			props_parser& pack_obj = psu->get_player_role().get_role_props().get_pack();
			const std::map<int,int>& need_item_map_ = legion_info->get_upgrade_need_item();
			std::map<int,int>::const_iterator itr = need_item_map_.begin();
			for (; itr != need_item_map_.end(); ++itr){
				if (pack_obj.get_props_num(itr->first) < static_cast<uint32>(itr->second)){
					return NULL;
				}
			}
		}
		break;
	default:
		return NULL;
		break;
	}

    game_send_center(ss, *req);

    return NULL;
}
// 扣除创建军团的消耗
void* on_pb_set_create_famliy_consume(client::set_create_famliy_consume* req, void *ud, size_t* rsp_len)
{
	 PB_FUNC_GUARD_VIA(req);

	 if (req->is_succeed() != set_create_famliy_consume_result_RESULT_SUCCESS){
		 return NULL;
	 }

	 const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(1);
	 if (legion_info == NULL){
		 return NULL;
	 }

	 switch(req->type())
	 {
	 case create_family_req_use_copper:// 铜钱
		 {
			 uint32 money = psu->mutable_attr()->get_val(TAEL);
			 uint32 need_pay = legion_info->get_create_need_money();
			 if (money >= need_pay){
				psu->sub_money(TAEL,need_pay);
				psu->send_client_change_attr();
			 }
		 }
		 break;
	 case create_family_req_use_silver:// 元宝
		 {
			 uint32 cur_ingot = psu->mutable_attr()->get_val(INGOT);
			 uint32 need_siliver = legion_info->get_create_need_silver();
			 if (cur_ingot >= need_siliver){
				 psu->sub_money(INGOT,need_siliver);
				 psu->send_client_change_attr();
			 }

			 psu->sub_money_log(MC_FAMILY_SPEND, MC_CREATE_FAMILY, cur_ingot, psu->get_attr(INGOT));
		 }
		 break;
	 case create_family_req_use_token:// 军团令
		 {
			 props_parser& pack_obj = psu->get_player_role().get_role_props().get_pack();
			 const std::map<int,int>& need_item_map_ = legion_info->get_upgrade_need_item();
			 std::map<int,int>::const_iterator itr = need_item_map_.begin();
			 for (; itr != need_item_map_.end(); ++itr){
				 if (pack_obj.get_props_num(itr->first) >= static_cast<uint32>(itr->second)){
					 pack_obj.remove_props(itr->first,itr->second);
					 psu->send_client_change_props_msg();
				 }
			 }
		 }
		 break;
	 default:
		 return NULL;
		 break;
	 }

	 return NULL;
}
// 设置个人的军团ID与头衔center_sever2game_sever
void* on_pb_set_family_req(client::set_family_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD_VIA(req);

	if(req->family_id() == 0)
	{
		psu->clear_friend_family();
		client::load_friend_family_rsp lff;
		lff.set_role_id(psu->get_role_id());
		psu->send_client(lff);
	}
	else
	{
		if(req->family_id() != psu->get_family_id())
		{
			client::load_friend_family_req llf;
			llf.set_family_id(req->family_id());
			llf.set_role_id(psu->get_role_id());
			psu->send_center(ss->ctx, llf);
		}
	}

    psu->set_family_id(req->family_id());
    psu->mutable_attr()->set_val(FAMILY_POSITION, req->title());
	psu->mutable_attr()->set_val(FAMILY_LEVEL,req->family_level());
	if (req->has_donate()){
		psu->mutable_attr()->set_val(CAN_JOIN_FAMILY,req->donate());
	}
	psu->send_client_change_attr();
	psu->calculate_attr();
    psu->send_client_change_attr();

    return NULL;
}
// 军团贡献
void* on_game_pb_donate_family_req(client::donate_family_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

	// 检测一下捐献的东西够不
    client::donate_family_rsp rsp;
    int ret = 0;

    rsp.set_type(req->type());

    props_parser& pp = psu->get_player_role().get_role_props().get_pack();

    switch(req->type())
    {
    case DONATE_TYPE_COIN:
        {
            uint32 cur_money = psu->mutable_attr()->get_val(TAEL);
            if (cur_money < req->num()){
                rsp.set_ret(donate_family_rsp_result_not_enough);
                ret = -1;
            }
        }
        break;
    case DONATE_TYPE_DRAGON:// 青龙令
        {
            if (pp.get_props_num(50145001) < req->num()){
                rsp.set_ret(donate_family_rsp_result_not_enough);
                ret = -1;
            }
        }
        break;
    case DONATE_TYPE_TIGER: // 白虎令
        {
            if (pp.get_props_num(50145004) < req->num()){
                rsp.set_ret(donate_family_rsp_result_not_enough);
                ret = -1;
            }
        }
        break;
    case DONATE_TYPE_ROSEFINCH: // 朱雀
        {
            if (pp.get_props_num(50145002) < req->num()){
                rsp.set_ret(donate_family_rsp_result_not_enough);
                ret = -1;
            }
        }
        break;
    case DONATE_TYPE_TORROISE: // 玄武
        {
            if (pp.get_props_num(50145003) < req->num()){
                rsp.set_ret(donate_family_rsp_result_not_enough);
                ret = -1;
            }
        }
        break;
    default:
        rsp.set_ret(donate_family_rsp_result_donate_type_err);
        break;
    }

	if (ret != 0) {
		game_send_client(ss, req->svr_head(), rsp);
	} else{
        set_family_money via;
        via.set_role_id(req->svr_head().role_id());
        via.set_money(req->num());
        via.set_type(req->type());
		via.set_server_id(req->svr_head().svr_id());
        game_send_center(ss, via);

		CHECK_TARGET_PROGRESS(psu, FAMILY_DONATE, req->type());
    }

    return NULL;
}
// 自动申请退团
void* on_game_pb_secede_family_req(client::secede_family_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::secede_family_to_center center_msg;
	center_msg.set_family_id(req->family_id());
	center_msg.set_role_id(req->role_id());
	center_msg.set_server_id(req->svr_head().svr_id());
	game_send_center(ss, center_msg);

	return NULL;
}
// center2game捐献回复扣除相关道具与增加相关贡献值
void* on_pb_set_donate_consume(client::set_donate_consume* req, void *ud, size_t* rsp_len)
{
	 PB_FUNC_GUARD_VIA(req);

	 if (!req->is_success()){
		 return NULL;
	 }

	 uint32 item_id = 0;
	 switch(req->type())
	 {
	 case DONATE_TYPE_COIN:
		 {
			 psu->sub_money(TAEL,req->count());
			 psu->send_client_change_attr();
		 }
		 break;
	 case DONATE_TYPE_DRAGON:item_id = 50145001;break;
	 case DONATE_TYPE_TIGER:item_id = 50145004;break;
	 case DONATE_TYPE_ROSEFINCH:item_id = 50145002;break;
	 case DONATE_TYPE_TORROISE:item_id = 50145003;break;
	 default:
		 {
			 psu->sub_money(TAEL,req->count());
			 psu->send_client_change_attr();
		 }
		 break;
	 }

	if (item_id != 0){
		props_parser& pack_obj = psu->get_player_role().get_role_props().get_pack();
		pack_obj.remove_props(item_id,req->count());
		psu->send_client_change_props_msg();
	}

	if (req->contr_value() != 0){
		psu->add_money(CAN_JOIN_FAMILY,req->contr_value());
		psu->send_client_change_attr();
	}

	client::notive_center_donate_sucess rsp_msg;
	rsp_msg.set_role_id(req->role_id());
	rsp_msg.set_type(req->type());
	rsp_msg.set_total_donate(req->total_value());
	rsp_msg.set_money(req->money());
	if (req->is_success()){
		rsp_msg.set_ret(donate_family_rsp_result_success);
	}
	game_send_center(ss, rsp_msg);
	return NULL;
}
// 创建军团失败后的处理
//void* on_pb_create_family_failure(client::create_family_failure* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req->mutable_cfr());
//
//    psu->on_pb_create_family_failure(req);
//    
//    return NULL;
//}


//void* on_game_pb_family_welfare_req(client::family_welfare_req* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req);
//
//    family_welfare_rsp rsp;
//    psu->on_pb_family_welfare_req(rsp);
//    game_send_client(ss, req->svr_head().client_id(), rsp);
//
//    return NULL;
//}

//void* on_game_pb_jump_to_family_scene(client::jump_to_family_scene* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD_VIA(req)
//    
//    bool leave_scene = false;
//    client::jump_user ju;
//    ju.set_role_id(psu->get_role_id());
//    //psu->on_pb_jump_to_family_scene(leave_scene, ju);
//    if (leave_scene) {
//        ss->sim->logout_scene(psu, psu->get_scene());
//        psu->send_center(ss->ctx, ju);
//    }
//
//    return NULL;
//}

//void* on_game_pb_call_family_boss(client::call_family_boss* req, void* ud, size_t* rsp_len)
//{
//    struct game_service_t* ss = (struct game_service_t*)ud;
//    if (ss == NULL) {
//        return NULL;
//    }
//
//    return NULL;
//}

//void* on_game_pb_family_monster_boss_rsp(client::family_monster_boss_rsp* rsp, void* ud, size_t* rsp_len)
//{
//    struct game_service_t* ss = (struct game_service_t*)ud;
//    if (ss == NULL) {
//        return NULL;
//    }
//    
//    std::vector<scene_inst*> vsi;
//    uint32 scene_id = family_scene_inst::FAMILY_SCENE_ID;
//    ss->sim->get_scene_by_scene_id(scene_id, vsi);
//    size_t size = vsi.size();
//    for (size_t i = 0; i < size; ++i) {
//    }
//
//    return NULL;
//}

//void* on_game_pb_inferno_struck_open_via(client::inferno_struck_open_via* req, void* ud, size_t* rsp_len)
//{
//    struct game_service_t* ss = (struct game_service_t*)ud;
//    if (ss == NULL) {
//        return NULL;
//    }
//    
//
//    return NULL;
//}

//void* on_game_pb_inferno_struck_skill_req(client::inferno_struck_skill_req* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req);
//    return NULL;
//}

//void* on_game_pb_family_role_attr_req(client::family_role_attr_req* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req);
//
//    player_role_ext_attr_mgr& a = psu->get_player_role().get_player_role_ext_attr_mgr();
//    player_role_ext_attr* p = a.get(REASTE_FAMILY, psu->get_role_id());
//    if (p == NULL) {
//        return NULL;
//    }
//    
//    client::family_role_attr_rsp rsp;
//    static_cast<player_role_ext_attr_family*>(p)->fill(rsp);
//    game_send_client(ss, req->svr_head().client_id(), rsp);
//
//    return NULL;
//}

//void* on_game_pb_family_role_attr_upgrade_req(client::family_role_attr_upgrade_req* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req);
//    return NULL;
//}

//void to_family_role_attr(uint32 lv, uint32 num, uint32* attr)
//{
//    static uint32 FAMILY_SKILL_ATTR[10][16] = {
//        {11,  17 , 2 , 3 , 1, 1 , 1 , 2 , 1, 1 , 1, 1 , 1, 1 , 1, 1 }, 
//        {22,  33 , 4 , 6 , 2, 3 , 2 , 4 , 2, 3 , 1, 3 , 2, 3 , 2, 3 }, 
//        {33,  50 , 6 , 9 , 3, 4 , 3 , 5 , 3, 4 , 2, 3 , 3, 4 , 3, 4 }, 
//        {44,  67 , 8 , 11, 4, 5 , 4 , 7 , 4, 5 , 3, 4 , 4, 5 , 4, 5 }, 
//        {56,  83 , 10, 14, 5, 7 , 6 , 8 , 5, 7 , 4, 5 , 5, 7 , 5, 7 }, 
//        {66,  100, 12, 17, 6, 8 , 7 , 10, 6, 8 , 4, 7 , 6, 8 , 6, 8 }, 
//        {78,  116, 14, 20, 6, 10, 8 , 12, 6, 10, 5, 8 , 6, 10, 6, 10}, 
//        {88,  133, 15, 23, 7, 11, 9 , 13, 7, 11, 6, 8 , 7, 11, 7, 11}, 
//        {100, 149, 17, 26, 8, 13, 10, 15, 8, 13, 6, 10, 8, 13, 8, 13}, 
//        {111, 166, 19, 29, 9, 14, 11, 17, 9, 14, 7, 11, 9, 14, 9, 14}};
//
//    --lv;
//    for (uint32 i = 0; i < 8; ++i) {
//        attr[i] = FAMILY_SKILL_ATTR[lv][num + i * 2];
//    }
//}

//void* on_game_pb_family_role_attr_upgrade_via(client::family_role_attr_upgrade_via* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD_VIA(req)
//
//    client::family_role_attr_upgrade_rsp rsp;
//    if (req->family_id() == 0 || req->lv() == 0 || req->lv() > 10) {
//        rsp.set_ret(3);
//        psu->send_client(rsp);
//        return NULL;
//    }
//    
//    player_role_ext_attr_mgr& a = psu->get_player_role().get_player_role_ext_attr_mgr();
//    player_role_ext_attr* p = a.get(REASTE_FAMILY, psu->get_role_id());
//    if (p == NULL) {
//        return NULL;
//    }
//    
////     player_role_ext_attr_family* pa = static_cast<player_role_ext_attr_family*>(p);
//    player_role_statis_mgr& s = psu->get_player_role().get_role_statis_mgr();
//    uint32 num = s.get_family_skill_attr(req->type());
//    uint32 lv  = req->lv();
//    if (num < 2 && lv > 0 && lv <= 10) {
////         uint32 fc = num == 0 ? 150 : 200;
////         if (psu->sub_family_contribution(fc) == 0) {
////             uint32 attr[8];
////             to_family_role_attr(lv, num, attr);
////             pa->add(req->type(), attr);
////             s.add_family_skill_attr(req->type());
////             psu->send_client_change_attr();
////             rsp.set_ret(0);
////         } else {
////             rsp.set_ret(1);
////         }
//    } else {
//        rsp.set_ret(2);
//    }
//
//    psu->send_client(rsp);
//
//    return NULL;
//}
// center to game
void* on_pb_update_player_power_to_smy(client::update_player_power_to_smy* req, void *ud, size_t* rsp_len)
{
	PB_FUNC_GUARD_VIA(req);

	if (psu == NULL){
		return NULL;
	}

	uint32 fight_power = psu->mutable_attr()->get_val(FIGHTING_POWER);

	client::update_smy_role_fight_power update_msg;

	update_msg.set_role_id(psu->get_role_id());
	update_msg.set_fight_power(fight_power);
	game_send_center(ss, update_msg);

	return NULL;
}

void *on_pb_load_friend_family_rsp(client::load_friend_family_rsp *rsp, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(rsp);
	for(int i = 0; i < rsp->id_size(); ++i)
	{
		psu->add_friend_family(rsp->id(i));
	}
	psu->send_client(*rsp);
	return NULL;
}

void *on_pb_update_friend_family_rsp(client::update_friend_family_rsp *rsp, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(rsp);
	switch(rsp->type())
	{
		case 0:
			psu->remove_friend_family(rsp->family_id());
			break;
		case 1:
			psu->add_friend_family(rsp->family_id());
			break;
		default:
			return NULL;
	}
	psu->send_client(*rsp);
	return NULL;
}
