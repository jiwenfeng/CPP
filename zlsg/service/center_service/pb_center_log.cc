#include "mysql.t_upgradelog.pb.h"
#include "mysql.t_newtask.pb.h"
#include "mysql.t_login.pb.h"
#include "mysql.t_offline.pb.h"
#include "mysql.t_online.pb.h"
#include "mysql.t_recharge.pb.h"
#include "mysql.t_moneylog.pb.h"
#include "db_log.h"
#include "center_interface.h"

void* on_pb_center_t_upgradelog(mysql::t_upgradelog* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_newtask(mysql::t_newtask* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_login(mysql::t_login* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_offline(mysql::t_offline* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_online_data(mysql::t_online* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_recharge(mysql::t_recharge* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}

void* on_pb_center_t_moneylog(mysql::t_moneylog* req, void* ud, size_t* rsp_len)
{
    center_service_t* service = (center_service_t*)ud;

    service->_log->push(req);

    return NULL;
}