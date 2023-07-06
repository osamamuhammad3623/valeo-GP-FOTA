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
#include "rtc.h"
#include "iwdg.h"
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
/* Definitions for InitTask */
osThreadId_t InitTaskHandle;
const osThreadAttr_t InitTask_attributes = {
  .name = "InitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for RefreshWDGTask */
osThreadId_t RefreshWDGTaskHandle;
const osThreadAttr_t RefreshWDGTask_attributes = {
  .name = "RefreshWDGTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for targetsMutex */
osMutexId_t targetsMutexHandle;
const osMutexAttr_t targetsMutex_attributes = {
  .name = "targetsMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void StartUdsTask(void *argument);
void StartUartTask(void *argument);
void StartInstallTask(void *argument);
void StartInitTask(void *argument);
void StartRefreshWDGTask(void *argument);

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
  /* Create the mutex(es) */
  /* creation of targetsMutex */
  targetsMutexHandle = osMutexNew(&targetsMutex_attributes);

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

  /* creation of InitTask */
  InitTaskHandle = osThreadNew(StartInitTask, NULL, &InitTask_attributes);

  /* creation of RefreshWDGTask */
  RefreshWDGTaskHandle = osThreadNew(StartRefreshWDGTask, NULL, &RefreshWDGTask_attributes);

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
  //MX_LWIP_Init();
  /* USER CODE BEGIN StartUdsTask */
	UDS_init(target_update);
	// Terminate task
	osThreadTerminate(UdsTaskHandle);

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
	HAL_Delay(LCD_PRINT_DELAY);
	lcd_clear();
	lcd_write_string_up("Version Number:");
	lcd_display_version(current_version_number);

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
			HAL_Delay(LCD_PRINT_DELAY);
			lcd_clear();
			lcd_write_string_up("Pending Install");
		}
		osDelay(1);
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

			osThreadTerminate(UartTaskHandle);

			//ask user to install
			lcd_clear();
			lcd_write_string_up("Install Now?");
			lcd_write_string_down("  A:Yes   B:No  ");

			char pressedButton;
			while(1){
				pressedButton = Keys_Detect();
				if(pressedButton == 'A'){
					lcd_clear();
					lcd_write_string_up("Installing ...");
					break;
				}else if(pressedButton == 'B'){
					lcd_clear();
					break;
				}else{
					// Do nothing
				}
			}

			if(pressedButton == 'B'){
				continue;				// wait for next parking mode to install
			}

			downloadFinishedFlag = 0; // must be handled in case of installation failure

			if (target_update[1]) {
				osThreadResume(target1ThreadID);
				osThreadSuspend(InstallTaskHandle);

				//check that new version number is correct
				if(memcmp(target1_version_number, new_version_number, 3) == 0){
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
					HAL_Delay(500);
					HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
				}else{
					// handle negative scenario
				}

				target1_version_received = 0;////////
				target1InstalledFlag = 0;///////////

				osThreadTerminate(target1ThreadID);
			}

			if(target_update[2]){
				osThreadResume(target2ThreadID);
				osThreadSuspend(InstallTaskHandle);

				//check that new version number is correct
				if(memcmp(target2_version_number, new_version_number, 3) == 0){
					HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
					HAL_Delay(500);
					HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
				}else{
					// handle negative scenario
				}

				target2_version_received = 0;////////
				target2InstalledFlag = 0;///////////

				osThreadTerminate(target2ThreadID);
			}
			installationReadyFlag = 0; /////////

			if(target_update[0]){
				// reboot
				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
				bootloader_switch_to_inactive_bank();
				bootloader_reboot();
			}else{
				current_version_number[0] = new_version_number[0];
				current_version_number[1] = new_version_number[1];
				current_version_number[2] = new_version_number[2];

				UdsTaskHandle = osThreadNew(StartUdsTask, NULL, &UdsTask_attributes);
				UartTaskHandle = osThreadNew(StartUartTask, NULL, &UartTask_attributes);
			}
		}else{
			// Do nothing
		}
		osDelay(1);
	}
  /* USER CODE END StartInstallTask */
}

/* USER CODE BEGIN Header_StartInitTask */
/**
 * @brief Function implementing the InitTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInitTask */
void StartInitTask(void *argument)
{
  /* USER CODE BEGIN StartInitTask */
	//zero attempt counter
	write_backup_reg(0x00 , 0x00);

	//refresh watchdog
	HAL_IWDG_Refresh(&hiwdg);

	//if you want to revoke root 1
	//	write_backup_reg(0x04, 0x01);
	//if you want to revoke root 2
	//	write_backup_reg(0x05, 0x01);

	//if you want update version
	write_backup_reg(0x1,0x5);
	write_backup_reg(0x2,0x5);
	write_backup_reg(0x3,0x5);

	osThreadTerminate(InitTaskHandle);

	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartInitTask */
}

/* USER CODE BEGIN Header_StartRefreshWDGTask */
/**
 * @brief Function implementing the RefreshWDGTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartRefreshWDGTask */
void StartRefreshWDGTask(void *argument)
{
  /* USER CODE BEGIN StartRefreshWDGTask */
	/* Infinite loop */
	for(;;)
	{
		//refresh watchdog
		HAL_IWDG_Refresh(&hiwdg);
		osDelay(1000);
	}
  /* USER CODE END StartRefreshWDGTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

