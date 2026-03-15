#include "main.h"
#include "stm32f4xx_it.h"
#include "cmsis_os.h"

void SysTick_Handler(void)
{
    HAL_IncTick();
    osSystickHandler();
}
