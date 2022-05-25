// gcc motor.c -l bcm2835
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <errno.h>

void initBcm2835() {
    int ret = bcm2835_init();
    if (!ret) {
        fprintf(stderr, "Fail to init bcm2835 library\nError code : %d\n", errno);
        exit(errno);
    }
}

void initMotor(const int pin) {
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
}

void motorChangeMode(const int pin, const int mode) {
    bcm2835_gpio_write(pin, mode);
    bcm2835_delay(500);
}

#ifdef UNIT_TEST
int main(int argc, char*argv[]) {
    initBcm2835();
    initMotor(RPI_GPIO_P1_11);
    for(int i = 0; i < 3; i++) {
        motorChangeMode(RPI_GPIO_P1_11, HIGH);
        motorChangeMode(RPI_GPIO_P1_11, LOW);
    }
    bcm2835_close();
}
#endif/*UNIT_TEST*/
