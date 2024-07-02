/**
  ******************************************************************************
  * @file           : HC595.h
  * @brief          : 数码管驱动芯片
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __HC595_H
#define __HC595_H
/*----------------------------------------------------------------------------*/
/*头文件               																									     */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*接口定义               																									   */
/*----------------------------------------------------------------------------*/
#define HC595_SER_Clr() GPIOB->BRR 	= GPIO_PIN_3//SER 串行数据
#define HC595_SER_Set() GPIOB->BSRR = GPIO_PIN_3

#define HC595_SCK_Clr() GPIOB->BRR 	= GPIO_PIN_4//SCK 移位寄存器时钟
#define HC595_SCK_Set() GPIOB->BSRR = GPIO_PIN_4

#define HC595_RCK_Clr() GPIOB->BRR 	= GPIO_PIN_5//RCK 存储寄存器时钟
#define HC595_RCK_Set() GPIOB->BSRR = GPIO_PIN_5

/*----------------------------------------------------------------------------*/
/*常变量声明               																									   */
/*----------------------------------------------------------------------------*/
extern const uint8_t FNumber[];

/*----------------------------------------------------------------------------*/
/*函数声明                																								   */
/*----------------------------------------------------------------------------*/
void send_HC595(uint8_t data);		//发送数据到HC595
void test_HC595(uint16_t longTime);		//测试数码管所有段位
void ShowNumber(uint8_t Num, uint16_t delay);//输入00~99数值

#endif

