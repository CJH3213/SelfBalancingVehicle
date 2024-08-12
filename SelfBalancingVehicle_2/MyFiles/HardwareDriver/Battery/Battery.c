#include "Battery.h"
#include "adc.h"
#include "Delay.h"

#define BATTERY_VREF 3294.0f		// �����̵�ѹ��AVDD��ѹ��
#define BATTERY_BUFFER_SIZE 200		// DMA��������С
uint16_t gBatteryADCValues[BATTERY_BUFFER_SIZE];
float gBatterySingleVoltages = 0;
#define BATTERY_EMA_ALPHA 0.1f	// ָ���ƶ�ƽ������ֵռ��ϵ��
float gBatterySmoothVoltages = 0;
//#define BATTERY_THRESHOLD 3		// �����ϵ�ѹֵ����ڸ���ֵ�Ÿ��µ�ѹֵ
//uint16_t gBatteryLastVoltages = 0;

// �����ֵ
#define ABS(x) (x>=0?x:-x)

void InitBatteryMonitoring()
{
	HAL_ADCEx_Calibration_Start(&hadc1);	//ADC��У׼
	
	// ��ͨ�����ѹ��·��NMOS
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
	
	// ȥ�������Сֵ����ƽ��
	sum -= min + max;
	sum /= (BATTERY_BUFFER_SIZE-2);
	gBatterySingleVoltages = sum * BATTERY_VREF / 4095.0f;
	
	// ָ���ƶ�ƽ����EMA������ֵռ��0.9����ֵռ��0.1
	if(gBatterySmoothVoltages > 600)
			gBatterySmoothVoltages = BATTERY_EMA_ALPHA*gBatterySingleVoltages +
		(1-BATTERY_EMA_ALPHA)*gBatterySmoothVoltages;
	else
		gBatterySmoothVoltages = gBatterySingleVoltages;	// �״�ֱ�Ӹ�ֵ

	
	// �ͺ������������ã�
//	if( ABS(gBatterySmoothVoltages-gBatteryLastVoltages) > BATTERY_THRESHOLD )
//		gBatteryLastVoltages = gBatterySmoothVoltages;
	
	// �жϵ����ѹ��·��NMOS��ʡ��
	HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_RESET);
//	HAL_ADC_Stop_DMA(&hadc1);
  
}

void OnTIMForBatteryMonitoring()
{
	static uint16_t tCount = 0;

	tCount += BATTERY_TIM_PERIOD;
	
	// ÿ��1�����һ�ε�ѹ���ȴ�NMOS��5ms��������ADC
	if(tCount >= 1000)
		tCount = 0;
		
	if(tCount == 0)
		HAL_GPIO_WritePin(VBAT_CTRL_GPIO_Port, VBAT_CTRL_Pin, GPIO_PIN_SET);
	else if(tCount == 10)
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)gBatteryADCValues, BATTERY_BUFFER_SIZE);

}

uint16_t GetBatteryMillivolt(){return gBatterySmoothVoltages +0.5f;}
