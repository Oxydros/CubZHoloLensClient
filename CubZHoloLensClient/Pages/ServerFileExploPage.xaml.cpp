﻿//
// ServerFileExploPage.xaml.cpp
// Implémentation de la classe ServerFileExploPage
//

#include "pch.h"
#include <Objects\HoloLensContext.h>
#include "Objects\FileDataTemplateSelector.h"
#include "Pages\ServerFileExploPage.xaml.h"

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

// Pour plus d'informations sur le modèle d'élément Page vierge, consultez la page https://go.microsoft.com/fwlink/?LinkId=234238

ServerFileExploPage::ServerFileExploPage()
{
	InitializeComponent();

	Windows::UI::Xaml::DataTemplate ^dTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("DirectoryTemplate"));
	Windows::UI::Xaml::DataTemplate ^fTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("FileTemplate"));

	this->FileView->ItemTemplateSelector = ref new FileDataTemplateSelector(fTemplate, dTemplate);
	this->path = ".";
}

void CubZHoloLensClient::ServerFileExploPage::OnListFile(Windows::Foundation::Collections::IVector<WinNetwork::FileDescription>^ fileList)
{
	auto files = ref new Platform::Collections::Vector<File^>();
	for (WinNetwork::FileDescription file : fileList)
	{
		if (file.type == CubZHoloLensClient::WinNetwork::FileType::DIRECTORY) {
			files->Append(ref new File(file.name, FileType::DIRECTORY));
		}
		else if (file.type == CubZHoloLensClient::WinNetwork::FileType::FILE) {
			files->Append(ref new File(file.name, FileType::FILE));
		}
	}

	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, files]() {
		this->FileView->ItemsSource = files;
	}));
}


void CubZHoloLensClient::ServerFileExploPage::Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Unregister for event
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->ListFileEvent -= _listFileToken;
	_threadPoolTimerListFiles->Cancel();
	delete _threadPoolTimerListFiles;

	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->GoBack();
	}));
}

void CubZHoloLensClient::ServerFileExploPage::FileView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
}

void CubZHoloLensClient::ServerFileExploPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^ e)
{
	//Register to receive info on fileList
	_listFileToken = CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->ListFileEvent += ref new CubZHoloLensClient::WinNetwork::FileListEvent(this,
		&CubZHoloLensClient::ServerFileExploPage::OnListFile);

	//Update file scene every 5 seconds
	_timeSpanListFile.Duration = 50000000;

	_threadPoolTimerListFiles = Windows::System::Threading::ThreadPoolTimer::CreatePeriodicTimer(
		ref new Windows::System::Threading::TimerElapsedHandler([&, this](Windows::System::Threading::ThreadPoolTimer^ source)
	{
		CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->listServerFiles(this->path);
	}), _timeSpanListFile);


	TRACE("Navigated to ServerFileExplo" << std::endl);
	CubZHoloLensClient::HoloLensContext::Instance()->getTCPClient()->listServerFiles(this->path);
}


void CubZHoloLensClient::ServerFileExploPage::GoInsideButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->FileType->Text != "File") {
		this->path = this->path + "/" + this->FileName->Text;
	}
}


void CubZHoloLensClient::ServerFileExploPage::GoPrevFolder_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->path != ".") {
		std::string newpath;
		newpath = Utility::platformStringToString(this->path);
		std::vector<std::string> array;
		std::stringstream ss(newpath);
		std::string tmp;
		while (std::getline(ss, tmp, '/'))
		{
			array.push_back(tmp);
		}
		newpath = "";
		array.pop_back();
		for (auto it = array.begin(); it != array.end(); ++it)
		{
			if (it != array.end()) {
				newpath = newpath + (*it) + "/";
			}
		}
		newpath = newpath.substr(0, newpath.size() - 1);
		this->path = Utility::stringToPlatformString(newpath);
	}
}