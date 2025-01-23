#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"

typedef struct {
    uint8_t buf[1024];
    int sockfd;
    bool running;
    struct sockaddr_in addr;
}client_t;

static void send_str(client_t* c)
{
    assert(c->sockfd > 0);

    write(c->sockfd, c->buf, sizeof(c->buf));
    memset(c->buf, 0, sizeof(c->buf));
}

static void read_str(client_t* c)
{
    assert(c->sockfd > 0);

    uint8_t buf_[1024];
    read(c->sockfd, buf_, 1024);

    memset(c->buf, 0, sizeof(c->buf));
    memcpy(buf_, c->buf, sizeof(c->buf));
}

int main(int argc, char *argv[])
{
    client_t c;

    c.running = true;

    c.addr.sin_addr.s_addr = INADDR_ANY;
    c.addr.sin_port = htonl(8000);
    c.addr.sin_family = AF_INET;

    c.sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (c.sockfd < 0) {
        LOG_ERROR("Failed to create socket");
        goto cleanup;
    }

    if(setsockopt(c.sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
               &(int){1}, sizeof(&(int){1})))
    {
        LOG_ERROR("Failed to setup socket options");
        goto cleanup;
    }

    if(bind(c.sockfd, (struct sockaddr *)&c.addr, sizeof(struct sockaddr*)) < 0)
    {
        LOG_ERROR("Failed to bind to port 8000");
        goto cleanup;
    }

    if(listen(c.sockfd, 3) < 0)
    {
        LOG_ERROR("Failed to listen on port 8000");
        goto cleanup;
    }

    while(c.running)
    {

    }

cleanup:
    close(c.sockfd);
    return 0;
}
