/*******************************************************************************

  Eurecom OpenAirInterface
  Copyright(c) 1999 - 2010 Eurecom

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information
  Openair Admin: openair_admin@eurecom.fr
  Openair Tech : openair_tech@eurecom.fr
  Forums       : http://forums.eurecom.fsr/openairinterface
  Address      : Eurecom, 2229, route des crêtes, 06560 Valbonne Sophia Antipolis, France

 *******************************************************************************/
/*! \file eNB_scheduler.c
 * \brief procedures related to UE
 * \author  Navid Nikaein and Raymond Knopp
 * \date 2010 - 2014
 * \email: navid.nikaein@eurecom.fr
 * \version 0.5
 * @ingroup _mac

 */

#include "assertions.h"
#include "PHY/defs.h"
#include "PHY/extern.h"

#include "SCHED/defs.h"
#include "SCHED/extern.h"

#include "LAYER2/MAC/defs.h"
#include "LAYER2/MAC/extern.h"

#include "LAYER2/MAC/proto.h"
#include "UTIL/LOG/log.h"
#include "UTIL/LOG/vcd_signal_dumper.h"
#include "UTIL/OPT/opt.h"
#include "OCG.h"
#include "OCG_extern.h"
#include "ARCH/CBMIMO1/DEVICE_DRIVER/extern.h"
#include "ARCH/CBMIMO1/DEVICE_DRIVER/defs.h"
#include "ARCH/CBMIMO1/DEVICE_DRIVER/from_grlib_softregs.h"

#include "RRC/LITE/extern.h"
#include "RRC/L2_INTERFACE/openair_rrc_L2_interface.h"

//#include "LAYER2/MAC/pre_processor.c"
#include "pdcp.h"

#if defined(ENABLE_ITTI)
# include "intertask_interface.h"
#endif

#define ENABLE_MAC_PAYLOAD_DEBUG
#define DEBUG_eNB_SCHEDULER 1
//#define DEBUG_HEADER_PARSING 1
//#define DEBUG_PACKET_TRACE 1

/*
  #ifndef USER_MODE
  #define msg debug_msg
  #endif
 */




void eNB_dlsch_ulsch_scheduler(module_id_t module_idP,uint8_t cooperation_flag, frame_t frameP, sub_frame_t subframeP) {//, int calibration_flag) {

  start_meas(&eNB_mac_inst[module_idP].eNB_scheduler);
  unsigned char nprb=0;
  unsigned int nCCE=0;
  int mbsfn_status=0;
  uint32_t RBalloc=0;
#ifdef EXMIMO
  int ret;
#endif
#if defined(ENABLE_ITTI)
  MessageDef   *msg_p;
  const char   *msg_name;
  instance_t    instance;
  int           result;
#endif

  DCI_PDU *DCI_pdu= &eNB_mac_inst[module_idP].DCI_pdu;
  //  LOG_D(MAC,"[eNB %d] Frame %d, Subframe %d, entering MAC scheduler\n",module_idP, frameP, subframeP);

  vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_ENB_DLSCH_ULSCH_SCHEDULER,1);

#if defined(ENABLE_ITTI)
  do {
      // Checks if a message has been sent to MAC sub-task
      itti_poll_msg (TASK_MAC_ENB, &msg_p);

      if (msg_p != NULL) {
          msg_name = ITTI_MSG_NAME (msg_p);
          instance = ITTI_MSG_INSTANCE (msg_p);

          switch (ITTI_MSG_ID(msg_p)) {
          case MESSAGE_TEST:
            LOG_D(MAC, "Received %s\n", ITTI_MSG_NAME(msg_p));
            break;

          case RRC_MAC_BCCH_DATA_REQ:
            LOG_D(MAC, "Received %s from %s: instance %d, frameP %d, eNB_index %d\n",
                msg_name, ITTI_MSG_ORIGIN_NAME(msg_p), instance,
                RRC_MAC_BCCH_DATA_REQ (msg_p).frame, RRC_MAC_BCCH_DATA_REQ (msg_p).enb_index);

            // TODO process BCCH data req.
            break;

          case RRC_MAC_CCCH_DATA_REQ:
            LOG_D(MAC, "Received %s from %s: instance %d, frameP %d, eNB_index %d\n",
                msg_name, ITTI_MSG_ORIGIN_NAME(msg_p), instance,
                RRC_MAC_CCCH_DATA_REQ (msg_p).frame, RRC_MAC_CCCH_DATA_REQ (msg_p).enb_index);

            // TODO process CCCH data req.
            break;

#ifdef Rel10
          case RRC_MAC_MCCH_DATA_REQ:
            LOG_D(MAC, "Received %s from %s: instance %d, frameP %d, eNB_index %d, mbsfn_sync_area %d\n",
                msg_name, ITTI_MSG_ORIGIN_NAME(msg_p), instance,
                RRC_MAC_MCCH_DATA_REQ (msg_p).frame, RRC_MAC_MCCH_DATA_REQ (msg_p).enb_index, RRC_MAC_MCCH_DATA_REQ (msg_p).mbsfn_sync_area);

            // TODO process MCCH data req.
            break;
#endif

          default:
            LOG_E(MAC, "Received unexpected message %s\n", msg_name);
            break;
          }

          result = itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
          AssertFatal (result == EXIT_SUCCESS, "Failed to free memory (%d)!\n", result);
      }
  } while(msg_p != NULL);
