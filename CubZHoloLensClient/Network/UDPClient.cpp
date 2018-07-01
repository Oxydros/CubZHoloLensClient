#include "pch.h"
#include "Network\UDPClient.h"
#include "Objects\HoloLensContext.h"
#include <Network\Objects\TypeConversion.h>

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

void CubZHoloLensClient::WinNetwork::UDPClient::notifyEntityUpdate(WinNetwork::EntityDescription const & entityDesc,
																	WinNetwork::SpaceDescription const & spatialDesc)
{
	auto packet = std::make_shared<Network::UDPPacket>();
	auto entity = WinNetwork::UWPEntityDescriptionToNative(entityDesc);
	auto space = WinNetwork::UWPSpaceDescriptionToNative(spatialDesc);

	packet->setType(CubZPacket::PacketUDP_Type::PacketUDP_Type_ENTITY);
	packet->getUDPPacket().set_userid(HoloLensContext::Instance()->GetUID());

	packet->getUDPPacket().mutable_entitymessage()->set_allocated_entityinfos(entity);
	packet->getUDPPacket().mutable_entitymessage()->set_allocated_spaceinfos(space);

	_client.sendPacket(packet);
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

	WinNetwork::EntityDescription entityDescription = WinNetwork::NativeEntityDescriptionToUWP(entity);
	WinNetwork::SpaceDescription spaceDescription = NativeSpaceDescriptionToUWP(space);

	EntityUpdated(entityDescription, spaceDescription);
}
