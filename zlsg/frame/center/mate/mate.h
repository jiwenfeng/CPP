#ifndef __MATE_H__
#define __MATE_H__

#include "pb_mgr.h"
#include "pb_writer.h"

#define MAX_DAY_DEPTH_MATE_TIMES 3
#define FREE_DAY_DEPTH_MATE_TIMES 1
#define DEPTH_MATE_TIME 120

class mate : public pb_writer
{
public:
    mate();

    virtual ~mate();

    int attach(mysql::tb_mate* p);

    int save_data(amtsvr_context* ctx);

public:
    client::MATE_OPERATE_RET_E fill_mate_info_rsp(client::mate_info_rsp& rsp);

    inline uint32 get_mate_role_id() const {return data != NULL ? data->mate_role_id() : 0;}

    int create(uint32 role_id, uint32 mate_id);

    int remove();

    uint32 get_can_depth_times();

    bool is_in_depth_mate();

    int depth_mate(uint32 now, bool advanced);

    client::MATE_OPERATE_RET_E depth_mate_end(uint32 now, bool& advanced);
    client::MATE_OPERATE_RET_E depth_mate_end(bool& advanced);

    uint32 get_miss_times();
    uint32 get_total_times() const;
    
private:
    mysql::tb_mate* data;
};

#endif // !__MATE_H__
