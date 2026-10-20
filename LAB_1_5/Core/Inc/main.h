/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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
#define EW_RED_Pin GPIO_PIN_1
#define EW_RED_GPIO_Port GPIOA
#define EW_YEL_Pin GPIO_PIN_2
#define EW_YEL_GPIO_Port GPIOA
#define EW_GRE_Pin GPIO_PIN_3
#define EW_GRE_GPIO_Port GPIOA
#define NS_RED_Pin GPIO_PIN_4
#define NS_RED_GPIO_Port GPIOA
#define NS_YEL_Pin GPIO_PIN_5
#define NS_YEL_GPIO_Port GPIOA
#define NS_GRE_Pin GPIO_PIN_6
#define NS_GRE_GPIO_Port GPIOA
#define a_Pin GPIO_PIN_7
#define a_GPIO_Port GPIOA
#define a1_Pin GPIO_PIN_0
#define a1_GPIO_Port GPIOB
#define b1_Pin GPIO_PIN_1
#define b1_GPIO_Port GPIOB
#define c1_Pin GPIO_PIN_2
#define c1_GPIO_Port GPIOB
#define b_Pin GPIO_PIN_8
#define b_GPIO_Port GPIOA
#define c_Pin GPIO_PIN_9
#define c_GPIO_Port GPIOA
#define d_Pin GPIO_PIN_10
#define d_GPIO_Port GPIOA
#define e_Pin GPIO_PIN_11
#define e_GPIO_Port GPIOA
#define f_Pin GPIO_PIN_12
#define f_GPIO_Port GPIOA
#define g_Pin GPIO_PIN_13
#define g_GPIO_Port GPIOA
#define d1_Pin GPIO_PIN_3
#define d1_GPIO_Port GPIOB
#define e1_Pin GPIO_PIN_4
#define e1_GPIO_Port GPIOB
#define f1_Pin GPIO_PIN_5
#define f1_GPIO_Port GPIOB
#define g1_Pin GPIO_PIN_6
#define g1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
