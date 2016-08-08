//
// Created by vienna on 7/8/2016.
//

#include <cstdlib>
#include <pthread.h>
#include "concurrent.h"


void* thread_runner(void *arg)
{
    int clientfd = *(int *)arg;
    free(arg);

    printf("I am thread-%d, what can I help u ?\n", pthread_self());
    echo(clientfd);
    close(clientfd);

    // let system to clean up thread's resources
    pthread_detach(pthread_self());
    return NULL;
}


int main(int argc, char *argv[])
{
    int listenfd, port;
    sockaddr_in client_addr;
    socklen_t socklen;
    int *p_clientfd;
    pthread_t thread_id;

    if (argc != 2) {
        printf("usage: %s <port>\n", argv[1]);
        exit(0);
    }

    port = atoi(argv[1]);
    listenfd = open_listenfd(port);

    while (1) {
        // passing clientfd through individual const heap memory: avoid race condition
        p_clientfd = (int *)malloc(sizeof(int));
        *p_clientfd = Accept(listenfd, (SA *)&client_addr, &socklen);
        Pthread_create(&thread_id, NULL, thread_runner, p_clientfd);
    }
}