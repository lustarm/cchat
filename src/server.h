#include <stdbool.h>
#include <arpa/inet.h>

typedef struct {
    int sockfd;
    bool running;
    struct sockaddr_in addr;
} server_t;

