#include "stm32h7xx_hal.h"

#include "ROS_mainpp.h"
#include "ros.h"
#include "DebugMode.h"

#include "Omni.h"

// For ROS::loop
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim15;

ros::NodeHandle nh;

geometry_msgs::Twist CarVnow;
static CAR_INFO NowCarInfo;
static CAR_INFO NowCarLoc;

ros::Subscriber<geometry_msgs::Twist> CarVelSub("cmd_vel", ROS::GoalVel_CB);
ros::Subscriber<std_msgs::Bool> StartSub("/startornot", ROS::Start_CB);
ros::Subscriber<std_msgs::String> FinishSub("/mission0", ROS::Finish_CB);

#ifdef DEBUGGER_MODE
//ros::Subscriber<geometry_msgs::Pose> DebugCarGoalSub("/STM_Run", ROS::Test_GetGoal_CB);
//ros::Subscriber<std_msgs::Float64> Wheel_Sub("/STM_Wheel", ROS::Test_WheelRadius_CB);
//ros::Subscriber<std_msgs::Float64> CarRadius_Sub("/STM_CarRadius", ROS::Test_CarRadius_CB);
#endif

ros::Publisher CarVelPub("Toposition", &CarVnow);

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

	nh.advertise(CarVelPub);

	HAL_TIM_Base_Start_IT(&htim7);
}

void ROS::loop() {
	nh.spinOnce();
}

void ROS::PubCarVnow() {
	NowCarInfo = omni.GetNowCarInfo();
	NowCarLoc = omni.GetNowCarLocation();

	CarVnow.linear.x = NowCarInfo.Vx;
	CarVnow.linear.y = NowCarInfo.Vy;
	CarVnow.angular.z = NowCarInfo.Omega;

	CarVnow.angular.x = NowCarLoc.Vx;
	CarVnow.angular.y = NowCarLoc.Vy;
	CarVnow.linear.z = NowCarLoc.Omega;

	CarVelPub.publish(&CarVnow);
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

