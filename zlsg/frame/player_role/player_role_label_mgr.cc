#include "player_role_label_mgr.h"

player_role_label_mgr::player_role_label_mgr()
{
    _count = 0;
}

player_role_label_mgr::~player_role_label_mgr()
{
    map<uint32, player_role_label*>::iterator i = _label.begin();
    
    for (; i != _label.end(); ++i) {
        delete i->second;
    }

    _label.clear();
}

int player_role_label_mgr::push(mysql::tb_role_label* trl)
{
    if (trl == NULL) {
        return -1;
    }

    player_role_label* prl = new player_role_label(trl);
    _label.insert(pair<uint32, player_role_label*>(trl->label_code(), prl));
    
    return 0;
}

int player_role_label_mgr::save(amtsvr_context* ctx)
{
    map<uint32, player_role_label*>::iterator i = _label.begin();
    for (; i != _label.end(); ++i) {
        i->second->save(ctx);
    }

    return 0;
}

int player_role_label_mgr::create(uint32 role_id, uint32 label_code)
{
//     const mysql::tb_sys_label* tsl = NULL;
//     tsl = CONFMGR->get_sys_label(label_code);
//     if (tsl == NULL) {
//         return -1;
//     }
        
    map<uint32, player_role_label*>::iterator i = _label.find(label_code);
    if (i != _label.end()) {
        return -1;
    }
    
    player_role_label* prl = new player_role_label;
    prl->create(role_id, label_code);
    _label.insert(pair<uint32, player_role_label*>(label_code, prl));
    
    return 0;
}

int player_role_label_mgr::label_list(client::label_list_req* llr, client::label_list_rsp& rsp)
{
    map<uint32, player_role_label*>::iterator i = _label.begin();
    for (; i != _label.end(); ++i) {
        i->second->fill_label_info(rsp.add_li());
    }
    
    return 0;
}

int player_role_label_mgr::operate_label(client::operate_label_req* olr, client::operate_label_rsp& rsp)
{
    uint32 result = 0;
    rsp.set_type((client::operate_label_rsp_OPERATE_TYPE)olr->type());
    
    switch (olr->type()) {
    case client::operate_label_req_OPERATE_TYPE_ADD:
        {
            result = put_on(olr->label_code());
            break;
        }
    case client::operate_label_req_OPERATE_TYPE_REMOVE:
        {
            result = take_off(olr->label_code());
            break;
        }
    case client::operate_label_req_OPERATE_TYPE_REPLACE:
        {
            result = put_on(olr->label_code());
            break;
        }
    }

    rsp.set_result((client::operate_label_rsp_OPERATE_RESULT)(result));

    return result;
}

int player_role_label_mgr::use_label_list(client::get_role_label_info_rsp& rsp)
{
    map<uint32, player_role_label*>::iterator i = _label.begin();
    for (; i != _label.end(); ++i) {
        mysql::tb_role_label* trl = i->second->get();
        if (trl == NULL/* || trl->use_state() == 0*/) {
            continue;
        }

        i->second->fill_label_info(rsp.add_li());
    }

    return 0;
}

int player_role_label_mgr::create_pushfig_label(uint32 role_id, uint32 code, uint32 star_num)
{
    if (code > 3 || star_num > 3) {
        return -1;
    }

    int ret = -1;
    for (int i = star_num; i >= 0; --i) {
        uint32 label_code = 50 + code * 4 + i;
        if (create(role_id, label_code) != 0) {
            break;
        } else {
            ret = 0;
        }
    }

    return ret;
}

void player_role_label_mgr::cal_add_attr()
{
    memset(add_attr, 0, sizeof(add_attr));
    map<uint32, player_role_label*>::iterator i = _label.begin();
    for (; i != _label.end(); ++i) {
//         const mysql::tb_sys_label* tsl = NULL;
//         tsl = CONFMGR->get_sys_label(i->first);
//         if (tsl == NULL) {
//             continue;
//         }
// 
//         if ((ROLE_ATTR_E)tsl->add_state_code_1() < ATTIB_MAX) {
//             add_attr[tsl->add_state_code_1()] += tsl->add_state_val_1();
//         }
//         
//         if ((ROLE_ATTR_E)tsl->add_state_code_2() < ATTIB_MAX) {
//             add_attr[tsl->add_state_code_2()] += tsl->add_state_val_2();
//         }
// 
//         if ((ROLE_ATTR_E)tsl->add_state_code_3() < ATTIB_MAX) {
//             add_attr[tsl->add_state_code_3()] += tsl->add_state_val_3();
//         }
// 
//         if ((ROLE_ATTR_E)tsl->add_state_code_4() < ATTIB_MAX) {
//             add_attr[tsl->add_state_code_4()] += tsl->add_state_val_4();
//         }
// 
//         if ((ROLE_ATTR_E)tsl->add_state_code_5() < ATTIB_MAX) {
//             add_attr[tsl->add_state_code_5()] += tsl->add_state_val_5();
//         }
    }
}


int player_role_label_mgr::put_on(uint32 label_code)
{
    map<uint32, player_role_label*>::iterator i = _label.find(label_code);
    if (i == _label.end()) {
        return 1;
    }
    
    if (check_unique(label_code) == 1) {
        return 1;
    }

    i->second->set_use_state(1);
    
    return 0;
}

int player_role_label_mgr::take_off(uint32 label_code)
{
    map<uint32, player_role_label*>::iterator i = _label.find(label_code);
    if (i == _label.end()) {
        return 1;
    }

    i->second->set_use_state(0);
    
    return 0;
}

#define RESET_TYPE 2

int player_role_label_mgr::check_unique(uint32 label_code)
{
    map<uint32, player_role_label*>::iterator i = _label.find(label_code);
    if (i == _label.end()) {
        return 1;
    }
    
//     const mysql::tb_sys_label* tsl = NULL;
//     tsl = CONFMGR->get_sys_label(label_code);
//     if (tsl == NULL) {
//         return 1;
//     }else if (tsl->label_type() == RESET_TYPE) {
//         reset();
//     }
    
    return 0;
}

int player_role_label_mgr::reset()
{
//     map<uint32, player_role_label*>::iterator i = _label.begin();
//     for (; i != _label.end(); ++i) {
//         const mysql::tb_sys_label* tsl = NULL;
//         tsl = CONFMGR->get_sys_label(i->second->label_code());
//         if (tsl == NULL) {
//             continue ;
//         } else if (tsl->label_type() == RESET_TYPE) {
//             i->second->set_use_state(0);
//         }
//     }
    
    return 0;
}

int player_role_label_mgr::attach_role_label(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[0].pb_msg == NULL) {
            continue;
        }
        if (push(static_cast<mysql::tb_role_label*>(pb_ctx[i].pb_msg)) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

uint32 player_role_label_mgr::get_first_label()
{
    map<uint32, player_role_label*>::reverse_iterator i = _label.rbegin();
    for (; i != _label.rend(); ++i) {
//         const mysql::tb_sys_label* tsl = CONFMGR->get_sys_label(i->first);
//         if (tsl != NULL && tsl->label_type() == RESET_TYPE) {
//             return i->first;
//         }
    }

    return 0;
}
