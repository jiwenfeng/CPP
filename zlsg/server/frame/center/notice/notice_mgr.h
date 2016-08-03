#ifndef __NOTICE_MGR_H__
#define __NOTICE_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"
#include "mysql.tb_center_notice.pb.h"

class notice_mgr: public pb_writer
{
public:
    notice_mgr();
    virtual ~notice_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

	int update();

	bool add_notice_record(string content,uint32 start_time,uint32 close_time,uint32 interval_time);

	void set_over(uint32 msg_id);

	void set_broast_time(uint32 msg_id);

	void remove_notice(uint32 msg_id);

	inline  std::map<uint32, mysql::tb_center_notice*>& get_notice_map(){ return _notices_map;}
public:
	inline void set(uint32 mid) {
		_msg_id    = mid;
	}

	inline uint32 get() {
		uint32 id = _msg_id + 1;
		if (id == 0) {
			return 0;
		}

		++_msg_id;

		return _msg_id;
	}

	inline uint32 get_msg_id() const { return _msg_id; }

	int set_msg_id(const mysql::tb_center_notice& db_notice);

private:
	// key ΪϵͳID
	std::map<uint32, mysql::tb_center_notice*> _notices_map;
	vector<mysql::tb_center_notice *>		   _update;
	vector<mysql::tb_center_notice *>		   _insert;
	vector<mysql::tb_center_notice *>		   _delete;
	uint32 _msg_id;
};

#endif /* __NOTICE_MGR_H__ */

