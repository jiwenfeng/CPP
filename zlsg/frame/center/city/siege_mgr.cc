#include "siege_mgr.h"
#include "client.city.pb.h"

#define ACTIVITY_INTERVAL 7 * 24 * 3600

siege_mgr::siege_mgr():_data(NULL), _ctx(NULL)
{
	writer_type = WOTE_INSERT;
}

siege_mgr::~siege_mgr()
{
	if(_data != NULL)
	{
		delete _data;
		_data = NULL;
	}
}

void siege_mgr::reset_siege_info()
{
	_open_time = calc_open_time();
	reset_status();
	_temp_id = _data == NULL ? 0 : _data->family_id();
}

int siege_mgr::send_center(amtsvr_context *ctx, google::protobuf::Message &coi)
{
	google::protobuf::Message *new_msg = create_msg(coi.GetTypeName());
	if(new_msg == NULL)
	{
		return -3;
	}
	new_msg->CopyFrom(coi);
	msg_handler *mh = MSG_HANDLER_POOL->pop();
	if(mh == NULL)
	{
		delete new_msg;
		return -4;
	}
	mh->msg = new_msg;
	return amtsvr_sendname(ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int siege_mgr::send_game(amtsvr_context *ctx, google::protobuf::Message &coi)
{
	if(0 == _scene_svr_id)
	{
		return -1;
	}
	google::protobuf::Message *new_msg = create_msg(coi.GetTypeName());
	if(new_msg == NULL)
	{
		return -3;
	}
	new_msg->CopyFrom(coi);
	msg_handler *mh = MSG_HANDLER_POOL->pop();
	if(mh == NULL)
	{
		delete new_msg;
		return -4;
	}
	mh->msg = new_msg;
	return amtsvr_send(ctx, 0, _scene_svr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

void siege_mgr::init(amtsvr_context *ctx)
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_city_attack_info();
	while(cur != NULL)
	{
		mysql::tb_city_attack *tca = static_cast<mysql::tb_city_attack *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tca)
		{
			continue;
		}
		_data = tca;
		writer_type = WOTE_UPDATE;
		break;
	}
	_ctx = ctx;
	reset_siege_info();
}

uint32 siege_mgr::calc_open_time()
{
	uint32 now = (uint32)time(NULL);
	// 第一次开战时间
	uint32 n = time_utils::get_time(amtsvr_svr_start_time(), 3, 20, 0, 0);
	if(now < n)
	{
		return n;
	}
	// 理论上第n+1次开战时间
	uint32 t = time_utils::make_time_special(0, 5, 20, 0, 0);
	if(t - n >= ACTIVITY_INTERVAL) 
	{
		return t;
	}
	return t + ACTIVITY_INTERVAL;
}

int siege_mgr::send_30_min_notice(amtsvr_context *ctx)
{
	change_status();
	client::broadcast_msg_req req;
	req.set_area_id(24);
	return send_center(ctx, req);
}

int siege_mgr::send_10_min_notice(amtsvr_context *ctx)
{
	change_status();

	broadcast_act_status(ctx);

	client::siege_field_open sfo;		// 创建战场场景
	sfo.set_time(0);
	send_game(ctx, sfo);

	client::broadcast_msg_req req;
	req.set_area_id(25);
	return send_center(ctx, req);
}

int siege_mgr::do_siege_start(amtsvr_context *ctx)
{
	change_status();
	client::siege_start ss;
	uint32 now = time_utils::tenms();
	ss.set_open_time(now);
	ss.set_close_time(now + (uint32)CONFMGR->get_basic_value(10049) * 100);
	ss.set_family_id(get_family_id());
	_temp_id = get_family_id();
	return send_center(ctx, ss);
//	return send_game(ctx, ss);
}

int siege_mgr::save()
{
	if(NULL == _data)
	{
		return -1;
	}
	mysql::tb_city_attack *tca = new mysql::tb_city_attack(*_data);
	CACHE_KEY k(tca->id(), 0);
	write(_ctx, k, tca);
	writer_type = WOTE_UPDATE;
	return 0;
}

int siege_mgr::update(amtsvr_context *ctx)
{
	uint32 now = (uint32)time(NULL);
	uint32 status = get_status();
	if(status == 0 && now >= get_open_time() - BROADCAST_THIRTY)
	{
		return send_30_min_notice(ctx);
	}
	if(status == 1 && now >= get_open_time() - BROADCAST_TEN)
	{
		return send_10_min_notice(ctx);
	}
	if(status == 2 && now >= get_open_time())
	{
		return do_siege_start(ctx);
	}
	return 0;
}

void siege_mgr::reset_data(uint32 family_id)
{
	if(0 == family_id)
	{
		return;
	}
	if(NULL == _data)
	{
		_data = new mysql::tb_city_attack;
		_data->set_title("");
		_data->set_id(SIEGE_ID);
	}
	_data->set_family_id(family_id);
	_data->set_occupy_time((uint32)time(NULL));
	_data->set_mod_title_time(0);
	save();
}

void siege_mgr::on_siege_over(uint32 family_id)
{
	reset_data(family_id);
	reset_siege_info();
	broadcast_act_status(_ctx);
}

int siege_mgr::open_act(uint32 tl)
{
	if(get_status() != 0)
	{
		return -1;
	}
	_open_time = (uint32)time(NULL) + tl;
	return 0;
}

const string &siege_mgr::get_title()
{
	if(_data == NULL)
	{
		static string str = "";
		return str;
	}
	return _data->title();
}

void siege_mgr::set_city_title(const string &str)
{
	if(_data == NULL)
	{
		return;
	}
	_data->set_title(str);
	_data->set_mod_title_time((uint32)time(NULL));
	save();
}

int siege_mgr::broadcast_act_status(amtsvr_context *ctx)
{
	client::siege_status_rsp rsp;
	rsp.set_status(get_act_status());
	send_center(ctx, rsp);
	return 0;
}
