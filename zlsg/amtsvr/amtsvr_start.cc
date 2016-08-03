#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_server.h"
#include "amtsvr_imp.h"
#include "amtsvr_mq.h"
#include "amtsvr_handle.h"
#include "amtsvr_module.h"
#include "amtsvr_timer.h"
#include "amtsvr_monitor.h"

#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#endif 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct monitor {
    int count;
    struct amtsvr_monitor ** m;
};

#define CHECK_ABORT if (amtsvr_context_total()==0) break;

int game_env_init(struct amtsvr_config * config);
extern bool GAME_EXIT;

#ifndef _WIN32
static void *
#else
unsigned __stdcall 
#endif //_WIN32
    _monitor(void *p) {
        struct monitor * m = (struct monitor *)p;
        int i;
        int n = m->count;
        for (;;) {
            for (i = 0; i < n; ++i) {
                amtsvr_monitor_check(m->m[i]);
            }

            CHECK_ABORT;
            usleep(6000000);
        }

        for (i = 0; i < n; ++i) {
            amtsvr_monitor_delete(m->m[i]);
        }

        free(m->m);
        free(m);

#ifndef _WIN32
        return NULL;
#else 
        return 0;
#endif // _WIN32
}

#ifndef _WIN32
static void *
#else
unsigned __stdcall 
#endif //_WIN32
    _timer(void *p) {
        for (;;) {
            amtsvr_updatetime();
            CHECK_ABORT
#ifndef _WIN32
                usleep(2500);
#else 
                sleep(3);
#endif // _WIN32
        }
#ifndef _WIN32
        return NULL;
#else 
        return 0;
#endif // _WIN32    
}

void amtsvr_over();

#ifndef _WIN32
static void *
#else
unsigned __stdcall 
#endif //_WIN32
    _worker(void *p) {
        struct amtsvr_monitor *sm = (struct amtsvr_monitor *)p;
#ifndef _WIN32
        int loop_space = 1000;
#else 
        int loop_space = 1;
#endif // _WIN32
        for (;;) {
            if (amtsvr_context_message_dispatch(sm)) {
                CHECK_ABORT
#ifndef _WIN32
                    usleep(loop_space);
#else 
                    sleep(loop_space);
#endif // _WIN32
            } 
        }
#ifndef _WIN32
        return NULL;
#else
        return 0;
#endif // _WIN32
}

#ifndef _WIN32
static void
    _start(int thread) {
        pthread_t pid[thread+2];

        struct monitor *m = (struct monitor *)malloc(sizeof(*m));
        m->count = thread;
        m->m = (struct amtsvr_monitor **)malloc(thread * sizeof(struct amtsvr_monitor *));
        int i;
        for (i = 0; i < thread; ++i) {
            m->m[i] = amtsvr_monitor_new();
        }

        pthread_create(&pid[0], NULL, _monitor, m);
        pthread_create(&pid[1], NULL, _timer, NULL);

        for (i = 0; i < thread; ++i) {
            pthread_create(&pid[i + 2], NULL, _worker, m->m[i]);
        }

        for (i = 1; i < thread + 2; ++i) {
            pthread_join(pid[i], NULL); 
        }
}
#else
static void
    _start(int thread) {
        struct monitor *m = (struct monitor *)malloc(sizeof(*m));
        m->count = thread;
        m->m = (struct amtsvr_monitor **)malloc(thread * sizeof(struct amtsvr_monitor *));
        int i;
        for (i = 0; i < thread; ++i) {
            m->m[i] = amtsvr_monitor_new();
        }

        HANDLE* hThread = new HANDLE[thread + 2];
        unsigned int* thread_id = new unsigned int[thread + 2];
        hThread[0] = (HANDLE)_beginthreadex(NULL, 0, &_monitor, m, 0, &thread_id[0]);
        hThread[1] = (HANDLE)_beginthreadex(NULL, 0, &_timer, NULL, 0, &thread_id[1]);

        for (i = 0; i < thread; ++i) {
            hThread[i + 2] = (HANDLE)_beginthreadex(NULL, 0, &_worker, m->m[i], 0, &thread_id[i + 2]);
            break;
        }

        WaitForSingleObject(hThread[0], INFINITE);

        delete []hThread;
        delete []thread_id;
}
#endif // _WIN32

