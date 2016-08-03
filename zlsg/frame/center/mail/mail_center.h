#ifndef __MAIL_CENTER_H__
#define __MAIL_CENTER_H__

#include "define.h"
#include "pb_mgr.h"
#include "mail_parser.h"

enum CENTER_MAIL_OPERATE_FLAG_E
{
    CMOFE_NULL  = 0,
    CMOFE_ADD   = 1,
    CMOFE_MODIFY = 2,
    CMOFE_DEL    = 3,
};


class mail_center : public mail_parser, public pb_writer
{
public:
    mail_center(mysql::tb_mail_center* tmc = NULL);

    virtual ~mail_center();

    void release();

    inline int attach(mysql::tb_mail_center* tmc)
    {
        if (db_mail_center == tmc){
            return -1;
        }

        if (db_mail_center != NULL){
            delete db_mail_center;
        }

        db_mail_center = tmc;

		parse_mail(db_mail_center->mail());

        return 0;
    }

    int push_new_mail(uint32 role_id, const client::mail_info& mi);

    inline vector<role_mail_info>& mut_mails(){return mail_list;}

    inline void set_change(bool c = true){change = c;}

    int save_mail(amtsvr_context* ctx);

    inline uint32 get_role_id()
    {
        if (db_mail_center == NULL){
            return 0;
        }
        return db_mail_center->role_id();
    }

protected:
    mysql::tb_mail_center* db_mail_center;
    bool change;
};

#endif
