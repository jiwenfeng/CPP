#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_handle.h"
#include "amtsvr_mq.h"
#include "amtsvr_server.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LOG_MESSAGE_SIZE 1024

void amtsvr_syslog(struct amtsvr_context * context, const char *msg, ...) 
{
    static int logger = amtsvr_handle_findname("logger_sys");
    if (logger < 0) {
        return;
    }

    char tmp[LOG_MESSAGE_SIZE];

    va_list ap;
    va_start(ap,msg);
    int len = vsnprintf(tmp, LOG_MESSAGE_SIZE, msg, ap);
    va_end(ap);

    if (len >= LOG_MESSAGE_SIZE) {
        len = LOG_MESSAGE_SIZE - 1;
        tmp[len] = '\0';
    }

    struct amtsvr_message smsg;
    if (context == NULL) {
        smsg.source = 0;
    } else {
        smsg.source = amtsvr_context_handle(context);
    }

    smsg.session = 0;
    smsg.data = strdup(tmp);
    smsg.sz = len | (PTYPE_TEXT << HANDLE_REMOTE_SHIFT);
    if (amtsvr_context_push(logger, &smsg) != 0) {
        free(smsg.data);
    }
}

void amtsvr_billlog(struct amtsvr_context * context, const char *msg, ...) 
{
    static int logger = amtsvr_handle_findname("logger_bill");
    if (logger < 0) {
        return;
    }

    char tmp[LOG_MESSAGE_SIZE];

    va_list ap;
    va_start(ap,msg);
    int len = vsnprintf(tmp, LOG_MESSAGE_SIZE, msg, ap);
    va_end(ap);

    if (len >= LOG_MESSAGE_SIZE) {
        len = LOG_MESSAGE_SIZE - 1;
        tmp[len] = '\0';
    }

    struct amtsvr_message smsg;
    if (context == NULL) {
        smsg.source = 0;
    } else {
        smsg.source = amtsvr_context_handle(context);
    }

    smsg.session = 0;
    smsg.data = strdup(tmp);
    smsg.sz = len | (PTYPE_TEXT << HANDLE_REMOTE_SHIFT);
    amtsvr_context_push(logger, &smsg);
}


