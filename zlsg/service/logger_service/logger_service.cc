#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "logger_service.h"

struct logger {
    FILE *   handle;
    int      close;
    char     pattern[32];
    char     name[128];
    uint32_t next_time;
    int      count;
};

static bool _change_name(struct logger * inst, time_t now, char* new_name, int len)
{
    if (now < inst->next_time) {
        return false;
    }

    struct tm mt;
    if (localtime_r(&now, &mt) == NULL) {
        return false;
    }

    snprintf(new_name, len, "./log/%s_%d_%02d_%02d_%02d.log", 
        inst->pattern, mt.tm_year+1900, mt.tm_mon+1, mt.tm_mday, mt.tm_hour);

    mt.tm_min = 59;
    mt.tm_sec = 59;
    inst->next_time = (uint32_t)mktime(&mt); // 到这一个小时的结束，需要换文件

    return true;
}

struct logger * logger_create(void) 
{
    struct logger * inst = (struct logger *)malloc(sizeof(*inst));
    inst->handle = NULL;
    inst->close = 0;
    memset(inst->pattern, 0, sizeof(inst->pattern));
    memset(inst->name, 0, sizeof(inst->name));
    inst->next_time = 0;
    inst->count = 0;
    return inst;
}

void logger_release(struct logger * inst) 
{
    amtsvr_log("\n");
    if (inst->close) {
        fflush(inst->handle);
        fclose(inst->handle);
    }

    free(inst);
}

static int _logger(struct amtsvr_context * context, void *ud, int type, int session, uint32_t source, const void * msg, size_t sz) 
{
    struct logger * inst = (struct logger *)ud;

    if (type == PTYPE_SYSTEM) {
        const char* cmd = (const char*)msg;
        if (strcmp(cmd, "write_all") == 0) {
            fflush(inst->handle);
            amtsvr_command(context, "EXIT", NULL);
            return 0;
        }
    }

    time_t now = time(NULL);
    if (_change_name(inst, now, inst->name, sizeof(inst->name))) {
        if (inst->close) {
            fflush(inst->handle);
            fclose(inst->handle);
        }

        inst->handle = fopen(inst->name, "a+");
        if (inst->handle == NULL) {
            return 0;
        }

        inst->close = 1;
    }

    char tmp_buf[64] = {0};
    struct tm tm_temp;
    if (localtime_r(&now, &tm_temp) == NULL || !strftime(tmp_buf, sizeof(tmp_buf), "%Y-%m-%d %H:%M:%S", &tm_temp)) {
        sprintf(tmp_buf, "%u", (uint32_t)now);
    }

    fprintf(inst->handle, "%x|%s|", source, tmp_buf);
    fwrite(msg, sz , 1, inst->handle);
    fprintf(inst->handle, "\n");

#if defined(__INNER_DEBUG__PACKAGE_) && defined(_WIN32)
    fflush(inst->handle);
#endif // __INNER_DEBUG__PACKAGE_ && _WIN32

    if (++inst->count >= 50 && inst->close) {
        fflush(inst->handle);
        inst->count = 0;
    }

    return 0;
}

int logger_init(struct logger * inst, struct amtsvr_context *ctx, const char * parm) 
{
    if (parm) {
        time_t now = time(NULL);
        snprintf(inst->pattern, sizeof(inst->pattern), "%s", parm);
        _change_name(inst, now, inst->name, sizeof(inst->name));
        inst->handle = fopen(inst->name, "a+");
        if (inst->handle == NULL) {
            return 1;
        }
        inst->close = 1;
    } else {
        inst->handle = stdout;
    }

    if (inst->handle) {
        amtsvr_callback(ctx, inst, _logger);
        if (parm){
            char tmp[128] = {0};
            snprintf(tmp, sizeof(tmp), ".logger_%s", parm);
            amtsvr_command(ctx, "REG", tmp);
        } else {
            amtsvr_command(ctx, "REG", ".logger");
        }
        return 0;
    }

    return 1;
}
