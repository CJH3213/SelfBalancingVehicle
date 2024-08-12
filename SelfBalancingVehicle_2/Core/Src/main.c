/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "Motors.h"
#include "oled.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "SerialPort.h"
#include "Protocol.h"
#include "BalanceControl.h"
#include "BLE_HC09.h"
#include "Key.h"
#include <stdio.h>
#include "Battery.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//	int16_t ax, ay, az;
//	float pitch, roll, yaw;
//	uint8_t mpuErrorCount = 0;

//	uint8_t rxData[50];
//	uint32_t rxLen;
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	
	// 初始化电机驱动
	InitMotorDriver();
	// 初始化编码器驱动
	InitMotorEncodersDriver();

	// 串口初始化
	SerialPortOpen();
	
	// 电压监测初始化
//	InitBatteryMonitoring();

//	OLED_Init();			//初始化OLED
//	fill_picture(0xFF);
	HAL_Delay(100);
//	OLED_ShowString(4*16,1,"BY:CJH2",8);
	HAL_Delay(100);

	// 初始化MPU6050
//	OLED_ShowString(0, 0, "MPU is initializing.", 6);
	while( mpu_dmp_init() ){};
//	OLED_Clear();
//	OLED_ShowString(0, 0, "Running.", 6);
		
	// 初始化所有按钮标志位
	InitAllKeys();
		
	// 启动定时器中断
	HAL_TIM_Base_Start_IT(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		HAL_Delay(500);
		// Test();
		// ProtocolEncode("1234\x7D\x7E", 6);
  	ProtocolDecode(NULL, 0);

		// 按钮状态显示
//		char keyState = Key1.KEY_DOWN ? '1' : '0';
//		OLED_Buffer_ShowChar(0, 3*8, keyState, 6);
//		keyState = Key2.KEY_DOWN ? '1' : '0';
//		OLED_Buffer_ShowChar(1*6, 3*8, keyState, 6);
//		keyState = Key3.KEY_DOWN ? '1' : '0';
//		OLED_Buffer_ShowChar(2*6, 3*8, keyState, 6);
//		keyState = Key4.KEY_DOWN ? '1' : '0';
//		OLED_Buffer_ShowChar(3*6, 3*8, keyState, 6);
		
		if(Key1.KEY_DOWN)
		{
			HAL_NVIC_SystemReset();
		}
//			// 暂定I2C错误时处理方案为：重新初始化
//			HAL_I2C_DeInit(&hi2c1);
//			HAL_I2C_Init(&hi2c1);

//			HAL_I2C_DeInit(&hi2c2);
//			HAL_I2C_Init(&hi2c2);
//			
//			MX_I2C1_Init();
//			MX_I2C2_Init();
//		}

//		if(Key2.KEY_DOWN)
//		{
//			while( mpu_dmp_init() ){};
//		}
		
		
		OnMainForBalanceControl();
		
		// 俯仰角（±90°，平衡角）
//		OLED_Buffer_ShowString(0, 0, OLED_FloatToString(gPitch+0.05f, 1), 6);
//		// 横滚角（±180°）
//		OLED_Buffer_ShowString(0, 1*8, OLED_FloatToString(gRoll+0.05f, 1), 6);
//		// 航向角（0~360°）
//		OLED_Buffer_ShowString(0, 2*8, OLED_FloatToString(gYaw+0.05f, 1), 6);
//		
//		// 显示电池电压，显示小数点后两位，所以在第三位四舍五入
//		float batteryVoltage = GetBatteryMillivolt() * 2 / 1000.0f + 0.005f;
//		OLED_Buffer_ShowString(17*6, 0, OLED_FloatToString(batteryVoltage, 2), 6);
		
//		Test(&gSerialPort2);
		//SerialWrite(&gSerialPort2, (uint8_t*)"USART2", 6);
//  	printf("p:%.1f, r:%.1f, y:%.1f\r\n", pitch, roll, yaw);
//		printf("LME:%d, RME:%d\r\n", GetLeftMotorSpeed(), GetRightMotorSpeed());
//		printf("Battery Voltage: %d mV\r\n", GetBatteryMillivolt());
		
//		OLED_Buffer_Refresh();
//		OLED_Buffer_Clear();

		// 主循环内容
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
