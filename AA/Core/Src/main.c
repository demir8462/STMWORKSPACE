/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
	TIM_HandleTypeDef timer2;

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
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void activeTimer()
{
	__TIM2_CLK_ENABLE();
	TIM_ClockConfigTypeDef clockcnf = {0};
		TIM_MasterConfigTypeDef mastercnfdef;
		timer2.Instance = TIM2;
		timer2.Init.Prescaler =36000-1;
		timer2.Init.CounterMode = TIM_COUNTERMODE_UP;
		timer2.Init.Period = 200;
		timer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		timer2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		if(HAL_TIM_Base_Init(&timer2) != HAL_OK)
		{

		}
		clockcnf.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&timer2, &clockcnf);
		mastercnfdef.MasterOutputTrigger = TIM_TRGO_UPDATE;
		HAL_TIMEx_MasterConfigSynchronization(&timer2, &mastercnfdef);
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
void stopTimer()
{
	HAL_TIM_Base_Stop_IT(&timer2);
}
void startTimer()
{
	HAL_TIM_Base_Start_IT(&timer2);
}
void activePinA(uint8_t pin)
{
	GPIOA->CRL = GPIOA->CRL & ~(0xF <<pin*4);
	  GPIOA->CRL |= (1<<pin*4);
}
void activeExtiPin(int pin)
{

	AFIO->EXTICR[0] &= ~(0xf<<1);
	EXTI->IMR |= (1<<pin);
	EXTI->RTSR |= (1<<1);  // Enable Rising Edge Trigger for PA1

	EXTI->FTSR &= ~(1<<1);
	NVIC_SetPriority (EXTI1_IRQn, 1);  // Set Priority
	NVIC_EnableIRQ (EXTI1_IRQn);
}
int led=0,cnt=0;
void EXTI1_IRQHandler(void)
{
	// Clear the interrupt flag by writing a 1
	if (EXTI->PR & (1<<1))    // If the PA1 triggered the interrupt
	{
		EXTI->PR |= (1<<1);
		cnt++;
		startStopTimer();

	}


}
int calisio=0;
void startStopTimer()
{
	if(calisio == 1)
	{
		calisio = 0;
		stopTimer();
	}else
	{
		calisio = 1;
		startTimer();
	}
}
void writePinA(uint8_t pin,uint8_t value)
{
	if(value == 1)
		GPIOA->ODR |= (1<<pin);
	else
		GPIOA->ODR &= (0<<pin);
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer2);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(TIM2->CNT %100 == 0 && htim == &timer2)
	{
		if(led == 0)
		{
			led = 1;
			writePinA(0, 1);
		}else
		{
			led = 0;
			writePinA(0, 0);
		}
	}
}

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
  RCC->APB2ENR |= (1<<2);

	RCC->APB2ENR |= (1<<0);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  //activeTimer();

  activePinA(0);
  activeTimer();


  //activeTimer();
  activeExtiPin(1);
  	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
