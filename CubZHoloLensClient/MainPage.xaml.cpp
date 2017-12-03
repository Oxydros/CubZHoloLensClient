﻿//
// MainPage.xaml.cpp
// Implémentation de la classe MainPage.
//

#include "pch.h"
#include "AppView.h"
#include "MainPage.xaml.h"

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

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

//Create a new view and launch the 3D App in it
//https://stackoverflow.com/questions/19917466/how-to-wait-for-an-iasyncaction
//http://blog.infernored.com/mixing-hololens-2d-and-3d-xaml-views-in-holographicspace
void CubZHoloLensClient::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	HoloLensClient::AppViewSource^ appViewSource = ref new HoloLensClient::AppViewSource();
	::CoreApplicationView^ newView = ::CoreApplication::CreateNewView(appViewSource);
	int newViewId = 0;

	::IAsyncAction ^a = newView->Dispatcher->RunAsync(::CoreDispatcherPriority::Normal, ref new DispatchedHandler(
		[this, newViewId]() mutable {
		ApplicationView ^newV = ::ApplicationView::GetForCurrentView();
		newViewId = newV->Id;
		CoreWindow ^thW = CoreWindow::GetForCurrentThread();
		thW->Activate();
		ApplicationViewSwitcher::TryShowAsStandaloneAsync(newViewId);
	}
	));
}
