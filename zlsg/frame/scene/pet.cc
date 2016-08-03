#include "pet.h"

pet::pet(client::pet_db_info* prp /* = NULL */)
        : _prp(prp)
{
    if (prp != NULL) {
        _pet_id = prp->pet_id();
    } else {
        _pet_id = 0;
    }
}

pet::pet(const pet& p)
{
    _pet_id = p._pet_id;
}

pet::~pet()
{
    _prp    = 0;
    _pet_id = 0;
}
