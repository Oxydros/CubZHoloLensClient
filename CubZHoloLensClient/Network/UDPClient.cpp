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
	auto udpPacket = std::static_pointer_cast<Network::UDPPacket>(packet);
	auto type = udpPacket->getPacketType();

	TRACE("Received " << udpPacket << std::endl);

	if (type == Network::UDPPacket::Type::PacketUDP_Type_ENTITY)
		return handleEventPacket(udpPacket);
}

void CubZHoloLensClient::WinNetwork::UDPClient::handleEventPacket(Network::UDPPacket::SharedPtr packet)
{
	auto entity = packet->getUDPPacket().entitymessage().entityinfos();
	auto space = packet->getUDPPacket().entitymessage().spaceinfos();

	WinNetwork::EntityDescription entityDescription =
	{
		WinNetwork::EntityType(entity.type()),
		{
			WinNetwork::FileType(entity.file().type()),
			Utility::stringToPlatformString(entity.file().name()),
			Utility::stringToPlatformString(entity.file().extension()),
			Utility::stringToPlatformString(entity.file().path()),
			entity.file().size(),
			entity.file().rights()
		},
		entity.id()
	};

	WinNetwork::SpaceDescription spaceDescription =
	{
		Windows::Foundation::Numerics::float3(space.x(), space.y(), space.z()),
		Windows::Foundation::Numerics::float3(space.rx(), space.ry(), space.rz()),
		Windows::Foundation::Numerics::float3(space.sx(), space.sy(), space.sz())
	};

	EntityUpdated(entityDescription, spaceDescription);
}
