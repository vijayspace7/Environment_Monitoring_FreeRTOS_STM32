#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>

typedef struct
{
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    uint16_t mq135_adc_raw;
    float mq135_voltage;
    uint32_t timestamp_ms;
} sensor_packet_t;

typedef struct
{
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
    float air_quality_index;
    float comfort_index;
    uint32_t timestamp_ms;
} processed_packet_t;

#endif
