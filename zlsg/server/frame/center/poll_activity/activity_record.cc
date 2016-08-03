#include "activity_record.h"
#include "config_activity_item.h"
activity_record::activity_record(mysql::tb_activity_record* activity_record_db)
	:_db_activity_record(activity_record_db),_change(false)
{
	if (_db_activity_record != NULL){
		parse_record(_db_activity_record->act_record());
	}

	writer_type = WOTE_NULL;
	
	_today_is_reset = false;

	is_record = false;
}

activity_record::activity_record(uint32 activity_id):_db_activity_record(NULL),_change(false)
{
	create_record(activity_id);
}

activity_record::~activity_record()
{
	if (_db_activity_record != NULL) {
		delete _db_activity_record;
		_db_activity_record = NULL;
	}
}

void activity_record::create_record(uint32 activity_id)
{
	if (_db_activity_record == NULL) {
		_db_activity_record = new mysql::tb_activity_record;
		writer_type = WOTE_INSERT;
	} else {
		writer_type = WOTE_UPDATE;
	}

	_activity_id = activity_id;

	_db_activity_record->set_activity_id(activity_id);

	_change = true;

	_today_is_reset = false;

	is_record = false;
}

int activity_record::remove()
{
	if (writer_type == WOTE_INSERT) {
		_change = false;
		return 0;
	} else {
		writer_type = WOTE_REMOVE;
		_change = true;
		return 0;
	}

	return 0;
}

int activity_record::save(amtsvr_context* ctx)
{
	string out;
	int count = serialize_record(out);

	if (!_change || _db_activity_record == NULL || count <= 0) {
		return -1;
	}
	
	mysql::tb_activity_record* tar = new mysql::tb_activity_record(*_db_activity_record);
	CACHE_KEY k(tar->activity_id(),0);
	tar->set_act_record(out);
	write(ctx, k, tar);

	_change      = false;
	writer_type = WOTE_NULL;

	return 0;
}

void activity_record::fill_record_data(uint32 role_id,client::act_db_info& info)
{
	const client::act_db_info& db_info = get_act_db_info();

	for (int i = 0; i < db_info.center_item_size(); ++i){
		client::item_info* it_info = info.add_center_item();
		it_info->CopyFrom(db_info.center_item(i));
	}

	for (int j = 0; j < db_info.flag_info_size(); ++j){
		if (db_info.flag_info(j).role_id() == role_id){
			client::reward_flag* flag_info = info.add_flag_info();
			flag_info->CopyFrom(db_info.flag_info(j));
			break;
		}
	}

	for (int k = 0;k < db_info.person_item_size(); ++ k){
		if (db_info.person_item(k).role_id() == role_id){
			client::item_info* per_info = info.add_person_item();
			per_info->CopyFrom(db_info.person_item(k));
		}
	}

	for (int m = 0;m < db_info.values_info_size(); ++ m){
		if (db_info.values_info(m).role_id() == role_id){
			client::value_info* va_info = info.add_values_info();
			va_info->CopyFrom(db_info.values_info(m));
		}
	}
}

void activity_record::add_buy_record_data(uint32 role_id,uint32 buy_type,uint32 item_count,uint32 act_type)
{
	if (buy_type == 1){
		add_person_data(role_id,item_count,act_type);
	}else if (buy_type == 2){
		add_server_data(item_count,act_type);
	}
}

uint32 activity_record::get_buy_record_data(uint32 role_id,uint32 buy_type,uint32 act_type)
{
	uint32 count = 0;

	if (buy_type == 1){
		count = get_person_data_count(role_id,act_type);
	}else if (buy_type == 2){
		count = get_server_data_count(act_type);
	}

	return count;
}

void activity_record::add_person_data(uint32 role_id,uint32 item_count,uint32 act_type)
{
	const client::act_db_info& db_info = get_act_db_info();
	bool is_new = true;
	for (int i = 0; i < mut_db_info().person_item_size(); ++i){
		if (db_info.person_item(i).activity_type() == act_type && db_info.person_item(i).role_id() == role_id){
			is_new = false;
			mut_db_info().mutable_person_item(i)->set_item_count(db_info.person_item(i).item_count() + item_count);
			_change = true;
			if (writer_type != WOTE_INSERT){
				writer_type = WOTE_UPDATE;
			}
		}
	}

	if (is_new){
		client::item_info* info = mut_db_info().add_person_item();
		info->set_item_count(item_count);
		info->set_role_id(role_id);
		info->set_activity_type(act_type);
		_change = true;
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}
}

