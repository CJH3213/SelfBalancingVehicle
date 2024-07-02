/**
  ******************************************************************************
  * @file           : HC595.c
  * @brief          : ���������оƬ
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
		(Sdata>>7)? (HC595_SER_Set()) : (HC595_SER_Clr());	//��8bit��ֳɴ������ݣ���λ��ǰ
		Sdata <<= 1;

		delay_us(1);
		HC595_SCK_Set();		//������
		delay_us(1);
		HC595_SCK_Clr();
	}
	
	HC595_RCK_Set();	//�����أ���λ�ƼĴ�������ת���洢�Ĵ���
	delay_us(1);
	HC595_RCK_Clr();
}

//������������ж�λ
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

//����00~99��ֵ
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

