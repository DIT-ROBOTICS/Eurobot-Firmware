#ifndef DEAD_WHEEL_H
#define DEAD_WHEEL_H

#include "stm32h7xx_hal.h"

// Timer for reading encoder CNT : 1k Hz
#define COUNT_TIME 0.001

// 4 * Resolution(2000) * GearBox(1) * CountTime
#define RES_Ratio_Dead 8

// Encoder 1 :
// 		ENC : TIM_5
// Encoder 2 :
// 		ENC : TIM_8
// Encoder 3 :
// 		ENC : TIM_23
// Encoder 4 :
// 		ENC : TIM_24

namespace Dead_Wheel {

class Encoder {
public:
    Encoder() {
    }

    // Initialize encoder data
    void Init(short num, TIM_HandleTypeDef *TIM);

    // Update encoder's Vnow
    void UpdateVnow();

    // Get encoder's Vnow
    double GetVnow();

    // Intergral distance
    double MoveDis();

private:
    // Encoder's number
    short num;

    // Timer for reading encoder CNT
    TIM_HandleTypeDef *TIM;

    /**
	 * @brief
	 * Record and reset CNT
	 * For get location to ROS
	 * */
	void Record_CNT();

	int32_t continue_CNT = 0;
	int16_t CNT = 0;

	double Vnow = 0.;
};

void Init();

}

#endif /* DEAD_WHEEL_H */
