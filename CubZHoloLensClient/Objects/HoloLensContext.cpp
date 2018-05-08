#include "pch.h"
#include "Objects\HoloLensContext.h"

using namespace CubZHoloLensClient;

WinNetwork::TCPClient	^HoloLensContext::getTCPClient()
{
	return (_tcpClient);
}

void CubZHoloLensClient::HoloLensContext::login()
{
	_isLoggedIn = true;
	onLoggedIn();
}

void CubZHoloLensClient::HoloLensContext::logout()
{
	_isLoggedIn = false;
	onLoggedOut();
}

void CubZHoloLensClient::HoloLensContext::setUsername(Platform::String ^ username)
{
	_tcpClient->setUsername(username);
}