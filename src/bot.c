#include <pthread.h>
#include <unistd.h>

#include "bot.h"
#include "c.h"
#include "log.h"
#include "util.h"

void bot(client_t* c)
{
    LOG_INFO("Got bot");

    while(c->running)
    {
        send_str(c, "ping");
        sleep(10);
    }
}
