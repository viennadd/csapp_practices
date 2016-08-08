//
// Created by vienna on 30/7/2016.
//
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "sysio.h"


int main()
{
    rio_t rio_buf;
    int fd = open("/etc/services", O_RDONLY);
    char c;
    rio_initb(&rio_buf, fd);
    for (int i = 0; i < 1000000; ++i) {
        read(fd, &c, 1);
//        rio_readnb(&rio_buf, &c, 1);
    }

    return 0;
}