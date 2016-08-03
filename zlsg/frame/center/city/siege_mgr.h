#ifndef __SIEGE_MGR_H__
#define __SIEGE_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

#define BROADCAST_THIRTY 1800
#define BROADCAST_TEN	 600

#define SIEGE_ID		 0x01

class siege_mgr : public pb_writer
{
public:
	siege_mgr();
	~siege_mgr();
	
public:
	static uint32 calc_open_time();

public:
	int open_act(uint32 tl);

protected:
	int send_center(amtsvr_context *ctx, google::protobuf::Message &coi);
	int send_game(amtsvr_context *ctx, google::protobuf::Message &coi);

public:
	void init(amtsvr_context *ctx);
	void reset_siege_info();
	void on_siege_over(uint32 family_id);
	void set_city_title(const string &str);
	int save();
	int update(amtsvr_context *ctx);
	inline uint32 get_act_status() { return get_status(); }
	inline bool is_act_open() { return get_status() > 1; }
	inline void set_scene_svr_id(uint32 scene_svr_id) { _scene_svr_id = scene_svr_id; }
	inline void set_temp_family(uint32 family_id) { _temp_id = family_id; }
	inline uint32 get_scene_svr_id() { return _scene_svr_id; }
	inline uint32 get_open_time() { return _open_time; }
	inline uint32 get_temp_family() { return _temp_id; }
	inline uint32 get_family_id() { return _data == NULL ? 0 : _data->family_id(); }
	inline uint32 get_occupy_time() { return _data == NULL ? 0 : _data->occupy_time(); }
	inline uint32 get_mod_title_time() { return _data == NULL ? 0 : _data->mod_title_time(); }
	const string &get_title();

private:
	inline void change_status() { _status++; }
	inline void reset_status() { _status = 0; }
	inline uint32 get_status() { return _status; }
	int send_30_min_notice(amtsvr_context *ctx);
	int send_10_min_notice(amtsvr_context *ctx);
	int do_siege_start(amtsvr_context *ctx);
	int broadcast_act_status(amtsvr_context *ctx);
	void reset_data(uint32 family_id);

private:
	mysql::tb_city_attack *_data;
	amtsvr_context *_ctx;
	uint32 _status;	// 0 未开启 1 30分钟广播 2 10分钟广播/战场开启 3 攻城战开始	
	uint32 _open_time;
	uint32 _scene_svr_id;
	uint32 _temp_id;
};

#endif
