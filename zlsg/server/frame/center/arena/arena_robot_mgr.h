#ifndef __ARENA_ROBOT_MGR_H__
#define __ARENA_ROBOT_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"
#include "arena_mgr.h"

class cmp
{
public:
    bool operator()(const mysql::tb_arena_robot *t1, const mysql::tb_arena_robot *t2)
    {
        return t1->fighting_power() > t2->fighting_power();
    }
};

class robot : public pb_writer
{
public:
	robot(mysql::tb_arena_robot *tar = NULL);
	~robot();
	int save(amtsvr_context *ctx);
	const mysql::tb_arena_robot *get_data() const { return _tar; }

	int serialize_attr(string &str, const map<int, int> &m);
	int unserialize_attr(const string &str, map<int, int> &m);
	int unserialize_robot_attr(const string &str);
	int unserialize_pet_attr(const string &str);
	int unserialize_standby_attr(const string &str);

	void set_robot_attr(const map<int, int> &m) { _attr = m; }
	void set_pet_attr(const map<int, int> &m) { _b_attr = m; }
	void set_standby_attr(const map<int, int> &m) { _b_attr = m; }
	void change_writer_type() { writer_type = WOTE_UPDATE; }

	const map<int, int> &get_robot_attr() const { return _attr; }
	const map<int, int> &get_pet_attr() const { return _b_attr; }
	const map<int, int> &get_standby_attr() const { return _b_attr; }
	const string &get_role_name() const;
	const uint32 get_config_id() const;
	const uint32 get_role_id() const;
	const uint32 get_fight_power() const;
	const uint32 get_pet_id() const;
	const uint32 get_standby_id() const;


private:
	mysql::tb_arena_robot *_tar;
	map<int, int>	_attr;
	map<int, int>	_g_att;				// 出战美人属性
	map<int, int>	_b_attr;			// 备战美人属性
};

class robot_mgr
{
public:
	robot_mgr();
	~robot_mgr();
public:
	robot *add_robot(mysql::tb_arena_robot *tar);
	int init();
	int save(amtsvr_context *ctx);
	bool need_grenerate_robot() { return _robot.empty(); }
	const robot *get_robot(uint32 role_id) const;
	// true if exist
	bool robot_name_exist(const string &str) { return _name.find(str) != _name.end(); }
private:
	map<uint32, robot *> _robot;
	map<string, uint32> _name;
};

#endif

