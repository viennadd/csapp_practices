//
// Created by vienna on 6/8/2016.
//

/*
 * 使用 Pool 结构储存生存的 client connections,
 * 每次 select 返回时首先处理和排除新连接 (状态是 waiting for ready to read)
 * 然后处理所有的 (状态转变成 ready to be read 的 clients)
 * */

#include <cstdlib>
#include "concurrent.h"

typedef struct {
    int maxfd;          // select system call need a (maximum fd + 1) as a parameter
    int nready;         // nready, maxi for indicating maximum times ready_set polling
    int maxi;
    fd_set ready_set;
    fd_set read_set;
    int client_fds[FD_SETSIZE];
    rio_t client_rio[FD_SETSIZE];
} Pool ;

void init_pool(int listenfd, Pool &p)
{
    FD_ZERO(&p.read_set);
    FD_SET(listenfd, &p.read_set);
    p.maxfd = listenfd;
    p.maxi = -1;
    // p.nready = 0;  // no need to init

    for (int i = 0; i < FD_SETSIZE; ++i) {
        p.client_fds[i] = -1;
    }
}

void add_client(int clientfd, Pool &pool)
{
    pool.nready--;         // nready = n clientfd are ready for read, not including new connection

    int i;

    for (i = 0; i < FD_SETSIZE; ++i) {

        if (pool.client_fds[i] == -1) {
            // available slot
            pool.client_fds[i] = clientfd;

            if (i > pool.maxi)
                pool.maxi = i;
            if (clientfd > pool.maxfd)
                pool.maxfd = clientfd;

            rio_initb(&pool.client_rio[i], clientfd);

            FD_SET(clientfd, &pool.read_set);
            break;
        }
    }

    if (i == FD_SETSIZE) {
        fprintf(stderr, "Add client error: Too many clients\n");
    }
}

void check_clients(Pool &pool)
{
    char line[MAXLINE];
    int clientfd, nread;
    rio_t *p_rio;

    for (int i = 0; i <= pool.maxi && pool.nready > 0; ++i) {
        clientfd = pool.client_fds[i];
        p_rio = &pool.client_rio[i];

        if (clientfd > 0 && FD_ISSET(clientfd, &pool.ready_set)) {
            // active client

            if ((nread = rio_readlineb(p_rio, line, MAXLINE)) > 0) {
                rio_writen(clientfd, line, nread);

            } else {
                FD_CLR(clientfd, &pool.read_set);
                close(clientfd);
                pool.client_fds[i] = -1;
            }

            pool.nready--;

        }
    }
}

int main(int argc, char *argv[])
{
    sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int listenfd, clientfd, port;
    static Pool pool;       // too large if place in stack

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    port = atoi(argv[1]);
    listenfd = open_listenfd(port);

    init_pool(listenfd, pool);

    while (1) {
        pool.ready_set = pool.read_set;
        pool.nready = select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &pool.ready_set)) {
            clientfd = Accept(listenfd, (SA *)&client_addr, &socklen);
            add_client(clientfd, pool);
        }

        check_clients(pool);
    }

    return 0;
}