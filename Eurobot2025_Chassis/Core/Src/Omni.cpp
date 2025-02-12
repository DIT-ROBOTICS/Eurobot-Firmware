#include "Omni.h"

#include "DC_Motor.h"
#include "geometry_msgs/Twist.h"

// Encoder
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

// Motor PWM
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim15;

Omni omni;

double CAR_RADIUS = 0.118901;

Omni::Omni() {
}

void Omni::Init() {
	this->motors[0].Init(0, &htim1, 3.7, 471.0);
	this->motors[1].Init(1, &htim2, 3.7, 471.0);
	this->motors[2].Init(2, &htim3, 3.7, 471.0);
	this->motors[3].Init(3, &htim4, 3.7, 471.0);

	SetCarRadius(CAR_RADIUS);

	DC_Motor::Init();
}

void Omni::UpdateCarLocation() {
	double m[4];
	for (int i = 0; i < 4; i++) {
		m[i] = this->motors[i].MoveDis() / 1000.0;
	}
	NowCarLocation.Vx += (-m[0]*cos(omni_angle) - m[1]*cos(omni_angle) + m[2]*cos(omni_angle) + m[3]*cos(omni_angle))/(4.0*0.3912*1.3157);
	NowCarLocation.Vy += (m[0]*sin(omni_angle) - m[1]*sin(omni_angle) - m[2]*sin(omni_angle) + m[3]*sin(omni_angle))/(4.0*0.4176*1.1179);
	NowCarLocation.Omega += (m[0] + m[1] + m[2] + m[3]) / (CarRadius*4.0*0.9543);
}

void Omni::UpdateNowCarInfo() {
	// Get each motors' Vnow
	// Unit : m/s , rad/s
	// Angle : 0.62465335 * 2
	this->UpdateMotorVnow();

	NowCarInfo.Vx = (-motors[0].GetVnow()*cos(omni_angle) - motors[1].GetVnow()*cos(omni_angle) + motors[2].GetVnow()*cos(omni_angle) + motors[3].GetVnow()*cos(omni_angle))/(4.0*0.3912*1.3157);
	NowCarInfo.Vy = (motors[0].GetVnow()*sin(omni_angle) - motors[1].GetVnow()*sin(omni_angle) - motors[2].GetVnow()*sin(omni_angle) + motors[3].GetVnow()*sin(omni_angle))/(4.0*0.4176*1.1179);
	NowCarInfo.Omega = (motors[0].GetVnow() + motors[1].GetVnow() + motors[2].GetVnow() + motors[3].GetVnow()) / (CarRadius*4.0*0.9543);
}

// Set all motors' velocity base on Car Vgoal.
void Omni::SetGoalCarInfo(double Vx, double Vy, double Omega) {
	this->GoalCarInfo.Vx = Vx;
	this->GoalCarInfo.Vy = Vy;
	this->GoalCarInfo.Omega = Omega;

	SetMotorVgoal();
}

void Omni::Update_PID() {
	for (int i = 0; i < 4; i++) {
		this->motors[i].UpdatePI();
	}
}

void Omni::UpdateMotorVnow() {
	for (int i = 0; i < 4; i++) {
		this->motors[i].UpdateVnow();
	}
}

// TODO : Check for the DIR
void Omni::Move() {
	// DIR
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, (motors[0].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (motors[1].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, (motors[2].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (motors[3].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);

	// PWM
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, int(fabs(motors[0].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, int(fabs(motors[1].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, int(fabs(motors[2].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, int(fabs(motors[3].u) * MOTOR_PWM_PULSE));
}

void Omni::SetMotorVgoal() {
	// Unit : m/s
	this->motors[0].SetVgoal(-GoalCarInfo.Vx*(0.6346) + GoalCarInfo.Vy*(0.7983) + 0.1135 * GoalCarInfo.Omega);
	this->motors[1].SetVgoal(-GoalCarInfo.Vx*(0.6346) - GoalCarInfo.Vy*(0.7983) + 0.1135 * GoalCarInfo.Omega);
	this->motors[2].SetVgoal(GoalCarInfo.Vx*(0.6346) - GoalCarInfo.Vy*(0.7983) + 0.1135 * GoalCarInfo.Omega);
	this->motors[3].SetVgoal(GoalCarInfo.Vx*(0.6346) + GoalCarInfo.Vy*(0.7983) + 0.1135 * GoalCarInfo.Omega);
}

CAR_INFO Omni::GetNowCarInfo() {
	return NowCarInfo;
}

CAR_INFO Omni::GetNowCarLocation() {
	return NowCarLocation;
}

void Omni::SetCarRadius(double CarRadius) {
	this->CarRadius = CarRadius;
}

