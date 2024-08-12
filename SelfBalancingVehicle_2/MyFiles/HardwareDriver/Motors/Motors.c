/**
*	电机驱动
*	使用DRV88833驱动两个直流电机
* 当一组输入为低电平时，输出没有刹车，当一组输入都为高电平时，输出有刹车
*/

#include "Motors.h"
#include <stdio.h>
#include "tim.h"


void InitMotorDriver()
{
	HAL_TIM_PWM_Start(&MOTOR_PWM_HTIM, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&MOTOR_PWM_HTIM, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&MOTOR_PWM_HTIM, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&MOTOR_PWM_HTIM, TIM_CHANNEL_4);
	SetLeftMotor(0);
	SetRightMotor(0);
}

/**** 电机驱动部分 ****/

void BrakingRightMotor()
{
	// 对DRV8833该组电机输入都置为高电平进行刹车
	__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_1, MOTOR_PWM_MAX);
	__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_2, MOTOR_PWM_MAX);
}

void SetRightMotor(int16_t speed)
{
	if(speed > MOTOR_PWM_MAX)
		speed = MOTOR_PWM_MAX;
	else if(speed < -MOTOR_PWM_MAX)
		speed = -MOTOR_PWM_MAX;
	
	if(speed >= 0)
	{		
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_2, speed);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_1, -speed);
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_2, 0);
	}
}

void BrakingLeftMotor()
{
	__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_3, MOTOR_PWM_MAX);
	__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_4, MOTOR_PWM_MAX);
}

void SetLeftMotor(int16_t speed)
{
	if(speed > MOTOR_PWM_MAX)	// 注意有符型不能跟无符型比较
		speed = MOTOR_PWM_MAX;
	else if(speed < -MOTOR_PWM_MAX)
		speed = -MOTOR_PWM_MAX;
	
	if(speed >= 0)
	{
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_4, speed);
	}
	else
	{		
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_3, -speed);
		__HAL_TIM_SET_COMPARE(&MOTOR_PWM_HTIM, TIM_CHANNEL_4, 0);
	}
}


/**** 编码器测速部分 ****/
int16_t mLeftMotorSpeed = 0;
int16_t mRightMotorSpeed = 0;

void InitMotorEncodersDriver()
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

void OnTIMForMotorEncoders()
{
	static uint16_t tCount = 0;

	tCount ++;
	
	if(tCount >= 5)	// 结合定时器，确保每5ms读取一次编码计数值作为速度
	{
		tCount = 0;
		
		// 车轮转一圈约为350个计数值
		mLeftMotorSpeed = -__HAL_TIM_GET_COUNTER(&htim2) ;
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		mRightMotorSpeed = -__HAL_TIM_GET_COUNTER(&htim3) ;	
		__HAL_TIM_SET_COUNTER(&htim3, 0);
	}
}

int16_t GetLeftMotorSpeed(void){return mLeftMotorSpeed;}

int16_t GetRightMotorSpeed(void){return mRightMotorSpeed;}

