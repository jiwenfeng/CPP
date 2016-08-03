#ifndef __PLAYER_ROLE_LABEL_MGR_H__
#define __PLAYER_ROLE_LABEL_MGR_H__

#include "player_role_label.h"

class player_role_label_mgr
{
public:
    player_role_label_mgr();
    virtual ~player_role_label_mgr();

public:
    int push(mysql::tb_role_label* trl);

    int save(amtsvr_context* ctx);

    int attach_role_label(int pb_count, cmq_svr_pb_ctx* pb_ctx);

public:
    int create(uint32 role_id, uint32 label_code);

    int label_list(client::label_list_req* llr, client::label_list_rsp& rsp);

    int operate_label(client::operate_label_req* olr, client::operate_label_rsp& rsp);

    int use_label_list(client::get_role_label_info_rsp& rsp);

    int create_pushfig_label(uint32 role_id, uint32 code, uint32 star_num);

    uint32 get_first_label();

public:
    void cal_add_attr();
    inline const uint32* get_add_attr() const {return add_attr;}
    
private:
    int put_on(uint32 label_code);

    int take_off(uint32 label_code);

    int check_unique(uint32 label_code);

    int reset();
    
private:
    uint32 _count;
    map<uint32, player_role_label*> _label;
    uint32      add_attr[ATTIB_MAX]; // 增加到人物的属性
};

#endif /* __PLAYER_ROLE_LABEL_MGR_H__ */
