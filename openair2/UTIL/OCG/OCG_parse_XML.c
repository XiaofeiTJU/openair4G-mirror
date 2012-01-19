/*******************************************************************************

  Eurecom OpenAirInterface
  Copyright(c) 1999 - 2011 Eurecom

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

/*! \file OCG_parse_XML.c
* \brief Parse the content of the XML configuration file
* \author Lusheng Wang  & Navid Nikaein
* \date 2011
* \version 0.1
* \company Eurecom
* \email: lusheng.wang@eurecom.fr
* \note
* \warning
*/

/*--- INCLUDES ---------------------------------------------------------------*/
#include <libxml/tree.h>
#include <libxml/parser.h>
//#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include "OCG_vars.h"
#include "OCG_parse_XML.h"
#include "UTIL/LOG/log.h"
/*----------------------------------------------------------------------------*/

#ifndef HAVE_STRNDUP
	char * strndup (const char *s, size_t size);
#endif

static int oai_emulation_;	/*!< \brief indicating that the parsing position is now within OAI_Emulation_*/

static int environment_system_config_;		/*!< \brief indicating that the parsing position is now within Envi_Config_*/
static int fading_;
static int large_scale_;
static int free_space_model_parameters_;
static int pathloss_exponent_;
static int pathloss_0_dB_;
static int small_scale_;
static int ricean_8tap_;
static int rice_factor_dB_;
static int shadowing_;
static int decorrelation_distance_m_;
static int variance_dB_;
static int inter_site_correlation_;
static int wall_penetration_loss_dB_;
static int system_bandwidth_MB_;
static int system_frequency_GHz_;
static int antenna_;
static int eNB_antenna_;
static int UE_antenna_;
static int number_of_sectors_;
static int beam_width_dB_;
static int alpha_rad_1_;
static int alpha_rad_2_;
static int alpha_rad_3_;
static int antenna_gain_dBi_;
static int tx_power_dBm_;
static int rx_noise_level_dB_;
static int antenna_orientation_degree_1_;
static int antenna_orientation_degree_2_;
static int antenna_orientation_degree_3_;

static int topology_config_;
static int area_;				/*!< \brief indicating that the parsing position is now within Area_*/
static int x_km_;
static int y_km_;
static int network_type_;
static int cell_type_;
static int relay_;
static int number_of_relays_;
static int mobility_;
static int UE_mobility_;
static int UE_mobility_type_;
static int grid_walk_;
static int grid_map_;
static int horizontal_grid_;
static int vertical_grid_;
static int grid_trip_type_;
static int UE_initial_distribution_;
static int random_UE_distribution_;
static int number_of_nodes_;
static int concentrated_UE_distribution_;
static int grid_UE_distribution_;
static int random_grid_;
static int border_grid_;
static int UE_moving_dynamics_;
static int min_speed_mps_;
static int max_speed_mps_;
static int min_sleep_ms_;
static int max_sleep_ms_;
static int min_journey_time_ms_;
static int max_journey_time_ms_;
static int eNB_mobility_;
static int eNB_mobility_type_;
static int eNB_initial_distribution_;
static int random_eNB_distribution_;
static int number_of_cells_;
static int hexagonal_eNB_distribution_;
static int inter_eNB_distance_km_;
static int grid_eNB_distribution_;
static int number_of_grid_x_;
static int number_of_grid_y_;

static int application_config_;		/*!< \brief indicating that the parsing position is now within App_Config_*/
static int predefined_traffic_;
static int customized_traffic_;
static int application_type_;			/*!< \brief indicating that the parsing position is now within App_Type_*/
static int source_id_;
static int destination_id_;
static int traffic_;			/*!< \brief indicating that the parsing position is now within Traffic_*/
static int transport_protocol_;	/*!< \brief indicating that the parsing position is now within Transport_Protocol_*/
static int ip_version_;
static int idt_dist_;
static int idt_min_ms_;
static int idt_max_ms_;
static int idt_standard_deviation_;
static int idt_lambda_;
static int size_dist_;
static int size_min_byte_;
static int size_max_byte_;
static int size_standard_deviation_;
static int size_lambda_;
static int stream_;
static int destination_port_;

static int emulation_config_;		/*!< \brief indicating that the parsing position is now within Emu_Config_*/
static int emulation_time_ms_;
static int performance_;		/*!< \brief indicating that the parsing position is now within Performance_*/
static int metrics_;
static int throughput_;
static int latency_;
static int signalling_overhead_;
static int layer_;				/*!< \brief indicating that the parsing position is now within Layer_*/
static int phy_;
static int mac_;
static int rlc_;
static int pdcp_;
static int rrc_;
static int emu_;
static int omg_;
static int otg_;
static int log_emu_;
static int debug_;
static int info_;
static int warning_;
static int error_;
static int packet_trace_;
static int seed_;
static int user_seed_;
static int seed_value_;

