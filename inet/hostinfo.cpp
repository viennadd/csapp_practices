//
// Created by vienna on 31/7/2016.
//

#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>
#include <cstdio>


int main(int argc, char *argv[])
{
    hostent *p_host;
    in_addr addr;
    char **pp;

    if (argc != 2)
        exit(0);

    if (inet_aton(argv[1], &addr) != 0)     // it is a doted decimal address
        p_host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    else                                    // it is a hostname
        p_host = gethostbyname(argv[1]);

    printf("Official hostname: %s\n", p_host->h_name);

    for (pp = p_host->h_aliases; *pp; ++pp) {
        printf("alias: %s\n", *pp);
    }

    for (pp = p_host->h_addr_list; *pp; ++pp) {
        addr.s_addr = ((in_addr *)*pp)->s_addr;
        printf("address: %s\n", inet_ntoa(addr));
    }
    return 0;
}