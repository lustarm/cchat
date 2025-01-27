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

void *handle(void* c)
{
    LOG_INFO("Client accepted");

    client_t client = *(client_t *)c;

    assert(client.sockfd > 0);

    send_str(&client, "-> ");

    while(1)
    {
        read_str(&client);
        client.buf[strcspn((char*)client.buf, "\n")] = 0;
        LOG_DEBUG("%s", client.buf);

        send_str(&client, "-> ");
    }

cleanup:
    close(client.sockfd);

    pthread_exit(NULL);
}

