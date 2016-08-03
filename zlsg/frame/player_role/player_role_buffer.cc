#include "player_role_buffer.h"

player_role_buffer::player_role_buffer(mysql::tb_role_buffer* b) : db_role_buffer(b)
{

}

player_role_buffer::~player_role_buffer()
{
    if (db_role_buffer != NULL) {
        delete db_role_buffer;
        db_role_buffer = NULL;
    }
}

int player_role_buffer::set_db_buffer(mysql::tb_role_buffer* b)
{
    if (b == NULL) {
        return -1;
    }

    if (db_role_buffer == b) {
        return 0;
    }

    if (db_role_buffer != NULL) {
        delete db_role_buffer;
    }

    db_role_buffer = b;
    parse_buffer(db_role_buffer->buffers());

    return 0;
}

int player_role_buffer::save(uint32 role_id, amtsvr_context* ctx)
{
    string out;
    int count = serialize_buffer(out);
    if (count <= 0) {
        if (db_role_buffer == NULL) {
            return 0;
        }
        writer_type = WOTE_REMOVE;
    } else {
        if (db_role_buffer == NULL) {
            db_role_buffer = new mysql::tb_role_buffer;
            db_role_buffer->set_role_id(role_id);
            writer_type = WOTE_INSERT;
        } else {
            writer_type = WOTE_UPDATE;
        }
        db_role_buffer->set_buffers(out);
    }

    mysql::tb_role_buffer* trb = new mysql::tb_role_buffer(*db_role_buffer);
    CACHE_KEY k(trb->role_id(), 0);
    write(ctx, k, trb);

    return 0;
}

int player_role_buffer::attach_role_buffer(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    if (pb_count <= 0){
        return 0;
    }

    for (int i = 0; i < pb_count; ++i){
        if (i == 0){
            set_db_buffer(static_cast<mysql::tb_role_buffer*>(pb_ctx[i].pb_msg));
        } else {
            delete pb_ctx[i].pb_msg;
        }
    }
    return 0;
}
