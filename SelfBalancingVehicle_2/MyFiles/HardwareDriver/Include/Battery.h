#ifndef __BATTERY_H
#define __BATTERY_H

#include "main.h"

#define BATTERY_TIM_PERIOD	1		//按键处理所在定时器的中断周期（处理周期）

// 非用户调用：在HAL_ADC_ConvCpltCallback()调用
void OnADCConversionCompleteForBattery(void);
// 非用户调用：在ms定时器调用
void OnTIMForBatteryMonitoring(void);
// 初始化
void InitBatteryMonitoring(void);
// 开始一次电压测量
//void StartSingleBatteryADCConvert(void);
// 获取电池电压，毫伏
uint16_t GetBatteryMillivolt(void);

#endif
