#include "nrf24.h"
#include "main.h"

static SPI_HandleTypeDef *g_hspi = NULL;

HAL_StatusTypeDef NRF24_Init(SPI_HandleTypeDef *hspi)
{
    g_hspi = hspi;
    HAL_GPIO_WritePin(NRF24_CSN_GPIO_Port, NRF24_CSN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(NRF24_CE_GPIO_Port, NRF24_CE_Pin, GPIO_PIN_RESET);
    return (g_hspi != NULL) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef NRF24_Send(uint8_t *data, uint16_t len)
{
    if (g_hspi == NULL) return HAL_ERROR;

    HAL_GPIO_WritePin(NRF24_CSN_GPIO_Port, NRF24_CSN_Pin, GPIO_PIN_RESET);
    HAL_StatusTypeDef status = HAL_SPI_Transmit(g_hspi, data, len, 100);
    HAL_GPIO_WritePin(NRF24_CSN_GPIO_Port, NRF24_CSN_Pin, GPIO_PIN_SET);
    return status;
}
