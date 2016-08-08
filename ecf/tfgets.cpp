//
// Created by vienna on 27/7/2016.
//
/*

#include <unistd.h>
#include <cstdio>
#include <signal.h>
#include <setjmp.h>

sigjmp_buf env;

void tfgets_handler(int signo)
{
    signal(SIGALRM, SIG_DFL);
    siglongjmp(env, 1);
}

char *tfgets(char *str, int length, FILE *stream)
{
    char *p = NULL;
    int time_sec = 5;

    signal(SIGALRM, tfgets_handler);

    int r = sigsetjmp(env, 1);

    if (r == 0) {
        alarm(time_sec);
        p = fgets(str, length, stream);
        // if normal return, cancel alarm handler
        alarm(0);
    } else {
        // after 5 sec

    }

    return p;
}

int main()

{
    char buf[256];
    char *p = tfgets(buf, 100, stdin);

    printf("p = %x\n", p);

    sleep(20);
    return 0;
}

 */