#include "operate_log.h"

operate_log::operate_log()
{
    writer_type = WOTE_INSERT;
    cd = 0;
}

operate_log::~operate_log()
{
}

void operate_log::push(server::operate_log& log)
{
    logs.push_back(log);
}

void operate_log::save(amtsvr_context* ctx)
{
    size_t size = logs.size();
    if (size == 0) {
        return ;
    }

    for (size_t i = 0; i < size; ++i) {
        if (logs[i].has_ot()) {
            save_task(ctx, logs[i].ot());
        } else if (logs[i].has_om()) {
            if (cd == logs[i].om().create_date()) {
                logs[i].mutable_om()->set_create_date(logs[i].om().create_date() + 1);
                cd = logs[i].om().create_date();
            }
            
            save_money(ctx, logs[i].om());
        }
    }

    logs.clear();
}

void operate_log::save_task(amtsvr_context* ctx, const server::operate_task& log)
{
    mysql::tb_task_log* p = new mysql::tb_task_log;
    p->set_sid(log.sid());
    p->set_role_id(log.role_id());
    p->set_task_id(log.task_id());
    p->set_status(log.status());
    p->set_quality(log.quality());
    p->set_create_date(log.create_date());
    CACHE_KEY k(p->role_id(), p->task_id());
    write(ctx, k, p);
}

void operate_log::save_money(amtsvr_context* ctx, const server::operate_money& log)
{
    mysql::tb_money_log* p = new mysql::tb_money_log;
    p->set_sid(log.sid());
    p->set_role_id(log.role_id());
    p->set_root_id(log.root_id());
    p->set_type_id(log.type_id());
    p->set_old_value(log.old_value());
    p->set_new_value(log.new_value());
    p->set_goods_id(log.goods_id());
    p->set_num(log.num());
    p->set_create_date(log.create_date());
    CACHE_KEY k(p->role_id(), p->create_date());
    write(ctx, k, p);
}


