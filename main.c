#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    struct addrinfo hint, *res, *p;
    int err;

    if(argc != 2) {
        fprintf(stderr," usage: iplookup hostname\n ex: iplookup ddo.me\n");
        return 1;
    }

    memset(&hint, 0, sizeof hint);
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;

    // look up
    err = getaddrinfo(argv[1], "http", &hint, &res);
    if(err != 0) {
        printf("ERR: %s\n", gai_strerror(err));
        return err;
    }

    // list all ips
    void *addr;
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    char ipstr[INET6_ADDRSTRLEN];

    for(p = res ;p != NULL; p = p->ai_next) {

        // ipv4
        if(p->ai_family == AF_INET) {
            // convert generic addr to internet addr
            ipv4 = (struct sockaddr_in *)p->ai_addr;

            // point to binary addr for convert to str later
            addr = &(ipv4->sin_addr);

        // ipv6
        } else {
            // convert generic addr to internet addr
            ipv6 = (struct sockaddr_in6 *)p->ai_addr;

            // point to binary addr for convert to str later
            addr = &(ipv6->sin6_addr);
        }

        // convert binary addr to str
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s\n", ipstr);
    }

    // don't forget to free the addrinfo
    freeaddrinfo(res);
    return 0;
}