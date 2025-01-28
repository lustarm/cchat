#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include "c.h"
#include "log.h"
#include "util.h"

void trim_newline(char* buf)
{
    buf[strcspn(buf, "\n")] = 0;
}

void srw(client_t* c)
{
    c->running = true;

    while(c->running)
    {
        read_str(c);
        trim_newline((char*)c->buf);

        LOG_DEBUG("%s", c->buf);

        send_str(c, "-> ");
    }
}

void *handle(void* c)
{
    LOG_INFO("Client accepted");

    client_t client = *(client_t *)c;

    assert(client.sockfd > 0);

    send_str(&client, "-> ");

    srw(&client);

cleanup:
    close(client.sockfd);

    pthread_exit(NULL);
}

