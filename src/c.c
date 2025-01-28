#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include "c.h"
#include "cmd.h"
#include "log.h"
#include "util.h"

void trim(char* buf)
{
    buf[strcspn(buf, "\n")] = 0;
    buf[strcspn(buf, "\r")] = 0;
}

void test(client_t* c)
{
    LOG_DEBUG("Called test command");
    send_str(c, "this is a test\r\n");
}

void srw(client_t* c)
{
    commands_t cmds;

    memcpy(cmds[0].name, "test", sizeof(char [4]));
    cmds[0].fn = &test;

    c->running = true;

    while(c->running)
    {
        read_str(c);
        trim((char*)c->buf);

        if(memcmp(c->buf, cmds[0].name, strlen(cmds[0].name)) == 0)
        {
            cmds[0].fn(c);
        }

        else
        {
            send_str(c, "Invalid command\r\n");
        }

        send_str(c, "-> ");
    }
}

void *handle(void* c)
{
    LOG_INFO("Client accepted");

    client_t client = *(client_t *)c;

    assert(client.sockfd > 0);

    send_str(&client, "-> ");

    srw(&client);

cleanup:
    close(client.sockfd);

    pthread_exit(NULL);
}

