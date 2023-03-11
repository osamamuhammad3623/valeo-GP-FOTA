//boot manger from sector 0 to sector 4
//sector 6 for certificates , meta-data, signature
//2k for each certificate , 1k for meta-data , 1k for signature then application
//sector 7 for app
#define APPLICATION_EXENSIONS_ADDRESS    0x08040000

#define ROOT_CERT_ADDRESS 				 APPLICATION_EXENSIONS_ADDRESS
#define SB_CERT_ADDRESS 			    (APPLICATION_EXENSIONS_ADDRESS + 0x0800)    //0x08040800 //2K byte offset from Application_extensions_Address
#define META_DATA_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01000)   //0x08041000 //4k byte offset from Application_extensions_Address
#define SIGNATURE_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01400)	//0x08041400 //5k byte offset from Application_extensions_Address
#define CERTIFICATES_METADATA_ADDRESS   (APPLICATION_EXENSIONS_ADDRESS + 0x01600)   //0x08041600

#define MAIN_APPLICATION_START_ADDRESS  (0x08060000)   //6k byte offset from Application_extensions_Address

#define ROOT_1_REVOCATION_ADDRESS  (0x1fff7800) //first byte in the Block 0 - OTP
#define ROOT_2_REVOCATION_ADDRESS  (0x1fff7820) //first byte in the Block 1 - OTP


#define META_DATA_LENGTH 82

#define MAJOR_V_METADATA_OFFSET         8
#define MINOR_V_METADATA_OFFSET         10
#define PATCH_V_METADATA_OFFSET         12
#define APP_SIZE__METADATA_OFFSET       20
#define ROOT_INDEX__METADATA_OFFSET     38
#define APP_HASH__METADATA_OFFSET       50

#define ROOT_CERT_SIZE_C_METADATA_OFFSET  15
#define SB_CERT_SIZE_C_METADATA_OFFSET    34
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
#include "rng.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "wolfssl/wolfcrypt/signature.h"
#include "wolfssl/wolfcrypt/rsa.h"
#include "wolfssl/ssl.h"
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


int sigLen = 256;
byte *sigBuf =(byte *)SIGNATURE_ADDRESS;
byte *Meta_Data=(byte *)META_DATA_ADDRESS;

long expected_n_root_chain_1_public_key[] = {
		    0xa204ba2d ,0x3d6db507 ,0x4cf9130c ,0x7a2696bb ,0x3b95941b ,0x60c4c14b ,0xb9db3d47 ,
		   	0x96c5d01e ,0x4c294e55 ,0xe9fbe300 ,0x1cd028c5 ,0x8d2387e9 ,0xc1a80302 ,0x5f12af64 ,
			0x7708d0b9 ,0x9714e69d ,0xf94449d4 ,0xad00fd72 ,0xd56f1391 ,0xfb439ee5 ,0xbf35a512 ,
		   	0xda25deeb ,0xdea24a14 ,0xec3d81a2 ,0x5d44de9e ,0xfcad0f71 ,0xaabef73d ,0x63b95934 ,
		   	0xe5438b15 ,0xc92bcd84 ,0x4b7bd6b6 ,0x96080734 ,0x9e5a9a96 ,0xa30cebb2 ,0x5bf80843 ,
		   	0x69800465 ,0x135bc1db ,0x584a8dd  ,0x85a63d2c ,0x213f1976 ,0x1a5f03aa ,0xa346e215 ,
		   	0x75e22fd9 ,0x761ed052 ,0xdbeb04e3 ,0xb239e131 ,0x881d104a ,0xad471520 ,0x8aff3472 ,
		   	0x461decf  ,0xc60db6c3 ,0x775ffdc8 ,0x1281dd12 ,0x373b2262 ,0xc1c5416b ,0xa9148ac4 ,
			0xc2122bac ,0x2997d297 ,0x3ac1b66d ,0xe2796ee6 ,0xed0613e ,0x536f7034 ,0x15c594b3 ,
		   	0xb8a2cc72
};
long expected_e_root_chain_1_public_key= 0x010001;

