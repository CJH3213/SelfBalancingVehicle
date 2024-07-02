/**
*	电机驱动
*	使用DRV88833驱动两个直流电机
* 当一组输入为低电平时，输出没有刹车，当一组输入都为高电平时，输出有刹车
*/

#include "Motors.h"
#include "tim.h"


/**** 电机驱动部分 ****/

void BrakingRightMotor()
{
	// 对DRV8833该组电机输入都置为高电平进行刹车
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, MAXSPEED);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, MAXSPEED);
}

void SetRightMotor(enum MotorDirection dir, uint16_t speed)
{
	// 限制最大占空比
	if(speed > MAXSPEED)
		speed = MAXSPEED;
	
	switch(dir)
	{
		case FORWARD:	// 正转
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
			break;
		case REVERSE:	//反转
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
			break;
		default:	// 其他参数，速度为0，无刹车
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
			break;
	}
}

void BrakingLeftMotor()
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, MAXSPEED);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, MAXSPEED);
}

void SetLeftMotor(enum MotorDirection dir, uint16_t speed)
{
	if(speed > MAXSPEED)
		speed = MAXSPEED;
	
	switch(dir)
	{
		case FORWARD:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);
			return;
		case REVERSE:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
			return;
		default:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
			return;
	}
}


/**** 编码器测速部分 ****/
int16_t _mLeftMotorEncoderCount = 0;
int16_t _mRightMotorEncoderCount = 0;
int16_t mLeftMotorSpeed = 0;
int16_t mRightMotorSpeed = 0;

void OnEXTIHandlerForMotorEncoders(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case MotorEncoder_Left_A_Pin:	// 左电机A相中断
			HAL_GPIO_ReadPin(MotorEncoder_Left_B_GPIO_Port, MotorEncoder_Left_B_Pin)?
			_mLeftMotorEncoderCount ++ : _mLeftMotorEncoderCount --;
			return;
		case MotorEncoder_Right_A_Pin:	// 右电机B相中断
			HAL_GPIO_ReadPin(MotorEncoder_Right_B_GPIO_Port, MotorEncoder_Right_B_Pin) ?
			_mRightMotorEncoderCount ++ : _mRightMotorEncoderCount --;
			return;
		default:
			return;
	}
}

void OnTimerHandlerForMotorEncoders(uint16_t ms)
{
	static uint16_t iicount = 0;
	
	iicount ++;
	
	if(iicount == 1000-1)
	{
		iicount = 0;
	
		HAL_GPIO_TogglePin(TestPort_GPIO_Port, TestPort_Pin);
		
		ms = 1;
		// 车轮转一圈约为350个计数值
		mLeftMotorSpeed = _mLeftMotorEncoderCount ;
		_mLeftMotorEncoderCount = 0;
		mRightMotorSpeed = _mRightMotorEncoderCount ;
		_mRightMotorEncoderCount = 0;
	
	}
}

int16_t GetLeftMotorSpeed(void){return mLeftMotorSpeed;}

int16_t GetRightMotorSpeed(void){return mRightMotorSpeed;}

