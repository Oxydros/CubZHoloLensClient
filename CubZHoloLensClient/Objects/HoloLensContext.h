#pragma once

#include "pch.h"
#include "Network\TCPClient.h"

namespace CubZHoloLensClient
{
	public delegate void ConnectionEvent();

	public ref class HoloLensContext sealed
	{
	private:
		WinNetwork::TCPClient	^_tcpClient{};
		bool					_isLoggedIn{ false };

	public:
		event ConnectionEvent	^onLoggedIn;
		event ConnectionEvent	^onLoggedOut;

	private:
		HoloLensContext() : _tcpClient(ref new WinNetwork::TCPClient("DefaultUser"))
		{
		}

	public:
		static HoloLensContext ^Instance()
		{
			static HoloLensContext ^instance{ ref new HoloLensContext };
			return (instance);
		}

		WinNetwork::TCPClient ^getTCPClient();

	public:
		void login();
		void logout();
		bool isLoggedIn() { return _isLoggedIn; }

		void setUsername(Platform::String ^username);
	};
}