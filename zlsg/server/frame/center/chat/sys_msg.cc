#include "sys_msg.h"

int sys_msg::init()
{
//     id = 0;
//     uint32 now = (uint32)time(NULL);
//     const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = CONFMGR->get_sys_message_tpl_head();
//     while (cur != NULL){
//         const cfg_cache_obj& cco = cur->get_val();
//         const mysql::tb_sys_message_tpl* p = static_cast<const mysql::tb_sys_message_tpl*>(cco.get_msg());
//         cur = cur->get_next();
//         if (p == NULL) {
//             continue;
//         }
// 
//         scheduled_msg sm;
//         base_time et(p->end_date().c_str(), p->end_date().length());
//         base_time st(p->start_date().c_str(), p->end_date().length());
//         time_span ts(now);
//         sm.id         = ++id;
//         sm.msg_id     = p->id();
//         sm.type       = p->message_type();
//         sm.start_time = st.get_time();
//         sm.end_time   = et.get_time();
//         sm.interval   = p->interval() * 100;
//         sm.last_time  = 0;
//         sm.message    = p->content();
// 
//         if (sm.type == TYPE_SYS_MSG_DAY) {
//             //printf("\n-----\n");
//             //printf("s = %d, e = %d\n", sm.start_time, sm.end_time);
//             sm.start_time += ((now / 86400) * 86400 - 8 * 3600);
//             sm.end_time   += ((now / 86400) * 86400 - 8 * 3600);
//             //printf("s = %d, e = %d\n", sm.start_time, sm.end_time);
//             offset_time(sm, now);
//             //printf("s = %d, e = %d\n", sm.start_time, sm.end_time);
//             //printf("-----\n");
//         } else if (sm.type == TYPE_SYS_MSG_WEEK) {
//             sm.start_time += ((now / 86400) * 86400 - 8 * 3600);
//             sm.end_time   += ((now / 86400) * 86400 - 8 * 3600);
//             sm.wday = st.get_wday();
//             time_t timep = now;
//             struct tm result;
//             if (localtime_r(&timep, &result) != NULL) {
//                 if (result.tm_wday > sm.wday) {
//                     sm.start_time -= ((result.tm_wday - sm.wday) * 86400);
//                     sm.end_time   -= ((result.tm_wday - sm.wday) * 86400);
//                 } else {
//                     sm.start_time += ((sm.wday - result.tm_wday) * 86400);
//                     sm.end_time   += ((sm.wday - result.tm_wday) * 86400);
//                 }
// 
//                 offset_time(sm, now);
//             } else {
//                 sm.end_time = 0;
//             }
//         }
// 
//         if (sm.end_time < now) {
//             continue;
//         }
// 
//         s.push_back(sm);
//     }
// 
    return 0;
}

void sys_msg::add_msg(const string& message, uint32 start_time, uint32 end_time, uint32 interval, uint32 msg_id/* = 0*/)
{
    scheduled_msg sm;
    sm.msg_id     = msg_id;
    sm.start_time = start_time;
    sm.end_time   = end_time;
    sm.interval   = interval * 100;
    sm.last_time  = 0;
    sm.message    = message;
    add_msg(sm);
}
    
void sys_msg::add_msg(scheduled_msg& sm)
{
    sm.id = ++id;
    s.push_back(sm);
}

struct equal_msg {
    uint32 _id;
    equal_msg(uint32 id) : _id(id) {
    }

    bool operator() (const scheduled_msg& sm) {
        return _id == sm.id;
    }
};

void sys_msg::update_msg(uint32 id, const string& message, uint32 start_time, uint32 end_time, uint32 interval, uint32 msg_id/* = 0*/)
{
    std::vector<scheduled_msg>::iterator i = std::find_if(s.begin(), s.end(), equal_msg(id));
    if (i == s.end()) {
        add_msg(message, start_time, end_time, interval, msg_id);
        return ;
    }

    i->message    = message;
    i->start_time = start_time;
    i->end_time   = end_time;
    i->interval   = interval;
}

void sys_msg::del_msg(uint32 id)
{
    std::vector<scheduled_msg>::iterator i = std::find_if(s.begin(), s.end(), equal_msg(id));
    if (i != s.end()) {
        s.erase(i);
    }
}

void sys_msg::scheduled_broadcasts(uint32 tenms, std::vector<client::chat_msg>& cms)
{
    size_t size = s.size();
    uint32 now = (uint32)time(NULL);
    for (size_t i = 0; i < size; ++i) {
        offset_time(s[i], now);
        if (now < s[i].start_time || now > s[i].end_time) {
            continue;
        }
        
        if (tenms - s[i].last_time < s[i].interval) {
            continue;
        }

        s[i].last_time = tenms;
        client::chat_msg cm;
        cm.set_channel(client::chat_msg_CHANNEL_SYSTEM);
        cm.set_src_rold_id(s[i].msg_id);
        cm.set_content(s[i].message);
        cms.push_back(cm);
    }
}

void sys_msg::offset_time(scheduled_msg& sm, uint32 now)
{
    if (sm.type == TYPE_SYS_MSG_DAY) {
        if (time_utils::is_same_day(sm.start_time, now)) {
            return ;
        }

        sm.start_time += 86400;
        sm.end_time   += 86400;
    } else if (sm.type == TYPE_SYS_MSG_WEEK) {
        if (time_utils::is_same_week(sm.start_time, now)) {
            return ;
        }

        sm.start_time += (86400 * 7);
        sm.end_time   += (86400 * 7);
    }
}
