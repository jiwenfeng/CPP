#include "activity_promotions.h"

activity_promotions::activity_promotions(mysql::tb_promotions* db)
	:_db_promotions(db),_change(false)
{
	writer_type = WOTE_NULL;
}

activity_promotions::activity_promotions(uint32 act_id,string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time)
{
	_db_promotions = new mysql::tb_promotions;
	_db_promotions->set_id(act_id);
	_db_promotions->set_type(type);
	_db_promotions->set_name(name);
	_db_promotions->set_start_time(start_time);
	_db_promotions->set_duration_time(duration_time);
	_db_promotions->set_end_time(start_time + duration_time);
	_db_promotions->set_loop_time(loop_time);
	_db_promotions->set_state(BACKGROUND_ACTIVITY_OPEN_TYPE_NONE);

	writer_type = WOTE_INSERT;
	_change = true;

}

activity_promotions::~activity_promotions()
{
	if (_db_promotions != NULL) {
		delete _db_promotions;
		_db_promotions = NULL;
	}
}

int activity_promotions::save(amtsvr_context* ctx)
{
	if (!_change || _db_promotions == NULL){
		return -1;
	}

	mysql::tb_promotions* tpr = new mysql::tb_promotions(*_db_promotions);
	CACHE_KEY k(tpr->id(),0);
	write(ctx, k, tpr);

	_change      = false;
	writer_type = WOTE_NULL;


	return 0;
}

void activity_promotions::set_act_status(uint32 act_status)
{
	if (_db_promotions != NULL){
		_db_promotions->set_state(act_status);
		_change = true;
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}
}

int activity_promotions::update_pro(string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time)
{
	if (_db_promotions != NULL){

		_db_promotions->set_type(type);
		_db_promotions->set_name(name);
		_db_promotions->set_start_time(start_time);
		_db_promotions->set_duration_time(duration_time);
		_db_promotions->set_end_time(start_time + duration_time);
		_db_promotions->set_loop_time(loop_time);
		_change = true;
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}


	return 0;
}

void activity_promotions::set_activity_time(uint32 start_time,uint32 close_time)
{
	if (_db_promotions != NULL){
		_db_promotions->set_start_time(start_time);
		_db_promotions->set_end_time(close_time);
		_change = true;
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}
}