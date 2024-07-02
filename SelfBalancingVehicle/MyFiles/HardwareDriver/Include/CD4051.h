/**
  ******************************************************************************
  * @file           : CD4051.h
  * @brief          : 8位译码开关
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __CD4051_H
#define __CD4051_H
/*----------------------------------------------------------------------------*/
/*头文件               																									   */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*接口定义             																									   */
/*----------------------------------------------------------------------------*/
#define CD4051_COM_Clr() GPIOA->BRR = GPIO_PIN_8 
#define CD4051_COM_Set() GPIOA->BSRR = GPIO_PIN_8

#define CD4051_C_Clr() GPIOB->BRR = GPIO_PIN_15
#define CD4051_C_Set() GPIOB->BSRR = GPIO_PIN_15

#define CD4051_B_Clr() GPIOB->BRR = GPIO_PIN_14
#define CD4051_B_Set() GPIOB->BSRR = GPIO_PIN_14

#define CD4051_A_Clr() GPIOB->BRR = GPIO_PIN_13
#define CD4051_A_Set() GPIOB->BSRR = GPIO_PIN_13

/*----------------------------------------------------------------------------*/
/*常变量声明              																									   */
/*----------------------------------------------------------------------------*/
#define TG_SMG_A 	0
#define TG_SMG_B 	1
#define TG_INC 		2
#define TG_RI_TXD	3
#define TG_LED_L 	4
#define TG_LED_R 	5
#define TG_BEEP 	6

/*----------------------------------------------------------------------------*/
/*函数声明                																								   */
/*----------------------------------------------------------------------------*/
void CD4051_OutPut(uint8_t port, uint8_t level);

#endif
