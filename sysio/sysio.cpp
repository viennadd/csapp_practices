//
// Created by vienna on 30/7/2016.
//

#include "sysio.h"

// robust read/write, 会处理不足值
ssize_t rio_readn(int fd, void *buf, size_t size)
{
    ssize_t nread = 0;
    size_t nleft = size;
    char *p_buf = (char *)buf;

    while (nleft > 0) {

        // 因应情况调整 buf 指针和继续操作的 size, 直到读完或者读够 size 为止
        if ((nread = read(fd, p_buf, nleft)) < 0) {

            if (errno == EINTR)
                nread = 0;      // 标准 read 内被中断, 重新在相关位置再读, 不调整 buf 指针
            else
                return -1;      // 不是被中断就是出错了
        }
        else if (nread == 0)    // EOF
            break;

        nleft -= nread;
        p_buf += nread;
    }

    // 实际上读入了的大小
    return (size - nleft);
}


ssize_t rio_writen(int fd, void *buf, size_t size)
{
    ssize_t nwrite = 0;
    size_t nleft = size;
    char *p_buf = (char *)buf;

    while (nleft > 0) {
        if ((nwrite = write(fd, p_buf, nleft)) < 0) {

            if (errno == EINTR)
                nwrite = 0;
            else
                return -1;
        }

        nleft -= nwrite;
        p_buf += nwrite;
    }

    return size;
}


// 自动缓存的 read/write

// 不暴露给外部, 所以 static
static ssize_t rio_read(rio_t *p_rio, void *buf, size_t size)
{
    // 直到第一次读到内容为止
    while (p_rio->cnt_unread <= 0) {

        if ((p_rio->cnt_unread = read(p_rio->fd, p_rio->buf, sizeof(p_rio->buf))) < 0) {
            if (errno != EINTR)
                return -1;
        }
        else if (p_rio->cnt_unread == 0)        // EOF
            return 0;
        else        // success read, reset to base
            p_rio->buf_ptr = p_rio->buf;
    }

    // the buffer is ready at here

    // the size should be returned
    int cnt = size;
    if (p_rio->cnt_unread < cnt)        // size will be min(requested, we_can_offer)
        cnt = p_rio->cnt_unread;

    memcpy(buf, p_rio->buf_ptr, cnt);
    p_rio->buf_ptr += cnt;          // 调整下次再读时的位置
    p_rio->cnt_unread -= cnt;       // 调整本缓存剩下多少未读数据

    return cnt;
}

ssize_t rio_readlineb(rio_t *p_rio, void *usrbuf, int maxlen)
{
    int n, rc;
    char *p_buf = (char *)usrbuf;

    for (n = 1; n < maxlen; ++n) {

        // 每次 1 byte, 遇换行或到 max 就补 '\0' 返回
        if ((rc = rio_read(p_rio, p_buf, 1)) == 1) {
            p_buf++;
            if (*(p_buf - 1) == '\n')
                break;

        }
        else if (rc == 0) {
            if (n == 1)
                return 0;       // EOF, 0 byte read
            else
                break;          // EOF, n bytes read
        } else {
            return -1;
        }
    }

    *p_buf = '\0';
    return n;
}


ssize_t rio_readnb(rio_t *p_rio, void *buf, size_t size)
{
    ssize_t nread = 0;
    size_t nleft = size;
    char *p_buf = (char *)buf;

    while (nleft > 0) {

        // 因应情况调整 buf 指针和继续操作的 size, 直到读完或者读够 size 为止
        if ((nread = rio_read(p_rio, p_buf, nleft)) < 0) {

            if (errno == EINTR)
                nread = 0;      // 标准 read 内被中断, 重新在相关位置再读, 不调整 buf 指针
            else
                return -1;      // 不是被中断就是出错了
        }
        else if (nread == 0)    // EOF
            break;

        nleft -= nread;
        p_buf += nread;
    }

    // 实际上读入了的大小
    return (size - nleft);
}

void rio_initb(rio_t *p_rio, int fd)
{
    p_rio->fd = fd;
    p_rio->cnt_unread = 0;
    p_rio->buf_ptr = p_rio->buf;
}