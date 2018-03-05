#pragma once

#include "pch.h"
#include "Network\TCPClient.h"

namespace CubZHoloLensClient
{
	public ref class HoloLensContext sealed
	{
	private:
		WinNetwork::TCPClient	^_tcpClient;
	private:
		HoloLensContext() : _tcpClient(ref new WinNetwork::TCPClient)
		{
		}

	public:
		static HoloLensContext ^Instance()
		{
			static HoloLensContext ^instance{ ref new HoloLensContext };
			return (instance);
		}
		WinNetwork::TCPClient ^getTCPClient();
	};
}