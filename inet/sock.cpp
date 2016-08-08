//
// Created by vienna on 31/7/2016.
//
#include <cstdlib>
#include "sock.h"



int open_clientfd(char *hostname, int port)
{
    int clientfd;
    hostent *p_host;
    sockaddr_in server_addr;


    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    p_host = gethostbyname(hostname);
    if (p_host == NULL)     // get host 失败
        return -2;

    // construct server address struct
    bzero(&server_addr, sizeof(server_addr));
    bcopy(p_host->h_addr_list[0], &server_addr.sin_addr.s_addr, p_host->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);


    if (connect(clientfd, (SA *)&server_addr, sizeof(server_addr)) < 0)
        return -1;

    return clientfd;
}


int open_listenfd(int port)
{
    int listenfd, optval = 1;
    sockaddr_in server_addr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // Address already in use problem.
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
        return -1;

    // construct server address
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (SA *)&server_addr, sizeof(server_addr)) < 0)
        return -1;

    const int LISTENQ = 100;
    if (listen(listenfd, LISTENQ) < 0)
        return -1;

    return listenfd;
}



