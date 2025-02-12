#include "main.h"
#include "mainpp.h"
#include "stm32h7xx_hal.h"

#include "ROS_mainpp.h"
#include "DebugMode.h"
#include "Omni.h"

// For LED blink
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim15;

void main_function() {
	// Init
	ROS::setup();
	omni.Init();

	// For LED blink
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);

#ifdef DEBUGGER_MODE
	DebugMode.DebuggingMode();
	return;
#endif

	while (true) {
	}
}
