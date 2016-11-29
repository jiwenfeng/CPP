//$Id: callout.cpp 107163 2010-01-15 09:19:24Z tony@NETEASE.COM $
//$Author: shavingha $
#include <strings.h>
#include <math.h>
#include "callout.hpp"
#include "hash.hpp"

//#define evtimer_once(ev, cb, arg) event_once(ev, -1, 0, cb, arg)

struct ev_manager_t{
	evdata* evdata_header;
	int total_evdata;
	int free_evdata;
}ev_manager;

int get_total_ev()
{
	return ev_manager.total_evdata;
}

int get_free_ev()
{
	return ev_manager.free_evdata;
}

static evdata* new_evdata()
{
	evdata* ret = NULL;
	if(ev_manager.evdata_header)
	{
		ret = ev_manager.evdata_header;
		ev_manager.evdata_header = ret->next;
		ev_manager.free_evdata--;
	}
	else
	{
		ret = (evdata * )malloc(sizeof(evdata));
		assert(ret);
		bzero(ret, sizeof(evdata));
		ev_manager.total_evdata++;
	}
	return ret;
}

static void free_evdata(evdata* ptr)
{
	ptr->next = ev_manager.evdata_header;
	ev_manager.evdata_header = ptr;
	ev_manager.free_evdata++;
}

unsigned int handler = 0;
//Ϊ��׳�Կ��ǣ����ܽ�evdata��ϸ�ڷ��ظ��ű��㴦��.��������ظ�ɾ��������ɾ�����ڵ�timer��ᵼ��crash.
//���ｨ��һ����Ӳ㣬����evdata��handler���ű�������safe��ɾ��.
hash_table * call_hash; 

void init_callout()
{
	call_hash = init_hash_table(4096);
	bzero(&ev_manager, sizeof(ev_manager));
}

static void _oncecb(int fd, short event, void * arg)
{
	evdata * ed = (evdata *)arg;
	delete_inode(call_hash, ed->handler);
	ed->cb(ed->arg);
	free_evdata(ed);
}

//timeout��֮��ִ��һ��cb()
unsigned int call_once(callcb cb, void * arg, float timeout)
{
	evdata * ed = new_evdata();
	ed->cb = cb;
	ed->arg = arg;
	ed->handler = (handler++);
	ed->tv.tv_sec = timeout;
	ed->tv.tv_usec = (timeout - floor(timeout))* 1000000;

	evtimer_set(&(ed->ev), _oncecb, (void *)ed);
	evtimer_add(&(ed->ev), &(ed->tv));

	insert_inode(call_hash, ed->handler, (void *)ed);
	
	return ed->handler;
}

static void _multicb(int fd, short event, void * arg)
{
	evdata * ed = (evdata *)arg;
	evtimer_set(&(ed->ev), _multicb, (void *)ed);
	evtimer_add(&(ed->ev), &(ed->tv)); //�ټ�һ�Ρ�
	ed->cb(ed->arg);
}

//��timeout��Ƶ��ִ�ж��
unsigned int call_multi(callcb cb, void * arg, float timeout)
{
	evdata * ed = new_evdata();
	ed->cb = cb;
	ed->arg = arg;
	ed->handler = (handler++);
	ed->tv.tv_sec = timeout;
	ed->tv.tv_usec = (timeout - floor(timeout))* 1000000;
	evtimer_set(&(ed->ev), _multicb, (void *)ed);
	evtimer_add(&(ed->ev), &(ed->tv));

	insert_inode(call_hash, ed->handler, (void *)ed);
	return ed->handler;
}

bool rm_call(unsigned int handler, void ** arg)
{
	void * p;
	evdata * ed;
	if (!find_inode(call_hash, handler, &p)) {
		return false;
	}
	ed = (evdata*)p;
	assert(handler == ed->handler);
	//assert(evtimer_pending(&(ed->ev), NULL));
	evtimer_del(&(ed->ev));
	delete_inode(call_hash, ed->handler);
	*arg = ed->arg;
	free_evdata(ed);
	return true; //���������ظ����÷�.
}
