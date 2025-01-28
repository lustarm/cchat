#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include "c.h"
#include "cmd.h"
#include "log.h"
#include "util.h"
#include "bot.h"

void trim(char* buf)
{
    buf[strcspn(buf, "\n")] = 0;
    buf[strcspn(buf, "\r")] = 0;
}

void test(client_t* c)
{
    send_str(c, "this is a test\r\n");
}

void exit_cmd(client_t* c)
{
    send_str(c, "Exiting...\r\n");
}

void srw(client_t* c)
{
    commands_t cmds;

    memcpy(cmds[0].name, "test", sizeof(char [4]));
    cmds[0].fn = &test;

    memcpy(cmds[1].name, "exit", sizeof(char[4]));
    cmds[0].fn = &exit_cmd;

    c->running = true;

    while(c->running)
    {
        read_str(c);

        if(strcmp(c->buf, "\x00\x00\x00\x01"))
        {
            bot(c);
            if(!c->running)
                continue;
        }

        LOG_DEBUG("Not bot");

        trim((char*)c->buf);

        if(memcmp(c->buf, cmds[0].name, strlen(cmds[0].name)) == 0)
        {
            cmds[0].fn(c);
        }
        else if (memcmp(c->buf, cmds[1].name, sizeof(cmds[1].name)) == 0)
        {
            cmds[1].fn(c);
        }
        else
        {
            send_str(c, "Invalid command\r\n");
        }

        send_str(c, "-> ");
    }

    c->running = false;
    close(c->sockfd);
}

void *handle(void* c)
{
    client_t client = *(client_t *)c;

    assert(client.sockfd > 0);

    send_str(&client, "-> ");

    srw(&client);

    pthread_exit(NULL);
}

