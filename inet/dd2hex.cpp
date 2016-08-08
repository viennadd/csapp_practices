//
// Created by vienna on 31/7/2016.
//

#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    in_addr addr;
    inet_aton(argv[1], &addr);
    addr.s_addr = ntohl(addr.s_addr);

    printf("0x%x\n", addr.s_addr);
    return 0;
}