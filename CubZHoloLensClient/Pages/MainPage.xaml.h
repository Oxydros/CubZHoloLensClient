//
// MainPage.xaml.h
// Déclaration de la classe MainPage.
//

#pragma once

#include "MainPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void Button_Launch3D(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_LocalFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_ServerFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_DeviceExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	};
}
