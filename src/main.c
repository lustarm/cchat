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

const int port = 8000;

int main(void)
{
    // ! active user list
    users users;

    // ! server struct
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
        goto cleanup;
    }

    s.running = true;

    s.addr.sin_addr.s_addr = INADDR_ANY;
    s.addr.sin_port = htons(port);
    s.addr.sin_family = AF_INET;

    int addrlen = sizeof(s.addr);

    if(bind(s.sockfd, (struct sockaddr *)&s.addr, sizeof(s.addr)) < 0)
    {
        LOG_ERROR("Failed to bind to port %d", port);
        goto cleanup;
    }

    if(listen(s.sockfd, 3) != 0)
    {
        LOG_ERROR("Failed to listen on port %d", port);
        goto cleanup;
    }

    LOG_INFO("Listening on port 8000");

    while(s.running)
    {
        client_t c;

        c.sockfd = accept(s.sockfd, (struct sockaddr*)&s.addr,
                             (socklen_t*)&addrlen);

        if (c.sockfd < 0)
        {
            LOG_ERROR("Failed to accept client");
            close(c.sockfd);
            continue;
        }

        pthread_t thread_id;
        int p_ =  pthread_create(&thread_id, NULL, handle, (void*)&c);

        if (p_ < 0)
        {
            LOG_ERROR("Thread creation failed");
            close(c.sockfd);
        }
    }

cleanup:
    close(s.sockfd);
    return 0;
}
