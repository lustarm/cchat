#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

void send_str(int fd, char buf[])
{
    assert(fd > 0);

    write(fd, buf, strlen(buf));
}

void read_str(int fd)
{
    assert(fd > 0);

    uint8_t buf_[1024];
    read(fd, buf_, 1024);
}
