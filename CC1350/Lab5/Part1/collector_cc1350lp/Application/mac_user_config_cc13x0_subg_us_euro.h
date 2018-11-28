/******************************************************************************

 @file mac_user_config_cc13x0_subg_us_euro.h

 @brief override and power table

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
#ifndef _CONFIG_CC13X0_SUBG_US_EURO_H
#define _CONFIG_CC13X0_SUBG_US_EURO_H

/******************************************************************************
 Includes
 *****************************************************************************/
#include "mac_user_config_cc13x0_rftable.h"

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/******************************************************************************
 External Variables
 *****************************************************************************/

/******************************************************************************
 Global Variables
 *****************************************************************************/

#ifndef SL_LRM /* regular mode */
//
// RF patch pointers
//
RF_Mode rfPatchTable =
{
#ifdef RF_MULTI_MODE
  .rfMode = RF_MODE_MULTIPLE,
#else
  .rfMode = RF_MODE_PROPRIETARY_SUB_1,
#endif
  .cpePatchFxn = &rf_patch_cpe_genfsk,
  .mcePatchFxn = 0,
  .rfePatchFxn = &rf_patch_rfe_genfsk,
};
// Overrides for CMD_PROP_RADIO_DIV_SETUP
#if !CONFIG_RANGE_EXT_MODE
// US-EU Band, GFSK, no CC1190
const uint32_t *pOverridesTable[] =
    { (uint32_t *) pOverrides_fsk_CC1350,           /* NO RANGE EXTENDER */
      (uint32_t *) pOverrides_fsk_CC1350 };         /* RANGE EXTENDER */
#else
// US-EU Band, GFSK, CC1190
const uint32_t *pOverridesTable[] =
    { (uint32_t *) pOverrides_fsk_CC1310_CC1190,           /* NO RANGE EXTENDER */
      (uint32_t *) pOverrides_fsk_CC1310_CC1190 };         /* RANGE EXTENDER */
#endif
#else /* long range mode */
RF_Mode rfPatchTable =
{
#ifdef RF_MULTI_MODE
  .rfMode = RF_MODE_MULTIPLE,
#else
  .rfMode = RF_MODE_PROPRIETARY_SUB_1,
#endif
  .cpePatchFxn = &rf_patch_cpe_sl_longrange,
  .mcePatchFxn = &rf_patch_mce_sl_longrange,
  .rfePatchFxn = &rf_patch_rfe_sl_longrange,
};

// Overrides for CMD_PROP_RADIO_DIV_SETUP
#if !CONFIG_RANGE_EXT_MODE
// US-EU Band, SLRM, no CC1190
const uint32_t *pOverridesTable[] =
    { (uint32_t *) pOverrides_slr_CC1350,           /* NO RANGE EXTENDER */
      (uint32_t *) pOverrides_slr_CC1350 };         /* RANGE EXTENDER */
#else
// US-EU Band, SLRM, CC1190
const uint32_t *pOverridesTable[] =
    { (uint32_t *) pOverrides_slr_CC1310_CC1190,           /* NO RANGE EXTENDER */
      (uint32_t *) pOverrides_slr_CC1310_CC1190 };         /* RANGE EXTENDER */
#endif
#endif
// Tx Power Values (Pout, TC, GC, IB)
// 1st column is for CC1310 and 2nd column is for CC1350
// This table can apply to both EM and Launch Pad
#if !CONFIG_RANGE_EXT_MODE
// Tx Power Table
const macTxPwrTbl_t macTxPwrTbl[] =
    { { txPowerTable_subg_CC1310 },    /* power table for CC1310 */
      { txPowerTable_subg_CC1350 },    /* power table for CC1350 */
      { txPowerTable_subg_CC1350 } };  /* power table for CC1310-CC1190 */
#else
// Tx Power Table
#if defined(US_BAND_1) || defined(US_BAND_3)
const macTxPwrTbl_t macTxPwrTbl[] =
    { { txPowerTable_subg_US_CC1310_CC1190 },    /* power table for CC1310 */
      { txPowerTable_subg_US_CC1310_CC1190 },    /* power table for CC1350 */
      { txPowerTable_subg_US_CC1310_CC1190 } };  /* power table for CC1310-CC1190 */
#else
const macTxPwrTbl_t macTxPwrTbl[] =
    { { txPowerTable_subg_EU_CC1310_CC1190 },    /* power table for CC1310 */
      { txPowerTable_subg_EU_CC1310_CC1190 },    /* power table for CC1350 */
      { txPowerTable_subg_EU_CC1310_CC1190 } };  /* power table for CC1310-CC1190 */
#endif
#endif

const uint8_t loDivider = MAC_LO_DIVIDER_5;

#endif
