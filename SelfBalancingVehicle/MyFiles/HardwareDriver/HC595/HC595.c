/**
  ******************************************************************************
  * @file           : HC595.c
  * @brief          : 数码管驱动芯片
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "HC595.h"
#include "delay.h"
#include "CD4051.h"

const uint8_t FNumber[] = 
{0x11,0x9f,0x52,0x1a,0x9c,0x38,0x30,0x1f,0x10,0x18};

//Send to HC595
void send_HC595(uint8_t Sdata)
{
	HC595_SCK_Clr();
	HC595_RCK_Clr();
	for(uint8_t i=0; i<8; i++)
	{
		(Sdata>>7)? (HC595_SER_Set()) : (HC595_SER_Clr());	//将8bit拆分成串行数据，高位在前
		Sdata <<= 1;

		delay_us(1);
		HC595_SCK_Set();		//上升沿
		delay_us(1);
		HC595_SCK_Clr();
	}
	
	HC595_RCK_Set();	//上升沿，将位移寄存器数据转到存储寄存器
	delay_us(1);
	HC595_RCK_Clr();
}

//测试数码管所有段位
void test_HC595(uint16_t longTime)
{
	uint8_t i=0xff;
	for(uint8_t x=0; x<9; x++)
	{
		send_HC595(i);
		i >>= 1;
		delay_ms(longTime);
	}
}

//输入00~99数值
void ShowNumber(uint8_t Num, uint16_t delay)
{
	
	if(Num<100)
	{
		CD4051_OutPut(TG_SMG_A,1);
		send_HC595(*(FNumber+(Num/10%10)));
		delay_ms(delay);
		CD4051_OutPut(TG_SMG_A,0);

		CD4051_OutPut(TG_SMG_B,1);
		send_HC595(*(FNumber+Num%10));
		delay_ms(delay);		
		CD4051_OutPut(TG_SMG_B,0);
	}
	else
	{
		CD4051_OutPut(TG_SMG_A,1);
		send_HC595(0x64);
		delay_ms(delay);
		CD4051_OutPut(TG_SMG_A,0);

		CD4051_OutPut(TG_SMG_B,1);
		send_HC595(0x64);		
		delay_ms(delay);		
		CD4051_OutPut(TG_SMG_B,0);
	}
}

