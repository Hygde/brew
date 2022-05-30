#ifndef __UDP_H__
#define __UDP_H__

#include <stdio.h>
#include <stdint.h>

#define RX_BUFFER_SIZE 256

struct sockaddr_in6;

/**
 * @brief Create a Socket object
 * 
 * @return int 
 */
int createSocket();

/**
 * @brief Bind a given socket to a specific port
 * 
 * @param sock 
 * @param port 
 */
void bindSocketToPort(int sock, uint16_t port);

/**
 * @brief Receive data using a given socket
 * 
 * @param sock 
 * @param buf 
 * @param src_addr 
 * @return ssize_t 
 */
ssize_t receiveFrom(int sock, uint8_t*buf, struct sockaddr_in6*src_addr);

/**
 * @brief Send data using a given socket
 * 
 * @param sock 
 * @param buf 
 * @param len 
 * @param dst_addr 
 */
void sendTo(int sock, uint8_t*buf, uint8_t len, struct sockaddr_in6*dst_addr);

#endif/*__UDP_H__*/