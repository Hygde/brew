#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <bcm2835.h>

#include "udp.h"
#include "motor.h"
#include "state.h"
#include "ds18b20.h"

struct config {
    struct sockaddr_in6 caddr;
    int tx_socket;
    int rx_socket;
    unsigned int rx_timeout;
    uint16_t rx_port;
    uint8_t motor_pin;
    uint8_t motor_state;
    uint8_t state;
    char*sensor_id;
};

static uint8_t buffer[RX_BUFFER_SIZE];

static void init(struct config* cfg) {
    ssize_t datalen;
    initBcm2835();
    initMotor(cfg->motor_pin);
    bindSocketToPort(cfg->rx_socket, cfg->rx_port);
    isSensorAvailable(cfg->sensor_id);
    datalen = receiveFrom(cfg->rx_socket, buffer, 0, &cfg->caddr);
    if(datalen == 2) {
        cfg->caddr.sin6_port = *(uint16_t*) buffer;
        cfg->state = ACTIVE;
    } else {
        cfg->state = STOP;
    }
}

static void closeBrewd(struct config*cfg) {
    motorChangeMode(cfg->motor_pin, 0);
    closeMotor();
    close(cfg->tx_socket);
    close(cfg->rx_socket);
}

int main(int argc, char*argv[]) {
    int ret = 0;
    struct config cfg = {
        {},
        createSocket(),
        createSocket(),
        (unsigned int) 30,
        (uint16_t) 2000,
        (uint8_t) RPI_GPIO_P1_11,
        (uint8_t) 0,
        STOP,
        "28-3c01f0964257"
    };

    init(&cfg);

    while(cfg.state == ACTIVE) {
        if(receiveFrom(cfg.rx_socket, buffer, MSG_DONTWAIT, &cfg.caddr) >= 0) {
            fprintf(stdout, "Processing request %u\n", buffer[0]);
            switch(buffer[0]) {
                case STOP:
                    cfg.state = STOP;
                break;
                case TOGGLE_MOTOR:
                    cfg.motor_state = ~cfg.motor_state;
                    motorChangeMode(cfg.motor_pin, cfg.motor_state);
                break;
                case READ_TEMPERATURE:
                {
                    float temp = getTemperature(cfg.sensor_id);
                    int32_t value = htonl(temp * 1000);
                    fprintf(stdout, "Temperature is %f\n", temp);
                    sendTo(cfg.tx_socket, (uint8_t*) &value, sizeof(value), &cfg.caddr);
                }
                break;
            }
        }
        sleep(1);
    }

    closeBrewd(&cfg);

    return ret;
}