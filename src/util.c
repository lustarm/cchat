#include <assert.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "c.h"
#include "log.h"

void send_str(client_t* c, char buf[])
{
    assert(c->sockfd > 0);

    send(c->sockfd, buf, strlen(buf), 0);
}

void read_str(client_t* c)
{
    assert(c->sockfd > 0);

    ssize_t bytes_read = 0;
    memset(c->buf, 0, sizeof(c->buf));

    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds timeout
    timeout.tv_usec = 0;
    setsockopt(c->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    bytes_read = recv(c->sockfd, c->buf, sizeof(c->buf) - 1, 0);
    c->len = bytes_read;

    if (bytes_read < 0)
    {
        LOG_DEBUG("Failed to recieve data from client");
        return;
    }
    else if (bytes_read == 0)
    {
        LOG_INFO("Client disconnected");
        c->running = false;
    }
}

