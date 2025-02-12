#ifndef OMNI_H
#define OMNI_H

#include "DC_Motor.h"
#include "ROS_mainpp.h"

#define omni_angle 0.62465335
#define angle_cos 0.624
#define angle_sin 0.62465335

typedef struct {
	double Vx = 0.0;
	double Vy = 0.0;
	double Omega = 0.0;
} CAR_INFO;

// Omni control
class Omni {
public:
	Omni();

	// Initialize each motor settings.
	void Init();

	// Update each motor's Vnow and calculate NowCarInfo
	void UpdateNowCarInfo();

	// Set GoalCarInfo and Update each motor Vgoal.
	void SetGoalCarInfo(double Vx, double Vy, double Omega);

	void UpdateCarLocation();

	// Update each motor's PID value and set PWM.
	void Update_PID();

	// Move the car
	void Move();

	void SetCarRadius(double CarRadius);

	// Debug from Live Expressions
	void SetMotorVgoal();

	CAR_INFO GetNowCarInfo();

	CAR_INFO GetNowCarLocation();

	// Vgoal
	CAR_INFO GoalCarInfo;

private:

	// Get each motors' V_now
	void UpdateMotorVnow();

	DC_Motor::Motor motors[4];

	// Vnow
	CAR_INFO NowCarInfo;

	// Vgoal
//	CAR_INFO GoalCarInfo;

	CAR_INFO NowCarLocation;

	double CarRadius;
};

extern double CAR_RADIUS;
extern Omni omni;

#endif /* OMNI_H */
