/**
  ******************************************************************************
  * @file           : KEY.c
  * @brief          : 按键处理
  ******************************************************************************
  **
	*该按键处理函数放在定时器内进行处理
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "Key.h"

KEY Key1, Key2, Key3, Key4;		//给每个按键赋予一个KEY状态结构对象

//复位按键标志位
void KeyClear(KEY *KEYstr)
{
	KEYstr->KeepLong = 0;
	KEYstr->KEY_DOWN = 0;
	KEYstr->KEY_LONG = 0;
}

void KeyHandlers(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY *KEYstr)
{
	//记录按键按下时长
	if((GPIOx->IDR & GPIO_Pin) == 0)	//如果GPIO为0
	{
		if(KEYstr->KeepLong >= 20 && KEYstr->KeepLong < KEY_LONGVALUE)	// 持续按下20ms确认为按键按下
			KEYstr->KEY_DOWN = 1;
		if(KEYstr->KeepLong >= KEY_LONGVALUE)		// 持续按下1000ms后长按置位
			KEYstr->KEY_LONG = 1;
		
		if(KEYstr->KeepLong < 6000)	//最大6000ms后不再计时
			KEYstr->KeepLong += KEY_TIM_PERIOD;
	}
	else
	{
		KEYstr->KeepLong = 0;//按键释放归零 
		KEYstr->KEY_DOWN = 0;
		KEYstr->KEY_LONG = 0;
	}
}

void InitAllKeys()
{
	KeyClear(&Key1);
	KeyClear(&Key2);
	KeyClear(&Key3);
	KeyClear(&Key4);
}

void OnTIMForKeysHandler()
{
	KeyHandlers(KEY1_GPIO_Port, KEY1_Pin, &Key1);
	KeyHandlers(KEY2_GPIO_Port, KEY2_Pin, &Key2);
	KeyHandlers(KEY3_GPIO_Port, KEY3_Pin, &Key3);
	KeyHandlers(KEY4_GPIO_Port, KEY4_Pin, &Key4);
}
