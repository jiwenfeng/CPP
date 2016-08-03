#include "amtsvr_error.h"
#include "sql_operator.h"

int database::open()
{
    close();

    mmysql = mysql_init( NULL );
    if (mmysql == NULL) {
        return -1;
    }

    MYSQL* p = mysql_real_connect( mmysql, 
        mhost.c_str(), muser.c_str(), mpass.c_str(), mdb.c_str(), mport, 
        NULL, CLIENT_MULTI_RESULTS );
    if (p == NULL || mysql_autocommit(p, 1) != 0) {
        return -2;
    }

    mysql_query(mmysql, "set character set 'utf8'");

    return 0;
}

void database::close()
{
    if( mmysql != NULL ) {
        mysql_close( mmysql );
        mmysql = NULL;
    }
}

bool database::is_open()
{
    return (mmysql != NULL && mysql_ping(mmysql) == 0);
}

bool database::check()
{
    return (is_open() ? true : open() == 0);
}

record_set* database::query(const char* sql, my_ulonglong& rows, int iCommit)
{
    record_set* rst = NULL;
    //printf("sql = %s\n", sql);

    if (mysql_query(mmysql, sql) == 0) {
        MYSQL_RES* res = mysql_store_result(mmysql);
        while(mysql_next_result(mmysql) == 0) {
            ;
        }

        if (res != NULL) {
            rows = mysql_num_rows(res);
            if(rows > 0) {
                rst = new record_set(res);
            }else if(res != NULL ) {
                mysql_free_result(res);
            }

            if (iCommit) {
                mysql_commit(mmysql);
            }
        }
    } else {
        const char* err = mysql_error(mmysql);
        amtsvr_log("%s\n%s\n", sql, err);
        /*bool b = */check();
    }

    return rst;
}

uint32 database::get_errno()
{
    return mysql_errno(mmysql);
}
