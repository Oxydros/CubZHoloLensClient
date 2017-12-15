//
// DeviceManagerPage.xaml.cpp
// Implémentation de la classe DeviceManagerPage
//

#include "pch.h"
#include "Pages\AddDevicePage.xaml.h"
#include "DeviceManagerPage.xaml.h"

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

DeviceManagerPage::DeviceManagerPage()
{
	this->_devices = ref new Platform::Collections::Vector<UserDevice^>();
	InitializeComponent();

	this->_devices->Append(ref new UserDevice("Computer of Louis", DeviceType::COMPUTER));
	this->_devices->Append(ref new UserDevice("Louis' OnePlus5", DeviceType::PHONE));

	this->DeviceView->ItemsSource = this->_devices;
}

void DeviceManagerPage::Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Frame->GoBack();
}


void DeviceManagerPage::Button_AddDevice(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(AddDevicePage::typeid), this);
	}));
}