void amtsvr_start(struct amtsvr_config * config) {
    amtsvr_handle_init(1);
    amtsvr_mq_init();
    amtsvr_module_init(config->module_path);
    amtsvr_timer_init();
    int i = game_env_init(config);
    if (i != 0) {
        printf("game_env_init error code %d exit\n", i);
        exit(1);
    }

    struct amtsvr_context *ctx;
    ctx = amtsvr_context_new("logger", "sys");
    if (ctx == NULL) {
        fprintf(stderr,"launch sys logger error");
        exit(1);
    }

    ctx = amtsvr_context_new("logger", "bill");
    if (ctx == NULL) {
        fprintf(stderr,"launch bill logger error");
        exit(1);
    }

    char sql_mgr_param[512] = {0};
    snprintf(sql_mgr_param, sizeof(sql_mgr_param), "%s %s %s %s %d %d",
        config->db_host, config->db_user, config->db_pass, config->db_name, config->db_port, 0); // 用于校验用户登录的存储过程调用
    ctx = amtsvr_context_new("sqlmgr", sql_mgr_param);

    for (int i = 0; i < 8; ++i) {   // 8个服务，阻塞同步用于加载用户数据
        snprintf(sql_mgr_param, sizeof(sql_mgr_param), "%s %s %s %s %d %d",
            config->db_host, config->db_user, config->db_pass, config->db_name, config->db_port, i + 1);
        ctx = amtsvr_context_new("sqlmgr", sql_mgr_param);
    }

    for (int i = 10; i < 18;  ++i) { // 8个服务，阻塞同步用于写入数据
        snprintf(sql_mgr_param, sizeof(sql_mgr_param), "%s %s %s %s %d %d",
            config->db_host, config->db_user, config->db_pass, config->db_name, config->db_port, i + 1);
        ctx = amtsvr_context_new("sqlmgr", sql_mgr_param);
    }

    ctx = amtsvr_context_new("loader", "");
    ctx = amtsvr_context_new("writer", "");

    // ctx = amtsvr_context_new("chores", "");
    snprintf(sql_mgr_param, sizeof(sql_mgr_param), "1 %d", config->max_online_user);
    ctx = amtsvr_context_new("center", sql_mgr_param);
    ctx = amtsvr_context_new("role", "");

    int client_ports[6] = {0};
    sscanf(config->client_port, "%d|%d|%d|%d|%d|%d|", 
        &client_ports[0], &client_ports[1], &client_ports[2], &client_ports[3], &client_ports[4], &client_ports[5]);
    for (int i = 0; i < 6; ++i) {
        if (client_ports[i] <= 0) {
            continue;
        }

        snprintf(sql_mgr_param, sizeof(sql_mgr_param), "0.0.0.0 %d %d 0 0", client_ports[i], config->per_gate_num);
        ctx = amtsvr_context_new("watchdog", sql_mgr_param);
    }

#ifndef _WIN32
    snprintf(sql_mgr_param, sizeof(sql_mgr_param), "0.0.0.0 %d %d 0 1", config->inner_port, config->per_gate_num);
    ctx = amtsvr_context_new("watchdog", sql_mgr_param);
#endif // _WIN32

    if (config->center_server != NULL) {
        snprintf(sql_mgr_param, sizeof(sql_mgr_param), "%s %d 1 1 1", config->center_server, config->center_server_port);
        ctx = amtsvr_context_new("node", sql_mgr_param);
    }

    _start(config->thread);

    amtsvr_over();
}

void amtsvr_over() 
{
    amtsvr_handle_retireall();
    amtsvr_timer_release();
    amtsvr_module_release();
    amtsvr_mq_release_all();
    amtsvr_handle_release();
}