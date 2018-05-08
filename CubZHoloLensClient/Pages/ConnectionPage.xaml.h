//
// ConnectionPage.xaml.h
// Déclaration de la classe ConnectionPage
//

#pragma once

#include "ConnectionPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ConnectionPage sealed
	{
	private:
		Windows::Foundation::EventRegistrationToken	_onLoggedInToken{};

	public:
		ConnectionPage();

	public:
		void OnLoggedIn();

	private:
		void Button_Connect(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void connectingState();
		void waitingConnectionState(bool error = false);
	};
}
