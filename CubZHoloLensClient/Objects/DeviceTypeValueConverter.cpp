#include "pch.h"
#include "DeviceTypeValueConverter.h"

Platform::Object ^ CubZHoloLensClient::DeviceTypeValueConverter::Convert(Platform::Object ^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^ parameter, Platform::String ^ language)
{
	return (value->ToString());
}

Platform::Object ^ CubZHoloLensClient::DeviceTypeValueConverter::ConvertBack(Platform::Object ^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object ^ parameter, Platform::String ^ language)
{
	throw ref new Platform::NotImplementedException();
}
