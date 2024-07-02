/**
  ******************************************************************************
  * @file           : Page.h
  * @brief          : 处理页面
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __PAGE_H
#define __PAGE_H
/*----------------------------------------------------------------------------*/
/*头文件               																									   */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*常变量声明              																									   */
/*----------------------------------------------------------------------------*/
extern uint8_t aTxStartMessage[];
//extern uint8_t aRxBuffer[20];

/*----------------------------------------------------------------------------*/
/*函数声明                																								   */
/*----------------------------------------------------------------------------*/
void page_Welcome(void);
void page_Chose(void);

#endif
