#ifndef __MOTORS_H
#define __MOTORS_H

#include "main.h"


/**** 电机驱动部分 ****/

// 最大占空比即定时器的最大计数值，1000
#define MOTOR_PWM_HTIM htim1
#define MOTOR_PWM_MAX ((int16_t)MOTOR_PWM_HTIM.Init.Period)

// 初始化电机驱动程序和硬件
void InitMotorDriver(void);

// 左电机刹车
void BrakingLeftMotor(void);
// 设置左电机方向和速度
void SetLeftMotor(int16_t speed);
// 右电机刹车
void BrakingRightMotor(void);
// 设置右电机方向和速度
void SetRightMotor(int16_t speed);


/**** 编码器测速部分 ****/
#define MOTOR_ENCODERS_TIM_PERIOD 1	// 编码器处理所在定时器的中断周期：1ms

// 初始化电机驱动程序和硬件
void InitMotorEncodersDriver(void);
// 定时器中断回调，将计数值转为速度值
void OnTIMForMotorEncoders(void);
// 获取左电机速度（圈/秒）
int16_t GetLeftMotorSpeed(void);
// 获取右电机速度（圈/秒）
int16_t GetRightMotorSpeed(void);

#endif
