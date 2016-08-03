#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_mq.h"
#include "amtsvr_handle.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef _WIN32
#include <stdbool.h>
#endif // _WIN32

#define DEFAULT_QUEUE_SIZE 64
#define MAX_GLOBAL_MQ 0x10000

// 0 means mq is not in global mq.
// 1 means mq is in global mq , or the message is dispatching.
// 2 means message is dispatching with locked session set.
// 3 means mq is not in global mq, and locked session has been set.

#define MQ_IN_GLOBAL 1
#define MQ_DISPATCHING 2
#define MQ_LOCKED 3

struct message_queue {
	uint32_t handle;
	int cap;
	int head;
	int tail;
	int lock;
	int release;
	int lock_session;
	int in_global;
	struct amtsvr_message *queue;
};

struct global_queue {
	uint32_t head;
	uint32_t tail;
	struct message_queue ** queue;
	bool * flag;

};

static struct global_queue *Q = NULL;

#define LOCK(q) while (__sync_lock_test_and_set(&(q)->lock,1)) {}
#define UNLOCK(q) __sync_lock_release(&(q)->lock);

#define GP(p) ((p) % MAX_GLOBAL_MQ)

static void 
amtsvr_globalmq_push(struct message_queue * queue) {
	struct global_queue *q= Q;

	uint32_t tail = GP(__sync_fetch_and_add(&q->tail,1));
	q->queue[tail] = queue;
	__sync_synchronize();
	q->flag[tail] = true;
	__sync_synchronize();
}

struct message_queue * 
amtsvr_globalmq_pop() {
	struct global_queue *q = Q;
	uint32_t head =  q->head;
	uint32_t head_ptr = GP(head);
	if (head_ptr == GP(q->tail)) {
		return NULL;
	}

	if(!q->flag[head_ptr]) {
		return NULL;
	}

	struct message_queue * mq = q->queue[head_ptr];
	if (!__sync_bool_compare_and_swap(&q->head, head, head + 1)) {
#ifdef _WIN32
        if (q->head != 1 && head != 0) {
            return NULL;
        }
#else
		return NULL;
#endif // _WIN32
	}

	q->flag[head_ptr] = false;
	__sync_synchronize();

	return mq;
}

struct message_queue * 
amtsvr_mq_create(uint32_t handle) {
	struct message_queue *q = (struct message_queue *)malloc(sizeof(*q));
	q->handle = handle;
	q->cap = DEFAULT_QUEUE_SIZE;
	q->head = 0;
	q->tail = 0;
	q->lock = 0;
	q->in_global = MQ_IN_GLOBAL;
	q->release = 0;
	q->lock_session = 0;
	q->queue = (struct amtsvr_message *)malloc(sizeof(struct amtsvr_message) * q->cap);

	return q;
}

static void 
_release(struct message_queue *q) {
	free(q->queue);
	free(q);
}

uint32_t 
amtsvr_mq_handle(struct message_queue *q) {
	return q->handle;
}


int
amtsvr_mq_pop(struct message_queue *q, struct amtsvr_message *message) {
	int ret = 1;
	LOCK(q)

	if (q->head != q->tail) {
		*message = q->queue[q->head];
		ret = 0;
		if ( ++ q->head >= q->cap) {
			q->head = 0;
		}
	}

	if (ret) {
		q->in_global = 0;
	}
	
	UNLOCK(q)

	return ret;
}

static void
expand_queue(struct message_queue *q) {
	struct amtsvr_message *new_queue = (struct amtsvr_message *)malloc(sizeof(struct amtsvr_message) * q->cap * 2);
	int i;
	for (i=0;i<q->cap;i++) {
		new_queue[i] = q->queue[(q->head + i) % q->cap];
	}
	q->head = 0;
	q->tail = q->cap;
	q->cap *= 2;
	
	free(q->queue);
	q->queue = new_queue;
}

