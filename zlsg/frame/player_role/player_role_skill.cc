#include "config_skill.h"
#include "config_effect.h"
#include "player_role_skill.h"

player_role_skill::player_role_skill(mysql::tb_role_skill* s)
        : _db_role_skill(s)
{

}

player_role_skill::player_role_skill(const player_role_skill& prs)
{
    _db_role_skill = NULL;
    if (prs._db_role_skill != NULL) {
        writer_type = prs.writer_type;
        _db_role_skill = new mysql::tb_role_skill(*prs._db_role_skill);
    }

}

player_role_skill::~player_role_skill()
{
    release();
}

int player_role_skill::create(uint32 role_id, uint32 smy_id,const config_skill* cs,uint32 skill_index)
{
	if (cs == NULL) {
		return -1;
	}

	if (_db_role_skill == NULL) {
		_db_role_skill = new mysql::tb_role_skill;
	}

    writer_type = WOTE_INSERT;

	_db_role_skill->set_role_id(role_id);
	_db_role_skill->set_skill_id(cs->get_skill_id());
	_db_role_skill->set_skill_level(1);
	_db_role_skill->set_skill_grid_index(skill_index);
	_db_role_skill->set_skillful(0);
	_db_role_skill->set_studied(1);
	_db_role_skill->set_end_cd_time(0);
	_db_role_skill->set_created_date((uint32)time(NULL));
	_db_role_skill->set_status(1);
	_db_role_skill->set_smy_id(smy_id);
	const config_effect* skill_effect_info = CONFMGR->get_config_effect_mgr().get_config_effect_by_key(cs->get_key());
	if (skill_effect_info != NULL) {
        _db_role_skill->set_effect_id(skill_effect_info->get_effect_id());
	} else {
        _db_role_skill->set_effect_id(0);
    }

	return 0;
}

void player_role_skill::release()
{
    writer_type = WOTE_NULL;
    if (_db_role_skill != NULL) {
        delete _db_role_skill;
        _db_role_skill = NULL;
    }
}

void player_role_skill::delete_record(amtsvr_context* ctx)
{
    writer_type = WOTE_REMOVE;
	CACHE_KEY k(_db_role_skill->role_id(), _db_role_skill->smy_id());
	mysql::tb_role_skill *trk = new mysql::tb_role_skill(*_db_role_skill);
	write(ctx, k, trk);
//    write_skill(ctx);
//    release();
}

bool player_role_skill::write_skill(amtsvr_context* ctx)
{
//	if(_db_role_skill != NULL)
//	{
//		if(_db_role_skill->smy_id() != _old_smy_id /*&& _old_smy_id != 0*/){
//			writer_type = WOTE_INSERT;
//			_old_smy_id = _db_role_skill->smy_id();
//		}
//
//		//if (_old_smy_id == 0){
//		//	_old_smy_id = _db_role_skill->smy_id();
//		//}
//
//		CACHE_KEY k(_db_role_skill->role_id(), _db_role_skill->smy_id());
//		mysql::tb_role_skill *trk = new mysql::tb_role_skill(*_db_role_skill);
//		write(ctx, k, trk);
//		writer_type = WOTE_UPDATE;
//	}
//	return true;
//#if 0
//    if (_db_role_skill != NULL) {
//		if (_old_smy_id == 0){
//			CACHE_KEY k(_db_role_skill->role_id(), _db_role_skill->smy_id());
//			mysql::tb_role_skill* tmp = new mysql::tb_role_skill(*_db_role_skill);
//			write(ctx, k, tmp);
//			writer_type = WOTE_UPDATE;
//			return true;
//		}else{
//			printf("change smy_id @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \n");
//			// ÏÈÉ¾³ýÔÙ²åÈë
//			CACHE_KEY k(_db_role_skill->role_id(), _old_smy_id);
//			mysql::tb_role_skill* tmp = new mysql::tb_role_skill(*_db_role_skill);
//			write(ctx, k, tmp);
//			writer_type = WOTE_UPDATE;
//			_old_smy_id = 0;
//			return false;
//		}
//    }
//
//	return true;
//#endif

	if (_db_role_skill != NULL) {
		CACHE_KEY k(_db_role_skill->role_id(), _db_role_skill->smy_id());
		mysql::tb_role_skill* tmp = new mysql::tb_role_skill(*_db_role_skill);
		write(ctx, k, tmp);
		writer_type = WOTE_UPDATE;
	}

	return true;
}

bool player_role_skill::rewrite_skill(amtsvr_context* ctx)
{
	CACHE_KEY k(_db_role_skill->role_id(), _db_role_skill->smy_id());
	mysql::tb_role_skill* tmp = new mysql::tb_role_skill(*_db_role_skill);
	writer_type = WOTE_INSERT;
	write(ctx, k, tmp);

	return true;
}

void player_role_skill::fill_role_skill_data(client::skill_data& sd) const
{
    if (_db_role_skill == NULL) {
        return;
    }

    //printf("id = %d, lv = %d\n", db_role_skill->skill_id(), db_role_skill->skill_level());
    sd.set_id(_db_role_skill->skill_id());
    sd.set_cd(_db_role_skill->end_cd_time());
    sd.set_mas(_db_role_skill->studied());
    sd.set_serial(_db_role_skill->skill_grid_index());
    sd.set_level(_db_role_skill->skill_level());
	sd.set_effect_id(_db_role_skill->effect_id());
    /*if (get_default_skill() == player_role_skill::DEFAULT_SKILL) {
        sd.set_default_skill(player_role_skill::DEFAULT_SKILL);
        }*/
}

bool player_role_skill::max_lvl()
{
    const config_skill* ts = CONFMGR->get_config_skill(_db_role_skill->skill_id());
    return (ts == NULL || ts->get_upgrade_skill_id() == 0);
}

void player_role_skill::set_default_skill(uint32 is_default)
{
    if (_db_role_skill == NULL) {
        return ;
    }

    _db_role_skill->set_skillful(is_default);
}

uint32 player_role_skill::get_default_skill() const
{
    if (_db_role_skill == NULL) {
        return player_role_skill::DEFAULT_SKILL;
    }

    return _db_role_skill->skillful();
}

void player_role_skill::set_skill(const client::skill_data& skill)
{
    _db_role_skill->set_skill_id        (skill.id       ());
    _db_role_skill->set_end_cd_time     (skill.cd       ());
    _db_role_skill->set_studied         (skill.mas      ());
    _db_role_skill->set_skill_grid_index(skill.serial   ());
    _db_role_skill->set_skill_level     (skill.level    ());
    _db_role_skill->set_effect_id       (skill.effect_id());
}

void player_role_skill::set_skill_effect_data(uint32 effect_id,uint32 effect_level)
{
	 _db_role_skill->set_skill_level     (effect_level);
	 _db_role_skill->set_effect_id       (effect_id);
}

void player_role_skill::set_smy_id(uint32 smy_id)
{
	if (_db_role_skill->smy_id() != smy_id){
		_db_role_skill->set_smy_id(smy_id);
	}
}

//bool player_role_skill::need_delete()
//{
//	if(_db_role_skill == NULL || _old_smy_id == 0)
//	{
//		return false;
//	}
//
//	return _db_role_skill->smy_id() != _old_smy_id;
//}

//void player_role_skill::delete_old_record(amtsvr_context *ctx)
//{
//    writer_type = WOTE_REMOVE;
//	CACHE_KEY k(_db_role_skill->role_id(), _old_smy_id);
//	mysql::tb_role_skill *trk = new mysql::tb_role_skill(*_db_role_skill);
//	write(ctx, k, trk);
//}
