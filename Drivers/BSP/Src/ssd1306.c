#include <string.h>
#include "ssd1306.h"

static char screen_buffer[4][22];

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
    memset(screen_buffer, 0, sizeof(screen_buffer));
    return HAL_OK;
}

void SSD1306_Clear(void)
{
    memset(screen_buffer, 0, sizeof(screen_buffer));
}

void SSD1306_WriteString(uint8_t x, uint8_t y, const char *str)
{
    uint8_t row = y / 16U;
    (void)x;
    if (row < 4U)
    {
        strncpy(screen_buffer[row], str, sizeof(screen_buffer[row]) - 1U);
    }
}

HAL_StatusTypeDef SSD1306_UpdateScreen(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
    /* Replace with real SSD1306 frame push. */
    return HAL_OK;
}
