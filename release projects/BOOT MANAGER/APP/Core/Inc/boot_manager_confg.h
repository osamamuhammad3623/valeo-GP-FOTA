
/******************************************************************************
  * @file    boot_manager_conf.h
  * @author  BOOT MANAGER Team
  * @brief   Configuration file for boot manager Application Program.
 *******************************************************************************/

#ifndef BOOT_MANAGER_CONFG_H_
#define BOOT_MANAGER_CONFG_H_

/******************************************************************************/
/******************************************************************************/
/******************************** Definitions *********************************/
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* \Description: Addresses for data in flash memory
 * boot manger from sector 0 to sector 4
 * sector 6 for certificates , meta-data, signature
 * Main Application at sector 7
 * Addresses:
 * root certificates ->         0x08040000
 * secure boot certificates ->  0x08040800
 * Application Meta data ->     0x08041000
 * Signature ->                 0x08041400
 * certificate meta data->      0x08041602
 * APPLICATION                  0x08060000       */
/******************************************************************************/

//2k for each certificate , 1k for meta-data , 1k for signature then application
#define APPLICATION_EXENSIONS_ADDRESS    0x08040000

#define ROOT_CERT_ADDRESS 				 APPLICATION_EXENSIONS_ADDRESS
#define SB_CERT_ADDRESS 			    (APPLICATION_EXENSIONS_ADDRESS + 0x0800)
#define META_DATA_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01000)
#define SIGNATURE_ADDRESS               (APPLICATION_EXENSIONS_ADDRESS + 0x01400)
#define CERTIFICATES_METADATA_ADDRESS   (APPLICATION_EXENSIONS_ADDRESS + 0x01602)

#define MAIN_APPLICATION_START_ADDRESS  (0x08060000)   //128k byte offset from Application_extensions_Address
/******************************************************************************/
/* \Description: address of the version threshold                             */
/******************************************************************************/
//#define THRESHOLD_ADDRESS               (0x1fff7840)

/******************************************************************************/
/* \Description: Revocation blocks for root index 1 and 2 at OIP memory
 * Root 1 revocation -> first byte at Block 0
 * Root 2 revocation -> first byte at Block 1
 *                         */
/******************************************************************************/

#define ROOT_1_REVOCATION_reg  (0x4)
#define ROOT_2_REVOCATION_reg  (0x5)


/******************************************************************************
 * \Description: length of the fixed length data
 * Signature, application meta data                                           */
/******************************************************************************/
#define META_DATA_LENGTH 82
#define SIGNATURE_LENGTH  256
/******************************************************************************/
/* \Description: offset of specific data fields                               */
/******************************************************************************/
//related to Application Metadata
#define MAJOR_V_METADATA_OFFSET           8
#define MINOR_V_METADATA_OFFSET           10
#define PATCH_V_METADATA_OFFSET           12
#define APP_SIZE__METADATA_OFFSET         20
#define ROOT_INDEX__METADATA_OFFSET       38
#define APP_HASH__METADATA_OFFSET         50
//related to Certificates Metadata
#define ROOT_CERT_SIZE_C_METADATA_OFFSET  15
#define SB_CERT_SIZE_C_METADATA_OFFSET    34
/******************************************************************************/
/* \Description: State values                                                 */
/******************************************************************************/
#define SUCCEEDED 0x00
#define FAILED  0x01

#endif /* BOOT_MANGER_CONFG_H_ */
