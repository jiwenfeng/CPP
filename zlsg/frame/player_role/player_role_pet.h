#ifndef __PLAYER_ROLE_PET_H__
#define __PLAYER_ROLE_PET_H__

#include "pet_parser.h"

#define MAX_PETS_NUM  8

class player_role_pet : public pet_parser, public pb_writer
{
public:
    player_role_pet(mysql::tb_role_pet* db = NULL);
    virtual ~player_role_pet();

public:

    int save_pet(uint32 role_id, amtsvr_context* ctx);

    int attach_role_pet(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int upgrate_pet_level(uint32 player_level, uint32 add_exp, uint32 pet_id, std::vector<uint32>& act_skill_id);

private:
    int set_db_pet(mysql::tb_role_pet* b);

private:
    mysql::tb_role_pet* db_role_pet;
};

#endif
