#include <assert.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>

void send_str(int fd, char buf[])
{
    assert(fd > 0);

    send(fd, buf, strlen(buf), 0);
}

void read_str(int fd)
{
    assert(fd > 0);

    uint8_t buf_[1024];
    recv(fd, buf_, 1024, 0);
}
