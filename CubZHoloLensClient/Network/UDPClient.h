#pragma once

#include <libnetwork\UDPClient.h>
#include <libnetwork\UDPPacket.h>
#include "boost\thread.hpp"
#include "Utility\Utility.h"

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		public ref class UDPClient sealed
		{
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

		private:
			void handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet);
		};
	}
}