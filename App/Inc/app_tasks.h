#ifndef APP_TASKS_H
#define APP_TASKS_H

#include "cmsis_os.h"
#include "app_types.h"

extern QueueHandle_t g_sensorQueue;
extern QueueHandle_t g_processedQueue;
extern SemaphoreHandle_t g_i2cMutex;
extern TimerHandle_t g_heartbeatTimer;

void App_Init(void);
void SensorTask(void *argument);
void ProcessTask(void *argument);
void DisplayTask(void *argument);
void WirelessTask(void *argument);
void LoggerTask(void *argument);
void HeartbeatTimerCallback(TimerHandle_t xTimer);

#endif
