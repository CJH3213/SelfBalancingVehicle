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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
	
	// 启动PWM输出
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	SetLeftMotor(0);
	SetRightMotor(0);

	// 串口初始化
	SerialPortOpen();

//	OLED_Init();			//初始化OLED
//	OLED_ShowString(4*16,1,"BY:CJH2",8);
	
	// 初始化MPU6050
	// MPU_Init();
	while( mpu_dmp_init() ){};
	// 启动定时器中断
	HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */
	uint8_t data[100];
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
//		HAL_Delay(500);
		// Test();
		// ProtocolEncode("1234\x7D\x7E", 6);
		ProtocolDecode(NULL, 0);
		
//		SerialRead(rxData, &rxLen);
//		if(rxLen > 0)
//		{
//			SerialWrite(rxData, rxLen);
//		}
		
//		for(int c = 0; c <= 10; c++)
//		{
//				OLED_ShowNum(64, 0, c, 4, 6);
//			SetLeftMotor(FORWARD, c);
//		}
//		delay_ms(1000);
//		OLED_ShowNum(0, 0, 0, 4, 12);
//		BrakingLeftMotor();
//		delay_ms(1000);
		
//		MPU_Get_Accelerometer(&ax, &ay, &az);
//		
//		ax = GetLeftMotorSpeed();
//		ay = GetRightMotorSpeed();
//		
//		//az = 32767;

//		if(mpu_dmp_get_data(&pitch, &roll, &yaw) != 0)
//			continue;
		OnMainForBalanceControl();
		
//		ax = pitch * 10;	// 俯仰角（±90°，平衡角）
//		if(ax < 0)
//		{
//			OLED_ShowChar(0, 0, '-', 6);
//			ax=-ax;
//		}
//		else
//			OLED_ShowChar(0, 0, ' ', 6);
//		OLED_ShowNum(1*6, 0, ax, 5, 6);
//		
//		ay = roll * 10;		// 横滚角（±180°）
//		if(ay < 0)
//		{
//			OLED_ShowChar(0, 1, '-', 6);
//			ay=-ay;
//		}
//		else
//			OLED_ShowChar(0, 1, ' ', 6);
//		OLED_ShowNum(1*6, 1, ay, 5, 6);
//		
//		az = yaw * 10;		// 航向角（0~360°）
//		if(az < 0)
//		{
//			OLED_ShowChar(0, 2, '-', 6);
//			az=-az;
//		}
//		else
//			OLED_ShowChar(0, 2, ' ', 6);
//		OLED_ShowNum(1*6, 2, az, 5, 6);
//		delay_ms(500);

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
