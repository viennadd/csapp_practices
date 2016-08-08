//
// Created by vienna on 7/8/2016.
//
#include "concurrent.h"

void echo(int clientfd)
{
    char line[MAXLINE];
    rio_t rio;
    int n;

    rio_initb(&rio, clientfd);
    if ((n = rio_readlineb(&rio, line, MAXLINE)) != 0) {        // not EOF
        printf("Received: %s\n", line);
        rio_writen(clientfd, line, n);
    }
}


char *strcpy(char *dst, const char *src)
{
    if (dst == NULL || src == NULL)
        return NULL;

    while (*src) {
        *dst++ = *src++;
    }

    return dst;
}