/**
*	�������
*	ʹ��DRV88833��������ֱ�����
* ��һ������Ϊ�͵�ƽʱ�����û��ɲ������һ�����붼Ϊ�ߵ�ƽʱ�������ɲ��
*/

#include "Motors.h"
#include "tim.h"
#include <stdio.h>


/**** ����������� ****/

void BrakingRightMotor()
{
	// ��DRV8833���������붼��Ϊ�ߵ�ƽ����ɲ��
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, MAXSPEED);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, MAXSPEED);
}

void SetRightMotor(int16_t speed)
{
	if(speed > MAXSPEED)
		speed = MAXSPEED;
	else if(speed < -MAXSPEED)
		speed = -MAXSPEED;
	
	if(speed >= 0)
	{		
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -speed);
	}
}

void BrakingLeftMotor()
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, MAXSPEED);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, MAXSPEED);
}

void SetLeftMotor(int16_t speed)
{
	if(speed > MAXSPEED)	// ע���з��Ͳ��ܸ��޷��ͱȽ�
		speed = MAXSPEED;
	else if(speed < -MAXSPEED)
		speed = -MAXSPEED;
	
	if(speed >= 0)
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);
	}
	else
	{		
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, -speed);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0);
	}
}


/**** ���������ٲ��� ****/
int16_t _mLeftMotorEncoderCount = 0;
int16_t _mRightMotorEncoderCount = 0;
int16_t mLeftMotorSpeed = 0;
int16_t mRightMotorSpeed = 0;

void OnEXTIForMotorEncoders(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case MotorEncoder_Left_A_Pin:	// ����A���ж�
			HAL_GPIO_ReadPin(MotorEncoder_Left_A_GPIO_Port, MotorEncoder_Left_A_Pin) ==
			HAL_GPIO_ReadPin(MotorEncoder_Left_B_GPIO_Port, MotorEncoder_Left_B_Pin) ?
			_mLeftMotorEncoderCount ++ : _mLeftMotorEncoderCount --;
			return;
		case MotorEncoder_Left_B_Pin:	// ����B���ж�
			HAL_GPIO_ReadPin(MotorEncoder_Left_A_GPIO_Port, MotorEncoder_Left_A_Pin) !=
			HAL_GPIO_ReadPin(MotorEncoder_Left_B_GPIO_Port, MotorEncoder_Left_B_Pin) ?
			_mLeftMotorEncoderCount ++ : _mLeftMotorEncoderCount --;
			return;
		case MotorEncoder_Right_B_Pin:	// �ҵ��A���ж�
			HAL_GPIO_ReadPin(MotorEncoder_Right_A_GPIO_Port, MotorEncoder_Right_A_Pin) ==
			HAL_GPIO_ReadPin(MotorEncoder_Right_B_GPIO_Port, MotorEncoder_Right_B_Pin) ?
			_mRightMotorEncoderCount ++ : _mRightMotorEncoderCount --;
			return;
		case MotorEncoder_Right_A_Pin:	// �ҵ��B���ж�
			HAL_GPIO_ReadPin(MotorEncoder_Right_A_GPIO_Port, MotorEncoder_Right_A_Pin) !=
			HAL_GPIO_ReadPin(MotorEncoder_Right_B_GPIO_Port, MotorEncoder_Right_B_Pin) ?
			_mRightMotorEncoderCount ++ : _mRightMotorEncoderCount --;
			return;
		default:
			return;
	}
}

void OnTIMForMotorEncoders(uint16_t ms)
{
	static uint16_t tCount = 0;
	UNUSED(ms);

	tCount ++;
	
	if(tCount >= 5)	// ��϶�ʱ����ȷ��ÿ5ms��ȡһ�α������ֵ��Ϊ�ٶ�
	{
		tCount = 0;
	
		// HAL_GPIO_TogglePin(TestPort_GPIO_Port, TestPort_Pin);
		
		ms = 1;
		// ����תһȦԼΪ350������ֵ
		mLeftMotorSpeed = _mLeftMotorEncoderCount ;
		_mLeftMotorEncoderCount = 0;
		mRightMotorSpeed = _mRightMotorEncoderCount ;
		_mRightMotorEncoderCount = 0;
	
	}
}

int16_t GetLeftMotorSpeed(void){return mLeftMotorSpeed;}

int16_t GetRightMotorSpeed(void){return mRightMotorSpeed;}

