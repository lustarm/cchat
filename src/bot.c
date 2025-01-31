#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>

#include "bot.h"
#include "log.h"

void bot(bot_t* b)
{
    LOG_INFO("Got bot");

    while(b->running)
    {
        send(b->sockfd, "ping", 4, 0);
        sleep(10);
    }
}
