#ifndef __PLAYER_ROLE_LABEL_H__
#define __PLAYER_ROLE_LABEL_H__

#include "comm_define.h"

class player_role_label : public pb_writer
{
public:
    player_role_label(mysql::tb_role_label* p = NULL);
    virtual ~player_role_label();

public:
    void attach(mysql::tb_role_label* p) {_trl = p;};

    int save(amtsvr_context* ctx);

    mysql::tb_role_label* get() {return _trl;};

public:
    int create(uint32 role_id, uint32 label_code);

    uint32 label_code();
    
    int set_use_state(uint32 use_state);
    
    int set_effective_state(uint32 effective_state);

    int set_end_date(uint32 end_time);
    
    bool is_end();

    int fill_label_info(client::label_info* li);

private:
    mysql::tb_role_label* _trl;
};

#endif /* __PLAYER_ROLE_LABEL_H__ */
