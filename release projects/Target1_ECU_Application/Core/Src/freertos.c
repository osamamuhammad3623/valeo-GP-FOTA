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
#include "uds_server.h"
#include "ultrasonic.h"
#include "buzzer.h"
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

/* USER CODE END Variables */
/* Definitions for UdsTask */
osThreadId_t UdsTaskHandle;
const osThreadAttr_t UdsTask_attributes = {
  .name = "UdsTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for AppTask */
osThreadId_t AppTaskHandle;
const osThreadAttr_t AppTask_attributes = {
  .name = "AppTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for InitTask */
osThreadId_t InitTaskHandle;
const osThreadAttr_t InitTask_attributes = {
  .name = "InitTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for RefreshWDGTask */
osThreadId_t RefreshWDGTaskHandle;
const osThreadAttr_t RefreshWDGTask_attributes = {
  .name = "RefreshWDGTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartUdsTask(void *argument);
void StartAppTask(void *argument);
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

  /* creation of AppTask */
  AppTaskHandle = osThreadNew(StartAppTask, NULL, &AppTask_attributes);

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
  MX_LWIP_Init();
  /* USER CODE BEGIN StartUdsTask */
	UDS_init();
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartUdsTask */
}

/* USER CODE BEGIN Header_StartAppTask */
/**
 * @brief Function implementing the AppTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartAppTask */
void StartAppTask(void *argument)
{
  /* USER CODE BEGIN StartAppTask */
	ultrasonic_init();
	/* Infinite loop */
	for(;;)
	{
		ultrasonic_get_distance();
		if (g_ultrasonic_last_measured_distance <= ULTRASONIC_DISTANCE_2){
			buzzer_on();
			HAL_Delay(200);
			buzzer_off();
			HAL_Delay(200);
		}
		else if (g_ultrasonic_last_measured_distance <= ULTRASONIC_DISTANCE_1) {
			buzzer_on();
			HAL_Delay(400);
			buzzer_off();
			HAL_Delay(400);
		}
		else{
			buzzer_off();
		}

		osDelay(1);
	}
  /* USER CODE END StartAppTask */
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
		osDelay(2000);
	}
  /* USER CODE END StartRefreshWDGTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

