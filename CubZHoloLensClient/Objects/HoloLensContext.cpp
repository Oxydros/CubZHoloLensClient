#include "pch.h"
#include "Objects\HoloLensContext.h"

using namespace CubZHoloLensClient;

WinNetwork::TCPClient	^HoloLensContext::getTCPClient()
{
	return (_tcpClient);
}