long expected_n_root_chain_2_public_key[] = {
		0x8ca9bb9d,0xf0b5ab9a,0xe8f50ddf,0x6710305e,0xe91ab574,0x30f00f26,0xf2d6c49c,
		0x153b3795,0x2be83929,0x512688fe,0xe6e1b550,0x8cd0addf,0xe5b5fd6a,0x4e240f4c,
		0xc9504421,0xbfbcbadc,0x4ddf22de,0xee8421f2,0x53e7e1fd,0x7381cd96,0xd8e0c95 ,
		0xf607c3f ,0x35700c8e,0x50b87fc1,0x9f349d80,0x64283a4e,0x714fba4f,0x6aff8223,
		0x1871dfda,0x658d6e3f,0x422db028,0xf6c36d86,0x27179383,0x5a96bd24,0xa35533e1,
		0x221a4b4f,0xb749357 ,0xf4a5a69c,0x6e54aa51,0xe35090e2,0x8cb44963,0x29bf5a11,
		0x8b249dad,0xb3abc0a5,0x4c268acd,0xa1a5dbb4,0x55b07f26,0x5bdbfe,0xb9258160,
		0x73b75be2,0x7a4225d5,0x3a37b447,0xeb776627,0x8f337b19,0x7493a419,0x1a3a78dc,
		0x1a26114a,0xab29a71e,0xc737a1da,0xccc1b0c3,0x32ec74b4,0x58ca3b87,0xbd3f1763,
		0xe969ef22
};
long expected_e_root_chain_2_public_key= 0x010001;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int Certificates_Verify( const byte* ptr_rootCert, const byte* ptr_sbCert,int rootCert_size,int SBCert_size){
	   WOLFSSL_CERT_MANAGER* cm = NULL;
	   int ret;
	   int cSz1 = SBCert_size;
	   int cSz2 = rootCert_size;
	   wolfSSL_Init();

	   if ((cm = wolfSSL_CertManagerNew()) == NULL) {
	       printf("cert manager new failed\n");
	       return 0;
	   }

	   ret = wolfSSL_CertManagerLoadCABuffer(cm, ptr_rootCert, cSz2, SSL_FILETYPE_DEFAULT);
	   if (ret != WOLFSSL_SUCCESS) {
	       printf("loading the ca chain failed\n");
	       wolfSSL_CertManagerFree(cm);
	       return 0;
	   }

	   ret = wolfSSL_CertManagerVerifyBuffer(cm, ptr_rootCert, cSz2, SSL_FILETYPE_DEFAULT);
	   if (ret != WOLFSSL_SUCCESS) {
	       printf("could not verify certificate.\n");
	       wolfSSL_CertManagerFree(cm);
	       return 0;
	   }else{
	  	 printf("Verification successful on root cert!\n");
	   }

	   ret = wolfSSL_CertManagerVerifyBuffer(cm, ptr_sbCert, cSz1, SSL_FILETYPE_DEFAULT);
	   if (ret != WOLFSSL_SUCCESS) {
	       printf("could not verify certificate.\n");
	       wolfSSL_CertManagerFree(cm);
	       return 0;
	   }else{
	     	 printf("Verification successful on secure boot cert!\n");
	   }

	   return 0;
}

int signature_verify(const byte* signature, int sig_length, const byte* metaData,int metadata_lengt, RsaKey *pubKey){

	Sha256         sha256;
	Sha256*        pSha256 = NULL;

	unsigned char  digest[WC_SHA256_DIGEST_SIZE];
	unsigned char  encSig[WC_SHA256_DIGEST_SIZE + 32];
	word32         encSigLen = 0;
	unsigned char* decSig = NULL;
	word32         decSigLen = 0;
	int ret=0;

	/* Calculate SHA-256 digest of message */
	if (ret == 0){
		ret = wc_InitSha256(&sha256);
	}
	if (ret == 0) {
		pSha256 = &sha256;
		ret = wc_Sha256Update(&sha256, metaData, metadata_lengt);   //size of() should be implemented but maybe -1, dep on the format of the meta data (array of hex or string)
	}
	if (ret == 0){
		ret = wc_Sha256Final(&sha256, digest);
	}


	/** using the hash function **/

	//digest = wc_CalculateDigest(metaData,34);

	/* Encode digest with algorithm information as per PKCS#1.5 */
	if (ret == 0) {
		encSigLen = wc_EncodeSignature(encSig, digest, sizeof(digest), SHA256h);
		if ((int)encSigLen < 0){
			ret = (int)encSigLen;
		}
	}


	/* Verify the signature by decrypting the value */
	if (ret == 0) {
		decSigLen = wc_RsaSSL_VerifyInline(signature, sig_length, &decSig, pubKey);
		if ((int)decSigLen < 0){
			ret = (int)decSigLen;
		}
	}

	/* Check the decrypted result matches the encoded digest. */
	if (ret == 0 && encSigLen != decSigLen){
		/* failure */
		ret = -1;
	}

	if (ret == 0 && XMEMCMP(encSig, decSig, encSigLen) != 0){
		/* failure */
		ret = -1;
	}

	/*free*/
	if (pSha256 != NULL){
		wc_Sha256Free(pSha256);
	}

	/* Report on the verification */
	return ret;


}

