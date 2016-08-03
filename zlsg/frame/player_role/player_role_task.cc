#include "player_role_task.h"

player_role_task::player_role_task()
{
    db_role_task = NULL;
}
    
player_role_task::~player_role_task()
{
    if (db_role_task != NULL) {
        delete db_role_task;
        db_role_task = NULL;
    }
}

int player_role_task::init(mysql::tb_role_task* db)
{
    if (db == NULL) {
        return -1;
    }

    db_role_task = db;
    parse_acpt_task(db_role_task->acpt_task());
    parse_comp_task(db_role_task->comp_task());
    parse_open     (db_role_task->fresh_task());


    return 0;
}

int player_role_task::save(amtsvr_context* ctx, uint32 role_id)
{
    string acpt_out = "";
    uint32 count = serialize_acpt_task(acpt_out);
    string comp_out = "";
    count += serialize_comp_task(comp_out);
    string fresh_out = "";
    count += serialize_open(fresh_out);
    if (count == 0){
        return 0;
    }
   
    if (db_role_task == NULL) {
        writer_type = WOTE_INSERT;
        db_role_task = new mysql::tb_role_task;
        db_role_task->set_role_id(role_id);
    }

    mysql::tb_role_task* trt = new mysql::tb_role_task();
    CACHE_KEY k(role_id, 0);
    trt->set_role_id(role_id);
    trt->set_acpt_task(acpt_out);
    trt->set_comp_task(comp_out);
    trt->set_fresh_task(fresh_out);
    write(ctx, k, trt);
    writer_type = WOTE_UPDATE;

    return 0;
}

int player_role_task::attach_role_task(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (i == 0) {
            mysql::tb_role_task* trt = static_cast<mysql::tb_role_task*>(pb_ctx[i].pb_msg);
            if (init(trt) != 0) {
                delete trt;
            }
        } else {
            delete pb_ctx[i].pb_msg;
        }
    }

    if (pb_count == 0 && db_role_task == NULL) {
        writer_type = WOTE_INSERT;
        db_role_task = new mysql::tb_role_task;
        init(db_role_task);
    }

    return 0;
}

