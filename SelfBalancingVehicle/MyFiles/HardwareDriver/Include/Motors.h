#ifndef __MOTORS_H
#define __MOTORS_H

#include "main.h"


/**** 电机驱动部分 ****/

// 最大占空比即定时器的最大计数值，1000
#define MAXSPEED (htim2.Init.Period)

// 方向枚举：0前进，1后退
enum MotorDirection
{
	FORWARD=0, REVERSE
};

// 左电机刹车
void BrakingLeftMotor(void);
// 设置左电机方向和速度
void SetLeftMotor(enum MotorDirection dir, uint16_t speed);
// 右电机刹车
void BrakingRightMotor(void);
// 设置右电机方向和速度
void SetRightMotor(enum MotorDirection dir, uint16_t speed);


/**** 编码器测速部分 ****/
// 外部输入中断回调，捕获编码器中断和计数
void OnEXTIHandlerForMotorEncoders(uint16_t GPIO_Pin);
// 定时器中断回调，将计数值转为速度值
void OnTimerHandlerForMotorEncoders(uint16_t ms);
// 获取左电机速度（圈/秒）
int16_t GetLeftMotorSpeed(void);
// 获取右电机速度（圈/秒）
int16_t GetRightMotorSpeed(void);

#endif