uint32 activity_record::get_person_data_count(uint32 role_id,uint32 act_type)
{
	uint32 person_count = 0;

	const client::act_db_info& db_info = get_act_db_info();
	for (int i = 0; i < mut_db_info().person_item_size(); ++i){
		if (db_info.person_item(i).activity_type() == act_type && db_info.person_item(i).role_id() == role_id){
			person_count = db_info.person_item(i).item_count();
		}
	}

	return person_count;
}

uint32 activity_record::get_his_data(uint32 role_id,uint32 limit_type)
{
	uint32 his_value = 0;

	const client::act_db_info& db_info = get_act_db_info();
	for (int i = 0; i < mut_db_info().values_info_size(); ++i){
		if (db_info.values_info(i).limit_type() == limit_type && db_info.values_info(i).role_id() == role_id){
			his_value = db_info.values_info(i).old_value();
		}
	}

	return his_value;
}

uint32 activity_record::get_new_data(uint32 role_id,uint32 limit_type)
{
	uint32 new_value = 0;

	const client::act_db_info& db_info = get_act_db_info();
	for (int i = 0; i < mut_db_info().values_info_size(); ++i){
		if (db_info.values_info(i).limit_type() == limit_type && db_info.values_info(i).role_id() == role_id){
			new_value = db_info.values_info(i).new_value();
		}
	}

	return new_value;
}

void activity_record::add_server_data(uint32 item_count,uint32 act_type)
{
	const client::act_db_info& db_info = get_act_db_info();
	for (int i = 0; i < mut_db_info().center_item_size(); ++i){
		if (db_info.center_item(i).activity_type() == act_type){
			mut_db_info().mutable_center_item(i)->set_item_count(db_info.center_item(i).item_count() + item_count);
			_change = true;
			if (writer_type != WOTE_INSERT){
				writer_type = WOTE_UPDATE;
			}
		}
	}
	
}

void activity_record::add_his_data(uint32 role_id,uint32 limit_type,uint32 value_count)
{
	for (int i = 0; i < mut_db_info().values_info_size(); ++i){
		if (mut_db_info().mutable_values_info(i)->role_id() == role_id){
			if (limit_type == REWARD_TYPE_POWER){
				if (mut_db_info().mutable_values_info(i)->has_new_value()){
					if (mut_db_info().mutable_values_info(i)->new_value() < value_count){
						mut_db_info().mutable_values_info(i)->set_new_value(value_count);
					}
				}else{
					mut_db_info().mutable_values_info(i)->set_new_value(value_count);
				}

				_change = true;
				if (writer_type != WOTE_INSERT){
					writer_type = WOTE_UPDATE;
				}
			}else if (limit_type == REWARD_TYPE_GEM_LEVEL){
				if (mut_db_info().mutable_values_info(i)->has_new_value()){
					mut_db_info().mutable_values_info(i)->set_new_value(mut_db_info().mutable_values_info(i)->new_value() + value_count);
				}else{
					mut_db_info().mutable_values_info(i)->set_new_value(value_count);
				}
			}
		}
	}
}

bool activity_record::is_has_record(uint32 role_id)
{
	for (int i = 0; i < mut_db_info().values_info_size(); ++i){
		if (mut_db_info().mutable_values_info(i)->role_id() == role_id){
			return true;
		}
	}

	return false;
}

void activity_record::creat_limit_record(uint32 role_id,uint32 limit_type,uint32 value_count)
{
	client::value_info* info = mut_db_info().add_values_info();
	info->set_old_value(value_count);
	info->set_role_id(role_id);
	info->set_limit_type(limit_type);
	_change = true;
	if (writer_type != WOTE_INSERT){
		writer_type = WOTE_UPDATE;
	}
}

