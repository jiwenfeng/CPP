#include "mail_center.h"

mail_center::mail_center(mysql::tb_mail_center* tmc) : db_mail_center(tmc)
{
    if (db_mail_center != NULL){
        parse_mail(db_mail_center->mail());   
    }
    change = false;
}

mail_center::~mail_center()
{
    
}

void mail_center::release()
{
    if (db_mail_center != NULL){
        delete db_mail_center;
        db_mail_center = NULL;
    }
}

int mail_center::push_new_mail(uint32 role_id, const client::mail_info& mi)
{
    mail_list.push_back(mi);
    if (db_mail_center == NULL){
        db_mail_center = new mysql::tb_mail_center;
        db_mail_center->set_role_id(role_id);
        writer_type = WOTE_INSERT;
    }

    change = true;

    return 0;
}

int mail_center::save_mail(amtsvr_context* ctx)
{
    if (!change){
        return -1;
    }
    change = false;
    
    if (db_mail_center == NULL){
        return 0;
    }

    string out;
    int count = serialize_mail(out);
    if (count == 0){  
        writer_type = WOTE_REMOVE;
        mysql::tb_mail_center* tmc = new mysql::tb_mail_center(*db_mail_center);
        CACHE_KEY k(tmc->role_id(), 0);
        write(ctx, k, tmc);

        delete db_mail_center;
        db_mail_center = NULL;
        return 0;
    }

    mysql::tb_mail_center* tmc = new mysql::tb_mail_center(*db_mail_center);
    CACHE_KEY k(tmc->role_id(), 0);
    tmc->set_mail(out);
    write(ctx, k, tmc);
    writer_type = WOTE_UPDATE;
    

    return 0;
}

