#include "operate_log.h"
#include "player_role.h"
#include "player_role_order_mgr.h"
#include "player_role_statis_mgr.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "player_role_treasure.h"
// ¹ºÂòÎïÆ·
void* on_pb_buy_goods_req(client::buy_goods_req* b, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(b->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

	if (!su->is_can_add_props()){
		return NULL;
	}

	client::buy_goods_rsp rsp_msg;
    su->on_pb_buy_goods_npc_shop(b,rsp_msg);
	su->send_client(ss->ctx,rsp_msg);

	CHECK_TARGET_PROGRESS(su, SHOPPING, b->shop_id());

    return NULL;
}

void* on_pb_shop_item_list_req(client::shop_item_list_req* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL){
		return NULL;
	}

	client::shop_item_list_rsp rsp_msg;
	rsp_msg.set_shop_type(req->shop_type());

	std::map<uint32, client::treasure_db_info> shop_limit_map;
	shop_limit_map.clear();

	su->get_player_role().get_role_treasures().get_shop_treasure_db_info(req->shop_type(),shop_limit_map);
	std::map<uint32, client::treasure_db_info>::iterator it = shop_limit_map.begin();
	for (; it != shop_limit_map.end(); ++it){
		client::treasure_db_info* info = rsp_msg.add_info();
		info->CopyFrom(it->second);
	}

	su->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(), req->shop_type());

	return NULL;
}

void* on_pb_sell_goods_req(client::sell_goods_req* s, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(s->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", su->get_role_id(), s->GetTypeName().c_str(), s->grid_index(), s->num());

    return NULL;
}

void* on_pb_buy_mall_req(client::buy_mall_req* b, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(b->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    //su->on_pb_buy_mall_req(b);

    char buf[64] = {0};
    std::string format = "";
    int32 size = b->bm_size();
    for (int32 i = 0; i < size && i < 10; ++i) {
        const client::buy_mall& bm = b->bm(i);
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "|%d|%d|%d", bm.goods_id(), bm.num(), bm.is_goods_id());
        format += buf;
    }
    
    amtsvr_billlog(ss->ctx, "%u|%s%s", su->get_role_id(), b->GetTypeName().c_str(), format.c_str());

    return NULL;
}

void* on_game_pb_order_tracking_req(client::order_tracking_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    order_tracking_rsp rsp;
    player_role& spr = su->get_player_role();
    spr.get_order_mgr().fill(rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_game_pb_buy_tael_req(client::buy_tael_req* req, void* ud, size_t* rsp_len)
{
    //struct game_service_t* ss = (struct game_service_t*)ud;
    //scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    //if (su == NULL){
    //    return NULL;
    //}
    //
    //client::buy_tael_rsp rsp;
    //uint32 count = su->get_player_role().get_role_statis_mgr().exchange_tael();
    //if (count < 5) {
    //    uint32 old_value = su->get_attr().ingot();
    //    if (count == 0) {
    //        su->add_money(TAEL, 20000);
    //        rsp.set_ret(0);
    //    } else if (su->sub_money(INGOT, 2 * count) == 0) {
    //        uint32 new_value = su->get_attr().ingot();
    //        su->sub_money_log(operate_log::ROOT_LUCKY, operate_log::TYPE_LUCKY, old_value, new_value);
    //        su->add_money(TAEL, 20000 * (count + 1));
    //        rsp.set_ret(0);
    //    } else {
    //        rsp.set_ret(1);
    //    }
    //} else {
    //    rsp.set_ret(1);
    //}

    //if (rsp.ret() == 0) {
    //    su->get_player_role().get_role_statis_mgr().add_exchange_tael();
    //    su->send_client_change_attr();
    //}
    //
    //game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_pb_get_svr_start_time_req(client::get_svr_start_time_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    uint32 now = (uint32)time(NULL);
    client::get_svr_start_time_rsp rsp;
    uint32 days = amtsvr_svr_start_day(now);
    rsp.set_days(days);
    rsp.set_now(now);
    if (days > 3){
        rsp.set_shop_discount_seconds(0);
    } else {
        time_t tnow = now;
        struct tm pnow;
        localtime_r(&tnow, &pnow);
        pnow.tm_hour = 23;
        pnow.tm_min = 59;
        pnow.tm_sec = 59;
        uint32 seconds = (uint32)mktime(&pnow);
        rsp.set_shop_discount_seconds((seconds - now) + ((3 - days) * 86400));
    }

    su->send_client(rsp);

    return NULL;
}

