/******************************************************************************
  * @file    boot_manager.c
  * @author  BOOT MANAGER Team
  * @brief   source file for boot manager Application Program implementation.
 *******************************************************************************/

/********************************** Includes **********************************/
#include "boot_manager.h"
#include "stdio.h"
#include "wolfssl/wolfcrypt/signature.h"
#include "wolfssl/wolfcrypt/rsa.h"
#include "wolfssl/ssl.h"

/******************************************************************************/
/******************************************************************************/
/************************** Private Global Variables **************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/* stored public key data for Hardware Anchor verification */
static uint32_t expected_n_root_chain_1_public_key[] = {
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
static uint32_t expected_e_root_chain_1_public_key= 0x010001;

static uint32_t expected_n_root_chain_2_public_key[] = {
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
static uint32_t expected_e_root_chain_2_public_key= 0x010001;


/******************************************************************************/
/******************************************************************************/
/************************* Private Function prototypes ************************/
/******************************************************************************/
/******************************************************************************/

static uint8_t Certificates_Verify( const uint8_t* ptr_rootCert, const uint8_t* ptr_sbCert,uint32_t rootCert_size,uint32_t SBCert_size);

static uint8_t public_key_Extraction(const uint8_t* ptr_Cert,uint32_t Cert_size,RsaKey *pubKey);

static uint8_t compare_keys(uint32_t *n_public_key_1,uint32_t *n_public_key_2,uint32_t size);

static uint8_t signature_verify(const uint8_t* signature, uint32_t sig_length, const uint8_t* metaData,uint32_t metadata_lengt, RsaKey *pubKey);

static uint8_t * CalculateDigest(const uint8_t* image , uint32_t image_size );

static uint8_t DigestCompare ( uint8_t *Digest , uint8_t *MetaDigest);

/******************************************************************************/
/******************************************************************************/
/*************************** Functions Definitions ****************************/
/******************************************************************************/
/******************************************************************************/

uint8_t secure_boot_verify(void){
	int8_t ret=0;

	/************** fetch needed data from memory****************/
	uint8_t rootIndex = *((char *)META_DATA_ADDRESS + ROOT_INDEX__METADATA_OFFSET)-'0';
	uint32_t appSize = atoi((char *)META_DATA_ADDRESS+APP_SIZE__METADATA_OFFSET);

	const uint8_t* ptr_sbCert = (uint8_t*)(SB_CERT_ADDRESS);
	const uint8_t* ptr_rootCert = (uint8_t*)(ROOT_CERT_ADDRESS);

	uint32_t SB_cert_size = atoi((uint8_t *)CERTIFICATES_METADATA_ADDRESS + SB_CERT_SIZE_C_METADATA_OFFSET);
	uint32_t root_cert_size = atoi((uint8_t *)CERTIFICATES_METADATA_ADDRESS +ROOT_CERT_SIZE_C_METADATA_OFFSET);

	/*************** Root certificates Revocation*********************/
	uint8_t root_1_revoked = *((uint8_t *)ROOT_1_REVOCATION_ADDRESS);
	uint8_t root_2_revoked = *((uint8_t *)ROOT_2_REVOCATION_ADDRESS);
	//still need Revisited
	if( (rootIndex != 1) && (rootIndex != 2) ){
			  return FAILED;
    }else if(((rootIndex == 1) && (root_1_revoked == 0x00)) || ((rootIndex == 2) && (root_2_revoked == 0x00))){
    	return FAILED;
    }


	/*************** certificates verifications *********************/
	ret = Certificates_Verify(ptr_rootCert, ptr_sbCert, root_cert_size, SB_cert_size);
	if (ret != SUCCEEDED){
		return FAILED;
	}

	/*****************************************************/
	/************* Check Anchor  *************************/
	/*****************************************************/

	RsaKey ROOT_pubKey ;
	ret = public_key_Extraction(ptr_rootCert,root_cert_size,&ROOT_pubKey);
	if (ret != SUCCEEDED){
		return FAILED;
	}


	//based on root index
	if(rootIndex == 1){
		/*********** compare n value *************/
		ret = compare_keys((uint32_t *)expected_n_root_chain_1_public_key,(uint32_t *)ROOT_pubKey.n.dp,64);
		if (ret != SUCCEEDED){
			return FAILED;
		}
		/*********** compare e value   ***********/
		ret = !((uint32_t)(ROOT_pubKey.e.dp[0]) == expected_e_root_chain_1_public_key);
		if (ret != SUCCEEDED){
			return FAILED;
		}
	}else if(rootIndex == 2){
		/*********** compare n value **************/
		ret = compare_keys((uint32_t *)expected_n_root_chain_2_public_key,(uint32_t *)ROOT_pubKey.n.dp,64);
		if (ret != SUCCEEDED){
			return FAILED;
		}
		/*********** compare e value   ***********/
		ret = !((uint32_t)(ROOT_pubKey.e.dp[0]) == expected_e_root_chain_2_public_key);
		if (ret != SUCCEEDED){
			return FAILED;
		}
	}else{
		return FAILED;
	}




	/************** secure boot public key Extraction ****************/
	RsaKey SB_pubKey ;
	ret = public_key_Extraction(ptr_sbCert,SB_cert_size,&SB_pubKey);
	if (ret != SUCCEEDED){
		return FAILED;
	}


	/************ signature verification ************/

	uint8_t *Meta_Data=(byte *)META_DATA_ADDRESS;
	uint8_t signatureBuffer [256];
	uint32_t SignatureIndex = SIGNATURE_ADDRESS;
	for(int i=0;i<256;i++){
		signatureBuffer [i]= *(uint8_t *)SignatureIndex;
		SignatureIndex++;
	}
	ret=signature_verify(signatureBuffer,SIGNATURE_LENGTH, Meta_Data,META_DATA_LENGTH ,&SB_pubKey);
	if (ret == SUCCEEDED){
		printf("Signature Verified\n");
	}
	else{
		printf("Signature Failure\n");
		return FAILED;
	}
	wc_FreeRsaKey(&SB_pubKey);

	/**************** Threshold  ********************/

	uint8_t Major_Version = *((char *)META_DATA_ADDRESS+MAJOR_V_METADATA_OFFSET) - '0';
	uint8_t Minor_Version = *((char *)META_DATA_ADDRESS+MINOR_V_METADATA_OFFSET) - '0';
	uint8_t Patch_Version = *((char *)META_DATA_ADDRESS+PATCH_V_METADATA_OFFSET) - '0';

	uint8_t threshold_Major_Version = *((uint8_t *)THRESHOLD_ADDRESS);
	uint8_t threshold_Minor_Version = *((uint8_t *)THRESHOLD_ADDRESS + 1);
	uint8_t threshold_Patch_Version = *((uint8_t *)THRESHOLD_ADDRESS + 2);

	byte threshold_approval=0;

	if(Major_Version > threshold_Major_Version){
		//approved
		threshold_approval=SUCCEEDED;
	}
	else if(Major_Version == threshold_Major_Version){
		//check minor
		if(Minor_Version > threshold_Minor_Version){
			//approved
			threshold_approval=SUCCEEDED;
		}
		else if(Minor_Version == threshold_Minor_Version){
			//check patch
			if(Patch_Version >threshold_Patch_Version){
				//approved
				threshold_approval=SUCCEEDED;
			}
			else{
				//Patch_Version < threshold_Patch_Version
				//denial
				threshold_approval=FAILED;
			}
		}
		else{
			//Minor_Version < threshold_Minor_Version
			//denial
			threshold_approval=FAILED;
		}
	}
	else{
		//Major_Version < threshold_Major_Version
		//denial
		threshold_approval=FAILED;
	}

	if(threshold_approval!=SUCCEEDED){
		return FAILED;
	}



	/************* Hash  ***************/
	uint8_t *Hash_value_in_metatdata= (uint8_t*)(Meta_Data+APP_HASH__METADATA_OFFSET);
	uint8_t *Start_Add_App = (uint8_t*)MAIN_APPLICATION_START_ADDRESS;
	uint8_t *Hash_Result = CalculateDigest( Start_Add_App ,appSize );
	ret = DigestCompare ( Hash_Result , Hash_value_in_metatdata);
	if(ret!=SUCCEEDED){
		return FAILED;
	}
	printf("Hash Verified\n");

	return SUCCEEDED;
}

void jump_to_application(uint32_t start_addr){

	/* First, disable all IRQs */
	__disable_irq(); // ensure to __enable_irq() in the application main function

	// set vector table offset
	SCB->VTOR = (start_addr - 0x08000000);

	/* Get the main application start address */
	uint32_t jump_address = *(uint32_t *)(start_addr + 4);

	/* Set the main stack pointer to to the application start address */
	__set_MSP(*(uint32_t *)start_addr);
	//__set_PSP(*(uint32_t *)start_addr);

	// Create function pointer for the main application
	void (*app_ptr)(void);
	app_ptr = (void *)(jump_address);

	// Now jump to the main application
	app_ptr();
}


/******************************************************************************/
/* \Description:
 * function to load and verify both the root certificate
 * and the secure boot certificate                                            */
/******************************************************************************/
static uint8_t Certificates_Verify( const uint8_t* ptr_rootCert, const uint8_t* ptr_sbCert,uint32_t rootCert_size,uint32_t SBCert_size){
	WOLFSSL_CERT_MANAGER* cm = NULL;
	int ret;
	uint32_t secure_boot_cert_size = SBCert_size;
	uint32_t root_cert_size = rootCert_size;
	wolfSSL_Init();

	if ((cm = wolfSSL_CertManagerNew()) == NULL) {
		printf("cert manager new failed\n");
		return FAILED;
	}

	ret = wolfSSL_CertManagerLoadCABuffer(cm, ptr_rootCert, root_cert_size, SSL_FILETYPE_DEFAULT);
	if (ret != WOLFSSL_SUCCESS) {
		printf("loading the ca chain failed\n");
		wolfSSL_CertManagerFree(cm);
		return FAILED;
	}

	ret = wolfSSL_CertManagerVerifyBuffer(cm, ptr_rootCert, root_cert_size, SSL_FILETYPE_DEFAULT);
	if (ret != WOLFSSL_SUCCESS) {
		printf("could not verify certificate.\n");
		wolfSSL_CertManagerFree(cm);
		return FAILED;
	}else{
		printf("Verification successful on root cert!\n");
	}

	ret = wolfSSL_CertManagerVerifyBuffer(cm, ptr_sbCert, secure_boot_cert_size, SSL_FILETYPE_DEFAULT);
	if (ret != WOLFSSL_SUCCESS) {
		printf("could not verify certificate.\n");
		wolfSSL_CertManagerFree(cm);
		return FAILED;
	}else{
		printf("Verification successful on secure boot cert!\n");
	}

	return SUCCEEDED;
}

/******************************************************************************/
/* \Description:
 * function to extract the public key from a given certificate
 *  returns public key in RSA key Format                                      */
/******************************************************************************/
static uint8_t public_key_Extraction(const uint8_t* ptr_Cert,uint32_t Cert_size,RsaKey *pubKey){

	int ret =0;
	WOLFSSL_X509* x509cert;
	WOLFSSL_EVP_PKEY* pubKeyTmp;
	word32 idx = 0;

	x509cert = wolfSSL_X509_load_certificate_buffer(ptr_Cert, Cert_size, WOLFSSL_FILETYPE_DEFAULT);
	if (x509cert == NULL) {
		printf("Failed to load cert, abort!\n");
		wolfSSL_EVP_PKEY_free(pubKeyTmp);
		wolfSSL_X509_free(x509cert);
		return FAILED;
	}
	printf("Loaded cert successfully\n");

	pubKeyTmp = wolfSSL_X509_get_pubkey(x509cert);
	if (pubKeyTmp == NULL) {
		printf("Failed to extract public key, abort!\n");
		wolfSSL_EVP_PKEY_free(pubKeyTmp);
		wolfSSL_X509_free(x509cert);
		return FAILED;
	}
	printf("Extracted public key successfully\n");

	/* setup a key structure to receive the extracted key */
	wc_InitRsaKey(pubKey, NULL);
	ret = wc_RsaPublicKeyDecode((byte*)pubKeyTmp->pkey.ptr, &idx, pubKey,
			(word32) pubKeyTmp->pkey_sz);
	if (ret != 0) {
		printf("Failed to decode public key from pubKeyTmp, abort!\n");
		wolfSSL_EVP_PKEY_free(pubKeyTmp);
		wolfSSL_X509_free(x509cert);
		return FAILED;
	}
	printf("Successfully decoded public key\n");
	wolfSSL_EVP_PKEY_free(pubKeyTmp);
	wolfSSL_X509_free(x509cert);
	return SUCCEEDED;
}

/******************************************************************************/
/* \Description:
 * function to compare two very long numbers                                  */
/******************************************************************************/
static uint8_t compare_keys(uint32_t *n_public_key_1,uint32_t *n_public_key_2,uint32_t size){

	for(uint32_t i=0;i<size;i++){
		if(n_public_key_1[i] != n_public_key_2[i]){
			return FAILED;
		}
	}
	return SUCCEEDED;
}

/******************************************************************************/
/* \Description:
 * function to verify the signature,
 * given the data and the signature and the public key that required for decryption
 *                                                                            */
/******************************************************************************/
static uint8_t signature_verify(const uint8_t* signature, uint32_t sig_length, const uint8_t* metaData,uint32_t metadata_lengt, RsaKey *pubKey){

	Sha256         sha256;
	Sha256*        pSha256 = NULL;

	unsigned char  digest[WC_SHA256_DIGEST_SIZE];
	unsigned char  encSig[WC_SHA256_DIGEST_SIZE + 32];
	word32         encSigLen = 0;
	unsigned char* decSig = NULL;
	word32         decSigLen = 0;
	int ret=0;

	/* Calculate SHA-256 digest of message */

	ret = wc_InitSha256(&sha256);
	if (ret == 0) {
		pSha256 = &sha256;
		ret = wc_Sha256Update(&sha256, metaData, metadata_lengt);   //size of() should be implemented but maybe -1, dep on the format of the meta data (array of hex or string)
	}else{
		return FAILED;
	}

	if (ret == 0){
		ret = wc_Sha256Final(&sha256, digest);
	}else{
		return FAILED;
	}


	/** using the hash function **/

	/* Encode digest with algorithm information as per PKCS#1.5 */
	if (ret == 0) {
		encSigLen = wc_EncodeSignature(encSig, digest, sizeof(digest), SHA256h);
		if ((int)encSigLen < 0){
			ret = (int)encSigLen;
		}
	}else{
		return FAILED;
	}


	/* Verify the signature by decrypting the value */
	if (ret == 0) {
		decSigLen = wc_RsaSSL_VerifyInline(signature, sig_length, &decSig, pubKey);
		if ((int)decSigLen < 0){
			ret = (int)decSigLen;
		}
	}else{
		return FAILED;
	}

	/* Check the decrypted result matches the encoded digest. */
	if (ret == 0 && encSigLen != decSigLen){
		/* failure */
		ret = FAILED;
	}

	if (ret == 0 && XMEMCMP(encSig, decSig, encSigLen) != 0){
		/* failure */
		ret = FAILED;
	}

	/*free*/
	if (pSha256 != NULL){
		wc_Sha256Free(pSha256);
	}

	/* Report on the verification */
	return ret;

}

/******************************************************************************/
/* \Description:
 * function to calculate the hash using SHA256 algorithm given the size
 * and the data to be hashed                                                  */
/******************************************************************************/
static uint8_t * CalculateDigest(const uint8_t* image , uint32_t image_size ){
	/* Variables Declaration */
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
	if (ret != 0){
		printf("Failed to calculate the hash\n");
	}else if (ret == 0){
		ret =  wc_Sha256Final(&sha256, hash);
	}

	if (ret != 0){
		printf("ERROR: Hash operation failed");
	}else
	{
		/*
		 * Do nothing
		 */
	}
	return hash;
}
/******************************************************************************/
/* \Description:
 * function to compare two digest values                                      */
/******************************************************************************/
static uint8_t DigestCompare ( byte *Digest , byte *MetaDigest){
	int ret = 0;
	ret = memcmp(Digest,MetaDigest,32);
	if(ret== 0){
		ret = SUCCEEDED;
	}else{
		ret = FAILED;
	}
	return ret;
}



