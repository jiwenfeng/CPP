#include <algorithm>
#include "msg_handler_pool.h"

msg_handler_pool::msg_handler_pool()
{
    /*lock = 0;
    cap = 0;
    num = 0;
    head = NULL;

    init();*/
}

msg_handler_pool::~msg_handler_pool()
{
    /*struct msg_handler* cur = head;
    while (cur != NULL){
        head = cur->next;
        delete cur;
        cur = head;
    }*/

#ifdef __INNER_DEBUG__PACKAGE_
    //for (size_t i = 0; i < _msgs.size(); ++i) {
    //    if (_msgs[i]->msg != NULL) {
    //        delete _msgs[i]->msg;
    //        _msgs[i]->msg = NULL;
    //    }
    //    free(_msgs[i]);
    //}
    //_msgs.clear();
#endif // __INNER_DEBUG__PACKAGE_
}

/*
int msg_handler_pool::init()
{
    return alloc();
}*/

int msg_handler_pool::push(struct msg_handler* mh)
{
#ifdef __INNER_DEBUG__PACKAGE_
//     std::vector<struct msg_handler*>::iterator i = std::find(_msgs.begin(), _msgs.end(), mh);
//     if (i != _msgs.end()) {    
//         _msgs.erase(i);
//     }
#endif // __INNER_DEBUG__PACKAGE_

    if (mh == NULL) {
        return -1;
    }

    free(mh);

    /*while (__sync_lock_test_and_set(&lock,1)) {}

    mh->msg = NULL;
    mh->next = head;
    head = mh;
    --num;

    __sync_lock_release(&lock);
*/

    return 0;
}

struct msg_handler* msg_handler_pool::pop()
{
// #ifdef __INNER_DEBUG__PACKAGE_
//     struct msg_handler* p = (struct msg_handler*)malloc(sizeof(struct msg_handler));
//     //_msgs.push_back(p);
//     return p;
// #else
    return (struct msg_handler*)malloc(sizeof(struct msg_handler));
// #endif // __INNER_DEBUG__PACKAGE_

    /*struct msg_handler* mh = NULL;
    
    while (__sync_lock_test_and_set(&lock,1)) {}

    do {
        if (num >= cap){
            alloc();
        }

        if (num >= cap){
            break;
        }

        mh = head;
        if (mh == NULL){
            break;
        }

        head = head->next;
        mh->next = NULL;
        ++num;

    } while (false);

    __sync_lock_release(&lock);

    return mh;*/
}

/*
int msg_handler_pool::alloc()
{
    uint32 new_size = 0;
    if (cap == 0){
        new_size = START_MSG_HANDLER_NUM;
    } else {
        new_size = ADD_MSG_HANDLER_PER_NUM;

    }
   
    uint32 count = 0;
    for (uint32 i = 0; i < new_size; ++i){
        struct msg_handler* new_mh = new msg_handler;
        if (new_mh == NULL){
            break;
        }

        ++count;
        new_mh->next = head;
        head = new_mh;
    }

    cap += count;

    return 0;
}*/
