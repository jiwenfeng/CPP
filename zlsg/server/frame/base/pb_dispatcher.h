#ifndef __PB_DISPATCHER_H__
#define __PB_DISPATCHER_H__

#include <iostream>
#include <map>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include "amtsvr.h"
#include "cmq_svr_msg.h"

using namespace std;

class callback
{
public:
    virtual ~callback(){};
    virtual void* on_pb_msg(google::protobuf::Message* msg, void* ud, size_t* rsp_len) const = 0;
};

template<typename T>
class callback_t : public callback
{
public:
    typedef void* (*fpb_callback)(T *msg, void* ud, size_t* rsp_len);

    callback_t(const fpb_callback& cb) : mcb(cb)
    {
    }

    virtual void* on_pb_msg(google::protobuf::Message* msg, void* ud, size_t* rsp_len) const
    {
        T* t = dynamic_cast<T*>(msg);    
        if (t != NULL){
            return mcb(t, ud, rsp_len);
        }

        return NULL;
    }

private:
    fpb_callback mcb;
};

class pb_dispatcher_s 
{
public:
    typedef void* (*default_fpb_callback)(google::protobuf::Message* msg, void* ud, size_t* rsp_len);

    pb_dispatcher_s()
    {
        default_cb = NULL;
    }

    virtual ~pb_dispatcher_s()
    {
        std::map<string, callback*>::iterator it = mapcbs.begin();
        while (it != mapcbs.end()){
            if (it->second != NULL){
                delete it->second;
            }
            mapcbs.erase(it++);
        }
    }

    void* on_pb_msg(google::protobuf::Message* msg, void* ud, size_t* rsp_len) const  
    {
        std::map<string, callback*>::const_iterator it = mapcbs.find(msg->GetTypeName());   
        if (it != mapcbs.end()){      
            return it->second->on_pb_msg(msg, ud, rsp_len);
        } else if (default_cb != NULL){
            return default_cb(msg, ud, rsp_len);
        }

        return NULL;
    }

    template<typename T>  
    void reg(const string& type_name, const typename callback_t<T>::fpb_callback& cb) 
    {   
        callback_t<T> *p = new callback_t<T>(cb);   
        mapcbs[type_name] = p;  
    }

    void reg_default(default_fpb_callback cb) 
    {    
        default_cb = cb;  
    }

public:
    std::map<string, callback*> mapcbs;
    default_fpb_callback       default_cb;
};

#define ONE_SECONDS 100 // 100¸ö10ms

class timer_tags
{
private:
    uint32 last_one_seconds;
    uint32 last_five_seconds;
    uint32 last_ten_seconds;
    uint32 last_thirty_seconds;

    uint32 last_one_minute;

public:
    timer_tags()
    {
        last_one_seconds = 0;
        last_five_seconds = 0;
        last_ten_seconds = 0;
        last_thirty_seconds = 0;
        last_one_minute = 0;
    }

    virtual ~timer_tags()
    {

    }

    inline bool judge_one_seconds(uint32 now)
    {
        if (now - last_one_seconds >= ONE_SECONDS){
            last_one_seconds = now;
            return true;
        }
        return false;
    }

    inline bool judge_five_seconds(uint32 now)
    {
        if (now - last_five_seconds >= ONE_SECONDS * 5){
            last_five_seconds = now;
            return true;
        }
        return false;
    }

    inline bool judge_ten_seconds(uint32 now)
    {
        if (now - last_ten_seconds >= ONE_SECONDS * 10){
            last_ten_seconds = now;
            return true;
        }
        return false;
    }

    inline bool judge_thirty_seconds(uint32 now)
    {
        if (now - last_thirty_seconds >= ONE_SECONDS * 30){
            last_thirty_seconds = now;
            return true;
        }
        return false;
    }

    inline bool judge_one_minute(uint32 now)
    {
        if (now - last_one_minute >= ONE_SECONDS * 60){
            last_one_minute = now;
            return true;
        }
        return false;
    }
};


#endif
