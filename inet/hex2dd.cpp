//
// Created by vienna on 31/7/2016.
//
#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    in_addr addr;

    int hex_value;
    sscanf(argv[1], "%x", &hex_value);
    addr.s_addr = htonl(hex_value);
    char *dd_addr = inet_ntoa(addr);

    printf("%s\n", dd_addr);
    return 0;

}
