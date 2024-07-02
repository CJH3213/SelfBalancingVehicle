/**
  ******************************************************************************
  * @file           : ultrasonic.c
  * @brief          : ���������
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ultrasonic.h"
#include "delay.h"
#include "tim.h"

////��ʼ��
//void Ultransonic_Init(void)
//{
//	INC_Set();
//}

uint8_t Ultrasonic_State = UltraFree;		//������ִ��״̬��δ��ʼ�����ڽ��У��������
uint32_t Ultrasonic_Num = 0;		//��¼���������͵�����ʱ��
float Ultrasonic_Value = 0;		//ת����ľ���ֵ
uint16_t dis =0;		//��������ֵ

//���������
void Ultrasonic_Ranging(void)
{
	
	INC_Set();
	delay_us(3);	
	//������
		Ultrasonic_Num = 0;	
		HAL_TIM_Base_Start_IT(&htim3);			//����3�Ŷ�ʱ��
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);		//���TIM3��ʱ��
	//�ⲿ�жϿ���
		//		HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);		//����ⲿ�ж�������ٴ��ⲿ�ж�
		__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI15_10_IRQn);
		__HAL_GPIO_EXTI_CLEAR_IT(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	INC_Clr();		//���ͳ�����10ms
	delay_ms(30);
	INC_Set();
	delay_ms(30);
}

//ת��Ϊ����ֵ
void Ultrasonic_transValue(void)
{
	Ultrasonic_Value = Ultrasonic_Num;
	Ultrasonic_Value =(float)Ultrasonic_Value*1.72-20;       // ������붨ʱ10us��S=Vt/2����2��������
	dis = (uint16_t) Ultrasonic_Value;
}

//ֹͣ&��λ
void Ultrasonic_STOP(void)
{
	//������
		Ultrasonic_Num = 0;
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);		//���TIM3��ʱ��
		HAL_TIM_Base_Stop_IT(&htim3);			//ֹͣTIM3��ʱ���ж�
	//�ⲿ�ж�
		__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI15_10_IRQn);
		__HAL_GPIO_EXTI_CLEAR_IT(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	//��־λ
	Ultrasonic_State = UltraFree;				//��λ���׼����һ�β���
}
