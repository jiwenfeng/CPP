#include "center_role.h"

center_role::center_role()
{

}

center_role::center_role(const center_role* cr)
{
    if (cr != NULL && cr->crs.get_tb_role_set() != NULL) {
        mysql::tb_role_center_set* set = crs.create();
        set->CopyFrom(*(cr->crs.get_tb_role_set()));
        crs.set_writer_type(cr->crs.get_writer_type());
    }
}

center_role::~center_role()
{
}

int center_role::attach_role_set(mysql::tb_role_center_set* trs)
{
    return crs.attach(trs);
}

void center_role::write(amtsvr_context* ctx)
{
    crs.write(ctx);
}
