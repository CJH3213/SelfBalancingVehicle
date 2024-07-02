/**
  ******************************************************************************
  * @file           : turnLight.c
  * @brief          : 转向灯
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

#define turnLeft_LED(level) 	CD4051_OutPut(TG_LED_R, !level)	//左转向灯1开，0关
#define turnRight_LED(level) 	CD4051_OutPut(TG_LED_L, level)	//右转向灯1开，0关

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
			//无法实现左转灯亮右转灯灭，CD4051将左转灯选通后，右转灯输入会上拉，右转灯也会亮
				turnLeft_LED(1);
			break;
		case turnLED_turnRight:
				turnRight_LED(1);
			break;
	}
}

//测试转向灯
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
