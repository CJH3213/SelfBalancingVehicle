#include "Battery.h"
#include "adc.h"
#include "Delay.h"

#define BATTERY_VREF 3294.0f		// 满量程电压（AVDD电压）
#define BATTERY_BUFFER_SIZE 200		// DMA缓存区大小
uint16_t gBatteryADCValues[BATTERY_BUFFER_SIZE];
float gBatterySingleVoltages = 0;
#define BATTERY_EMA_ALPHA 0.1f	// 指数移动平均的新值占比系数
float gBatterySmoothVoltages = 0;
//#define BATTERY_THRESHOLD 3		// 当新老电压值差大于该阈值才更新电压值
//uint16_t gBatteryLastVoltages = 0;

// 求绝对值
#define ABS(x) (x>=0?x:-x)

void InitBatteryMonitoring()
{
	HAL_ADCEx_Calibration_Start(&hadc1);	//ADC自校准
	
	// 接通电阻分压电路的NMOS
	HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_RESET);
//	Delay_ms(3);
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)gBatteryADCValues, BATTERY_BUFFER_SIZE);
}

//void StartSingleBatteryADCConvert()
//{
//	HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_SET);
//	Delay_ms(3);
//	// HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_RESET);
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)gBatteryADCValues, BATTERY_BUFFER_SIZE);
//}

void OnADCConversionCompleteForBattery()
{
	uint16_t min = gBatteryADCValues[0];
	uint16_t max = gBatteryADCValues[0];
	uint32_t sum = gBatteryADCValues[0];
	
	for(uint8_t i=1; i<BATTERY_BUFFER_SIZE; ++i)
	{
		sum += gBatteryADCValues[i];
		
		if(gBatteryADCValues[i] < min)
			min = gBatteryADCValues[i];
		else if(gBatteryADCValues[i] > max)
			max = gBatteryADCValues[i];
	}
	
	// 去除最大最小值后求平均
	sum -= min + max;
	sum /= (BATTERY_BUFFER_SIZE-2);
	gBatterySingleVoltages = sum * BATTERY_VREF / 4095.0f;
	
	// 指数移动平均（EMA），老值占比0.9，新值占比0.1
	if(gBatterySmoothVoltages > 600)
			gBatterySmoothVoltages = BATTERY_EMA_ALPHA*gBatterySingleVoltages +
		(1-BATTERY_EMA_ALPHA)*gBatterySmoothVoltages;
	else
		gBatterySmoothVoltages = gBatterySingleVoltages;	// 首次直接赋值

	
	// 滞后处理抖动（不好用）
//	if( ABS(gBatterySmoothVoltages-gBatteryLastVoltages) > BATTERY_THRESHOLD )
//		gBatteryLastVoltages = gBatterySmoothVoltages;
	
	// 切断电阻分压电路的NMOS，省电
	HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_RESET);
//	HAL_ADC_Stop_DMA(&hadc1);
  
}

void OnTIMForBatteryMonitoring()
{
	static uint16_t tCount = 0;

	tCount += BATTERY_TIM_PERIOD;
	
	// 每隔1秒测量一次电压，先打开NMOS到5ms后，再启动ADC
	if(tCount >= 1000)
		tCount = 0;
		
	if(tCount == 0)
		HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_SET);
	else if(tCount == 10)
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)gBatteryADCValues, BATTERY_BUFFER_SIZE);

}

uint16_t GetBatteryMillivolt(){return gBatterySmoothVoltages +0.5f;}
