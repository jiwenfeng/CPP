#ifndef __MAIL_CENTER_MGR_H__
#define __MAIL_CENTER_MGR_H__

#include "mail_center.h"

class mail_center_mgr
{
public:
    mail_center_mgr();

    virtual ~mail_center_mgr();

    int push_new_mail(uint32 role_id, const client::mail_info& mi);

    mail_center* mutable_mail(uint32 role_id);

    int save_mail(amtsvr_context* ctx);

    int init(struct cmq_t* l);

private:
    map<uint32, mail_center> center;

    vector<uint32> changes;
};

#endif