uint32 activity_record::get_server_data_count(uint32 act_type)
{
	uint32 server_count = 0;

	const client::act_db_info& db_info = get_act_db_info();
	for (int i = 0; i < mut_db_info().center_item_size(); ++i){
		if (db_info.center_item(i).activity_type() == act_type){
			server_count = db_info.center_item(i).item_count();
		}
	}

	return server_count;
}

uint32 activity_record::get_person_reward_flag(uint32 role_id,uint32 act_type)
{
	bool is_new = true;
	for (int i = 0; i < mut_db_info().flag_info_size(); ++i){
		if (mut_db_info().flag_info(i).role_id() == role_id){
			is_new =  false;
			return mut_db_info().flag_info(i).rflag();
		}
	}

	if (is_new){
		client::reward_flag* info = mut_db_info().add_flag_info();
		info->set_role_id(role_id);
		info->set_rflag(0);
		info->set_activity_type(act_type);
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}

	return 0;
}

bool  activity_record::set_person_reward_flag(uint32 role_id,uint32 flag,uint32 act_type)
{
	bool is_new = true;
	for (int i = 0; i < mut_db_info().flag_info_size(); ++i){
		if (mut_db_info().flag_info(i).role_id() == role_id){
			is_new =  false;
			mut_db_info().mutable_flag_info(i)->set_rflag(flag);
			_change = true;
			if (writer_type != WOTE_INSERT){
				writer_type = WOTE_UPDATE;
			}
		}
	}

	if (is_new){
		client::reward_flag* info = mut_db_info().add_flag_info();
		info->set_role_id(role_id);
		info->set_rflag(flag);
		info->set_activity_type(act_type);
		_change = true;
		if (writer_type != WOTE_INSERT){
			writer_type = WOTE_UPDATE;
		}
	}

	return true;
}

bool  activity_record::remove_person_reward_flag(uint32 role_id,uint32 flag)
{
	for (int i = 0; i < mut_db_info().flag_info_size(); ++i){
		if (mut_db_info().flag_info(i).role_id() == role_id){
			mut_db_info().mutable_flag_info(i)->set_rflag(flag);
			_change = true;
			if (writer_type != WOTE_INSERT){
				writer_type = WOTE_UPDATE;
			}
		}
	}

	return true;
}

bool activity_record::remove_record_count(uint32 role_id,uint32 buy_type,uint32 act_type,uint32 delete_count)
{
	const client::act_db_info& db_info = get_act_db_info();
	if (buy_type == 1){
		for (int i = 0; i < mut_db_info().person_item_size(); ++i){
			if (db_info.person_item(i).activity_type() == act_type && db_info.person_item(i).role_id() == role_id){
				mut_db_info().mutable_person_item(i)->set_item_count(db_info.person_item(i).item_count() - delete_count);
			}
		}
	}else if (buy_type == 2){
		for (int j = 0; j < mut_db_info().center_item_size(); ++j){
			mut_db_info().mutable_center_item(j)->set_item_count(db_info.center_item(j).item_count() - delete_count);
		}
	}else{
		return false;
	}

	return true;
}

bool activity_record::check_reset()
{
	if (!today_is_reset()){
		client::act_db_info& db_info = mut_db_info();

		for (int i = 0; i < db_info.center_item_size(); ++i){
			const config_activity_item* act_item = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(db_info.mutable_center_item(i)->activity_type());
			if (act_item == NULL){
				continue;
			}
			if (act_item->get_is_reset() == 1){
				// ÖØÖÃ
				db_info.mutable_center_item(i)->Clear();
			}
		}

		for (int j = 0; j < db_info.flag_info_size(); ++j){
			const config_activity_item* act_item = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(db_info.mutable_flag_info(j)->activity_type());
			if (act_item == NULL){
				continue;
			}

			if (act_item->get_is_reset() == 1){
				// ÖØÖÃ
				db_info.mutable_flag_info(j)->Clear();
			}
		}

		for (int k = 0;k < db_info.person_item_size(); ++ k){
			const config_activity_item* act_item = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(db_info.mutable_person_item(k)->activity_type());
			if (act_item == NULL){
				continue;
			}

			if (act_item->get_is_reset() == 1){
				// ÖØÖÃ
				db_info.mutable_person_item(k)->Clear();
			}
		}

		set_today_is_reset(true);
	}

	return true;
}