#ifndef ROS_MAINPP_H_
#define ROS_MAINPP_H_

#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float64.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"

#define ROS_CAR_PUB_FREQUENCY 10
#define ROS_STOP_PUB_FREQUENCY 100

namespace ROS {

void setup();
void loop();
void init();

// Publisher
void PubCarVnow_Dead();
void PubCarVnow_Driving();
void PubResetNavigation(bool data);

// Subscriber
void GoalVel_CB(const geometry_msgs::Twist &msg);
void Start_CB(const std_msgs::Bool &msg);
void Finish_CB(const std_msgs::String &msg);

#ifdef DEBUGGER_MODE
void Test_GetGoal_CB(const geometry_msgs::Pose &msg);
void Test_CarRadius_CB(const std_msgs::Float64 &msg);
void Test_WheelRadius_CB(const std_msgs::Float64 &msg);
#endif

}

#endif /* ROS_MAINPP_H_ */
