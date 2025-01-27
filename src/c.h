#pragma once

#include <stdint.h>
#include <arpa/inet.h>

typedef struct client_t{
    uint8_t buf[1024];
    int sockfd;
} client_t;

// heap allocated client
typedef client_t users[];

void *handle(void*);
