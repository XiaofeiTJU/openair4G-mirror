#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "SIMULATION/TOOLS/defs.h"
#include "SIMULATION/RF/defs.h"
#include "PHY/types.h"
#include "PHY/defs.h"
#include "oaisim_config.h"
#include "init_lte.h"

#ifdef OPENAIR2
#include "LAYER2/MAC/defs.h"
#include "UTIL/OMV/structures.h"
#endif

eNB_MAC_INST* get_eNB_mac_inst(void);
OAI_Emulation* get_OAI_emulation(void);
void init_channel_vars(LTE_DL_FRAME_PARMS *frame_parms, double ***s_re,double ***s_im,double ***r_re,double ***r_im,double ***r_re0,double ***r_im0);

void do_UL_sig(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *UE2eNB[NUMBER_OF_UE_MAX][NUMBER_OF_eNB_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],uint16_t next_slot,uint8_t abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms, uint32_t);

void do_DL_sig(double **r_re0,double **r_im0,double **r_re,double **r_im,double **s_re,double **s_im,channel_desc_t *eNB2UE[NUMBER_OF_eNB_MAX][NUMBER_OF_UE_MAX],node_desc_t *enb_data[NUMBER_OF_eNB_MAX],node_desc_t *ue_data[NUMBER_OF_UE_MAX],uint16_t next_slot,uint8_t abstraction_flag,LTE_DL_FRAME_PARMS *frame_parms,uint8_t UE_id);

void init_ue(node_desc_t  *ue_data, UE_Antenna ue_ant);//Abstraction changes
void init_enb(node_desc_t  *enb_data, eNB_Antenna enb_ant);//Abstraction changes
void extract_position(node_list* input_node_list, node_desc_t**, int nb_nodes);//Abstraction changes
void get_beta_map(void);//Abstraction changes
void get_MIESM_param(void);

void init_snr(channel_desc_t *,  node_desc_t *, node_desc_t *, double*, double*, uint8_t, uint16_t, uint8_t);//Abstraction changes
void init_snr_up(channel_desc_t *,  node_desc_t *, node_desc_t *, double*, double*, uint16_t, uint16_t);//Abstraction changes
void calculate_sinr(channel_desc_t *,  node_desc_t *, node_desc_t *, double *sinr_dB);//Abstraction changes
void get_beta_map(void); 
int dlsch_abstraction_EESM(double* sinr_dB, uint32_t rb_alloc[4], uint8_t mcs, uint8_t); //temporary testing for PHY abstraction
int dlsch_abstraction_MIESM(double* sinr_dB,uint8_t TM, uint32_t rb_alloc[4], uint8_t mcs,uint8_t);
int ulsch_abstraction_MIESM(double* sinr_dB,uint8_t TM, uint8_t mcs,uint16_t nb_rb, uint16_t first_rb);
int ulsch_abstraction(double* sinr_dB,uint8_t TM, uint8_t mcs,uint16_t nb_rb, uint16_t first_rb);

void calc_path_loss(node_desc_t* node_tx, node_desc_t* node_rx, channel_desc_t *ch_desc, Environment_System_Config env_desc, double **SF);

void do_OFDM_mod(mod_sym_t **txdataF, int32_t **txdata, frame_t frame, uint16_t next_slot, LTE_DL_FRAME_PARMS *frame_parms);

void reset_opp_meas(void);
void print_opp_meas(void);

#ifdef OPENAIR2
int omv_write (int pfd,  node_list* enb_node_list, node_list* ue_node_list, Data_Flow_Unit omv_data);
void omv_end (int pfd, Data_Flow_Unit omv_data);
#endif








