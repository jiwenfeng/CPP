#ifndef AMTSVR_MESSAGE_QUEUE_H
#define AMTSVR_MESSAGE_QUEUE_H

#include <stdlib.h>
#include <stdint.h>

struct amtsvr_message {
	uint32_t source;
	int session;
	void * data;
	size_t sz;
};

struct message_queue;

struct message_queue * amtsvr_globalmq_pop(void);

struct message_queue * amtsvr_mq_create(uint32_t handle);
void amtsvr_mq_mark_release(struct message_queue *q);
int amtsvr_mq_release(struct message_queue *q);
void amtsvr_mq_release_all();
uint32_t amtsvr_mq_handle(struct message_queue *);

// 0 for success
int amtsvr_mq_pop(struct message_queue *q, struct amtsvr_message *message);
void amtsvr_mq_push(struct message_queue *q, struct amtsvr_message *message);
void amtsvr_mq_lock(struct message_queue *q, int session);

void amtsvr_mq_force_push(struct message_queue *q);
void amtsvr_mq_pushglobal(struct message_queue *q);

void amtsvr_mq_init();

int get_amtsvr_mq_count(struct message_queue *q);
void* get_amtsvr_mq_data(struct message_queue *q, int index);

#endif
