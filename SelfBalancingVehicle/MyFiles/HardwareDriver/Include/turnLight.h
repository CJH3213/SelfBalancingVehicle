/**
  ******************************************************************************
  * @file           : turnLight.h
  * @brief          : 转向灯
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __TURNLIGHT_H
#define __TURNLIGHT_H
/*----------------------------------------------------------------------------*/
/*头文件               																									   */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*接口定义             																									   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*常变量声明              																									   */
/*----------------------------------------------------------------------------*/
//转向灯状态控制
enum turnStateType
{
	turnLED_ALLOFF,		//全关
	turnLED_turnLeft,	//左转灯
	turnLED_turnRight,//右转灯
	turnLED_ALLON			//全开
};

/*----------------------------------------------------------------------------*/
/*函数声明                																								   */
/*----------------------------------------------------------------------------*/
void turnLight(enum turnStateType turnState);
void turnLight_Test(void);	//测试转向灯

#endif

