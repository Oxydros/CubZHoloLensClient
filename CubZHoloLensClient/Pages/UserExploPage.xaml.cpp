//
// UserExploPage.xaml.cpp
// Implementation of the UserExploPage class
//

#include "pch.h"
#include "UserDataTemplateSelector.h"
#include "UserExploPage.xaml.h"

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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

UserExploPage::UserExploPage()
{
	this->users = ref new Platform::Collections::Vector<User^>();

	InitializeComponent();

	this->users->Append(ref new User("Louis", UserRank::ADMIN));
	this->users->Append(ref new User("Eric", UserRank::GUEST));

	this->UserView->ItemsSource = this->users;

	Windows::UI::Xaml::DataTemplate ^adminTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("AdminTemplate"));
	Windows::UI::Xaml::DataTemplate ^guestTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("GuestTemplate"));

	this->UserView->ItemTemplateSelector = ref new UserDataTemplateSelector(adminTemplate, guestTemplate);
}


void CubZHoloLensClient::UserExploPage::Button_Back(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->GoBack();
	}));
}

void CubZHoloLensClient::UserExploPage::UserView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
}