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
#include "../ECUAL/KEYPAD/KEYPAD.h"
#include "../ECUAL/LCD16x2/LCD16x2.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
#define KEY_1       1
#define KEY_2       2
#define KEY_3       3
#define KEY_4       4
#define KEY_5       5
#define KEY_6       6
#define KEY_7       7
#define KEY_8       8
#define KEY_9       9
#define KEY_A       A
#define KEY_B       B
#define KEY_C       C
#define KEY_D       D
#define KEY_0       0
#define KEY_star    s
#define KEY_E       E
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

uint8_t gu8_KeyStatesArr[KEYS] = {0};
uint8_t SysTicks = 0;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void SysTick_CallBack(void);
void Display_Handler(void);
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
  /* USER CODE BEGIN 2 */
  KEYPAD_Init(0, gu8_KeyStatesArr);
  LCD_Init();
  LCD_Clear();
  LCD_Set_Cursor(1, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  Display_Handler();
	  HAL_Delay(10);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
void SysTick_CallBack(void)
{
    SysTicks++;
    if(SysTicks == 5) // Each 5msec
    {
    KEYPAD_Scan(0);
    SysTicks = 0;
    }
}

void Display_Handler(void)
{
    if(gu8_KeyStatesArr[KEY_1] == KEY_PRESSED)
    {
    LCD_Write_Char('1');
    while(gu8_KeyStatesArr[KEY_1] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_2] == KEY_PRESSED)
    {
    LCD_Write_Char('2');
    while(gu8_KeyStatesArr[KEY_2] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_3] == KEY_PRESSED)
    {
    LCD_Write_Char('3');
    while(gu8_KeyStatesArr[KEY_3] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_4] == KEY_PRESSED)
    {
    LCD_Write_Char('4');
    while(gu8_KeyStatesArr[KEY_4] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_5] == KEY_PRESSED)
    {
    LCD_Write_Char('5');
    while(gu8_KeyStatesArr[KEY_5] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_6] == KEY_PRESSED)
    {
    LCD_Write_Char('6');
    while(gu8_KeyStatesArr[KEY_6] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_7] == KEY_PRESSED)
    {
    LCD_Write_Char('7');
    while(gu8_KeyStatesArr[KEY_7] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_8] == KEY_PRESSED)
    {
    LCD_Write_Char('8');
    while(gu8_KeyStatesArr[KEY_8] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_9] == KEY_PRESSED)
    {
    LCD_Write_Char('9');
    while(gu8_KeyStatesArr[KEY_9] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_0] == KEY_PRESSED)
    {
    LCD_Write_Char('0');
    while(gu8_KeyStatesArr[KEY_0] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_A] == KEY_PRESSED)
    {
    LCD_Write_Char('A');
    while(gu8_KeyStatesArr[KEY_A] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_B] == KEY_PRESSED)
    {
    LCD_Write_Char('B');
    while(gu8_KeyStatesArr[KEY_B] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_C] == KEY_PRESSED)
    {
    LCD_Write_Char('C');
    while(gu8_KeyStatesArr[KEY_C] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_D] == KEY_PRESSED)
    {
    LCD_Write_Char('D');
    while(gu8_KeyStatesArr[KEY_D] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_star] == KEY_PRESSED)
    {
    LCD_Write_Char('*');
    while(gu8_KeyStatesArr[KEY_star] == KEY_PRESSED);
    }
    if(gu8_KeyStatesArr[KEY_E] == KEY_PRESSED)
    {
    LCD_Write_Char('#');
    while(gu8_KeyStatesArr[KEY_E] == KEY_PRESSED);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Initializes the CPU, AHB and APB busses clocks
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
	  /** Initializes the CPU, AHB and APB busses clocks
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
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOD_CLK_ENABLE();

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