static int cli_enabled_;
static int cli_start_enb_;
static int cli_start_ue_;


static int profile_;


void start_document(void *user_data) {
	//printf("Start parsing ............ \n");
}

void end_document(void *user_data) {
	//printf("End parsing .\n");
}

void start_element(void *user_data, const xmlChar *name, const xmlChar **attrs) { // called once at the beginning of each element 
	if (!xmlStrcmp(name,(unsigned char*) "OAI_EMULATION")) {
		oai_emulation_ = 1;

	} else if (!xmlStrcmp(name,(unsigned char*) "ENVIRONMENT_SYSTEM_CONFIG")) {
		environment_system_config_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "FADING")) {
		fading_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "LARGE_SCALE")) {
		large_scale_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "FREE_SPACE_MODEL_PARAMETERS")) {
		free_space_model_parameters_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PATHLOSS_EXPONENT")) {
		pathloss_exponent_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PATHLOSS_0_dB")) {
		pathloss_0_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SMALL_SCALE")) {
		small_scale_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RICEAN_8TAP")) {
		ricean_8tap_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RICE_FACTOR_dB")) {
		rice_factor_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SHADOWING")) {
		shadowing_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "DECORRELATION_DISTANCE_m")) {
		decorrelation_distance_m_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "VARIANCE_dB")) {
		variance_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "INTER_SITE_CORRELATION")) {
		inter_site_correlation_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "WALL_PENETRATION_LOSS_dB")) {
		wall_penetration_loss_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SYSTEM_BANDWIDTH_MB")) {
		system_bandwidth_MB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SYSTEM_FREQUENCY_GHz")) {
		system_frequency_GHz_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA")) {
		antenna_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_ANTENNA")) {
		eNB_antenna_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_ANTENNA")) {
		UE_antenna_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_SECTORS")) {
		number_of_sectors_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "BEAM_WIDTH_dB")) {
		beam_width_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_1")) {
		alpha_rad_1_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_2")) {
		alpha_rad_2_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_3")) {
		alpha_rad_3_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_GAIN_dBi")) {
		antenna_gain_dBi_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "TX_POWER_dBm")) {
		tx_power_dBm_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RX_NOISE_LEVEL_dB")) {
		rx_noise_level_dB_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree1")) {
		antenna_orientation_degree_1_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree2")) {
		antenna_orientation_degree_2_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree3")) {
		antenna_orientation_degree_3_ = 1;

	} else if (!xmlStrcmp(name,(unsigned char*) "TOPOLOGY_CONFIG")) {
		topology_config_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "AREA")) {
		area_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "X_km")) {
		x_km_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "Y_km")) {
		y_km_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NETWORK_TYPE")) {
		network_type_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "CELL_TYPE")) {
		cell_type_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RELAY")) {
		relay_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_RELAYS")) {
		number_of_relays_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MOBILITY")) {
		mobility_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOBILITY")) {
		UE_mobility_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOBILITY_TYPE")) {
		UE_mobility_type_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_WALK")) {
		grid_walk_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_MAP")) {
		grid_map_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "HORIZONTAL_GRID")) {
		horizontal_grid_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "VERTICAL_GRID")) {
		vertical_grid_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_TRIP_TYPE")) {
		grid_trip_type_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_INITIAL_DISTRIBUTION")) {
		UE_initial_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_UE_DISTRIBUTION")) {
		random_UE_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_NODES")) {
		number_of_nodes_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "CONCENTRATED_UE_DISTRIBUTION")) {
		concentrated_UE_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_UE_DISTRIBUTION")) {
		grid_UE_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_GRID")) {
		random_grid_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "BORDER_GRID")) {
		border_grid_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOVING_DYNAMICS")) {
		UE_moving_dynamics_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_SPEED_mps")) {
		min_speed_mps_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_SPEED_mps")) {
		max_speed_mps_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_SLEEP_ms")) {
		min_sleep_ms_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_SLEEP_ms")) {
		max_sleep_ms_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_JOURNEY_TIME_ms")) {
		min_journey_time_ms_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_JOURNEY_TIME_ms")) {
		max_journey_time_ms_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_MOBILITY")) {
		eNB_mobility_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_MOBILITY_TYPE")) {
		eNB_mobility_type_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_INITIAL_DISTRIBUTION")) {
		eNB_initial_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_eNB_DISTRIBUTION")) {
		random_eNB_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_CELLS")) {
		number_of_cells_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "HEXAGONAL_eNB_DISTRIBUTION")) {
		hexagonal_eNB_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "INTER_eNB_DISTANCE_km")) {
		inter_eNB_distance_km_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_eNB_DISTRIBUTION")) {
		grid_eNB_distribution_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_GRID_X")) {
		number_of_grid_x_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_GRID_Y")) {
		number_of_grid_y_ = 1;

	} else if (!xmlStrcmp(name, "APPLICATION_CONFIG")) {
		application_config_ = 1;
		oai_emulation.info.max_predefined_traffic_config_index = 0;
		oai_emulation.info.max_customized_traffic_config_index = 0;
	} else if (!xmlStrcmp(name, "PREDEFINED_TRAFFIC")) {
		predefined_traffic_ = 1;
		oai_emulation.info.max_predefined_traffic_config_index ++;
	} else if (!xmlStrcmp(name, "CUSTOMIZED_TRAFFIC")) {
		customized_traffic_ = 1;
		oai_emulation.info.max_customized_traffic_config_index ++;
	} else if (!xmlStrcmp(name, "SOURCE_ID")) {
		source_id_ = 1;
	} else if (!xmlStrcmp(name, "DESTINATION_ID")) {
		destination_id_ = 1;
	} else if (!xmlStrcmp(name, "APPLICATION_TYPE")) {
		application_type_ = 1;
	} else if (!xmlStrcmp(name, "TRAFFIC")) {
		traffic_ = 1;
	} else if (!xmlStrcmp(name, "TRANSPORT_PROTOCOL")) {
		transport_protocol_ = 1;
	} else if (!xmlStrcmp(name, "IP_VERSION")) {
		ip_version_ = 1;
	} else if (!xmlStrcmp(name, "IDT_DIST")) {
		idt_dist_ = 1;
	} else if (!xmlStrcmp(name, "IDT_MIN_ms")) {
		idt_min_ms_ = 1;
	} else if (!xmlStrcmp(name, "IDT_MAX_ms")) {
		idt_max_ms_ = 1;
	} else if (!xmlStrcmp(name, "IDT_STANDARD_DEVIATION")) {
		idt_standard_deviation_ = 1;
	} else if (!xmlStrcmp(name, "IDT_LAMBDA")) {
		idt_lambda_ = 1;
	} else if (!xmlStrcmp(name, "SIZE_DIST")) {
		size_dist_ = 1;
	} else if (!xmlStrcmp(name, "SIZE_MIN_byte")) {
		size_min_byte_ = 1;
	} else if (!xmlStrcmp(name, "SIZE_MAX_byte")) {
		size_max_byte_ = 1;
	} else if (!xmlStrcmp(name, "SIZE_STANDARD_DEVIATION")) {
		size_standard_deviation_ = 1;
	} else if (!xmlStrcmp(name, "SIZE_LAMBDA")) {
		size_lambda_ = 1;
	} else if (!xmlStrcmp(name, "STREAM")) {
		stream_ = 1;
	} else if (!xmlStrcmp(name, "DESTINATION_PORT")) {
		destination_port_ = 1;

	} else if (!xmlStrcmp(name,(unsigned char*) "EMULATION_CONFIG")) {
		emulation_config_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "EMULATION_TIME_ms")) {
		emulation_time_ms_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PERFORMANCE")) {
		performance_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "METRICS")) {
		metrics_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "THROUGHPUT")) {
		throughput_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "LATENCY")) {
		latency_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SIGNALLING_OVERHEAD")) {
		signalling_overhead_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "LAYER")) {
		layer_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PHY")) {
		phy_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAC")) {
		mac_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RLC")) {
		rlc_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PDCP")) {
		pdcp_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "RRC")) {
		rrc_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "OMG")) {
		omg_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "OTG")) {
		otg_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "EMU")) {
		emu_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "LOG_EMU")) {
		log_emu_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "DEBUG")) {
		debug_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "INFO")) {
		info_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "WARNING")) {
		warning_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "ERROR")) {
		error_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "PACKET_TRACE")) {
		packet_trace_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SEED")) {
		seed_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "USER_SEED")) {
		user_seed_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "SEED_VALUE")) {
		seed_value_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "CLI")) {
	  cli_enabled_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "START_ENB")) {
		cli_start_enb_ = 1;
	} else if (!xmlStrcmp(name,(unsigned char*) "START_UE")) {
		cli_start_ue_ = 1;

	} else if (!xmlStrcmp(name,(unsigned char*) "PROFILE")) {
		profile_ = 1;
	} else {
		LOG_W(OCG, "One element could not be parsed : unknown element name '%s'\n", name);
	}
}

