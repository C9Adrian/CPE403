/******************************************************************************

 @file fh_global.c

 @brief TIMAC 2.0 FH API

 Group: WCS LPC
 Target Device: CC13xx

 ******************************************************************************
 
 Copyright (c) 2016-2018, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 Release Name: simplelink_cc13x0_sdk_2_30_00_
 Release Date: 2018-09-21 11:39:45
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include "hal_types.h"
#include "fh_api.h"
#include "fh_data.h"
#include "fh_dh1cf.h"
#include "fh_ie.h"
#include "fh_mgr.h"
#include "fh_nt.h"
#include "fh_pib.h"
#include "fh_sm.h"
#include "fh_util.h"
#include "mac_data.h"

#ifdef FEATURE_FREQ_HOP_MODE
/*!
 This module is the FH module interface for the HMAC and LMAC. All function
 APIS in this file can be used by HMAC and LMAC.
 */

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/******************************************************************************
 Local variables
 *****************************************************************************/

/******************************************************************************
 Local Function Prototypes
 *****************************************************************************/

/******************************************************************************
 Glocal variables
 *****************************************************************************/
/*!
 FH_hnd is the FH module handler. It is used to store the FH internal data
 structure
 */
FH_HND_s FH_hnd;
/*!
 FH NT handle
 */
FHNT_HND_s FHNT_hnd;
/*!
 FH_txTiming
 */
FH_TX_TIMING_TABLE_s FH_txTiming =
{
 .maxNumEntry = FH_TX_TIMING_TABLE_SIZE,
};

/*!
 FH temp table
 */
FHNT_TEMP_TABLE_s FHNT_tempTable;
/*!
 FH channel Plan (fh_ie.c)
 */
FHIE_channelPlan_t FHIE_channelPlan[NO_OF_CHANNEL_PLAN] =
{
    {1, 7,  902200, 0, 129},
    {1, 8,  902400, 1, 64},
    {1, 27, 902600, 2, 42},
    {2, 1,  920600, 0, 38},
    {2, 2,  920900, 1, 18},
    {2, 3,  920800, 2, 12},
    {3, 2,  863125, 0, 34}
};

/*!
 FH NT debug
 */
FHNT_DEBUG_s FHNT_Dbg;
/*!
 FH PIB default
 */