static void 
_pushhead(struct message_queue *q, struct amtsvr_message *message) {
	int head = q->head - 1;
	if (head < 0) {
		head = q->cap - 1;
	}
	if (head == q->tail) {
		expand_queue(q);
		--q->tail;
		head = q->cap - 1;
	}

	q->queue[head] = *message;
	q->head = head;

	// this api use in push a unlock message, so the in_global flags must not be 0 , 
	// but the q is not exist in global queue.
	if (q->in_global == MQ_LOCKED) {
		amtsvr_globalmq_push(q);
		q->in_global = MQ_IN_GLOBAL;
	} else {
		assert(q->in_global == MQ_DISPATCHING);
	}
	q->lock_session = 0;
}

void 
amtsvr_mq_push(struct message_queue *q, struct amtsvr_message *message) {
	assert(message);
	LOCK(q)
	
	if (q->lock_session !=0 && message->session == q->lock_session) {
		_pushhead(q,message);
	} else {
		q->queue[q->tail] = *message;
		if (++ q->tail >= q->cap) {
			q->tail = 0;
		}

		if (q->head == q->tail) {
			expand_queue(q);
		}

		if (q->lock_session == 0) {
			if (q->in_global == 0) {
				q->in_global = MQ_IN_GLOBAL;
				amtsvr_globalmq_push(q);
			}
		}
	}
	
	UNLOCK(q)
}

void
amtsvr_mq_lock(struct message_queue *q, int session) {
	LOCK(q)
	assert(q->lock_session == 0);
	assert(q->in_global == MQ_IN_GLOBAL);
	q->in_global = MQ_DISPATCHING;
	q->lock_session = session;
	UNLOCK(q)
}

void 
amtsvr_mq_init() {
	struct global_queue *q = (struct global_queue *)malloc(sizeof(*q));
	memset(q,0,sizeof(*q));
	q->queue = (struct message_queue **)malloc(MAX_GLOBAL_MQ * sizeof(struct message_queue *));
    memset(q->queue, 0, MAX_GLOBAL_MQ * sizeof(struct message_queue *));
	q->flag = (bool *)malloc(MAX_GLOBAL_MQ * sizeof(bool));
	memset(q->flag, 0, sizeof(bool) * MAX_GLOBAL_MQ);
	Q=q;
}

void 
amtsvr_mq_force_push(struct message_queue * queue) {
	assert(queue->in_global);
	amtsvr_globalmq_push(queue);
}

void 
amtsvr_mq_pushglobal(struct message_queue *queue) {
	LOCK(queue)
	assert(queue->in_global);
	if (queue->in_global == MQ_DISPATCHING) {
		// lock message queue just now.
		queue->in_global = MQ_LOCKED;
	}
	if (queue->lock_session == 0) {
		amtsvr_globalmq_push(queue);
		queue->in_global = MQ_IN_GLOBAL;
	}
	UNLOCK(queue)
}

void 
amtsvr_mq_mark_release(struct message_queue *q) {
	assert(q->release == 0);
	q->release = 1;
}

static int
_drop_queue(struct message_queue *q) {
	// todo: send message back to message source
	struct amtsvr_message msg;
	int s = 0;
	while(!amtsvr_mq_pop(q, &msg)) {
		++s;
		free(msg.data);
	}
	_release(q);
	return s;
}

int 
amtsvr_mq_release(struct message_queue *q) {
	int ret = 0;
	LOCK(q)
	
	if (q->release) {
		UNLOCK(q)
		ret = _drop_queue(q);
	} else {
		amtsvr_mq_force_push(q);
		UNLOCK(q)
	}
	
	return ret;
}

void amtsvr_mq_release_all() {
    while (GP(Q->head) != GP(Q->tail)) {
        uint32_t head_ptr = GP(Q->head);
        if (Q->queue[head_ptr] == NULL) {
            continue;
        }

        struct message_queue *queue = Q->queue[head_ptr];
        for (int j = 0; j < queue->cap; ++j) {
            if (queue->queue[j].data == NULL) {
                continue;
            }

            free(queue->queue[j].data);
        }

        free(queue);
        ++Q->head;
    }

    free(Q->queue);
    Q->queue = NULL;
    free(Q->flag);
    Q->flag = NULL;
    free(Q);
}

void* get_amtsvr_mq_data(struct message_queue *q, int index)
{
    return q->queue[index].data;
}

int get_amtsvr_mq_count(struct message_queue *q)
{
    return q->tail;
}
