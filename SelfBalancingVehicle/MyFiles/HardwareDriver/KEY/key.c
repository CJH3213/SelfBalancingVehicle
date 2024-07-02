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
#include "key.h"

KEY KEY_NEXT,KEY_ENTER;		//��ÿ����������һ��KEY״̬�ṹ��

//��λ������־λ
void KeyClear(KEY *KEYstr)
{
	KEYstr->KeepLong = 0;
	KEYstr->KEY_DOWN = 0;
	KEYstr->KEY_LONG = 0;
	KEYstr->KEY_UP = 0;	
	KEYstr->KEY_State = 0;
}

void KeyHandlers(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY *KEYstr)
{
	//��¼��������ʱ��
	if((GPIOx->IDR & GPIO_Pin) == (uint32_t)GPIO_PIN_RESET)	//���GPIOΪ0
	{
		if(KEYstr->KeepLong < 6000)//���6000*10ms���ټ�ʱ
			KEYstr->KeepLong ++;
	}
	else
		KEYstr->KeepLong = 0;//�����ͷŹ��� 

	
	//�԰���״̬�����ж�
	switch(KEYstr->KEY_State)
	{
		//�״ΰ����ж�
		case 0:
			if(KEYstr->KeepLong && !(KEYstr->KEY_DOWN))
				KEYstr->KEY_State = 1;		//�´δ�����ȥ�����ж�
			break;
		//ȥ�������ж�
		case 1:
			if(KEYstr->KeepLong)
			{
				KEYstr->KEY_DOWN = 1;		//��ǰ����ձ�����
				KEYstr->KEY_UP = 0;
				KEYstr->KEY_LONG =0;
				KEYstr->KEY_State = 2;		//�´δ������жϳ���
			}
			else
				KEYstr->KEY_State = 0;//���������
			break;
		//�����ж�
		case 2:
			if(!(KEYstr->KeepLong))		//����������ͷ��ˣ���ת�������ͷŴ���
				KEYstr->KEY_State = 3;
			else
				if(KEYstr->KeepLong >= KEY_LONGVALUE)		//�ﵽ����ʱ��
				{
					KEYstr->KEY_LONG = 1;		//��ǳ���
					KEYstr->KEY_State = 3;	//��תȥ�ȴ������ͷ�
				}
			break;
		//�����ͷŴ���
		case 3:
			if(!(KEYstr->KeepLong))
			{
				KEYstr->KEY_UP = 1;			//��ǰ������ͷ�
				KEYstr->KEY_State = 0;
			}
			break;
	}
}
