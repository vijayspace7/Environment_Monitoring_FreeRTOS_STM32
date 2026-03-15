#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c);
void SSD1306_Clear(void);
void SSD1306_WriteString(uint8_t x, uint8_t y, const char *str);
HAL_StatusTypeDef SSD1306_UpdateScreen(I2C_HandleTypeDef *hi2c);

#endif
