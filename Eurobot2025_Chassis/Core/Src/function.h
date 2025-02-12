#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "stm32h7xx_hal.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

#endif
