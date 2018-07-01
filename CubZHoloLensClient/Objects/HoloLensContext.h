#pragma once

#include "pch.h"
#include "Network\TCPClient.h"
#include "Network\UDPClient.h"

namespace CubZHoloLensClient
{
	public delegate void ConnectionEvent();

	public ref class HoloLensContext sealed
	{
	private:
		WinNetwork::TCPClient	^_tcpClient{};
		WinNetwork::UDPClient	^_udpClient{};
		bool					_isLoggedIn{ false };

	public:
		event ConnectionEvent	^onLoggedIn;
		event ConnectionEvent	^onLoggedOut;

	private:
		HoloLensContext() : _tcpClient(ref new WinNetwork::TCPClient("DefaultUser")),
							_udpClient(ref new WinNetwork::UDPClient())
		{
		}

	public:
		static HoloLensContext ^Instance()
		{
			static HoloLensContext ^instance{ ref new HoloLensContext };
			return (instance);
		}

		WinNetwork::TCPClient ^getTCPClient();
		WinNetwork::UDPClient ^getUDPClient();

		unsigned int GetUID() { return _tcpClient->GetUID(); }

	public:
		void login();
		void logout();
		bool isLoggedIn() { return _isLoggedIn; }

		void setUsername(Platform::String ^username);
	};
}