byte * CalculateDigest(const byte* image , int image_size )
{
	/*
	 * Variables Declaration
	 */
	  wc_Sha256 sha256;
	 static byte hash[WC_SHA256_DIGEST_SIZE];  //Hash value

	  int ret = -1;

	  //Initialization of SHA256
	  ret = wc_InitSha256(&sha256);
	  if (ret != 0)
	  {
		  printf("Failed to initialize SHA structure\n");
	  }

	  //calculate digest API
	  ret = wc_Sha256Update(&sha256, image , image_size);
	  if (ret != 0)
	  {
	       printf("Failed to calculate the hash\n");
	  }

	  else if (ret == 0)
	  {
		  ret =  wc_Sha256Final(&sha256, hash);

	  }
	  if (ret != 0)
	  {
	       printf("ERROR: Hash operation failed");
	  }

	  else
	  {
		/*
		 * Do nothing
		 */
	  }
	  return hash;
}

int DigestCompare ( byte *Digest , byte *MetaDigest)
{
       int ret = 0;
      ret = memcmp(Digest,MetaDigest,32);
      return ret;
}

int public_key_Extraction(const byte* ptr_Cert,int Cert_size,RsaKey *pubKey){

	int ret =0;
	WOLFSSL_X509* x509cert;
	WOLFSSL_EVP_PKEY* pubKeyTmp;
	word32 idx = 0;

	x509cert = wolfSSL_X509_load_certificate_buffer(ptr_Cert, Cert_size, WOLFSSL_FILETYPE_DEFAULT);
	if (x509cert == NULL) {
		printf("Failed to load cert, abort!\n");
		return ret;
	}
	printf("Loaded cert successfully\n");

	pubKeyTmp = wolfSSL_X509_get_pubkey(x509cert);
	if (pubKeyTmp == NULL) {
		printf("Failed to extract public key, abort!\n");
		return ret;
	}
	printf("Extracted public key successfully\n");

	/* setup a key structure to receive the extracted key */
	wc_InitRsaKey(pubKey, NULL);
	ret = wc_RsaPublicKeyDecode((byte*)pubKeyTmp->pkey.ptr, &idx, pubKey,
			(word32) pubKeyTmp->pkey_sz);
	if (ret != 0) {
		printf("Failed to decode public key from pubKeyTmp, abort!\n");
		return ret;
	}
	printf("Successfully decoded public key\n");
	wolfSSL_EVP_PKEY_free(pubKeyTmp);
	wolfSSL_X509_free(x509cert);
	ret =0;
	return ret;
}

int compare_long(long *n_public_key_1,long *n_public_key_2,int size){

	for(int i=0;i<size;i++){
		if(n_public_key_1[i] != n_public_key_2[i]){
			return 1;
		}
	}
	return 0;
}

void bootloader_jump_to_application(uint32_t start_addr){

	/* First, disable all IRQs */
    __disable_irq(); // ensure to __enable_irq() in the application main function

    // set vector table offset
    SCB->VTOR = (start_addr - 0x08000000);

    /* Get the main application start address */
    uint32_t jump_address = *(uint32_t *)(start_addr + 4);

    /* Set the main stack pointer to to the application start address */
    __set_MSP(*(uint32_t *)start_addr);
    __set_PSP(*(uint32_t *)start_addr);

    // Create function pointer for the main application
    void (*app_ptr)(void);
    app_ptr = (void *)(jump_address);

    // Now jump to the main application
    app_ptr();
}

