#pragma once

#include <netinet/in.h>
#include <stdint.h>
#include <stdbool.h>
#include <arpa/inet.h>

// ! if u need more thats just a lie
#define MAX_USERS 1024

typedef struct client_t{
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

    // ! user?
    bool user;
} client_t;

// heap allocated users array
typedef client_t* users[MAX_USERS];

void *handle(void*);
