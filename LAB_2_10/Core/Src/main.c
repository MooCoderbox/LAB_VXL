/* USER CODE BEGIN Header */
// b_2_1
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

void display7SEG(int num);

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT (& htim2 );

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  HAL_Delay (1000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DOT_Pin|LED_RED_Pin|EN0_Pin|EN1_Pin
                          , GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|SEG3_Pin
                          |SEG4_Pin|SEG5_Pin|SEG6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DOT_Pin LED_RED_Pin EN0_Pin EN1_Pin
                           EN2_Pin EN3_Pin d_Pin e_Pin
                           f_Pin g_Pin */
  GPIO_InitStruct.Pin = DOT_Pin|LED_RED_Pin|EN0_Pin|EN1_Pin
                          ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin SEG3_Pin
                           SEG4_Pin SEG5_Pin SEG6_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|SEG3_Pin
                          |SEG4_Pin|SEG5_Pin|SEG6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*
void control_led(STATE_FSM state)
{
    switch (state)
    {
        case EW_RED_NS_GREEN:
            HAL_GPIO_WritePin(GPIOA, EW_RED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, EW_YEL_Pin | EW_GRE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, NS_GRE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | NS_YEL_Pin, GPIO_PIN_RESET);
            break;

        case EW_RED_NS_YELLOW:
            HAL_GPIO_WritePin(GPIOA, EW_RED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, EW_YEL_Pin | EW_GRE_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOA, NS_YEL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin | NS_GRE_Pin, GPIO_PIN_RESET);
            break;

        case EW_GREEN_NS_RED:
            HAL_GPIO_WritePin(GPIOA, EW_GRE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, EW_RED_Pin | EW_YEL_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, NS_YEL_Pin | NS_GRE_Pin, GPIO_PIN_RESET);
            break;

        case EW_YELLOW_NS_RED:
            HAL_GPIO_WritePin(GPIOA, EW_YEL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, EW_RED_Pin | EW_GRE_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOA, NS_RED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, NS_YEL_Pin | NS_GRE_Pin, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}

void display7SEG(int num)
{
    if (num < 0 || num > 9) return;

    // state {a, b, c, d, e, f, g}
    //  Common Anode: 0 =  RESET, 1 = SET
    static const uint8_t seg_table[10][7] = {
        //  a, b, c, d, e, f, g
        { 0, 0, 0, 0, 0, 0, 1 }, // 0
        { 1, 0, 0, 1, 1, 1, 1 }, // 1
        { 0, 0, 1, 0, 0, 1, 0 }, // 2
        { 0, 0, 0, 0, 1, 1, 0 }, // 3
        { 1, 0, 0, 1, 1, 0, 0 }, // 4
        { 0, 1, 0, 0, 1, 0, 0 }, // 5
        { 0, 1, 0, 0, 0, 0, 0 }, // 6
        { 0, 0, 0, 1, 1, 1, 1 }, // 7
        { 0, 0, 0, 0, 0, 0, 0 }, // 8
        { 0, 0, 0, 0, 1, 0, 0 }  // 9
    };

    HAL_GPIO_WritePin(GPIOA, a_Pin, seg_table[num][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, b_Pin, seg_table[num][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, c_Pin, seg_table[num][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, d_Pin, seg_table[num][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, e_Pin, seg_table[num][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, f_Pin, seg_table[num][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, g_Pin, seg_table[num][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void display7SEG_1(int num)
{
    if (num < 0 || num > 9) return;


    static const uint8_t seg_table[10][7] = {
        // a1, b1, c1, d1, e1, f1, g1
        {  0,  0,  0,  0,  0,  0,  1 }, // 0
        {  1,  0,  0,  1,  1,  1,  1 }, // 1
        {  0,  0,  1,  0,  0,  1,  0 }, // 2
        {  0,  0,  0,  0,  1,  1,  0 }, // 3
        {  1,  0,  0,  1,  1,  0,  0 }, // 4
        {  0,  1,  0,  0,  1,  0,  0 }, // 5
        {  0,  1,  0,  0,  0,  0,  0 }, // 6
        {  0,  0,  0,  1,  1,  1,  1 }, // 7
        {  0,  0,  0,  0,  0,  0,  0 }, // 8
        {  0,  0,  0,  0,  1,  0,  0 }  // 9
    };

    HAL_GPIO_WritePin(GPIOB, a1_Pin, seg_table[num][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, b1_Pin, seg_table[num][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, c1_Pin, seg_table[num][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, d1_Pin, seg_table[num][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, e1_Pin, seg_table[num][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, f1_Pin, seg_table[num][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, g1_Pin, seg_table[num][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
*/

void display7SEG(int num)
{
    if (num < 0 || num > 9) return;


    static const uint8_t seg_table[10][7] = {
        // a1, b1, c1, d1, e1, f1, g1
        {  0,  0,  0,  0,  0,  0,  1 }, // 0
        {  1,  0,  0,  1,  1,  1,  1 }, // 1
        {  0,  0,  1,  0,  0,  1,  0 }, // 2
        {  0,  0,  0,  0,  1,  1,  0 }, // 3
        {  1,  0,  0,  1,  1,  0,  0 }, // 4
        {  0,  1,  0,  0,  1,  0,  0 }, // 5
        {  0,  1,  0,  0,  0,  0,  0 }, // 6
        {  0,  0,  0,  1,  1,  1,  1 }, // 7
        {  0,  0,  0,  0,  0,  0,  0 }, // 8
        {  0,  0,  0,  0,  1,  0,  0 }  // 9
    };

    HAL_GPIO_WritePin(GPIOB, SEG0_Pin, seg_table[num][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG1_Pin, seg_table[num][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG2_Pin, seg_table[num][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG3_Pin, seg_table[num][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG4_Pin, seg_table[num][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG5_Pin, seg_table[num][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SEG6_Pin, seg_table[num][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int counter = 50;
int isFirstLed = 1;
void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef * htim )
{
	if (isFirstLed == 1) {
		 counter --;
		 if( counter <= 0){
		 counter = 50;
		 isFirstLed = 0;
		 }
		 HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_RESET);
		 display7SEG ( 1 );

	} else {
		counter --;
				 if( counter <= 0){
				 counter = 50;
				 isFirstLed = 1;
				 }

				 HAL_GPIO_WritePin(GPIOA, EN0_Pin, GPIO_PIN_SET);
				 		 HAL_GPIO_WritePin(GPIOA, EN1_Pin, GPIO_PIN_RESET);
				 display7SEG ( 2 );

	}


 }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
