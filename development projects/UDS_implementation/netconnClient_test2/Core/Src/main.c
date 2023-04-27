/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcp_client.h"
#include "uds_client.h"
#include "flash_memory.h"
#include "bootloader.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t data_received[app_size];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint32_t masterDataSize;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
HASH_HandleTypeDef hhash;
__ALIGN_BEGIN static const uint8_t pKeyHASH[17] __ALIGN_END = {
                            0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
                            0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10};

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UART_Task */
osThreadId_t UART_TaskHandle;
const osThreadAttr_t UART_Task_attributes = {
  .name = "UART_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal-1,
};
/* USER CODE BEGIN PV */
sys_sem_t ethernetSem;
sys_sem_t uartSem;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_HASH_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void UartTask(void *argument);

/* USER CODE BEGIN PFP */
//#define app_size 6516
uint8_t flag = 0;
//uint8_t data_received[app_size]={0};
//uint8_t *data_received = (uint8_t *)0x1000000U;
uint8_t aArray[100] = {0};
uint8_t bArray[100] = {0};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	flag = 100;
	//HAL_UART_Receive_IT(&huart2, (uint8_t *)bArray, 100);
	//sys_sem_signal(&uartSem);

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_HASH_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  init_uds_request_callback(UDS_start_request);
  init_uds_recv_resp_clbk(UDS_receive_response);
