//
// Created by vienna on 30/7/2016.
//

#ifndef CSAPP_SYSIO_H
#define CSAPP_SYSIO_H



#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

// robust read/write, 会处理不足值
ssize_t rio_readn(int fd, void *buf, size_t size);


ssize_t rio_writen(int fd, void *buf, size_t size);


// 自动缓存的 read/write

#define RIO_BUFFER_SIZE (8192)
typedef struct
{
    int fd;
    int cnt_unread;
    char *buf_ptr;
    char buf[RIO_BUFFER_SIZE];
} rio_t;

// 不暴露给外部, 所以 static
static ssize_t rio_read(rio_t *p_rio, void *buf, size_t size);

ssize_t rio_readlineb(rio_t *p_rio, void *usrbuf, int maxlen);

ssize_t rio_readnb(rio_t *p_rio, void *buf, size_t size);

void rio_initb(rio_t *p_rio, int fd);


#endif //CSAPP_SYSIO_H
