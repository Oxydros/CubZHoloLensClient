#pragma once

#include "pch.h"

namespace CubZHoloLensClient
{
	public enum class FileType
	{
		UNKNOWN = -1,
		FILE = 0,
		DIRECTORY = 1
	};

	//[WFM::WebHostHidden]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class File sealed
	{
	private:
		Platform::String	^_name;
		FileType			_type;

	public:
		File(Platform::String ^name, FileType type);

		property Platform::String ^Name
		{
			Platform::String ^get();
		}

		property FileType Type
		{
			FileType get();
		}
	};

}