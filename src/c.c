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
    LOG_INFO("Client accepted");

    send_str(client.sockfd, "Hello!");

    pthread_exit(NULL);
}

