//
// Created by vienna on 6/8/2016.
//

/*
 * can use `telnet 0.0.0.0 port_number` be an echo client
 *
 * */

#include <cstdlib>
#include "concurrent.h"


void sigchld_handler(int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        printf("回收\n");
}


int main(int argc, char *argv[])
{
    int listenfd = open_listenfd(8000);
    int clientfd;

    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    signal(SIGCHLD, sigchld_handler);
    while (1) {
        clientfd = accept(listenfd, (SA *)&client_addr, &client_addr_len);

        printf("a client %s:%d connected\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
        pid_t pid_child;
        if ((pid_child = fork()) == 0) {
            // fork 后 fd 增加了引用数, 需要在子进程
            echo(clientfd);
            close(clientfd);
            close(listenfd);

            // 记得退出子进程
            exit(0);
        }
        close(clientfd);
    }


    return 0;

}

