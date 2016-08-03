#ifndef __PLAYER_ROLE_LEVELS_H__
#define __PLAYER_ROLE_LEVELS_H__

#include "comm_define.h"
#include "mysql.tb_role_levels.pb.h"
#include "client.levels.pb.h"

class config_giantraid;

class player_role_levels : public pb_writer
{
public:
    player_role_levels(mysql::tb_role_levels *trl = NULL);
    ~player_role_levels();

public:
	int save(amtsvr_context *ctx, uint32 role_id);
	int serialize_levels(string &str);
	int unserialize_levels(const string &str);
	void init(uint32 role_id, uint32 lv, uint32 power);
	void clear_all_levels();
	void init_all_levels(uint32 job);
	void update_role_info(uint32 role_id, uint32 lv, uint32 power);
	void update_role_info();
	void attach_role_levels(int count, cmq_svr_pb_ctx *pb_ctx);
	void init_role_levels(uint32 role_id, uint32 job, uint32 lv, uint32 power);
	void change_cur_levels();
	inline const std::vector<level_info *> &get_levels_info() { return _li; }
	uint32 get_cur_level();
	const client::level_info * get_level_info(uint32 lv) const;
    int set_cur_level(uint32 level);
    void clear_level_info();
	void set_level_info(std::vector<level_info *>& li);
	const config_giantraid *get_config_giantraid();
	void fill_role_levels_list(client::levels_list_rsp &rsp);
	bool levels_need_reset();
	uint32 get_levels_count() { return _li.size(); }

private:
	mysql::tb_role_levels *_trl;
	std::vector<level_info *> _li;			// ¹Ø¿¨ÐÅÏ¢
#ifdef __LIB_MINI_SERVER__
	uint32 _cur_level;
#endif
};

#endif /* __PLAYER_ROLE_LEVELS_H__ */
