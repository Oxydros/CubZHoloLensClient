#include "pch.h"
#include "Network\TCPClient.h"
#include "Objects\HoloLensContext.h"
#include <exception>

using namespace Utility;
using namespace CubZHoloLensClient;
#include <iostream>
#include <exception>
using namespace std;


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
	TRACE(&_user << std::endl);
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
	packet->getTCPPacket().mutable_authmessage()->set_code(1);
	packet->getTCPPacket().mutable_authmessage()->set_allocated_user(&_user);

	_client.sendPacket(packet);

	//Release allocated user to get ownership of the data back
	//This way protobuf wont delete the resource
	packet->getTCPPacket().mutable_authmessage()->release_user();
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

	_client.sendPacket(packet);
}

void CubZHoloLensClient::WinNetwork::TCPClient::listServerUsers()
{
	auto packet = std::make_shared<Network::TCPPacket>();

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_USER);

	_client.sendPacket(packet);

	packet->getTCPPacket().mutable_listfilemessage()->release_pathtolist();
}

void CubZHoloLensClient::WinNetwork::TCPClient::handlePacket(Network::IConnection::SharedPtr co, Network::IPacket::SharedPtr packet)
{
	auto tcpPacket = std::static_pointer_cast<Network::TCPPacket>(packet);

	TRACE("Received " << tcpPacket << std::endl);
	if (tcpPacket->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
		return handleAuthPacket(co, tcpPacket);
	else if (tcpPacket->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE)
		return handleFileListPacket(co, tcpPacket);
	else if (tcpPacket->getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_LIST_USER)
		return handleUserListPacket(co, tcpPacket);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket::SharedPtr packet)
{
	TRACE("Handling auth message" << std::endl);
	if (!packet->getTCPPacket().authmessage().user().username().compare(_user.username()) &&
		packet->getTCPPacket().authmessage().code() == 0 &&
		packet->getTCPPacket().authmessage().action() == CubZPacket::AuthMessage::LOGIN)
	{
		HoloLensContext::Instance()->login();
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
		result->Append(Utility::stringToPlatformString(file.file().name()));
	}
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
		result->Append(Utility::stringToPlatformString(user.user().username()));
	}
	ListUserEvent(result);
}
