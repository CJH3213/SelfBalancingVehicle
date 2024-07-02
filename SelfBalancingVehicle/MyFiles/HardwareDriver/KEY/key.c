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
#include "key.h"

KEY KEY_NEXT,KEY_ENTER;		//给每个按键赋予一个KEY状态结构体

//复位按键标志位
void KeyClear(KEY *KEYstr)
{
	KEYstr->KeepLong = 0;
	KEYstr->KEY_DOWN = 0;
	KEYstr->KEY_LONG = 0;
	KEYstr->KEY_UP = 0;	
	KEYstr->KEY_State = 0;
}

void KeyHandlers(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, KEY *KEYstr)
{
	//记录按键按下时长
	if((GPIOx->IDR & GPIO_Pin) == (uint32_t)GPIO_PIN_RESET)	//如果GPIO为0
	{
		if(KEYstr->KeepLong < 6000)//最大6000*10ms后不再计时
			KEYstr->KeepLong ++;
	}
	else
		KEYstr->KeepLong = 0;//按键释放归零 

	
	//对按键状态进行判断
	switch(KEYstr->KEY_State)
	{
		//首次按下判断
		case 0:
			if(KEYstr->KeepLong && !(KEYstr->KEY_DOWN))
				KEYstr->KEY_State = 1;		//下次处理是去抖后判断
			break;
		//去抖后按下判断
		case 1:
			if(KEYstr->KeepLong)
			{
				KEYstr->KEY_DOWN = 1;		//标记按键刚被按下
				KEYstr->KEY_UP = 0;
				KEYstr->KEY_LONG =0;
				KEYstr->KEY_State = 2;		//下次处理是判断长按
			}
			else
				KEYstr->KEY_State = 0;//如果是误判
			break;
		//长按判断
		case 2:
			if(!(KEYstr->KeepLong))		//如果按键被释放了，跳转到按键释放处理
				KEYstr->KEY_State = 3;
			else
				if(KEYstr->KeepLong >= KEY_LONGVALUE)		//达到长按时长
				{
					KEYstr->KEY_LONG = 1;		//标记长按
					KEYstr->KEY_State = 3;	//跳转去等待按键释放
				}
			break;
		//按键释放处理
		case 3:
			if(!(KEYstr->KeepLong))
			{
				KEYstr->KEY_UP = 1;			//标记按键被释放
				KEYstr->KEY_State = 0;
			}
			break;
	}
}
