/******************************************************************************

 @file  mac_user_config.c

 @brief User configurable variables for the TIMAC radio.

 Group: WCS LPC
 Target Device: CC13xx

 ******************************************************************************
 
 Copyright (c) 2016-2018, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc13x0_sdk_2_30_00_
 Release Date: 2018-09-21 11:39:46
 *****************************************************************************/

/******************************************************************************
 * INCLUDES
 */
#if defined(CC13XX_LAUNCHXL)
#include "CC13X0_LAUNCHXL/Board.h"
#elif defined(CC26XX_LAUNCHXL)
#include "CC2650_LAUNCHXL/Board.h"
#elif defined(CC13X2R1_LAUNCHXL) && !defined(Board_CC1352R1_LAUNCHXL)
#include "CC1352R1_LAUNCHXL/Board.h"
#elif defined(Board_CC1352R1_LAUNCHXL)
#include "Board.h"
#else
/* This is only other platform to be supported */
#include "CC1310DK_7XD/Board.h"
#endif
#include "mac_user_config.h"
#if !defined(CC13X2R1_LAUNCHXL) && !defined(CC26X2R1_LAUNCHXL)
#include <ti/drivers/crypto/CryptoCC26XX.h>
#else
#include <ti/drivers/AESCCM.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>
#endif
/******************************************************************************
 * MACROS
 */

/******************************************************************************
 * CONSTANTS
 */

/******************************************************************************
 * TYPEDEFS
 */

/******************************************************************************
 * LOCAL VARIABLES
 */

/******************************************************************************
 * GLOBAL VARIABLES
 */
/* CC13XX_LAUNCH */
#if defined (CC13XX_LAUNCHXL)
//
// Tx Power Table Used Depends on Device Package
//
#if defined( COPROCESSOR )
#include "mac_user_config_cc13x0_subg_cop.h"
#else // other projects
#if defined( US_BAND_1 ) || defined( ETSI_BAND )
#include "mac_user_config_cc13x0_subg_us_euro.h"
#elif defined( US_BAND_3 ) || defined( ETSI_BAND_2 )
#include "mac_user_config_cc13x0_subg_us_euro_200k.h"
#elif defined( CHINA_BAND )
#include "mac_user_config_cc13x0_subg_china.h"
#else /* phy id is something else not defined */
#error "***MAC USER CONFIG BUILD ERROR*** Unknown phy id!"
#endif /* end of else */
#endif /* end of defined( COPROCESSOR ) */

/* CC13X2_LAUNCH */
#else
#if defined( US_BAND_1 ) || defined( ETSI_BAND )
#include "mac_user_config_cc13x2r1_subg_us_euro.h"
#elif defined( CHINA_BAND )
#include "mac_user_config_cc13x2r1_subg_china.h"
#else /* phy id is something else not defined */
#error "***MAC USER CONFIG BUILD ERROR*** Unknown phy id!"
#endif /* end of defined( US_BAND_1 ) || defined( ETSI_BAND ) */
#endif /* end of else */

// RF Driver API Table
const uint32_t macRfDriverTable[] =
  { (uint32_t) RF_open,
    (uint32_t) RF_close,
    (uint32_t) NULL,
	(uint32_t) NULL,
	(uint32_t) NULL,
    (uint32_t) RF_cancelCmd,
    (uint32_t) RF_flushCmd,
    (uint32_t) RF_yield,
    (uint32_t) RF_Params_init,
    (uint32_t) RF_runImmediateCmd,
    (uint32_t) RF_runDirectCmd,
    (uint32_t) RF_ratCompare,
	(uint32_t) NULL,
	(uint32_t) RF_RatConfigOutput_init,
    (uint32_t) RF_ratDisableChannel,
    (uint32_t) RF_getCurrentTime,
    (uint32_t) RF_getRssi,
    (uint32_t) RF_getInfo,
    (uint32_t) RF_getCmdOp,
    (uint32_t) RF_control,
    (uint32_t) RF_scheduleCmd,
    (uint32_t) RF_getTxPower,
    (uint32_t) RF_setTxPower,
    (uint32_t) NULL,
    (uint32_t) RF_TxPowerTable_findValue,
    (uint32_t) RF_ScheduleCmdParams_init,
    (uint32_t) RF_requestAccess };

// Crypto Driver API Table
#if !defined(CC13X2R1_LAUNCHXL) && !defined(CC26X2R1_LAUNCHXL)
const uint32_t macCryptoDriverTable[] =
  { (uint32_t) CryptoCC26XX_close,
    (uint32_t) CryptoCC26XX_init,
    (uint32_t) CryptoCC26XX_open,
    (uint32_t) CryptoCC26XX_Params_init,
    (uint32_t) CryptoCC26XX_Transac_init,
    (uint32_t) CryptoCC26XX_allocateKey,
    (uint32_t) CryptoCC26XX_releaseKey,
    (uint32_t) CryptoCC26XX_transact,
    (uint32_t) CryptoCC26XX_transactPolling,
    (uint32_t) CryptoCC26XX_transactCallback };
#else
const uint32_t macCryptoDriverTable[] =
  {   (uint32_t) AESCCM_init,
      (uint32_t) AESCCM_open,
      (uint32_t) AESCCM_close,
      (uint32_t) AESCCM_Params_init,
      (uint32_t) AESCCM_Operation_init,
      (uint32_t) AESCCM_oneStepEncrypt,
      (uint32_t) AESCCM_oneStepDecrypt,
      (uint32_t) NULL, //AESECB_init
      (uint32_t) NULL, //AESECB_open
      (uint32_t) NULL, //AESECB_close
      (uint32_t) NULL, //AESECB_Params_init
      (uint32_t) NULL, //AESECB_Operation_init
      (uint32_t) NULL, //AESECB_oneStepEncrypt
      (uint32_t) NULL, //AESECB_oneStepDecrypt
      (uint32_t) CryptoKeyPlaintext_initKey,
      (uint32_t) CryptoKeyPlaintext_initBlankKey
  };
#endif
/******************************************************************************
 */

