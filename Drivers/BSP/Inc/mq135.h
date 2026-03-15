#ifndef MQ135_H
#define MQ135_H

#include "stm32f4xx_hal.h"

uint16_t MQ135_ReadRaw(ADC_HandleTypeDef *hadc);
float MQ135_ConvertVoltage(uint16_t raw);

#endif
