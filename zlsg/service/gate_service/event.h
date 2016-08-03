#ifndef amtsvr_event_h
#define amtsvr_event_h

#ifndef _WIN32

#include <stdbool.h>
#include <stdlib.h>

/* Test for polling API */
#ifdef __linux__
#define HAVE_EPOLL 1
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#define HAVE_KQUEUE 1
#endif

#if !defined(HAVE_EPOLL) && !defined(HAVE_KQUEUE)
#error "system does not support epoll or kqueue API"
#endif
/* ! Test for polling API */

#include <sys/types.h>

#ifdef HAVE_EPOLL
#include <sys/epoll.h>
#elif HAVE_KQUEUE
#include <time.h>
#include <sys/event.h>
#endif

#define MAX_EVENT 32

struct event {
	void * s;
	bool read;
	bool write;
};

static int
event_init(int max) {
	return epoll_create(max);
}

static int
event_add(int efd, int sock, void *ud) {
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = ud;
	if (epoll_ctl(efd, EPOLL_CTL_ADD, sock, &ev) == -1) {
		return 1;
	}
	return 0;
}

static void
event_del(int efd, int fd) {
	epoll_ctl(efd, EPOLL_CTL_DEL, fd , NULL);
}

static int
event_wait(int efd, struct event * e, int timeout) {
	struct epoll_event ev[MAX_EVENT];
	int n = epoll_wait(efd , ev, MAX_EVENT, timeout);
	int i;
	for (i=0;i<n;i++) {
		e[i].s = ev[i].data.ptr;
		unsigned flag = ev[i].events;
		e[i].write = (flag & EPOLLOUT) != 0;
		e[i].read = (flag & EPOLLIN) != 0;
	}

	return n;
}

static void
event_write(int efd, int fd, void *ud, bool enable) {
	struct epoll_event ev;
	ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
	ev.data.ptr = ud;
	epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);
}

#else // _WIN32

#define MAX_EVENT 32

struct event {
	void * s;
	bool read;
	bool write;
};

#ifdef __cplusplus
extern "C" {
#endif

int  event_init (int max);
int  event_add  (int efd, int sock, void *ud);
void event_del  (int efd, int fd);
int  event_wait (int efd, struct event * e, int timeout);
void event_write(int efd, int fd, void *ud, bool enable);

#ifdef __cplusplus
}
#endif

#endif // _WIN32

#endif
