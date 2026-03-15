#include <stdio.h>
#include <string.h>

#include "main.h"
#include "cmsis_os.h"
#include "app_tasks.h"
#include "app_config.h"
#include "app_utils.h"
#include "bme280.h"
#include "mq135.h"
#include "ssd1306.h"
#include "nrf24.h"
#include "uart_log.h"

QueueHandle_t g_sensorQueue = NULL;
QueueHandle_t g_processedQueue = NULL;
SemaphoreHandle_t g_i2cMutex = NULL;
TimerHandle_t g_heartbeatTimer = NULL;

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern ADC_HandleTypeDef hadc1;

static void App_CreateObjects(void)
{
    g_sensorQueue = xQueueCreate(SENSOR_QUEUE_LENGTH, sizeof(sensor_packet_t));
    g_processedQueue = xQueueCreate(PROCESSED_QUEUE_LENGTH, sizeof(processed_packet_t));
    g_i2cMutex = xSemaphoreCreateMutex();
    g_heartbeatTimer = xTimerCreate("heartbeat", pdMS_TO_TICKS(HEARTBEAT_PERIOD_MS), pdTRUE, NULL, HeartbeatTimerCallback);
}

void App_Init(void)
{
    App_CreateObjects();

    if ((g_sensorQueue == NULL) || (g_processedQueue == NULL) || (g_i2cMutex == NULL) || (g_heartbeatTimer == NULL))
    {
        Error_Handler();
    }

    if (BME280_Init(&hi2c1) != HAL_OK)
    {
        UART_Log("BME280 init failed\r\n");
    }

    if (SSD1306_Init(&hi2c1) != HAL_OK)
    {
        UART_Log("SSD1306 init failed\r\n");
    }

    if (NRF24_Init(&hspi1) != HAL_OK)
    {
        UART_Log("NRF24 init failed\r\n");
    }

    xTaskCreate(SensorTask, "sensor", SENSOR_TASK_STACK_WORDS, NULL, SENSOR_TASK_PRIORITY, NULL);
    xTaskCreate(ProcessTask, "process", PROCESS_TASK_STACK_WORDS, NULL, PROCESS_TASK_PRIORITY, NULL);
    xTaskCreate(DisplayTask, "display", DISPLAY_TASK_STACK_WORDS, NULL, DISPLAY_TASK_PRIORITY, NULL);
    xTaskCreate(WirelessTask, "wireless", WIRELESS_TASK_STACK_WORDS, NULL, WIRELESS_TASK_PRIORITY, NULL);
    xTaskCreate(LoggerTask, "logger", LOGGER_TASK_STACK_WORDS, NULL, LOGGER_TASK_PRIORITY, NULL);

    xTimerStart(g_heartbeatTimer, 0);
}

void SensorTask(void *argument)
{
    (void)argument;
    sensor_packet_t pkt;
    bme280_data_t bme;

    for (;;)
    {
        memset(&pkt, 0, sizeof(pkt));

        if (xSemaphoreTake(g_i2cMutex, pdMS_TO_TICKS(100)) == pdPASS)
        {
            if (BME280_ReadData(&hi2c1, &bme) == HAL_OK)
            {
                pkt.temperature_c = bme.temperature_c;
                pkt.humidity_pct = bme.humidity_pct;
                pkt.pressure_hpa = bme.pressure_hpa;
            }
            xSemaphoreGive(g_i2cMutex);
        }

        pkt.mq135_adc_raw = MQ135_ReadRaw(&hadc1);
        pkt.mq135_voltage = MQ135_ConvertVoltage(pkt.mq135_adc_raw);
        pkt.timestamp_ms = HAL_GetTick();

        xQueueSend(g_sensorQueue, &pkt, pdMS_TO_TICKS(50));
        vTaskDelay(pdMS_TO_TICKS(SENSOR_SAMPLE_PERIOD_MS));
    }
}

