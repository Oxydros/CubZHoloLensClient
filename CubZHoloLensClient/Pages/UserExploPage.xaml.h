//
// UserExploPage.xaml.h
// Declaration of the UserExploPage class
//

#pragma once

#include "Pages\UserExploPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UserExploPage sealed
	{
	public:
		UserExploPage();
	private:
		void Button_Back(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
