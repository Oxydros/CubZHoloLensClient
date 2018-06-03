#include "pch.h"
#include "Network\UDPClient.h"
#include "Objects\HoloLensContext.h"

using namespace Utility;
using namespace CubZHoloLensClient;

WinNetwork::UDPClient::UDPClient()
{
	_client.setPacketCallback([this](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
	{
		this->handlePacket(co, packet);
	});
}

WinNetwork::UDPClient::~UDPClient()
{
	if (_thread)
	{
		_thread->join();
		delete _thread;
	}
}

void WinNetwork::UDPClient::run()
{
	_client.run();
}

void WinNetwork::UDPClient::runAsync()
{
	if (_thread)
		throw std::runtime_error("A thread is already running for this client instance");
	_thread = new boost::thread([&] {
		_client.run();
	});
}

void WinNetwork::UDPClient::connect(Platform::String ^ip, Platform::String ^port)
{
	_client.connect(platformStringToString(ip), platformStringToString(port));
}

void CubZHoloLensClient::WinNetwork::UDPClient::disconnect()
{
	_client.disconnect();
}

void CubZHoloLensClient::WinNetwork::UDPClient::handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
{
}
