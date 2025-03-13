#include "stm32h7xx_hal.h"

#include "ROS_mainpp.h"
#include "ros.h"
#include "DebugMode.h"

#include "Omni.h"

// For ROS::loop
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim23;
extern TIM_HandleTypeDef htim24;

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim15;

ros::NodeHandle nh;

geometry_msgs::Twist CarVnow_Dead;
static CAR_INFO NowCarInfo_Dead;
static CAR_INFO NowCarLoc_Dead;

geometry_msgs::Twist CarVnow_Driving;
static CAR_INFO NowCarInfo_Driving;
static CAR_INFO NowCarLoc_Driving;

ros::Subscriber<geometry_msgs::Twist> CarVelSub("cmd_vel", ROS::GoalVel_CB);
ros::Subscriber<std_msgs::Bool> StartSub("/startornot", ROS::Start_CB);
ros::Subscriber<std_msgs::String> FinishSub("/mission0", ROS::Finish_CB);

#ifdef DEBUGGER_MODE
//ros::Subscriber<geometry_msgs::Pose> DebugCarGoalSub("/STM_Run", ROS::Test_GetGoal_CB);
//ros::Subscriber<std_msgs::Float64> Wheel_Sub("/STM_Wheel", ROS::Test_WheelRadius_CB);
//ros::Subscriber<std_msgs::Float64> CarRadius_Sub("/STM_CarRadius", ROS::Test_CarRadius_CB);
#endif

ros::Publisher CarVelPub_Dead("odoo_googoogoo", &CarVnow_Dead);
ros::Publisher CarVelPub_Driving("driving_duaiduaiduai", &CarVnow_Driving);

void ROS::GoalVel_CB(const geometry_msgs::Twist &msg) {
	omni.SetGoalCarInfo(msg.linear.x, msg.linear.y, msg.angular.z);
}

void ROS::Start_CB(const std_msgs::Bool &msg) {
	if (msg.data == true) {
//		HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
//		HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
//		HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
//		HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
	}
}

void ROS::Finish_CB(const std_msgs::String &msg) {
	if (msg.data[0] == 'f' && msg.data[1] == '0') {
		HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_2);
	}
}

#ifdef DEBUGGER_MODE

void ROS::Test_GetGoal_CB(const geometry_msgs::Pose &msg) {
	if (msg.position.x != 0.0) {
		DebugMode.GoalLength = msg.position.x;
		if (msg.orientation.x != 0.0)
			DebugMode.Vx = msg.orientation.x;
		DebugMode.isVx = true;
	}
	else if (msg.position.y != 0.0) {
		DebugMode.GoalLength = msg.position.y;
		if (msg.orientation.y != 0.0)
			DebugMode.Vy = msg.orientation.y;
		DebugMode.isVy = true;
	}
	else if (msg.position.z != 0.0) {
		DebugMode.GoalLength = msg.position.z;
		if (msg.orientation.z != 0.0)
			DebugMode.Vomega = msg.orientation.z;
		DebugMode.isVomega = true;
	}
}

// TODO: Modify CarRadius for short and long if using this function.
void ROS::Test_CarRadius_CB(const std_msgs::Float64 &msg) {
	CAR_RADIUS = msg.data;
	DebugMode.UpdateCarConstant();
}

void ROS::Test_WheelRadius_CB(const std_msgs::Float64 &msg) {
	DC_Motor::WheelRadius = msg.data;
	DebugMode.UpdateCarConstant();
}

#endif

void ROS::setup() {
	nh.initNode();

	nh.subscribe(CarVelSub);
	nh.subscribe(StartSub);
	nh.subscribe(FinishSub);

#ifdef DEBUGGER_MODE
	nh.subscribe(DebugCarGoalSub);
	nh.subscribe(Wheel_Sub);
	nh.subscribe(CarRadius_Sub);
#endif

	nh.advertise(CarVelPub_Dead);
	nh.advertise(CarVelPub_Driving);

	HAL_TIM_Base_Start_IT(&htim7);
}

void ROS::loop() {
	nh.spinOnce();
}
// int i = 0;
// double MotorVnow_0[1000];
// double MotorVnow_1[1000];
// double MotorVnow_2[1000];
// double MotorVnow_3[1000];

void ROS::PubCarVnow_Dead() {
	NowCarInfo_Dead = omni.GetNowCarInfo_Dead();
	NowCarLoc_Dead = omni.GetNowCarLocation_Dead();

	CarVnow_Dead.linear.x = NowCarInfo_Dead.Vx;
	CarVnow_Dead.linear.y = NowCarInfo_Dead.Vy;
	CarVnow_Dead.angular.z = NowCarInfo_Dead.Omega;

	CarVnow_Dead.angular.x = NowCarLoc_Dead.Vx;
	CarVnow_Dead.angular.y = NowCarLoc_Dead.Vy;
	CarVnow_Dead.linear.z = NowCarLoc_Dead.Omega;

//	CarVnow_Dead.linear.x = omni.GetMotorVnow(0);
//	CarVnow_Dead.linear.y = omni.GetMotorVnow(1);
//	CarVnow_Dead.angular.z = omni.GetMotorVnow(2);
//
//	CarVnow_Dead.angular.x = omni.GetMotorVnow(3);
//	CarVnow_Dead.angular.y = NowCarLoc_Dead.Vy;
//	CarVnow_Dead.linear.z = NowCarLoc_Dead.Omega;

//	if(i<1000){
//		MotorVnow_0[i] = omni.GetMotorVnow(0);
//		MotorVnow_1[i] = omni.GetMotorVnow(1);
//		MotorVnow_2[i] = omni.GetMotorVnow(2);
//		MotorVnow_3[i] = omni.GetMotorVnow(3);
//	}
//	if(omni.GoalCarInfo.Vx != 0 || omni.GoalCarInfo.Vy != 0)	i++;

//	CarVnow_Dead.linear.x = TIM5->CNT;
//	CarVnow_Dead.linear.y = TIM8->CNT;
//	CarVnow_Dead.angular.z = TIM23->CNT;
//
//	CarVnow_Dead.angular.x = TIM24->CNT;
//	CarVnow_Dead.angular.y = NowCarLoc_Dead.Vy;
//	CarVnow_Dead.linear.z = NowCarLoc_Dead.Omega;

	CarVelPub_Dead.publish(&CarVnow_Dead);
}

void ROS::PubCarVnow_Driving() {
	NowCarInfo_Driving = omni.GetNowCarInfo_Driving();
	NowCarLoc_Driving = omni.GetNowCarLocation_Driving();

	CarVnow_Driving.linear.x = NowCarInfo_Driving.Vx;
	CarVnow_Driving.linear.y = NowCarInfo_Driving.Vy;
	CarVnow_Driving.angular.z = NowCarInfo_Driving.Omega;

	CarVnow_Driving.angular.x = NowCarLoc_Driving.Vx;
	CarVnow_Driving.angular.y = NowCarLoc_Driving.Vy;
	CarVnow_Driving.linear.z = NowCarLoc_Driving.Omega;

	CarVelPub_Driving.publish(&CarVnow_Driving);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	nh.getHardware()->reset_rbuf();
}

void ROS::init(void) {
//	nh.getHardware()->init();
}

