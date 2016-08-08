//
// Created by vienna on 31/7/2016.
//

#ifndef CSAPP_SOCK_H
#define CSAPP_SOCK_H
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>


typedef sockaddr SA;
int open_clientfd(char *hostname, int port);
int open_listenfd(int port);

#endif //CSAPP_SOCK_H
