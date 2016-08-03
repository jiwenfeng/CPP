#include <stdio.h>
#include "cmq_svr_msg.h"

extern "C"
struct cmq_svr_msg_t* cmq_msg_create(SVR_SRCMSG_TYPE_E src_type, const char* src_name, int max_pb_count, size_t* len)
{
    *len = CMQ_SVR_MSG_SIZE(max_pb_count);
    cmq_svr_msg_t* msg = (cmq_svr_msg_t*)malloc(*len);
    msg->src_type = src_type;
    snprintf(msg->src_name, sizeof(msg->src_name), "%s", src_name);
    msg->max_pb_count = max_pb_count;
    msg->pb_count = 0;
    msg->ret = 0;
    memset(&msg->head, 0, sizeof(msg->head));
    return msg;
}

extern "C"
int cmq_msg_add(cmq_svr_msg_t* msg, google::protobuf::Message* pb, bool release)
{
    if (msg->pb_count >= msg->max_pb_count){
        return -1;
    }

    msg->pb_ctx[msg->pb_count].pb_msg = pb;
    msg->pb_ctx[msg->pb_count++].release = release;

    return 0;
}

extern "C"
void cmq_msg_set_head(cmq_svr_msg_t* msg, uint32 svr_id, uint32 user_id, uint32 role_id, uint32 client_id, const char* ip, const char* name)
{
    msg->head.svr_id = svr_id;
    msg->head.user_id = user_id;
    msg->head.role_id = role_id;
    msg->head.client_id = client_id;
    snprintf(msg->head.ip, sizeof(msg->head.ip), "%s", ip);
    snprintf(msg->head.user_name, sizeof(msg->head.user_name), "%s", name);
}

extern "C"
size_t cmq_msg_copy(cmq_svr_msg_t* src, cmq_svr_msg_t** dst)
{
    size_t len = CMQ_SVR_MSG_SIZE(src->max_pb_count);
    *(dst) = (cmq_svr_msg_t*)malloc(len);
    memcpy(*dst, src, len);
    return len;
}
