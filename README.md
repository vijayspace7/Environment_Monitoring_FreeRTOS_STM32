# Smart Environmental Monitoring Node (STM32F446RE + FreeRTOS)

A GitHub-ready STM32 + FreeRTOS project for a multi-sensor environmental monitoring node.

## Features
- BME280 over I2C (temperature, humidity, pressure)
- MQ135 over ADC (air quality proxy)
- SSD1306 OLED over I2C
- nRF24L01 over SPI for wireless telemetry
- Optional SD logging stub
- FreeRTOS tasks, queues, mutexes, and software timers
- HAL-oriented structure designed for STM32CubeIDE integration

## Target MCU
- **STM32F446RE / STM32F4 family**
- Easily adaptable to STM32F446R3 if your board uses that variant

## RTOS Architecture
- **SensorTask**: Reads BME280 + MQ135 periodically
- **ProcessTask**: Filters raw samples and computes AQI proxy
- **DisplayTask**: Updates OLED
- **WirelessTask**: Sends telemetry with nRF24L01
- **LoggerTask**: Emits log lines / placeholder for SD card logging
- **HeartbeatTimer**: Toggles a status LED periodically

## Inter-task Communication
- `sensorQueue`: raw sensor packets from SensorTask
- `processedQueue`: processed packets to Display/Wireless/Logger
- `i2cMutex`: shared protection for BME280 and OLED on same bus

## Repository Layout
```
freertos_env_monitor_repo/
├── App/
│   ├── Inc/
│   └── Src/
├── Core/
│   ├── Inc/
│   └── Src/
├── Drivers/
│   └── BSP/
│       ├── Inc/
│       └── Src/
├── .github/
│   └── workflows/
├── README.md
├── LICENSE
└── .gitignore
```

## CubeMX / CubeIDE Configuration
Configure this in STM32CubeMX before building:

### Peripherals
- **I2C1**: BME280 + SSD1306 OLED
- **SPI1**: nRF24L01
- **ADC1**: MQ135 analog output
- **USART2**: debug logging
- **GPIO**:
  - LED pin (e.g. `LD2_Pin` on Nucleo)
  - nRF24 CSN pin
  - nRF24 CE pin
- **FreeRTOS** enabled

### Clock
- Typical STM32F446 Nucleo clock profile is fine

### ADC
- Single conversion mode
- One regular channel connected to MQ135 output

### FreeRTOS Heap
- `heap_4.c`
- Heap size >= 16 KB recommended for this demo

## Build Integration
This repo provides the **application code and BSP layer**.

1. Create a new STM32CubeIDE project for STM32F446RE / your exact board.
2. Enable the peripherals listed above.
3. Copy these folders into your CubeIDE project:
   - `App/`
   - `Drivers/BSP/`
   - `Core/Src/main.c`
   - `Core/Inc/main.h`
   - `Core/Src/stm32f4xx_it.c`
   - `Core/Inc/stm32f4xx_it.h`
4. Make sure include paths contain:
   - `App/Inc`
   - `Drivers/BSP/Inc`
5. Regenerate code only after preserving user sections.

## Pin Assumptions
You can change these in `Core/Inc/main.h`.

| Signal | Default Example |
|---|---|
| LED | PA5 |
| NRF CE | PB0 |
| NRF CSN | PB6 |
| MQ135 ADC | PA0 / ADC1 IN0 |
| I2C1 | PB8/PB9 or board default |
| SPI1 | PA5/PA6/PA7 or board default |

## Sensor Processing
The project uses:
- moving average filtering for environmental values
- simple voltage-based MQ135 air quality proxy
- derived `comfort_index`

This is meant as a clean embedded systems project and can be extended with:
- SD card logging
- LoRa/Wi-Fi/BLE
- cloud dashboard
- alarm thresholds
- OTA updates via companion MCU

## GitHub Suggestions
Add screenshots of:
- serial telemetry
- OLED display
- task timing on logic analyzer / debugger
- hardware setup

## Future Improvements
- real SD card FATFS logger
- calibration routine for MQ135
- low-power tickless idle mode
- packet acknowledgement/retry stats
- CRC on application payloads

## License
MIT
