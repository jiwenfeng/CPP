#include "rank_center.h"
#if 0
rank_center::rank_center(mysql::tb_rank_center* trc)
    : db_rank_center(trc)
    , change(false)
{
    writer_type = WOTE_UPDATE;
}

rank_center::~rank_center()
{
    if (db_rank_center != NULL) {
        delete db_rank_center;
        db_rank_center = NULL;
    }
}

int rank_center::set(uint32 index, uint32 type, uint64 id, uint32 val)
{
    if (db_rank_center == NULL) {
        db_rank_center = new mysql::tb_rank_center;
        writer_type = WOTE_INSERT;
    } else if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    if (index == db_rank_center->index() && type == db_rank_center->type() && id == db_rank_center->id() && val == db_rank_center->val()) {
        return -1;
    }

    change = true;
    db_rank_center->set_index(index);
    db_rank_center->set_type(type);
    db_rank_center->set_id((uint32)id);
    db_rank_center->set_pet_id(0);
    db_rank_center->set_val(val);
    //if (type == RANK_PET_POWER || type == RANK_PET_CLASS || type == RANK_PET_QUALITY) {
    //    db_rank_center->set_id(id >> 32);
    //    db_rank_center->set_pet_id(id & 0x00000000FFFFFFFF);
    //}
    
    return 0;
}

mysql::tb_rank_center* rank_center::get()
{
    return this->db_rank_center;
}

int rank_center::save(amtsvr_context* ctx)
{
    if (!change || db_rank_center == NULL) {
        return -1;
    }

    mysql::tb_rank_center* trc = new mysql::tb_rank_center(*db_rank_center);
    CACHE_KEY k(trc->index(), 0);
    if (writer_type != WOTE_REMOVE){
    }

    //printf("%d,%d,%d,%d\n", trc->index(), trc->type(), trc->id(), trc->val());
    
    write(ctx, k, trc);
    change = false;
    writer_type = WOTE_NULL;
    
    return 0;
}
#endif
