/**
  ******************************************************************************
  * @file           : turnLight.c
  * @brief          : ת���
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "turnLight.h"
#include "CD4051.h"
#include "delay.h"

#define turnLeft_LED(level) 	CD4051_OutPut(TG_LED_R, !level)	//��ת���1����0��
#define turnRight_LED(level) 	CD4051_OutPut(TG_LED_L, level)	//��ת���1����0��

void turnLight(enum turnStateType turnState)
{
	switch((uint16_t)turnState)
	{
		case turnLED_ALLOFF:
				turnRight_LED(0);
			break;
		case turnLED_ALLON:
				turnLeft_LED(1);
			break;
		case turnLED_turnLeft:
			//�޷�ʵ����ת������ת����CD4051����ת��ѡͨ����ת���������������ת��Ҳ����
				turnLeft_LED(1);
			break;
		case turnLED_turnRight:
				turnRight_LED(1);
			break;
	}
}

//����ת���
void turnLight_Test(void)
{
	turnLight(turnLED_ALLON);
	delay_ms(800);
	turnLight(turnLED_turnRight);
	delay_ms(800);
	turnLight(turnLED_turnLeft);
	delay_ms(800);
	turnLight(turnLED_ALLOFF);
	delay_ms(800);
}
