#ifndef __MOTORS_H
#define __MOTORS_H

#include "main.h"


/**** ����������� ****/

// ���ռ�ձȼ���ʱ����������ֵ��1000
#define MOTOR_PWM_HTIM htim1
#define MOTOR_PWM_MAX ((int16_t)MOTOR_PWM_HTIM.Init.Period)

// ��ʼ��������������Ӳ��
void InitMotorDriver(void);

// ����ɲ��
void BrakingLeftMotor(void);
// ��������������ٶ�
void SetLeftMotor(int16_t speed);
// �ҵ��ɲ��
void BrakingRightMotor(void);
// �����ҵ��������ٶ�
void SetRightMotor(int16_t speed);


/**** ���������ٲ��� ****/
#define MOTOR_ENCODERS_TIM_PERIOD 1	// �������������ڶ�ʱ�����ж����ڣ�1ms

// ��ʼ��������������Ӳ��
void InitMotorEncodersDriver(void);
// ��ʱ���жϻص���������ֵתΪ�ٶ�ֵ
void OnTIMForMotorEncoders(void);
// ��ȡ�����ٶȣ�Ȧ/�룩
int16_t GetLeftMotorSpeed(void);
// ��ȡ�ҵ���ٶȣ�Ȧ/�룩
int16_t GetRightMotorSpeed(void);

#endif
