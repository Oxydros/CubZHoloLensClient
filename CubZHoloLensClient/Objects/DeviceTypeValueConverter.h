#pragma once
#include "pch.h"
#include "Objects\UserDevice.h"

namespace CubZHoloLensClient
{
	public ref class DeviceTypeValueConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		virtual Platform::Object ^Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType,
			Platform::Object^ parameter, Platform::String^ language);

		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType,
			Platform::Object^ parameter, Platform::String^ language);
	};
}