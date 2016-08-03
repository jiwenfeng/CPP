#include "boss_mgr.h"
#include "base_time.h"
#include "client.operate_mail.pb.h"
#define BEFORE_TIMELONG 300
#define AFTER_TIMELONG  300
#define BROADCAST		600

int boss_mgr::broadcast_refresh_msg(amtsvr_context *ctx, bool &ret)
{
	ret = true;
	client::boss_refresh_msg_rsp rsp;
	rsp.set_time((uint32)time(NULL));
	send_center(ctx, rsp);
	return 0;
}

int boss_mgr::update(amtsvr_context *ctx)
{
	uint32 now = (uint32)time(NULL);
	if(now >= m_before && !m_before_send)
	{
		broadcast_refresh_msg(ctx, m_before_send);
	}
	if(now >= _broadcast && !m_broadcast)
	{
		client::broadcast_msg_req req;
		req.set_area_id(39);
		send_center(ctx, req);
		m_broadcast = true;
	}
	if(now >= m_refresh_time)
	{
		refresh_world_boss(ctx);
	}
	if(now >= m_after && !m_after_send)
	{
		broadcast_refresh_msg(ctx, m_after_send);
		m_after = m_refresh_time + AFTER_TIMELONG;
		m_after_send = false;
	}
	return 0;
}

uint32 boss_mgr::get_next_refresh_time()
{
	const char *t[] = {"10:30:00", "14:30:00", "18:30:00", "22:30:00"};
#define ss sizeof(t) / sizeof(t[0])
	uint32 now = (uint32)time(NULL);
	for(size_t i = 0; i < ss; i++)
	{
		uint32 tt = time_utils::make_time(now, t[i]);
		if(now < tt)
		{
			return tt;
		}
	}
	return time_utils::make_time_special(0, 10, 30, 0);
}

int boss_mgr::init()
{
	m_refresh_time = get_next_refresh_time();
	m_before = m_refresh_time - BEFORE_TIMELONG;
	m_after = m_refresh_time + AFTER_TIMELONG;
	_broadcast = m_refresh_time - BROADCAST;

	m_before_send = false;
	m_after_send = false;
	m_broadcast = false;
	return 0;
}

void boss_mgr::refresh_world_boss(amtsvr_context *ctx)
{
	m_refresh_time = get_next_refresh_time();
	m_before = m_refresh_time - BEFORE_TIMELONG;
	m_before = false;

	_broadcast = m_refresh_time - BROADCAST;
	m_broadcast = false;

	vector<uint32>::iterator it = m_boss_scene.begin();
	client::boss_refresh_req req;
	req.set_time(m_refresh_time);
	for(; it != m_boss_scene.end(); ++it)
	{
		send_game(ctx, req, *it);
	}
}

int boss_mgr::send_game(amtsvr_context *ctx, google::protobuf::Message &coi, uint32 ssi)
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
	return amtsvr_send(ctx, 0, ssi, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int boss_mgr::send_center(amtsvr_context *ctx, google::protobuf::Message &coi)
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

void boss_mgr::change_boss_status(uint32 raid_id, uint32 status)
{
	_boss_status[raid_id] = status;
}

uint32 boss_mgr::get_boss_status(uint32 raid_id)
{
	return _boss_status[raid_id];
}
