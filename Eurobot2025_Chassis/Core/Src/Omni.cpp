#include "Omni.h"

#include "DC_Motor.h"
#include "Dead_Wheel.h"
#include "geometry_msgs/Twist.h"

// Encoder for Motor
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

// Encoder for Dead Wheel
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim23;
extern TIM_HandleTypeDef htim24;

// Motor PWM
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim15;

Omni omni;

CAR_DIMENSION CAR_RADIUS;

Omni::Omni() {
}

void Omni::Init() {
	this->motors[0].Init(0, &htim5, 3.7, 471.0);
	this->motors[1].Init(1, &htim8, 3.7, 471.0);
	this->motors[2].Init(2, &htim23, 3.7, 471.0);
	this->motors[3].Init(3, &htim24, 3.7, 471.0);

	this->encoders[0].Init(0, &htim1);
	this->encoders[1].Init(1, &htim2);
	this->encoders[2].Init(2, &htim3);
	this->encoders[3].Init(3, &htim4);

	SetCarRadius(CAR_RADIUS);

	DC_Motor::Init();
	Dead_Wheel::Init();
}

// ** Dead Wheel Encoder **
// TODO: Check for kinematics - wheel to robot
// TODO: Modify encoder use
// ------------------------
//	     1    x    \               
//	    /-----|-----\            
//	   / |    |    | 0          
//	     |    |    |               
//	   y-------    |           
//	     |         |            
//	   2 |         | /            
//	    \-----------/            
//	     \         3              
// ------------------------
void Omni::UpdateNowCarInfo_Dead() {
	// Get each encoders' Vnow
	// Unit : m/s , rad/s
	this->UpdateEncoderVnow();

	NowCarInfo_Dead.Vx = sqrt(2) * (encoders[0].GetVnow() - encoders[1].GetVnow()
		- encoders[2].GetVnow() + encoders[3].GetVnow()) / 4.0;
	NowCarInfo_Dead.Vy = sqrt(2) * (encoders[0].GetVnow() + encoders[1].GetVnow()
		- encoders[2].GetVnow() - encoders[3].GetVnow()) / 4.0;
	NowCarInfo_Dead.Omega = (encoders[0].GetVnow() + encoders[1].GetVnow()
		+ encoders[2].GetVnow() + encoders[3].GetVnow()) / (CarRadius_.Sq * 4.0);
}
void Omni::UpdateCarLocation_Dead() {
	double e[4];
	for (int i = 0; i < 4; i++) {
		e[i] = this->encoders[i].MoveDis() / 1000.0;
	}
	NowCarLocation_Dead.Vx += sqrt(2) * (e[0] - e[1] - e[2] + e[3]) / 4.0;
	NowCarLocation_Dead.Vy += sqrt(2) * (e[0] + e[1] - e[2] - e[3]) / 4.0;
	NowCarLocation_Dead.Omega += (e[0] + e[1] + e[2] + e[3]) / (CarRadius_.Sq * 4.0);
}

// ** Driving Wheel Encoder **
// TODO: Check for kinematics - wheel to robot
// ------------------------
// 		-----0-----
// 		|         |
// 		1         3
// 		|         |
// 		-----2-----
// ------------------------
void Omni::UpdateNowCarInfo_Driving() {
	// Get each motors' Vnow
	// Unit : m/s , rad/s
	this->UpdateMotorVnow();

	NowCarInfo_Driving.Vx = (-motors[1].GetVnow() + motors[3].GetVnow()) / (2.0);
	NowCarInfo_Driving.Vy = (motors[0].GetVnow() - motors[2].GetVnow()) / (2.0);
	NowCarInfo_Driving.Omega = ((motors[0].GetVnow() +  motors[2].GetVnow()) / (2.0 * CarRadius_.Short)
		+ (motors[1].GetVnow() + motors[3].GetVnow())/(2.0 * CarRadius_.Long)) / 2.0;
}
void Omni::UpdateCarLocation_Driving() {
	double m[4];
	for (int i = 0; i < 4; i++) {
		m[i] = this->motors[i].MoveDis() / 1000.0;
	}
	NowCarLocation_Driving.Vx += (-m[1] + m[3])/(2.0);
	NowCarLocation_Driving.Vy += (m[0] - m[2])/(2.0);
	NowCarLocation_Driving.Omega += ((m[0] +  m[2])/(2.0 * CarRadius_.Short) + (m[1] + m[3]) / (2.0 * CarRadius_.Long)) / 2.0;
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

void Omni::UpdateEncoderVnow() {
	for (int i = 0; i < 4; i++) {
		this->encoders[i].UpdateVnow();
	}
}

// TODO : Check for the DIR
void Omni::Move() {
	// DIR
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, (motors[0].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, (motors[1].u > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, (motors[2].u > 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (motors[3].u > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// PWM
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, int(fabs(motors[0].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, int(fabs(motors[1].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, int(fabs(motors[2].u) * MOTOR_PWM_PULSE));
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, int(fabs(motors[3].u) * MOTOR_PWM_PULSE));
}

// TODO: Check for kinematics - robot to wheel
// ------------------------
// 		-----0-----
// 		|         |
// 		1         3
// 		|         |
// 		-----2-----
// ------------------------
void Omni::SetMotorVgoal() {
	// Unit : m/s
	this->motors[0].SetVgoal(GoalCarInfo.Vy + CarRadius_.Short * GoalCarInfo.Omega);
	this->motors[1].SetVgoal(-GoalCarInfo.Vx + CarRadius_.Long * GoalCarInfo.Omega);
	this->motors[2].SetVgoal(-GoalCarInfo.Vy + CarRadius_.Short * GoalCarInfo.Omega);
	this->motors[3].SetVgoal(GoalCarInfo.Vx + CarRadius_.Long * GoalCarInfo.Omega);
}

// ** Dead Wheel Encoder **
CAR_INFO Omni::GetNowCarInfo_Dead() {
	return NowCarInfo_Dead;
}
CAR_INFO Omni::GetNowCarLocation_Dead() {
	return NowCarLocation_Dead;
}

// ** Driving Wheel Encoder **
CAR_INFO Omni::GetNowCarInfo_Driving() {
	return NowCarInfo_Driving;
}
CAR_INFO Omni::GetNowCarLocation_Driving() {
	return NowCarLocation_Driving;
}

void Omni::SetCarRadius(CAR_DIMENSION CarRadius) {
	this->CarRadius_.Short = CarRadius.Short;
	this->CarRadius_.Long = CarRadius.Long;
}
