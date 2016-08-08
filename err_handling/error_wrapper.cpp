//
// Created by vienna on 7/8/2016.
//

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include "error_wrapper.h"

void unix_error(char *msg)
{
    char errno_msg[MAXLINE];
    fprintf(stderr, "%s: %s\n", msg, strerror_r(errno, errno_msg, MAXLINE));
    exit(0);
}

void dns_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, hstrerror(h_errno));
    exit(0);
}

void posix_error(int code, char *msg)
{
    char code_msg[MAXLINE];
    fprintf(stderr, "%s: %s\n", msg, strerror_r(code, code_msg, MAXLINE));
    exit(0);
}

void app_error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(0);
}

pid_t Fork()
{
    pid_t pid;
    char errmsg[MAXLINE];
    if ((pid = fork()) < 0) {
        unix_error("fork error");
    }

    return pid;
}

pid_t Wait(int *status)
{
    pid_t pid;
    if ((pid = wait(status)) < 0)
        unix_error("wait error");

    return pid;
}


int Accept(int clientfd, sockaddr *client_addr, socklen_t *client_addr_len)
{
    int fd;
    char errmsg[MAXLINE];
    if ((fd = accept(clientfd, client_addr, client_addr_len)) == -1) {
        fprintf(stderr, "Accept Error\n");
        fprintf(stderr, "%s\n", strerror_r(errno, errmsg, MAXLINE));
        exit(1);
    }

    return fd;
}


int Pthread_create(pthread_t *pid,
                   const pthread_attr_t *,
                   void* (*thread_handler)(void *arg),
                   void *arg)
{
    int ret = pthread_create(pid, NULL, thread_handler, arg);
    if (ret != 0)
        posix_error(ret, "pthread create error");
}


