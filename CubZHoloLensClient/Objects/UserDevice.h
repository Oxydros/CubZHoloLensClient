#pragma once

#include "pch.h"

namespace CubZHoloLensClient
{
	public enum class DeviceType
	{
		UNKNOWN = -1,
		PHONE = 0,
		COMPUTER = 1
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class UserDevice sealed
	{
	private:
		DeviceType			_type;
		Platform::String	^_name;
	public:
		UserDevice(Platform::String ^name, DeviceType type);

		property Platform::String ^Name
		{
			Platform::String ^get();
		}

		property DeviceType Type
		{
			DeviceType get();
		}

		property Platform::String ^TypeName
		{
			Platform::String ^get();
		}
	};
}