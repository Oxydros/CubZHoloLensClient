//
// HomePage.xaml.cpp
// Implémentation de la classe HomePage.
//

#include "pch.h"
#include <Utility\Utility.h>
#include "Pages\LocalFileExploPage.xaml.h"
#include "Pages\ServerFileExploPage.xaml.h"
#include "Pages\DeviceManagerPage.xaml.h"
#include "Pages\HomePage.xaml.h"
#include "Pages\UserExploPage.xaml.h"
#include "3D\AppView.h"

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
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Core;
using namespace CubZHoloLensClient;

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

HomePage::HomePage()
{
	InitializeComponent();
}

//Create a new view and launch the 3D App in it
//https://stackoverflow.com/questions/19917466/how-to-wait-for-an-iasyncaction
//http://blog.infernored.com/mixing-hololens-2d-and-3d-xaml-views-in-holographicspace
// TODO: Look on the thread problems
void CubZHoloLensClient::HomePage::Button_Launch3D(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	int xamlViewId = ::ApplicationView::GetForCurrentView()->Id;

	TRACE("XAML View id " << xamlViewId << std::endl);

	HoloLensClient::AppViewSource^ appViewSource = ref new HoloLensClient::AppViewSource();
	::CoreApplicationView^ newView = ::CoreApplication::CreateNewView(appViewSource);

	TRACE("Dispatching 3D view thread" << std::endl);
	::IAsyncAction ^a = newView->Dispatcher->RunAsync(::CoreDispatcherPriority::Normal, ref new DispatchedHandler(
		[]() {
		try
		{
			ApplicationView ^newV = ::ApplicationView::GetForCurrentView();
			int spaceViewId = newV->Id;

			TRACE("3D View id " << spaceViewId << std::endl);
			CoreWindow ^thW = CoreWindow::GetForCurrentThread();

			thW->Activate();
			auto task = concurrency::create_task(ApplicationViewSwitcher::TryShowAsStandaloneAsync(spaceViewId))
				.then([](bool result) {
			});
		}
		catch (std::exception &e)
		{
			TRACE("EXCEPTION IN LAUNCHING 3D " << e.what() << std::endl);
		}
	}
	));

	//auto task = concurrency::create_task(a);

	//task.then([spaceViewId]() {
	//	TRACE("GOT HERE " << ::CoreApplication::Views->Size << std::endl);
	//	CoreApplicationView ^view = ::CoreApplication::Views->GetAt(0);
	//	int id = ApplicationView::GetApplicationViewIdForWindow(view->CoreWindow);
	//	TRACE("Got id " << id << " need " << spaceViewId << std::endl);
	//	ApplicationViewSwitcher::TryShowAsStandaloneAsync(spaceViewId);
	//});
}


void CubZHoloLensClient::HomePage::Button_LocalFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	/*this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(LocalFileExploPage::typeid));
	}));*/

	Windows::Storage::Pickers::FileOpenPicker^ openPicker = ref new Windows::Storage::Pickers::FileOpenPicker();
	openPicker->ViewMode = Windows::Storage::Pickers::PickerViewMode::List;
	openPicker->SuggestedStartLocation = Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
	openPicker->FileTypeFilter->Append("*");

	concurrency::create_task(openPicker->PickMultipleFilesAsync()).then([this](IVectorView<Windows::Storage::StorageFile^>^ files)
	{
		if (files->Size > 0)
		{
			String^ output = "Picked files:\n";
			std::for_each(begin(files), end(files), [this, &output](Windows::Storage::StorageFile ^file)
			{
				output += file->Name + "\n";
			});
			TRACE("Got " << Utility::platformStringToString(output) << std::endl);
		}
		else
		{
			TRACE("Operation canceled" << std::endl);
		}
	});
}

void CubZHoloLensClient::HomePage::Button_ServerFileExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(ServerFileExploPage::typeid));
	}));
}

void CubZHoloLensClient::HomePage::Button_DeviceExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(DeviceManagerPage::typeid));
	}));
}

void CubZHoloLensClient::HomePage::Button_UserExplo(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->Navigate(Windows::UI::Xaml::Interop::TypeName(UserExploPage::typeid));
	}));
}

void CubZHoloLensClient::HomePage::Button_TestNetwork(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
}