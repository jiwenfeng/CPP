#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

#include <stdint.h>
#include <iostream>
#include <sstream>
#include "define.h"

#define MAX_BROADCAST_AGENT_NUM 5000 // 最大只能广播给5000在线用户

#define MAX_PB_MOD_NAME_LEN 128
#define MAX_IP_LEN 32
#define MAX_NAME_LEN 19
#define MIN_NAME_LEN 3
#define MAX_USER_NAME_LEN 64
#define MAX_FILE_PATH 256
#define MAX_PB_NAME_LEN 64

typedef std::pair<uint32,uint32> CACHE_KEY;  // 项目中目前仅支持最大双key，有需要的话，后面再扩展
#define MAX_CACHE_KEY_NUM  2
#define UINT64_MAKE(high, low)   ((uint64)(((unsigned int)((low) & 0xFFFFFFFF)) | ((uint64)((unsigned int)((high) & 0xFFFFFFFF))) << 32))
#define UINT64_LOW(i)            ((unsigned int)((uint64)(i) & 0xFFFFFFFF))
#define UINT64_HIGH(i)           ((unsigned int)((uint64)(i) >> 32))

#define UINT32_MAKE(high, low) ((uint32)(((unsigned int)((low) & 0xFFFF)) | ((uint32)((unsigned int)((high) & 0xFFFF))) << 16))
#define UINT32_LOW(i)         ((unsigned int)((uint32)(i) & 0xFFFF))
#define UINT32_HIGH(i)        ((unsigned int)((uint32)(i) >> 16)) 

#define SVR_ID_MASK  0xFFF  // 高1.4个字节为服务器ID,一个平台下最大支持开4095个服，如果4095已满，在该平台下再开新的大区，大区之间不支持合服
#define ROLE_ID_MASK 0xFFFFF    // 低3.8个字节为当前服务器下的用户ID 最大支持100万个ID
#define GET_SVR_ID(id) (((id>>20)&SVR_ID_MASK)) // 根据ID获取服务器ID
#define GET_ROLE_ID(id) ((id)&ROLE_ID_MASK)     // 根据ID获取角色ID
#define PACK_SVR_ROLE_ID(svr_id,role_id) (((svr_id)<<20) | ((role_id)&ROLE_ID_MASK)) // 根据服务器ID 角色ID 组装成正常ID
#define FAMILY_ID_MASK 0xFFFFF
#define GET_FAMILY_ID(id) ((id)&FAMILY_ID_MASK)
#define PACK_SVR_FAMILY_ID(svr_id,f_id) (((svr_id)<<20) | ((f_id)&FAMILY_ID_MASK)) 

#define ONE_DAY_SECONDS 86400

#define SHUTDOWN_CMD  "shutdown " // game主动断开
#define CLOSE_ALL     "close_all" // 关闭掉所有agent
#define LOSE_CONN     "lose_conn" // 客户端失去连接

extern bool GAME_EXIT;

struct attr_time 
{
    attr_time()
    {
        memset(this, 0, sizeof(attr_time));
    }

    attr_time operator=(const attr_time& rhs)
    {
        day = rhs.day;
        hour = rhs.hour;
        min = rhs.min;
        second = rhs.second;
        duration = rhs.duration;
        act_flag = rhs.act_flag;
        start_time = rhs.start_time;
        end_time = rhs.end_time;
        return *this;
    }

	uint8  day;     
	uint8  hour;    
	uint8  min;
	uint8  second;
	uint32 duration;	
	uint8  act_flag;
	uint32 start_time;
	uint32 end_time;
};

#define MAX_CMQ_SVR_NAME_LEN 32

enum SVR_SRCMSG_TYPE_E
{
    SSTE_AGENT  = 0, 
    SSTE_AUTH   = 1,
    SSTE_SQLMGR = 2,
    SSTE_WORLD  = 3,
    SSTE_SCENE  = 4,
    SSTE_CLIENT = 5, // 透传的用户的PB消息，需要校验，其余类型均为服务器发送，可信任消息
    SSTE_WATCHDOG = 6, 
    SSTE_LOADER = 7,
    SSTE_MAX
};

#define AGENT_NAME    "agent"
#define ROLE_NAME     ".role"
#define SQLMGR_NAME   ".sqlmgr"
#define CENTER_NAME   ".center"
#define GAME_NAME     "game"
#define CLIENT_NAME   "client"
#define WATCHDOG_NAME ".watchdog"
#define LOADER_NAME   ".loader"
#define WRITER_NAME   ".writer"
#define CHORES_NAME   ".chores"
#define SYS_LOG_NAME  "logger_sys"
#define BILL_LOG_NAME "logger_bill"

#define GET_READ_SQLMGR_NAME(role_id) char sqlmgr_name[128];\
    sprintf(sqlmgr_name, "%s_%d", SQLMGR_NAME, (role_id) % 8);

#define GET_WRITE_SQLMGR_NAME(role_id) char sqlmgr_name[128];\
    sprintf(sqlmgr_name, "%s_%d", SQLMGR_NAME, (role_id) % 8 + 10);

class pb_obj
{
public:
	string dst_name;   // 该PB对象k应该发往何处
	string query_sql;  // 从DB层拿数据的SQL语句
	string table_name; // 关联的表名，即该表的结构对应pb的结构
	vector<string> vec_key; // 该pb对应的map主键，如果为多值主键，则将几个主键的数值串联
	google::protobuf::Message* msg;

public:
	pb_obj(google::protobuf::Message* m)
	{
		msg = m;
	}

	pb_obj(google::protobuf::Message* m, const char* dst)
	{
		msg = m;
		dst_name = dst;
	}

	pb_obj(google::protobuf::Message* m, const char* dst, const char* sql, const char* table)
	{
		msg = m;
		dst_name = dst;
		query_sql = sql;
		table_name = table;
	}

	void add_key(const char* key)
	{
		vec_key.push_back(key);
	}

	~pb_obj()
	{
        vec_key.clear();
	}
};

extern std::vector<std::string> WATCHDOG_NAMES;
extern std::vector<std::string> NODE_NAMES;


#endif

