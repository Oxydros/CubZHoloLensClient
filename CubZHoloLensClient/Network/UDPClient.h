#pragma once

#include <libnetwork\UDPClient.h>
#include <libnetwork\UDPPacket.h>
#include "boost\thread.hpp"
#include "Utility\Utility.h"
#include <Network\Objects\TypeDescription.h>

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{

		public delegate void EntityUpdateEvent(WinNetwork::EntityDescription, WinNetwork::SpaceDescription);

		public ref class UDPClient sealed
		{
		public:
			event EntityUpdateEvent ^ EntityUpdated;

		private:
			Network::UDPClient					_client{};
			boost::thread						*_thread{ nullptr };

		public:
			UDPClient();
		private:
			~UDPClient();

		public:
			void run();
			void runAsync();

			void connect(Platform::String ^ip, Platform::String ^port);
			void disconnect();

			void notifyEntityUpdate(WinNetwork::EntityDescription const &entityDesc, WinNetwork::SpaceDescription const &spatialDesc);

		private:
			void handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet);
			void handleEventPacket(Network::UDPPacket::SharedPtr packet);
		};
	}
}