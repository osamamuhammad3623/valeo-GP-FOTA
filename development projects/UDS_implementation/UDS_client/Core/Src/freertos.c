/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NodeMCU_COM.h"
#include "usart.h"
#include "uds_client.h"
#include "bootloader.h"
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
/* USER CODE BEGIN Variables */
uint8_t installationReadyFlag = 0;
/* USER CODE END Variables */
/* Definitions for UdsTask */
osThreadId_t UdsTaskHandle;
const osThreadAttr_t UdsTask_attributes = {
  .name = "UdsTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTask */
osThreadId_t UartTaskHandle;
const osThreadAttr_t UartTask_attributes = {
  .name = "UartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for InstallTask */
osThreadId_t InstallTaskHandle;
const osThreadAttr_t InstallTask_attributes = {
  .name = "InstallTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void StartUdsTask(void *argument);
void StartUartTask(void *argument);
void StartInstallTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of UdsTask */
  UdsTaskHandle = osThreadNew(StartUdsTask, NULL, &UdsTask_attributes);

  /* creation of UartTask */
  UartTaskHandle = osThreadNew(StartUartTask, NULL, &UartTask_attributes);

  /* creation of InstallTask */
  InstallTaskHandle = osThreadNew(StartInstallTask, NULL, &InstallTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartUdsTask */
/**
  * @brief  Function implementing the UdsTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUdsTask */
void StartUdsTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartUdsTask */
/*----------------------------------------------------------------*/
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
/*----------------------------------------------------------------*/
  UDS_init(target_update);
//  osThreadSetPriority(UdsTaskHandle, osPriorityNormal);
  osThreadTerminate(UdsTaskHandle);
  // Terminate task !!!!!

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUdsTask */
}

/* USER CODE BEGIN Header_StartUartTask */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTask */
void StartUartTask(void *argument)
{
  /* USER CODE BEGIN StartUartTask */
	uint8_t versionNumberFrame[4] = {VERSION_NUMBER, current_version_number[0], current_version_number[1], current_version_number[2]};
	HAL_UART_Transmit(&huart2, (uint8_t *)versionNumberFrame, sizeof(versionNumberFrame), HAL_MAX_DELAY);
	while(HAL_UART_Receive(&huart2, (uint8_t *)data_received, 6, HAL_MAX_DELAY) != HAL_OK);

	/* Infinite loop */
	for(;;)
	{
		if(!downloadFinishedFlag){
			UART_stateHandler();
		}
		else{
			// Do Nothing
		}
	}
  /* USER CODE END StartUartTask */
}

/* USER CODE BEGIN Header_StartInstallTask */
/**
* @brief Function implementing the InstallTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInstallTask */
void StartInstallTask(void *argument)
{
  /* USER CODE BEGIN StartInstallTask */
  /* Infinite loop */
  for(;;)
  {
	if (downloadFinishedFlag && HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET) {
		installationReadyFlag = 1;
		//ask user to install

		osThreadTerminate(UartTaskHandle);
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
		if (target_update[1]) {
			osThreadResume(target1ThreadID);
		}
	}
	if (target_update[0] && target1InstalledFlag) {
		// reboot
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
		bootloader_switch_to_inactive_bank();
		bootloader_reboot();
	}
    osDelay(1);
  }
  /* USER CODE END StartInstallTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

