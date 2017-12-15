//
// AddDevicePage.xaml.cpp
// Implémentation de la classe AddDevicePage
//

#include "pch.h"
#include "AddDevicePage.xaml.h"

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

AddDevicePage::AddDevicePage()
{
	InitializeComponent();
}

void AddDevicePage::Button_Cancel(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->GoBack();
	}));
}

void AddDevicePage::Button_Confirm(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_manager->Devices->Append(ref new UserDevice(this->DeviceNameInput->Text, DeviceType::PHONE));

	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->GoBack();
	}));
}

void AddDevicePage::OnNavigatedTo(NavigationEventArgs ^e)
{
	_manager = dynamic_cast<DeviceManagerPage^>(e->Parameter);
}