const FHPIB_DB_t FHPIB_defaults =
{
    /*! macTrackParentEUI */
    FHPIB_DEFAULT_EUI,
    /*! macBcInterval : 4.25s */
    4250,
    /*! macUcExcludedChannels */
    {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00,
    },
    /*! macBcExcludedChannels */
    {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00,
    },
    /*! macUcDwellInterval */
    250,
    /*! macBcDwellInterval */
    250,
    /*! macClockDrift */
    20,
    /*! macTimingAccuracy */
    0x00,
    /*! macUcChannelFunction */
    0x00,
    /*! macBcChannelFunction */
    0x00,
    /*! macUseParentBSIE */
    0x00,
    /*! macBroadcastSchedId */
    0x0000,
    /*! macUcFixedChannel */
    0x0000,
    /*! macBcFixedChannel */
    0x0000,
    /*! macPanSize */
    0x0001,
    /*! macRoutingCost */
    0x00,
    /*! macRoutingMethod : 1(L3 RPL), 0(L2 MHDS)*/
    0x01,
    /*! macEapolReady : 1(ready to accept EAPOL message)*/
    0x01,
    /*! macFanTpsVersion : 1(WiSUN FAN version 1.0) */
    0x00,
    /*! macNetName */
    {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
    /*! macPanVersion */
    0x0000,
    /*! macGtk0Hash */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /*! macGtk1Hash */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /*! macGtk2Hash */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /*! macGtk3Hash */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /*! macNeighborValidTime : 120 mins*/
    120,
    /*! macBaseBackOff : 0 */
    0x08,
    /*! maximum non-sleep nodes */
    2,
    /*! maximum sleep node */
    (FHPIB_MAX_NUM_DEVICE -2),
    /*! maximum temp node */
#ifndef FEATURE_MAC_SECURITY
/*! Maximum number of temp Nodes */
    MAX_DEVICE_TABLE_ENTRIES,
#else
    /*! Maximum number of fixed Nodes */
    10,
#endif



};

/*!
 FH PIB table
 */
const FH_PibTbl_t FH_PibTbl[] =
{
    /*!< FHPIB_TRACK_PARENT_EUI */
    {offsetof(FHPIB_DB_t, macTrackParentEUI), sizeof(sAddrExt_t), 0, 0},
    /*!< FHPIB_BC_INTERVAL */
    {offsetof(FHPIB_DB_t, macBcInterval), sizeof(uint32_t), 250, 16777215},
    /*!< FHPIB_UC_EXCLUDED_CHANNELS */
    {offsetof(FHPIB_DB_t, macUcExcludedChannels),
              FHPIB_MAX_BIT_MAP_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_BC_EXCLUDED_CHANNELS */
    {offsetof(FHPIB_DB_t, macBcExcludedChannels),
              FHPIB_MAX_BIT_MAP_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_UC_DWELL_INTERVAL */
    {offsetof(FHPIB_DB_t, macUcDwellInterval), sizeof(uint8_t), 15, 250},
    /*!< FHPIB_BC_DWELL_INTERVAL */
    {offsetof(FHPIB_DB_t, macBcDwellInterval), sizeof(uint8_t), 0, 250},
    /*!< FHPIB_CLOCK_DRIFT */
    {offsetof(FHPIB_DB_t, macClockDrift), sizeof(uint8_t), 0, 255},
    /*!< FHPIB_TIMING_ACCURACY */
    {offsetof(FHPIB_DB_t, macTimingAccuracy), sizeof(uint8_t), 0, 255},
    /*!< FHPIB_UC_CHANNEL_FUNCTION */
    {offsetof(FHPIB_DB_t, macUcChannelFunction), sizeof(uint8_t), 0, 3},
    /*!< FHPIB_BC_CHANNEL_FUNCTION */
    {offsetof(FHPIB_DB_t, macBcChannelFunction), sizeof(uint8_t), 0, 3},
    /*!< FHPIB_USE_PARENT_BS_IE */
    {offsetof(FHPIB_DB_t, macUseParentBSIE), sizeof(uint8_t), 0, 1},
    /*!< FHPIB_BROCAST_SCHED_ID */
    {offsetof(FHPIB_DB_t, macBroadcastSchedId), sizeof(uint16_t), 0, 65535},
    /*!< FHPIB_UC_FIXED_CHANNEL */
    {offsetof(FHPIB_DB_t, macUcFixedChannel), sizeof(uint16_t), 0, 255},
    /*!< FHPIB_BC_FIXED_CHANNEL */
    {offsetof(FHPIB_DB_t, macBcFixedChannel), sizeof(uint16_t), 0, 255},
    /*!< FHPIB_PAN_SIZE */
    {offsetof(FHPIB_DB_t, macPanSize), sizeof(uint16_t), 0, 65535},
    /*!< FHPIB_ROUTING_COST */
    {offsetof(FHPIB_DB_t, macRoutingCost), sizeof(uint8_t), 0, 255},
    /*!< FHPIB_ROUTING_METHOD */
    {offsetof(FHPIB_DB_t, macRoutingMethod), sizeof(uint8_t), 0, 1},
    /*!< FHPIB_EAPOL_READY */
    {offsetof(FHPIB_DB_t, macEapolReady), sizeof(uint8_t), 0, 1},
    /*!< FHPIB_FAN_TPS_VERSION */
    {offsetof(FHPIB_DB_t, macFanTpsVersion), sizeof(uint8_t), 0, 7},
    /*!< FHPIB_NET_NAME */
    {offsetof(FHPIB_DB_t, macNetName),
              FHPIB_NET_NAME_SIZE_MAX*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_PAN_VERSION */
    {offsetof(FHPIB_DB_t, macPanVersion), sizeof(uint16_t), 0, 65535},
    /*!< FHPIB_GTK_0_HASH */
    {offsetof(FHPIB_DB_t, macGtk0Hash),
              FHPIB_GTK_HASH_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_GTK_1_HASH */
    {offsetof(FHPIB_DB_t, macGtk1Hash),
              FHPIB_GTK_HASH_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_GTK_2_HASH */
    {offsetof(FHPIB_DB_t, macGtk2Hash),
              FHPIB_GTK_HASH_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_GTK_3_HASH */
    {offsetof(FHPIB_DB_t, macGtk3Hash),
              FHPIB_GTK_HASH_SIZE*sizeof(uint8_t), 0, 0},
    /*!< FHPIB_HOPPING_NEIGHBOR_VALID_TIME */
    {offsetof(FHPIB_DB_t, macNeighborValidTime), sizeof(uint16_t), 5, 600},
    /*!< CSMA Base Backoff */
    {offsetof(FHPIB_DB_t, macBaseBackOff), sizeof(uint8_t), 0, 16},
    /*!< maximum non-sleep nodes */
    {offsetof(FHPIB_DB_t, macMaxNonSleepNodes), sizeof(uint16_t), 0, 50},
    /*!< maximum sleep node */
    {offsetof(FHPIB_DB_t, macMaxSleepNodes), sizeof(uint16_t), 0, 50},
    /*!< maximum temp table node */
    {offsetof(FHPIB_DB_t, macMaxTempNodes), sizeof(uint16_t), 0, 10},

};

/*!
 FH PIB database
 */
FHPIB_DB_t FHPIB_db;

FHNT_OPT_Table_s FHNT_tableOpt;

const FSM_actionCell_t FH_stateMatrix[FH_ST_NUM_STATES][FH_EVT_NUM_EVENTS] =
{
    /* HOP state */
    {
         /*! UC_TIMER_EVT */
         {FH_ST_HOP,                     FHMGR_updateRadioUCChannel},
         /*! BC_TIMER_EVT */
         {FH_ST_HOP,                     FHMGR_updateRadioBCChannel},
         /*! RX_START from LMAC */
         {FH_ST_RX,                      FHUTIL_noAction},
         /*! RX_DONE */
         {FH_ST_HOP,                     FHUTIL_assert},
         /*! TX_START */
         {FH_ST_TX,                      FHUTIL_noAction},
         /*! TX_DONE */
         {FH_ST_HOP,                     FHUTIL_assert},
         /*! ASYNC START */
         {FH_ST_ASYNC,                   FHDATA_procASYNCReq},
         /*! ASYNC DONE/STOP */
         {FH_ST_HOP,                     FHUTIL_assert},
         /*! Req frame type and parsed FCIE */
         {FH_ST_EDFE,                    FHDATA_startEDFEReq},
         /*! EDFE TIMER EXP/DONE */
         {FH_ST_HOP,                     FHUTIL_assert},
    },

    /* TX state */
    {
         /*! UC_TIMER_EVT */
         {FH_ST_TX,                      FHMGR_pendUcChUpdate},
         /*! BC_TIMER_EVT */
         {FH_ST_TX,                      FHMGR_pendBcChUpdate},
         /*! RX_START */
         {FH_ST_TX,                      FHUTIL_assert},
         /*! RX_DONE  */
         {FH_ST_TX,                      FHUTIL_assert},
         /*! TX_START */
         {FH_ST_TX,                      FHUTIL_assert},
         /*! TX_DONE  */
         {FH_ST_HOP,                     FHMGR_updateHopping},
         /*! ASYNC_START */
         {FH_ST_TX,                      FHUTIL_assert},
         /*! ASYNC DONE/STOP */
         {FH_ST_TX,                      FHUTIL_assert},
         /*! EDFE from higher layer/Poll Req */
         {FH_ST_TX,                      FHUTIL_assert,},
         /*! EDFE TIMER EXP */
         {FH_ST_TX,                      FHUTIL_assert},
    },

    /* RX state */
    {
         /*! UC_TIMER_EVT */
         {FH_ST_RX,                      FHMGR_pendUcChUpdate},
         /*! BC_TIMER_EVT */
         {FH_ST_RX,                      FHMGR_pendBcChUpdate},
         /*! RX_START */
         {FH_ST_RX,                      FHUTIL_assert},
         /*! RX_DONE */
         {FH_ST_HOP,                     FHMGR_updateHopping},
         /*! TX_START */
         {FH_ST_RX,                      FHUTIL_assert},
         /*! TX_DONE */
         {FH_ST_RX,                      FHUTIL_assert},
         /*! ASYNC START */
         {FH_ST_RX,                      FHDATA_requeuePkt},
         /*! ASYNC DONE/STOP */
         {FH_ST_RX,                      FHUTIL_assert},
         /*! EDFE START based on RX frame FCIE, higher layer control? */
         {FH_ST_EDFE,                    FHDATA_startEDFETimer},
         /*! EDFE TIMER EXP */
         {FH_ST_HOP,                     FHUTIL_assert},
    },

    /* ASYNC  state */
    {
         /*! UC_TIMER_EVT */
         {FH_ST_ASYNC,                   FHMGR_pendUcChUpdate},
         /*! BC_TIMER_EVT */
         {FH_ST_ASYNC,                   FHMGR_pendBcChUpdate},
         /*! RX_START */
         {FH_ST_ASYNC,                   FHUTIL_noAction},
         /*! RX_DONE   */
         {FH_ST_ASYNC,                   FHUTIL_noAction},
         /*! TX_START */
         {FH_ST_ASYNC,                   FHUTIL_assert},
         /*! TX_DONE */
         {FH_ST_ASYNC,                   FHDATA_procASYNCReq},
         /*! ASYNC START */
         {FH_ST_ASYNC,                   FHUTIL_assert},
         /*! ASYNC DONE/STOP */
         {FH_ST_HOP,                     FHMGR_updateHopping},
         /*! EDFE START based on RX frame FCIE, higher layer control */
         {FH_ST_ASYNC,                   FHUTIL_assert},
         /*! EDFE TIMER EXP */
         {FH_ST_ASYNC,                   FHUTIL_assert},
    },
    /* EDFE   state */
    {
         /*! UC_TIMER_EVT */
         {FH_ST_EDFE,                    FHMGR_pendUcChUpdate},
         /*! BC_TIMER_EVT */
         {FH_ST_EDFE,                    FHMGR_pendBcChUpdate},
         /*! RX_START */
         {FH_ST_EDFE,                    FHUTIL_noAction},
         /*! RX_DONE */
         {FH_ST_EDFE,                    FHUTIL_noAction},
         /*! TX_START */
         {FH_ST_EDFE,                    FHUTIL_noAction},
         /*! TX_DONE  */
         {FH_ST_EDFE,                    FHDATA_startEDFETimer},
         /*! ASYNC START  //based on frame type from HMAC */
         {FH_ST_EDFE,                    FHUTIL_assert},
         /*! ASYNC DONE/STOP */
         {FH_ST_EDFE,                    FHUTIL_assert},
         /*! EDFE START based on RX frame FCIE, higher layer control */
         {FH_ST_EDFE,                    FHDATA_procEDFEReq},
         /*! EDFE TIMER EXP */
         {FH_ST_HOP,                     FHMGR_updateHopping},
    },
};
FHFSM_debugBuffer_t FHFSM_Dbg;

/*****************************************************************************/
/* CSMA SM State machine                                                     */
/*****************************************************************************/
FSM_stateMachine_t FH_FSM =
{
    &FH_stateMatrix[0][0],
    FH_ST_NUM_STATES,
    FH_EVT_NUM_EVENTS,

    FH_ST_HOP,  /* prev state */
    FH_ST_HOP,  /* current state */
    /* for debug */
    FSM_transaction_debug
};

/*!
 FH_Dbg is the FH module debug handler. It is used to store the FH debug
 information
 */
#ifdef FH_HOP_DEBUG
FH_DEBUG_s FH_Dbg;
#endif

uint8 macAddrSize[4] = {0, 0, 2, 8};

const uint8  fhSupFrameTypes[FH_SUPPORTED_FRAME_TYPES] = {MAC_FRAME_TYPE_ASYNC, MAC_FRAME_TYPE_EDFE, MAC_FRAME_TYPE_BROADCAST,
                                                      MAC_FRAME_TYPE_UNICAST, MAC_INTERNAL_ASSOC_REQ, MAC_INTERNAL_ASSOC_RSP,
                                                      MAC_INTERNAL_DATA_REQ, MAC_INTERNAL_DISASSOC_NOTIF};

#endif
