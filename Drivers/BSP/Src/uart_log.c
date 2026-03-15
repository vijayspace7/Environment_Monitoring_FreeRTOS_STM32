#include <string.h>
#include "uart_log.h"
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

void UART_Log(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
}
