//
// DeviceManagerPage.xaml.h
// Déclaration de la classe DeviceManagerPage
//

#pragma once

#include "Objects\UserDevice.h"
#include "Pages\DeviceManagerPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class DeviceManagerPage sealed
	{
	private:
		Windows::Foundation::Collections::IVector<UserDevice^>	^_devices;
	public:
		DeviceManagerPage();

	private:
		void Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_AddDevice(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	public:
		property Windows::Foundation::Collections::IVector<UserDevice^> ^Devices
		{
			Windows::Foundation::Collections::IVector<UserDevice^> ^get()
			{
				if (this->_devices == nullptr)
				{
					this->_devices = ref new Platform::Collections::Vector<UserDevice^>();
				}
				return this->_devices;
			}
		};
	};
}
