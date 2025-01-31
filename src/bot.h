#pragma once

#include "c.h"

typedef struct
{
    // ! universal buffer
    char buf[1024];

    // ! universal length
    ssize_t len;

    // ! socket
    int sockfd;

    // ! client info
    struct sockaddr_in addr;

    // ! running
    bool running;

    // ! version
    uint8_t version;

    char name[32];
} bot_t;

void bot(bot_t*);
