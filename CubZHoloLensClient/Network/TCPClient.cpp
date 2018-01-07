#include "pch.h"
#include "TCPClient.h"

using namespace CubZHoloLensClient;

WinNetwork::TCPClient::TCPClient()
{

}

WinNetwork::TCPClient::~TCPClient()
{
	_thread->join();
	delete _thread;
}

void WinNetwork::TCPClient::connect(Platform::String ^ip, Platform::String ^port)
{
	_client.connect(platformStringToString(ip), platformStringToString(port));
}

void WinNetwork::TCPClient::run()
{
	_client.run();
}

void WinNetwork::TCPClient::runAsync()
{
	_thread = new boost::thread([&] {
		_client.run();
	});
}

std::string WinNetwork::TCPClient::platformStringToString(Platform::String^ ps) {
	int len = WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), 0, 0, NULL, NULL);
	std::string r("", len);
	char* pRBuf = const_cast<char*>(r.c_str());
	WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), pRBuf, len, NULL, NULL);
	return r;
}