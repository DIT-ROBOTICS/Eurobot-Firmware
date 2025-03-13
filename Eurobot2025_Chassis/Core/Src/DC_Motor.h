#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include "stm32h7xx_hal.h"

// Gearbox -> 22 : 1
// Encoder Resolution : 1024
// Wheel Radius : 0.024 m
// Car Radius Short : 0.104 m
// Car Radius Long : 0.131 m
// Timer for reading encoder CNT : 1k Hz
#define COUNT_TIME_DRIVING 0.001

// 4 * Resolution * GearBox * CountTime
#define RES_Ratio_Driving 90.112

// PWM frequency : 20 kHz (Timer Freq : 128 Mhz)
#define MOTOR_PWM_PULSE 6400

// TODO: Modify for new motor and encoder layout
// Motor 1 :
// 		ENC : TIM_1
// 		PWM : TIM_12-CH1 (PB14)
// 		DIR : PD8
// Motor 2 :
// 		ENC : TIM_2
// 		PWM : TIM_12-CH2 (PB15)
// 		DIR : PB13
// Motor 3 :
// 		ENC : TIM_3
// 		PWM : TIM_15-CH1 (PE5)
// 		DIR : PE3
// Motor 4 :
// 		ENC : TIM_4
// 		PWM : TIM_15-CH2 (PE6)
// 		DIR : PC13

namespace DC_Motor {

class Motor {
public:
	Motor() {
	}

	bool isMove = false;

	// Initialize motor data
	void Init(short num, TIM_HandleTypeDef *TIM, double P, double I);

	// Count PI value
	void UpdatePI();

	void UpdateVnow();

	void SetVgoal(double Vgoal);

	double MoveDis();

	double GetVnow();

	// For Motor PWM output
	double u = 0;

private:
	// The num of "this" object's instance.
	short num;

	TIM_HandleTypeDef *TIM;

	/**
	 * @brief
	 * Record and reset CNT
	 * For get location to ROS
	 * */
	void Record_CNT();

	int32_t continue_CNT = 0;
	int16_t CNT = 0;

	double Vnow = 0.0;
	double Vgoal = 0.0;

	// PID controller
	double I_lim = 1;
	double P, I;
	double i = 0;
	double error = 0, error_before = 0;
	double prev_u;

	// Collecting data. For matlab.
	//	double DC_motor_Vnow[2000];
	//	int DC_index = 0;

};

/**
 * @brief
 * For basic initialize :
 * 		Interrupt for DC_MOTOR
 * 		Encoder Mode
 * 		Motor PWM timer
 * */
void Init();

extern double WheelRadius[4];
extern double ROUND[4];
extern double CONST_FOR_MOTOR[4];

}

#endif /* DC_MOTOR_H */
