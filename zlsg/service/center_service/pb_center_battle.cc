#include "center_interface.h"

void* on_center_pb_battle_info_req(client::battle_info_req* req, void* ud, size_t* rsp_len)
{
//     center_service_t* w = (center_service_t*)ud;
//     client::battle_info_rsp rsp;
//     //uint32 map_id   = 291;    // 291 三阵营战场地图ID
//     uint32 scene_id = 352;    // 352 三阵营战场场景ID
//     uint32 service_id = w->wm_mgr.get_scene_service_id(scene_id);
//     if (service_id == 0) { // 战场场景不存在 如果还没有开启 下发给客户端倒计时
//         uint32 battle_id = 6; // 6 三阵营战场ID
//         const config_battle* cb = CONFMGR->get_config_battle_mgr().get_config_battle(battle_id);
//         const mysql::tb_battle& tb = cb->get_tb();
//         const vector<const mysql::tb_battle_actvt_time*>& at = cb->get_tbat();
//         uint32 now = (uint32)time(NULL);
//         for (uint32 i = 0; i < at.size(); ++i) {
//             uint32 open_time = time_utils::make_time(at[i]->time_type(), at[i]->from_hh(), at[i]->from_mm(), at[i]->from_ss());
//             if (open_time > now) {
//                 rsp.set_open_seconds(open_time - now);
//             } else if (open_time + tb.duration() > now) {
//                 // 临时组织初始化战场信息 
//                 rsp.set_open_seconds(0);
//                 rsp.mutable_bi()->set_index(1);
//                 rsp.mutable_bi()->set_close_seconds(1800);
//                 rsp.mutable_bi()->set_angel_num(0);
//                 rsp.mutable_bi()->set_witch_num(0);
//                 rsp.mutable_bi()->set_nether_num(0);
// 
//                 // 启动战场场景
//             }
//             
//             center_send_client(w, req->svr_head(), rsp);
//             break;
//         }
//         
//     } else { // 转发消息到战场场景服务中 取出正确的数据再下发给客户端
//         center_send_scene_by_service_id(w, service_id, *req);
//     }

    return NULL;
}

void* on_pb_delivery_mail(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len);

// void send_reward_mail(center_service_t* w, void* ud, size_t* rsp_len, const char* title, uint32 role_id, const std::vector<const mysql::tb_sys_reward_detail*>& detail)
// {
//     client::delivery_mail_info dmi;
//     dmi.set_tael_cost(0);
//     int32 size = detail.size();
//     static const int32 max_size = 5;
//     client::mail_info* mi = dmi.mutable_mi();
//     client::mail_head* mh = mi->mutable_mh();
//     mh->set_send_role_name("落仙传");
//     mh->set_title(title);
//     mh->set_send_time((uint32)time(NULL));
//     mh->set_attachment_num(size < max_size ? size : max_size);
//     mh->set_opened(false);
//     client::mail_body* mb = mi->mutable_mb();
//     mb->set_content("本次战斗奖励");
//     for (int32 i = 0; i < size && i < max_size; ++i) {
// //         const mysql::tb_sys_reward_detail* tsrd = detail[i];
// //         if (tsrd == NULL || tsrd->reward_type() != REWARD_TYPE_GOODS) {
// //             continue;
// //         }
//         
//         //client::props_info* pi = mi->add_ma();
//         //const config_goods* cg = CONFMGR->get_config_goods_mgr().get_config_goods(tsrd->reward_enum());
//         //if (cg == NULL) {
//         //continue;
//         //}
//         
//         //CONFMGR->get_config_cmpst_mgr().assemble_goods(*pi, cg, tsrd->reward_val(), tsrd->quality(), tsrd->is_binding());
//     }
// 
//     const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(role_id);
//     if (smy == NULL) {
//         return ;
//     } 
// 
//     dmi.set_recv_name(smy->role_name());
//     dmi.mutable_mi()->mutable_mh()->set_recv_role_id(role_id);
//     on_pb_delivery_mail(&dmi, ud, rsp_len);
// }

void* on_center_pb_battle_reward_via(client::battle_reward_via* req, void* ud, size_t* rsp_len)
{
//     center_service_t* w = (center_service_t*)ud;
//     static const char* TITLE[] = {"战场奖励", "领土战奖励", "攻城战奖励", "宝石矿场奖励"};
//     if (req->type() >= sizeof(TITLE) / sizeof(char*)) {
//         return NULL;
//     }

//     const config_sys_reward_mgr& csrm = CONFMGR->get_sys_reward_mgr();
//     for (int32 i = 0; i < req->bri_size(); ++i) {
//         uint32 role_id   = req->bri(i).role_id();
//         std::vector<const mysql::tb_sys_reward_detail*> detail;
//         for (int32 j = 0; j < req->bri(i).reward_id_size(); ++j) {
//             uint32 reward_id = req->bri(i).reward_id(j);
//             const std::vector<const mysql::tb_sys_reward_detail*>& d = csrm.get_reward_detail(reward_id);
//             detail.insert(detail.end(), d.begin(), d.end());
//         }
//         send_reward_mail(w, ud, rsp_len, TITLE[req->type()], role_id, detail);
//     }
    
    return NULL;
}

