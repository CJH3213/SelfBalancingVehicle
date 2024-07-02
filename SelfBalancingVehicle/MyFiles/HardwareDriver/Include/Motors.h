#ifndef __MOTORS_H
#define __MOTORS_H

#include "main.h"


/**** ����������� ****/

// ���ռ�ձȼ���ʱ����������ֵ��1000
#define MAXSPEED (htim2.Init.Period)

// ����ö�٣�0ǰ����1����
enum MotorDirection
{
	FORWARD=0, REVERSE
};

// ����ɲ��
void BrakingLeftMotor(void);
// ��������������ٶ�
void SetLeftMotor(enum MotorDirection dir, uint16_t speed);
// �ҵ��ɲ��
void BrakingRightMotor(void);
// �����ҵ��������ٶ�
void SetRightMotor(enum MotorDirection dir, uint16_t speed);


/**** ���������ٲ��� ****/
// �ⲿ�����жϻص�������������жϺͼ���
void OnEXTIHandlerForMotorEncoders(uint16_t GPIO_Pin);
// ��ʱ���жϻص���������ֵתΪ�ٶ�ֵ
void OnTimerHandlerForMotorEncoders(uint16_t ms);
// ��ȡ�����ٶȣ�Ȧ/�룩
int16_t GetLeftMotorSpeed(void);
// ��ȡ�ҵ���ٶȣ�Ȧ/�룩
int16_t GetRightMotorSpeed(void);

#endif
