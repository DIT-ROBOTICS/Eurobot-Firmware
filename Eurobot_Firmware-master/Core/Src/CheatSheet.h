/*
 * This file is STM32 api Cheat Sheet.
 * Don't include this header file into your code. This is useless. ==
 *
 */

/*
 *
 * HAL_GPIO_WritePin(GPIO"B", GPIO_PIN_"0", GPIO_PIN_"SET / RESET");
 * HAL_Delay("500");   // In MicroSecond
 * HAL_GPIO_TogglePin(GPIO"B", GPIO_PIN_"0");
 * HAL_GPIO_ReadPin(GPIO"F", GPIO_PIN_"13");   // return 1 / 0
 *
 * *************************** ADC ***************************
 *
 * -> ADC Start
 * HAL_ADC_Start(ADC_HandleTypeDef *hadc);
 *
 * -> Wait for the ADC convert.
 * HAL_ADC_PollForConversion(ADC_HandleTypeDef *hadc, uint32_t Timeout);
 *
 * -> Get the return value of ADC
 * HAL_ADC_GetValue(ADC_HandleTypeDef *hadc);
 *
 * *************************** Base TIMER ***************************
 *
 * -> Base Timer Start
 * HAL_TIM_Base_Start(&htim"2");
 *
 * -> Base Timer Start with interrupt
 * HAL_TIM_Base_Start_IT(&htim"2");
 *
 * -> Set the Timer's PSR
 * __HAL_TIM_SET_PRESCALER(&htim"2", 63);
 *
 * -> Get the Timer's CNT
 * __HAL_TIM_GET_COUNTER(&htim"2");
 *
 * -> Set the Timer's ARR.
 * __HAL_TIM_SET_AUTORELOAD(&htim"1", 100);
 *
 * *************************** PWM TIMER ***************************
 *
 * -> PWM Timer Start
 * HAL_TIM_PWM_Start(&htim"2", TIM_CHANNEL_"1");
 *
 * -> PWM Timer Start with interrupt
 * HAL_TIM_PWM_Start_IT(&htim"2", TIM_CHANNEL_"1");
 *
 * -> Set the Timer's Pulse (Recommand : ARR * Duty)
 * __HAL_TIM_SET_COMPARE(&htim"2", TIM_CHANNEL_"1", "wanted_pulse");
 *
 * *************************** GPIO Interrupt ***************************
 *
 * -> GPIO External Interupt
 * void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
 *
 * *************************** Callback ***************************
 *
 * -> Timer Callback
 * void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
 *
 * -> PWM Callback
 * void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
 *
 */
