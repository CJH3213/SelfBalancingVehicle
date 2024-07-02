/**
*	�������
*	ʹ��DRV88833��������ֱ�����
* ��һ������Ϊ�͵�ƽʱ�����û��ɲ������һ�����붼Ϊ�ߵ�ƽʱ�������ɲ��
*/

#include "Motors.h"
#include "tim.h"


/**** ����������� ****/

void BrakingRightMotor()
{
	// ��DRV8833���������붼��Ϊ�ߵ�ƽ����ɲ��
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, MAXSPEED);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, MAXSPEED);
}

void SetRightMotor(enum MotorDirection dir, uint16_t speed)
{
	// �������ռ�ձ�
	if(speed > MAXSPEED)
		speed = MAXSPEED;
	
	switch(dir)
	{
		case FORWARD:	// ��ת
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
			break;
		case REVERSE:	//��ת
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
			break;
		default:	// �����������ٶ�Ϊ0����ɲ��
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


/**** ���������ٲ��� ****/
int16_t _mLeftMotorEncoderCount = 0;
int16_t _mRightMotorEncoderCount = 0;
int16_t mLeftMotorSpeed = 0;
int16_t mRightMotorSpeed = 0;

void OnEXTIHandlerForMotorEncoders(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case MotorEncoder_Left_A_Pin:	// ����A���ж�
			HAL_GPIO_ReadPin(MotorEncoder_Left_B_GPIO_Port, MotorEncoder_Left_B_Pin)?
			_mLeftMotorEncoderCount ++ : _mLeftMotorEncoderCount --;
			return;
		case MotorEncoder_Right_A_Pin:	// �ҵ��B���ж�
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
		// ����תһȦԼΪ350������ֵ
		mLeftMotorSpeed = _mLeftMotorEncoderCount ;
		_mLeftMotorEncoderCount = 0;
		mRightMotorSpeed = _mRightMotorEncoderCount ;
		_mRightMotorEncoderCount = 0;
	
	}
}

int16_t GetLeftMotorSpeed(void){return mLeftMotorSpeed;}

int16_t GetRightMotorSpeed(void){return mRightMotorSpeed;}

