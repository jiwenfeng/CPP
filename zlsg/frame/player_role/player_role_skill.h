#ifndef __PLAYER_ROLE_SKILL_H__
#define __PLAYER_ROLE_SKILL_H__

#include "comm_define.h"

class player_role_skill : public pb_writer
{
public:
    player_role_skill(mysql::tb_role_skill* s = NULL);
    player_role_skill(const player_role_skill& prs);
//     player_role_skill(player_role_skill&& prs);
    virtual ~player_role_skill();

public:
    player_role_skill& operator=(const player_role_skill& prs) {
        release();
        if (prs._db_role_skill != NULL) {
            writer_type = prs.writer_type;
            _db_role_skill = new mysql::tb_role_skill(*prs._db_role_skill);
        }

        return *this;
    }

    bool operator==(uint32 skill_id) {
        if (_db_role_skill == NULL) {
            return false;
        }

        return _db_role_skill->skill_id() == skill_id;
    }
    
public:
    int create(uint32 role_id,uint32 smy_id, const config_skill* cs,uint32 skill_index = 0);
    inline uint32 attach(mysql::tb_role_skill* s) {
        if (_db_role_skill == s) {
            return 0;
        }
 
        if (_db_role_skill != NULL) {
            delete _db_role_skill;
        }

        _db_role_skill = s;

		return _db_role_skill->smy_id();
    }

    inline mysql::tb_role_skill* get_db_role_skill() { return _db_role_skill; }

    uint32 get_skill_id() {
        if (_db_role_skill == NULL) {
            return 0;
        }

        return _db_role_skill->skill_id();
    }

	uint32 get_skill_ser_index()
	{
		if (_db_role_skill == NULL) {
			return 0;
		}
		
		return _db_role_skill->skill_grid_index();
	}

	uint32 get_effect_id() {
		if (_db_role_skill == NULL) {
			return 0;
		}

		return _db_role_skill->effect_id();

	}

    bool write_skill(amtsvr_context* ctx);

	bool rewrite_skill(amtsvr_context* ctx);

    void fill_role_skill_data(client::skill_data& sd) const;

    void release();

    void delete_record(amtsvr_context* ctx);

    bool studied() const {return (_db_role_skill != NULL ? _db_role_skill->studied() == 1 : false);}

    bool max_lvl();

    void set_default_skill(uint32 is_default);
	
    uint32 get_default_skill() const;

    void set_skill(const client::skill_data& skill);

	void set_skill_effect_data(uint32 effect_id,uint32 effect_level);

	void set_smy_id(uint32 smy_id);

	//bool need_delete();

	//void delete_old_record(amtsvr_context *ctx);
	
	//inline void set_old_id(uint32 old_id){ _old_smy_id = old_id;}
	inline void set_skill_ser_index(uint32 ser_index){ _db_role_skill->set_skill_grid_index(ser_index);}
public:
    static const uint32 DEFAULT_SKILL     = 1;
    static const uint32 NOT_DEFAULT_SKILL = 0;

protected:
    mysql::tb_role_skill* _db_role_skill;
	//uint32				  _old_smy_id;
};

#endif
