//
// Created by vienna on 31/7/2016.
//

/*
 * can use `telnet 0.0.0.0 port_number` be an echo client
 *
 * */

#include "sock.h"
#include "../sysio/sysio.h"

int echo(int clientfd)
{

    char line[MAXLINE];
    rio_t rio;
    int n;

    rio_initb(&rio, clientfd);
    if ((n = rio_readlineb(&rio, line, MAXLINE)) != 0) {        // not EOF
        printf("Received: %s\n", line);
        rio_writen(clientfd, line, n);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int listenfd = open_listenfd(8000);
    int clientfd;

    sockaddr_in client_addr;
    socklen_t client_addr_len;

    while (1) {
        clientfd = accept(listenfd, (SA *)&client_addr, &client_addr_len);

        printf("a client %s:%d connected\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);

        echo(clientfd);
        close(clientfd);
    }
}