#ifndef __PLAYER_ROLE_ORDER_MGR_H__
#define __PLAYER_ROLE_ORDER_MGR_H__

#include "player_role_order.h"

class player_role_order_mgr
{
public:
    player_role_order_mgr();
    virtual ~player_role_order_mgr();

public:
    int init();

    int attach_role_order(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int push(mysql::tb_role_order* tro);

    int save(amtsvr_context* ctx);

public:
    int add(uint32 role_id, uint32 store_id, uint32 count, uint32 type);

    uint32 get(uint32 store_id, uint32 type);
    
    int fill(client::order_tracking_rsp& rsp);

private:
    void time_out(); // һ�� ���� ��������������һ������ Ϊ��û�����⻹���Ȳ�����������

public:
    static const uint32 RED_STORE = 0;
    static const uint32 NPC_STORE = 1;

private:
    map<uint32, player_role_order> _order; // map������ȶ��Ľṹ������������ʱ��������ԭ���Ķ���
    map<uint32, player_role_order> _npc; // map������ȶ��Ľṹ������������ʱ��������ԭ���Ķ���
};

#endif /* __PLAYER_ROLE_ORDER_MGR_H__ */
