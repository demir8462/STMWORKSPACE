/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "font.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t s_0[6][4] = {
		{0,0,0,0},
		{0,1,1,0},
		{0,0,1,0},
		{0,1,1,0},
		{0,1,0,0},
		{0,0,0,0}
};


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile uint8_t displayArea[4][16];
uint8_t EKRAN[16][32];
uint8_t transmitData;
int brightness =500;
void Latch()
{
  HAL_GPIO_WritePin(GPIOA, LATCH_Pin, 1);
  HAL_GPIO_WritePin(GPIOA, LATCH_Pin, 0);
}
void writeData()
{
   for (uint8_t row = 0; row < 4; row++)
   {
	 HAL_GPIO_WritePin(GPIOA,A_Pin, row & 1);
	 HAL_GPIO_WritePin(GPIOA,B_Pin, row & 2);
    for (uint8_t col = 0; col < 16; col++)
    {
    	transmitData = displayArea[row][col];
    	HAL_SPI_Transmit(&hspi1, &transmitData, sizeof(transmitData), 2);
    }

    Latch();
    HAL_GPIO_WritePin(GPIOA, OE_Pin,1);
    HAL_Delay(5);
    HAL_GPIO_WritePin(GPIOA, OE_Pin,0);
   }

}
void clearScreen()
{
  for (uint8_t row = 0; row < 4; row++)
    for (uint8_t col = 0; col < 16; col++)
      displayArea[row][col] = 0xFF;
  for (uint8_t row = 0; row < 16; row++)
      for (uint8_t col = 0; col < 32; col++)
        EKRAN[row][col] = 1;
}
void drawPixel(uint8_t X,uint8_t Y,uint8_t Value)
{
	if(Value != 0 && Value != 1)
		return;
	EKRAN[Y][X] = Value;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim == &htim2)
	{

	}
}
uint8_t SATIR1INDEX=0;
uint8_t SATIR2INDEX=0;
uint8_t size = sizeof(s_0[0]);
int pows(int poww,int sayi)
{
	if (poww == 0)
		return 1;
	int buf = sayi;
	for (size_t i = 0; i < poww-1; i++)
	{
		buf *= sayi;
	}

	return buf;
}
void setAreaData(uint8_t bolge,uint8_t satir,uint8_t value)
{
	uint8_t etken=0;
	if(bolge <= 4)
	{
		displayArea[satir][3+(bolge-1)*4] = value;
	}else if(bolge <= 8 && bolge > 4)
	{
		displayArea[satir][2+(bolge-5)*4] = value;
	}else if(bolge > 8 && bolge <= 12)
	{
		displayArea[satir][1+(bolge-9)*4] = value;
	}else if(bolge > 12 && bolge <= 16)
	{
		displayArea[satir][0+(bolge-13)*4] = value;
	}

}
void WRITEE(uint8_t *STR,uint8_t len,uint8_t satirx)
{
	if(satirx == 1)
	{
		for(int i = 0;i<len;i++)
			{
				if(STR[i] != '0')
					continue;
				if(SATIR1INDEX + sizeof(s_0[0]) <= 32)
				{
					for(int satirr = 0;satirr<6;satirr++)
					{
						for(int j=0;j<size;j++)
						{
							EKRAN[satirr][SATIR1INDEX+j] = s_0[satirr][j];
						}
					}
					SATIR1INDEX += size+1;

				}else
					break;
			}
	}else
	{
		for(int i = 0;i<len;i++)
			{
				if(STR[i] != '0')
					continue;
				if(SATIR2INDEX + sizeof(s_0[0]) <= 32)
				{
					for(int satirr = 0;satirr<6;satirr++)
					{
						for(int j=0;j<size;j++)
						{
							EKRAN[satirr+8][SATIR2INDEX+j] = s_0[satirr][j];
						}
					}
					SATIR2INDEX += size+1;

				}else
					break;
			}
	}
	// PİYASA BAŞLANGIÇÇÇ

	uint8_t BUFFERSAYI=0,TUR=0,vv=0;
	for(int satirr = 0;satirr<16;satirr++)
	{
		TUR=0;BUFFERSAYI =0;
		for(int sutun = 0;sutun<32;sutun++)
		{
			if(EKRAN[satirr][sutun] == 1)
				BUFFERSAYI += pows(7-(sutun-TUR*8), 2);
			if(sutun-TUR*8 == 7)
			{
				TUR++; // ÖNEMLİ BÖLGE 0 OLMAMALI TURU ARTTIR ALLAHINI SİKERİM
				uint8_t bolge=0;
				bolge = (satirr/4)*4+TUR;
				setAreaData(bolge,satirr%4, BUFFERSAYI);
				BUFFERSAYI = 0;
				vv++;
			}
		}
	}

	// PIYASA BITIS BABALAR SİKİŞ
}
uint8_t a[] = "0000000";
uint8_t b[] = "000";
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
  MX_SPI1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(GPIOA, OE_Pin,0);
    HAL_GPIO_WritePin(GPIOA, A_Pin,0);
    HAL_GPIO_WritePin(GPIOA, B_Pin,0);
    HAL_GPIO_WritePin(GPIOA, LATCH_Pin,0);

  HAL_TIM_Base_Start_IT(&htim2);
  clearScreen();
  displayArea[1][6]=0x99;
  displayArea[2][6]=0x99;
  displayArea[3][6]=0b10000001;
  displayArea[0][5]=0b11000011;
  displayArea[1][5]=0b11100111;
  displayArea[2][5]=0b11100111;
  displayArea[3][5]=0b11100111;
  displayArea[3][10]=0b11001111;
  displayArea[0][9]=0b10110110;
  displayArea[1][9]=0b10000110;
  displayArea[2][9]=0b10111111;
  displayArea[3][9]=0b11000110;
  displayArea[1][14]=displayArea[2][14]=0b11111011;
  displayArea[3][14]=displayArea[1][13]=0b11011;
  displayArea[0][13]=0b11111011;
  displayArea[2][13]=0b11011111;
  displayArea[3][13]=0b111011;
  clearScreen();
  WRITEE(a, sizeof(a), 1);
  WRITEE(b, sizeof(b), 2);
  //writeData();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  writeData();

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  htim2.Init.Prescaler = 1000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 160;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A_Pin|B_Pin|OE_Pin|LATCH_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : A_Pin B_Pin OE_Pin LATCH_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|OE_Pin|LATCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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
