#pragma once

#include <string>

namespace Utility {
	std::string platformStringToString(Platform::String^ ps);
	Platform::String ^ stringToPlatformString(const std::string & input);
	std::string	GetRealInstallPath();
}