#endif

  // clear DCI and BCCH contents before scheduling
  DCI_pdu->Num_common_dci  = 0;
  DCI_pdu->Num_ue_spec_dci = 0;
  eNB_mac_inst[module_idP].bcch_active = 0;

#ifdef Rel10
  eNB_mac_inst[module_idP].mcch_active =0;
#endif

  eNB_mac_inst[module_idP].frame    = frameP;
  eNB_mac_inst[module_idP].subframe = subframeP;

  //if (subframeP%5 == 0)
#ifdef EXMIMO
  pdcp_run(frameP, 1, 0, module_idP);
#endif
#ifdef CELLULAR
  rrc_rx_tx(module_idP, frameP, 0, 0);
#else
  // check HO
  rrc_rx_tx(module_idP,
      frameP,
      1,
      module_idP);
#endif

#ifdef Rel10
  if (eNB_mac_inst[module_idP].MBMS_flag >0) {
    start_meas(&eNB_mac_inst[module_idP].schedule_mch);
      mbsfn_status = schedule_MBMS(module_idP,frameP,subframeP);
    stop_meas(&eNB_mac_inst[module_idP].schedule_mch);
  }
#endif

  switch (subframeP) {
  case 0:
    // FDD/TDD Schedule Downlink RA transmissions (RA response, Msg4 Contention resolution)
    // Schedule ULSCH for FDD or subframeP 4 (TDD config 0,3,6)
    // Schedule Normal DLSCH

    schedule_RA(module_idP,frameP,subframeP,2,&nprb,&nCCE);

    if (mac_xface->lte_frame_parms->frame_type == FDD) {  //FDD
        schedule_ulsch(module_idP,frameP,cooperation_flag,0,4,&nCCE);//,calibration_flag);
    }
    else if  ((mac_xface->lte_frame_parms->tdd_config == TDD) || //TDD
        (mac_xface->lte_frame_parms->tdd_config == 3) ||
        (mac_xface->lte_frame_parms->tdd_config == 6))
      //schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,4,&nCCE);//,calibration_flag);


      // schedule_ue_spec(module_idP,subframeP,nprb,&nCCE,mbsfn_status);

      fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,1,mbsfn_status);
    break;

  case 1:
    // TDD, schedule UL for subframeP 7 (TDD config 0,1) / subframeP 8 (TDD Config 6)
    // FDD, schedule normal UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == TDD) { // TDD
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 0:
        case 1:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,7,&nCCE);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        case 6:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,8,&nCCE);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else {  //FDD
        schedule_ulsch(module_idP,frameP,cooperation_flag,1,5,&nCCE);
        // schedule_ue_spec(module_idP,subframeP,nprb,&nCCE,mbsfn_status);
        // fill_DLSCH_dci(module_idP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 2:
    // TDD, nothing
    // FDD, normal UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == FDD) {  //FDD
        schedule_ulsch(module_idP,frameP,cooperation_flag,2,6,&nCCE);
        // schedule_ue_spec(module_idP,subframeP,nprb,&nCCE,mbsfn_status);
        // fill_DLSCH_dci(module_idP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 3:
    // TDD Config 2, ULSCH for subframeP 7
    // TDD Config 2/5 normal DLSCH
    // FDD, normal UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == TDD) {
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 2:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,7,&nCCE);
        case 5:
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else { //FDD
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,3,7,&nCCE);
        // schedule_ue_spec(module_idP,subframeP,0,0,mbsfn_status);
        // fill_DLSCH_dci(module_idP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 4:
    // TDD Config 1, ULSCH for subframeP 8
    // TDD Config 1/2/4/5 DLSCH
    // FDD UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == 1) { // TDD
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 1:
          //        schedule_RA(module_idP,frameP,subframeP,&nprb,&nCCE);
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,8,&nCCE);
        case 2:
        case 4:
        case 5:
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,1,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else {
        if (mac_xface->lte_frame_parms->frame_type == FDD) {  //FDD
            schedule_RA(module_idP, frameP, subframeP, 0, &nprb, &nCCE);
            //	schedule_ulsch(module_idP, frameP, cooperation_flag, 4, 8, &nCCE);
            //schedule_ue_spec(module_idP, frameP, subframeP, nprb, &nCCE, mbsfn_status);
            fill_DLSCH_dci(module_idP, frameP, subframeP, RBalloc, 1, mbsfn_status);

        }
    }
    break;

  case 5:
    // TDD/FDD Schedule SI
    // TDD Config 0,6 ULSCH for subframes 9,3 resp.
    // TDD normal DLSCH
    // FDD normal UL/DLSCH
    schedule_SI(module_idP,frameP,&nprb,&nCCE);
    //schedule_RA(module_idP,frameP,subframeP,5,&nprb,&nCCE);
    if ((mac_xface->lte_frame_parms->frame_type == FDD) ) {
        //      schedule_RA(module_idP,frameP,subframeP,1,&nprb,&nCCE);
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,5,9,&nCCE);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);

    }
    else if ((mac_xface->lte_frame_parms->tdd_config == 0) || // TDD Config 0
        (mac_xface->lte_frame_parms->tdd_config == 6)) { // TDD Config 6
        //schedule_ulsch(module_idP,cooperation_flag,subframeP,&nCCE);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    else {
        //schedule_ue_spec(module_idP,subframeP,nprb,&nCCE,mbsfn_status);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 6:
    // TDD Config 0,1,6 ULSCH for subframes 2,3
    // TDD Config 3,4,5 Normal DLSCH
    // FDD normal ULSCH/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == TDD) { // TDD
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 0:
          break;
        case 1:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,2,&nCCE);
          //	schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        case 6:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,3,&nCCE);
          //	schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        case 5:
          schedule_RA(module_idP,frameP,subframeP,2,&nprb,&nCCE);
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,1,mbsfn_status);
          break;
        case 3:
        case 4:
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;

        default:
          break;
        }
    }
    else {  //FDD
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,6,0,&nCCE);
        schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 7:
    // TDD Config 3,4,5 Normal DLSCH
    // FDD Normal UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == TDD) { // TDD
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 3:
        case 4:
          //	  schedule_RA(module_idP,frameP,subframeP,3,&nprb,&nCCE);  // 3 = Msg3 subframeP, not
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status); //1,mbsfn_status);
          break;
        case 5:
          schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else {  //FDD
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,7,1,&nCCE);
        schedule_ue_spec(module_idP,frameP,subframeP,nprb,&nCCE,mbsfn_status);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 8:
    // TDD Config 2,3,4,5 ULSCH for subframeP 2
    //
    // FDD Normal UL/DLSCH
    if (mac_xface->lte_frame_parms->frame_type == TDD) { // TDD
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 2:
        case 3:
        case 4:
        case 5:

          //	schedule_RA(module_idP,subframeP,&nprb,&nCCE);
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,2,&nCCE);
          schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else {  //FDD
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,8,2,&nCCE);
        schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  case 9:
    // TDD Config 1,3,4,6 ULSCH for subframes 3,3,3,4
    if (mac_xface->lte_frame_parms->frame_type == TDD) {
        switch (mac_xface->lte_frame_parms->tdd_config) {
        case 1:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,3,&nCCE);
          schedule_RA(module_idP,frameP,subframeP,7,&nprb,&nCCE);  // 7 = Msg3 subframeP, not
          schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,1,mbsfn_status);
          break;
        case 3:
        case 4:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,3,&nCCE);
          schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        case 6:
          schedule_ulsch(module_idP,frameP,cooperation_flag,subframeP,4,&nCCE);
          //schedule_RA(module_idP,frameP,subframeP,&nprb,&nCCE);
          schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        case 2:
        case 5:
          //schedule_RA(module_idP,frameP,subframeP,&nprb,&nCCE);
          schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
          fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
          break;
        default:
          break;
        }
    }
    else {  //FDD
        //      schedule_ulsch(module_idP,frameP,cooperation_flag,9,3,&nCCE);
        schedule_ue_spec(module_idP,frameP,subframeP,0,&nCCE,mbsfn_status);
        fill_DLSCH_dci(module_idP,frameP,subframeP,RBalloc,0,mbsfn_status);
    }
    break;

  }

  DCI_pdu->nCCE = nCCE;
  LOG_D(MAC,"frameP %d, subframeP %d nCCE %d\n",frameP,subframeP,nCCE);

  vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_ENB_DLSCH_ULSCH_SCHEDULER,0);
  stop_meas(&eNB_mac_inst[module_idP].eNB_scheduler);

}



