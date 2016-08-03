#ifndef AMTSVR_TIMER_H
#define AMTSVR_TIMER_H

#include <stdint.h>

int amtsvr_timeout(uint32_t handle, int time, int session);
void amtsvr_updatetime(void);
uint32_t amtsvr_gettime(void);
uint32_t amtsvr_gettime_fixsec(void);

void amtsvr_timer_init(void);
void amtsvr_timer_release(void);

#endif
