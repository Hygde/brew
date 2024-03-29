#ifndef __MOTOR_H__
#define __MOTOR_H__

/**
 * @brief Initializes BCM 2835 library
 * 
 */
void initBcm2835();

/**
 * @brief Set pin to write mode
 * 
 * @param pin 
 */
void initMotor(const int pin);

/**
 * @brief Send a rising/falling edge to the bistable relay
 * 
 * @param pin 
 */
void motorChangeMode(const int pin);

/**
 * @brief Close BCM2835 library
 * 
 */
void closeMotor();

#endif /*__MOTOR_H__*/