int Secure_boot_goooooo(){
	int ret=0;
	/************** fetch needed data from memory****************/
	byte rootIndex = *((char *)META_DATA_ADDRESS+ROOT_INDEX__METADATA_OFFSET) - '0';
	int appSize = atoi((char *)META_DATA_ADDRESS+APP_SIZE__METADATA_OFFSET);




	  const byte* ptr_sbCert = (byte*)(SB_CERT_ADDRESS);
	  const byte* ptr_rootCert = (byte*)(ROOT_CERT_ADDRESS);

	  int SB_cert_size = atoi((char *)CERTIFICATES_METADATA_ADDRESS + SB_CERT_SIZE_C_METADATA_OFFSET);
	  int root_cert_size = atoi((char *)CERTIFICATES_METADATA_ADDRESS +ROOT_CERT_SIZE_C_METADATA_OFFSET);

	/*************** Root certificates Revocation*********************/
	 byte root_1_revoked = *((byte *)ROOT_1_REVOCATION_ADDRESS);
	 byte root_2_revoked = *((byte *)ROOT_2_REVOCATION_ADDRESS);
	  if(((rootIndex == 1) && (root_1_revoked != 0xff)) || ((rootIndex == 2) && (root_2_revoked != 0xff))){
		  return 1;
	  }


	  /*************** certificates verifications *********************/
	  ret = Certificates_Verify(ptr_rootCert, ptr_sbCert, root_cert_size, SB_cert_size);
	   if (ret != 0){
		   return 1;
	   }
	   /*****************************************************/
	   /************* Check Anchor  ************************/
	   /*****************************************************/

	   RsaKey ROOT_pubKey ;
	   ret = public_key_Extraction(ptr_rootCert,root_cert_size,&ROOT_pubKey);
	   if (ret != 0){
		   return 1;
	   }


	   //based on root index
	   if(rootIndex == 1){
		    /*********** compare n value *************/
			ret = compare_long((long *)expected_n_root_chain_1_public_key,(long *)ROOT_pubKey.n.dp,64);
		    if (ret != 0){
				   return 1;
		    }
			/*********** compare e value   ***********/
			ret = !((long)(ROOT_pubKey.e.dp[0]) == expected_e_root_chain_1_public_key);
			if (ret != 0){
			   return 1;
			}
	   }else if(rootIndex == 2){
		   /*********** compare n value *************/
			ret = compare_long((long *)expected_n_root_chain_2_public_key,(long *)ROOT_pubKey.n.dp,64);
			if (ret != 0){
				   return 1;
			}
			/*********** compare e value   ***********/
			ret = !((long)(ROOT_pubKey.e.dp[0]) == expected_e_root_chain_2_public_key);
			if (ret != 0){
			   return 1;
			}
	   }else{
		   return 1;
	   }




	  /************** secure boot public key Extraction ****************/
	   RsaKey SB_pubKey ;
	   ret = public_key_Extraction(ptr_sbCert,SB_cert_size,&SB_pubKey);
	   if (ret != 0){
		   return 1;
	   }


	   /******* signature verification ********/
		byte sigBuffer [256];
		uint32_t StartSectorAddress = SIGNATURE_ADDRESS;
		for(int i=0;i<256;i++){
			sigBuffer [i]= *(uint8_t *)StartSectorAddress;
			StartSectorAddress++;
		}
		ret=signature_verify(sigBuffer,sigLen, Meta_Data,META_DATA_LENGTH ,&SB_pubKey);
		if (ret == 0){
			printf("Verified\n");
		}
		else{
			printf("Failure\n");
		}
		wc_FreeRsaKey(&SB_pubKey);

		/************* Threshold  *****************/

		byte Major_Version = *((char *)META_DATA_ADDRESS+MAJOR_V_METADATA_OFFSET) - '0';
		byte Minor_Version = *((char *)META_DATA_ADDRESS+MINOR_V_METADATA_OFFSET) - '0';
		byte Patch_Version = *((char *)META_DATA_ADDRESS+PATCH_V_METADATA_OFFSET) - '0';

//		int Threshold = *((byte*)0x080E000C);
//		int Current_Version = *(rawData+8);
//		if (Current_Version < Threshold ){
//			return 1;
//		}


		/************* Hash  ***************/
		byte *Hash_value= (byte*)(Meta_Data+APP_HASH__METADATA_OFFSET);
		byte *Start_Add_App = (byte*)MAIN_APPLICATION_START_ADDRESS;
		byte *Hash_Result = CalculateDigest( Start_Add_App ,appSize );
		ret = DigestCompare ( Hash_Result , Hash_value);
		if(ret!=0){
			return 1;
		}
		printf("Succeed\n");

	   	return ret;
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
  MX_RNG_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */

//  int ret =1;
//  ret = Secure_boot_goooooo();
//  if(ret == 0){
//	  //jump to application
//
//  }else{
//	  //stuck on the while 1
//  }
  bootloader_jump_to_application(MAIN_APPLICATION_START_ADDRESS);

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

