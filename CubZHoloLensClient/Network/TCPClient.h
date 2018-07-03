#pragma once

#include <libnetwork\TCPClient.h>
#include <libnetwork\TCPPacket.h>
#include "boost\thread.hpp"
#include "Utility\Utility.h"
#include <Network\Objects\TypeDescription.h>

namespace CubZHoloLensClient
{
	namespace WinNetwork
	{
		public enum class EntityAction : int
		{
			ADD = 0,
			REMOVE
		};

		public delegate void FileListEvent(Windows::Foundation::Collections::IVector<WinNetwork::FileDescription>^);
		public delegate void UserListEvent(Windows::Foundation::Collections::IVector<Platform::String^>^);
		public delegate void DeviceListEvent(Windows::Foundation::Collections::IVector<Platform::String^>^);
		public delegate void UDPMessageEvent(Platform::String ^ip, Platform::String ^port);
		public delegate void EntityEvent(WinNetwork::EntityAction, WinNetwork::EntityDescription);

		public ref class TCPClient sealed
		{
		public:
			event FileListEvent		^ListFileEvent;
			event UserListEvent		^ListUserEvent;
			event DeviceListEvent	^ListDeviceEvent;
			event UDPMessageEvent	^UDPEvent;
			event EntityEvent		^EntityEvent;

		private:
			unsigned int						_id;
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

			void createEntity(WinNetwork::EntityDescription const &entityDesc);
			void deleteEntity(WinNetwork::EntityDescription const &entityDesc);

			unsigned int GetUID() { return _id; }

		private:
			void handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet);

			void handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleUserListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleDeviceListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handlePingPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleUDPPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);
			void handleEntityPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet);

		private:
			void entityCreateDelete(WinNetwork::EntityAction const &action, WinNetwork::EntityDescription const &entityDesc);
		};
	}
}