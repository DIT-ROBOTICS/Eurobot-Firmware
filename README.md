# Eurobot-Firmware
The STM firmware for DIT Eurobot2025 Chassis

The firmware is use for omni-directional chassis with DIT Eurobot2025-12 robot 

If you want to modify the .ioc file, please make sure "MX_DMA_Init();" must be in front of "MX_USART1_UART_Init();" in main.c to establish rosserial communicaton
