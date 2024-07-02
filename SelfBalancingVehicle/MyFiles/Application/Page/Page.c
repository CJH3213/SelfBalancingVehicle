/**
  ******************************************************************************
  * @file           : Page.c
  * @brief          : ����ҳ��
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
	
	OLED_ShowCHinese(0*16, 0, 0);			//��ʾ��������������
	OLED_ShowCHinese(1*16, 0, 1);
	OLED_ShowCHinese(2*16, 0, 2);
	OLED_ShowCHinese(3*16, 0, 3);
	OLED_ShowCHinese(4*16, 0, 4);
	OLED_ShowCHinese(5*16, 0, 5);
	OLED_ShowString(4*16,2,"BY:CJH",16);
	delay_ms(400);
	OLED_Clear();			//800ms������
	
	//IIC���ؼ��
	OLED_ShowString(0*8,0,(IIC_CheckDevice(IICSLADDR_OLED,IIC1)==bTrue)?"OLED:Found":"OLED:NotFound",16);
//	OLED_ShowString(0*8,2,(IIC_CheckDevice(IICSLADDR_BH1750,IIC2)==bTrue)?"BH1750:Found":"BH1750:NotFound",16);
	OLED_ShowString(0*8,2,(IIC_CheckDevice(IICSLADDR_BH1750,IIC2)==bTrue)?"BH1750:Found":"BH1750:NotFound",16);
	
	delay_ms(800);
	OLED_Clear();			//800ms������



//		SYN_TTS((uint8_t *)"�뷢�������Ѵʣ�������ʻ");
//	Status_Query();  //��ѯģ�鵱ǰ�Ĺ���״̬
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
//	SYN7318_Rst();  //����ģ�鸴λ
//	SYN_SetWakeUPName("������ʻ");
//	SYN_SetParameter(near, 5000, 5000, 0x02);		//��������ģ�����
	OLED_ShowString(0*8,0,FuntionItems[menu],16);			
}


//�൱��main()����
void page_Chose(void)
{
	
	if(KEY_NEXT.KEY_DOWN)
	{
		if(KEY_NEXT.KEY_UP)	//�����ͷ�ʱ�������¼�
		{
			if((++menu)>5)	menu = 0;
			OLED_Clear();			//����
			OLED_ShowString(0*8,0,FuntionItems[menu],16);
		KeyClear(&KEY_NEXT);	//��λ�������б�־λ			
		}
	}

	switch(menu)
	{
		case 0://ת��Ʋ���
			if(KEY_ENTER.KEY_UP)
			{
				count = (++count>3)?0:count;
				OLED_ShowNum(0*8, 2, count, 1, 16);
				KeyClear(&KEY_ENTER);
			}
			turnLight((enum turnStateType)count);		
			break;
		case 1://����ܲ���
			ShowNumber(count++, 10);		
			break;
		case 2://����������
			beep(1);		
			break;
		case 3:		//������������
			if(KEY_ENTER.KEY_UP)
			{
				Ultrasonic_STOP();		//ֹͣ����λ������
				KeyClear(&KEY_ENTER);
			}
		
			if(Ultrasonic_State == UltraFree)		//�������������
			{
				Ultrasonic_State = UltraWorking;		//��ǳ��������ڹ���
				Ultrasonic_Ranging();		//����������
			}
			if(Ultrasonic_State == UltraFinish)	//������ɺ���ʾ����������ǳ���������
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
			Infrared_Send(HW_K,6);		//�������ⱨ��
			delay_ms(500);
			Infrared_Send(H_1,4);		//��Դ��λ��1
			delay_ms(500);
			break;
		case 5:
			if(KEY_ENTER.KEY_UP)
			{
				Infrared_Send(HW_K,6);		//�������ⱨ��
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
