/**
  ******************************************************************************
  * @file           : Page.c
  * @brief          : 处理页面
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Page.h"
#include "stdio.h"
#include "my_IIC.h"
#include "oled.h"
#include "delay.h"
#include "key.h"
#include "HC595.h"
#include "infrared.h"
#include "BH1750.h"
#include "usart.h"
#include "CD4051.h"
#include "beep.h"
#include "turnLight.h"
#include "ultrasonic.h"
#include "syn7318.h"

uint8_t aTxStartMessage[] = "rn UART-communication based on IT rn";

uint8_t count = 0;
uint8_t menu = 0;
float Value;
char * FuntionItems[] = 
{
	"turnLight",
	"Digital tube",
	"Beep",
	"Ultrasonic",
	"RI",
	"SYN7318"
};

void page_Welcome(void)
{
//	beep(1);
	turnLight(turnLED_ALLOFF);

	GPIOC->BSRR = GPIO_PIN_13;
	KeyClear(&KEY_NEXT);
	KeyClear(&KEY_ENTER);
	
	OLED_ShowCHinese(0*16, 0, 0);			//显示“任务板测试器”
	OLED_ShowCHinese(1*16, 0, 1);
	OLED_ShowCHinese(2*16, 0, 2);
	OLED_ShowCHinese(3*16, 0, 3);
	OLED_ShowCHinese(4*16, 0, 4);
	OLED_ShowCHinese(5*16, 0, 5);
	OLED_ShowString(4*16,2,"BY:CJH",16);
	delay_ms(400);
	OLED_Clear();			//800ms后清屏
	
	//IIC挂载检测
	OLED_ShowString(0*8,0,(IIC_CheckDevice(IICSLADDR_OLED,IIC1)==bTrue)?"OLED:Found":"OLED:NotFound",16);
//	OLED_ShowString(0*8,2,(IIC_CheckDevice(IICSLADDR_BH1750,IIC2)==bTrue)?"BH1750:Found":"BH1750:NotFound",16);
	OLED_ShowString(0*8,2,(IIC_CheckDevice(IICSLADDR_BH1750,IIC2)==bTrue)?"BH1750:Found":"BH1750:NotFound",16);
	
	delay_ms(800);
	OLED_Clear();			//800ms后清屏



//		SYN_TTS((uint8_t *)"请发语音唤醒词，语音驾驶");
//	Status_Query();  //查询模块当前的工作状态
//	SYN7318_Test();	
//	OLED_ShowNum_HEX(0*8, 0, S[0], 2, 16);
//	OLED_ShowNum_HEX(3*8, 0, S[1], 2, 16);
//	OLED_ShowNum_HEX(6*8, 0, S[2], 2, 16);
//	OLED_ShowNum_HEX(9*8, 0, S[3], 2, 16);
//	OLED_ShowNum_HEX(12*8, 0, S[1], 2, 16);	

//	OLED_ShowNum_HEX(0*8, 2, Back[0], 2, 16);
//	OLED_ShowNum_HEX(3*8, 2, Back[1], 2, 16);
//	OLED_ShowNum_HEX(6*8, 2, Back[2], 2, 16);
//	OLED_ShowNum_HEX(9*8, 2, Back[3], 2, 16);
//	OLED_ShowNum_HEX(12*8, 2, Back[1], 2, 16);

//	OLED_ShowNum_HEX(12*8, 2, ASR[0], 2, 16);
//	beep(0);
//	HAL_UART_Transmit(&huart1, aTxStartMessage, 1, 0xFFFF);

//	turnLight_Test();
//	SYN7318_Rst();  //语音模块复位
//	SYN_SetWakeUPName("语音驾驶");
//	SYN_SetParameter(near, 5000, 5000, 0x02);		//设置语音模块参数
	OLED_ShowString(0*8,0,FuntionItems[menu],16);			
}


//相当于main()函数
void page_Chose(void)
{
	
	if(KEY_NEXT.KEY_DOWN)
	{
		if(KEY_NEXT.KEY_UP)	//按键释放时处理按键事件
		{
			if((++menu)>5)	menu = 0;
			OLED_Clear();			//清屏
			OLED_ShowString(0*8,0,FuntionItems[menu],16);
		KeyClear(&KEY_NEXT);	//复位按键所有标志位			
		}
	}

	switch(menu)
	{
		case 0://转向灯测试
			if(KEY_ENTER.KEY_UP)
			{
				count = (++count>3)?0:count;
				OLED_ShowNum(0*8, 2, count, 1, 16);
				KeyClear(&KEY_ENTER);
			}
			turnLight((enum turnStateType)count);		
			break;
		case 1://数码管测试
			ShowNumber(count++, 10);		
			break;
		case 2://蜂鸣器测试
			beep(1);		
			break;
		case 3:		//超声波测距测试
			if(KEY_ENTER.KEY_UP)
			{
				Ultrasonic_STOP();		//停止并复位超声波
				KeyClear(&KEY_ENTER);
			}
		
			if(Ultrasonic_State == UltraFree)		//如果超声波空闲
			{
				Ultrasonic_State = UltraWorking;		//标记超声波正在工作
				Ultrasonic_Ranging();		//开启超声波
			}
			if(Ultrasonic_State == UltraFinish)	//接收完成后显示出来，并标记超声波空闲
			{
				OLED_ShowNum(0*8, 2, Ultrasonic_Num, 5, 16);		
				Value = Ultrasonic_Value/10.0;
					OLED_ShowNum(5*8, 2, (uint32_t)Value, 5, 16);
					OLED_ShowChar(10*8, 2, '.', 16);
					OLED_ShowChar(11*8, 2, (uint32_t)(Value*10)%10 +'0', 16);
					OLED_ShowChar(12*8, 2, (uint32_t)(Value*100)%10 +'0', 16);
					OLED_ShowString(14*8,2,"cm",16);
				Ultrasonic_State = UltraFree;
			}
			break;
		case 4:
			Infrared_Send(HW_K,6);		//开启红外报警
			delay_ms(500);
			Infrared_Send(H_1,4);		//光源档位加1
			delay_ms(500);
			break;
		case 5:
			if(KEY_ENTER.KEY_UP)
			{
				Infrared_Send(HW_K,6);		//开启红外报警
				KeyClear(&KEY_ENTER);
			}
//			SYN7318_Test();
			break;
		default:
			OLED_ShowString(0*8,0,"menu>5",16);
			break;
	}
}

/**

  * @brief Rx Transfer completed callbacks

  * @param huart: uart handle

  * @retval None

  */
