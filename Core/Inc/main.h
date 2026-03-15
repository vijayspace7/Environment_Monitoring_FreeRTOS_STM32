#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void Error_Handler(void);

/* Pin definitions - adjust for your board */
#define LED_Pin                 GPIO_PIN_5
#define LED_GPIO_Port           GPIOA

#define NRF24_CE_Pin            GPIO_PIN_0
#define NRF24_CE_GPIO_Port      GPIOB

#define NRF24_CSN_Pin           GPIO_PIN_6
#define NRF24_CSN_GPIO_Port     GPIOB

#ifdef __cplusplus
}
#endif

#endif
