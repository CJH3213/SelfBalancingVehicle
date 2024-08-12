/**
  ******************************************************************************
  * @file           : KEY.c
  * @brief          : ��������
  ******************************************************************************
  **
	*�ð������������ڶ�ʱ���ڽ��д���
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Key.h"

KEY Key1, Key2, Key3, Key4;		//��ÿ����������һ��KEY״̬�ṹ����

//��λ������־λ
void KeyClear(KEY *KEYstr)
{
	KEYstr->KeepLong = 0;
	KEYstr->KEY_DOWN = 0;
	KEYstr->KEY_LONG = 0;
}

void KeyHandlers(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY *KEYstr)
{
	//��¼��������ʱ��
	if((GPIOx->IDR & GPIO_Pin) == 0)	//���GPIOΪ0
	{
		if(KEYstr->KeepLong >= 20 && KEYstr->KeepLong < KEY_LONGVALUE)	// ��������20msȷ��Ϊ��������
			KEYstr->KEY_DOWN = 1;
		if(KEYstr->KeepLong >= KEY_LONGVALUE)		// ��������1000ms�󳤰���λ
			KEYstr->KEY_LONG = 1;
		
		if(KEYstr->KeepLong < 6000)	//���6000ms���ټ�ʱ
			KEYstr->KeepLong += KEY_TIM_PERIOD;
	}
	else
	{
		KEYstr->KeepLong = 0;//�����ͷŹ��� 
		KEYstr->KEY_DOWN = 0;
		KEYstr->KEY_LONG = 0;
	}
}

void InitAllKeys()
{
	KeyClear(&Key1);
	KeyClear(&Key2);
	KeyClear(&Key3);
	KeyClear(&Key4);
}

void OnTIMForKeysHandler()
{
	KeyHandlers(KEY1_GPIO_Port, KEY1_Pin, &Key1);
	KeyHandlers(KEY2_GPIO_Port, KEY2_Pin, &Key2);
	KeyHandlers(KEY3_GPIO_Port, KEY3_Pin, &Key3);
	KeyHandlers(KEY4_GPIO_Port, KEY4_Pin, &Key4);
}