void ProcessTask(void *argument)
{
    (void)argument;
    sensor_packet_t in;
    processed_packet_t out;

    float temp_buf[FILTER_WINDOW] = {0};
    float hum_buf[FILTER_WINDOW] = {0};
    float pres_buf[FILTER_WINDOW] = {0};
    float air_buf[FILTER_WINDOW] = {0};
    uint32_t temp_idx = 0, hum_idx = 0, pres_idx = 0, air_idx_buf = 0;

    for (;;)
    {
        if (xQueueReceive(g_sensorQueue, &in, portMAX_DELAY) == pdPASS)
        {
            float temp_avg = 0.0f, hum_avg = 0.0f, pres_avg = 0.0f, air_avg = 0.0f;
            float air_idx = ComputeAirQualityIndex(in.mq135_voltage);

            MovingAverage_Update(temp_buf, FILTER_WINDOW, &temp_idx, in.temperature_c, &temp_avg);
            MovingAverage_Update(hum_buf, FILTER_WINDOW, &hum_idx, in.humidity_pct, &hum_avg);
            MovingAverage_Update(pres_buf, FILTER_WINDOW, &pres_idx, in.pressure_hpa, &pres_avg);
            MovingAverage_Update(air_buf, FILTER_WINDOW, &air_idx_buf, air_idx, &air_avg);

            out.temperature_c = temp_avg;
            out.humidity_pct = hum_avg;
            out.pressure_hpa = pres_avg;
            out.air_quality_index = air_avg;
            out.comfort_index = ComputeComfortIndex(temp_avg, hum_avg);
            out.timestamp_ms = in.timestamp_ms;

            xQueueOverwrite(g_processedQueue, &out);
        }
    }
}

void DisplayTask(void *argument)
{
    (void)argument;
    processed_packet_t pkt;
    char line1[32], line2[32], line3[32], line4[32];

    for (;;)
    {
        if (xQueuePeek(g_processedQueue, &pkt, pdMS_TO_TICKS(DISPLAY_REFRESH_MS)) == pdPASS)
        {
            snprintf(line1, sizeof(line1), "T: %.1f C", pkt.temperature_c);
            snprintf(line2, sizeof(line2), "H: %.1f %%", pkt.humidity_pct);
            snprintf(line3, sizeof(line3), "P: %.1f hPa", pkt.pressure_hpa);
            snprintf(line4, sizeof(line4), "AQI: %.0f CFI: %.0f", pkt.air_quality_index, pkt.comfort_index);

            if (xSemaphoreTake(g_i2cMutex, pdMS_TO_TICKS(100)) == pdPASS)
            {
                SSD1306_Clear();
                SSD1306_WriteString(0, 0, line1);
                SSD1306_WriteString(0, 16, line2);
                SSD1306_WriteString(0, 32, line3);
                SSD1306_WriteString(0, 48, line4);
                SSD1306_UpdateScreen(&hi2c1);
                xSemaphoreGive(g_i2cMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(DISPLAY_REFRESH_MS));
    }
}

void WirelessTask(void *argument)
{
    (void)argument;
    processed_packet_t pkt;
    char payload[64];

    for (;;)
    {
        if (xQueuePeek(g_processedQueue, &pkt, pdMS_TO_TICKS(WIRELESS_SEND_MS)) == pdPASS)
        {
            snprintf(payload, sizeof(payload), "T=%.1f,H=%.1f,P=%.1f,AQI=%.0f,CI=%.0f",
                     pkt.temperature_c, pkt.humidity_pct, pkt.pressure_hpa,
                     pkt.air_quality_index, pkt.comfort_index);
            NRF24_Send((uint8_t *)payload, strlen(payload) + 1U);
        }
        vTaskDelay(pdMS_TO_TICKS(WIRELESS_SEND_MS));
    }
}

void LoggerTask(void *argument)
{
    (void)argument;
    processed_packet_t pkt;
    char logline[128];

    for (;;)
    {
        if (xQueuePeek(g_processedQueue, &pkt, pdMS_TO_TICKS(LOGGER_PERIOD_MS)) == pdPASS)
        {
            snprintf(logline, sizeof(logline),
                     "[%lu ms] T=%.2fC H=%.2f%% P=%.2fhPa AQI=%.1f Comfort=%.1f\r\n",
                     pkt.timestamp_ms, pkt.temperature_c, pkt.humidity_pct,
                     pkt.pressure_hpa, pkt.air_quality_index, pkt.comfort_index);
            UART_Log(logline);
        }
        vTaskDelay(pdMS_TO_TICKS(LOGGER_PERIOD_MS));
    }
}

void HeartbeatTimerCallback(TimerHandle_t xTimer)
{
    (void)xTimer;
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
