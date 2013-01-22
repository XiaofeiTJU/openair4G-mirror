/*******************************************************************************

  Eurecom OpenAirInterface
  Copyright(c) 1999 - 2012 Eurecom

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

/*!
 * \file mgmt_packet_handler.hpp
 * \brief A container with packet handling functionality, all the packets read on the socket is passed here
 * \company EURECOM
 * \date 2012
 * \author Baris Demiray
 * \email: baris.demiray@eurecom.fr
 * \note none
 * \bug none
 * \warning none
 */

#ifndef MGMT_PACKET_HANDLER_HPP_
#define MGMT_PACKET_HANDLER_HPP_

#include <boost/array.hpp>
using namespace boost;

#include "packets/mgmt_fac_packet_configuration_notification.hpp"
#include "packets/mgmt_lte_packet_wireless_state_response.hpp"
#include "packets/mgmt_gn_packet_location_table_response.hpp"
#include "packets/mgmt_gn_packet_wireless_state_response.hpp"
#include "packets/mgmt_gn_packet_comm_profile_request.hpp"
#include "packets/mgmt_gn_packet_get_configuration.hpp"
#include "packets/mgmt_gn_packet_location_update.hpp"
#include "packets/mgmt_gn_packet_network_state.hpp"
#include "packets/mgmt_gn_packet.hpp"
#include "mgmt_information_base.hpp"
#include "mgmt_packet_factory.hpp"
#include "util/mgmt_log.hpp"
#include "mgmt_client.hpp"

/**
 * A container for PacketHandler classes feedback to its callers
 */
class PacketHandlerResult {
	public:
		/**
		 * Result of packet processing to be returned to relevant unit calling handle()
		 */
		enum Result {
			/**
			 * Packet is processed and no further action is necessary
			 */
			DISCARD_PACKET = 0,
			/**
			 * Invalid packet
			 */
			INVALID_PACKET = 1,
			/**
			 * Deliver the packet given within this PacketHandlerResult object
			 */
			DELIVER_PACKET = 2,
			/**
			 * All the clients with relevant configuration requirements
			 * (meaning NET or FAC parameters) should be informed of new configuration
			 * PS: This comment above is valid only for GN right now since it's the only "other" client than FAC
			 */
			SEND_CONFIGURATION_UPDATE_AVAILABLE = 3,
			/**
			 * Relays the packet as-is to GN
			 * This case is used for LOCATION_UPDATE packet
			 */
			RELAY_TO_GN = 4
		};

	public:
		/**
		 * Constructor for PacketHandlerResult class
		 *
		 * @param result PacketHandler's result after it processes given packet
		 * @param packet GeonetPacket after it's generated by PacketHandler
		 */
		PacketHandlerResult(PacketHandlerResult::Result result, GeonetPacket* packet) : result(result), packet(packet) {}
		/**
		 * Deconstructor for this result set, responsible for the deallocation of GeonetPacket pointer
		 */
		~PacketHandlerResult() {
			delete packet;
		}

	public:
		/**
		 * Returns the result packet of this class
		 *
		 * @return Resulting packet of type GeonetPacket
		 */
		GeonetPacket* getPacket() const {
			return this->packet;
		}
		/**
		 * Returns the result enumeration
		 *
		 * @return Return enumeration of type PacketHandlerResult::Result
		 */
		PacketHandlerResult::Result getResult() const {
			return this->result;
		}

	private:
		/**
		 * Result enumeration
		 */
		PacketHandlerResult::Result result;
		/**
		 * GeonetPacket pointer
		 */
		GeonetPacket* packet;
};

/**
 * A container with packet handling functionality, all the packets read on
 * the socket is passed here
 */
class PacketHandler {
	public:
		/**
		 * Constructor for PacketHandler class
		 *
		 * @param mib ManagementInformationBase reference
		 * @param logger Logger object reference
		 */
		PacketHandler(ManagementInformationBase& mib, Logger& logger);
		/**
		 * Destructor for PacketHandler class
		 */
		~PacketHandler();

	private:
		/**
		 * Copy constructor to prevent the usage of default one
		 */
		PacketHandler(const PacketHandler& packetHandler);

	public:
		/**
		 * Takes buffer of a packet and processes accordingly
		 *
		 * @param packetBuffer Packet buffer
		 * @return relevant PacketHandler::Result enumeration according to the result
		 */
		PacketHandlerResult* handle(const vector<unsigned char>& packetBuffer);

	private:
		/**
		 * Handles a Get Configuration message creating its reply utilizing relevant
		 * PacketFactory method
		 *
		 * @param packet Pointer to Get Configuration packet object
		 * @param Management client type information (Event type/subtype changes according to the client type)
		 * @return Pointer to a PacketHandlerResult object
		 */
		PacketHandlerResult* handleGetConfigurationEvent(GeonetGetConfigurationEventPacket* packet, ManagementClient::ManagementClientType clientType);
		/**
		 * Handles a Network State message and triggers an update at MIB
		 *
		 * @param packet Pointer to Network State packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		static PacketHandlerResult* handleNetworkStateEvent(GeonetNetworkStateEventPacket* packet);
		/**
		 * Handles a Wireless State Response message and triggers an update at MIB
		 *
		 * @param packet Pointer to incoming Wireless State Response packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		static PacketHandlerResult* handleWirelessStateResponseEvent(GeonetPacket* packet);
		/**
		 * Handles a Location Table Response packet
		 *
		 * @param Pointer to a Location Table Response packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		static PacketHandlerResult* handleLocationTableResponse(GeonetLocationTableResponseEventPacket* packet);
		/**
		 * Handles a Configuration Notification packet
		 *
		 * @param Pointer to a Configuration Notification packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		PacketHandlerResult* handleConfigurationNotification(FacConfigurationNotificationPacket* packet);
		/**
		 * Handles a Communication Profile Request event message and creates a
		 * Communication Profile Response packet
		 *
		 * @param Pointer to a Communication Profile Request packet
		 * @param Management client type information (Event type/subtype changes according to the client type)
		 * @return Pointer to a PacketHandlerResult object
		 */
		PacketHandlerResult* handleCommunicationProfileRequestEvent(GeonetCommunicationProfileRequestPacket* packet, ManagementClient::ManagementClientType clientType);
		/**
		 * Handles a Communication Profile Selection Request event message and creates a
		 * Communication Profile Selection Response packet
		 *
		 * @param Pointer to a Communication Profile Selection Request packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		PacketHandlerResult* handleCommunicationProfileSelectionRequest(FacCommunicationProfileSelectionRequestPacket* request);
		/**
		 * Handles an incoming Location Update message and updates MIB with
		 * this incoming information
		 *
		 * @param Pointer to a Location Update packet
		 * @return Pointer to a PacketHandlerResult object
		 */
		PacketHandlerResult* handleLocationUpdate(GeonetLocationUpdateEventPacket* packet);

	private:
		/**
		 * GeonetPacketFactory object to hide packet generation details from PacketHandler class
		 */
		ManagementPacketFactory* packetFactory;
		/**
		 * ManagementInformationBase object to fetch necessary information when needed
		 */
		ManagementInformationBase& mib;
		/**
		 * Logger object reference
		 */
		Logger& logger;
};

#endif /* MGMT_PACKET_HANDLER_HPP_ */
