//
// LocalFileExploPage.xaml.cpp
// Implémentation de la classe LocalFileExploPage
//

#include "pch.h"
#include "Objects\File.h"
#include "Objects\FileDataTemplateSelector.h"
#include "Pages\LocalFileExploPage.xaml.h"

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
//https://docs.microsoft.com/en-us/windows/uwp/design/controls-and-patterns/resourcedictionary-and-xaml-resource-references
LocalFileExploPage::LocalFileExploPage()
{
	this->_files = ref new Platform::Collections::Vector<File^>();

	InitializeComponent();

	this->_files->Append(ref new File("TestFile", FileType::FILE));
	this->_files->Append(ref new File("TestDirectory", FileType::DIRECTORY));

	this->FileView->ItemsSource = this->_files;
	Windows::UI::Xaml::DataTemplate ^dTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("DirectoryTemplate"));
	Windows::UI::Xaml::DataTemplate ^fTemplate = static_cast<DataTemplate^>(this->Resources->Lookup("FileTemplate"));
	
	this->FileView->ItemTemplateSelector = ref new FileDataTemplateSelector(fTemplate, dTemplate);
}


void CubZHoloLensClient::LocalFileExploPage::Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
		this->Frame->GoBack();
	}));
}


void CubZHoloLensClient::LocalFileExploPage::FileView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
}
