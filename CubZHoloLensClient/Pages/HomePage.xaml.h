//
// HomePage.xaml.h
// Déclaration de la classe HomePage.
//

#pragma once

#include "HomePage.g.h"

class Observer : public Network::PacketObserver
{
	virtual void handlePacket(Network::IConnection::SharedPtr from, Network::IPacket const &packet)
	{
		auto tcpPacket = static_cast<Network::TCPPacket const &>(packet);

		std::cout << "Received " << tcpPacket << std::endl;
	}
};

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	public ref class HomePage sealed
	{
	public:
		HomePage();

	private:
		void Button_Launch3D(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_LocalFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_ServerFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_DeviceExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_TestNetwork(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_UserExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
