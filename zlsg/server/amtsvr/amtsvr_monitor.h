#ifndef AMTSVR_MONITOR_H
#define AMTSVR_MONITOR_H

#include <stdint.h>

struct amtsvr_monitor;

struct amtsvr_monitor * amtsvr_monitor_new();
void amtsvr_monitor_delete(struct amtsvr_monitor *);
void amtsvr_monitor_trigger(struct amtsvr_monitor *, uint32_t source, uint32_t destination);
void amtsvr_monitor_check(struct amtsvr_monitor *);

#endif
