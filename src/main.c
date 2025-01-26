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

    s.sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(s.sockfd == 0)
    {
        LOG_ERROR("Failed to create socket");
        goto cleanup;
    }

    if(setsockopt(s.sockfd, SOL_SOCKET, SO_REUSEADDR, (int[]){1}, sizeof(int)) < 0)
    {
        LOG_ERROR("Failed to setup socket options");
        perror("bruh");
        goto cleanup;
    }

    s.running = true;

    s.addr.sin_addr.s_addr = INADDR_ANY;
    s.addr.sin_port = htons(8000);
    s.addr.sin_family = AF_INET;

    if(bind(s.sockfd, (struct sockaddr *)&s.addr, sizeof(s.addr)) < 0)
    {
        LOG_ERROR("Failed to bind to port 8000");
        goto cleanup;
    }

    if(listen(s.sockfd, 3) != 0)
    {
        LOG_ERROR("Failed to listen on port 8000");
        goto cleanup;
    }

    LOG_INFO("Listening on port 8000");

    int addrlen = sizeof(s.addr);

    while(s.running)
    {
        // Create new heap client
        client_t *c = malloc(sizeof(client_t));
        if(!c)
        {
            LOG_ERROR("Get more ram nerd :)");
            goto cleanup;
        }

        c->sockfd = accept(s.sockfd, (struct sockaddr*)&s.addr,
                             (socklen_t*)&addrlen);

        if (c->sockfd < 0)
        {
            LOG_ERROR("Failed to accept client");
            close(c->sockfd);
            continue;
        }

        pthread_t thread_id;
        int p_ =  pthread_create(&thread_id, NULL, handle, (void *)c);
        LOG_DEBUG("%d", thread_id);

        if (p_ < 0)
        {
            LOG_ERROR("Thread creation failed");
            close(c->sockfd);
            free(c);
        }

        LOG_DEBUG("Closing thread %d", thread_id);
    }

cleanup:
    close(s.sockfd);
    return 0;
}
