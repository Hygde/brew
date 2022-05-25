#ifndef __DS18B20_H__
#define __DS18B20_H__

/**
 * @brief Check if the sensor is available
 * 
 * @param sensor_id 
 * @return int 
 */
int isSensorAvailable(const char* sensor_id);

/**
 * @brief Reads the temperature in degree celsisus
 * 
 */
float getTemperature(const char* sensor_id);

#endif/*__DS18B20_H__*/