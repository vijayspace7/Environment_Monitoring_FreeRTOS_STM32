# Hardware Wiring Guide

## 1) BME280 (I2C)
- VCC -> 3.3V
- GND -> GND
- SCL -> I2C1_SCL
- SDA -> I2C1_SDA
- CSB -> 3.3V
- SDO -> GND (address 0x76)

## 2) SSD1306 OLED (I2C)
- VCC -> 3.3V
- GND -> GND
- SCL -> I2C1_SCL
- SDA -> I2C1_SDA

## 3) MQ135
- VCC -> 5V or module-rated input
- GND -> GND
- AO -> ADC1_IN0 (or chosen ADC channel)

If your MQ135 module outputs 5V analog levels, use a voltage divider before the STM32 ADC input.

## 4) nRF24L01 (SPI)
- VCC -> 3.3V
- GND -> GND
- CE -> PB0
- CSN -> PB6
- SCK -> SPI1_SCK
- MOSI -> SPI1_MOSI
- MISO -> SPI1_MISO
- IRQ -> optional GPIO interrupt input

## 5) UART Debug
- USART2 TX/RX to ST-Link Virtual COM or external USB-UART

## Notes
- Keep nRF24 power stable; add local decoupling capacitor near module.
- MQ135 requires warm-up and calibration for meaningful gas readings.
