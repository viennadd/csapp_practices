//
// Created by vienna on 27/7/2016.
//

/*
 * The solution of CSAPP 8.22 (mysystem) & 8.20 (myls)
 *
 * */

/*
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
extern char **environ;


int mysystem(char *command)
{
    pid_t pid;
    if ((pid = fork()) == 0) {
        char *argv[] = {"/bin/sh", "-c", (char *)NULL, (char *)NULL };
        argv[2] = command;
        execve("/bin/sh", argv, environ);
    } else {
        int status;
        pid = waitpid(pid, &status, 0);

        return status;
    }
}


int main(int argc, char *argv[])
{
    // execve("/bin/ls", argv, environ);

    int ret = system("ls /abc");
    printf("ret = %d\n", ret);
    ret = mysystem("ls /abc");
    printf("ret = %d\n", ret);
}
 */