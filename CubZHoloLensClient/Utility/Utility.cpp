#include <pch.h>
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