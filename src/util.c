#include <assert.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>

#include "c.h"

void send_str(client_t* c, char buf[])
{
    assert(c->sockfd > 0);

    send(c->sockfd, buf, strlen(buf), 0);
}

void read_str(client_t* c)
{
    assert(c->sockfd > 0);

    recv(c->sockfd, c->buf, 1024, 0);
}
