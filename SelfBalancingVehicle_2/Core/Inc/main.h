/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TestPort_Pin GPIO_PIN_13
#define TestPort_GPIO_Port GPIOC
#define VBAT_ADC_Pin GPIO_PIN_4
#define VBAT_ADC_GPIO_Port GPIOA
#define VBAT_CTRL_Pin GPIO_PIN_5
#define VBAT_CTRL_GPIO_Port GPIOA
#define MOTOR_DRIVER_FAULT_Pin GPIO_PIN_0
#define MOTOR_DRIVER_FAULT_GPIO_Port GPIOB
#define KEY4_Pin GPIO_PIN_12
#define KEY4_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_13
#define KEY3_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_14
#define KEY2_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_15
#define KEY1_GPIO_Port GPIOB
#define MPU_INT_Pin GPIO_PIN_12
#define MPU_INT_GPIO_Port GPIOA
#define BT_RST_Pin GPIO_PIN_4
#define BT_RST_GPIO_Port GPIOB
#define BT_CTRL_Pin GPIO_PIN_5
#define BT_CTRL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
