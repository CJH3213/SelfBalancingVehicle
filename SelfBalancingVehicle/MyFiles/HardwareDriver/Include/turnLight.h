/**
  ******************************************************************************
  * @file           : turnLight.h
  * @brief          : ת���
  ******************************************************************************
  **
	*
  *
  ******************************************************************************
  */
#ifndef __TURNLIGHT_H
#define __TURNLIGHT_H
/*----------------------------------------------------------------------------*/
/*ͷ�ļ�               																									   */
/*----------------------------------------------------------------------------*/
#include "main.h"

/*----------------------------------------------------------------------------*/
/*�ӿڶ���             																									   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*����������              																									   */
/*----------------------------------------------------------------------------*/
//ת���״̬����
enum turnStateType
{
	turnLED_ALLOFF,		//ȫ��
	turnLED_turnLeft,	//��ת��
	turnLED_turnRight,//��ת��
	turnLED_ALLON			//ȫ��
};

/*----------------------------------------------------------------------------*/
/*��������                																								   */
/*----------------------------------------------------------------------------*/
void turnLight(enum turnStateType turnState);
void turnLight_Test(void);	//����ת���

#endif
