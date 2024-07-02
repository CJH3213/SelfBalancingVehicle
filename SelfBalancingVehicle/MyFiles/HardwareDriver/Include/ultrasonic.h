/**
  ******************************************************************************
  * @file           : ultrasonic.h
  * @brief          : 超声波测距
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H
/*----------------------------------------------------------------------------*/
/*头文件               																									   */
/*----------------------------------------------------------------------------*/
#include "main.h"
#include "CD4051.h"

/*----------------------------------------------------------------------------*/
/*接口定义             																									   */
/*----------------------------------------------------------------------------*/
//#define INC_Clr() GPIOA->BRR = GPIO_PIN_11
//#define INC_Set() GPIOA->BRR = GPIO_PIN_11
#define INC_Clr() CD4051_OutPut(TG_INC, 0)
#define INC_Set() CD4051_OutPut(TG_INC, 1)
//接收是PA12

/*----------------------------------------------------------------------------*/
/*常变量声明              																									   */
/*----------------------------------------------------------------------------*/
extern uint8_t Ultrasonic_State;		//超声波执行状态：未开始，正在进行，接收完成
extern uint32_t Ultrasonic_Num;		//中断计时
extern float Ultrasonic_Value;		//转换后的距离值
extern uint16_t dis;		//整数距离值

//#define UltraFree 0u
//#define UltraWorking 1u
//#define UltraFinish 2u
//#define UltraError 3u
enum FinishFlagType{UltraFree=0,UltraWorking,UltraFinish,UltraError};

/*----------------------------------------------------------------------------*/
/*函数声明                																								   */
/*----------------------------------------------------------------------------*/
void Ultransonic_Init(void);	//初始化
void Ultrasonic_Ranging(void);	//超声波测距
void Ultrasonic_transValue(void);//转换为距离值
void Ultrasonic_STOP(void);//停止&复位

#endif
