#include "pch.h"

// The main function is only used to initialize our IFrameworkView class.
// Under most circumstances, you should not need to modify this function.
[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	//Launch the XAML App
	::Windows::UI::Xaml::Application::Start(
		ref new ::Windows::UI::Xaml::ApplicationInitializationCallback(
			[](::Windows::UI::Xaml::ApplicationInitializationCallbackParams^ p) {
		(void)p; // Unused parameter
		auto app = ref new CubZHoloLensClient::App();
	}));
	/*AppViewSource^ appViewSource = ref new ::AppViewSource();
	CoreApplication::Run(appViewSource);*/
	return 0;
}