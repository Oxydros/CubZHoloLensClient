#pragma once

#include <string>

namespace HoloLensClient
{
	std::string	GetRealInstallPath()
	{
		Windows::Storage::StorageFolder^ installedLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;

		auto path = installedLocation->Path;

		std::wstring fooW(path->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		return (fooA);
	}
}

namespace Utility {
	std::string platformStringToString(Platform::String^ ps);
	Platform::String ^ stringToPlatformString(const std::string & input);
}