# Eurobot-Firmware

The STM firmware for DIT Eurobot2025 Chassis

## Overview
This firmware is designed for the omni-directional chassis of the DIT Eurobot2025-12 robot. It manages motion control and communication with external systems, including ROS via rosserial.

## Important Notes
If you need to modify the `.ioc` file, ensure that the function call `MX_DMA_Init();` is placed **before** `MX_USART1_UART_Init();` in `main.c`. This is necessary for establishing proper `rosserial` communication.

## Getting Started
### Prerequisites
- STM32CubeIDE
- STM32 Hardware (compatible with Eurobot2025 chassis)
- ROS with `rosserial` package

### Compilation & Flashing
1. Open the project in **STM32CubeIDE**.
2. Ensure the correct microcontroller is selected in the `.ioc` file.
3. Build the project and flash it to the STM32.

### ROS Integration
1. Connect the STM32 via UART to the ROS host machine.
2. Run the `rosserial` node to establish communication.

## License
This firmware is developed for DIT Eurobot2025 and follows the applicable open-source licensing guidelines.

## Contact
For questions or contributions, please reach out to the DIT Eurobot team.