//  erase_inactive_bank();
  int k;
  for (k = 0; k < app_size; k++) {
	  data_received[k] = 'A';
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of UART_Task */
  UART_TaskHandle = osThreadNew(UartTask, NULL, &UART_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief HASH Initialization Function
  * @param None
  * @retval None
  */
static void MX_HASH_Init(void)
{

  /* USER CODE BEGIN HASH_Init 0 */

  /* USER CODE END HASH_Init 0 */

  /* USER CODE BEGIN HASH_Init 1 */

  /* USER CODE END HASH_Init 1 */
  hhash.Init.DataType = HASH_DATATYPE_32B;
  hhash.Init.KeySize = 17;
  hhash.Init.pKey = (uint8_t *)pKeyHASH;
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN HASH_Init 2 */

  /* USER CODE END HASH_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : STLK_RX_Pin STLK_TX_Pin */
  GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
  //sys_arch_sem_wait(&ethernetSem, HAL_MAX_DELAY);
  //sys_arch_sem_wait(&ethernetSem, HAL_MAX_DELAY);
  tcpclient_init(0x02); // data_received[5]
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_UartTask */
/**
* @brief Function implementing the UART_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UartTask */
void UartTask(void *argument)
{
  /* USER CODE BEGIN UartTask */
	/*--------------TESTING UART WITH UDS--------------------------*/
//	sys_arch_sem_wait(&uartSem, HAL_MAX_DELAY); //////////////

	uint8_t downloadByte[] = {0x03};
	//uint8_t fileSizeInBytes[4];

	//uint8_t data_received2[200];
	//uint32_t dataWords[1602] = {0};
	//uint32_t *dataWords = (uint32_t *)0x10000000U;

//	HAL_UART_Transmit(&huart2, (uint8_t *)downloadByte, sizeof(downloadByte), HAL_MAX_DELAY);
////	HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 6416);  // 3208  6416  1604
//	HAL_UART_Receive(&huart2, data_received, 6416, HAL_MAX_DELAY); // 3208  6416  1604
	//while(flag!=100);
	flag = 0;

	//uint8_t flashingStatus = flash_memory_write((uint32_t *)data_received, 1604, APP); // dataWords

	//sys_sem_signal(&ethernetSem);

	//uint32_t sizeInWords = bytesToWords(data_received, 160, (uint32_t *)data_received); // 3204  6408  200  dataWords
	//erase_inactive_bank();


	//bootloader_switch_to_inactive_bank();
	//bootloader_reboot();
	/*HAL_UART_Transmit(&huart2, (uint8_t *)downloadByte, sizeof(downloadByte), HAL_MAX_DELAY);
	HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 100);
	//HAL_UART_Receive(&huart2, (uint8_t *)data_received, 100, HAL_MAX_DELAY); // 3204
	//while(flag!=100);
	//flag = 0;
	int j;
	for (j = 0; j < 801; j++) {
		dataWords[j] = 0;
	}
	sizeInWords = bytesToWords(data_received, 100, dataWords);  // 3204
	flashingStatus = flash_memory_write(dataWords, sizeInWords, APP);*/


//	for(i = 0; i<2; i++) {
//		HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 6408);
//		while(flag!=100);
//		flag = 0;
//		uint32_t sizeInWords = bytesToWords(data_received, 6408, dataWords);
//		uint8_t flashingStatus = flash_memory_write(dataWords, sizeInWords, APP);
//	}
	//HAL_UART_Receive_IT(&huart2, (uint8_t *)fileSizeInBytes, 4);
	//sys_arch_sem_wait(&uartSem, HAL_MAX_DELAY);

	//int j;
	/*uint8_t versionNumber[4]={0x01, 0x02, 0x01, 0x00};
	HAL_UART_Transmit(&huart2, (uint8_t *)versionNumber, sizeof(versionNumber), HAL_MAX_DELAY);

	HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 6);
	// print version number on HMI
	// wait on uart semaphore for NEW PACKAGE frame
	// ... uart ISR gives uart semaphore
	data_received[5] = 0x07; // for testing


	// parse received frame, notify user, check connect with a target or not

	//sys_sem_signal(&ethernetSem);
	// if update is for fota master: erase memory and flash the received frame
	uint8_t downloadPackageFrame[4] = {0x03, 0x02, 0x01, 0x03};
	HAL_UART_Transmit(&huart2, (uint8_t *)downloadPackageFrame, sizeof(downloadPackageFrame), HAL_MAX_DELAY);

	// erase memory at fota master if it has an update (in a separate task)

	HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 1);
	// wait on uart semaphore for PACKAGE DOWNLOADED frame
	// ... uart ISR gives uart semaphore
	data_received[0] = 0x04; // for testing

	if (data_received[0] == 0x04) {
		uint8_t targetUpdateFrame[4] = {0x05, 0x00, 0x01, 0xFF}; // fota master image file
		HAL_UART_Transmit(&huart2, (uint8_t *)targetUpdateFrame, sizeof(targetUpdateFrame), HAL_MAX_DELAY);
	}

	// receive file size
	HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 4);
	// wait on uart semaphore for SECUREBOOT/IMAGE SIZE frame
	// ... uart ISR gives uart semaphore
	data_received[0] = 0x07; // for testing
	data_received[1] = 0x00; // for testing
	data_received[2] = 0x00; // for testing
	data_received[3] = 0x0A; // for testing

	if (data_received[0] == 0x07) {
		masterDataSize = (((uint32_t)data_received[1])<<2*8|((uint32_t)data_received[2])<<8|((uint32_t)data_received[3]));
	}

	// start sending frame
	uint8_t startSendingFrame[4] = {0x08, 0xFF, 0xFF, 0xFF};
	HAL_UART_Transmit(&huart2, (uint8_t *)startSendingFrame, sizeof(startSendingFrame), HAL_MAX_DELAY);

	int i = masterDataSize;
	for (; i > 0; i-=1024) {
		if (i >= 1024) {
			HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, 1024);
		} else {
			HAL_UART_Receive_IT(&huart2, (uint8_t *)data_received, i);
		}
		// wait on uart semaphore for data frame
		// ... uart ISR gives uart semaphore
		//sys_arch_sem_wait(&uartSem, HAL_MAX_DELAY);

		uint32_t dummyData[] = {0x22441133, 0x88775566, 0xAAEEAAEE};
		// forward to target, or flash at master
		uint8_t flashStatus = flash_memory_write(dummyData, 3, APP);
		// if ok, send ok frame
		if (flashStatus == SUCCEED) {
			uint8_t okFrame[4] = {0x00, 0x00, 0x00, 0x00};
			HAL_UART_Transmit(&huart2, (uint8_t *)okFrame, sizeof(okFrame), HAL_MAX_DELAY);
		}

	}*/


  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END UartTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
