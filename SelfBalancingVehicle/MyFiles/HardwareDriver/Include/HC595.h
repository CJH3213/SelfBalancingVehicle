/**
  ******************************************************************************
  * @file           : HC595.h
  * @brief          : ���������оƬ
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __HC595_H
#define __HC595_H
/*----------------------------------------------------------------------------*/
/*ͷ�ļ�               																									     */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*�ӿڶ���               																									   */
/*----------------------------------------------------------------------------*/
#define HC595_SER_Clr() GPIOB->BRR 	= GPIO_PIN_3//SER ��������
#define HC595_SER_Set() GPIOB->BSRR = GPIO_PIN_3

#define HC595_SCK_Clr() GPIOB->BRR 	= GPIO_PIN_4//SCK ��λ�Ĵ���ʱ��
#define HC595_SCK_Set() GPIOB->BSRR = GPIO_PIN_4

#define HC595_RCK_Clr() GPIOB->BRR 	= GPIO_PIN_5//RCK �洢�Ĵ���ʱ��
#define HC595_RCK_Set() GPIOB->BSRR = GPIO_PIN_5

/*----------------------------------------------------------------------------*/
/*����������               																									   */
/*----------------------------------------------------------------------------*/
extern const uint8_t FNumber[];

/*----------------------------------------------------------------------------*/
/*��������                																								   */
/*----------------------------------------------------------------------------*/
void send_HC595(uint8_t data);		//�������ݵ�HC595
void test_HC595(uint16_t longTime);		//������������ж�λ
void ShowNumber(uint8_t Num, uint16_t delay);//����00~99��ֵ

#endif
