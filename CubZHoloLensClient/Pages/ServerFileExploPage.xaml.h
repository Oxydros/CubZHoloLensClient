//
// ServerFileExploPage.xaml.h
// Déclaration de la classe ServerFileExploPage
//

#pragma once

#include "File.h"
#include "Pages\ServerFileExploPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ServerFileExploPage sealed
	{
	private:

		//https://docs.microsoft.com/en-us/windows/uwp/data-binding/data-binding-quickstart
		//https://stackoverflow.com/questions/36106829/uwp-listview-different-item-template-for-first-and-last-item
		Windows::Foundation::Collections::IVector<File^>	^files;

		Windows::Foundation::EventRegistrationToken			_listFileToken;

	public:
		ServerFileExploPage();

		void OnListFile(Windows::Foundation::Collections::IVector<Platform::String^>^fileList);

	private:
		void Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	public:
		property Windows::Foundation::Collections::IVector<File^> ^Files
		{
			Windows::Foundation::Collections::IVector<File^> ^get()
			{
				if (this->files == nullptr)
				{
					this->files = ref new Platform::Collections::Vector<File^>();
				}
				return this->files;
			}
		};

	private:
		void FileView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;
	};
}
