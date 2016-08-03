#ifndef __CENTER_ROLE_SET_H__
#define __CENTER_ROLE_SET_H__

#include "comm_define.h"
#include "set_parser.h"

#define DEFAULT_TEAM_AUTO_AGREE true
#define DEFAULT_TEAM_AUTO_JOIN  true
#define DEFAULT_TEAM_MIN_LVL    30
#define DEFAULT_TEAM_MAX_LVL    100

class center_role_set : public pb_writer
{
public:
    center_role_set();
    virtual ~center_role_set();

public:
    void write(amtsvr_context* ctx);
    int attach(mysql::tb_role_center_set* trs);
    inline mysql::tb_role_center_set* get_tb_role_set() { return db_role_set; }
    inline const mysql::tb_role_center_set* get_tb_role_set() const { return db_role_set; }

    mysql::tb_role_center_set* create();
   // int set(uint32 role_id, const client::team_set& ts);

   // void fill_team_set(client::team_set& ts) const;

    uint32 get_role_id() const;
    bool is_auto_agree() const;
    bool is_auto_join() const;
    uint32 get_min_lvl() const;
    uint32 get_max_lvl() const;

public:
    inline WRITER_OPERATE_TYPE_E get_writer_type() const { return writer_type; }
    inline void set_writer_type(WRITER_OPERATE_TYPE_E t) { writer_type = t; }

private:
    mysql::tb_role_center_set* db_role_set;
};

#endif
