#ifndef __PLAYER_ROLE_EXT_ATTR_H__
#define __PLAYER_ROLE_EXT_ATTR_H__

// 给用户增加永久性属性的相关数据

#include "comm_define.h"

enum ROLE_EXT_ATTR_SRC_TYPE_E
{
    REASTE_MATE              = 0, // 双修小伙伴通过深度双修永久增加的属性
    REASTE_FAMILY            = 1, // 家族等级增加人物属性
    REASTE_TEST              = 2, // 神奇的增加了属性
    REASTE_MINI_SERVER_SET   = 3, // miniserver固定属性
    REASTE_MINI_SERVER_EQUIP = 4, // miniserver装备属性
    REASTE_MAX,
};

void role_attr_add(client::role_attr& a, const client::role_attr& b);

class player_role_ext_attr : public pb_writer
{
public:
    player_role_ext_attr();

    virtual ~player_role_ext_attr();

    int set(uint32 role_id, uint32 src_type, const client::role_attr& attr);

    int add(uint32 role_id, uint32 src_type, const client::role_attr& attr);

    int sub(uint32 role_id, uint32 src_type, const client::role_attr& attr);

    int attach(mysql::tb_role_ext_attr* p);

    void save(amtsvr_context* ctx);

    const client::role_attr& get_attr();

    void clear();

protected:
    mysql::tb_role_ext_attr* db_role_ext_attr;
    client::role_attr        _attr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////

class player_role_ext_attr_mgr
{
public:
    player_role_ext_attr_mgr();

    virtual ~player_role_ext_attr_mgr();

    int attach(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int add_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr);

    int sub_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr);

    int set_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr);

    int clear_ext_attr(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type);

    player_role_ext_attr* get(ROLE_EXT_ATTR_SRC_TYPE_E src_type, uint32 role_id = 0);

    inline void mark_recalc() { _recalc = true; }

    const client::role_attr& get_all_attr();

    void save(amtsvr_context* ctx);

private:
    int create(uint32 role_id, ROLE_EXT_ATTR_SRC_TYPE_E src_type, const client::role_attr& attr);

private:
    std::vector<player_role_ext_attr*> ext_attrs;
    client::role_attr                  _attr;
    bool                               _recalc;
};

#endif
