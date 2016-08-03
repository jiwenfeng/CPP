#ifndef __PLAYER_ROLE_EXT_ATTR_H__
#define __PLAYER_ROLE_EXT_ATTR_H__

// ���û��������������Ե��������

#include "comm_define.h"

enum ROLE_EXT_ATTR_SRC_TYPE_E
{
    REASTE_MATE              = 0, // ˫��С���ͨ�����˫���������ӵ�����
    REASTE_FAMILY            = 1, // ����ȼ�������������
    REASTE_TEST              = 2, // ���������������
    REASTE_MINI_SERVER_SET   = 3, // miniserver�̶�����
    REASTE_MINI_SERVER_EQUIP = 4, // miniserverװ������
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
