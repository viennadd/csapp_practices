//
// Created by vienna on 6/8/2016.
//

#include <stdlib.h>
#include <unistd.h>
#include "concurrent.h"
#include "../inet/sock.h"
#include "../sysio/sysio.h"

void command(void);

int main(int argc, char *argv[])
{
    int listenfd, clientfd, port;
    sockaddr_in client_addr;
    socklen_t socklen = sizeof(sockaddr_in);
    fd_set ready_set, read_set;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    listenfd = open_listenfd(port);

    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);
    FD_SET(listenfd, &read_set);

    printf("value of read_set = %x\n", read_set);

    while (1) {
        ready_set = read_set;
        printf("selecting\n");
        select(listenfd + 1, &ready_set, NULL, NULL, NULL);
        printf("one or more fd are ready\n");
        if (FD_ISSET(STDIN_FILENO, &ready_set)) {
            command();
        }

        if (FD_ISSET(listenfd, &ready_set)) {
            clientfd = Accept(listenfd, (SA *)&client_addr, &socklen);
            echo(clientfd);
            close(clientfd);
        }
    }

}

void command(void)
{
    char buf[MAXLINE];
    if (!fgets(buf, MAXLINE, stdin)) {
        exit(0);
    }

    printf("%s\n", buf);

}