#ifndef __MATE_MGR_H__
#define __MATE_MGR_H__

#include "mate.h"

class mate_mgr
{
public:
    mate_mgr();

    ~mate_mgr();

    int init();

public:

    client::MATE_OPERATE_RET_E fill_mate_info_rsp(uint32 role_id, client::mate_info_rsp& rsp);

    client::MATE_OPERATE_RET_E create_mate_relation(uint32 role_id, uint32 mate_id);

    client::MATE_OPERATE_RET_E can_mate_with_role(uint32 role_id, mate*& src, uint32 mate_id, mate*& dst);

    client::MATE_OPERATE_RET_E remove_mate_relation(uint32 role_id, uint32& mate_id);

    client::MATE_OPERATE_RET_E can_enter_depth_mate(uint32 role_id, mate*& src, uint32& src_depth_times, uint32& mate_id, mate*& dst, uint32& dst_depth_times);

    client::MATE_OPERATE_RET_E enter_depth_mate(uint32 role_id, uint32& mate_id, bool advanced);

    client::MATE_OPERATE_RET_E end_depth_mate(uint32 role_id, client::MATE_OPERATE_RET_E& src_ret, bool& src_advanced, uint32& mate_id, client::MATE_OPERATE_RET_E& dst_ret, bool& dst_advanced);

    client::MATE_OPERATE_RET_E end_depth_mate(uint32 role_id, bool& advanced, uint32& mate_id);

    const mate* get_my_mate(uint32 role_id) const;

    inline void attach_ctx( amtsvr_context* p) {ctx = p;}

private:
    mate* get(uint32 role_id);

private:
    amtsvr_context* ctx;
    map<uint32, mate*> map_mates;  // role_id ---> mate 
};

#endif
