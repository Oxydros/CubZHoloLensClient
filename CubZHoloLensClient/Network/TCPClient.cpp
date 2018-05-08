#include "pch.h"
#include "TCPClient.h"
#include "Objects\HoloLensContext.h"

using namespace Utility;
using namespace CubZHoloLensClient;

WinNetwork::TCPClient::TCPClient(Platform::String ^username)
{
	_user.set_username(Utility::platformStringToString(username));
	_client.setCallback([this](Network::IConnection::SharedPtr co, Network::IPacket const &packet)
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
	Network::TCPPacket *packet = new Network::TCPPacket();

	TRACE("Connecting to " << platformStringToString(ip) << ":" << platformStringToString(port) << std::endl);
	_client.connect(platformStringToString(ip), platformStringToString(port));

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_AUTH);
	packet->getMutableAuthMessage()->set_code(0);
	packet->getMutableAuthMessage()->set_allocated_user(&_user);

	_client.sendPacket(*packet);

	//Release allocated user to get ownership of the data back
	//This way protobuf wont delete the resource
	packet->getMutableAuthMessage()->release_user();
	delete packet;
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
	Network::TCPPacket *packet = new Network::TCPPacket();
	std::string filePath = Utility::platformStringToString(path);

	packet->setType(Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE);
	packet->getMutableTCPPacket()->mutable_listfilemessage()->set_allocated_pathtolist(&filePath);

	_client.sendPacket(*packet);

	packet->getMutableTCPPacket()->mutable_listfilemessage()->release_pathtolist();
	delete packet;
}

void CubZHoloLensClient::WinNetwork::TCPClient::handlePacket(Network::IConnection::SharedPtr co, Network::IPacket const & packet)
{
	auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

	TRACE("Received " << tcpPacket << std::endl);
	if (tcpPacket.getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_AUTH)
		return handleAuthPacket(co, tcpPacket);
	else if (tcpPacket.getPacketType() == Network::TCPPacket::Type::PacketTCP_Type_LIST_FILE)
		return handleFileListPacket(co, tcpPacket);
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleAuthPacket(Network::IConnection::SharedPtr co, Network::TCPPacket const & packet)
{
	TRACE("Handling auth message" << std::endl);
	if (!packet.getAuthMessage().user().username().compare(_user.username()) &&
		packet.getAuthMessage().code() == 0 &&
		packet.getAuthMessage().action() == CubZPacket::AuthMessage::LOGIN)
	{
		HoloLensContext::Instance()->login();
	}
	else if (!packet.getAuthMessage().user().username().compare(_user.username()) &&
		packet.getAuthMessage().code() == 0 &&
		packet.getAuthMessage().action() == CubZPacket::AuthMessage::LOGOUT)
	{
		HoloLensContext::Instance()->logout();
	}
}

void CubZHoloLensClient::WinNetwork::TCPClient::handleFileListPacket(Network::IConnection::SharedPtr co, Network::TCPPacket const & packet)
{
	TRACE("Handling file list message" << std::endl);
	auto listFilePacket = packet.getTCPPacket().listfilemessage();
	auto fileList = listFilePacket.list();
	auto result = ref new Platform::Collections::Vector<Platform::String^>();
	for (auto &file : fileList)
	{
		result->Append(Utility::stringToPlatformString(file.file().name()));
	}
	ListFileEvent(result);
}
