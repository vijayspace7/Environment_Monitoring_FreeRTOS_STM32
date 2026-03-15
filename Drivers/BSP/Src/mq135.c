#include "mq135.h"
#include "app_config.h"

uint16_t MQ135_ReadRaw(ADC_HandleTypeDef *hadc)
{
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 10);
    return (uint16_t)HAL_ADC_GetValue(hadc);
}

float MQ135_ConvertVoltage(uint16_t raw)
{
    return ((float)raw / MQ135_ADC_MAX) * ADC_REF_VOLTAGE;
}
