#include <pch.h>
#define generic test
#include <boost\filesystem.hpp>
#undef generic
#include <Utility\Utility.h>

std::string Utility::platformStringToString(Platform::String^ ps) {
	int len = WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), 0, 0, NULL, NULL);
	std::string r("", len);
	char* pRBuf = const_cast<char*>(r.c_str());
	WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), pRBuf, len, NULL, NULL);
	return r;
}
Platform::String ^ Utility::stringToPlatformString(const std::string & input)
{
	std::wstring w_str = std::wstring(input.begin(), input.end());
	const wchar_t* w_chars = w_str.c_str();

	return (ref new Platform::String(w_chars));
}

std::string	Utility::GetRealInstallPath()
{
	Windows::Storage::StorageFolder^ installedLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;

	auto path = installedLocation->Path;

	std::wstring fooW(path->Begin());
	std::string fooA(fooW.begin(), fooW.end());
	return (fooA);
}

std::string Utility::GetDirectory(std::string const & path)
{
	boost::filesystem::path p(path);
	boost::filesystem::path dir = p.parent_path();
	return dir.string() + "\\";
}
