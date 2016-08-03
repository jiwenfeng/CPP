#ifndef __PLAYER_ROLE_ORDER_H__
#define __PLAYER_ROLE_ORDER_H__

#include "comm_define.h"

class player_role_order : public pb_writer
{
public:
    player_role_order(mysql::tb_role_order* tro = NULL);

    int create(uint32 role_id, uint32 store_id, uint32 type, uint32 count);
    
    virtual ~player_role_order();

public:
    inline void attach(mysql::tb_role_order* tro){_order = tro;}
    inline void release()
    {
        if (_order != NULL) {
            delete _order;
            _order = NULL;
        }
    }
    void save(amtsvr_context* ctx);

public:
    int create(uint32 role_id, uint32 store_id, uint32 count, uint32 type, uint32 time);

    int add(uint32 count);

    uint32 get(); 

    int check_time();

    void fill(client::order_tracking_info* oti);

    int remove();

private:
    mysql::tb_role_order* _order;
};

#endif /* __PLAYER_ROLE_ORDER_H__ */
