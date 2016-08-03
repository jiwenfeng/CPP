#ifndef  __SYS_MSG_H__
#define  __SYS_MSG_H__

#include "comm_define.h"
#include "client.chat_msg.pb.h"

class scheduled_msg
{
public:
    scheduled_msg() {}
    scheduled_msg(const scheduled_msg& sm) {
        this->id         = sm.id;
        this->msg_id     = sm.msg_id;
        this->type       = sm.type;
        this->start_time = sm.start_time;
        this->end_time   = sm.end_time;
        this->interval   = sm.interval;
        this->last_time  = sm.last_time;
        this->message    = sm.message;
        this->wday       = sm.wday;
    }
    ~scheduled_msg() {}

public:
    uint32 id;
    uint32 msg_id;
    uint32 type;
    uint32 start_time;
    uint32 end_time;
    uint32 interval;
    uint32 last_time;
    string message;
    int    wday;
};

class sys_msg
{
public:
    sys_msg() {}
    ~sys_msg() {}

public:
    int init();

public:
    std::vector<scheduled_msg>& get_msg() { return s; }
    
    void add_msg(const string& message, uint32 start_time, uint32 end_time, uint32 interval, uint32 msg_id = 0);
    
    void add_msg(scheduled_msg& sm);

    void update_msg(uint32 id, const string& message, uint32 start_time, uint32 end_time, uint32 interval, uint32 msg_id = 0);

    void del_msg(uint32 id);
    
    void scheduled_broadcasts(uint32 tenms, std::vector<client::chat_msg>& cms);

    void broadcast(client::client_chat_msg* cm);

private:
    void offset_time(scheduled_msg& sm, uint32 now);

private:
    static const uint32 TYPE_SYS_MSG_DAY  = 2;
    static const uint32 TYPE_SYS_MSG_WEEK = 4;

private:
    std::vector<scheduled_msg> s;
    uint32 id;
};

#endif // __SYS_MSG_H__
