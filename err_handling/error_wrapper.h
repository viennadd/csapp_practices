//
// Created by vienna on 7/8/2016.
//

#ifndef CSAPP_ERROR_WRAPPER_H
#define CSAPP_ERROR_WRAPPER_H

#include <sys/socket.h>

#define MAXLINE 256

extern int errno;

void unix_error(char *msg);
void dns_error(char *msg);
void posix_error(int code, char *msg);
void app_error(char *msg);

pid_t Fork();
pid_t Wait(int *status);

int Accept(int listenfd, sockaddr *client_addr, socklen_t *client_addr_len);

int Mmap();
int Unmmap();

// posix threads
int Pthread_create(pthread_t *pid,
                   const pthread_attr_t *,
                   void* (*thread_handler)(void *arg),
                   void *arg);



#endif //CSAPP_ERROR_WRAPPER_H
