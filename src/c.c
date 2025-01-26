#include <assert.h>
#include <unistd.h>

#include <pthread.h>

#include "c.h"
#include "log.h"
#include "util.h"

void *handle(void* c)
{
    LOG_INFO("Client accepted");

    client_t client = *(client_t *)c;

    assert(client.sockfd > 0);

    LOG_DEBUG("%s | %d", "bruv", client.sockfd);

    send_str(client.sockfd, "Hello!\n");

    pthread_exit(NULL);
}

