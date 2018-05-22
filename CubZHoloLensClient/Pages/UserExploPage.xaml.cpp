//
// UserExploPage.xaml.cpp
// Implementation of the UserExploPage class
//

#include "pch.h"
#include <Objects\HoloLensContext.h>
#include "Objects\UserDataTemplateSelector.h"
#include "Pages\UserExploPage.xaml.h"

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

	InitializeComponent();


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

void CubZHoloLensClient::UserExploPage::OnListUser(Windows::Foundation::Collections::IVector<Platform::String^>^ userList)
{
	auto users = ref new Platform::Collections::Vector<User^>();
	/*TRACE("Got files:" << std::endl);*/
	for (Platform::String ^userName : userList)
	{
		users->Append(ref new User(userName, UserRank::GUEST));
		/*TRACE("Name: " << Utility::platformStringToString(fileName) << std::endl);*/
	}

	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, users]() {
		this->UserView->ItemsSource = users;
	}));
}

void CubZHoloLensClient::UserExploPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^ e)
{
	//Register to receive info on fileList
	_listUserToken = CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->ListUserEvent += ref new CubZHoloLensClient::WinNetwork::UserListEvent(this,
		&CubZHoloLensClient::UserExploPage::OnListUser);

	//Update file scene every 3 seconds
	_timeSpanListUser.Duration = 50000000;

	_threadPoolTimerListUsers = Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(
		ref new Windows::System::Threading::TimerElapsedHandler([](Windows::System::Threading::ThreadPoolTimer^ source)
	{
		CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->listServerUsers();
	}), _timeSpanListUser);


	TRACE("Navigated to ServerFileExplo" << std::endl);
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->listServerUsers();
}