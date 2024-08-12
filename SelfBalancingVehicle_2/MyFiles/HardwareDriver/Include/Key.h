/**
  ******************************************************************************
  * @file           : KEY.h
  * @brief          : ��������
  ******************************************************************************
  **
	*�ð��������������ڶ�ʱ���ڽ��д���
  *
  ******************************************************************************
  */
#ifndef __KEY_H
#define __KEY_H
/*----------------------------------------------------------------------------**
**ͷ�ļ�               																									  		
**----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------**
**���� �� �� ��              																									
**----------------------------------------------------------------------------*/
#define KEY_TIM_PERIOD	1		//�����������ڶ�ʱ�����ж����ڣ��������ڣ�
#define KEY_LONGVALUE (600/KEY_TIM_PERIOD)	//����ʱ���ж��㣬600ms

typedef struct
{
	uint16_t KeepLong;		//������ͨʱ��
	uint8_t  KEY_DOWN;		//����������
	uint8_t  KEY_LONG;		//����������
}KEY;

extern KEY Key1, Key2, Key3, Key4;		//��������ÿ����������һ��KEY״̬�ṹ��

/*----------------------------------------------------------------------------**
**��������                																								 		
**----------------------------------------------------------------------------*/
void KeyClear(KEY *KEYstr);			//��λ������־λ
void InitAllKeys(void);		// ��λ���а�ť��־
void OnTIMForKeysHandler(void);			// ��ʱ���ڴ���

#endif