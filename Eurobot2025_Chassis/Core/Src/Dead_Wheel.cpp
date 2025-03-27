# include "stm32h7xx_hal.h"
# include "Dead_Wheel.h"

// Timer Interrupt
extern TIM_HandleTypeDef htim16;

// Encoder
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim23;
extern TIM_HandleTypeDef htim24;

namespace Dead_Wheel {

// TODO: Check for data
double WheelRadius[4] = {0.0164207, 0.01625, 0.0165633, 0.0161001};
double ROUND[4] = {2 * WheelRadius[0] * 3.14159, 2 * WheelRadius[1] * 3.14159, 2 * WheelRadius[2] * 3.14159, 2 * WheelRadius[3] * 3.14159};
double CONST_FOR_ENCODER[4] = { ROUND[0] / RES_Ratio_Dead, -ROUND[1] / RES_Ratio_Dead, ROUND[2] / RES_Ratio_Dead, -ROUND[3] / RES_Ratio_Dead };

void Init() {
	// Init interrupt for reading encoder's CNT
	HAL_TIM_Base_Start_IT(&htim16);

    // Init Encoder
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);  // Encoder[0]
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);  // Encoder[1]
    HAL_TIM_Encoder_Start(&htim23, TIM_CHANNEL_ALL); // Encoder[2]
    HAL_TIM_Encoder_Start(&htim24, TIM_CHANNEL_ALL); // Encoder[3]
}

void Encoder::Init(short num, TIM_HandleTypeDef *TIM) {
    this->num = num;
    this->TIM = TIM;
}

void Encoder::UpdateVnow() {
    // First, get CNT for this loop
    CNT = __HAL_TIM_GET_COUNTER(TIM);

    Record_CNT();
    Vnow = CNT * CONST_FOR_ENCODER[num];  // Unit : m / s

    // Finally, reset the counter CNT
    __HAL_TIM_SET_COUNTER(TIM, 0);
}

void Encoder::Record_CNT() {
    continue_CNT += CNT;
}

double Encoder::MoveDis() {
    double dis = continue_CNT * CONST_FOR_ENCODER[num];

    continue_CNT = 0;

    return dis;
}

double Encoder::GetVnow() {
    return Vnow;
}

}
