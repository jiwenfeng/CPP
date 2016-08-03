#ifdef _WIN32
#include <map>
#include "LZSGConfig.h"
#include "event.h"

struct fd_set readset;
struct fd_set listenset;

std::map<int, void*> events;

#ifdef __cplusplus
extern "C" {
#endif

int event_init(int max) {
    return 0;
}

int event_add(int efd, int sock, void *ud) {
    events[sock] = ud;
    if ((void *)((intptr_t)~0) == ud) {
        FD_SET(sock, &listenset);
    } else if (!FD_ISSET(sock, &readset)) {
        FD_SET(sock, &readset);
    }

    return 0;
}

void event_del(int efd, int fd) {
    FD_CLR(fd, &readset);
    events.erase(fd);
}

int event_wait(int efd, struct event * e, int timeout) {
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = timeout;
    struct fd_set temp;
    FD_ZERO(&temp);
    for (u_int i = 0; i < listenset.fd_count; ++i) {
        FD_SET(listenset.fd_array[i], &temp);
    }

    for (u_int i = 0; i < readset.fd_count; ++i) {
        FD_SET(readset.fd_array[i], &temp);
    }

    int ret = select(NULL, &temp, NULL, NULL, &tv);
    for (u_int i = 0; i < temp.fd_count; ++i) {
        //if (ret != 0 && ret != -1 && FD_ISSET(listenset.fd_array[i], &temp))
        e[i].s     = events[temp.fd_array[i]];
        e[i].write = false;
        e[i].read  = true;
    }

    return ret;
}

void event_write(int efd, int fd, void *ud, bool enable) {

}

#ifdef __cplusplus
}
#endif

#endif // _WIN32