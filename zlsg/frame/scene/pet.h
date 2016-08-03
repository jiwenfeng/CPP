#ifndef __PET_H__
#define __PET_H__

#include "player_role_pet.h"

class pet
{
public:
    pet(client::pet_db_info* prp = NULL);
    pet(const pet& p);
    virtual ~pet();

public:
    inline client::pet_db_info* get_pet() const { return _prp; }

protected:
    client::pet_db_info* _prp;
    uint32               _pet_id; // 宠物的系统ID
};

#endif
