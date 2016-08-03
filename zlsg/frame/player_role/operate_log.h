#ifndef __OPERATE_LOG_H__
#define __OPERATE_LOG_H__

#include "comm_define.h"

class operate_log : public pb_writer
{
public:
    operate_log();
    virtual ~operate_log();

public:
    void push(server::operate_log& log);
    void save(amtsvr_context* ctx);

public:
    static const uint32 ROOT_BATTLE = 1;
    static const uint32 TYPE_BATTLE = 1;
    static const uint32 ROOT_INVEST = 2;
    static const uint32 TYPE_INVEST = 1;
    static const uint32 ROOT_SHOP   = 3;
    static const uint32 TYPE_SHOP   = 1;
    static const uint32 TYPE_SHOP_NPC   = 2;
    static const uint32 ROOT_TASK   = 4;
    static const uint32 TYPE_TASK   = 1;
    static const uint32 ROOT_MAIL   = 5;
    static const uint32 TYPE_MAIL   = 1;
    static const uint32 ROOT_EQUIP  = 6;
    static const uint32 TYPE_EQUIP_SHIFT = 1;
    static const uint32 ROOT_REFINING = 7;
    static const uint32 TYPE_REFINING = 1;
    static const uint32 ROOT_PET      = 8;
    static const uint32 TYPE_PET_SKILL_SEAL = 1;
    static const uint32 ROOT_WING     = 9;
    static const uint32 TYPE_WING_TRAIN = 1;
    static const uint32 ROOT_TOWER      = 10;
    static const uint32 TYPE_TOWER = 1;
    static const uint32 ROOT_EXPLORE = 11;
    static const uint32 TYPE_EXPLORE = 1;
    static const uint32 ROOT_FAMILY = 12;
    static const uint32 TYPE_FAMILY_DONATE = 1;
    static const uint32 ROOT_ARENA = 13;
    static const uint32 TYPE_ARENA = 1;
    static const uint32 ROOT_LUCKY = 14;
    static const uint32 TYPE_LUCKY = 1;

private:
    void save_task(amtsvr_context* ctx, const server::operate_task& log);
    void save_money(amtsvr_context* ctx, const server::operate_money& log);

private:
    std::vector<server::operate_log> logs;
    uint32 cd;
};

#endif // __OPERATE_LOG_H__
