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
		void Button_Explo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
