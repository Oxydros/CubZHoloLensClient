#pragma once

#include <libnetwork\TCPClient.h>
#include <libnetwork\TCPPacket.h>
#include "boost\thread.hpp"
#include "Utility\Utility.h"

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		public delegate void FileListEvent(Windows::Foundation::Collections::IVector<Platform::String^>^);
		public delegate void UserListEvent(Windows::Foundation::Collections::IVector<Platform::String^>^);
		public delegate void DeviceListEvent(Windows::Foundation::Collections::IVector<Platform::String^>^);
		public delegate void UDPMessageEvent(Platform::String ^ip, Platform::String ^port);

		public ref class TCPClient sealed
		{
		public:
			event FileListEvent^	ListFileEvent;
			event UserListEvent^	ListUserEvent;
			event DeviceListEvent^	ListDeviceEvent;
			event UDPMessageEvent^	UDPEvent;

		private:
			Network::TCPClient					_client{};
			CubZPacket::UserDescription			_user{};
			boost::thread						*_thread{ nullptr };

		public:
			TCPClient(Platform::String ^username);
		private:
			~TCPClient();

		public:
			void run();
			void runAsync();

			void connect(Platform::String ^ip, Platform::String ^port);
			void disconnect();

			void setUsername(Platform::String ^username);

			void listServerFiles(Platform::String ^path);
			void listServerUsers();

			void requestUDPInfos();

		private:
			void handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet);

			void handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleUserListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleDeviceListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handlePingPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleUDPPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
		};
	}
}