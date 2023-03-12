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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void bootloader_set_boot_bank(uint8_t bank){

	if ((bank != 1) && (bank!=2)){
		return;
	}

	// clear OPTLOCK bit to enable modifying the option bytes
	HAL_FLASH_OB_Unlock();

	// Check that no Flash memory operation is ongoing
	while(FLASH->SR & FLASH_SR_BSY_Msk);

	// if it's required to set bank 1, then clear BFB2 bit
	// and if it's required to set bank 2, set the BFB2 bit
	if (bank==1){
		CLEAR_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk);
	}else{
		SET_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk);
	}

	// start changing user option bytes
	//HAL_FLASH_OB_Launch();
	SET_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTSTRT_Msk);

	// Wait for the operation to be completed
	while(FLASH->SR & FLASH_SR_BSY_Msk);

	// unlock option-bytes-bit
	HAL_FLASH_OB_Lock();

}


void bootloader_switch_to_inactive_bank(void){

	// 0 represents bank 1,
	// 1 represents bank 2
	uint8_t active_bank = READ_BIT(FLASH->OPTCR, FLASH_OPTCR_BFB2_Msk);

	if (active_bank == 0){
		bootloader_set_boot_bank(2);
	}else{
		bootloader_set_boot_bank(1);
	}

	bootloader_reboot();
}


void bootloader_reboot(void){
	NVIC_SystemReset();
}
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

/* USER CODE BEGIN PV */
#define FAILED 0
#define SUCCEED 1
static uint32_t GetSector(uint32_t Address)
{
	uint32_t sector = 0;

	/* BANK 1 */
	if((Address >= 0x08000000 ) && (Address < 0x08003FFF))
	{
		sector = FLASH_SECTOR_0;
	}

	else if((Address >= 0x08004000 ) && (Address <  0x08007FFF))
	{
		sector = FLASH_SECTOR_1;
	}

	else if((Address >= 0x08008000) && (Address <  0x0800BFFF))
	{
		sector = FLASH_SECTOR_2;
	}

	else if((Address >= 0x0800C000) && (Address < 0x0800FFFF))
	{
		sector = FLASH_SECTOR_3;
	}

	else if((Address >=  0x08010000) && (Address <  0x0801FFFF))
	{
		sector = FLASH_SECTOR_4;
	}

	else if((Address >=  0x08020000) && (Address < 0x0803FFFF))
	{
		sector = FLASH_SECTOR_5;
	}

	else if((Address >= 0x08040000) && (Address < 0x0805FFFF))
	{
		sector = FLASH_SECTOR_6;
	}

	else if((Address >= 0x08060000) && (Address < 0x0807FFFF))
	{
		sector = FLASH_SECTOR_7;
	}

	else if((Address >= 0x08080000) && (Address < 0x0809FFFF))
	{
		sector = FLASH_SECTOR_8;
	}

	else if((Address >= 0x080A0000) && (Address < 0x080BFFFF))
	{
		sector = FLASH_SECTOR_9;
	}

	else if((Address >= 0x080C0000) && (Address < 0x080DFFFF))
	{
		sector = FLASH_SECTOR_10;
	}

	else if((Address >= 0x080E0000) && (Address < 0x080FFFFF))
	{
		sector = FLASH_SECTOR_11;
	}

	/* BANK 2 */
	if((Address >= 0x08100000 ) && (Address < 0x08103FFF))
	{
		sector = FLASH_SECTOR_12;
	}

	else if((Address >= 0x08104000 ) && (Address <  0x08107FFF))
	{
		sector = FLASH_SECTOR_13;
	}

	else if((Address >= 0x08108000) && (Address <  0x0810BFFF))
	{
		sector = FLASH_SECTOR_14;
	}

	else if((Address >= 0x0810C000) && (Address < 0x0810FFFF))
	{
		sector = FLASH_SECTOR_15;
	}

	else if((Address >=  0x08110000) && (Address <  0x0811FFFF))
	{
		sector = FLASH_SECTOR_16;
	}

	else if((Address >=  0x08120000) && (Address < 0x0813FFFF))
	{
		sector = FLASH_SECTOR_17;
	}

	else if((Address >= 0x08140000) && (Address < 0x0815FFFF))
	{
		sector = FLASH_SECTOR_18;
	}

	else if((Address >= 0x08160000) && (Address < 0x0817FFFF))
	{
		sector = FLASH_SECTOR_19;
	}

	else if((Address >= 0x08180000) && (Address < 0x0819FFFF))
	{
		sector = FLASH_SECTOR_20;
	}

	else if((Address >= 0x081A0000) && (Address < 0x081BFFFF))
	{
		sector = FLASH_SECTOR_21;
	}

	else if((Address >= 0x081C0000) && (Address < 0x081DFFFF))
	{
		sector = FLASH_SECTOR_22;
	}

	else if((Address >= 0x081E0000) && (Address < 0x081FFFFF))
	{
		sector = FLASH_SECTOR_23;
	}
	return sector;
}


uint8_t Flash_Memory_Write(uint32_t StartSectorAddress ,uint32_t *data, uint32_t dataSizeInBytes){

	static FLASH_EraseInitTypeDef EraseInitStruct;   /* Struct to erase the flash area */
	uint32_t SECTORError;
	uint32_t numofWords=dataSizeInBytes/4;
	uint32_t numofWordsWritten=0;

	/* Getting the number of sector to erase from the first sector */
	uint32_t StartSector = GetSector(StartSectorAddress);
	uint32_t EndSectorAddress = StartSectorAddress + dataSizeInBytes;
	uint32_t EndSector = GetSector(EndSectorAddress);


	/* Filling the erasing struct */
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector        = StartSector;
	EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

	/* Unlocking the Flash control register */
	HAL_FLASH_Unlock();

	/* check if the erasing process is done correctly */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*Error occurred while page erase*/
		return FAILED;
	}

	/* looping on the data word by word to write it in the flash */
	while(numofWordsWritten < numofWords){

		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, data[numofWordsWritten]) == HAL_OK)
		{
			StartSectorAddress += 4;
			numofWordsWritten++;
		}
		else
		{
			/* Error occurred while writing data in Flash memory*/
			return FAILED;
		}

	}

	/* Locking the Flash control register */
	HAL_FLASH_Lock();

	return SUCCEED;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
#define app_size 6516
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t data_received[app_size]={0};
int flag=0;

//uint64_t array_data[]
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	flag =100;

}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){



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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  int8_t xx[1]={0x05};
  HAL_UART_Transmit(&huart2, (uint8_t *)xx, sizeof(xx), HAL_MAX_DELAY);
  //HAL_UART_Transmit_IT(&huart2,xx, 2);

  HAL_UART_Receive_IT(&huart2, data_received, app_size);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //int counter =0;
  while (1)
  {
	  if(flag==100){
		  Flash_Memory_Write(0x08100000,(uint32_t *)data_received,app_size);
		  bootloader_switch_to_inactive_bank();
		  flag=0;
	  }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
