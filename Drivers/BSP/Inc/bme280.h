#ifndef BME280_H
#define BME280_H

#include "stm32f4xx_hal.h"

typedef struct
{
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
} bme280_data_t;

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef BME280_ReadData(I2C_HandleTypeDef *hi2c, bme280_data_t *data);

#endif
