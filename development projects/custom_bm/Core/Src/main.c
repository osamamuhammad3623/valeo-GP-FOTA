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
#include "bootloader.h"

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
#define APPLICATION_EXENSIONS_ADDRESS    0x08040000

#define ROOT_CERT_ADDRESS 				 APPLICATION_EXENSIONS_ADDRESS
#define SB_CERT_ADDRESS 			    (APPLICATION_EXENSIONS_ADDRESS + 0x0800)    //0x08040800 //2K byte offset from Application_extensions_Address
#define META_DATA_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01000)   //0x08041000 //4k byte offset from Application_extensions_Address
#define SIGNATURE_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01400)	//0x08041400 //5k byte offset from Application_extensions_Address
#define CERTIFICATES_METADATA_ADDRESS   (APPLICATION_EXENSIONS_ADDRESS + 0x01600)   //0x08041600

#define MAIN_APPLICATION_START_ADDRESS  (0x08060000U)   //6k byte offset from Application_extensions_Address

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
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

int Secure_boot_goooooo(){
	int ret=0;
	//2
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
		  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);HAL_Delay(3000);

	/************** fetch needed data from memory****************/
	byte rootIndex = *((char *)META_DATA_ADDRESS+ROOT_INDEX__METADATA_OFFSET) - '0';
	int appSize = atoi((char *)META_DATA_ADDRESS+APP_SIZE__METADATA_OFFSET);
	//3
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
		  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);HAL_Delay(3000);


	  const byte* ptr_sbCert = (byte*)(SB_CERT_ADDRESS);
	  const byte* ptr_rootCert = (byte*)(ROOT_CERT_ADDRESS);

	  int SB_cert_size = atoi((char *)CERTIFICATES_METADATA_ADDRESS + SB_CERT_SIZE_C_METADATA_OFFSET);
	  int root_cert_size = atoi((char *)CERTIFICATES_METADATA_ADDRESS +ROOT_CERT_SIZE_C_METADATA_OFFSET);
	  //4
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
			  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);HAL_Delay(3000);

	/*************** Root certificates Revocation*********************/
	 byte root_1_revoked = *((byte *)ROOT_1_REVOCATION_ADDRESS);
	 byte root_2_revoked = *((byte *)ROOT_2_REVOCATION_ADDRESS);
	  if(((rootIndex == 1) && (root_1_revoked != 0xff)) || ((rootIndex == 2) && (root_2_revoked != 0xff))){
		  return 1;
	  }
	  //5
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);HAL_Delay(3000);
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
	   //6
	   HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
	   HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);


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
		//7
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(3000);
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //1
  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(2000);
  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); HAL_Delay(2000);

  int ret = Secure_boot_goooooo();
  //8
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); HAL_Delay(2000);
  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); HAL_Delay(2000);

  bootloader_jump_to_application(0x08060000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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
