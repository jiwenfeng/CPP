#include "base_time.h"
#include "mate.h"

mate::mate()
{
    data = NULL;
}

mate::~mate()
{
    if (data != NULL){
        delete data;
        data = NULL;
    }
}

int mate::attach(mysql::tb_mate* p)
{
    if (data == p){
        return 0;
    }

    if (p == NULL){
        return 0;
    }

    if (data != NULL){
        delete data;
        data = NULL;
    }

    data = p;
    return 0;
}

int mate::save_data(amtsvr_context* ctx)
{
    if (data == NULL){
        return -1;
    }

    mysql::tb_mate* tm = new mysql::tb_mate(*data);
    CACHE_KEY k(tm->role_id(), 0);
    write(ctx, k, tm);
    writer_type = WOTE_UPDATE;

    return 0;
}

client::MATE_OPERATE_RET_E mate::fill_mate_info_rsp(client::mate_info_rsp& rsp)
{
    if (data == NULL){
        rsp.set_mate_role_id(0);
        return client::MORE_NO_MATE;
    }

    uint32 now = (uint32)time(NULL);
    rsp.set_mate_role_id(data->mate_role_id());
    rsp.set_today_times(get_can_depth_times());
    rsp.set_last_time(data->last_time());
    rsp.set_miss_times(get_miss_times());
    rsp.set_total_times(data->total_times());
    rsp.set_depth_time(data->depth_time() > 0 ? now - data->depth_time() : 0);
    rsp.set_advance_depth(data->advance_depth());
    return client::MORE_SUCCESS;
}

int mate::create(uint32 role_id, uint32 mate_id)
{
    if (data == NULL){
        data = new mysql::tb_mate;
        data->set_role_id(role_id);
        data->set_today_times(MAX_DAY_DEPTH_MATE_TIMES);
        data->set_last_time(0);
        data->set_miss_times(0);
        data->set_total_times(0);
        data->set_depth_time(0);
        data->set_advance_depth(0);
        writer_type = WOTE_INSERT;
    }
    data->set_mate_role_id(mate_id);
    
    return 0;
}

int mate::remove()
{
    if (data == NULL){
        return 0;
    }

    data->set_mate_role_id(0);
    data->set_depth_time(0);
    data->set_advance_depth(0);
    return 0;
}

uint32 mate::get_can_depth_times()
{
    if (data == NULL){
        return 0;
    }

    if (!time_utils::is_same_day(data->last_time(), (uint32)time(NULL))){
        data->set_today_times(MAX_DAY_DEPTH_MATE_TIMES);
    }

    return data->today_times();
}

int mate::depth_mate(uint32 now, bool advanced)
{
    uint32 today_times = data->today_times();
    if (today_times <= 0){
        return -1;
    }

    if (today_times > MAX_DAY_DEPTH_MATE_TIMES){
        today_times = MAX_DAY_DEPTH_MATE_TIMES;
    }

    --today_times;
    data->set_today_times(today_times);
    data->set_last_time(now);
    data->set_total_times(data->total_times()+1);
    data->set_depth_time(now);
    data->set_advance_depth(advanced ? 1 : 0);

    return 0;
}

bool mate::is_in_depth_mate()
{
    return (data != NULL && data->depth_time() > 0);
}

client::MATE_OPERATE_RET_E mate::depth_mate_end(uint32 now, bool& advanced)
{
    if (data->depth_time() == 0){
        return client::MORE_NOT_IN_DEPTH;
    }

    if (data->depth_time() + DEPTH_MATE_TIME > now){
        return client::MORE_DEPTH_TIME_LIMIT;
    }

    advanced = data->advance_depth() >= 1;
    data->set_depth_time(0);
    data->set_advance_depth(0);
    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate::depth_mate_end(bool& advanced)
{
    if (data->depth_time() == 0){
        return client::MORE_NOT_IN_DEPTH;
    }

    advanced = data->advance_depth() >= 1;
    data->set_depth_time(0);
    data->set_advance_depth(0);
    return client::MORE_SUCCESS;
}

uint32 mate::get_miss_times()
{
    if (data == NULL){
        return 0;
    }

    return data->miss_times();
}

uint32 mate::get_total_times() const
{
    if (data == NULL) {
        return 0;
    }

    return data->total_times();
}