void end_element(void *user_data, const xmlChar *name) { // called once at the end of each element 
	if (!xmlStrcmp(name,(unsigned char*) "OAI_EMULATION")) {
		oai_emulation_ = 0;

	} else if (!xmlStrcmp(name,(unsigned char*) "ENVIRONMENT_SYSTEM_CONFIG")) {
		environment_system_config_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "FADING")) {
		fading_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "LARGE_SCALE")) {
		large_scale_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "FREE_SPACE_MODEL_PARAMETERS")) {
		free_space_model_parameters_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PATHLOSS_EXPONENT")) { 
		pathloss_exponent_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PATHLOSS_0_dB")) {
		pathloss_0_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SMALL_SCALE")) {
		small_scale_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RICEAN_8TAP")) {
		ricean_8tap_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RICE_FACTOR_dB")) {
		rice_factor_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SHADOWING")) {
		shadowing_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "DECORRELATION_DISTANCE_m")) {
		decorrelation_distance_m_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "VARIANCE_dB")) {
		variance_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "INTER_SITE_CORRELATION")) {
		inter_site_correlation_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "WALL_PENETRATION_LOSS_dB")) {
		wall_penetration_loss_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SYSTEM_BANDWIDTH_MB")) {
		system_bandwidth_MB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SYSTEM_FREQUENCY_GHz")) {
		system_frequency_GHz_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA")) {
		antenna_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_ANTENNA")) {
		eNB_antenna_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_ANTENNA")) {
		UE_antenna_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_SECTORS")) {
		number_of_sectors_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "BEAM_WIDTH_dB")) {
		beam_width_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_1")) {
		alpha_rad_1_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_2")) {
		alpha_rad_2_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ALPHA_RAD_3")) {
		alpha_rad_3_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_GAIN_dBi")) {
		antenna_gain_dBi_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "TX_POWER_dBm")) {
		tx_power_dBm_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RX_NOISE_LEVEL_dB")) {
		rx_noise_level_dB_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree1")) {
		antenna_orientation_degree_1_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree2")) {
		antenna_orientation_degree_2_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ANTENNA_ORIENTATION_degree3")) {
		antenna_orientation_degree_3_ = 0;

	} else if (!xmlStrcmp(name,(unsigned char*) "TOPOLOGY_CONFIG")) {
		topology_config_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "AREA")) {
		area_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "X_km")) {
		x_km_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "Y_km")) {
		y_km_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NETWORK_TYPE")) {
		network_type_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "CELL_TYPE")) {
		cell_type_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RELAY")) {
		relay_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_RELAYS")) {
		number_of_relays_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MOBILITY")) {
		mobility_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOBILITY")) {
		UE_mobility_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOBILITY_TYPE")) {
		UE_mobility_type_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_WALK")) {
		grid_walk_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_MAP")) {
		grid_map_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "HORIZONTAL_GRID")) {
		horizontal_grid_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "VERTICAL_GRID")) {
		vertical_grid_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_TRIP_TYPE")) {
		grid_trip_type_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_INITIAL_DISTRIBUTION")) {
		UE_initial_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_UE_DISTRIBUTION")) {
		random_UE_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_NODES")) {
		number_of_nodes_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "CONCENTRATED_UE_DISTRIBUTION")) {
		concentrated_UE_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_UE_DISTRIBUTION")) {
		grid_UE_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_GRID")) {
		random_grid_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "BORDER_GRID")) {
		border_grid_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "UE_MOVING_DYNAMICS")) {
		UE_moving_dynamics_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_SPEED_mps")) {
		min_speed_mps_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_SPEED_mps")) {
		max_speed_mps_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_SLEEP_ms")) {
		min_sleep_ms_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_SLEEP_ms")) {
		max_sleep_ms_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MIN_JOURNEY_TIME_ms")) {
		min_journey_time_ms_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAX_JOURNEY_TIME_ms")) {
		max_journey_time_ms_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_MOBILITY")) {
		eNB_mobility_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_MOBILITY_TYPE")) {
		eNB_mobility_type_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "eNB_INITIAL_DISTRIBUTION")) {
		eNB_initial_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RANDOM_eNB_DISTRIBUTION")) {
		random_eNB_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_CELLS")) {
		number_of_cells_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "HEXAGONAL_eNB_DISTRIBUTION")) {
		hexagonal_eNB_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "INTER_eNB_DISTANCE_km")) {
		inter_eNB_distance_km_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "GRID_eNB_DISTRIBUTION")) {
		grid_eNB_distribution_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_GRID_X")) {
		number_of_grid_x_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "NUMBER_OF_GRID_Y")) {
		number_of_grid_y_ = 0;

	} else if (!xmlStrcmp(name, "APPLICATION_CONFIG")) {
		application_config_ = 0;
	} else if (!xmlStrcmp(name, "PREDEFINED_TRAFFIC")) {
		predefined_traffic_ = 0;
	} else if (!xmlStrcmp(name, "CUSTOMIZED_TRAFFIC")) {
		customized_traffic_ = 0;
	} else if (!xmlStrcmp(name, "SOURCE_ID")) {
		source_id_ = 0;
	} else if (!xmlStrcmp(name, "DESTINATION_ID")) {
		destination_id_ = 0;
	} else if (!xmlStrcmp(name, "APPLICATION_TYPE")) {
		application_type_ = 0;
	} else if (!xmlStrcmp(name, "TRAFFIC")) {
		traffic_ = 0;
	} else if (!xmlStrcmp(name, "TRANSPORT_PROTOCOL")) {
		transport_protocol_ = 0;
	} else if (!xmlStrcmp(name, "IP_VERSION")) {
		ip_version_ = 0;
	} else if (!xmlStrcmp(name, "IDT_DIST")) {
		idt_dist_ = 0;
	} else if (!xmlStrcmp(name, "IDT_MIN_ms")) {
		idt_min_ms_ = 0;
	} else if (!xmlStrcmp(name, "IDT_MAX_ms")) {
		idt_max_ms_ = 0;
	} else if (!xmlStrcmp(name, "IDT_STANDARD_DEVIATION")) {
		idt_standard_deviation_ = 0;
	} else if (!xmlStrcmp(name, "IDT_LAMBDA")) {
		idt_lambda_ = 0;
	} else if (!xmlStrcmp(name, "SIZE_DIST")) {
		size_dist_ = 0;
	} else if (!xmlStrcmp(name, "SIZE_MIN_byte")) {
		size_min_byte_ = 0;
	} else if (!xmlStrcmp(name, "SIZE_MAX_byte")) {
		size_max_byte_ = 0;
	} else if (!xmlStrcmp(name, "SIZE_STANDARD_DEVIATION")) {
		size_standard_deviation_ = 0;
	} else if (!xmlStrcmp(name, "SIZE_LAMBDA")) {
		size_lambda_ = 0;
	} else if (!xmlStrcmp(name, "STREAM")) {
		stream_ = 0;
	} else if (!xmlStrcmp(name, "DESTINATION_PORT")) {
		destination_port_ = 0;

	} else if (!xmlStrcmp(name,(unsigned char*) "EMULATION_CONFIG")) {
		emulation_config_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "EMULATION_TIME_ms")) {
		emulation_time_ms_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PERFORMANCE")) {
		performance_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "METRICS")) {
		metrics_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "THROUGHPUT")) {
		throughput_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "LATENCY")) {
		latency_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SIGNALLING_OVERHEAD")) {
		signalling_overhead_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "LAYER")) {
		layer_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PHY")) {
		phy_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "MAC")) {
		mac_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RLC")) {
		rlc_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PDCP")) {
		pdcp_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "RRC")) {
		rrc_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "OMG")) {
		omg_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "OTG")) {
		otg_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "EMU")) {
		emu_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "LOG_EMU")) {
		log_emu_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "DEBUG")) {
		debug_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "INFO")) {
		info_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "WARNING")) {
		warning_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "ERROR")) {
		error_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PACKET_TRACE")) {
		packet_trace_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SEED")) {
		seed_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "USER_SEED")) {
		user_seed_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "SEED_VALUE")) {
		seed_value_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "CLI")) {
		cli_enabled_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "START_ENB")) {
		cli_start_enb_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "START_UE")) {
		cli_start_ue_ = 0;
	} else if (!xmlStrcmp(name,(unsigned char*) "PROFILE")) {
		profile_ = 0;
	}
}

