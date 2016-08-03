#ifndef __MSG_HANDLER_POOL_H__
#define __MSG_HANDLER_POOL_H__

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include "define.h"

struct msg_handler
{
    google::protobuf::Message* msg;
    uint32                     flag;
    uint32                     ext1;
    uint32                     ext2;

    struct msg_handler*        next;

    msg_handler() {
        msg = NULL;
        flag = 0;
        ext1 = 0;
        ext2 = 0;
        next = NULL;
    }

    ~msg_handler() {
        /*����Ӧ����Ϣ�Ľ��մ��Ѿ�����msg���ͷ� 
        if (msg != NULL) {
        delete msg;
        msg = NULL;
        }*/
    }
};

// #define START_MSG_HANDLER_NUM 100000
// #define ADD_MSG_HANDLER_PER_NUM 1000

// ������amtsvr��������ȶ���פ��ģ����ceneter\game\role\writerģ�鷢����Ϣ�ÿ����������Ϣ������push��pop
class msg_handler_pool
{
public:
    msg_handler_pool();

    virtual ~msg_handler_pool();

    int push(struct msg_handler* mh);

    struct msg_handler* pop();

private:
    /*int alloc();

    int init();*/

private:
    /*int                 lock;

    uint32              cap;
    uint32              num;              
    struct msg_handler* head;*/
#ifdef __INNER_DEBUG__PACKAGE_
    std::vector<struct msg_handler*> _msgs;
#endif // __INNER_DEBUG__PACKAGE_
};

extern msg_handler_pool* MSG_HANDLER_POOL;

#endif
