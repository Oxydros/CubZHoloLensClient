//
// LocalFileExploPage.xaml.h
// Déclaration de la classe LocalFileLocalFileExploPage
//

#pragma once

#include "Objects\File.h"
#include "Pages\LocalFileExploPage.g.h"

namespace CubZHoloLensClient
{
	/// <summary>
	/// Une page vide peut être utilisée seule ou constituer une page de destination au sein d'un frame.
	/// </summary>
	//Insure it can't be seen in javascript
	//cf: https://docs.microsoft.com/en-us/cpp/cppcx/ref-classes-and-structs-c-cx
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LocalFileExploPage sealed
	{
	private:

		//https://docs.microsoft.com/en-us/windows/uwp/data-binding/data-binding-quickstart
		//https://stackoverflow.com/questions/36106829/uwp-listview-different-item-template-for-first-and-last-item
		Windows::Foundation::Collections::IVector<File^>	^_files;

	public:
		LocalFileExploPage();
	private:
		void Button_GoBack(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	public:
		property Windows::Foundation::Collections::IVector<File^> ^Files
		{
			Windows::Foundation::Collections::IVector<File^> ^get()
			{
				if (this->_files == nullptr)
				{
					this->_files = ref new Platform::Collections::Vector<File^>();
				}
				return this->_files;
			}
		};
	private:
		void FileView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
