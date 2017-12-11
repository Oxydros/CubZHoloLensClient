#pragma once

#include "pch.h"
#include "File.h"

namespace CubZHoloLensClient
{
	//https://docs.microsoft.com/en-us/cpp/cppcx/ref-classes-and-structs-c-cx
	//https://msdn.microsoft.com/en-us/library/es7h5kch.aspx
	public ref class FileDataTemplateSelector sealed : Windows::UI::Xaml::Controls::DataTemplateSelector
	{
	private:
		Windows::UI::Xaml::DataTemplate	^_fileTemplate;
		Windows::UI::Xaml::DataTemplate	^_dirTemplate;
		
	public:
		FileDataTemplateSelector(Windows::UI::Xaml::DataTemplate ^f, Windows::UI::Xaml::DataTemplate ^d)
			: _fileTemplate(f), _dirTemplate(d)
		{
		}

	public:
		property Windows::UI::Xaml::DataTemplate^ FileTemplate
		{
			void set(Windows::UI::Xaml::DataTemplate ^fileTemplate)
			{
				_fileTemplate = fileTemplate;
			}
		}

		property Windows::UI::Xaml::DataTemplate^ DirectoryTemplate
		{
			void set(Windows::UI::Xaml::DataTemplate ^dirTemplate)
			{
				_dirTemplate = dirTemplate;
			}
		}

	public:
		virtual Windows::UI::Xaml::DataTemplate ^SelectTemplateCore(Platform::Object ^item, Windows::UI::Xaml::DependencyObject ^container) override
		{
			File ^f = dynamic_cast<File^>(item);
			if (f->Type == FileType::DIRECTORY)
				return (_dirTemplate);
			return (_fileTemplate);
		}
	};
}