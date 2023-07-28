// gcc motor.c -l bcm2835
#include "motor.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef __x86_64__
#include <bcm2835.h>
#endif

void initBcm2835() {
#ifndef __x86_64__
    int ret = bcm2835_init();
    if (!ret) {
        fprintf(stderr, "Fail to init bcm2835 library\nError code : %d\n", errno);
        exit(errno);
    }
#endif
}

void initMotor(const int pin) {
#ifndef __x86_64__
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
#endif
}

void motorChangeMode(const int pin) {
#ifndef __x86_64__
    bcm2835_gpio_write(pin, 1);
    bcm2835_delay(500);
    bcm2835_gpio_write(pin, 0);
#endif
}

void closeMotor() {
#ifndef __x86_64__
    bcm2835_close();
#endif
}

#ifdef UNIT_TEST
int main(int argc, char*argv[]) {
    initBcm2835();
    initMotor(RPI_GPIO_P1_11);
    for(int i = 0; i < 3; i++) {
        motorChangeMode(RPI_GPIO_P1_11);
        motorChangeMode(RPI_GPIO_P1_11);
    }
    bcm2835_close();
}
#endif/*UNIT_TEST*/
