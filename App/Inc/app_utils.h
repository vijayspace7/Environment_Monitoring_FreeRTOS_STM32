#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <stdint.h>
#include "app_types.h"

float ComputeComfortIndex(float temperature_c, float humidity_pct);
float ComputeAirQualityIndex(float mq135_voltage);
void MovingAverage_Update(float *buffer, uint32_t len, uint32_t *index, float sample, float *avg_out);

#endif
