#pragma once

#include "pch.h"
#include "boost\thread.hpp"
#include "Objects\Utility.h"

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		public ref class TCPClient sealed
		{
		private:
			Network::TCPClient					_client;
			boost::thread						*_thread;

		public:
			TCPClient();
		private:
			~TCPClient();

		public:
			void connect(Platform::String ^ip, Platform::String ^port);
			void run();
			void runAsync();

		private:
			std::string platformStringToString(Platform::String^ ps);
		};
	}
}