# Eurobot-Firmware

The STM firmware for DIT Eurobot2025 Chassis

## Overview
This firmware is designed for the omni-directional chassis of the DIT Eurobot2025-14 robot. It manages motion control and communication with external systems, including ROS via rosserial.

## Important Notes
If you need to modify the `.ioc` file, ensure that the function call `MX_DMA_Init();` is placed **before** `MX_USART1_UART_Init();` in `main.c`. This is necessary for establishing proper `rosserial` communication.

## Getting Started
### Prerequisites
- STM32CubeIDE
- DIT Eurobot2025 chassis MCU PCB
- odometry wheel * 4
- motor with encoder * 4

## License
This firmware is developed for DIT Eurobot2025 and follows the applicable open-source licensing guidelines.

## Contact
For questions or contributions, please reach out to the DIT Eurobot team.

