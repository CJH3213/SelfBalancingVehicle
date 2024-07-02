/**
  ******************************************************************************
  * @file           : ultrasonic.c
  * @brief          : 超声波测距
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

////初始化
//void Ultransonic_Init(void)
//{
//	INC_Set();
//}

uint8_t Ultrasonic_State = UltraFree;		//超声波执行状态：未开始，正在进行，接收完成
uint32_t Ultrasonic_Num = 0;		//记录超声波发送到接收时长
float Ultrasonic_Value = 0;		//转换后的距离值
uint16_t dis =0;		//整数距离值

//超声波测距
void Ultrasonic_Ranging(void)
{
	
	INC_Set();
	delay_us(3);	
	//计数器
		Ultrasonic_Num = 0;	
		HAL_TIM_Base_Start_IT(&htim3);			//开启3号定时器
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);		//清除TIM3定时器
	//外部中断开启
		//		HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);		//清除外部中断请求后再打开外部中断
		__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI15_10_IRQn);
		__HAL_GPIO_EXTI_CLEAR_IT(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	INC_Clr();		//发送超声波10ms
	delay_ms(30);
	INC_Set();
	delay_ms(30);
}

//转换为距离值
void Ultrasonic_transValue(void)
{
	Ultrasonic_Value = Ultrasonic_Num;
	Ultrasonic_Value =(float)Ultrasonic_Value*1.72-20;       // 计算距离定时10us，S=Vt/2（减2是误差补尝）
	dis = (uint16_t) Ultrasonic_Value;
}

//停止&复位
void Ultrasonic_STOP(void)
{
	//计数器
		Ultrasonic_Num = 0;
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC1);		//清除TIM3定时器
		HAL_TIM_Base_Stop_IT(&htim3);			//停止TIM3定时器中断
	//外部中断
		__HAL_GPIO_EXTI_CLEAR_FLAG(EXTI15_10_IRQn);
		__HAL_GPIO_EXTI_CLEAR_IT(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	//标志位
	Ultrasonic_State = UltraFree;				//复位后可准备下一次测量
}
