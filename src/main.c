#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#include "log.h"
#include "c.h"
#include "server.h"

int main(void)
{
    server_t s;

    if((s.sockfd = socket(AF_INET, SOCK_STREAM, 0) == 0))
    {
        LOG_ERROR("Failed to create socket");
        goto cleanup;
    }

    if(setsockopt(s.sockfd, SOL_SOCKET, SO_REUSEADDR,
               &(int){1}, sizeof(int)) < 0)
    {
        LOG_ERROR("Failed to setup socket options");
        perror("bruh");
        goto cleanup;
    }

    s.running = true;

    s.addr.sin_addr.s_addr = INADDR_ANY;
    s.addr.sin_port = htonl(8000);
    s.addr.sin_family = AF_INET;

    if(bind(s.sockfd, (struct sockaddr *)&s.addr, sizeof(s.addr)) < 0)
    {
        LOG_ERROR("Failed to bind to port 8000");
        goto cleanup;
    }

    LOG_DEBUG("Binded to port 8000");

    if(listen(s.sockfd, 3) < 0)
    {
        LOG_ERROR("Failed to listen on port 8000");
        goto cleanup;
    }

    LOG_INFO("Listening on port 8000");

    int addrlen = sizeof(s.addr);
    while(s.running)
    {
        // Create new heap client
        client_t *c = malloc(sizeof(client_t*));

        if((c->sockfd = accept(s.sockfd, (struct sockaddr*)&s.addr,
                             (socklen_t*)&addrlen)) < 0)
        {
            LOG_ERROR("Failed to accept client");
            close(c->sockfd);
            continue;
        }
        // Cant get passed this

        LOG_DEBUG("Got client starting thread");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle, (void *)&c) < 0)
        {
            LOG_ERROR("Thread creation failed");
            free(c);
            close(c->sockfd);
        }

        LOG_DEBUG("Closing thread %d", thread_id);
    }

cleanup:
    close(s.sockfd);
    return 0;
}
