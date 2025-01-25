#include "function.h"

#include "ROS_mainpp.h"
#include "DebugMode.h"

#include "stm32h7xx_hal.h"

#include <algorithm>
#include "Omni.h"

double Vx_goal = 0.0;
double Vy_goal = 0.0;
double Vz_goal = 0.0;

// ROS spinOnce
extern TIM_HandleTypeDef htim7;

// Mecanum
extern TIM_HandleTypeDef htim12;

// Count ROS frequency.
static int ROS_CAR_FREQUENCY = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM13) {
		// Update Car Vnow
		omni.UpdateNowCarInfo();

		// Update four wheel's PID value.
		omni.Update_PID();

		// Output GPIO and PWM
		omni.Move();

		// Debug from Live Expressions ( Optional )
		omni.SetMotorVgoal();
//		omni.SetGoalCarInfo(Vx_goal,Vy_goal,Vz_goal);]

		omni.UpdateCarLocation();

		// ROS pub -> Mecanum
		if (++ROS_CAR_FREQUENCY >= ROS_CAR_PUB_FREQUENCY) {
			ROS_CAR_FREQUENCY = 0;
			ROS::PubCarVnow();
		}
	}
	else if (htim->Instance == TIM7) {
		ROS::loop();
	}
}
