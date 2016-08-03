#ifndef __FAMILY_LOG_MGR_H__
#define __FAMILY_LOG_MGR_H__

#include "comm_define.h"
#include "family_log.h"


class log_sid
{
public:
	log_sid() {
		svr_id    = 0;
		log_id    = 0;
	}

	virtual ~log_sid() {

	}

	inline void set(uint32 sid, uint32 fid) {
		svr_id    = sid;
		log_id    = fid;
	}

	inline uint32 get() {
		uint32 id = log_id + 1;
		if (id == 0 || id > FAMILY_ID_MASK) {
			return 0;
		}

		++log_id;

		return PACK_SVR_FAMILY_ID(svr_id, log_id);
	}

	inline uint32 get_log_id() const { return log_id; }
private:
	uint32 svr_id;
	uint32 log_id;
};

class family_log_mgr
{
public:
    family_log_mgr();
    virtual ~family_log_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);
    
	int init_log_smy_id();

	int set_log_lgsid(const mysql::tb_family_log& tlf);
public:
	// 插入记录的数据
    int insert(uint32 family_id, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id);
	// 移除记录（这里需要改）根据type类型做数据的移除
    int remove(uint32 family_id);
	// 
	int check_log_time(uint32 family_id);

	bool generate_log_id(uint32 svr_id, uint32& fid);

	int delete_log(uint32 family_id,uint32 log_id);
public:
    int fill_log(uint32 family_id, client::family_log_rsp& rsp);
    
private:
    std::multimap<uint32, family_log*> _log;
    std::vector<family_log*>           _modify;
    std::vector<family_log*>           _remove;
	map<uint32, log_sid>               _lgsids; 
};

#endif /* __FAMILY_LOG_MGR_H__ */
