#ifndef __PROMO_H__
#define __PROMO_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"
#if 0
class promo : public pb_writer
{
public:
    promo(mysql::tb_promo_list_of_names* tb);
    promo(uint32 type, uint32 role_id, uint32 val, uint32 reward);
    ~promo();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_promo_list_of_names* get();

    void set_reward(uint32 reward);

private:
    void init(uint32 type, uint32 role_id, uint32 val, uint32 reward);
    
private:
    mysql::tb_promo_list_of_names* _promo_list_of_names;
    bool _change;
};
#endif
#endif // __PROMO_H__

