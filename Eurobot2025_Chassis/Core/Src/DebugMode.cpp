#include "DebugMode.h"

#ifdef DEBUGGER_MODE

#include "ROS_mainpp.h"

DEBUGMODE DebugMode;

void DEBUGMODE::RunVx(double GoalLength) {

	CAR_INFO cur_car_info_;
	CAR_INFO goal_car_info_;

	goal_car_info_.Vx = omni.GetNowCarLocation().Vx + GoalLength;

	if (GoalLength >= 0) {
		omni.SetGoalCarInfo(this->Vx, 0.0, 0.0);
		while (isVx) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Vx >= goal_car_info_.Vx) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
	else {
		omni.SetGoalCarInfo(-this->Vx, 0.0, 0.0);
		while (isVx) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Vx <= goal_car_info_.Vx) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
}

void DEBUGMODE::RunVy(double GoalLength) {

	CAR_INFO cur_car_info_;
	CAR_INFO goal_car_info_;

	goal_car_info_.Vy = omni.GetNowCarLocation().Vy + GoalLength;

	if (GoalLength >= 0) {
		omni.SetGoalCarInfo(0.0, this->Vy, 0.0);
		while (isVy) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Vy >= goal_car_info_.Vy) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
	else {
		omni.SetGoalCarInfo(0.0, -this->Vy, 0.0);
		while (isVy) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Vy <= goal_car_info_.Vy) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
}

void DEBUGMODE::RunVomega(double GoalLength) {

	CAR_INFO cur_car_info_;
	CAR_INFO goal_car_info_;

	goal_car_info_.Omega = omni.GetNowCarLocation().Omega + GoalLength;

	if (GoalLength >= 0) {
		omni.SetGoalCarInfo(0.0, 0.0, this->Vomega);
		while (isVomega) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Omega >= goal_car_info_.Omega) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
	else {
		omni.SetGoalCarInfo(0.0, 0.0, -this->Vomega);
		while (isVomega) {
			cur_car_info_ = omni.GetNowCarLocation();
			if (cur_car_info_.Omega <= goal_car_info_.Omega) {
				omni.SetGoalCarInfo(0.0, 0.0, 0.0);
				break;
			}
		}
	}
}

bool DEBUGMODE::isReach(double a, double b, double range) {
	return (fabs(a - b) < range);
}

// TODO: Modify CarRadius for short and long if using this function.
void DEBUGMODE::UpdateCarConstant() {
	DC_Motor::ROUND = 2 * DC_Motor::WheelRadius * 3.14159;

	DC_Motor::CONST_FOR_MOTOR[0] = DC_Motor::ROUND / RES_Ratio;
	DC_Motor::CONST_FOR_MOTOR[1] = -DC_Motor::ROUND / RES_Ratio;
	DC_Motor::CONST_FOR_MOTOR[2] = DC_Motor::ROUND / RES_Ratio;
	DC_Motor::CONST_FOR_MOTOR[3] = -DC_Motor::ROUND / RES_Ratio;

	omni.SetCarRadius(CAR_RADIUS);
}

void DEBUGMODE::DebuggingMode() {
	while (true) {
		if (isVx) {
			RunVx(this->GoalLength);
			isVx = false;
		}

		if (isVy) {
			RunVy(this->GoalLength);
			isVy = false;
		}

		if (isVomega) {
			RunVomega(this->GoalLength);
			isVomega = false;
		}

		if (isUpdateConstant) {
			UpdateCarConstant();
			isUpdateConstant = false;
		}
	}
}

#endif