void characters(void *user_data, const xmlChar *xmlch, int xmllen) { // called once when there is content in each element 

  int i;
  const char *ch = (const char*) xmlch;
  size_t len = (size_t) xmllen;

	if (oai_emulation_) {
		if (environment_system_config_) {
			if (fading_) {
				if (large_scale_) {
				  oai_emulation.environment_system_config.fading.large_scale.selected_option = strndup(ch, len);
				} else if (small_scale_) {
					oai_emulation.environment_system_config.fading.small_scale.selected_option = strndup(ch, len);
				} else if (shadowing_) {
					if (decorrelation_distance_m_) {
						oai_emulation.environment_system_config.fading.shadowing.decorrelation_distance_m = atof(ch);
					} else if (variance_dB_) {
						oai_emulation.environment_system_config.fading.shadowing.variance_dB = atof(ch);
					} else if (inter_site_correlation_) {
						oai_emulation.environment_system_config.fading.shadowing.inter_site_correlation = atof(ch);
					}
				} else if (free_space_model_parameters_) {
					if (pathloss_exponent_) {
						oai_emulation.environment_system_config.fading.free_space_model_parameters.pathloss_exponent = atof(ch);
					} else if (pathloss_0_dB_) {
						oai_emulation.environment_system_config.fading.free_space_model_parameters.pathloss_0_dB = atof(ch);
					}
				} else if (ricean_8tap_) {
					oai_emulation.environment_system_config.fading.ricean_8tap.rice_factor_dB = atof(ch);
				}
			} else if (wall_penetration_loss_dB_) {
				oai_emulation.environment_system_config.wall_penetration_loss_dB = atof(ch);
			} else if (system_bandwidth_MB_) {
				oai_emulation.environment_system_config.system_bandwidth_MB = atof(ch);
			} else if (system_frequency_GHz_) {
				oai_emulation.environment_system_config.system_frequency_GHz = atof(ch);
			} else if (antenna_) {
				if (eNB_antenna_) {
					if (number_of_sectors_) {
						oai_emulation.environment_system_config.antenna.eNB_antenna.number_of_sectors = atoi(ch);
					} else if (beam_width_dB_) {
						oai_emulation.environment_system_config.antenna.eNB_antenna.beam_width_dB = atof(ch);
					} else if (alpha_rad_1_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.alpha_rad[1] = atof(ch);
					} else if (alpha_rad_2_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.alpha_rad[2] = atof(ch);
					} else if (alpha_rad_3_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.alpha_rad[3] = atof(ch);
					} else if (antenna_gain_dBi_) {
						oai_emulation.environment_system_config.antenna.eNB_antenna.antenna_gain_dBi = atof(ch);
					} else if (tx_power_dBm_) {
						oai_emulation.environment_system_config.antenna.eNB_antenna.tx_power_dBm = atof(ch);
					} else if (rx_noise_level_dB_) {
						oai_emulation.environment_system_config.antenna.eNB_antenna.rx_noise_level_dB = atof(ch);
					} else if (antenna_orientation_degree_1_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.antenna_orientation_degree[1] = atof(ch);
					} else if (antenna_orientation_degree_2_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.antenna_orientation_degree[2] = atof(ch);
					} else if (antenna_orientation_degree_3_) {
					  		oai_emulation.environment_system_config.antenna.eNB_antenna.antenna_orientation_degree[3] = atof(ch);
					}
				} else if (UE_antenna_) {
					if (antenna_gain_dBi_) {
							oai_emulation.environment_system_config.antenna.UE_antenna.antenna_gain_dBi = atof(ch);
					} else if (tx_power_dBm_) {
							oai_emulation.environment_system_config.antenna.UE_antenna.tx_power_dBm = atof(ch);
					} else if (rx_noise_level_dB_) {
							oai_emulation.environment_system_config.antenna.UE_antenna.rx_noise_level_dB = atof(ch);
					}
				}
			}

		} else if (topology_config_) {
			if (area_) {
				if (x_km_) {
					oai_emulation.topology_config.area.x_km = atof(ch);
				} else if (y_km_) {
					oai_emulation.topology_config.area.y_km = atof(ch);
				}
			} else if (network_type_) {
			  oai_emulation.topology_config.network_type.selected_option = strndup(ch, len);
			} else if (cell_type_) {
				oai_emulation.topology_config.cell_type.selected_option = strndup(ch, len);
			} else if (relay_) {
				if (number_of_relays_) {
					oai_emulation.topology_config.relay.number_of_relays = atoi(ch);
				}
			} else if (mobility_) {
				if (UE_mobility_) {
					if (UE_mobility_type_) {
						oai_emulation.topology_config.mobility.UE_mobility.UE_mobility_type.selected_option = strndup(ch, len);
					} else if (grid_walk_) {
						if (grid_map_) {
							if (horizontal_grid_) {
								oai_emulation.topology_config.mobility.UE_mobility.grid_walk.grid_map.horizontal_grid = atoi(ch);
							} else if (vertical_grid_) {
								oai_emulation.topology_config.mobility.UE_mobility.grid_walk.grid_map.vertical_grid = atoi(ch);
							}
						} else if (grid_trip_type_) {
							oai_emulation.topology_config.mobility.UE_mobility.grid_walk.grid_trip_type.selected_option = strndup(ch, len);
						}	
					} else if (UE_initial_distribution_) {
						oai_emulation.topology_config.mobility.UE_mobility.UE_initial_distribution.selected_option = strndup(ch, len);
					} else if (random_UE_distribution_) {
						if (number_of_nodes_) {
							oai_emulation.topology_config.mobility.UE_mobility.random_UE_distribution.number_of_nodes = atoi(ch);
							oai_emulation.info.nb_ue_local = oai_emulation.topology_config.mobility.UE_mobility.random_UE_distribution.number_of_nodes;
						}
					} else if (concentrated_UE_distribution_) {
						if (number_of_nodes_) {
							oai_emulation.topology_config.mobility.UE_mobility.concentrated_UE_distribution.number_of_nodes = atoi(ch);
							oai_emulation.info.nb_ue_local = oai_emulation.topology_config.mobility.UE_mobility.concentrated_UE_distribution.number_of_nodes;
						}
					} else if (grid_UE_distribution_) {
						if (random_grid_) {
							if (number_of_nodes_) {
								oai_emulation.topology_config.mobility.UE_mobility.grid_UE_distribution.random_grid.number_of_nodes = atoi(ch);
								oai_emulation.info.nb_ue_local = oai_emulation.topology_config.mobility.UE_mobility.grid_UE_distribution.random_grid.number_of_nodes;
							}
						} else if (border_grid_) {
							if (number_of_nodes_) {
								oai_emulation.topology_config.mobility.UE_mobility.grid_UE_distribution.border_grid.number_of_nodes = atoi(ch);
								oai_emulation.info.nb_ue_local = oai_emulation.topology_config.mobility.UE_mobility.grid_UE_distribution.border_grid.number_of_nodes;
							}
						}
					} else if (UE_moving_dynamics_) {
						if (min_speed_mps_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.min_speed_mps = atof(ch);
						} else if (max_speed_mps_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.max_speed_mps = atof(ch);
						} else if (min_sleep_ms_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.min_sleep_ms = atof(ch);
						} else if (max_sleep_ms_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.max_sleep_ms = atof(ch);
						} else if (min_journey_time_ms_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.min_journey_time_ms = atof(ch);
						} else if (max_journey_time_ms_) {
							oai_emulation.topology_config.mobility.UE_mobility.UE_moving_dynamics.max_journey_time_ms = atof(ch);
						}
					}
				} else if (eNB_mobility_) {
					if (eNB_mobility_type_) {
						oai_emulation.topology_config.mobility.eNB_mobility.eNB_mobility_type.selected_option = strndup(ch, len);
					} else if (eNB_initial_distribution_) {
						oai_emulation.topology_config.mobility.eNB_mobility.eNB_initial_distribution.selected_option = strndup(ch, len);
					} else if (random_eNB_distribution_) {
						if (number_of_cells_) {
							oai_emulation.topology_config.mobility.eNB_mobility.random_eNB_distribution.number_of_cells = atoi(ch);
							oai_emulation.info.nb_enb_local = oai_emulation.topology_config.mobility.eNB_mobility.random_eNB_distribution.number_of_cells;
						}
					} else if (hexagonal_eNB_distribution_) {
						if (number_of_cells_) {
							oai_emulation.topology_config.mobility.eNB_mobility.hexagonal_eNB_distribution.number_of_cells = atoi(ch);
							oai_emulation.info.nb_enb_local = oai_emulation.topology_config.mobility.eNB_mobility.hexagonal_eNB_distribution.number_of_cells;
						} else if (inter_eNB_distance_km_) {
							oai_emulation.topology_config.mobility.eNB_mobility.hexagonal_eNB_distribution.inter_eNB_distance_km = atof(ch);
						}
					} else if (grid_eNB_distribution_) {
						if (number_of_grid_x_) {
							oai_emulation.topology_config.mobility.eNB_mobility.grid_eNB_distribution.number_of_grid_x = atoi(ch);
						} else if (number_of_grid_y_) {
							oai_emulation.topology_config.mobility.eNB_mobility.grid_eNB_distribution.number_of_grid_y = atoi(ch);
							oai_emulation.info.nb_enb_local = oai_emulation.topology_config.mobility.eNB_mobility.grid_eNB_distribution.number_of_grid_x * oai_emulation.topology_config.mobility.eNB_mobility.grid_eNB_distribution.number_of_grid_y;
						}
					}
				} 
			}

		} else if (application_config_) {
			if (predefined_traffic_) {
				if (source_id_) {
					oai_emulation.application_config.predefined_traffic.source_id[oai_emulation.info.max_predefined_traffic_config_index] = strndup(ch, len);
				} else if (destination_id_) {
					oai_emulation.application_config.predefined_traffic.destination_id[oai_emulation.info.max_predefined_traffic_config_index] = strndup(ch, len);
				} else if (application_type_) {
					oai_emulation.application_config.predefined_traffic.application_type[oai_emulation.info.max_predefined_traffic_config_index] = strndup(ch, len);
				}
			} else if (customized_traffic_) {
				if (source_id_) {
					oai_emulation.application_config.customized_traffic.source_id[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (destination_id_) {
					oai_emulation.application_config.customized_traffic.destination_id[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (transport_protocol_) {
					oai_emulation.application_config.customized_traffic.transport_protocol[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (ip_version_) {
					oai_emulation.application_config.customized_traffic.ip_version[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (idt_dist_) {
					oai_emulation.application_config.customized_traffic.idt_dist[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (idt_min_ms_) {
					oai_emulation.application_config.customized_traffic.idt_min_ms[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (idt_max_ms_) {
					oai_emulation.application_config.customized_traffic.idt_max_ms[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (idt_standard_deviation_) {
					oai_emulation.application_config.customized_traffic.idt_standard_deviation[oai_emulation.info.max_customized_traffic_config_index] = atof(ch);
				} else if (idt_lambda_) {
					oai_emulation.application_config.customized_traffic.idt_lambda[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (size_dist_) {
					oai_emulation.application_config.customized_traffic.size_dist[oai_emulation.info.max_customized_traffic_config_index] = strndup(ch, len);
				} else if (size_min_byte_) {
					oai_emulation.application_config.customized_traffic.size_min_byte[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (size_max_byte_) {
					oai_emulation.application_config.customized_traffic.size_max_byte[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (size_standard_deviation_) {
					oai_emulation.application_config.customized_traffic.size_standard_deviation[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (size_lambda_) {
					oai_emulation.application_config.customized_traffic.size_lambda[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (stream_) {
					oai_emulation.application_config.customized_traffic.stream[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				} else if (destination_port_) {
					oai_emulation.application_config.customized_traffic.destination_port[oai_emulation.info.max_customized_traffic_config_index] = atoi(ch);
				}
			}

		} else if (emulation_config_) {
			if (emulation_time_ms_) {
			  oai_emulation.emulation_config.emulation_time_ms = atof(ch);
			} else if (performance_) {
			  if (metrics_) {
			    if (throughput_) {
			      oai_emulation.emulation_config.performance.metrics.throughput = atoi(ch);
			    } else if (latency_) {
			      oai_emulation.emulation_config.performance.metrics.latency = atoi(ch);
			    } else if (signalling_overhead_) {
			      oai_emulation.emulation_config.performance.metrics.signalling_overhead = atoi(ch);
			    }
			  } else if (layer_) {
			    if (phy_) {
			      oai_emulation.emulation_config.performance.layer.phy = atoi(ch);
			    } else if (mac_) {
			      oai_emulation.emulation_config.performance.layer.mac = atoi(ch);
			    } else if (rlc_) {
			      oai_emulation.emulation_config.performance.layer.rlc = atoi(ch);
			    } else if (pdcp_) {
			      oai_emulation.emulation_config.performance.layer.pdcp = atoi(ch);
			    } else if (rrc_) {
			      oai_emulation.emulation_config.performance.layer.rrc = atoi(ch);
			    } else if (omg_) {
			      oai_emulation.emulation_config.performance.layer.omg = atoi(ch);
			    } else if (otg_) {
			      oai_emulation.emulation_config.performance.layer.otg = atoi(ch);
			    } else if (emu_) {
			      oai_emulation.emulation_config.performance.layer.emu = atoi(ch);
			    }
			  } else if (log_emu_) {
			    if (debug_) {
			      oai_emulation.emulation_config.performance.log_emu.debug = atoi(ch);
			      oai_emulation.info.g_log_level=LOG_DEBUG;
			    } else if (info_) {
			      oai_emulation.emulation_config.performance.log_emu.info = atoi(ch);
			      oai_emulation.info.g_log_level=LOG_INFO;
			    } else if (warning_) {
			      oai_emulation.emulation_config.performance.log_emu.warning = atoi(ch);
			      oai_emulation.info.g_log_level=LOG_WARNING;
			    } else if (error_) {
			      oai_emulation.emulation_config.performance.log_emu.error = atoi(ch);
			      oai_emulation.info.g_log_level=LOG_ERR;
			    }
			  } else if (packet_trace_) {
			    if (mac_) {
			      oai_emulation.emulation_config.performance.packet_trace.mac = atoi(ch);
			    }
			  } else if (seed_) {
			    oai_emulation.emulation_config.seed.selected_option = strndup(ch, len);
			  } else if (user_seed_) {
			    oai_emulation.emulation_config.user_seed.seed_value = atoi(ch);
			  }
			  else if (cli_enabled_) {
			    oai_emulation.info.cli_enabled = 1;
			    if (cli_start_enb_) {
			      for (i=0; i < oai_emulation.info.cli_num_enb; i++)
				oai_emulation.info.cli_start_enb[i] = atoi(ch);
			    }else if (cli_start_ue_) {
			      for (i=0; i < oai_emulation.info.cli_num_ue; i++)
				oai_emulation.info.cli_start_ue[i] =  atoi(ch);
			    }
			  }
			}
		} else if (profile_) {
			oai_emulation.profile = strndup(ch, len);
		}

	}
}


int parse_XML(char src_file[FILENAME_LENGTH_MAX + DIR_LENGTH_MAX]){
	// config the parser, refer to 'http://www.saxproject.org/apidoc/org/xml/sax/ContentHandler.html'
	xmlSAXHandler sax_handler = { 0 }; // a Simple API for XML (SAX) handler 
	sax_handler.startDocument = start_document;
	sax_handler.endDocument = end_document;
	sax_handler.startElement = start_element;
	sax_handler.endElement = end_element;
	sax_handler.characters = characters;

	// Parsing the XML file
	if (xmlSAXUserParseFile(&sax_handler, NULL, src_file) != 0) {
		LOG_E(OCG, "An error occurs while parsing the configuration file!\n");
		return MODULE_ERROR;
	}
	LOG_I(OCG, "The configuration file is parsed\n");
	return MODULE_OK;
}
