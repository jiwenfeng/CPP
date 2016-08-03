#ifndef __BASE_DEFINE_H__
#define __BASE_DEFINE_H__

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <google/protobuf/descriptor.h>

#ifdef _WIN32
#include "LZSGConfig.h"
#endif // _WIN32

using namespace std;

typedef uint8_t   uint8;
typedef char      int8;
typedef uint16_t  uint16;
typedef short     int16;
typedef uint32_t  uint32;
typedef int       int32;
typedef uint64_t  uint64;
typedef long long int64;

#define MAX_BROADCAST_AGENT_NUM 5000 // ���ֻ�ܹ㲥��5000�����û�

#define MAX_PB_MOD_NAME_LEN 128
#define MAX_IP_LEN 32
#define MAX_NAME_LEN 32
#define MIN_NAME_LEN 3
#define MAX_USER_NAME_LEN 64
#define MAX_FILE_PATH 256
#define MAX_PB_NAME_LEN 64

typedef std::pair<uint32,uint32> CACHE_KEY;  // ��Ŀ��Ŀǰ��֧�����˫key������Ҫ�Ļ�����������չ
#define MAX_CACHE_KEY_NUM  2
//#define CMB 1000 // ��װ����
#define UINT64_MAKE(high, low)   ((uint64)(((unsigned int)((low) & 0xFFFFFFFF)) | ((uint64)((unsigned int)((high) & 0xFFFFFFFF))) << 32))
#define UINT64_LOW(i)            ((unsigned int)((uint64)(i) & 0xFFFFFFFF))
#define UINT64_HIGH(i)           ((unsigned int)((uint64)(i) >> 32))

#define UINT32_MAKE(high, low) ((uint32)(((unsigned int)((low) & 0xFFFF)) | ((uint32)((unsigned int)((high) & 0xFFFF))) << 16))
#define UINT32_LOW(i)         ((unsigned int)((uint32)(i) & 0xFFFF))
#define UINT32_HIGH(i)        ((unsigned int)((uint32)(i) >> 16)) 

#define SVR_ID_MASK  0xFFF  // ��1.4���ֽ�Ϊ������ID,һ��ƽ̨�����֧�ֿ�4095���������4095�������ڸ�ƽ̨���ٿ��µĴ���������֮�䲻֧�ֺϷ�
#define ROLE_ID_MASK 0xFFFFF    // ��3.8���ֽ�Ϊ��ǰ�������µ��û�ID ���֧��100���ID
#define GET_SVR_ID(id) (((id>>20)&SVR_ID_MASK)) // ����ID��ȡ������ID
#define GET_ROLE_ID(id) ((id)&ROLE_ID_MASK)     // ����ID��ȡ��ɫID
#define PACK_SVR_ROLE_ID(svr_id,role_id) (((svr_id)<<20) | ((role_id)&ROLE_ID_MASK)) // ���ݷ�����ID ��ɫID ��װ������ID
// ��װ����ϵͳID
#define FAMILY_ID_MASK 0xFFFFF
#define GET_FAMILY_ID(id) ((id)&FAMILY_ID_MASK)
#define PACK_SVR_FAMILY_ID(svr_id,f_id) (((svr_id)<<20) | ((f_id)&FAMILY_ID_MASK)) 
// ��װ����ϵͳID
#define SKILL_ID_MASK 0xFFFFF
#define GET_SKILL_ID(id) ((id)&SKILL_ID_MASK)
#define PACK_SVR_SKILL_ID(role_id,s_id) (((role_id)<<20) | ((s_id)&SKILL_ID_MASK)) 
// ��װ��ƷϵͳID
#define PROPS_SMY_ID_MASK 0xFFFFF
#define GET_PROP_SMY_ID(id) ((id)&PROPS_SMY_ID_MASK)
#define PACK_SVR_PROP_ID(beg_index,psid) (((beg_index)<<20) | ((psid)&PROPS_SMY_ID_MASK)) 

#define ONE_DAY_SECONDS 86400
#define TEN_THOUSAND    10000

#define SHUTDOWN_CMD  "shutdown " // game�����Ͽ�
#define CLOSE_ALL     "close_all" // �رյ�����agent
#define LOSE_CONN     "lose_conn" // �ͻ���ʧȥ����
#define OPEN_NODE     "open_node" // �����ڵ�

extern bool GAME_EXIT;

#define MAX_CMQ_SVR_NAME_LEN 32

enum SVR_SRCMSG_TYPE_E
{
    SSTE_AGENT    = 0, 
    SSTE_AUTH     = 1,
    SSTE_SQLMGR   = 2,
    SSTE_WORLD    = 3,
    SSTE_SCENE    = 4,
    SSTE_CLIENT   = 5, // ͸�����û���PB��Ϣ����ҪУ�飬�������;�Ϊ���������ͣ���������Ϣ
    SSTE_WATCHDOG = 6, 
    SSTE_LOADER   = 7,
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
#define SYS_LOG_NAME  ".logger_sys"
#define BILL_LOG_NAME ".logger_bill"

#define GET_READ_SQLMGR_NAME(role_id) char sqlmgr_name[128];\
    sprintf(sqlmgr_name, "%s_%d", SQLMGR_NAME, (role_id) % 8);

#define GET_WRITE_SQLMGR_NAME(role_id) char sqlmgr_name[128];\
    sprintf(sqlmgr_name, "%s_%d", SQLMGR_NAME, (role_id) % 8 + 10);

class pb_obj
{
public:
	std::string dst_name;   // ��PB����kӦ�÷����δ�
	std::string query_sql;  // ��DB�������ݵ�SQL���
	std::string table_name; // �����ı��������ñ�Ľṹ��Ӧpb�Ľṹ
	std::vector<std::string> vec_key; // ��pb��Ӧ��map���������Ϊ��ֵ�������򽫼�����������ֵ����
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

#endif // __BASE_DEFINE_H__

