#ifndef __BATTERY_H
#define __BATTERY_H

#include "main.h"

#define BATTERY_TIM_PERIOD	1		//�����������ڶ�ʱ�����ж����ڣ��������ڣ�

// ���û����ã���HAL_ADC_ConvCpltCallback()����
void OnADCConversionCompleteForBattery(void);
// ���û����ã���ms��ʱ������
void OnTIMForBatteryMonitoring(void);
// ��ʼ��
void InitBatteryMonitoring(void);
// ��ʼһ�ε�ѹ����
//void StartSingleBatteryADCConvert(void);
// ��ȡ��ص�ѹ������
uint16_t GetBatteryMillivolt(void);

#endif
