#include "pch.h"
#include <map>
#include "Network\TCPClient.h"
#include "Objects\HoloLensContext.h"
#include <Network\Objects\TypeConversion.h>

using namespace Utility;
using namespace CubZHoloLensClient;

WinNetwork::TCPClient::TCPClient(Platform::String ^username)
{
	_user.set_username(Utility::platformStringToString(username));
	_client.setPacketCallback([this](Network::IPacketConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
	{
		this->handlePacket(co, packet);
	});
}

WinNetwork::TCPClient::~TCPClient()
{
	if (_thread)
	{
		_thread->join();
		delete _thread;
	}
}

void WinNetwork::TCPClient::run()
{
	_client.run();
}

void WinNetwork::TCPClient::runAsync()
{
	if (_thread)
		throw std::runtime_error("A thread is already running for this client instance");
	_thread = new boost::thread([&] {
		_client.run();
	});
}

void WinNetwork::TCPClient::connect(Platform::String ^ip, Platform::String ^port)
{
	auto packet = std::make_shared<Network::TCPPacket>();

	TRACE("Connecting to " << platformStringToString(ip) << ":" << platformStringToString(port) << std::endl);
	_client.connect(platformStringToString(ip), platformStringToString(port));

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
	packet->getTCPPacket().mutable_authmessage()->set_code(0);
	packet->getTCPPacket().mutable_authmessage()->set_allocated_user(&_user);

	_client.sendPacket(packet);

	//Release allocated user to get ownership of the data back
	//This way protobuf wont delete the resource
	packet->getTCPPacket().mutable_authmessage()->release_user();
}

void CubZHoloLensClient::WinNetwork::TCPClient::disconnect()
{
	_client.disconnect();
}

void CubZHoloLensClient::WinNetwork::TCPClient::setUsername(Platform::String ^ username)
{
	_user.set_username(Utility::platformStringToString(username));
}

void CubZHoloLensClient::WinNetwork::TCPClient::listServerFiles(Platform::String ^ path)
{
	auto packet = std::make_shared<Network::TCPPacket>();
	std::string filePath = Utility::platformStringToString(path);

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE);
	packet->getTCPPacket().mutable_listfilemessage()->set_allocated_pathtolist(&filePath);
	packet->getTCPPacket().mutable_listfilemessage()->set_userid(_id);

	_client.sendPacket(packet);

	packet->getTCPPacket().mutable_listfilemessage()->release_pathtolist();
}

void CubZHoloLensClient::WinNetwork::TCPClient::listServerUsers()
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_USER);
	packet->getTCPPacket().mutable_listusermessage()->set_userid(_id);

	_client.sendPacket(packet);
}

void CubZHoloLensClient::WinNetwork::TCPClient::requestUDPInfos()
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_UDP);
	packet->getTCPPacket().mutable_udpmessageid()->set_userid(_id);

	_client.sendPacket(packet);
}

void CubZHoloLensClient::WinNetwork::TCPClient::entityCreateDelete(WinNetwork::EntityAction const &action, WinNetwork::EntityDescription const &entityDesc)
{
	auto packet = std::make_shared<Network::TCPPacket>();
	auto entity = WinNetwork::UWPEntityDescriptionToNative(entityDesc);

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_ENTITY);

	packet->getTCPPacket().mutable_entitymessage()->set_action(CubZPacket::EntityMessage_Action(action));
	packet->getTCPPacket().mutable_entitymessage()->set_userid(GetUID());
	packet->getTCPPacket().mutable_entitymessage()->set_allocated_entity(entity);

	_client.sendPacket(packet);
}

void CubZHoloLensClient::WinNetwork::TCPClient::createEntity(WinNetwork::EntityDescription const &entityDesc)
{
	entityCreateDelete(WinNetwork::EntityAction::ADD, entityDesc);
}

void CubZHoloLensClient::WinNetwork::TCPClient::deleteEntity(WinNetwork::EntityDescription const &entityDesc)
{
	entityCreateDelete(WinNetwork::EntityAction::REMOVE, entityDesc);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
{
	auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);
	auto type = tcpPacket->getPacketType();

	TRACE("Received " << tcpPacket << std::endl);
	if (type == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
		return handleAuthPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE)
		return handleFileListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_USER)
		return handleUserListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_LIST_DEVICE)
		return handleDeviceListPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_UDP)
		return handleUDPPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_PING)
		return handlePingPacket(co, tcpPacket);
	else if (type == Network::TCPPacket::Type::PacketTCP_Type_ENTITY)
		return handleEntityPacket(co, tcpPacket);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling auth message" << std::endl);
	if (!packet->getTCPPacket().authmessage().user().username().compare(_user.username()) &&
		packet->getTCPPacket().authmessage().code() == 0 &&
		packet->getTCPPacket().authmessage().action() == CubZPacket::AuthMessage::LOGIN)
	{
		HoloLensContext::Instance()->login();
		_user = packet->getTCPPacket().authmessage().user();
		_id = packet->getTCPPacket().authmessage().user().id();
	}
	else if (!packet->getTCPPacket().authmessage().user().username().compare(_user.username()) &&
		packet->getTCPPacket().authmessage().code() == 0 &&
		packet->getTCPPacket().authmessage().action() == CubZPacket::AuthMessage::LOGOUT)
	{
		HoloLensContext::Instance()->logout();
	}
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling file list message" << std::endl);
	auto listFilePacket = packet->getTCPPacket().listfilemessage();
	auto fileList = listFilePacket.list();
	auto result = ref new Platform::Collections::Vector<Platform::String^>();
	for (auto &file : fileList)
	{
		TRACE("file found" << std::endl);
		result->Append(Utility::stringToPlatformString(file.name()));
	}
	TRACE("end of files" << std::endl);
	ListFileEvent(result);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleUserListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling user list message" << std::endl);
	auto listUserPacket = packet->getTCPPacket().listusermessage();
	auto userList = listUserPacket.list();
	auto result = ref new Platform::Collections::Vector<Platform::String^>();
	for (auto &user : userList)
	{
		result->Append(Utility::stringToPlatformString(user.username()));
	}
	ListUserEvent(result);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleDeviceListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling device list message" << std::endl);
	auto listDevicePacket = packet->getTCPPacket().listdevicemessage();
	auto deviceList = listDevicePacket.list();
	auto result = ref new Platform::Collections::Vector<Platform::String^>();
	for (auto &device : deviceList)
	{
		result->Append(Utility::stringToPlatformString(device.name()));
	}
	ListDeviceEvent(result);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handlePingPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	throw ref new Platform::NotImplementedException();
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleUDPPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling UDP id message" << std::endl);
	auto port = Utility::stringToPlatformString(packet->getTCPPacket().udpmessageid().port());
	auto ip = Utility::stringToPlatformString(packet->getTCPPacket().udpmessageid().ip());

	CubZHoloLensClient::HoloLensContext::Instance()->getUDPClient()->connect(ip, port);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleEntityPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	auto entity = packet->getTCPPacket().entitymessage().entity();

	auto entityAction = CubZHoloLensClient::WinNetwork::EntityAction(packet->getTCPPacket().entitymessage().action());

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

	EntityEvent(entityAction, entityDescription);
}
