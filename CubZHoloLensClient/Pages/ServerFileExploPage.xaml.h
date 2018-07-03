//
// ServerFileExploPage.xaml.h
// Déclaration de la classe ServerFileExploPage
//

#pragma once

#include "Objects\File.h"
#include "Pages\ServerFileExploPage.g.h"

namespace CubZHoloLensClient
{

	//https://docs.microsoft.com/en-us/windows/uwp/data-binding/data-binding-quickstart
	//https://stackoverflow.com/questions/36106829/uwp-listview-different-item-template-for-first-and-last-item

	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ServerFileExploPage sealed
	{
	private:
		//Network events
		Windows::Foundation::EventRegistrationToken				_listFileToken;
		Windows::Foundation::TimeSpan							_timeSpanListFile;
		Windows::System::Threading::ThreadPoolTimer				^_threadPoolTimerListFiles;
		Platform::String										^path;

	public:
		ServerFileExploPage();

		void OnListFile(Windows::Foundation::Collections::IVector<WinNetwork::FileDescription>^fileList);

	private:
		void Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	private:
		void FileView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;
	private:
		void GoInsideButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void GoPrevFolder_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}