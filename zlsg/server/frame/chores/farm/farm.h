#ifndef __FARM_H__
#define __FARM_H__

#include <map>

#include "pb_mgr.h"
#include "pb_writer.h"
#include "config_mgr.h"

#define MONEY_TREE_GAIN_INTERVAL 86400
#define FARM_MAX_LEVEL 8
#define MAX_FARM_RECORD 18

struct farm_level_cfg
{
    uint32 max_lands;
    uint32 money;
    uint32 upscore;
};

class farm : public pb_writer
{
public:
    farm();

    ~farm();

public:

    int attach(mysql::tb_role_farm* p);

    int save(amtsvr_context* ctx);

    int create(uint32 role_id, uint32 level, uint32 now);

    uint32 get_role_id() const {return fi.role_id();}

    const client::farm_info& get_farm_info() const {return fi;}

    int update(uint32 now);

    client::FARM_OPERATE_E sow(uint32 now, uint32 land_id, uint32 serial);

    client::farm_land_info* get_land_info(uint32 land_id);

    client::FARM_OPERATE_E steal(uint32 now, uint32 thief_role_id, uint32 land_id, uint32& goods_id, uint32& num);

    client::FARM_OPERATE_E gain(client::farm_gain_req& req, uint32 now, client::farm_gain_msg& msg, uint32& add_score);

    client::FARM_OPERATE_E gain_land(uint32 now, uint32 land_id, client::farm_gain_msg& msg, uint32& add_score);

    client::FARM_OPERATE_E improve_land(uint32 now, uint32 land_id, client::FARM_IMPROVE_TYPE_E type);

    client::FARM_OPERATE_E gain_money_tree(uint32 now, uint32& money);

    void add_farm_score(uint32 add_score);
    uint32 get_total_score(){return fi.total_score();}

    void fill_farm_record_rsp(client::farm_record_rsp& rsp);

private:
    int init_land(uint32 now);

    int parse(mysql::tb_role_farm* data);

    int serial(string* out);

//     void land_status(client::farm_land_info* fli, uint32 now, const mysql::tb_farm_set* tfs);

    farm_level_cfg& get_level_cfg();

    bool can_sow();

    void land_gift_sow(uint32 now, client::farm_land_info* fli);

    client::FARM_OPERATE_E sow_land_ex(uint32 now, client::farm_land_info* fli, uint32 serial);

    void record_steal(uint32 now, uint32 thief_role_id, uint32 serial, uint32 num);

    void record_improve(uint32 now, uint32 role_id, uint32 serial, uint32 add_num, uint32 times);

    void just_recored_pos();

private:
    client::farm_info      fi;
    
    uint32                 head;
    uint32                 tail;
    client::farm_record    fr[MAX_FARM_RECORD];
};




#endif
