#ifndef __CMQ_SVR_MSG__
#define __CMQ_SVR_MSG__

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <string> 
#include <stdio.h>

#include "define.h"

#define MAX_PB_MSG_NUM 10  // 一次最多传递PB消息数目

struct cmq_svr_msg_head_t
{
    uint32 svr_id;
    uint32 user_id;
    uint32 role_id;
    uint32 agent_id;
    uint32 client_id;
    uint32 scene_service_id;
    char   ip[MAX_IP_LEN];
    char   user_name[MAX_USER_NAME_LEN];
};

struct cmq_svr_pb_ctx
{
    bool release;   // 是否释放
    google::protobuf::Message* pb_msg;
};

struct cmq_svr_msg_t
{
    SVR_SRCMSG_TYPE_E src_type;
    char              src_name[MAX_CMQ_SVR_NAME_LEN];
    uint32            ret;

    cmq_svr_msg_head_t head;
    
    int max_pb_count;
    int pb_count;

    cmq_svr_pb_ctx pb_ctx[1];
};

#define CMQ_SVR_MSG_SIZE(n) (sizeof(cmq_svr_msg_t) + sizeof(cmq_svr_pb_ctx)*(n))

extern "C"
struct cmq_svr_msg_t* cmq_msg_create(SVR_SRCMSG_TYPE_E src_type, const char* src_name, int max_pb_count, size_t* len);

extern "C"
int cmq_msg_add(cmq_svr_msg_t* msg, google::protobuf::Message* pb, bool release);

extern "C"
void cmq_msg_set_head(cmq_svr_msg_t* msg, uint32 svr_id, uint32 user_id, uint32 role_id, uint32 client_id, const char* ip, const char* name);

extern "C"
size_t cmq_msg_copy(cmq_svr_msg_t* src, cmq_svr_msg_t** dst);

#endif
