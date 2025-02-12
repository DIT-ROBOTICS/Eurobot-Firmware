#ifndef _DEBUGMODE_H_
#define _DEBUGMODE_H_

// NOTE : Uncomment the define below to enable debugmode.
//#define DEBUGGER_MODE

#ifdef DEBUGGER_MODE

#include <cmath>
#include "Omni.h"
#include "DC_Motor.h"

class DEBUGMODE {
public:
	void DebuggingMode();

	double Vx = 0.02;
	double Vy = 0.02;
	double Vomega = 0.02;

	bool isVx = false;
	bool isVy = false;
	bool isVomega = false;
	bool isUpdateConstant = false;

	double GoalLength = 1.0;

	void UpdateCarConstant();

	void RunVx(double GoalLength);
	void RunVy(double GoalLength);
	void RunVomega(double GoalLength);

	double Range = 0.005;
	bool isReach(double a, double b, double range);
};

extern DEBUGMODE DebugMode;

#endif

#endif
