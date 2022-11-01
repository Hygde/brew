#include "udp.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int createSocket() {
    int ret = socket(AF_INET6, SOCK_DGRAM, 0);
    if (ret < 0) {
        fprintf(stderr, "Fail to create socket! socket return %d\n", ret);
        exit(ret);
    }
    return ret;
}

void bindSocketToPort(int sock, uint16_t port) {
    struct sockaddr_in6 saddr;
    int ret;

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr = in6addr_any;
    saddr.sin6_port = htons(port);

    ret = bind(sock, (struct sockaddr*)&saddr, sizeof(saddr));
    if (ret < 0) {
        fprintf(stderr, "Fail to bind socket! bind returned %d\n", ret);
        exit(ret);
    }
}

ssize_t receiveFrom(int sock, uint8_t*buf, int flag, struct sockaddr_in6*src_addr) {
    ssize_t ret;
    socklen_t src_addrlen = sizeof(struct sockaddr_in6);

    if(buf == NULL) {
        fprintf(stderr, "Buffer is a null pointer %hhn\n", buf);
        exit(EIO);
    }

    ret = recvfrom(sock, buf, RX_BUFFER_SIZE, flag, (struct sockaddr *) src_addr, &src_addrlen);
    if (ret <= 0 && (flag & MSG_DONTWAIT) == 0) {
        fprintf(stderr, "Fail to receive data! recvfrom returned %d\n", (int) ret);
        exit(ret);
    } else {
        printf("Received from: %x:%x:%x:%x:%x:%x:%x:%x\n",
            ntohs(src_addr->sin6_addr.s6_addr16[0]),
            ntohs(src_addr->sin6_addr.s6_addr16[1]),
            ntohs(src_addr->sin6_addr.s6_addr16[2]),
            ntohs(src_addr->sin6_addr.s6_addr16[3]),
            ntohs(src_addr->sin6_addr.s6_addr16[4]),
            ntohs(src_addr->sin6_addr.s6_addr16[5]),
            ntohs(src_addr->sin6_addr.s6_addr16[6]),
            ntohs(src_addr->sin6_addr.s6_addr16[7])
        );
    }

    return ret;
}

void sendTo(int sock, uint8_t*buf, uint8_t len, struct sockaddr_in6*dst_addr) {
    printf("Sendiing to: %x:%x:%x:%x:%x:%x:%x:%x\n",
            ntohs(dst_addr->sin6_addr.s6_addr16[0]),
            ntohs(dst_addr->sin6_addr.s6_addr16[1]),
            ntohs(dst_addr->sin6_addr.s6_addr16[2]),
            ntohs(dst_addr->sin6_addr.s6_addr16[3]),
            ntohs(dst_addr->sin6_addr.s6_addr16[4]),
            ntohs(dst_addr->sin6_addr.s6_addr16[5]),
            ntohs(dst_addr->sin6_addr.s6_addr16[6]),
            ntohs(dst_addr->sin6_addr.s6_addr16[7])
        );
    }
    socklen_t dst_addr_len = sizeof(struct sockaddr_in6);
    int ret = sendto(sock, buf, len, 0,(struct sockaddr*)dst_addr, dst_addr_len);
    if (ret < 0) {
        fprintf(stderr, "Fail to send data! sendto returned %d\n", ret);
        exit(ret);
    }
}

#ifdef UNIT_TEST
int main(int argc, char*argv[]) {
    int rc = 0;
    int sock = createSocket();
    uint8_t buf[RX_BUFFER_SIZE];
    uint8_t datalen;
    struct sockaddr_in6 caddr;
    socklen_t caddrlen = sizeof(caddr);

    bindSocketToPort(sock, 2000);

    datalen = receiveFrom(sock, buf, 0, &caddr);
    caddr.sin6_port = *(uint16_t*)buf;
    printf("Received port: %u\n", ntohs(caddr.sin6_port));
    sendTo(sock, buf, datalen, &caddr);

    close(sock);

    return rc;
}
#endif/*UNIT_TEST*/
