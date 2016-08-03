#ifndef AMTSVR_IMP_H
#define AMTSVR_IMP_H

struct amtsvr_config {
    int          thread;
    const char * logger;
    const char * module_path;
    const char * db_host;
    int          db_port;
    const char * db_name;
    const char * db_user;
    const char * db_pass;
    const char * client_port;
    int          per_gate_num;
    int          inner_port;
    int          max_online_user;
    unsigned int start_svr_time;
    const char * center_server;
    int          center_server_port;
};

void amtsvr_start(struct amtsvr_config * config);

#endif
