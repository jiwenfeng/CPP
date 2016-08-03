#include "node_interface.h"


int node_dispatcher_init(struct node_service_t* w)
{
    return 0;
}

int node_send_server(struct node_service_t* w, const char* buf, int len)
{
    int ret      = 0;
    int send_len = 0;
    do {
#ifdef _WIN32
        ret = send(w->socket, buf + send_len, len - send_len, 0);
#else 
        ret = write(w->socket, buf + send_len, len - send_len);
#endif // _WIN32
        if (ret > 0) {
            send_len += ret;
        } else {
            
        }
    } while (send_len < len);

    return 0;
}
