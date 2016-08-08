//
// Created by vienna on 27/7/2016.
//

/*
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void unix_error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

pid_t Fork()
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        unix_error("fork error");
    }

    return pid;
}



// ====================

#define MAXARGS (128)
#define MAXLINE (256)

extern char **environ;

void eval(char *cmdline);
int parseline(char *cmdline, char **argv);
int buildin_command(char **argv);

int main()
{
    char cmdline[MAXLINE];


    while (1) {
        printf("> ");
        fgets(cmdline, MAXLINE, stdin);

        if (feof(stdin)) {
            exit(0);
        }

        eval(cmdline);
    }
}


void eval(char *cmdline)
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    strcpy(buf, cmdline);

    bg = parseline(buf, argv);
    if (argv[0] == NULL)
        return;

    if (!buildin_command(argv)) {
        if ((pid = Fork()) == 0) {

            // child process
            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        }

        // parent process
        if (!bg) {
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                unix_error("waitfg: waitpid error");
            }

        } else {

            printf("%d %s\n", pid, cmdline);
        }

    }

}


int buildin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }

    if (!strcmp(argv[0], "&")) {
        return 1;
    }

    return 0;
}


int parseline(char *buf, char **argv)
{
    char *delim;
    int argc;
    int bg;

    buf[strlen(buf) - 1] = ' ';
    while (*buf && (*buf == ' ')) {  // skip leading space
        buf++;
    }

    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;

        while (*buf && (*buf == ' ')) {  // skip leading space
            buf++;
        }

    }

    argv[argc] = NULL;

    if (argc == 0)
        return 1;

    if ((bg = (*argv[argc - 1] == '&')) != 0) {
        argv[--argc] = NULL;
    }

    return bg;
}
 */