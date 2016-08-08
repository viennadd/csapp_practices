//
// Created by vienna on 27/7/2016.
//

#include <signal.h>
#include <cstdio>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

/*
 * The solution of CSAPP 8.24
 * */

/*

int main()
{

    FILE *f = fopen("/etc/passwd", "r");

    int status;
    pid_t pid;

//    for (int i = 0; i < 2; i++) {
//        if ((pid = fork()) == 0) {
//            printf("0\n");
//        } else {
//            sleep(1);
//            kill(pid, 11);
//        }
//    }

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            fprintf(stdout, "child %d terminated by signal %d: %s\n", pid, sig, strsignal(sig));
        }
    }



    return 0;
}

 */