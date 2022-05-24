#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char*argv[]) {
    int rc = 0;
    int sock = 0;
    struct sockaddr_in6 saddr;
    struct sockaddr_in6 caddr;
    socklen_t caddrlen = sizeof(caddr);

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = in6addr_any;
    saddr.sin6_port = htons(2000);

    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if(sock >= 0) {
        if(bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) >= 0){
            uint8_t buf[2] = {0};
            if(recvfrom(sock, buf, sizeof(uint16_t), 0, (struct sockaddr *) &caddr, &caddrlen) > 0) {
                uint16_t*port = (uint16_t*) buf;
                printf("Received from: %x:%x:%x:%x:%x:%x:%x:%x\n",
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[0]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[1]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[2]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[3]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[4]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[5]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[6]),
                    ntohs(caddr.sin6_addr.__in6_u.__u6_addr16[7])
                );
                printf("Received port: %u\n", ntohs(*port));
            } else {
                perror("can not recvfrom\n");
                rc = EIO;
            }
        } else {
            perror("Can not bind");
            rc = EIO;
        }
    } else {
        perror("Can not create socket\n");
        rc = EIO;
    }

    close(sock);

    return rc;
}