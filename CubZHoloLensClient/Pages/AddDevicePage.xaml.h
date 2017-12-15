//
// AddDevicePage.xaml.h
// Déclaration de la classe AddDevicePage
//

#pragma once

#include "pch.h"
#include "Pages\DeviceManagerPage.xaml.h"
#include "Pages\AddDevicePage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class AddDevicePage sealed
	{
	private:
		DeviceManagerPage	^_manager;

	public:
		AddDevicePage();

	private:
		void Button_Cancel(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Confirm(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;
	};
}
