#include <stdio.h>
#include <stdarg.h>

#include "sql_mgr.h"
#include "sql_operator.h"
#include "sql_parser.h"
#include "pb_mgr.h"
#include "msg_handler_pool.h"

struct sqlmgr_t
{
    database  db;
    sql_parser parser;
};

static void _parm(char *msg, int sz, int command_sz) 
{
	while (command_sz < sz) {
		if (msg[command_sz] != ' ')
			break;
		++command_sz;
	}
	int i;
	for (i=command_sz;i<sz;i++) {
		msg[i-command_sz] = msg[i];
	}
	msg[i-command_sz] = '\0';
}

static void sqlmgr_cmd_call(struct amtsvr_context* ctx, struct sqlmgr_t* s, uint32_t dst, int session, char* modname, char* sql)
{
    google::protobuf::Message* msg = s->parser.get(modname, sql, s->db);
    if (msg == NULL) {
        //printf("sqlmgr_cmd_call failed:%s\n", sql);
        return;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete msg;
        return;
    }

    mh->msg = msg;
    amtsvr_send(ctx, 0, dst, PTYPE_SYSTEM | PTYPE_TAG_DONTCOPY, session, mh, sizeof(msg_handler));
}

static void sqlmgr_cmd_calls(struct amtsvr_context* ctx, struct sqlmgr_t* s, uint32_t dst, int session, char* modname, char* sql)
{
    size_t rsp_len = 0;
    struct cmq_svr_msg_t* rsp = s->parser.gets(modname, sql, s->db, rsp_len);
    amtsvr_send(ctx, 0, dst, PTYPE_RESPONSE | PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
}

static void sqlmgr_cmd_get(struct amtsvr_context* ctx, struct sqlmgr_t* s, uint32_t dst, int session, char* modname, ...)
{
    const char* sqlformat = PBMGR->get_querysql(modname);
    if (sqlformat == NULL || strlen(sqlformat) == 0) {
        return;
    }
    
    char sql[4096] = {0}; 
    va_list ap;
    va_start(ap, modname);
    vsnprintf(sql, sizeof(sql), sqlformat, ap);
    va_end(ap);

    size_t rsp_len = 0;
    struct cmq_svr_msg_t* rsp = s->parser.gets(modname, sql, s->db, rsp_len);
    /*if (rsp->ret != 0){
        printf("sqlmgr_cmd_get failed ret=%d, sql=%s\n", rsp->ret, sql);
    }*/
    amtsvr_send(ctx, 0, dst, PTYPE_RESPONSE | PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
}

static void sqlmgr_cmd_set(struct amtsvr_context* ctx, struct sqlmgr_t* s, uint32_t dst, int session, char* sql)
{
    int ret = 0;
    if (s->parser.set(sql, s->db, ret) != 0 && ret != 0) {
        amtsvr_syslog(ctx, "s->parser.set failed, ret=%d, sql=%s", ret, sql);
        amtsvr_log("s->parser.set failed, ret=%d, sql=%s", ret, sql);
        return ;
    }/*else{
        printf("%s\n", sql);
    }*/
}

static void sqlmgr_cmd_push(struct amtsvr_context* ctx, struct sqlmgr_t* s, uint32_t dst, int session, char* modname, char* sql)
{
    int ret = 0;
    if (s->parser.set(sql, s->db, ret) != 0 && ret != 0) {
        amtsvr_syslog(ctx, "s->parser.set failed, ret=%d, sql=%s", ret, sql);
        amtsvr_log("s->parser.set failed, ret=%d, sql=%s", ret, sql);
        return;
    }/*else{
        printf("%s\n", sql);
    }*/
}

static void sqlmgr_cmd(struct amtsvr_context* ctx, struct sqlmgr_t* s, int type, int session, uint32_t source, const void * msg, size_t sz)
{
    if (sz == 0) {
		return;
    }

    uint32_t dst = source;
    char* tmp = (char*)malloc(sizeof(char) * (sz + 1));
	memcpy(tmp, msg, sz);
	tmp[sz] = '\0';
	char* command = tmp;
	size_t i = 0;

	for (i = 0; i < sz; ++i) {
		if (command[i]==' ') {
			break;
		}
	}

    do {
    // get pbmoudle_name key1 key2
	if (memcmp(command,"get",i)==0) {
        _parm(tmp, sz, i);
        char* k2 = tmp;
        char* modname = strsep(&k2, " ");
		if (modname == NULL) {
			break;
		}
        char* k1 = strsep(&k2, " ");
        sqlmgr_cmd_get(ctx, s, dst, session, modname, k1, k2);
        break;
	}

    // set pbmodule_name sql
    if (memcmp(command,"set",i)==0) {
        _parm(tmp, sz, i);
        char* sql = tmp;
        strsep(&sql, " ");
        sqlmgr_cmd_set(ctx, s, dst, session, sql);
        break;
    }

    // call pbmodule_name sql
    if (memcmp(command,"call",i)==0) {
        _parm(tmp, sz, i);
        char* sql = tmp;
        char* modname = strsep(&sql, " ");
        if (modname == NULL) {
            break;
        }
        sqlmgr_cmd_call(ctx, s, dst, session, modname, sql);
        break;
    }

    if (memcmp(command, "calls", i) == 0) {
        _parm(tmp, sz, i);
        char* sql = tmp;
        char* modname = strsep(&sql, " ");
        if (modname == NULL) {
            break;
        }
        sqlmgr_cmd_calls(ctx, s, dst, session, modname, sql);
        break;
    }

    if (memcmp(command, "push", i) == 0) {
        _parm(tmp, sz, i);
        char* sql = tmp;
        char* modname = strsep(&sql, " ");
        if (modname == NULL) {
            break;
        }
        sqlmgr_cmd_push(ctx, s, dst, session, modname, sql);
        break;
    }

    if (memcmp(command, "close", i) == 0) {
        amtsvr_syslog(ctx, "sqlmgr_service_%d|will exit now|", session);
        amtsvr_command(ctx, "EXIT", NULL);
        break;
    }
    } while (0);

    free(tmp);
}

static int sqlmgr_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void * msg, size_t sz)
{
    struct sqlmgr_t* s = (struct sqlmgr_t*)ud;
    if (type == PTYPE_CLIENT) {
        sqlmgr_cmd(ctx, s, type, session, source, msg, sz);
        return 0;
    }

    // ping mysql with keep connection
    s->db.check();
    amtsvr_command(ctx, "TIMEOUT", SQLMGR_PING_TIME);

    return 0;
}

struct sqlmgr_t* sqlmgr_create(void)
{
    struct sqlmgr_t* s = new sqlmgr_t;
    if (s == NULL) {
        return NULL;
    }

    // other init

    return s;
}

// "127.0.0.1 root 1 WJYX 3306"
int sqlmgr_init(struct sqlmgr_t* s, struct amtsvr_context* ctx, char* args)
{
    int sz = strlen(args)+1;
    char* host    = (char*)malloc(sizeof(char) * sz);
    char* user    = (char*)malloc(sizeof(char) * sz);
    char* pass    = (char*)malloc(sizeof(char) * sz);
    char* db_name = (char*)malloc(sizeof(char) * sz);
    int ret = 0;

    do {
    int  port;
    int  index;
    int n = sscanf(args, "%s %s %s %s %d %d", host, user, pass, db_name, &port, &index);
    if (n < 5) {
        amtsvr_syslog(ctx, "Invalid sqlmgr args %s", args);
        ret = 1;
		break;
    }

    s->db.set_param(host, user, pass, db_name, (short)port);
    if (s->db.open() != 0) {
        amtsvr_syslog(ctx, "open sql failed, check mysql status! sqlmgr args %s", args);
		ret = 1;
        break;
    }

    amtsvr_callback(ctx, s, sqlmgr_cb);
    amtsvr_command(ctx, "TIMEOUT", SQLMGR_PING_TIME);
    if (index == 0) {
        amtsvr_command(ctx, "REG", SQLMGR_NAME);
    } else {
        char tmp_buf[64] = {0};
        snprintf(tmp_buf, sizeof(tmp_buf), "%s_%d", SQLMGR_NAME, index-1);
        amtsvr_command(ctx, "REG", tmp_buf);
    }
    } while (0);

    free(host    );
    free(user    );
    free(pass    );
    free(db_name );

    return ret;
}

void sqlmgr_release(struct sqlmgr_t* s)
{
    static int i = 0;
    amtsvr_log("%d\n", i++);

    if (s != NULL) {
        delete s;
        s = NULL;
    }
}

