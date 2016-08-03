#include "stdafx.h"
#include "amtsvr_monitor.h"
#include "amtsvr_server.h"
#include "amtsvr.h"

#include <stdlib.h>
#include <string.h>

struct amtsvr_monitor {
	int version;
	int check_version;
	uint32_t source;
	uint32_t destination;
};

struct amtsvr_monitor * 
amtsvr_monitor_new() {
	struct amtsvr_monitor * ret = (struct amtsvr_monitor *)malloc(sizeof(*ret));
	memset(ret, 0, sizeof(*ret));
	return ret;
}

void 
amtsvr_monitor_delete(struct amtsvr_monitor *sm) {
	free(sm);
}

void 
amtsvr_monitor_trigger(struct amtsvr_monitor *sm, uint32_t source, uint32_t destination) {
	sm->source = source;
	sm->destination = destination;
	__sync_fetch_and_add(&sm->version , 1);
}

void 
amtsvr_monitor_check(struct amtsvr_monitor *sm) {
	if (sm->version == sm->check_version) {
		if (sm->destination) {
			amtsvr_context_endless(sm->destination);
			amtsvr_syslog(NULL, "A message from [ :%08x ] to [ :%08x ] maybe in an endless loop", sm->source , sm->destination);
		}
	} else {
		sm->check_version = sm->version;
	}
}
