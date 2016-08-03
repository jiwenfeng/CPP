#ifndef __ACTIVITY_PROMOTIONS_MGR_H__
#define __ACTIVITY_PROMOTIONS_MGR_H__

#include <map>
#include "comm_define.h"
#include "activity_promotions.h"
// 活动后台配置
class activity_promotions_mgr
{
public:

	activity_promotions_mgr();

	virtual ~activity_promotions_mgr();

	int init(struct cmq_t* l);

	int save(amtsvr_context* ctx);

	int create_new_promotions(uint32 act_id,string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time);

public:

	inline bool get_is_copy(){return _is_copy;}

	inline bool set_is_copy(bool is_copy){return _is_copy = is_copy;}

	inline const std::map<uint32,activity_promotions*>& get_promotions_list() const{return _promotions_map;}

	inline std::map<uint32,activity_promotions*>& mutable_promotions_list() {return _promotions_map;}

public:

	const uint32 get_activity_status(uint32 activity_id);

	void set_activity_status(uint32 activity_id,uint32 status_value);

	void update_activity_time(uint32 activity_id,uint32 start_time,uint32 close_time);

private:

	std::map<uint32,activity_promotions*> _promotions_map;

	bool								  _is_copy;
};

#endif