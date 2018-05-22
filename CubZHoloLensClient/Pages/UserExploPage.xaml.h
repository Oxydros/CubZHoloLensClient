﻿//
// UserExploPage.xaml.h
// Declaration of the UserExploPage class
//

#pragma once

#include "Objects\User.h"
#include "Pages\UserExploPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UserExploPage sealed
	{
	private:
		//Network events
		Windows::Foundation::EventRegistrationToken				_listUserToken;
		Windows::Foundation::TimeSpan							_timeSpanListUser;
		Windows::System::Threading::ThreadPoolTimer				^_threadPoolTimerListUsers;
		//https://docs.microsoft.com/en-us/windows/uwp/data-binding/data-binding-quickstart
		//https://stackoverflow.com/questions/36106829/uwp-listview-different-item-template-for-first-and-last-item
		Windows::Foundation::Collections::IVector<User^>	^users;

	public:
		UserExploPage();

		void OnListUser(Windows::Foundation::Collections::IVector<Platform::String^>^userList);

	private:
		void Button_Back(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	private:
		void UserView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;
	};
}
