/**
*	�������
*	ʹ��DRV88833��������ֱ�����
* ��һ������Ϊ�͵�ƽʱ�����û��ɲ������һ�����붼Ϊ�ߵ�ƽʱ�������ɲ��
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

/**** ����������� ****/

void BrakingRightMotor()
{
	// ��DRV8833���������붼��Ϊ�ߵ�ƽ����ɲ��
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
	if(speed > MOTOR_PWM_MAX)	// ע���з��Ͳ��ܸ��޷��ͱȽ�
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


/**** ���������ٲ��� ****/
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
	
	if(tCount >= 5)	// ��϶�ʱ����ȷ��ÿ5ms��ȡһ�α������ֵ��Ϊ�ٶ�
	{
		tCount = 0;
		
		// ����תһȦԼΪ350������ֵ
		mLeftMotorSpeed = -__HAL_TIM_GET_COUNTER(&htim2) ;
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		mRightMotorSpeed = -__HAL_TIM_GET_COUNTER(&htim3) ;	
		__HAL_TIM_SET_COUNTER(&htim3, 0);
	}
}

int16_t GetLeftMotorSpeed(void){return mLeftMotorSpeed;}

int16_t GetRightMotorSpeed(void){return mRightMotorSpeed;}

