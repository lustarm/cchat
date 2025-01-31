#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include "bot.h"
#include "log.h"

void bot(bot_t* b)
{
    char *p = inet_ntoa(b->addr.sin_addr);
    LOG_INFO("Bot client connected: %s", p);

    while(b->running)
    {
        send(b->sockfd, "ping", 4, 0);
        sleep(10);
    }
}
