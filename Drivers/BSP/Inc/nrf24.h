#ifndef NRF24_H
#define NRF24_H

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef NRF24_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef NRF24_Send(uint8_t *data, uint16_t len);

#endif
