#include "app_utils.h"

float ComputeComfortIndex(float temperature_c, float humidity_pct)
{
    /* Simple heuristic: 100 is ideal, lower means less comfortable */
    float temp_penalty = (temperature_c > 24.0f) ? (temperature_c - 24.0f) * 2.0f : (24.0f - temperature_c) * 1.5f;
    float hum_penalty  = (humidity_pct > 50.0f) ? (humidity_pct - 50.0f) * 0.5f : (50.0f - humidity_pct) * 0.4f;
    float score = 100.0f - temp_penalty - hum_penalty;

    if (score < 0.0f) score = 0.0f;
    if (score > 100.0f) score = 100.0f;
    return score;
}

float ComputeAirQualityIndex(float mq135_voltage)
{
    /* Demo proxy mapping only. Replace with calibrated gas concentration model. */
    float index = (mq135_voltage / 3.3f) * 500.0f;
    if (index < 0.0f) index = 0.0f;
    if (index > 500.0f) index = 500.0f;
    return index;
}

void MovingAverage_Update(float *buffer, uint32_t len, uint32_t *index, float sample, float *avg_out)
{
    float sum = 0.0f;
    uint32_t i;

    buffer[*index] = sample;
    *index = (*index + 1U) % len;

    for (i = 0; i < len; i++)
    {
        sum += buffer[i];
    }

    *avg_out = sum / (float)len;
}
