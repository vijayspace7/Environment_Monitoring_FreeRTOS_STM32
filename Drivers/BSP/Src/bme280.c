#include "bme280.h"

#define BME280_I2C_ADDR      (0x76 << 1)
#define BME280_REG_ID        0xD0
#define BME280_CHIP_ID       0x60

HAL_StatusTypeDef BME280_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t id = 0;
    if (HAL_I2C_Mem_Read(hi2c, BME280_I2C_ADDR, BME280_REG_ID, I2C_MEMADD_SIZE_8BIT, &id, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return (id == BME280_CHIP_ID) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef BME280_ReadData(I2C_HandleTypeDef *hi2c, bme280_data_t *data)
{
    /* Placeholder simplified driver.
       Replace with full Bosch compensation algorithm for production use. */
    (void)hi2c;
    data->temperature_c = 26.5f;
    data->humidity_pct = 55.0f;
    data->pressure_hpa = 1009.2f;
    return HAL_OK;
}
