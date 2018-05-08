//
// ConnectionPage.xaml.cpp
// Implémentation de la classe ConnectionPage
//

#include "pch.h"
#include "Utility\Utility.h"
#include "HomePage.xaml.h"
#include "Objects\HoloLensContext.h"
#include "ConnectionPage.xaml.h"

using namespace CubZHoloLensClient;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=234238

ConnectionPage::ConnectionPage()
{
	InitializeComponent();
	this->ProgressRing->IsActive = false;
	this->ErrorText->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void CubZHoloLensClient::ConnectionPage::OnLoggedIn()
{
	TRACE("LOGGED IN" << std::endl)
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(HomePage::typeid), this);
	}));
}


void CubZHoloLensClient::ConnectionPage::Button_Connect(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	connectingState();
	WinNetwork::TCPClient ^client = CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient();

	CubZHoloLensClient::HoloLensContext::Instance()->setUsername(this->UsernameInput->Text);
	CubZHoloLensClient::HoloLensContext::Instance()->onLoggedIn += ref new ConnectionEvent(this, &ConnectionPage::OnLoggedIn);

	try {
		client->connect(this->IPInput->Text, this->PortInput->Text);
		client->runAsync();
	}
	catch (std::exception &err)
	{
		waitingConnectionState(true);
	}
}

void CubZHoloLensClient::ConnectionPage::connectingState()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->ProgressRing->IsActive = true;
		this->ConnectButton->IsEnabled = false;
		this->ErrorText->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}));
}

void CubZHoloLensClient::ConnectionPage::waitingConnectionState(bool error)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, error]() {
		if (error)
			this->ErrorText->Visibility = Windows::UI::Xaml::Visibility::Visible;
		this->ProgressRing->IsActive = false;
		this->ConnectButton->IsEnabled = true;
	}));
}
